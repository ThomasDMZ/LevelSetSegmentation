#include "ressources.h"
#include <opencv2/core/core.hpp>

Ressources::Ressources()
{

}

cv::Mat_<double> Ressources::img;
cv::Mat Ressources::img_rgb;
std::vector<cv::Point> Ressources::vect;
std::string Ressources::title;
std::string Ressources::title1;
cv::Mat Ressources::src;
int Ressources::row = Ressources::img_rgb.rows;
int Ressources::col = Ressources::img_rgb.cols;
cv::Mat_<double> Ressources::imgTrans;
cv::Mat_<double> Ressources::kernel;
cv::Mat_<double> Ressources::Ix;
cv::Mat_<double> Ressources::Iy;
cv::Mat_<double> Ressources::f;
cv::Mat_<double> Ressources::g;
cv::Mat_<double> Ressources::Ix2;
cv::Mat_<double> Ressources::Iy2;
int Ressources::ksize = 15; //kernel size for what????
double Ressources::sigma = 1.5;
std::vector< std::vector <cv::Point> > Ressources::contours;
std::vector<cv::Vec4i> Ressources::hierarchy;

cv::Mat_<double> Ressources::u;
cv::Mat_<uchar> Ressources::imgfinal;
