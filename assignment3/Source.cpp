//#include <iostream>
//#include <cstring>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc.hpp>
//#include <fstream>
//
//#include <ctime>
//
//using namespace std;
//using namespace cv;
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//int SobelX3x3(Mat& src, Mat3s& dst)
//{
//	/* Declaring a 3 channel signed Variable*/
//	Mat3s temp;
//	/* Storing image data from source to destination*/
//	src.copyTo(temp);
//	/*Declaring Variables for channels, rows and columns*/
//	int c, i, j;
//	/*looping to acces each channel in each pixel*/
//	for (c = 0; c < 3; c++)
//	{
//		for (i = 0; i < src.rows - 1; i++)
//		{
//			for (j = 1; j < src.cols - 1; j++)
//			{
//				/* Appying [1 2 1] Filter to each column*/
//				temp.at<Vec3s>(i, j)[c] = src.at<Vec3b>(i, j - 1)[c] * 1 + src.at<Vec3b>(i, j)[c] * 2 + src.at<Vec3b>(i, j + 1)[c] * 1;
//				/*Dividing output by summation of all constants in filter to bring result back to 256bits*/
//				temp.at<Vec3s>(i, j)[c] /= 4;
//			}
//		}
//	}
//	for (c = 0; c < 3; c++)
//	{
//		for (i = 1; i < src.rows - 1; i++)
//		{
//			for (j = 0; j < src.cols - 1; j++)
//			{
//				/* Appying [-1 0 1] Filter to each row and storing the final result in destination variable*/
//				dst.at<Vec3s>(i, j)[c] = temp.at<Vec3s>(i - 1, j)[c] * -1 + (temp.at<Vec3s>(i, j)[c] * 0) + temp.at<Vec3s>(i + 1, j)[c];
//			}
//		}
//	}
//	return 0;
//}
//
///*Ysobel*/
///* Same as X-sobel but applying [1 2 1] to rows instead of column and [-1 0 1] to columns*/
//int SobelY3x3(Mat& src, Mat3s& dst)
//{
//	Mat3s temp;
//	src.copyTo(temp);
//	int c, i, j;
//	for (c = 0; c < 3; c++)
//	{
//		for (i = 0; i < src.rows - 1; i++)
//		{
//			for (j = 1; j < src.cols - 1; j++)
//			{
//
//				temp.at<Vec3s>(i, j)[c] = src.at<Vec3b>(i, j - 1)[c] * -1 + (src.at<Vec3b>(i, j)[c] * 0) + src.at<Vec3b>(i, j + 1)[c] * 1;
//
//			}
//		}
//	}
//	for (c = 0; c < 3; c++)
//	{
//		for (i = 1; i < src.rows - 1; i++)
//		{
//			for (j = 0; j < src.cols - 1; j++)
//			{
//				dst.at<Vec3s>(i, j)[c] = temp.at<Vec3s>(i - 1, j)[c] * 1 + (temp.at<Vec3s>(i, j)[c] * 2) + temp.at<Vec3s>(i + 1, j)[c];
//				temp.at<Vec3s>(i, j)[c] /= 4;
//			}
//		}
//	}
//	return 0;
//}
//
//
///*Gradient Magnitude*/
//int Gradient_magnitude(Mat3s sobelx, Mat3s sobely, Mat3s& dst) {
//	/*Declaring variables for rows, columsn and channel*/
//	int c, i, j;
//	/*Looping to acces each channel of each pixel*/
//	for (i = 0; i < dst.rows; i++) {
//		for (j = 0; j < dst.cols; j++) {
//			for (c = 0; c < 3; c++) {
//				/*assigning each channel in destiantion new values by using the formula sqrt(xs^2+xy^2)*/
//				dst.at<Vec3s>(i, j)[c] = sqrt((sobelx.at<Vec3s>(i, j)[c] * sobelx.at<Vec3s>(i, j)[c]) + (sobely.at<Vec3s>(i, j)[c] * sobely.at<Vec3s>(i, j)[c]));
//				/*int x = dst.at<Vec3s>(i, j)[c];
//				cout << x << endl;*/
//			}
//		}
//	}
//	return 0;
//}
//
//float euclidian_distance(float x, float y) {
//	float z = sqrt((x-y)*(x-y));
//	return z;
//}
//
//
//int main(int argc, int argv[]) {
//	
//	ofstream myFile;
//	VideoCapture Cap(0);
//	if (!Cap.isOpened()) {
//		cerr << "The Camera Failed to Open";
//		return -1;
//	}
//	while (Cap.isOpened() == true) {
//		Mat Image;
//		/*Cap.read(Image);*/
//		Image = imread("C:\\Utkarsh\\New folder\\assignment3\\Resources\\2.jpg");
//		
//		Mat ImageBlur;
//		GaussianBlur(Image, ImageBlur, Size(3, 3), 0);
//		
//		
//	
//		
//		Mat ImageGray;
//		cvtColor(ImageBlur, ImageGray, COLOR_BGR2GRAY);
//		
//		Mat ImageHSV;
//		cvtColor(ImageBlur, ImageHSV, COLOR_BGR2HSV);
//		
//		
//		
//		
//		Mat3s Sobelx;
//		ImageHSV.copyTo(Sobelx);
//
//		Mat3s Sobely;
//		ImageHSV.copyTo(Sobely);
//
//		Mat3s Gradient;
//		ImageHSV.copyTo(Gradient);
//		Mat Gradient1;
//
//
//		SobelX3x3(ImageHSV, Sobelx);
//		SobelY3x3(ImageHSV, Sobely);
//		Gradient_magnitude(Sobelx, Sobely, Gradient);
//		/*for (int i = 0; i < Gradient.rows; i++) {
//			for (int j = 0; j < Gradient.cols; j++) {
//				if (Gradient.at<Vec3s>(i, j)[0] + Gradient.at<Vec3s>(i, j)[1] + Gradient.at<Vec3s>(i, j)[2] < 100) 
//				{
//					Gradient.at<Vec3s>(i, j)[0] = 0;
//					Gradient.at<Vec3s>(i, j)[1] = 0;
//					Gradient.at<Vec3s>(i, j)[2] = 0;
//				}
//				else
//				{
//					Gradient.at<Vec3s>(i, j)[0] = 255;
//					Gradient.at<Vec3s>(i, j)[1] = 255;
//					Gradient.at<Vec3s>(i, j)[2] = 255;
//				}
//				}
//		}*/
//		vector<pair<float, float>> cluster1;
//		vector<pair<float, float>> cluster2;
//		
//			// cluster1
//			int x_mean = 0;
//			int y_mean = 0;
//			int count = 0;
//			int c1=100;
//			int c2=100;
//
//			//cluster2
//			
//			int countt = 0;
//			while (countt < 2) {
//				
//			for (int i = 0; i < Gradient.rows; i++) {
//				for (int j = 0; j < Gradient.cols; j++) {
//					
//					float x = Gradient.at<Vec3s>(c1, c2)[0];
//					float y = Gradient.at<Vec3s>(i, j)[0];
//					float z = Gradient.at<Vec3s>(Gradient.rows - c1, Gradient.cols - c2)[0];
//
//					int a = euclidian_distance(y, x);
//					int b = euclidian_distance(y, z);
//
//					if (a < b) {
//						cluster1.emplace_back(i, j);
//
//						x_mean += i;
//						y_mean += j;
//						count++;
//					}
//					else {
//						cluster2.emplace_back(i, j);
//					}
//				}
//			}
//			
//			c1 = x_mean / count;
//			c2 = y_mean / count;
//			cout << c1 << c2 << endl;
//		/*	for (int i = 0; i < cluster1.size(); i++)
//			{
//				if (Gradient.at<Vec3s>(cluster1[i].first, cluster1[i].second)[0] = new_mean_clus1) {
//					c1 = cluster1[i].first;
//					c2 = cluster1[i].second;
//					
//				}
//
//			}*/
//			
//			countt++;
//		}
//		
//		Gradient.copyTo(Gradient1);
//		convertScaleAbs(Gradient, Gradient1);
//	
//				for (int k = 0; k < cluster1.size(); k++) {
//						Gradient1.at<Vec3b>(cluster1[k].first, cluster1[k].second)[0] = 255;
//						Gradient1.at<Vec3b>(cluster1[k].first, cluster1[k].second)[1] = 255;
//						Gradient1.at<Vec3b>(cluster1[k].first, cluster1[k].second)[2] = 255;
//					}
//			
//				for (int k = 0; k < cluster2.size(); k++) {
//					Gradient1.at<Vec3b>(cluster2[k].first, cluster2[k].second)[0] = 0;
//					Gradient1.at<Vec3b>(cluster2[k].first, cluster2[k].second)[1] = 0;
//					Gradient1.at<Vec3b>(cluster2[k].first, cluster2[k].second)[2] = 0;
//				}
//		
//
//		
//		
//		imshow("Output", Gradient1);
//		waitKey(1);
//		/*char key = waitKey(0);
//		if (key == 'q') {
//			break;
//		}*/
//	}
//	
//}


//int surfaceReflection(Mat src) {
//	Mat HSV;
//	cvtColor(src, HSV, COLOR_BGR2HSV);
//	const int Hsize = 32;
//	int dim[2] = { Hsize, Hsize };
//	Mat hist2d;
//	hist2d = Mat::zeros(2, dim, CV_16U);
//	for (int i = 0; i < src.rows; i++) {
//		for (int j = 0; j < src.cols; j++) {
//			for (int c = 0; c < 3; c++) {
//				int h = HSV.at<Vec3b>(i, j)[0];
//				int s = HSV.at<Vec3b>(i, j)[1];
//				int v = HSV.at<Vec3b>(i, j)[2];
//				int H = h / (h + s + v + 1);
//				int S = s / (h + s + v + 1);
//				int V = v / (h + s + v + 1);
//				hist2d.at<unsigned int>(S, V)++;
//			}
//		}
//	}
//	int mini = 0;
//	int maxi = 1;
//	for (int i = 0; i < Hsize; i++) {
//		for (int j = 0; j < Hsize; j++) {
//			if (hist2d.at<unsigned int>(i, j) < hist2d.at<unsigned int>(i + 1, j + 1)) {
//				mini = hist2d.at<unsigned int>(i, j);
//			}
//			else {
//				maxi = hist2d.at<unsigned int>(i + 1, j + 1);
//			}
//		}
//	}
//	for (int i = 0; i < Hsize; i++) {
//		for (int j = 0; j < Hsize; j++) {
//			hist2d.at<unsigned int>(i, j) = (1 - mini) / maxi;
//		}
//	}
//	for (int i = 0; i < Hsize; i++) {
//		for (int j = 0; j < Hsize; j++) {
//			for (int c = 0; c < 3; c++) {
//				src.at<Vec3b>(i, j)[c] = hist2d.at<unsigned int>(i, j) * src.at<Vec3b>(i, j)[c];
//			}
//		}
//	}
//	return 0;
//}