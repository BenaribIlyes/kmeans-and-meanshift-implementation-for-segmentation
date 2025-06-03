#ifndef KMEANS_FUNCTIONS_HPP
#define KMEANS_FUNCTIONS_HPP

#include "ocv_utils.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Fonction pour inverser une image en niveau de gris 
void invertImage(Mat& image);

// Affiche les arguments de Kmeans
void printHelpKmeans(const string& progName);

// Menu pour inverser l'image segumenté/binarisé
void Menu_To_Invert_Image(Mat& image, const string& window_name);

// Implémentation personnale de l'agorithme des kmeans
void kmeans_perso(const Mat& data, int K, Mat& labels_perso, TermCriteria criteria, Mat& centers_perso);

// Fonction pour évaluer la segumentation
void evaluateSegmentation(const Mat& estimated, const Mat& reference);

#endif // KMEANS_FUNCTIONS_HPP