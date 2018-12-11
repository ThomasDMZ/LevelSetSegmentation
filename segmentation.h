#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <opencv2/core/core.hpp>

class Segmentation
{
public:
    Segmentation();
    static cv::Mat evolution(cv::Mat_<double> u0, cv::Mat_<double> g, int lambda, double mu, int alf, double epsilon, double delt, int numIter);
    static cv::Mat selectROI(cv::Mat_<double> img, cv::Mat_<double> u);
};

#endif // SEGMENTATION_H
