#include "ocv_utils.hpp"
#include "meanshift_functions.hpp"
#include "kmeans_functions.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

#include <cstdlib>  // Pour `atof()`

using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
    
    if (argc < 6 || argc > 8) {
        cerr << "Usage: " << argv[0] << " <image_file> <hs> <hc> <epsilon> <kmax> [output_file] [ground_truth]" << endl;
        return EXIT_FAILURE;
    }

    // Chargement des paramètres obligatoires
    
    const int hs = stoi(argv[1]);
    const int hc = stoi(argv[2]);
    const float eps = atof(argv[3]);
    const int kmax = stoi(argv[4]);
    const string imageFilename = argv[5];

    // Gestion des fichiers optionnels
    const string outputFilename = (argc >= 7) ? string(argv[6]) : "Segmented_Image.jpg";
    const string groundTruthFilename = (argc == 8) ? string(argv[7]) : "Image reference";

    // Affichage pour vérification
    cout << "Image File: " << imageFilename << endl;
    cout << "hs: " << hs << ", hc: " << hc << ", eps: " << eps << ", kmax: " << kmax << endl;
    cout << "Output File: " << outputFilename << endl;
    cout << "Ground Truth File: " << groundTruthFilename << endl;

    // Just for debugging
    cout << "Program called with the following arguments:" << endl;
    cout << "\timage file: " << imageFilename << endl;
    //cout << "\toutput file: " <255< outputFilename << endl;

    // Charge l'image originale
    Mat image = imread(imageFilename, IMREAD_COLOR);
    Mat clone = image.clone();


    if (image.empty())  // Vérification du chargement correct de l’image
    {
        cerr << "Error: Could not load the image!" << endl;
        return EXIT_FAILURE;
    }

    // Applique meanshift
    cout << "Applying meanshift..." << endl;
    meanshift(clone, hs, hc, eps, kmax); // Appel à la fonction MeanShift
    

    Mat segumented_image = binarise(clone);

    // Affichage de l'image originale et de l'image segumenté
    cout << "Trying to load ground truth image from: " << groundTruthFilename << endl;
    Mat ground_truth_image = imread(groundTruthFilename, IMREAD_COLOR);

    // Affiche l'image originale
    cv::imshow("Original Image", image);
    cv::moveWindow("Original Image", 200, 0);

    //Affiche l'image segumenté de référence
    if (!groundTruthFilename.empty()) 
    {
        cv::imshow("Reference Segmented Image", ground_truth_image);
        cv::moveWindow("Reference Segmented Image", 1300, 0);
    }

    //Affiche l'image segumenté
    cv::imshow("Segmented Image", segumented_image);
    cv::moveWindow("Segmented Image", 700, 0);

    // Inverse l'image segumenté si nécessaire à la demande de l'utilisateur
    Menu_To_Invert_Image(segumented_image,"Segmented Image");


    // Sauvegarde l'image segumenté
    imwrite(outputFilename, segumented_image);

    //Evalue la segumentation
    cout<<"\nEvaluation of the segmentationu using meanshift:\n"<<endl;
    evaluateSegmentation(segumented_image, ground_truth_image);
    std::cout<<"\nTap Enter to exit"<<std::endl;
    cv::waitKey(0);



    
    return EXIT_SUCCESS;
}
