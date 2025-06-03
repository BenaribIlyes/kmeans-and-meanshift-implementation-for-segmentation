#include "ocv_utils.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using namespace cv;
using namespace std;

/* Function  */

// Function to invert a grayscale image
void invertImage(Mat& image)
{
    // Use OpenCV's arithmetic operation to invert the image
    image = 255 - image;
}

// Print teh arguments of the program
void printHelpKmeans(const string& progName)
{
cout << "Usage:\n\t " << progName << " <image_file> <K_num_of_clusters> [<image_ground_truth>] <" << endl;
}

//Menu to invert the image if it doesn't fit the colours of the reference segumented image 
void Menu_To_Invert_Image(Mat& image, const string& window_name)
{   
    std::cout << "Tap Enter to continue" << std::endl;

    // Invert the segmented image if needed
    char invert_kmeans;
    std::cout << "Do you want to invert the image (" << window_name << ")? (y/n): ";
    waitKey(0);

    std::cin >> invert_kmeans;
    
    cv::destroyWindow(window_name);
    if (invert_kmeans == 'y' || invert_kmeans == 'Y') 
    {
        invertImage(image);
        cv::imshow(window_name + " (Inverted)", image);
        cv::moveWindow(window_name + " (Inverted)", 700, 0);
    }
    else 
    {
        cv::imshow(window_name, image);
        cv::moveWindow(window_name, 700, 0);
    }
}


// Implémentation personnalisée de l'algorithme k-means
void kmeans_perso(const Mat& data, int K, Mat& labels_perso, TermCriteria criteria, Mat& centers_perso)
{
   
/*
  data: image d'entrée
  K: nombre de clusters
  labels_perso: matrice d'étiquettes des clusters pour chaque point
  criteria: critère d'arrêt (nombre max d'itérations ou seuil de convergence)
  centers_perso: matrice contenant les centres des clusters
*/
 
    // Nombre de points et dimensions des données
    int N = data.rows;  // Nombre de lignes de pixels de l'image 
    int M = data.cols; // Nombre de colonnes de pixels de l'image

    // Initialisation des structures de données
    labels_perso.create(N, 1, CV_32S);        // Initialise une matrice de N x 1 avec des entiers (CV_32S), qui stockera l'étiquette du cluster de chaque point.
    centers_perso.create(K, M, data.type());   //  Initialise une matrice de K x M, où chaque ligne représente un centre de cluster.


    // Initialisation aléatoire des centres
    randu(centers_perso, Scalar::all(0), Scalar::all(255)); // Remplit les centres avec des valeurs aléatoires entre 0 et 255


    //Mat oldLabels, dists;

    // Boucle principale de k-means, s'arrête après criteria.maxCount itérations
    for (int iter = 0; iter < criteria.maxCount; ++iter) {

        // Étape 1 : Attribution des étiquettes aux points en fonction du centre le plus proche
        for (int i = 0; i < N; ++i) {
            double minDist = DBL_MAX; // Distance minimale initialisée à l'infini
            int label_cluster = 0;  // Étiquette du cluster

            // Recherche du centre le plus proche
            for (int j = 0; j < K; ++j) {
                double dist = norm(data.row(i), centers_perso.row(j), NORM_L2); // Distance euclidienne
                if (dist < minDist) {
                    minDist = dist;
                    label_cluster = j;
                }
            }
            labels_perso.at<int>(i) = label_cluster; // Affectation de l'étiquette
        }

        // Étape 2 : On recalcule les centres des clusters
        Mat newCenters = Mat::zeros(K, M, data.type()); // Stocke les nouveaux centres
        Mat counts = Mat::zeros(K, 1, CV_32S); // Compte le nombre de points par cluster

        for (int i = 0; i < N; ++i) {
            int label = labels_perso.at<int>(i);
            newCenters.row(label) += data.row(i); // Ajout du point au cluster correspondant
            counts.at<int>(label)++; // Incrémentation du compteur
        }

        // Moyennage des centres (on évite la division par 0)
        for (int j = 0; j < K; ++j) {
            if (counts.at<int>(j) > 0) {
                newCenters.row(j) /= counts.at<int>(j); // Moyenne des points du cluster
            }
        }
        
        // Étape 3 : Vérification de la convergence (distance entre anciens et nouveaux centres)
        if (norm(newCenters - centers_perso, NORM_L2) < criteria.epsilon) {
            break; // Arrêt si les centres ne bougent presque plus
        }

        // Mise à jour des centres
        newCenters.copyTo(centers_perso);
    }
}

 
void evaluateSegmentation(const Mat& estimated, const Mat& reference) {
    if (estimated.size() != reference.size()) {
        cerr << "Les dimensions des images ne correspondent pas !" << endl;
        return;
    }

    int TP = 0, FP = 0, FN = 0, TN = 0;

    for (int i = 0; i < estimated.rows; i++) 
    {
        for (int j = 0; j < estimated.cols; j++) 
        {
            uchar est_pixel = estimated.at<uchar>(i, j);
            uchar ref_pixel = reference.at<uchar>(i, j);

            if (est_pixel == 255 && ref_pixel == 255) // Vrai positif  
            {
                TP=TP+1;
            } 
            else if (est_pixel == 255&& ref_pixel == 0) // Faux positif
            {
                FP=FP+1; 
            }   
            else if (est_pixel == 0 && ref_pixel == 255) // Faux négatif
            {
                FN=FN+1; 
            } 
            else if (est_pixel == 0 && ref_pixel == 0)// Vrai négatif
            {
                TN=TN+1; 
            } 
        }
    }

    double precision = TP / (double)(TP + FP);
    double recall = TP / (double)(TP + FN);
    double dice = (2.0 * TP) / (2 * TP + FP + FN);

    cout << "Précision (P) : " << precision << endl;
    cout << "Sensibilité (S) : " << recall << endl;
    cout << "Dice Similarity Coefficient (DSC) : " << dice << endl;
}
