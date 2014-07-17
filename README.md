Ball Tracker Pi
================

This project will help you in tracking color ball. In this project we will be using Raspberry Pi and Brick Pi. This project is to make an autonomous color ball tracker robot using the technique of image processing. It is a basic project in opencv. This document will guide you from the installation of opencv on your Pi to the completion of a color ball tracker robot.

Components required for this project are
- Raspberry Pi
- Raspberry Pi camera
- Brick Pi
- Two lego motors
- Wifi adapter 
- 8 GB SD card
- Power cable for your Raspberry Pi
- For the safety of the Raspberry Pi, user may prefer the case for Raspberry Pi and Brick Pi  

	
Setting up the software in the following simple steps:

1. **Installation of opencv on your Raspberry Pi** - We will install opencv on the Raspberry Pi. Here is the link to the installation guide for opencv on Raspberry Pi: https://docs.google.com/document/d/1op8RSzmRqjiwh9KCkuavni5GiDaExN0I9GA6pvZy1EI/edit 
	
	**General Information for the following steps** 
    
	Every color is made of 3 primary color namely Red(R), Green(G) and Blue(B). In short RGB. In opencv image captured from camera will be in BGR form means Blue, Green and Red. In opencv the order by which we specify the color is dependent on the image matrix.
	In opencv there are many color spaces other than RGB like HSV, GRAY etc. We will use HSV color space for this project.

2. **Calibration** - Since we will be dealing with the color image. It is always preferable to work in HSV color space. Camera gives BGR format image. In opencv it is very simple to convert the BGR format to HSV format.
	
	* H stands for Hue which has the color information. In opencv the value of H ranges from 0 to 180.

	* S stands for Saturation which represents the saturation of the color i.e. purity of the color. In opencv the value of S ranges from 0 to 255.

	* V stands for Value or Brightness which represents the brightness of the color i.e. how much bright the color is. In opencv the value of V ranges from 0 to 255.
		
	This project requires calibration. This has to be done for better result. Since there are so many colors and correspondingly there are so many HSV values.
		
	Since calibration is always an important part of the autonomous project. Compile and run the calibration function. This will pop up a window there you can see images which is being captured by your Raspberry Pi camera. Make sure that the ball is in front of the camera with the background in which you want to run your Ball Tracker Pi robot. Since different lighting condition will results in different HSV values. Therefore we will always have to run the calibration program before starting the Ball Tracker Pi robot. If the ball is completely coming in the window which was popped up, then press "y" from your keyboard to lock this image for the calibration.
		
	Since the image has been locked. We can start the calibration. After hitting the "y" you might observed that the window is showing only one image. Move your cursor to the center of the ball in that window. Once click the right mouse button on the center of the ball.	Now in terminal you can give the color name of the ball or any 	name which you think is best for that ball. After specifying the color in terminal hit enter from the keyboard. Go to that window but don't use your right mouse button for doing this. Since right mouse button has been used to calibrate the color values of the ball.
		
	You can make use of left mouse button for selecting the window. If you want to calibrate more than one different color balls then follow the same procedure to calibrate another ball. Once you are done with calibration, select the image window using the left mouse button and press "Esc" from the keyboard.
        
	The calibration function creates a test.yml file for storing the color values, which will be used by the main program. The test.yml file can be find in the directory in which the calibration function is.
        
**3. Detection and Tracking** - This part is simpler than the previous. First we will compile the source code. Now run the program with two arguments. See the end of this ReadMe file for *How to compile and run* the source code. 
	
**Arguments for running this program**

1. **color_name** - The name of the color of the ball which you have specified in step 2.
2. **speed** - The speed with which you want to move your Ball Tracker Pi. During the run of this program you will see the circle which outlines the ball.


If you feel that calibration is not perfect you can try to modify one of the calibration parameters. For doing this we have two options. First we can make some changes in the calibration function. Second and the most easiest way, direct access to the test.yml file. There you will see the intervals of HSV value for each balls which have been calibrated. Values can be tweaked easily for the ball which you want to track from this method. Save the file. Run the main program. 

**Note:**

ball_detect_7.cpp is the calibration function for the ball_detect_8.cpp main program.

ball_detect_8.cpp make use of HoughCircle method to find the ball.
			
Whereas ball_detect_9.cpp is the calibration function for the ball_detect_10.cpp main program.

ball_detect_10.cpp make use of Moments method to find the ball. 

ball_detect_10.cpp is somewhat faster but not as reliable as ball_detect_8.cpp.
			
You can try both of these.

**How to compile**

*For the calibration function*
```C++
g++ `pkg-config --cflags opencv` ball_detect_9.cpp -o test_1 `pkg-config --libs opencv` -I/home/pi/git/robidouille/raspicam_cv -L/home/pi/git/robidouille/raspicam_cv -lraspicamcv -L/home/pi/git/raspberrypi/userland/build/lib -lmmal_core -lmmal -l mmal_util -lvcos -lbcm_host
```
*For the main program*
```C++
g++ `pkg-config --cflags opencv` ball_detect_10.cpp -o test_2 `pkg-config --libs opencv` -I/home/pi/git/robidouille/raspicam_cv -L/home/pi/git/robidouille/raspicam_cv -lraspicamcv -L/home/pi/git/raspberrypi/userland/build/lib -lmmal_core -lmmal -l mmal_util -lvcos -lbcm_host -lrt -lm -L/usr/local/lib -lwiringPi
```

**How to run**

*For the calibration function*
```C++
./test_1
```
*For the main program*
```C++
./test_2 color_name speed
```
*Here color_name is the first argument and the speed is the second argument.*
