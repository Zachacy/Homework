#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#define mask_size 3
#define pi 3.14159
#define canny_threshold_H 100
#define threshold 127
#define canny_threshold_L 50
using namespace cv;
const std::string image_name("Lena.jpg");
int main(int argc, char** argv)
{
	Mat image = imread(image_name, CV_LOAD_IMAGE_UNCHANGED);
	if (!image.data)
	{
		printf("No image data \n");
		return -1;
	}
	Mat Gray_image(image.rows, image.cols, CV_8U);
	Mat Robert_image(image.rows, image.cols, CV_8U);
	Mat Prewitt_image(image.rows, image.cols, CV_8U);
	Mat Sobel_image(image.rows, image.cols, CV_8U);
	Mat Canny_image(image.rows, image.cols, CV_8U);
	Mat smooth_image(image.rows, image.cols, CV_8U);
	Mat Canny_mag(image.rows, image.cols, CV_8U);
	Mat Canny_theta(image.rows, image.cols, CV_32FC1);
	Mat RMS_image(image.rows, image.cols, CV_8U);
	MatIterator_<Vec3b> it;
	MatIterator_<uchar> Gray_it = Gray_image.begin<uchar>();
	float dx, dy;
	for (it = image.begin<Vec3b>(); it != image.end<Vec3b>(); ++it, ++Gray_it)
		(*Gray_it) = (*it)[2] * 0.299 + (*it)[1] * 0.587 + (*it)[0] * 0.114;

	//Robert
	for (int i = 0; i < Gray_image.rows - 1; ++i)
		for (int j = 0; j < Gray_image.cols - 1; ++j) {
			Robert_image.at<uchar>(i, j) = sqrt(pow(Gray_image.at<uchar>(i, j) - Gray_image.at<uchar>(i + 1, j + 1), 2)
				+ pow(-Gray_image.at<uchar>(i, j + 1) + Gray_image.at<uchar>(i + 1, j), 2));
			if (Robert_image.at<uchar>(i, j) > 255)
				Robert_image.at<uchar>(i, j) = 255;
		}
	//Prewitt

	for (int i = (mask_size - 1) / 2; i < image.rows - (mask_size - 1) / 2; ++i)
		for (int j = (mask_size - 1) / 2; j < image.cols - (mask_size - 1) / 2; ++j) {
			dx = pow(-Gray_image.at<uchar>(i - 1, j - 1) - Gray_image.at<uchar>(i - 1, j - 0) - Gray_image.at<uchar>(i - 1, j + 1)
				+ Gray_image.at<uchar>(i + 1, j + 1) + Gray_image.at<uchar>(i + 1, j + 0) + Gray_image.at<uchar>(i + 1, j - 1), 2);
			dy = pow(-Gray_image.at<uchar>(i - 1, j - 1) - Gray_image.at<uchar>(i - 0, j - 1) - Gray_image.at<uchar>(i + 1, j - 1)
				+ Gray_image.at<uchar>(i + 1, j + 1) + Gray_image.at<uchar>(i + 0, j + 1) + Gray_image.at<uchar>(i - 1, j + 1), 2);
			Prewitt_image.at<uchar>(i, j) = sqrt(dx + dy);
			if (Prewitt_image.at<uchar>(i, j) > 255)
				Prewitt_image.at<uchar>(i, j) = 255;
		}
	//Sobel
	for (int i = (mask_size - 1) / 2; i < image.rows - (mask_size - 1) / 2; ++i)
		for (int j = (mask_size - 1) / 2; j < image.cols - (mask_size - 1) / 2; ++j) {
			dx = pow(-Gray_image.at<uchar>(i - 1, j - 1) - 2 * Gray_image.at<uchar>(i - 1, j - 0) - Gray_image.at<uchar>(i - 1, j + 1)
				+ Gray_image.at<uchar>(i + 1, j + 1) + 2 * Gray_image.at<uchar>(i + 1, j + 0) + Gray_image.at<uchar>(i + 1, j - 1), 2);
			dy = pow(-Gray_image.at<uchar>(i - 1, j - 1) - 2 * Gray_image.at<uchar>(i - 0, j - 1) - Gray_image.at<uchar>(i + 1, j - 1)
				+ Gray_image.at<uchar>(i + 1, j + 1) + 2 * Gray_image.at<uchar>(i + 0, j + 1) + Gray_image.at<uchar>(i - 1, j + 1), 2);
			Sobel_image.at<uchar>(i, j) = sqrt(dx + dy);
			if (Sobel_image.at<uchar>(i, j) > 255)
				Sobel_image.at<uchar>(i, j) = 255;
		}
	//canny//

	//blur//
	for (int i = (mask_size - 1) / 2; i < image.rows - (mask_size - 1) / 2; ++i)
		for (int j = (mask_size - 1) / 2; j < image.cols - (mask_size - 1) / 2; ++j) {
			smooth_image.at<uchar>(i, j) = (Gray_image.at<uchar>(i - 1, j - 1) + 2 * Gray_image.at<uchar>(i, j - 1) + Gray_image.at<uchar>(i + 1, j - 1)
				+ Gray_image.at<uchar>(i - 1, j) * 2 + 4 * Gray_image.at<uchar>(i, j) + 2 * Gray_image.at<uchar>(i + 1, j)
				+ Gray_image.at<uchar>(i - 1, j + 1) + 2 * Gray_image.at<uchar>(i, j + 1) + Gray_image.at<uchar>(i + 1, j + 1)) / 16;
		}
	//calc edge Sobel//
	for (int i = (mask_size - 1) / 2; i < image.rows - (mask_size - 1) / 2; ++i)
		for (int j = (mask_size - 1) / 2; j < image.cols - (mask_size - 1) / 2; ++j) {
			dx = -smooth_image.at<uchar>(i - 1, j - 1) - 2 * smooth_image.at<uchar>(i - 1, j - 0) - smooth_image.at<uchar>(i - 1, j + 1)
				+ smooth_image.at<uchar>(i + 1, j + 1) + 2 * smooth_image.at<uchar>(i + 1, j + 0) + smooth_image.at<uchar>(i + 1, j - 1);
			dy = -smooth_image.at<uchar>(i - 1, j - 1) - 2 * smooth_image.at<uchar>(i - 0, j - 1) - smooth_image.at<uchar>(i + 1, j - 1)
				+ smooth_image.at<uchar>(i + 1, j + 1) + 2 * smooth_image.at<uchar>(i + 0, j + 1) + smooth_image.at<uchar>(i - 1, j + 1);
			Canny_mag.at<uchar>(i, j) = sqrt(pow(dx, 2) + pow(dy, 2));
			if (dx == 0)
				Canny_theta.at<float>(i, j) = pi / 2;
			else
				Canny_theta.at<float>(i, j) = atan(dy / dx);
		}
	//Non supress Maxmum//
	Canny_mag.copyTo(RMS_image);
	for (int i = (mask_size - 1) / 2; i < image.rows - (mask_size - 1) / 2; ++i)
		for (int j = (mask_size - 1) / 2; j < image.cols - (mask_size - 1) / 2; ++j) {
			float Tangent = Canny_theta.at<float>(i, j) * 180 / pi;
			if (((-22.5 < Tangent) && (Tangent <= 22.5)) || ((157.5 < Tangent) && (Tangent <= -157.5))) // horizontal
				if ((Canny_mag.at<uchar>(i, j) > Canny_mag.at<uchar>(i, j + 1)) && (Canny_mag.at<uchar>(i, j) > Canny_mag.at<uchar>(i, j - 1)));
				else
					RMS_image.at<uchar>(i, j) = 0;

			if (((-112.5 < Tangent) && (Tangent <= -67.5)) || ((67.5 < Tangent) && (Tangent <= 112.5)))//vertical
				if ((Canny_mag.at<uchar>(i, j) > Canny_mag.at<uchar>(i + 1, j)) && (Canny_mag.at<uchar>(i, j) > Canny_mag.at<uchar>(i - 1, j)));
				else
					RMS_image.at<uchar>(i, j) = 0;

			if (((-67.5 < Tangent) && (Tangent <= -22.5)) || ((112.5 < Tangent) && (Tangent <= 157.5)))//-45
				if ((Canny_mag.at<uchar>(i, j) > Canny_mag.at<uchar>(i - 1, j + 1)) && (Canny_mag.at<uchar>(i, j) > Canny_mag.at<uchar>(i + 1, j - 1)));
				else
					RMS_image.at<uchar>(i, j) = 0;

			if (((-157.5 < Tangent) && (Tangent <= -112.5)) || ((22.5 < Tangent) && (Tangent <= 67.5)))//45
				if ((Canny_mag.at<uchar>(i, j) > Canny_mag.at<uchar>(i + 1, j + 1)) && (Canny_mag.at<uchar>(i, j) > Canny_mag.at<uchar>(i - 1, j - 1)));
				else
					RMS_image.at<uchar>(i, j) = 0;
		}
	//connect each edge
	for (int i = 0; i < Gray_image.rows; ++i)
		for (int j = 0; j < Gray_image.cols; ++j) {
			Canny_image.at<uchar>(i, j) = RMS_image.at<uchar>(i, j);
			if (Canny_image.at<uchar>(i, j) > canny_threshold_H)
				Canny_image.at<uchar>(i, j) = 255;
			else if (Canny_image.at<uchar>(i, j) < canny_threshold_L)
				Canny_image.at<uchar>(i, j) = 0;
			else {
				bool anyHigh = false;
				bool anyBetween = false;
				for (int x = i - 1; x < i + 2; x++) {//kernel
					for (int y = j - 1; y<j + 2; y++) {
						if (x <= 0 || y <= 0 || x > Canny_image.rows || y > Canny_image.cols) //Out of bounds
							continue;
						else {
							if (Canny_image.at<uchar>(x, y) > canny_threshold_H) {
								Canny_image.at<uchar>(i, j) = 255;
								anyHigh = true;
								break;
							}
							else if (Canny_image.at<uchar>(x, y) <= canny_threshold_H && Canny_image.at<uchar>(x, y) >= canny_threshold_L)
								anyBetween = true;
						}
					}
					if (anyHigh)
						break;
				}
				if (!anyHigh && anyBetween)
					for (int x = i - 2; x < i + 3; x++) {
						for (int y = j - 1; y<j + 3; y++) {
							if (x < 0 || y < 0 || x > Canny_image.rows || y > Canny_image.cols) //Out of bounds
								continue;
							else {
								if (Canny_image.at<uchar>(x, y) > canny_threshold_H) {
									Canny_image.at<uchar>(i, j) = 255;
									anyHigh = true;
									break;
								}
							}
						}
						if (anyHigh)
							break;
					}
				if (!anyHigh)
					Canny_image.at<uchar>(i, j) = 0;
			}

		}
	// convert to binary image
	/*
	MatIterator_<uchar> Robert_it = Robert_image.begin<uchar>();
	MatIterator_<uchar> Prewitt_it = Prewitt_image.begin<uchar>();
	MatIterator_<uchar> sobel_it = Sobel_image.begin<uchar>();
	MatIterator_<uchar> canny_it = Canny_image.begin<uchar>();
	for (Gray_it = Gray_image.begin<uchar>(); Gray_it != Gray_image.end<uchar>(); ++Gray_it, ++Robert_it, ++sobel_it, ++ canny_it, ++Prewitt_it) {
		if ((*Robert_it) >= threshold*0.2)
			(*Robert_it) = 255;
		else
			(*Robert_it) = 0;
		if ((*Prewitt_it) >= threshold)
			(*Prewitt_it) = 255;
		else
			(*Prewitt_it) = 0;
		if ((*sobel_it) >= threshold)
			(*sobel_it) = 255;
		else
			(*sobel_it) = 0;
		if ((*canny_it) >= threshold)
			(*canny_it) = 255;
		else
			(*canny_it) = 0;
	}
	*/
	imwrite("Robert.jpg", Robert_image);
	imwrite("Prewitt.jpg", Prewitt_image);
	imwrite("Sobel.jpg", Sobel_image);
	imwrite("Canny.jpg", Canny_image);
	//imshow("Robert",Robert_image);
	//waitKey(2000);
	//imshow("Prewitt",Prewitt_image);
	//waitKey(2000);
	//imshow("Sobel",Sobel_image);
	//waitKey(2000);
	imshow("Canny", Canny_image);
	waitKey(0);
	destroyAllWindows();
	return 0;
}
