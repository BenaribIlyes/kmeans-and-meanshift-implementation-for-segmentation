#include "ocv_utils.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

#include <cstdlib>  // Pour `atof()`

using namespace cv;
using namespace std;

void printHelpMeansift(const string& progName)
{
    cout << "Usage:\n\t " << progName << " <image_file> [<image_ground_truth>]" << endl;
}


Vec3b mean(const Mat& image, int x, int y, float hs,float hc)
{
    int x_debut = max(0, static_cast<int>(x - hs / 2));
    int y_debut = max(0, static_cast<int>(y - hs / 2));
    int x_fin = min(image.rows - 1, static_cast<int>(x + hs / 2));
int y_fin = min(image.cols - 1, static_cast<int>(y + hs / 2));

    Vec3d resultat(0, 0, 0); // Utilisation de Vec3d pour éviter les débordements
    int count = 0;

    for (int l = x_debut; l <= x_fin; l++)
        for (int m = y_debut; m <= y_fin; m++)
        {
            if (norm(Vec3d(image.at<Vec3b>(l, m)) - Vec3d(image.at<Vec3b>(x, y)), NORM_L2) < hc)
            {
                Vec3b pixel = image.at<Vec3b>(l, m);
                resultat[0] += pixel[0];
                resultat[1] += pixel[1];
                resultat[2] += pixel[2];
                count++;
            }
        }

    return (count > 0) ? Vec3b(resultat[0] / count, resultat[1] / count, resultat[2] / count) : image.at<Vec3b>(x, y);
}


void meanshift(Mat& image, float hs, float hc, float epsilon, int kmax)
{
    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            Vec3b moy = mean(image, i, j, hs, hc);
            int k = 0;

            while ((norm(image.at<Vec3b>(i, j) - moy, NORM_L2) > epsilon) && (k < kmax))
            {
                image.at<Vec3b>(i, j) = moy;  // Appliquer la nouvelle couleur
                moy = mean(image, i, j, hs, hc);  // Recalcul de la moyenne
                k++;
            }
        }
    }
}

//Fonction utile pour comparer notre segmentation meanshift avec la segmentation binarisé
// Entrée : Image segmenté
// Sortie : Image binarisé
Mat binarise(Mat& mat)
{
    Vec3d moy_binaire(0, 0, 0); 
   
   // Parcourt de l'image pour calculer les intenistés moyennes
    int count = 0;
    for (int i = 0; i < mat.rows; i++)
    {
        for (int j = 0; j < mat.cols; j++)
        {
           
            Vec3b pixel = mat.at<Vec3b>(i, j);
                moy_binaire[0] += pixel[0];
                moy_binaire[1] += pixel[1];
                moy_binaire[2] += pixel[2];
                count++;
        }
    }

    moy_binaire /= count; // Valeur moyenne des inténsités de l'image (RGB)
    double moy = (moy_binaire[0] + moy_binaire[1] + moy_binaire[2])/3; // Valeur moyenne des inténsités de l'image R+G+B
    Mat copy = mat.clone();

        for (int i = 0; i < mat.rows; i++)
    {
        for (int j = 0; j < mat.cols; j++)
        {
            Vec3d point = mat.at<Vec3b>(i, j);
            double intensity = (point[0] + point[1] + point[2])/3; 
            if ( intensity < moy)
            
                copy.at<Vec3b>(i,j) = Vec3b(0,0,0);
            
            else 
            
                copy.at<Vec3b>(i,j) = Vec3b(255,255,255);
            
        }
    }
    return (copy);
}

/*
void invertImage(Mat& image)
{
    // Use OpenCV's arithmetic operation to invert the image
    image = 255 - image;
}
*/

/*
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
*/