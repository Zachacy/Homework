#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <omp.h>
#define Canny_theshold_H 200
#define Canny_theshold_L 150
#define Hough_theshold_polar 250
#define Hough_theshold_rec 200
#define Range_theta 360
#define pi 3.141592
using namespace cv;
using namespace std;
Mat src_gray,image;
int Max_radius;
void Hough_polar_transform(int x, int y, Mat &tranform, Mat Hough);
void Hough_plot_polar_line(int theta, int radius);
void Hough_rectangular(Mat &original, Mat &tranform, Mat gray);
int main(int argc, char** argv )
{
    image = imread(argv[1]);
    Max_radius = sqrt(pow(image.cols,2)+pow(image.rows,2));
    cvtColor(image,src_gray,CV_RGB2GRAY);
    Mat Hough_rec_line = image.clone();
    Mat dst(src_gray.size(),CV_8UC1);
    Mat Hough_tranform_rec = Mat::zeros(src_gray.size(),CV_8UC1);
    Mat Hough_tranform_polar = Mat::zeros(Max_radius/3,360,CV_8UC1);
    Mat Hough_polar=Mat::zeros(360,Max_radius,CV_8UC1);//[theta][radius]
    Canny(src_gray,dst,Canny_theshold_H,Canny_theshold_L,3);

    // Calculate Hough_polar Transform //
    for(int j = 0; j < src_gray.rows; j++)
        for(int i= 0; i < src_gray.cols; i++){
                if(dst.at<uchar>(j,i)==255)
                    Hough_polar_transform(j,i,Hough_tranform_polar,Hough_polar);
            }
    // Plot Hough_polar Line //
    for(int j = 0; j < Range_theta; j ++)
        for(int i = 0; i < Max_radius; i++){
            if(Hough_polar.at<uchar>(j,i) > Hough_theshold_polar)
                Hough_plot_polar_line(j,i);
        }

    // Calculate Hough_Rec Transform //
    Hough_rectangular(Hough_rec_line,Hough_tranform_rec,dst);
    imshow("Canny",dst);
    imshow("Hough_polar_coordinate",Hough_tranform_polar);
    imshow("Hough_polar_result",image);
    imshow("Hough_rec_coordinate",Hough_tranform_rec);
    imshow("Hough_rec_result",Hough_rec_line);
    imwrite("Canny.jpg",dst);
    imwrite("Hough_polar_coordinate.jpg",Hough_tranform_polar);
    imwrite("Hough_polar_result.jpg",image);
    imwrite("Hough_rec_coordinate.jpg",Hough_tranform_rec);
    imwrite("Hough_rec_result.jpg",Hough_rec_line);
    moveWindow("Hough_rec_result",image.rows,20);
    waitKey(0);
}
void Hough_polar_transform(int x, int y,Mat &tranform,Mat Hough){
    double radius;
    for(int theta = 0; theta < Range_theta; theta++){
        radius = (x)*cos(theta*pi/180)+(y)*sin(theta*pi/180);
        Hough.at<uchar>(theta,int(radius)) += 1;
        int r = (radius+Max_radius)/6;
        tranform.at<uchar>(r,theta)+=1;
    }
}
void Hough_plot_polar_line(int theta, int radius){
    int temp;
    for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++){
            temp = (y)*cos(theta*pi/180)+(x)*sin(theta*pi/180);
            if(temp == radius){
                image.at<Vec3b>(y,x)[0]=0;
                image.at<Vec3b>(y,x)[1]=0;
                image.at<Vec3b>(y,x)[2]=255;
            }
        }
}
void Hough_rectangular(Mat &original, Mat &tranform, Mat gray) {
    Mat output = Mat(2 * src_gray.rows*(src_gray.cols + 1), 2 * src_gray.rows, CV_8UC1, Scalar::all(0));
    for(int j = 0; j < src_gray.rows; j++)
        for(int i = 0; i < src_gray.cols; i++)
            if(gray.at<uchar>(j,i)==255){
            #pragma omp parallel for
                for (int h = -src_gray.rows ; h < src_gray.rows; h++) {
                    int  n = -h*i + j;
                    output.at<uchar>(n + src_gray.rows * (src_gray.cols + 1), h + src_gray.rows) +=  + 1;
                    int a = (h + src_gray.rows) * src_gray.cols / (2 * src_gray.rows);
                    int b = (n + src_gray.rows * (src_gray.cols + 1)) * src_gray.rows / (2 * src_gray.rows*(src_gray.cols + 1));
                    if(a <= src_gray.rows && b <= src_gray.cols)
                        tranform.at<uchar>(a,b) += 1;
                }
    }
    for (int h = 0; h < output.rows; h++)
        #pragma omp parallel for
        for (int w = 0; w < output.cols; w++)
            if (output.at<uchar>(h, w) > Hough_theshold_rec)
                for(int x = 0; x < src_gray.cols; x++){
                    int y = (w-src_gray.rows)*x+(h-(src_gray.rows*(src_gray.cols+1)));
                    if(y <= src_gray.rows && y >= 0)
                        original.at<Vec3b>(y , x) = 255;
                }
}
