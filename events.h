#ifndef EVENTS_H
#define EVENTS_H

#include <opencv2/imgproc/imgproc.hpp>

class Events
{
public:
    Events();
    static void on_mouse(int evt, int x, int y, int flags, void* param);
    static void Dcontours(cv::Mat_<double> matrix, cv::Mat img, std::string title,int k, cv::Scalar color);


};

#endif // EVENTS_H
