# Arduino Datalogger for Vernier Conductivity Meter and Peristaltic Pump Controller

This code was written for a datalogger that I (Luke Padgett) designed for my 
senior capstone and science fair project at Plano Academy High School from
August 2016-April 2017, which was original capacitive deionization research.

#PURPOSE
The purpose of this datalogger was to control the flow rate of a peristaltic
pump used in a capacitive deionization apparatus I had created, as well as log
data in the following manner: voltages were read from a vernier conductivity probe,
those analog voltage readings were transformed into digital signals via an ADS1115 ADC,
those digital signals were sent to an Arduino UNO, the UNO transformed those readings
into ppm (parts per million) values, and finally sent those values to a datalogging module
to be recorded in .csv files, depending on the test cycle ID.


#Schematics?
Yes, there are schematics for this. I included a picture of the schematics for the datalogger
I designed in this repository. It is entitled "schematics.pdf." Parts used that are not generic
(generic as in extremely common and general-purpose, e.g. wire, NPN transistors, power diodes, 
etc) are as follows: 1 ADS1115 ADC, 1 16x2 LCD, 1 SN74HC595N 8-bit shift register, 1 Analog British 
Telecom connector, 1 REF02 +5v voltage reference chip, 1 OPA177 operational amplifier, and 1
"Deek-Robot SHIELD MODEL Data logging board Products ID:8122" data logging module (which logged data
to a micro-SD card). 

NOTE: As of 9/2017, this design is outdated. The changes made include: adding an RF choke on the 
positive wire for the motor, adding 2 more electrolytic capacitors on the lines of the motor, 
and adding an electrolytic capacitor on the power lines of every "module" and IC. These changes
were made to resolve the issue of noise garbling what was to be displayed on the LCD display once
the motor had been running for too long.


#LICENSING
The only licenses that have to do with this project in terms of code are the licenses of the 
libraries that I did not write, such as any Adafruit libraries. I included the proper licensing
headers in those files. I do not wish to place cumbersome restrictions on the code that I wrote,
since I have found many pieces of code that I would like to use in potentially profitable ideas
or schoolwork before, only to find that the code was licensed or copyrighted in some form and I
couldn't use it without going through a painstaking process of releasing everything to the public
or paying royalties to the writer. So, I used the MIT software license, which is very liberal in
its terms.
