#ifndef OPERATIONS_H
#define OPERATIONS_H
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Operations
{
public:
    Operations();
    static cv::Mat grdX(cv::Mat_<double> matrix);
    static cv::Mat grdY(cv::Mat_<double> matrix);
    static cv::Mat central_curvature(cv::Mat_<double> nx, cv::Mat_<double> ny);
    static cv::Mat NeumannBoundCond(cv::Mat_<double> matrix);
    static cv::Mat dirac(cv::Mat_<double> matrix, double sigma);
    static cv::Mat del2(cv::Mat_<double> matrix);
    static cv::Mat fspecial(int ksize, double sigma);
    static cv::Mat conv2(cv::Mat_<double> src, cv::Mat_<double> kernel);

};

#endif // OPERATIONS_H
