/*
########################################################################                                                                 
# Program Name: Pi_Cam_Color_Ball_Picker.cpp                                     
# ================================     
# This code is to callibrate an autonomous color ball detector bot.
# The program can  callibrate many balls at a single run.
#Once you right clicked your mouse button on the ball, then you have specify some name for that ball in control panel.                            
# http://www.dexterindustries.com/                                                                
# History
# ------------------------------------------------
# Author       Date      Comments
# Govinda      09/07/14  Initial Authoring
#                                                                  
# These files have been made available online through a Creative Commons Attribution-ShareAlike 3.0  license.
# (http://creativecommons.org/licenses/by-sa/3.0/)           
#
########################################################################
*/

// Command:
// y  - select the image for callibration.
// Right click of the mouse to selest the pixel value for callibration.
//  Esc  - To stop the program

/* 
How to compile:

g++ `pkg-config --cflags opencv` my_code.cpp -o my_code `pkg-config --libs opencv` -I/home/pi/git/robidouille/raspicam_cv -L/home/pi/git/robidouille/raspicam_cv -lraspicamcv -L/home/pi/git/raspberrypi/userland/build/lib -lmmal_core -lmmal -l mmal_util -lvcos -lbcm_host

Replace my_code.cpp by your opencv c++ code. Let's say your file name is john.cpp then replace my_code.cpp to john.cpp
my_code is the executable file, which will be created after the compilation of this code. You can give any name to this executable file while compiling.
*/

/*
How to run:

./mycode color_name

color_name - The name of color for which you have callibrated. This name is completely provided by you while running the callibration program.

*/
#include <iostream>
#include <string>
#include <vector>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "RaspiCamCV.h"

using namespace cv;
using namespace std;

// Variables to store the ranges of HSV values
int LowH, HighH;	// Lower and higher limit of H values
int LowS, LowV;	// Lower limit of S and V values
bool assigned = false;	// Just to ensure that HSV value will be assigned for the specific color 

/*
onMouse - Name of the callback function. Whenever mouse events related to the above window occur, this callback function will be called. This function should have the signature like the following
void FunctionName(int event, int x, int y, int flags, void* userdata)
event - Type of the mouse event. These are the entire list of mouse events
	EVENT_MOUSEMOVE
	EVENT_LBUTTONDOWN
	EVENT_RBUTTONDOWN
	EVENT_MBUTTONDOWN
	EVENT_LBUTTONUP
	EVENT_RBUTTONUP
	EVENT_MBUTTONUP
	EVENT_LBUTTONDBLCLK
	EVENT_RBUTTONDBLCLK
	EVENT_MBUTTONDBLCLK
x - x coordinate of the mouse event
y - y coordinate of the mouse event
flags - Specific condition whenever a mouse event occurs. See the next OpenCV example code for the usage of this parameter. Here is the entire list of enum values which will be possesed by "flags".
			This can help us in using the same event for different purpose. Like we can use right click of mouse + Alt key and many more depending on your use. 
	EVENT_FLAG_LBUTTON
	EVENT_FLAG_RBUTTON
	EVENT_FLAG_MBUTTON
	EVENT_FLAG_CTRLKEY
	EVENT_FLAG_SHIFTKEY
	EVENT_FLAG_ALTKEY
userdata - Any pointer passes to the "setMouseCallback" function as the 3rd parameter. Here I am taking an image matrix.
*/
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if  ( event == EVENT_RBUTTONDOWN )	// If right mouse button has been clicked 
    {
		cout<<"R_Clicked"<<endl;
		
		Mat frame = *((Mat*)userdata);	// Since the pointer is of void data type. Convert it to Mat* then use the dereferencing operator to get the data.
		Mat HSV;	//To store HSV image
		Mat BGR = frame( Rect( x, y, 1, 1 ) ); // Create a ROI with the position x, y are as specified by the mouse pointer while clicking from the image. The BGR matrix is of 1*1. 
		cvtColor( BGR, HSV, COLOR_BGR2HSV);	// Convert the BGR color to HSV color. It is similar to what we do it on the image instead of doing on 1 by 1  matrix.
		
		Vec3b hsv=HSV.at<Vec3b>(0,0);	// To store the HSV values from the HSV matrix
		
		int H=hsv.val[0]; // Get the hue value
		int S=hsv.val[1]; // Get the saturation
		int V=hsv.val[2]; // Get the V
		
		LowH = H - 3;	// Lower range for hue value
		HighH = H + 3;	// Higher range for hue value
		
		LowS = (S - 0.3*255);	// Set the lower range for saturation value as 30% less than the detected S. This is because all the pixels will not be having the same S value.
		LowV = (V - 0.3*255);	// Set the lower range for V value as 30% less than the detected V. This is because all the pixels will not be having the same V value.
		assigned = true;	// This is just to say that intervals for the HSV valuse has been created.
		
    }
}


int main(int argc, char** argv)
{
	Mat imgOriginal;	// To store images taken from camera in BGR form
	Mat imgHSV;	// To store HSV image converted from imgOriginal
	Mat imgThresholded;	// Thresholded image after lot of processing
	
	string color_name;		// To get the color name from user while callibration.
	RaspiCamCvCapture* camera = raspiCamCvCreateCameraCapture(0);	// Declare a variable to handle the streaming from Raspberry Pi Camera

	cout<<"Opening Camera..."<<endl;
	// A check  for streaming
	if ( !camera )
	{
		cerr<<"Error opening the camera"<< endl;return -1;
	}
	//cout<<"If you feel the image is good for the callibration then hit any key"<<endl<<flush;
	
	namedWindow("Original", 0/*CV_WINDOW_AUTOSIZE*/); //create a window called "Control"
	
	// This loop is just to avoid any bad quality image taken from Raspberry Pi camera in the beginning of streaming.
	// Once the image has been selected. We can go for callibration.
	while(true)	// Infinte loop
	{
		imgOriginal = raspiCamCvQueryFrame( camera );	// Get the frame from which is captured by camera
		
		//imshow("Original", imgThresholded); //show the thresholded image
		imshow("Original", imgOriginal); //show the original image
		
		if(waitKey(1)==121) 
		{
			break;	// Wait for 1 milli second. If any key is hit then take this image for callibration.
		}
	}
	
	
	FileStorage fs("test.yml", FileStorage::WRITE);	// Create a file test.yml where we HSV values has to be stored

	while( true )	// Infinte loop
	{			
		//imshow("Original", imgThresholded); //show the thresholded image
		imshow("Original", imgOriginal); //show the original image
		
		/*
		void setMouseCallback(const string& winname, MouseCallback onMouse, void* userdata = 0)

		This function sets a callback function to be called every time any mouse events occurs in the specified window. Here is the detailed explanation of the each parameters of the above OpenCV function.
			winname - Name of the OpenCV window. All mouse events related to this window will be registered
			onMouse - Name of the callback function. Whenever mouse events related to the above window occur, this callback function will be called.
			userdata - This pointer will be passed to the callback function
		*/
		//set the callback function for any mouse event
		setMouseCallback("Original", CallBackFunc, &imgOriginal);  
		
		if(assigned)	// Just to ensure the right click of mouse from the user
		{
			cout<<"Enter the color name:"<<flush;
			cin>>color_name;	// Get the name of color
			fs<<color_name<<"{:";
			fs<<"LowH"<<LowH;		// Write the lower Hue value in the test.yml under the name_color(ther color which you have specified)
			fs<<"HighH"<<HighH;	// Write the higher Hue value in the test.yml under the name_color(ther color which you have specified)
			fs<<"LowS"<<LowS;		// Write the lower S value in the test.yml under the name_color(ther color which you have specified)
			fs<<"LowV"<<LowV;		// Write the lower V value in the test.yml under the name_color(ther color which you have specified)
			fs<<"}";
			
			cout<<"done"<<endl;		// Print the confirmation of the callibration of the spe
			assigned = false;
		}
		
		// If ESC key pressed
		if( waitKey(1)== 27 ) 
			break;
	}

	destroyWindow("Original");	//Close the window on which images were getting shown	
	raspiCamCvReleaseCapture(&camera);	// Close the the streaming.
	fs.release();	//close the test.yml file
	
	return 0;

}
