#ifndef MEANSHIFT_FUNCTIONS_HPP
#define MEANSHIFT_FUNCTIONS_HPP

#include <opencv2/core.hpp>

// Affiche les arguments de meanshift
void printHelpMeanshift(const std::string& progName);

// Fonction pour évaluer la segumentation
void evaluateSegmentation(const cv::Mat& estimated, const cv::Mat& reference);

// Fonction de qui retourne la moyenne du voisinage d'un point pour meanshift
cv::Vec3b mean(const cv::Mat& image, int x, int y, float hs, float hc);

// Implémentation de l'agotithme de meanshift
void meanshift(cv::Mat& image, float hs, float hc, float epsilon, int kmax);

// Fonction utiliser pour binariser l'image après meanshift 
cv::Mat binarise(cv::Mat& mat);

// Fonction pour inverser une image en niveau de gris 
void invertImage(cv::Mat& image);

#endif // MEANSHIFT_FUNCTIONS_HPP