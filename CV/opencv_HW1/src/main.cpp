/*  I have finished this project on Linux 16.04 LTS enviroment 
	so if you compile in other enviroment( ex : win7) by CMakeLists , it may be appread some error  */
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <string.h>
#define threshold 100 // Set threshold by binary-gray image
#define mask_size 5 // Mask size value for Gauss_filter or Median filter or Mean filter
using namespace cv;
const std::string image_name("Lena.jpg"); // Load picture 
uchar bubble_sort(int i, int j, Mat matrix, int ch);
uchar Gauss_filter(int i, int j, Mat matrix, int ch);
int main(int argc, char** argv)
{
	Mat image = imread(image_name, CV_LOAD_IMAGE_UNCHANGED);
	if (!image.data)
	{
		printf("No image data \n");
		return -1;
	}
	Mat Gray_image(image.rows, image.cols, CV_8U);
	Mat Binary_image(image.rows, image.cols, CV_8U);
	Mat Rotate_image(image.cols, image.rows, CV_8UC3);
	Mat Mean_filter_image(image.rows, image.cols, CV_8UC3);
	Mat Median_filter_image(image.rows, image.cols, CV_8UC3);
	Mat Gauss_image(image.rows, image.cols, CV_8UC3);
	Mat_<Vec3b>::iterator it = image.begin<Vec3b>();
	Mat_<uchar>::iterator Gray_it = Gray_image.begin<uchar>();
	Mat_<uchar>::iterator Binary_it = Binary_image.begin<uchar>();
	
	// Gray_level convert //
	for(it = image.begin<Vec3b>(); it != image.end<Vec3b>(); ++it, ++Gray_it)
		(*Gray_it) = (*it)[2] * 0.299 + (*it)[1] * 0.587 + (*it)[0] * 0.114;
	
	//Binary image convert //
	for (Gray_it = Gray_image.begin<uchar>(), Binary_it = Binary_image.begin<uchar>(); Gray_it != Gray_image.end<uchar>();
	++Gray_it, ++Binary_it)
		if (*Gray_it >= threshold)
			*Binary_it = 255;
		else
			*Binary_it = 0;
			
	// Rotate image processing //
	for (int i = 0; i < image.rows; ++i)
		for (int j = 0; j < image.cols; ++j) {
			Rotate_image.at<Vec3b>(j, i)[0] = image.at<Vec3b>(image.rows - i, j)[0];
			Rotate_image.at<Vec3b>(j, i)[1] = image.at<Vec3b>(image.rows - i, j)[1];
			Rotate_image.at<Vec3b>(j, i)[2] = image.at<Vec3b>(image.rows - i, j)[2];
		}
		
	// Low(Mean)_filter image processing //
	for (int i = (mask_size - 1) / 2; i < image.rows - (mask_size - 1) / 2; ++i)
		for (int j = (mask_size - 1) / 2; j < image.cols - (mask_size - 1) / 2; ++j)
			for (int x = -(mask_size - 1) / 2; x <= (mask_size - 1) / 2; x++)
				for (int y = -(mask_size - 1) / 2; y <= (mask_size - 1) / 2; y++) {
					Mean_filter_image.at<Vec3b>(i, j)[0] += image.at<Vec3b>(i + x, j + y)[0] / pow(mask_size, 2);
					Mean_filter_image.at<Vec3b>(i, j)[1] += image.at<Vec3b>(i + x, j + y)[1] / pow(mask_size, 2);
					Mean_filter_image.at<Vec3b>(i, j)[2] += image.at<Vec3b>(i + x, j + y)[2] / pow(mask_size, 2);
				}
				
	// Median_filter image processing //
	for (int i = (mask_size - 1) / 2; i < image.rows - (mask_size - 1) / 2; ++i)
		for (int j = (mask_size - 1) / 2; j < image.cols - (mask_size - 1) / 2; ++j) {
			int ch = 0;
			while (ch < 3) {
				Median_filter_image.at<Vec3b>(i, j)[ch] = bubble_sort(i, j, image, ch);
				++ch;
			}
		}

	// Gauss_filter image processing //
	for (int i = (mask_size - 1) / 2; i < image.rows - (mask_size - 1) / 2; ++i)
		for (int j = (mask_size - 1) / 2; j < image.cols - (mask_size - 1) / 2; ++j) {
			int ch = 0;
			while (ch <3) {
				Gauss_image.at<Vec3b>(i, j)[ch] = Gauss_filter(i, j, image, ch);
				++ch;
			}
		}


	// Show All_image result
	imshow("DisPlay original Image", image);

	imshow("Display Gray Image", Gray_image);
	moveWindow("Display Gray Image", image.rows * 2, 0);
	waitKey(2000);
	destroyWindow("Display Gray Image");

	imshow("Display Binaty Image", Binary_image);
	moveWindow("Display Binaty Image", image.rows * 2, 0);
	waitKey(2000);
	destroyWindow("Display Binaty Image");

	imshow("DisPlay Rotate Image", Rotate_image);
	moveWindow("DisPlay Rotate Image", image.rows * 2, 0);
	waitKey(2000);
	destroyWindow("DisPlay Rotate Image");

	imshow("DisPlay Mean filter Image", Mean_filter_image);
	moveWindow("DisPlay Mean filter Image", image.rows * 2, 0);
	waitKey(2000);
	destroyWindow("DisPlay Mean filter Image");

	imshow("DisPlay Median filter Image", Median_filter_image);
	moveWindow("DisPlay Median filter Image", image.rows * 2, 0);
	waitKey(2000);
	destroyAllWindows();
	return 0;
}
uchar bubble_sort(int i, int j, Mat matrix, int ch) {
	unsigned char temp[mask_size*mask_size];
	int w = 0;
	for (int x = -(mask_size - 1) / 2; x <= (mask_size - 1) / 2; x++)
		for (int y = -(mask_size - 1) / 2; y <= (mask_size - 1) / 2; y++) {
			temp[w] = matrix.at<Vec3b>(i + x, j + y)[ch];
			w++;
		}
	std::sort(temp, temp + mask_size*mask_size);
	return temp[(mask_size*mask_size - 1) / 2];
}
uchar Gauss_filter(int i, int j, Mat matrix, int ch) {
	double temp = 0;
	double weight = 0;
	for (int x = -(mask_size - 1) / 2; x <= (mask_size - 1) / 2; x++)
		for (int y = -(mask_size - 1) / 2; y <= (mask_size - 1) / 2; y++) {
			temp += matrix.at<Vec3b>(i + x, j + y)[ch] * exp(-(x*x + y*y) / 2);
			weight += exp(-(x*x + y*y) / 2);
		}
	return temp / weight + 0.5f;
}
