#include <iostream>
#include <cstring>
#include <dirent.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <fstream>
#include "Header.h"
#include <algorithm>


using namespace std;
using namespace cv;

// define a vector of points to store cordinate selected by left mouse click
vector<Point> points;
// function lets user select co-ordinate from left mouse click
void onMouse(int action, int x, int y, int, void*) {
	int count = 0;
	if (action == cv::EVENT_LBUTTONDOWN) {
		points.push_back(cv::Point{ x, y });
		count++;
	}
	else if (count > 1) {
		points.clear();
	}
}





int main(int argc, char* argv[]) {
	// declare all variables to get directory, get images, store images, 

	VideoCapture cap(1);
	ifstream rfile;
	Mat Image;





	char dirname[256];
	char buffer[256];
	struct dirent* dp;

	vector<string> filename;
	DIR* dirp;


	// vector to store filename without path
	vector<string> displayname;

	// vector to store confusion matrix 
	vector<pair<string, int>> confusion_matrix;

	// directory
	if (argc < 2)
	{
		printf("usage: %s<directory path>\n", argv[0]);
		exit(-1);
	}

	strcpy_s(dirname, argv[1]);


	printf("Processing directory %s\n", dirname);

	dirp = opendir(dirname);
	if (dirp == NULL) {
		printf("Cannot open directory %s\n", dirname);
		exit(-1);
	}
	// loop over all the files in the image file listing
	while ((dp = readdir(dirp)) != NULL) {
		// check if the file is an image
		if (strstr(dp->d_name, ".jpg") ||
			strstr(dp->d_name, ".png") ||
			strstr(dp->d_name, ".ppm") ||
			strstr(dp->d_name, ".tif")) {
			printf("processing image file: %s\n", dp->d_name);
			// build the overall filename
			displayname.push_back(dp->d_name);
			strcpy_s(buffer, dirname);
			strcat_s(buffer, "\\");
			strcat_s(buffer, dp->d_name);
			printf("full path name: %s\n", buffer);
			filename.push_back(buffer);
		}
	}



	// write a CSV file named database to store database
	ofstream myFile;
	myFile.open("Database.csv");


	for (int i = 0; i < filename.size(); i++)
	{
		Image = imread(filename[i]);
		myFile << filename[i];


		//custom filters to do morphological operations

		GaussianBlur(Image, Image, Size(5, 5), 2);
		ReduceSurfaceReflection(Image);
		Saturation_filter(Image);
		EdgeSharpening(Image);

		// closing extra holes
		dilate(Image, Image, 0, Point(-1, -1), 10);
		erode(Image, Image, 0, Point(-1, -1), 2);





		Mat Mask;
		// used inrange to convert image to 8UC3
		inRange(Image, Scalar(255, 255, 255), Scalar(255, 255, 255), Mask);

		Mat labels, stats, centroid;
		const int connectivity = 8;

		// used connectedcomponentswithstats to get area, centroid and location of connected componenents
		int nlabels = connectedComponentsWithStats(Mask, labels, stats, centroid, connectivity, CV_32S);
		confusion_matrix.push_back(pair<string, int>(filename[i], 0));






		// calculated 7 humoments and updated the csv file
		Moments moment = moments(Mask, false);
		double huMoments[7];
		HuMoments(moment, huMoments);
		for (int i = 0; i < 7; i++) {
			huMoments[i] = -1 * copysign(1.0, huMoments[i]) * log10(abs(huMoments[i]));
			myFile << ',' << huMoments[i];
		}

		//declared vector to store location
		vector<pair<int, int>> lefttop_location;
		vector<pair<int, int>> rightbot_location;
		vector<int> area;



		// looped through nlables ignoring nlabels=0 as it represents the background
		for (int i = 1; i < nlabels; i++) {
			int firstcol = stats.at<int>(i, CC_STAT_LEFT);
			int firstrow = stats.at<int>(i, CC_STAT_TOP);
			int width = stats.at<int>(i, CC_STAT_WIDTH);
			int height = stats.at<int>(i, CC_STAT_HEIGHT);
			int lastrow = stats.at<int>(i, CC_STAT_TOP) + stats.at<int>(i, CC_STAT_HEIGHT);
			int lastcolumn = stats.at<int>(i, CC_STAT_LEFT) + stats.at<int>(i, CC_STAT_WIDTH);
			int area2 = stats.at<int>(i, CC_STAT_AREA);
			lefttop_location.push_back(pair<int, int>(firstrow, firstcol));
			rightbot_location.push_back(pair<int, int>(lastrow, lastcolumn));
			area.push_back(area2);

		}

		//declared a image to store data of all major regions
		Mat cropped_image_src;
		int max_area_src = *max_element(area.begin(), area.end());

		//looped through all regions in area vector and assigned the biggest area to the cropped image.
		for (int u = 0; u < area.size(); u++)
		{
			if (area[u] == max_area_src) {

				cropped_image_src = Mask(Range(lefttop_location[u].first, rightbot_location[u].first), Range(lefttop_location[u].second, rightbot_location[u].second));
			}
		}
		Moments cm0 = moments(cropped_image_src, true);


		// stored centroid information in vector and then in the CSV file
		Point_<double> P0 = Point_<double>(cm0.m10 / cm0.m00, cm0.m01 / cm0.m00);

		myFile << ',' << P0.x << ',' << P0.y;




		myFile << endl;
	}





	myFile.close();



	if (!cap.isOpened()) {
		cout << "The Video Camera was not opened" << endl;
		return -1;
	}


	while (cap.isOpened()) {

		
		

		Mat Image1;
		// image1 reads input from video
		cap.read(Image1);
		
		//	// userimage stores image selected by user

		Mat userimage;

		//image purely to display
		Mat Image2;
		Image1.copyTo(Image2);

		// same filters
		GaussianBlur(Image1, Image1, Size(5, 5), 2);
		ReduceSurfaceReflection(Image1);
		Saturation_filter(Image1);
		EdgeSharpening(Image1);

		// morphological operations
		dilate(Image1, Image1, 0, Point(-1, -1), 10);
		erode(Image1, Image1, 0, Point(-1, -1), 2);





		Mat Mask1;
		// converts to 8UC3
		inRange(Image1, Scalar(255, 255, 255), Scalar(255, 255, 255), Mask1);



		Mat labels1, stats1, centroid1;
		const int connectivity1 = 8;

		// connectedcomponents with stats
		int nlabels1 = connectedComponentsWithStats(Mask1, labels1, stats1, centroid1, connectivity1, CV_32S);
		Mat colored_mask;
		Image1.copyTo(colored_mask);

		// vectors to store top most and bottomost rows and column information
		vector<pair<int, int>> lefttop_location1;
		vector<pair<int, int>> rightbot_location1;

		// vector to store area of all detected components
		vector<int> area1;



		//vector which generates random colors
		vector<Vec3b> randomcolors(nlabels1);
		for (int u = 0; u < nlabels1 - 1; u++) {
			randomcolors[u] = Vec3b(rand() % 255, rand() % 255, rand() % 255);
		}

		// looping through all connected components to get location, area and centroid information
		for (int i = 1; i < nlabels1; i++) {
			int firstcol = stats1.at<int>(i, CC_STAT_LEFT);
			int firstrow = stats1.at<int>(i, CC_STAT_TOP);
			int width = stats1.at<int>(i, CC_STAT_WIDTH);
			int height = stats1.at<int>(i, CC_STAT_HEIGHT);
			int lastrow = stats1.at<int>(i, CC_STAT_TOP) + stats1.at<int>(i, CC_STAT_HEIGHT);
			int lastcolumn = stats1.at<int>(i, CC_STAT_LEFT) + stats1.at<int>(i, CC_STAT_WIDTH);
			int area2 = stats1.at<int>(i, CC_STAT_AREA);
			lefttop_location1.push_back(pair<int, int>(firstrow, firstcol));
			rightbot_location1.push_back(pair<int, int>(lastrow, lastcolumn));
			area1.push_back(area2);
		}



		// displaying connected components in random color
		for (int u = 0; u < nlabels1 - 1; u++) {
			for (int i = lefttop_location1[u].first; i < rightbot_location1[u].first; i++) {
				for (int j = lefttop_location1[u].second; j < rightbot_location1[u].second; j++) {
					if (colored_mask.at<Vec3b>(i, j) == Vec3b(255, 255, 255)) {
						colored_mask.at<Vec3b>(i, j) = randomcolors[u];
					}

				}
			}
		}

		// storing biggest area component in an image 
		Mat cropped_image;
		int max_area = *max_element(area1.begin(), area1.end());
		int b;
		for (int u = 0; u < area1.size(); u++)
		{
			if (area1[u] == max_area) {
				b = u;
				cropped_image = Mask1(Range(lefttop_location1[u].first, rightbot_location1[u].first), Range(lefttop_location1[u].second, rightbot_location1[u].second));

			}
		}
		// calculating Hu moments

		Moments moment2 = moments(cropped_image, false);
		double huMoments2[7];
		HuMoments(moment2, huMoments2);
		for (int i = 0; i < 7; i++) {
			huMoments2[i] = -1 * copysign(1.0, huMoments2[i]) * log10(abs(huMoments2[i]));
		}



		Moments cm = moments(cropped_image, true);

		//P1 stores centroid of each cropped image
		Point_<double> P1 = Point_<double>(cm.m10 / cm.m00, cm.m01 / cm.m00);




		// open the database file where image information is stored
		rfile.open("database.csv");

		// vectors which store sum of difference of moments in between database image and image currently in the frame
		vector<double> Sum_of_Moments;

		// vector to store KNN
		vector<double> KNN;
		// vector to store objectname
		vector<string> list_objectname;

		// vector to store humoments of images in database csv file
		double compare_humoments[7];
		string objectname;
		string m1, m2, m3, m4, m5, m6, m7, m8, m9;
		// looping through csv file
		while (getline(rfile, objectname, ','))
		{
			list_objectname.push_back(objectname);
			double Sum = 0;
			getline(rfile, m1, ',');
			getline(rfile, m2, ',');
			getline(rfile, m3, ',');
			getline(rfile, m4, ',');
			getline(rfile, m5, ',');
			getline(rfile, m6, ',');
			getline(rfile, m7, ',');
			getline(rfile, m8, ',');
			getline(rfile, m9, '\n');

			int n1 = 0, n2 = 0, n3 = 0, n4 = 0, n5 = 0, n6 = 0, n7 = 0, n8 = 0, n9 = 0;
			stringstream temp1(m1);
			temp1 >> n1;
			compare_humoments[0] = n1;

			stringstream temp2(m1);
			temp2 >> n2;
			compare_humoments[1] = n2;

			stringstream temp3(m3);
			temp3 >> n3;
			compare_humoments[2] = n3;

			stringstream temp4(m4);
			temp4 >> n4;
			compare_humoments[3] = n4;

			stringstream temp5(m5);
			temp5 >> n5;
			compare_humoments[4] = n5;

			stringstream temp6(m6);
			temp6 >> n6;
			compare_humoments[5] = n6;

			stringstream temp7(m7);
			temp7 >> n7;
			compare_humoments[6] = n7;

			stringstream temp8(m8);
			temp8 >> n8;

			stringstream temp9(m9);
			temp9 >> n9;

			// storing eucleadean distance between centroids of cropped image and original image components
			int d = eucleadean_distance(P1.x, P1.y, n8, n9);
			KNN.push_back(d);

			// absolute difference between all 7 moments and their sum
			for (int i = 0; i < 6; i++) {
				double z = abs(huMoments2[i] - compare_humoments[i]);

				Sum += z;
			}

			Sum_of_Moments.push_back(Sum);



		}
		rfile.close();


		// minimum distance and its index between sum of difference of all 7 moments
		double min_moment = *min_element(Sum_of_Moments.begin(), Sum_of_Moments.end());
		int min_moment_index = min_element(Sum_of_Moments.begin(), Sum_of_Moments.end()) - Sum_of_Moments.begin();

		// minimum distance and its index between centroids of each image
		double min_centroid = *min_element(KNN.begin(), KNN.end());
		int min_centroid_index = min_element(KNN.begin(), KNN.end()) - KNN.begin();

		// note: KNN vector stores all closest image... can output the first n number of images anytime





		// bounded rectangle and text information
		for (int u = 0; u < area1.size(); u++)
		{
			if (area1[u] == max_area) {
				rectangle(Image2, Point(lefttop_location1[u].second, lefttop_location1[u].first), Point(rightbot_location1[u].second, rightbot_location1[u].first), Scalar(120, 25, 50), 2);
				putText(Image2, displayname[min_moment_index].substr(0, displayname[min_moment_index].find(".")), Point(lefttop_location1[u].second, lefttop_location1[u].first - 10), FONT_HERSHEY_SIMPLEX, 0.9, (120, 25, 50), 2);
				putText(Image2, displayname[min_centroid_index].substr(0, displayname[min_centroid_index].find(".")), Point(rightbot_location1[u].second, rightbot_location1[u].first - 10), FONT_HERSHEY_SIMPLEX, 0.9, (255, 0, 0), 2);
			}
		}

		// updating confusion matrix for each image detected
		for (int i = 0; i < confusion_matrix.size(); i++) {
			if (confusion_matrix[i].first == list_objectname[min_moment_index]) {
				confusion_matrix[i].second++;
			}
		}

		/*imshow("output", Image1);*/
		imshow("output1", Image2);

		char key = waitKey(1);

		// user can select first top and last bottom co-ordinate of the frame to be stored in database
		if (key == 't') {
			setMouseCallback("output1", onMouse);


		}
		// user can store the image in database if 't' key is pressed note: input object name when prompted
		if (key == 's') {
			userimage = Image2(Range(points[0].y, points[1].y), Range(points[0].x, points[1].x));
			string give_object_name;
			cout << "please enter the name of object you want to store in database";
			cin >> give_object_name;
			string new_image = "C:\\Utkarsh\\object3resource\\" + give_object_name + ".jpg";
			imwrite(new_image, userimage);
			imshow("selected object", userimage);
		}

		// shows colored contours	
		imshow("colored_contours", colored_mask);
		
		

		

		if (key == 'q') {
			break;
		}

		// displays confusion matrix if 'p' is pressed
		if (key == 'p') {
			for (int i = 0; i < confusion_matrix.size(); i++) {
				cout << confusion_matrix[i].first << "  " << confusion_matrix[i].second << endl;
			}
		}

	}
}
