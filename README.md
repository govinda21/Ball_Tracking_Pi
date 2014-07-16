Ball_Tracking_Pi
================

This project is for detection and tracking of any colored ball.

This can be done at two stage: 
  1> Callibration
  2> Making use of the callibration to detect the color ball

Callibration:
  In callibration program you will be able to callibrate any number of ball at a single and can also give the name to each    of the color ball. First you have to select the image for which you want to callibrate. Press y to select the image, now    you can pick any point on the ball. I prefer you to pick on the center. Then give a name to this ball in the command line.   Come back to image, If you have to callibrate for more number of ball just repeat the similar procedure. You can only       select the ball with right click from the mouse. So don't use unnecessarily right click of the mouse on the image window.   Finally if you have done the callibration press Esc from your keyboard.

  In case if you feel the callibrated value is not perfect for your environment. You can change this in the program or in     the file which will be created on running the callibration function. Making a change in the created file "test.yml" is      pretty handy and this will be easy to tweak. 
  
Detection and Tracking:
  Once you done with callibration, now you can use this dara to run your bot.
  You have to specify two thing while running. First is the name of ball which you specified during callibration and the      second is the speed with which you want to move your bot.
