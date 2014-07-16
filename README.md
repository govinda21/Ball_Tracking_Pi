Ball_Tracking_Pi
================

The project name Ball Tracking Pi is an opencv project. In this project we are using Raspberry Pi. 
The project is for making an autonomous color ball tracker using the technique of image processing. 
Though it is a basic project in opencv but for Raspberry Pi user it will be a good start to opencv.
No need to worry about it. Even you are a beginner to opencv. 
I will guide you from the installation of opencv on your Pi to the completion of this project.

Components required for this project are:
	1> Raspberry Pi
	2> Raspberry Pi camera
	3> Brick Pi ( for controlling motors )
	4> Two lego motors
	5> Wifi adapter 
	6> Wifi connection
	7> 8 GB SD card
	8> Power cable for your Raspberry Pi
	9> Preferably a new case for your Raspberry Pi and Brick Pi, which is recently designed by me. Just for the safety of your Raspberry Pi and Brick Pi. 
	
Now let us build your own Ball Tracker Pi in following simple steps :

	STEP 1: Installation of opencv on your Raspberry Pi - We will install opencv on the Raspberry Pi. I have documented the installation guide for opencv on Raspberry Pi
			Here is the link to the installatin guide for opencv on Raspberry Pi : https://docs.google.com/document/d/1op8RSzmRqjiwh9KCkuavni5GiDaExN0I9GA6pvZy1EI/edit
			With the help of that document you will be able to run some simple opencv program with and without using Raspberry Pi camera.
	
	STEP 2:  Making this program to run on your Pi - We all know every color is made of 3 primary color namely Red(R), Green(G) and Blue(B). In short RGB.
				In opencv image captured from camera will be in BGR form means Blue, Green and Red. You might have think that I have just changed the order of color.
				But it is not like that. In opencv the order by which we specify the color is dependent on the image matrix.
				There are many color spaces other than RGB like HSV, GRAY etc. In this project we will be using HSV color space.
				Here we divide our main task in two substeps:
				
			a> Callibration - Since we will be dealing with the color image, it is always prefer to work in HSV color space.
									Does it sound strange? Camera gives BGR format image then, how do I convert this image to HSV format image?
									No, it is very simple. In opencv it is very simple to convert the BGR format to HSV.
									There is a functin named cvtColor() can easily change your BGR format image to the HSV format.
									H stands for Hue which has the color information. In opencv the value of H ranges from 0 t0 180.
									S stands for Saturation which represents the saturation of the color i.e. purity of the color. In opencv the value of S ranges from 0 to 255.
									V stands for Value or Brightness which represents the brightness of the color i.e. how much bright the color is. In opencv the value of V ranges from 0 to 255.
												
									Any autonomous program requires callibration. This has to done to get the better and correct result.
									Since there are so many colors and correspondingly there are so many HSV values.
									We don't need to remember any of these HSV values for the callibration. Since callibration is always an important part of the project.
									So I have written a user friendly program for this job. All you have to do is, run the callibration program. It will pop up a window there you can see
									images which is being captured by your Raspberry Pi camera. Make sure your ball is in front of the camera with the background on which you want
									to run the your Ball Tracker Pi. Since different lighting condition will results in different HSV values. Therefore we will always have to run the callibration
									function before starting your Ball Tracker Pi. If the ball is completly coming in the window which was poed up, then press "y" from your keyboard to lock this image.
									Since image has been locked. We can go for callibration. This time even you move your Ball Tracker Pi image on that window will not be changing.
									Move your cursor to the center of the ball in that window. Click once your right mouse button on the center of the ball.
									Now in terminal you can give the color name of the ball or any name which you think is best for your ball. After specifying the color in terminal hit enter from the 
									keybpard. Go to that window but don't use your rifght mouse for doing this. Since right mouse button has been used to callibrate the color values of the ball.
									You can make use of left mouse button for selecting the window. If you want to callibrate more number of different color balls at a single run. You can do this	
									also in a very simple way. Just make sure that the image which has been locked is having that ball.That's it. Now can do the same thing what you have done for 
									the callibration of the first ball like move your cursor to center of the ball for which you want to callibrate. Then go to terminal give it a name. Hit enter and come 
									back to the image window. Once you are done with callibration hit "Esc" from your keyboard.
									This function creats a test.yml file for storing the color values, which can be used in the main program. You can also see this just open the directory in wich your program is.
									You can see the see the test.yml file which has been created after running the callibration program.
				
			b> Detection and Tracking - 	In this part all you have to do is run the program with two arguments. First one is the name of the color of the ball which you have specified in part a.
									Second one is the speed with which you want to move your Ball Tracker Pi. During the run of this program you will see the cirle which outlines the ball.
									
			That's all for this project. Congratulation! now you have your own Ball Tracker Pi with you.
			If you feel the caliration is not perfect you can or you would like to modify some callibration parameter then you can make some change in the callibration function or else 
			the easiest way to do so is. Go to the test.yml file. There you will see the interval of HSV value for each balls which have been calibrated. There you can tweak the values for the ball 
			which you want to track. Save the file. 
			
			ball_detect_7.cpp is the callibration function for the ball_detect_8.cpp main program.
			ball_detect_8.cpp make use of HoughCircle method to find the ball.
			
			Whereas ball_detect_9.cpp is the callibration function for the ball_detect_10.cpp main program.
			ball_detect_10.cpp make use of Moments method to find the ball.
			
			You can try both of these. Which suits best for you can go with that. Have a great fun with your Ball Tracker Pi.
			Feel free to contribute if you are interested to develope this to a next level.