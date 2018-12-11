#include "events.h"
#include "ressources.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>



Events::Events()
{

}

void Events::on_mouse(int evt, int x, int y, int flags, void* param)
{

    if(evt == CV_EVENT_LBUTTONDOWN)
    {
        cv::Point pt(x,y);
        Ressources::vect.push_back(pt);

        if(Ressources::vect.size()>1)
        {
            int size = Ressources::vect.size();
            cv::line(Ressources::img_rgb,Ressources::vect[size-2],Ressources::vect[size-1],CV_RGB(255,0,0),2);
            cv::line(Ressources::src,Ressources::vect[size-2],Ressources::vect[size-1],cv::Scalar(255),1);
            cv::imshow(Ressources::title, Ressources::img_rgb);

        }

    }

    if(evt == CV_EVENT_RBUTTONDOWN)
    {
        int size = Ressources::vect.size();
        if (Ressources::vect.size()>0)
        {
            cv::line(Ressources::img_rgb,Ressources::vect[size-1],Ressources::vect[0],CV_RGB(255,0,0),2);
            cv::line(Ressources::src,Ressources::vect[size-1],Ressources::vect[0],cv::Scalar(255),1);
            cv::imshow(Ressources::title, Ressources::img_rgb);
        }

    }
}

void Events::Dcontours(cv::Mat_<double> matrix, cv::Mat img, std::string title,int k, cv::Scalar color) //Draw contours on image
{
    int row = matrix.rows;
    int col = matrix.cols;
    cv::Mat_<uchar> maskInter(row,col);
    cv::Mat img_rgb_tmp;
    for (int i=0; i<row; i++)
        for (int j=0; j<col; j++)
        {
            if ((double)matrix(i,j) <0)
            {maskInter(i,j)=255;}
            else
            {maskInter(i,j)=0;}
        };

    cv::Mat mat_out = maskInter.clone();
    cv::findContours( mat_out, Ressources::contours, Ressources::hierarchy, CV_RETR_TREE , cv::CHAIN_APPROX_SIMPLE);

    cv::Mat img_gray_32f;
    img.convertTo(img_gray_32f, CV_32F, 1./255.);
    cv::cvtColor(img_gray_32f, img_rgb_tmp, CV_GRAY2RGB);
    cv::drawContours( img_rgb_tmp, Ressources::contours, -1, color,1.5,8, Ressources::hierarchy,2, cv::Point());
    cv::namedWindow(title);
    cv::moveWindow(title, 800, 50);
    cv::imshow(title, img_rgb_tmp);
    cv::waitKey(50);

}
