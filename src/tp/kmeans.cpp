#include "ocv_utils.hpp"
#include "kmeans_functions.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>



using namespace cv;
using namespace std;



int main(int argc, char** argv)
{

    // Vérifie le nombre d'arguments
    if (argc < 3 || argc > 5)
    {
        cout << " Incorrect number of arguments." << endl;
        printHelpKmeans(string(argv[0]));
        return EXIT_FAILURE;
    }

    const auto imageFilename = string(argv[1]);
    const int k = stoi(argv[2]);
    const string groundTruthFilename = (argc > 3) ? string(argv[3]) : "";
    const string outputFilename = (argc == 5) ? string(argv[4]) : "Segmented_Image.jpg";
   

    // Just for debugging
    {
        cout << " Program called with the following arguments:" << endl;
        cout << " \timage file: " << imageFilename << endl;
        cout << " \tk: " << k << endl;
        if(!groundTruthFilename.empty()) cout << " \tground truth segmentation: " << groundTruthFilename << endl;
        cout << " \toutput file: " << outputFilename << endl;
    }


    // Charge l'image
    Mat image = imread(imageFilename, IMREAD_COLOR);
    //PRINT_MAT_INFO(image);


    // 1) in order to call kmeans we need to first convert the image into floats (CV_32F)
    Mat image_float;
    image.convertTo(image_float,CV_32F);

    // 2) kmeans asks for a mono-dimensional list of "points". Our "points" are the pixels of the image that can be seen as 3D points
    // where each coordinate is one of the color channel (e.g. R, G, B). But they are organized as a 2D table, we need
    // to re-arrange them into a single vector.
    Mat image_1D = image_float.reshape(1, image_float.rows * image_float.cols);
    //PRINT_MAT_INFO(image_1D);


    // We nedd to initialise first the labels and the centers and the criteria
    Mat labels, centers;
    Mat labels_perso,centers_perso;

    // the values here needs to be decided regarding the kind of image we want to seguegment
    TermCriteria criteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 100, 0.2);

    // now we can call kmeans, the most important parameter is the initialization of the centers
    kmeans(image_1D, k, labels, criteria, 2, KMEANS_RANDOM_CENTERS, centers); // select random initial centers in each attempt
    //kmeans(image_1D, k, labels, criteria, 10, KMEANS_PP_CENTERS  , centers); // use kmeans++ center initialization by Arthur and Vassilvitskii [Arthur2007]
    //kmeans(image_1D, k, labels, criteria, 10, KMEANS_USE_INITIAL_LABELS, centers); //during the first (and possibly the only) attempt, use the user-supplied labels instead of computing them from the initial centers. For the second and further attempts, use the random or semi-random center
    kmeans_perso(image_1D, k, labels_perso, criteria, centers_perso); // kmeans_perso is a function that we have implemented
    

    // To convert the centers to 8-bit values, we need to convert the type of the matrix
    centers.convertTo(centers, CV_8U);
    centers_perso.convertTo(centers_perso, CV_8U);

    // labels is a matrix with only n values of n intensuty for the cluster, we need to make them more visible 
    labels = labels * 255/(k-1); // normalize the labels to 0-255
    labels_perso = labels_perso * 255/(k-1); // normalize the labels to 0-255

    //  Create  new images that will contain the segmented images
    Mat segmented_image(image_float.size(), CV_8UC1);
    Mat segmented_image_perso(image_float.size(), CV_8UC1);

    // 3) We need to reshape the labels to the original image size
    segmented_image = labels.reshape(1, image.rows);
    segmented_image_perso = labels_perso.reshape(1, image.rows);

    // We need to convert the image to 8-bit in order to display it
    Mat segmented_image_display,segmented_image_display_perso;
    segmented_image.convertTo(segmented_image_display, CV_8U);
    segmented_image_perso.convertTo(segmented_image_display_perso, CV_8U);

    // 4) Display the original image and the segumented image
    cout << "Trying to load ground truth image from: " << groundTruthFilename << endl;
    Mat ground_truth_image = imread(groundTruthFilename, IMREAD_COLOR);

    // Display the original image
    cv::imshow("Original Image", image);
    cv::moveWindow("Original Image", 200, 0);

    //Display the reference for segumentation if it exists
    if (!groundTruthFilename.empty()) 
    {
        cv::imshow("Reference Segmented Image", ground_truth_image);
        cv::moveWindow("Reference Segmented Image", 1300, 0);
    }

    // Display the segmented image
    cv::imshow("Segumented image using OpenCV kmeans", segmented_image_display);
    cv::moveWindow("Segumented image using OpenCV kmeans", 700, 0);

    // Invert the segmented image if needed for opencv kmeans
    Menu_To_Invert_Image(segmented_image_display,"Segumented image using OpenCV kmeans");

    // Display personal kmeans result
    cv::imshow("Segumented image using kmeans_perso", segmented_image_display_perso);
    cv::moveWindow("Segumented image using kmeans_perso", 0, 0);

    // Invert the segmented image if needed for kmeans perso
    Menu_To_Invert_Image(segmented_image_display_perso,"Segumented image using kmeans_perso");

    // 5) Save the segmented image
    imwrite(outputFilename, segmented_image_display);

    //cv::imshow("Segmented Image", segmented_image_display);
    //cv::moveWindow("Segmented Image", 700, 0);

    // 6) Evaluate the segmentation using precision, recall and Dice Similarity Coefficient (DSC)
    cout<<"Evaluation of the segmentationu using OpenCv kmeans:"<<endl;
    cout<<""<<endl;
    evaluateSegmentation(segmented_image_display, ground_truth_image);
    cout<<""<<endl;
    cout<<"Evaluation of the segmentationu using perso kmeans:"<<endl;
    cout<<""<<endl;
    evaluateSegmentation(segmented_image_display_perso, ground_truth_image);

    std::cout<<"Tap Enter to exit"<<std::endl;
    cv::waitKey(0);


    return EXIT_SUCCESS;
}
