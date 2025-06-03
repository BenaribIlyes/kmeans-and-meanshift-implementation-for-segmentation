#include "ocv_utils.hpp"
#include "kmeans_functions.hpp"
#include "meanshift_functions.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <cstdlib>  // Pour `atof()`

using namespace cv;
using namespace std;

/*
// Fonction pour gérer l'inversion d'une image segmentée
void Menu_To_Invert_Image(Mat& image, const string& window_name)
{
    char invert_choice;
    cout << "Do you want to invert the image (" << window_name << ")? (y/n): ";
    cin >> invert_choice;

    if (invert_choice == 'y' || invert_choice == 'Y') 
    {
        invertImage(image);
        destroyWindow(window_name);
        imshow(window_name + " (Inverted)", image);
        moveWindow(window_name + " (Inverted)", 700, 0);
    }
    else 
    {
        imshow(window_name, image);
        moveWindow(window_name, 700, 0);
    }
}
*/

int main(int argc, char** argv) 
{
    if (argc < 7 || argc > 9) {
        cerr << "Usage: " << argv[0] << " <image_file> <hs> <hc> <epsilon> <kmax> <output_kmeans> <output_meanshift> [ground_truth]" << endl;
        return EXIT_FAILURE;
    }

    // Chargement des paramètres
    const string imageFilename = argv[1];
    const int hs = stoi(argv[2]);
    const int hc = stoi(argv[3]);
    const float eps = atof(argv[4]);
    const int kmax = stoi(argv[5]);

    // Gestion des fichiers de sortie
    const string outputKMeansFilename = (argc >= 7) ? string(argv[6]) : "segmented_kmeans.jpg";
    const string outputMeanShiftFilename = (argc >= 8) ? string(argv[7]) : "segmented_meanshift.jpg";
    const string groundTruthFilename = (argc == 9) ? string(argv[8]) : "Image reference";

    // Chargement des images
    Mat image = imread(imageFilename, IMREAD_COLOR);
    if (image.empty()) 
    {
        cerr << "Error: Could not load the image!" << endl;
        return EXIT_FAILURE;
    }
    // Vérifier si une image de référence est fournie
    Mat reference_image;
    if (groundTruthFilename != "Image reference") 
    {
        reference_image = imread(groundTruthFilename, IMREAD_GRAYSCALE);
        if (reference_image.empty()) {
        cerr << "Error: Could not load ground truth image!" << endl;
    }
    }


    // Debugging info
    cout << "Processing Image: " << imageFilename << endl;
    cout << "Using hs: " << hs << ", hc: " << hc << ", epsilon: " << eps << ", kmax: " << kmax << endl;
    cout << "Output KMeans: " << outputKMeansFilename << endl;
    cout << "Output MeanShift: " << outputMeanShiftFilename << endl;

    // Copie de l'image pour les deux méthodes
    Mat clone_kmeans = image.clone();
    Mat clone_meanshift = image.clone();

    // Appliquer KMeans personnalisé
    cout << "Applying kmeans_perso..." << endl;
    Mat image_float;
    image.convertTo(image_float, CV_32F);
    Mat image_1D = image_float.reshape(1, image_float.rows * image_float.cols);

    Mat labels_perso, centers_perso;
    TermCriteria criteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 100, 0.2);
    //kmeans(image_1D, 2, labels_perso, criteria, 2, KMEANS_RANDOM_CENTERS, centers_perso);
    kmeans_perso(image_1D, 2, labels_perso, criteria, centers_perso);
    

    labels_perso = labels_perso * 255 / 1; // Normalisation (K=2)
    Mat segmented_kmeans = labels_perso.reshape(1, image.rows);
    segmented_kmeans.convertTo(segmented_kmeans, CV_8U);

    // Appliquer MeanShift
    cout << "Applying meanshift..." << endl;
    meanshift(clone_meanshift, hs, hc, eps, kmax);

    // Binarisation des résultats
    Mat binarised_meanshift = binarise(clone_meanshift);

    // Affichage des résultats
    imshow("Original Image", image);
    imshow("Reference Image for segumentation", reference_image);
    imshow("KMeans Segmented", segmented_kmeans);
    imshow("MeanShift Segmented", clone_meanshift);
    imshow("Binarised MeanShift", binarised_meanshift);
    cout<<"Press any key to continue..."<<endl;
    waitKey(0);
    

    // Gérer l'inversion des images par rapport à l'image de référence
    if (!reference_image.empty()) {
        Menu_To_Invert_Image(segmented_kmeans, "KMeans Segmented");
        Menu_To_Invert_Image(binarised_meanshift, "Binarised MeanShift");
    }

    // Sauvegarde des résultats
    imwrite(outputKMeansFilename, segmented_kmeans);
    imwrite(outputMeanShiftFilename, binarised_meanshift);

    // Évaluation si l'image de référence est fournie
    if (!reference_image.empty()) {
        cout << "\nEvaluation for KMeans:" << endl;
        evaluateSegmentation(segmented_kmeans, reference_image);
        cout << "\nEvaluation for MeanShift:" << endl;
        evaluateSegmentation(binarised_meanshift, reference_image);
    }

    cout << "\nTap Enter to exit." << endl;
    waitKey(0);
    return EXIT_SUCCESS;
}
