#include "operations.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//Define Pi constant
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace cv;

Operations::Operations()
{

}

Mat Operations::grdX(Mat_<double> matrix)
{
    int row = matrix.rows;
    int col = matrix.cols;
    Mat_<double> grd(row,col);
    for (int i=0; i<row ;i++)
    {
        for (int j=0; j<col; j++)
        {
            if (j==0)
            {grd(i,0) = (double)matrix(i,1)-(double)matrix(i,0);}
            else if (j==col-1)
            {grd(i,col-1) = (double)matrix(i,col-1)-(double)matrix(i,col-2);}
            else {grd(i,j)=((double)matrix(i,j+1)-(double)matrix(i,j-1))/2;}
        }
    }
    return grd;
}

Mat Operations::grdY(Mat_<double> matrix)
{
    int row = matrix.rows;
    int col = matrix.cols;
    Mat_<double> grd(row,col);
    for (int i=0; i<row ;i++)
    {
        for (int j=0; j<col; j++)
        {
            if (i==0)
            {grd(0,j) = (double)matrix(1,j)-(double)matrix(0,j);}
            else if (i==row-1)
            {grd(row-1,j) = (double)matrix(row-1,j)-(double)matrix(row-2,j);}
            else {grd(i,j)=((double)matrix(i+1,j)-(double)matrix(i-1,j))/2;}
        }
    }
    return grd;
}

Mat Operations::central_curvature(Mat_<double> nx, Mat_<double> ny)
{
    int row = nx.rows;
    int col = nx.cols;
    Mat_<double> nxx(row,col);
    Mat_<double> nyy(row,col);

    nxx = Operations::grdX(nx);
    nyy = Operations::grdY(ny);
    Mat_<double> K(row,col);
    K = nxx + nyy;
    return K;
}

Mat Operations::NeumannBoundCond(Mat_<double> matrix)
{
    int row = matrix.rows;
    int col = matrix.cols;
    Mat_<double> nbc(row,col);
    nbc = matrix;

    nbc(0,0)=(double)matrix(2,2);
    nbc(0,col-1)=(double)matrix(2,col-3);
    nbc(row-1,0)=(double)matrix(row-3,2);
    nbc(row-1,col-1)=(double)matrix(row-3,col-3);


    for (int j=1; j<col-1; j++)
    {
        nbc(0,j)=(double)matrix(2,j);
        nbc(row-1,j)=(double)matrix(row-3,j);
    }
    for (int i=1; i<row-1; i++)
    {
        nbc(i,0)=(double)matrix(i,2);
        nbc(i,col-1)=(double)matrix(i,col-3);
    }
    return nbc;

}

Mat Operations::dirac(Mat_<double> matrix, double sigma)
{
    int row = matrix.rows;
    int col = matrix.cols;
    Mat_<double> f(row,col);
    for (int i=0; i<row; i++)
    {
        for (int j=0; j<col; j++)
        {
            if ((double)matrix(i,j)<=sigma && (double)matrix(i,j)>=-sigma)
            {
                f(i,j)=(1/(2*sigma))*(1+cos(M_PI*(double)matrix(i,j)/sigma));
            }
            else
            {
                f(i,j)=0;
            }
        }
    }
    return f;
}

Mat Operations::del2(Mat_<double> matrix) //Matlab del2 function in C++
{
    int row = matrix.rows;
    int col = matrix.cols;
    Mat_<double> lap(row,col);
    matrix.copyTo(lap);

    //INSIDE MATRIX
    for (int i=1; i<row-1 ;i++)
    {
        for (int j=1; j<col-1; j++)
        {
            lap(i,j) = ((double)matrix(i-1,j)+(double)matrix(i+1,j)+(double)matrix(i,j-1)+(double)matrix(i,j+1))/4 - (double)matrix(i,j);
        }
    }

    //CORNERS
    lap(0,0)=(-5*((double)matrix(0,1)+(double)matrix(1,0))+4*((double)matrix(0,2)+(double)matrix(2,0))-((double)matrix(0,3)+(double)matrix(3,0))+4*(double)matrix(0,0))/4;
    lap(0,col-1)=(-5*((double)matrix(0,col-2)+(double)matrix(1,col-1))+4*((double)matrix(0,col-3)+(double)matrix(2,col-1))-((double)matrix(0,col-4)+(double)matrix(3,col-1))+4*(double)matrix(0,col-1))/4;
    lap(row-1,0)=(-5*((double)matrix(row-1,1)+(double)matrix(row-2,0))+4*((double)matrix(row-1,2)+(double)matrix(row-3,0))-((double)matrix(row-1,3)+(double)matrix(row-4,0))+4*(double)matrix(row-1,0))/4;
    lap(row-1,col-1)=(-5*((double)matrix(row-1,col-2)+(double)matrix(row-2,col-1))+4*((double)matrix(row-1,col-3)+(double)matrix(row-3,col-1))-((double)matrix(row-1,col-4)+(double)matrix(row-4,col-1))+4*(double)matrix(row-1,col-1))/4;

    //EDGES
    //left and right
    for (int i=1; i<row-1 ;i++)
    {
        lap(i,0)=(-5*(double)matrix(i,1)+4*(double)matrix(i,2)-(double)matrix(i,3)+(double)matrix(i-1,0)+(double)matrix(i+1,0))/4;
        lap(i,col-1)=(-5*(double)matrix(i,col-2)+4*(double)matrix(i,col-3)-(double)matrix(i,col-4)+(double)matrix(i-1,col-1)+(double)matrix(i+1,col-1))/4;
    }

    //up and down
    for (int j=1; j<col-1 ;j++)
    {
        lap(0,j)=(-5*(double)matrix(1,j)+4*(double)matrix(2,j)-(double)matrix(3,j)+(double)matrix(0,j-1)+(double)matrix(0,j+1))/4;
        lap(row-1,j)=(-5*(double)matrix(row-2,j)+4*(double)matrix(row-3,j)-(double)matrix(row-4,j)+(double)matrix(row-1,j-1)+(double)matrix(row-1,j+1))/4;
    }

    return lap;
}

Mat Operations::fspecial(int ksize, double sigma)
{
    Mat_<double> x(ksize,ksize);
    Mat_<double> y(ksize,ksize);
    int xini = -(ksize-1)/2;
    int yini = xini;
    int xvar = xini;
    int yvar = yini;
    //    int contMax = (ksize-1)/2; unused variable
    for (int i=0; i<ksize; i++)
    {
        for (int j=0; j<ksize; j++)
        {
            x(i,j)=xvar;
            xvar++;
            y(i,j)=yvar;
        }
        xvar = xini;
        yvar++;
    }
    Mat_<double> arg(ksize,ksize);
    Mat_<double> x2(ksize,ksize);
    Mat_<double> y2(ksize,ksize);
    multiply(x,x,x2);
    multiply(y,y,y2);
    arg = x2 + y2;
    multiply(-1,arg,arg);
    double div = 2*sigma*sigma;
    divide(arg,div,arg);
    Mat_<double> h(ksize,ksize);
    exp(arg,h);
    double hsum;
    hsum=sum(h)[0];
    divide(h,hsum,h);
    for (int i=0; i<ksize; i++)

        for (int j=0; j<ksize; j++)
        {
            if ((double)h(i,j) < 0.0001)
            {h(i,j)=0;}
        }
    return h;
}

Mat Operations::conv2(Mat_<double> src, Mat_<double> kernel) //Matlab conv2 function
{
    int row = src.rows;
    int col = src.cols;
    int krow = kernel.rows;
    int kcol = kernel.cols;
    int term = (krow-1)/2;
    Mat_<double> out(row,col);
    for (int k = 0; k<row; k++)
        for (int l = 0; l<col; l++)
        {
            out(k,l) = 0;
        }

    for (int i = 0; i<row; i++)
    {
        for (int j = 0; j<col; j++)
        {
            double trans = 0;
            for (int k = 0; k<krow; k++)
            {
                for (int l = 0; l<kcol; l++)
                {
                    if ((i+k-term)<0 || (j+l-term)<0 || (i+k-term)>=row || (j+l-term)>=col )
                    {
                        trans = trans + 0;
                    }
                    else
                    {
                        trans = trans + (double)kernel(k,l)*(double)src(i+k-term,j+l-term);
                    }
                }
            }
            out(i,j) = trans;
        }
    }
    return out;
}
