# Real-Time-2D-object-detection
The objective of the project was to develop real time 2-D object recognition system which can recognize a set of objects in a rotation, scale and translation invariant manner. This was achieved using the following steps: 

Image Thresholding: The frame from live video and image dataset were preprocessed to make highly saturated pixels darker to efficiently seperate them from white background and converting pixels with intensity > 120 in grayscale to black to create a binary image containing only the objects  

Cleaning Up: Filled the holes by applying morphological filters like dilate, erode, shrinking by writing grassfire transform to fill the additional holes  

Segmentation and feature computation: Segmented the image into major regions and computed features like centroids, HuMoments(0-7) and stored the values into a database (csv file). The user can select region to detect new object and store them into the database. The database acts like a training set(model)  

Classification: The program now detects major region from live video feed and computes the centroids and HuMoments of the region. These features are now compared with the features stored in the database with the help of different distance metrics like Euclidean distance, mix-max normalization, etc. and the closest matching object is displayed on screen.   The program also computes the confusion matrix for each object detected

//operating System and IDE
Windows 10 and Visual Studio 2022

// Instructions to Run:
1) Make sure the video output is connected, if prompted change the video output from 1 to 0 
2)Enter the directory name of the images of the objects (database) in command line argument
3)Please change string new_image = directory location in your own pc + (rest same) (line 460)[ i forgot to change the code to store in original directory, hence enter manually
2)The program will scan through each image from database and store the features in database.csv file in the parent folder
3) Move the camera on the object you want to detect
4) The object matching closest will be displayed on the output
5) The name on top corner is for sum of difference of humoments
6) The name on bottom corner is KNN of euclidean distance between centroids( far more accurate)

keys to be pressed:

1) key 'p' :
when p is pressed while on the output screen, the console prompt will show the confusion matrix live, the matrix will have updated value of all the objects detected.
(Note : The program does not have feature to not detect any object, so the closest matching object will always be shown, sorry for the inconvinience as time was running out)

2) key 't' : (new object select)
when t is pressed, you can select two points on the "output1" window, you cannot see the points pressed, but they will be stored in the vector
-first point : make sure the first point is the top right corner of the new object you want to store
- second point: make sure the second point is the bottom right corner of the same object
-dont press anyother button except the next one written

3) key 's' (new object save)
- dont press 's' before you press t.. or program will give error for vector subscript
- press 's' as soon as you clicked on two points using key't' and go to command window, enter the name you want to give to the new object and press enter
- the image of object will be saved, and the features will be collected as soon you run the program again


4) link to video:
https://www.youtube.com/user/dandrummer100/videos
