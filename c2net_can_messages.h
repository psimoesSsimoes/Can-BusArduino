/*
  message_type.h - Library for Arduino Project.
  Created by F�bio Lopes, 4 of July, 2017.
  GRIS, Uninova
*/

#ifndef c2net_can_messages_h
#define c2net_can_messages_h

#include <Canbus.h>
#include "c2net_can_defines.h"
/*#include <defaults.h>
#include <global.h>*/
#include <mcp2515.h>
#include <mcp2515_defs.h>



class c2net_can_messages
{
  public:
  void readMsg(tCAN message);
	void send_ping(uint8_t NODE_ID, uint8_t SENSOR_ID);
	void send_msg_with_sensor_value(uint8_t NODE_ID, uint8_t ID_SENSOR_TYPE, uint8_t ID_SENSOR, uint8_t VALUE_HIGH, uint8_t VALUE_LOW);
	  int getMessageType();
	char getMessageTypeName();
  int getNodeID();
  void sendCAN_Message(tCAN message);
  int getSensorValue();
  int getSensorTypeID();
  double getPingFrequency();
  char getSensorTypeName();
  int getSensorID();

  private:
  	tCAN _message;
  	int _merged1, _merged2;
  	double _frequency;
    uint8_t _b0, _b1, _b2, _b3, _b4, _b5, _b6, _b7;
    uint16_t _value;


};

#endif
