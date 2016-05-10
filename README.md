# nsa_arduino
The arduino project for the NSA UNO

## What it is
This project is part of the larger NSA UNO project, which is an alarm system for the home. 
This is the alarm unit, which through Radio Frequency, sends oregon protocol messages to communicate with an 
Telldus Tellstick Duo.

## How it works
* When the unit is on, and working properly, it will send the temperature 90, using the oregon protocol.
* When the alarm is triggered, it will send the temperature 30.
* When the alarmsensor is broken, it will send the temperature 60.
