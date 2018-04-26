/*
    Arms for iSENSE
    Copyright (C) 2018 Engaging Computing Group

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AXON_H
#define AXON_H

// TODO: Should these be global variables rather than preprocessor definitions?

// Debugging options
#define SHOW_WIFI_DIAGNISTICS 1

// Define LED codes by color
#define RED_LED LED_BUILTIN
#define BLUE_LED 2

// Define LED codes by purpose
#define ACTION_LED RED_LED
#define NETWORK_LED BLUE_LED

// Define LED toggle macros
#define LED_ON false
#define LED_OFF true

// Define pin # that controls servo
#define SERVO_PIN 14

// Arduino libraries
#include <Arduino.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <math.h>

// TODO: remove when callAPI() works
#include <WiFiClient.h>


// Configuration and Private key header files (in this directory)
#include "Keys.h"
#include "Config.h"

namespace ECG {

class Axon {

private:

    // Stores truth value for whether device is in a valid state
    bool _valid ;
    
    // Stores truth value for whether device has attempted to connect to WiFi since booting
    bool _hasBegunWiFi ;

    // Controls the servo arm attached to SERVO_PIN during object construction
    Servo _servo ;

    // Tracker WiFi client for connection to iSENSE
    //WiFiClientSecure _client ;
    // TODO: switch back to clientsecure when possible
    WiFiClient _client ;

    // Raw data recieved from iSENSE
    String _payload ;

    /*
    * Set a device LED on or off
    * 
    * Parameters:
    *   LEDCode: This is intended to be a macro that relates to the LED lights.
    *       See the #define directives at the top of this file for details.
    *   state: This is also intended to be a macro that corresponds to whether an
    *       LED is to be turned on or off. LED_ON and LED_OFF are defined above.
    */ 
    void setLED(int LEDCode, bool state) ;

    /*
    * Move the servo to an angle between 0 and 180 at a given speed
    * If an angle outside these bounds, the supplied value will be
    * mod'd by 181 to ensure the user's compliance with these requirements
    * 
    * Parameters:
    *   angle: The function will attempt to move the servo to this position
    *   speed: The speed that the servo moves into position in degrees per second.
    *       The value must be between 1 and 360, else the default value  will be used
    *       If the (int) version is called, the speed will be set to the default value.
    *       The default value is defined above the implementation of this function.
    */
    void moveServo(int angle, int speed) ;
    void moveServo(int angle) ;

public:

    // Main constructor, also initializes hardware
    Axon() ;

    /*
    * Check if tracker is in a valid state (i.e. connected network/valid endpoint)
    * 
    * Return: true if device state is valid, false if device state is invalid
    */
    bool isValid() ;

    /*
    * Check if tracker is connected to WiFi
    * 
    * Return: true if device is connected, otherwise returns false
    */
    bool isOnline() ;
    
    /*
    * Wrapper for delay() function to clean up the style
    * 
    * Parameters:
    *   milliseconds: Time in milliseconds for the device to freeze and do nothing.
    */
    void sleep(unsigned long milliseconds) ;

    /*
    * Attempts to connect or reconnect to the WiFi network with the settings described
    *   in config.h
    *  
    * Return: true if connection is successful, else false 
    */
    bool connectToWiFi() ;

    /*
    * Calls iSENSE API and and requests data based on config
    * 
    * Return: true if data is retrieved from iSENSE, else false
    */
    bool callAPI() ;

    /*
    * Get the device's local IP address
    * 
    * Return: A stringified IP address if the device has connected to WiFi, otherwise "not connected"
    */
    String getLocalIP() ;
    
    /*
    * Function to test lights on device. Will eternally flash the red and blue LEDs in succession
    */
    void endlessDebugFlash() ;

    /*
    * A function to test all display components attached to the device.
    * Will activate servo arm and LED lights before returning to the caller
    * Unlike endlessDebugFlash(), this is not endless. I (@theyoyojo) was
    * planning to use this as a sort of "boot animation"
    * 
    * The dance will execute at default speed unless otherwise requested
    * 
    * Parameters:
    *   speed: an integer that will modify the speed at which the dance is executed.
    *       Currently TODO, but there will be predefined constants intended to be passed
    *       as parameters
    */
    void debugDance() ;
    void debugDance(int speed) ;

} ; // class Axon

} // namespace ECG

#endif // AXON_H