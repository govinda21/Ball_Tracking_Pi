Ball Tracker Pi
================

The project name is Ball Tracking Pi is an opencv project. In this project we will be using Raspberry Pi and Brick Pi. 
This project is to make an autonomous color ball tracker using the technique of image processing. It is a basic project in opencv. This will be a good start to opencv on Raspberry Pi platform.This will guide you from the installation of opencv on your Pi to the completion of this project.

Components required for this project are
- Raspberry Pi
- Raspberry Pi camera
- Brick Pi ( for controlling motors )
- Two lego motors
- Wifi adapter 
- Wifi connection
- 8 GB SD card
- Power cable for your Raspberry Pi
- For safety of the Raspberry Pi, user may prefer the case for Raspberry Pi and Brick Pi  

	
Now let us build your own Ball Tracker Pi in following simple steps

1. **Installation of opencv on your Raspberry Pi** - We will install opencv on the Raspberry Pi. Here is a guide for the installation of opencv on Raspberry Pi. Here is the link to the installatin guide for opencv on Raspberry Pi : https://docs.google.com/document/d/1op8RSzmRqjiwh9KCkuavni5GiDaExN0I9GA6pvZy1EI/edit With this document. Installation will be easier. Can be able to run some simple opencv program with and without using Raspberry Pi camera. 
	
	**General Information about the following steps** 
    
	To run this program on your Pi. Every color is made of 3 primary color namely Red(R), Green(G) and Blue(B). In short RGB. In opencv image captured from camera will be in BGR form means Blue, Green and Red. This might seeems that it just a change in the order of color. But it is not like that. In opencv the order by which we specify the color is dependent on the image matrix.
	In opencv there are many color spaces other than RGB like HSV, GRAY etc. In this project we will be using HSV color space.

2. **Callibration** - Since we will be dealing with the color image, it is always preferable to work in HSV color 	space. Camera gives BGR format image. It is very simple in opencv to convert the BGR format to HSV.In opencv, a functin named cvtColor() easily this sort of conversion.
	
	* H stands for Hue which has the color information. In opencv the value of H ranges from 0 to 180.
	* S stands for Saturation which represents the saturation of the color i.e. purity of the color. In opencv the value of S ranges from 0 to 255.
	* V stands for Value or Brightness which represents the brightness of the color i.e. how much bright the color 	is. In opencv the value of V ranges from 0 to 255.
		
        For making any bot to be intelligent enough to carry out some operatin by it is own. This requires callibration. This has to done for better and correct result. Since there are so many colors and correspondingly there are so many HSV values.
		
        We don't need to remember any of these HSV values for the callibration. Since callibration is always an important part of the autonomous project. So to make the callibration easy enough that can be handle by any user. Here ten a user friendly program for this job. Run the callibration program. It will pop up a window there you can see images which is being captured by your Raspberry Pi camera. Make sure that the ball is in front of the camera with the background in which you want to run your Ball Tracker Pi. Since different lighting condition will 	results in different HSV values. Therefore we will always have to run the callibration function before starting the Ball Tracker Pi. If the ball is completly coming in the window which was poped up, then press "y" from your keyboard to lock this image for the callibration.
		
        Since the image has been locked. We can start the callibration. After the previous step you might obsserved that the window is showing only one image. Move your cursor to the center of the ball in that window. Click once your right mouse button on the center of the ball.	Now in terminal you can give the color name of the ball or any 	name which you think is best for your ball. After specifying the color in terminal hit enter from the keybpard. Go to that window but don't use your rifght mouse for doing this. Since right mouse button has been used to	callibrate the color values of the ball.
		
        You can make use of left mouse button for selecting the window. If you want to callibrate more than one different color balls at a single run. You can do this also in a very simple way. Just make sure that the image which has been locked is having that ball.That's it. Now can do the same thing what you have done for the callibration of the first ball like move your cursor to center of the ball for which you want to callibrate. 	Then go to terminal give it a name. Hit enter and come back to the image window. Once you are done with callibration then hit "Esc" from your keyboard.
		The callibration function creats a test.yml file for storing the color values, which will be used by the main program. The test.yml file can be find in the directory in wich the callibration function is.
        
3. **Detection and Tracking** - In this part is quite simpler than the callibration. First have to compile the source code. Now run the program with two arguments. *See the end of this ReadMe file for compilation and running of the source code*. 
	
**Arguments for running this program**	
1. The name of the color of the ball which you have specified in part a.
2. The speed with which you want to move your Ball Tracker Pi. During the run of this program you will see the cirle which outlines the ball.

That's all for this project. Now you have your own Ball Tracker Pi with you. If you feel that caliration is not perfect you can modify some callibration parameter. For doing this we have two option. First we can make some change in the callibration function. Second the more easier way: Go to the test.yml file. There you will see the interval of HSV value for each balls which have been calibrated. There values can be tweaked easily for the ball which you want to track. Save the file. And run the main program to see the change. 

**Note:**

ball_detect_7.cpp is the callibration function for the ball_detect_8.cpp main program.

ball_detect_8.cpp make use of HoughCircle method to find the ball.
			
Whereas ball_detect_9.cpp is the callibration function for the ball_detect_10.cpp main program.

ball_detect_10.cpp make use of Moments method to find the ball.
			
You can try both of these. Which suits best for you can go with that. Have a great fun with your Ball Tracker Pi.

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

*For the callibration function*
```C++
./test_1
```
*For the main program*
```C++
./test_2 color_name speed
```
*Here color_name is the first argument and the speed is the second argument.*
