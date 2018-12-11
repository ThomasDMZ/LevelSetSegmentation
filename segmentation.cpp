#include "segmentation.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "operations.h"
#include "ressources.h"
#include "events.h"

using namespace cv;

Segmentation::Segmentation()
{

}


Mat Segmentation::evolution(Mat_<double> u0, Mat_<double> g, int lambda, double mu, int alf, double epsilon, double delt, int numIter)
{
    //Declarations of useful parameters and matrix
    double deci = 0.0000000001;
    int row = u0.rows;
    int col = u0.cols;


    Mat_<double> u(row,col);
    Mat_<double> vx(row,col);
    Mat_<double> vy(row,col);
    Mat_<double> ux(row,col);
    Mat_<double> uy(row,col);
    Mat_<double> ux2(row,col);
    Mat_<double> uy2(row,col);
    Mat_<double> trans1(row,col);
    Mat_<double> normDu(row,col);
    Mat_<double> Nx(row,col);
    Mat_<double> Ny(row,col);
    Mat_<double> diracU(row,col);
    Mat_<double> K(row,col);
    Mat_<double> weightedLengthTerm(row,col);
    Mat_<double> vxNx(row,col);
    Mat_<double> vyNy(row,col);
    Mat_<double> gK(row,col);
    Mat_<double> trans3(row,col);
    Mat_<double> diracUg(row,col);
    Mat_<double> weightedAreaTerm(row,col);
    Mat_<double> penalizingTerm(row,col);
    Mat_<double> term(row,col);

    u0.copyTo(u);
    vx=Operations::grdX(g);
    vy=Operations::grdY(g);

    for (int k = 0; k < numIter; k++)
    {
        u=Operations::NeumannBoundCond(u);
        ux=Operations::grdX(u);
        uy=Operations::grdY(u);

        //normDu
        multiply(ux,ux,ux2);
        multiply(uy,uy,uy2);
        trans1 = ux2 + uy2 + deci;
        sqrt(trans1,normDu);

        divide(ux,normDu,Nx);
        divide(uy,normDu,Ny);
        diracU=Operations::dirac(u,epsilon);
        K=Operations::central_curvature(Nx,Ny);

        //weightedLengthTerm
        multiply(diracU,lambda,weightedLengthTerm);
        multiply(vx,Nx,vxNx);
        multiply(vy,Ny,vyNy);
        multiply(g,K,gK);
        trans3 = vxNx + vyNy + gK;
        multiply(weightedLengthTerm,trans3,weightedLengthTerm);

        //weightedAreaTerm
        multiply(diracU,g,diracUg);
        multiply(alf,diracUg,weightedAreaTerm);


        //penalizingTerm
        penalizingTerm = Operations::del2(u);
        multiply(4,penalizingTerm,penalizingTerm);
        subtract(penalizingTerm,K,penalizingTerm);
        multiply(mu,penalizingTerm,penalizingTerm);

        //evolution of u
        term = weightedLengthTerm + weightedAreaTerm + penalizingTerm;
        multiply(term,delt,term);
        add(u,term,u);
    }
    return u;
}

Mat Segmentation::selectROI(Mat_<double> img, Mat_<double> u) //Selection of the ROI with the mouse

{
    int row = img.rows;
    int col = img.cols;
    Mat_<double> mask (row,col);
    Mat_<double> initialLSF (row,col);
    Ressources::src = Mat::zeros( Size( row, col ), CV_8UC1 );
    Ressources::vect.clear();
    Mat img_gray_32f;
    img.convertTo(img_gray_32f, CV_32F, 1./255.);
    cvtColor(img_gray_32f, Ressources::img_rgb, CV_GRAY2RGB);
    namedWindow(Ressources::title,CV_WINDOW_AUTOSIZE);
    moveWindow(Ressources::title, 800,50);
    setMouseCallback(Ressources::title, Events::on_mouse, 0);
    imshow(Ressources::title, Ressources::img_rgb);
    waitKey(0);
    destroyWindow(Ressources::title);

    //Create contour
    Mat src_copy = Ressources::src.clone();
    findContours( src_copy, Ressources::contours, Ressources::hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    // Create Mask
    if (Ressources::contours.size())
    {
    for (int j=0; j<row; j++)
        for (int i=0; i<col; i++)
            {
                if (pointPolygonTest( Ressources::contours[0], Point2f(i,j), false ) >= 0)
                {mask(j,i)=1;}
                else
                {mask(j,i)=0;}
            };

     // Create inital LSF
    int c0 = 4; //the constant value used to define binary level set function;
    for (int i=0; i<row; i++)
        for (int j=0; j<col; j++)
            {
                initialLSF(i,j)= c0*2*(0.5-(double)mask(i,j));

            };

    initialLSF.copyTo(u);
    }
    return u;
}
