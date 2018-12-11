#ifndef RESSOURCES_H
#define RESSOURCES_H

#include <opencv2/core/core.hpp>

class Ressources
{
public:
    Ressources();
    static cv::Mat_<double> img;
    static cv::Mat img_rgb;
    static std::vector<cv::Point> vect;
    static std::string title;
    static std::string title1;
    static cv::Mat src;
    static int row;
    static int col;
    static int ksize;
    static cv::Mat_<double> imgTrans;
    static cv::Mat_<double> kernel;
    static cv::Mat_<double> Ix;
    static cv::Mat_<double> Iy;
    static cv::Mat_<double> f;
    static cv::Mat_<double> g;
    static cv::Mat_<double> Ix2;
    static cv::Mat_<double> Iy2;
    static double sigma;
    static std::vector< std::vector < cv::Point > > contours;
    static std::vector<cv::Vec4i> hierarchy;

    static cv::Mat_<double> u;
    static cv::Mat_<uchar> imgfinal;
};

#endif // RESSOURCES_H
