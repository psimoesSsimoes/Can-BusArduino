/*
  message_type.h - Library for Arduino Project.
  Created by F�bio Lopes, 4 of July, 2017.
  GRIS, Uninova
*/

//#include "Arduino.h"
#include "c2net_can_messages.h"

//read message, mandatory after receiving
void c2net_can_messages::readMsg(tCAN msg)
{
	_b0 = msg.data[0];
	_b1 = msg.data[1];
	_b2 = msg.data[2];
	_b3 = msg.data[3];
	_b4 = msg.data[4];
	_b5 = msg.data[5];
	_b6 = msg.data[6];
	_b7 = msg.data[7];
}

//gets
int c2net_can_messages::getMessageType()
{
	return _b0;
}

int c2net_can_messages::getNodeID()
{
		return _b1;
}

int c2net_can_messages::getSensorTypeID()
{
	if (_b0 == MSG_WITH_SENSOR_VALUE)
		return _b2;
}

int c2net_can_messages::getSensorID()
{
	if (_b0 == MSG_WITH_SENSOR_VALUE)
		return _b3;
	if (_b0 == REQUEST_ACTIVE_NODES_SENSOR_VALUE || _b0 == CHANGE_SENSOR_VALUE_RESPONSE_FREQUENCY || CHANGE_PING_FREQUENCY)
		return _b2;
}

double c2net_can_messages::getPingFrequency()
{
    _frequency = (int)_b3*100000000+(int)_b4*100000+(int)_b5*100+(int)_b6;
    return _frequency; 
}

int c2net_can_messages::getSensorValue()
{
  if (_b0 == MSG_WITH_SENSOR_VALUE)
  {
    _value = (_b4 << 8 ) | _b5;
    return _value; //4 bytes (_b2 to _b5) to int
  } 
}

//message construction
void c2net_can_messages::send_ping(uint8_t NODE_ID, uint8_t SENSOR_ID) {

  _message.header.rtr = 0;
  _message.header.length = 8; //formatted in DEC

  _message.data[0] = MSG_TYPE_PING_ON; // Message Type
  _message.data[1] = (0xFF & NODE_ID); //In the case of the CAN, it's the Node ID_CAN
  _message.data[2] = (0xFF & SENSOR_ID);
  _message.data[3] = 0x00;
  _message.data[4] = 0x00;
  _message.data[5] = 0x00;
  _message.data[6] = 0x00;
  _message.data[7] = 0x00;
  
  sendCAN_Message(_message);
}

void c2net_can_messages::sendCAN_Message(tCAN _message) {
	mcp2515_bit_modify(CANCTRL, (1<<REQOP2)|(1<<REQOP1)|(1<<REQOP0), 0);
	mcp2515_send_message(&_message);
}

void c2net_can_messages::send_msg_with_sensor_value (uint8_t NODE_ID, uint8_t ID_SENSOR_TYPE, uint8_t ID_SENSOR,
 uint8_t VALUE_HIGH, uint8_t VALUE_LOW) {
 
  _message.data[0] = MSG_WITH_SENSOR_VALUE; //Message Type
  _message.data[1] = (0xFF & NODE_ID); //In the case of the CAN, it's the Node ID_CAN
  _message.data[2] = ID_SENSOR_TYPE; //defined variables: TEMPERATURE_SENSOR,BUTTON_SENSOR,ULTRASONIC_SENSOR,INFRARED_SENSOR
  _message.data[3] = ID_SENSOR;
  _message.data[4] = VALUE_HIGH; //Value 2 Bytes
  _message.data[5] = VALUE_LOW;
  _message.data[6] = 0x00;
  _message.data[7] = 0x00;
  
  sendCAN_Message(_message);
}
