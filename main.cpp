#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "events.h"
#include "ressources.h"
#include "operations.h"
#include "segmentation.h"
#include <iostream>

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);

//    //-----------------------------------------------
//    //Create the base window with widget
//    //-----------------------------------------------

//    MainWindow w;

//    Ressources::img = cv::imread("C:/Users/212341658/Pictures/ct_1.jpg",0);
//    QPixmap piximg;
//    QWidget *widget = w.findChild<QWidget*>("widget");
//    Q_ASSERT(widget);
//    QLabel *label = new QLabel(widget);

//    //--------------------------------------------------
//    //Choose an OpenCV image to display in Qt
//    //--------------------------------------------------


//    QImage qimage(Ressources::img.data, Ressources::img.cols, Ressources::img.rows, QImage::Format_Indexed8);
//    piximg.convertFromImage(qimage);
//    label->setPixmap(piximg);


//    w.showMaximized();



    Ressources::img = cv::imread("C:\\Users\\212341658\\Pictures\\ct_1.jpg",0);
    Ressources::src = cv::Mat::zeros( cv::Size( Ressources::img.rows, Ressources::img.cols ), CV_8UC1 );
    cv::Mat img_gray_32f;
    Ressources::img.convertTo(img_gray_32f, CV_32F, 1./255.);
    cv::cvtColor(img_gray_32f, Ressources::img_rgb, CV_GRAY2RGB);
    Ressources::title = "window";
    cv::imshow(Ressources::title,Ressources::img_rgb);
//    cv::imshow(Ressources::title1, Ressources::src);
    cv::setMouseCallback(Ressources::title,Events::on_mouse,0);
    cv::waitKey(0);




//    //Display final contour and wait for user input and save contour and segmentations images
    imgfinal=imread(path,0);
    Dcontours(u,imgfinal,finaltitle,kfinal,contourColorG);
    PrintContour(u,contourColorR,path);
    system("cls");
    check++;

    //-----------------------------------------------------
    //Initial image processing

    //Converting img to double and applying Gaussian convolution
    cv::Mat_<double> imgDouble = cv::imread(path,0);
    cv::Mat kernel = Operations::fspecial(Ressources::ksize,Ressources::sigma);
    Ressources::imgTrans = Operations::conv2(Ressources::img,kernel);

    //Erode and Dilate with structural element structur
    cv::Mat_<double> structur = cv::Mat::zeros(cv::Size(3,3),CV_8UC1);
    structur = structur -1;
    structur(2,2) = 4;

    cv::erode(Ressources::imgTrans,Ressources::imgTrans,structur);
    cv::dilate(Ressources::imgTrans,Ressources::imgTrans,structur);

    //Calcul of matrix g
    Ressources::Ix = Operations::grdX(Ressources::imgTrans);
    Ressources::Iy = Operations::grdY(Ressources::imgTrans);
    cv::multiply(Ressources::Ix,Ressources::Ix,Ressources::Ix2);
    cv::multiply(Ressources::Iy,Ressources::Iy,Ressources::Iy2);
    cv::add(Ressources::Ix2,Ressources::Iy2,Ressources::f);
    Ressources::f = 1+ Ressources::f;
    cv::divide(1,Ressources::f,Ressources::g);

    //End of inital image processing
    //-----------------------------------------------------

    Ressources::u = Segmentation::selectROI(Ressources::img,Ressources::u);
    int kfinal;
    int delt = 5;
    double mu = 0.2/delt;
    int iter=500;
    int lambda = 5;
    int alf = 3;
    double epsilon = 1.5;
    cv::destroyAllWindows();
    for (int k = 0; k<iter; k++)
    {
        Ressources::u = Segmentation::evolution(Ressources::u,Ressources::g,lambda,mu,alf,epsilon,delt,1);
        if (k%5 == 0) //Display the contour on the image every 20 iterations
        {
            cv::Mat_<uchar> imgF = cv::imread("C:\\Users\\212341658\\Pictures\\ct_1.jpg",0);
            Events::Dcontours(Ressources::u,imgF,"segmentation in progress...",k, CV_RGB(255,0,0));
        }
        kfinal = k;
    }
    cv::destroyWindow("segmentation in progress...");
    cv::Mat_<uchar> imgF = cv::imread("C:\\Users\\212341658\\Pictures\\ct_1.jpg",0);
    Events::Dcontours(Ressources::u,imgF,"Segmentation complete",0, CV_RGB(255,0,0));
    cv::waitKey();
    cv::destroyWindow("Segmentation complete");


}
