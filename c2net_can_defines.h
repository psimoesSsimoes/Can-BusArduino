/*
  message_type.h - Library for Arduino Project.
  Created by Fabio Lopes, 4 of July, 2017.
  GRIS, Uninova
*/

#ifndef c2net_can_defines_h
#define c2net_can_defines_h

//messages
#define MSG_TYPE_PING_ON 					0x10
#define MSG_WITH_SENSOR_VALUE 				0x15
#define REQUEST_ACTIVE_NODES_SENSOR_VALUE 	0x21
#define CHANGE_PING_FREQUENCY 0x33
#define CHANGE_SENSOR_VALUE_RESPONSE_FREQUENCY 0x41

//sensors
#define TEMPERATURE_SENSOR 0x10
#define BUTTON_SENSOR 0x1F
#define ULTRASONIC_SENSOR 0x13
#define INFRARED_SENSOR 0x20

//variables
#define ALL_SENSORS_VALUES 0xFF
#define ONLY_PING 0x00
#define ALL_NODES_ONLY_FOR_PING 0x00
#define ALL_NODES_ALL_SENSORS_VALUES 0xFF
#define ALL_NODES 0xFF

#endif
