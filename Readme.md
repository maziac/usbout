# UsbOut

This project uses a Teensy board to control some digital outputs.
The purpose is to highlight buttons in an arcade cabinet. (Or better to light the LEDs inside the buttons.)
E.g. the available buttons for a specific game are highlighted when a game is started.

You can also set a delay on an output, the intensity and smooth in/out transition.


This is done through serial communication. E.g. "o2=100" would turn the digital out 2 (DOUT2) to 100%.

On mac you can find the serial device e.g. with:
~~~
$ ls /dev/cu*
$ /dev/cu.Bluetooth-Incoming-Port	/dev/cu.usbmodem56683601
~~~
It's a little bit unclear how the naming is done but it is clear that we are not the Bluetooth device so the right device is:
/dev/cu.usbmodem56683601

To turn on DOUT0 use:
~~~
$ echo o0=100 > /dev/cu.usbmodem56683601
~~~

The full syntax for this command is:

oN=X[,attack[,delay] : Set output N to X (0-100) with an 'attack' time and a delay.
'attack' means that the digital out will change smoothly from its current value to the target (X) within the 'attack' time.
'delay' adds an additional delay before 'attack' starts.
All times are in ms.
If you omit the values 0 is assumed for 'attack' and 'delay' and the value of the digital out is changed immediately.

PWM is used to allow to "dim" the digital outputs. It works with a frequency of 100kHz to avoid flickering.


# HW

Used HW is a cheap Teensy LC board that you can get around 10€.

The pinout is used as following (but can be changed to fit your needs):
![](Images/TeensyLCSchematics.png)


# Building the SW

<<Mention The USB setting>>
