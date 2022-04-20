#pragma once
#include <iostream>
#include <cstring>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>




using namespace std;
using namespace cv;


// saturation filter
Mat Saturation_filter(Mat src) {
	Mat ImageHSV;

	cvtColor(src, ImageHSV, COLOR_BGR2HSV);
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			if (ImageHSV.at<Vec3b>(i, j)[1] < 50) {
				src.at<Vec3b>(i, j)[0] = 0;
				src.at<Vec3b>(i, j)[1] = 0;
				src.at<Vec3b>(i, j)[2] = 0;
			}
			else {
				src.at<Vec3b>(i, j)[0] = 255;
				src.at<Vec3b>(i, j)[1] = 255;
				src.at<Vec3b>(i, j)[2] = 255;
			}
		}
	}
	return src;
}
int SobelX3x3(Mat& src, Mat3s& dst)
{
	/* Declaring a 3 channel signed Variable*/
	Mat3s temp;
	/* Storing image data from source to destination*/
	src.copyTo(temp);
	/*Declaring Variables for channels, rows and columns*/
	int c, i, j;
	/*looping to acces each channel in each pixel*/
	for (c = 0; c < 3; c++)
	{
		for (i = 0; i < src.rows - 1; i++)
		{
			for (j = 1; j < src.cols - 1; j++)
			{
				/* Appying [1 2 1] Filter to each column*/
				temp.at<Vec3s>(i, j)[c] = src.at<Vec3b>(i, j - 1)[c] * 1 + src.at<Vec3b>(i, j)[c] * 2 + src.at<Vec3b>(i, j + 1)[c] * 1;
				/*Dividing output by summation of all constants in filter to bring result back to 256bits*/
				temp.at<Vec3s>(i, j)[c] /= 4;
			}
		}
	}
	for (c = 0; c < 3; c++)
	{
		for (i = 1; i < src.rows - 1; i++)
		{
			for (j = 0; j < src.cols - 1; j++)
			{
				/* Appying [-1 0 1] Filter to each row and storing the final result in destination variable*/
				dst.at<Vec3s>(i, j)[c] = temp.at<Vec3s>(i - 1, j)[c] * -1 + (temp.at<Vec3s>(i, j)[c] * 0) + temp.at<Vec3s>(i + 1, j)[c];
			}
		}
	}
	return 0;
}

/*Ysobel*/
/* Same as X-sobel but applying [1 2 1] to rows instead of column and [-1 0 1] to columns*/
int SobelY3x3(Mat& src, Mat3s& dst)
{
	Mat3s temp;
	src.copyTo(temp);
	int c, i, j;
	for (c = 0; c < 3; c++)
	{
		for (i = 0; i < src.rows - 1; i++)
		{
			for (j = 1; j < src.cols - 1; j++)
			{

				temp.at<Vec3s>(i, j)[c] = src.at<Vec3b>(i, j - 1)[c] * -1 + (src.at<Vec3b>(i, j)[c] * 0) + src.at<Vec3b>(i, j + 1)[c] * 1;

			}
		}
	}
	for (c = 0; c < 3; c++)
	{
		for (i = 1; i < src.rows - 1; i++)
		{
			for (j = 0; j < src.cols - 1; j++)
			{
				dst.at<Vec3s>(i, j)[c] = temp.at<Vec3s>(i - 1, j)[c] * 1 + (temp.at<Vec3s>(i, j)[c] * 2) + temp.at<Vec3s>(i + 1, j)[c];
				dst.at<Vec3s>(i, j)[c] /= 4;
			}
		}
	}
	return 0;
}


/*Gradient Magnitude*/
int Gradient_magnitude(Mat3s& sobelx, Mat3s& sobely, Mat3s& dst) {


	/*Declaring variables for rows, columsn and channel*/
	int c, i, j;
	/*Looping to acces each channel of each pixel*/
	for (i = 0; i < dst.rows; i++) {
		for (j = 0; j < dst.cols; j++) {
			for (c = 0; c < 3; c++) {
				/*assigning each channel in destiantion new values by using the formula sqrt(xs^2+xy^2)*/
				dst.at<Vec3s>(i, j)[c] = sqrt((sobelx.at<Vec3s>(i, j)[c] * sobelx.at<Vec3s>(i, j)[c]) + (sobely.at<Vec3s>(i, j)[c] * sobely.at<Vec3s>(i, j)[c]));

			}
		}
	}
	return 0;
}


// gradient image
int ImageGrad(Mat& src, Mat& dst) {
	Mat3s sobelx;
	src.copyTo(sobelx);
	SobelX3x3(src, sobelx);
	Mat3s sobely;
	src.copyTo(sobely);
	SobelY3x3(src, sobely);

	Mat3s ImageGradient;
	src.copyTo(ImageGradient);
	Gradient_magnitude(sobelx, sobely, ImageGradient);

	ImageGradient.copyTo(dst);
	convertScaleAbs(ImageGradient, dst);
	return 0;
}
Mat EdgeSharpening(Mat src) {
	Mat Gradientabs;
	ImageGrad(src, Gradientabs);
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			for (int c = 0; c < 3; c++) {
				if (Gradientabs.at<Vec3b>(i, j)[c] > 0) {
					src.at<Vec3b>(i, j)[c] = 255;
				}
			}
		}
	}
	return src;
}


// draw edge of the image.. didnt use
Mat EdgeDrawing(Mat src) {
	Mat Gradientabs;
	ImageGrad(src, Gradientabs);
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			for (int c = 0; c < 3; c++) {
				if (Gradientabs.at<Vec3b>(i, j)[c] > 0) {
					src.at<Vec3b>(i, j)[c] = 120;
				}
				else {
					src.at<Vec3b>(i, j)[c] = 0;
				}
			}
		}
	}
	return src;
}
// euclidean distance
double eucleadean_distance(double x1, double x2, double y1, double y2) {
	double z = sqrt(pow(y1 - x1, 2) + pow(y2 - x2, 2));
	return z;
}

// reduce the surface reflection
int ReduceSurfaceReflection(Mat src) {
	Mat HSV;
	cvtColor(src, HSV, COLOR_BGR2HSV);
	const int Hsize = 32;

	int* hist2d = new int[Hsize * Hsize];
	for (int i = 0; i < Hsize; i++) {
		for (int j = 0; j < Hsize; j++) {
			hist2d[i * Hsize + j] = 0;
		}
	}
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			for (int c = 0; c < 3; c++) {
				int h = HSV.at<Vec3b>(i, j)[0];
				int s = HSV.at<Vec3b>(i, j)[1];
				int v = HSV.at<Vec3b>(i, j)[2];
				int H = h / (h + s + v + 1);
				int S = s / (h + s + v + 1);
				int V = v / (h + s + v + 1);
				hist2d[S * Hsize + V]++;
			}
		}
	}


	double min = 0, max = 0;
	minMaxLoc(*hist2d, &min, &max, 0, 0);

	for (int i = 0; i < Hsize; i++) {
		for (int j = 0; j < Hsize; j++) {
			hist2d[i * Hsize + j] = (hist2d[i * Hsize + j] - min) / max;
		}
	}
	for (int i = 0; i < Hsize; i++) {
		for (int j = 0; j < Hsize; j++) {
			for (int c = 0; c < 3; c++) {
				src.at<Vec3b>(i, j)[c] = hist2d[i * Hsize + j] * src.at<Vec3b>(i, j)[c];
				int x = src.at<Vec3b>(i, j)[c];
			}
		}
	}
	return 0;
}