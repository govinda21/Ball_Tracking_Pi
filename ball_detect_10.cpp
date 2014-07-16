/*
########################################################################                                                                 
# Program Name: Pi_Cam_Color_Ball_Tracker.cpp                                     
# ================================     
# This code is for making an autonomous color ball detecter bot.
# The bot can detect and track any color ball for which you have callibrated.
#For callibration you have to run callibration program. There you can easily callibrate for any specific monocolor ball. For more information : refer to the callibration program         
#In this version I have changed the method by which I was detecting the ball. Now I am using Moments for detection folllowed by an if condition. Which moves the bot only when the number of white pixels in the thresholded image is greater than the specified value.                         
#In this program  you can only see the thresholded image instead of original image just to know what is being tracked and changed the name of few variables
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
// Press Esc from your keyboard - To stop the program

/* 
How to compile:

g++ `pkg-config --cflags opencv` line_folower_2.cpp -o test_3 `pkg-config --libs opencv` -I/home/pi/git/robidouille/raspicam_cv -L/home/pi/git/robidouille/raspicam_cv -lraspicamcv -L/home/pi/git/raspberrypi/userland/build/lib -lmmal_core -lmmal -l mmal_util -lvcos -lbcm_host -lrt -lm -L/usr/local/lib -lwiringPi

Replace my_code.cpp by your opencv c++ code. Let's say your file name is john.cpp then replace my_code.cpp to john.cpp
my_code is the executable file, which will be created after the compilation of this code. You can give any name to this executable file while compiling.
*/

/*
How to run:

./mycode color_name

color_name - The name of color for which you have callibrated. This name is completely provided by you while running the callibration program.

*/


#include <iostream>
#include <stdio.h>
#include <vector>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "RaspiCamCV.h"

#include "tick.h"
#include <wiringPi.h>
#include "BrickPi.h"
#include <linux/i2c-dev.h>  
#include <fcntl.h>

using namespace cv;
using namespace std;

// Variables to store the ranges of HSV values
int LowH, HighH;	// Lower and higher limit of H values
int LowS, LowV;	// Lower limit of S and V values
int result,speed_1,speed_2;
int motor1,motor2;

#undef DEBUG

//Move Forward
void fwd()
{
	BrickPi.MotorSpeed[motor1] = speed_1;
	BrickPi.MotorSpeed[motor2] = speed_2;
}

//Move Left
void left()
{
	BrickPi.MotorSpeed[motor1] = speed_1;  
	BrickPi.MotorSpeed[motor2] = -speed_2;
}

//Move Right
void right()
{
	BrickPi.MotorSpeed[motor1] = -speed_1;  
	BrickPi.MotorSpeed[motor2] = speed_2;
}

//Move backward
void back()
{
	BrickPi.MotorSpeed[motor1] = -speed_1;  
	BrickPi.MotorSpeed[motor2] = -speed_2;
}

//Stop
void stop()
{
	BrickPi.MotorSpeed[motor1] = 0;  
	BrickPi.MotorSpeed[motor2] = 0;
}	

void move_bot(float offset, int normal_speed)
{	
	speed_1 = speed_2 = normal_speed;	//Set the normal speed 
	
	if(fabs(offset)>0.4 && fabs(offset)<0.6)	// If the offset is more than 70% on either side from the center of the image
	{
		speed_1=speed_2= 50; //Set the speed to 50
		
		if(offset>0)		// If the x coordinate of the path is on the left side from the center
		{
			speed_2 = 0;		//Stop the left wheel	
			fwd();	//Run forward with left wheel stopped i.e turn left
			//left();	
			BrickPiUpdateValues();	//Update the motor values
			//cout<<"l_0.4"<<endl;
		}
		else
		{	
			speed_1 = 0;	//Stop the right wheel
			fwd();	//Run forward with right wheel stopped i.e turn right
			//right();
			BrickPiUpdateValues();	//Update the motor values
			//cout<<"r_0.4"<<endl;
		}	
	}
	else if(fabs(offset)>0.6 )//&& fabs(offset)<0.6)	// If the offset is more than 70% on either side from the center of the image
	{
		speed_1=speed_2= 60;	//Set the seed to 60
	
		if(offset>0)		// If the x coordinate of the path is on the left side from the center
		{
			speed_2 = 0;		//Stop the left wheel
			fwd();	//Run forward with left wheel stopped i.e turn left	
			//left();	
			BrickPiUpdateValues();	//Update the motor values
			//cout<<"l_0.4"<<endl;
		}
		else
		{	
			speed_1 = 0;		//Stop the right wheel
			fwd();	//Run forward with right wheel stopped i.e turn right
			//right();
			BrickPiUpdateValues();	//Update the motor values
			//cout<<"r_0.4"<<endl;
		
		}	
	}
	
	else	// Move forward with the specified speed by the user
	{		
		fwd();
		BrickPiUpdateValues();	//Update the motor values
		//cout<<"s"<<endl;
	}
	
}

int main(int argc, char** argv)
{
ClearTick();
result = BrickPiSetup();
	// printf("BrickPiSetup: %d\n", result);
	if(result)
		return 0;

	BrickPi.Address[0] = 1;
	BrickPi.Address[1] = 2;

	motor1=PORT_B;	//Select the ports to be used by the motors
	motor2=PORT_C; 

	BrickPi.MotorEnable[motor1] = 1;	//Enable the motors
	BrickPi.MotorEnable[motor2] = 1;

	result = BrickPiSetupSensors();		//Set up the properties of sensors for the BrickPi
	//printf("BrickPiSetupSensors: %d\n", result); 

	BrickPi.Timeout=1000;			//Set timeout value for the time till which to run the motors after the last command is pressed
	BrickPiSetTimeout();				//Set the timeout
	Mat imgOriginal;	// To store images taken from camera in BGR form
	Mat imgHSV;	// To store HSV image converted from imgOriginal
	Mat imgThresholded;	// Thresholded image after lot of processing
	string color_name;	// Color of the ball for which user wants to track the ball
	color_name = argv[1];		// Get the color from the user at command line
	int normal_speed = atoi(argv[2]);	//Set the normal speed 
	
	//vector<Vec3f> circles;	// Store the circle information, if we want to use HoughCircle function for detecting circle
	float offset;	//Offset from the center of the image
	Moments mu;	//To get the moment property of the thresholded image
	
	FileStorage fs;	// A variable to handle the file operation
	
	fs.open("test.yml", FileStorage::READ);	// Open the file with name test.yml in read mode. This test.yml is the same file which has been written by the mouse callibration function 
	
	FileNode n = fs[color_name];	// Since you can callibrate for more than one ball from the callibration program at a single run. So need to specify the color of ball for which you want to detect.
	
	LowH = (int) ( n["LowH"]);	//Read the lower Hue value from the test.yml under the name_color(ther color which you have specified)
	HighH = (int) (n["HighH"]);	//Read the higher Hue value 
	LowS = (int) ( n["LowS"]);	//Read the lower Saturation value
	LowV = (int) ( n["LowV"]);	//Read the lower V value
	
	fs.release();	//close the test.yml file

	RaspiCamCvCapture* camera = raspiCamCvCreateCameraCapture(0);	// Declare a variable to handle the streaming from Raspberry Pi Camera and attaching it to Raspberry Pi camera

	//cout<<"Opening Camera..."<<endl;
	
	// A check  for streaming
	if ( !camera )
	{
		cerr<<"Error opening the camera"<< endl;return -1;
	}
	
	//clock_t t;
	int HighS = 255;	// Higher S value which is initialized to the maximum of it.
	
	int HighV = 255;		// Higher V value which is initialized to the maximum of it.
	
	namedWindow("Original", 0 ); //create a window called "Original"
 
	while( true )	// infinte loop
	{ 	
		//t = clock();
		imgOriginal = raspiCamCvQueryFrame( camera );		// Get the frame from camera

		/*
		Covert color space to HSV as it is much easier to filter colors in the HSV color-space.
		The function prototype is like	
											void cvtColor(Mat src, Mat dst, FLAG)
		src - source image or matrix
		dst - destination matrix in which we want to store the converted image
		FLAG - To tell from which color to which color conversion has to taken place
					Ex -	COLOR_BGR2GRAY : Will convert the BGR color to gray. Normal image taken from camera will be in BGR color.
							COLOR_BGR2HSV	: Will convert the BGR color toHSV. You can also convert the image from other color format to any other color format.
																Just you need to change the FLAG like COLOR_colorA2colorB. Where colorA and colorB can be any color format.
		*/
		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		/*
		inRange is a built in function in opencv which checks the each pixel values in the image. If the the pixel value is lying inside the specified interval, tehn it make that to white color.
		void inRange(InputArray src, InputArray lowerb, InputArray upperb, OutputArray dst)
		Parameters:	
			src – first input array.
			lowerb – inclusive lower boundary array or a scalar.
			upperb – inclusive upper boundary array or a scalar.
			dst – output array of the same size as src and CV_8U type.
			The function checks the range as follows:

		For every element of a single-channel input array:
			dst = lowerb<= src <=upperb
		For two-channel arrays:
			dst = (lowerb_channel_1<= src_channel_1 <=upperb_channel_1)AND(lowerb_channel_2<= src_channel_2 <=upperb_channel_2)
		
		and so forth.

		That is dst is set to 255 (all 1 -bits) if src  is within the specified 1D, 2D, 3D, ... box and 0 otherwise.
		*/
		inRange(imgHSV, Scalar(LowH, LowS, LowV), Scalar(HighH, HighS, HighV), imgThresholded); //Threshold the image

		//morphological opening (remove small objects from the foreground). It is obtained by the erosion of an image followed by a dilation.		
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
		dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

		//morphological closing (fill small holes in the foreground). Useful to remove small holes (dark regions)
		dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
		   
		/*
		hough detector works better with some smoothing of the image
		Smoothing the imgThreshold with GaussianBlur to reduce noise
		The most useful filter (although not the fastest). Gaussian filtering is done by convolving each point in the input array with a Gaussian kernel and then summing them all to produce the output array.
		GaussianBlur(src, dst, Size( i, i ), 0, 0 )
		src: Source image
		dst: Destination image
		Size(w, h): The size of the kernel to be used (the neighbors to be considered). w and h have to be odd and positive numbers otherwise thi size will be calculated using the \sigma_{x} and \sigma_{y} arguments.
		sigma_{x}: The standard deviation in x. Writing 0 implies that \sigma_{x} is calculated using kernel size.
		sigma_{y}: The standard deviation in y. Writing 0 implies that \sigma_{y} is calculated using kernel size.
		*/
		GaussianBlur( imgThresholded, imgThresholded, Size(3, 3), 2, 2 );
		
		/*
		Moment is class to store some information about the pixel
		values, from which we can easily get the center of white
		pixels.
		Inside the Moments definition:
		m00 - total no. of white pixels
		m10 - sum of x coordinate where pixels are white
		m01 - sum of y coordinate where pixels are white
		moments(InputArray array, bool binaryImage) is a function 	which returns Moments.
		*/
		
		mu = moments(imgThresholded,true);	//Get the moment property of the thresholded image
		
		Point2f center(mu.m10/mu.m00, mu.m01/mu.m00);	//Calculate center of the white pixels in the imgThresholded image
		//circle( imgOriginal, center, 5, Scalar(0,0,255), -1, 8, 0 );
		
		if(mu.m00 > 4000)	//Here 4000 is the number of white pixels. Since noise will always be there. Therefore we are moving the bot when some reasonable part of the ball gets detected.
		{
			offset = (1-2*center.x/imgOriginal.cols);	// Calculate the offset from the center of the image of the center of the center of the white pixels
			
			move_bot(offset, normal_speed);	//Call the move_bot function which moves the bot with the specified speed
		}
		else
		{
			stop();	// Stop the bot
			BrickPiUpdateValues();	// Update the motor values
		}
		
		//t = clock()-t;
		//cout<<((float)t)/CLOCKS_PER_SEC<<endl; 
		
		imshow("Original", imgThresholded); //show the thresholded image
		//imshow("Original", imgOriginal); 	//show the original image
		
		// Press ESc to stop this program
		if( waitKey(1)== 27 ) 
			break;
	}
	
	raspiCamCvReleaseCapture(&camera);	// Close the the streaming.
	
	destroyWindow("Original");	//Close the window on which images were getting shown
	
	return 0;

}