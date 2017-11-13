//Libraries
#include <Ultrasonic.h>
#include <Canbus.h>
#include <mcp2515.h>
#include <mcp2515_defs.h>
#include "c2net_can_messages.h"

//Nodes
#define C2NET_NODE_ID 0xE1

//Pins
#define pino_trigger1 8
#define pino_echo1 9
#define Threshold1 9
#define pino_trigger2 6
#define pino_echo2 7
#define Threshold2 9
 
//Initializes the sensors and related variables
uint8_t ID_SENSOR1 = 0x01;
Ultrasonic ultrasonic1(pino_trigger1, pino_echo1);
float cmMsec1;
boolean newPiece1;
long microsec1;
uint16_t newPieceCount1;
uint8_t ID_SENSOR2 = 0x02;
Ultrasonic ultrasonic2(pino_trigger2, pino_echo2);
float cmMsec2;
boolean newPiece2;
long microsec2;
uint16_t newPieceCount2;

//CAN messages and times
tCAN message;
c2net_can_messages can_messages;
uint32_t time_x1;
double frequency_ping1 = 3000;
double frequency_value1 = 5000;
double time_for_ping1 = 3000;
double last_time_ping1;
double time_for_value1 = 5000;
double last_time_value1;
uint32_t time_x2;
double frequency_ping2 = 3000;
double frequency_value2 = 5000;
double time_for_ping2 = 3000;
double last_time_ping2;
double time_for_value2 = 5000;
double last_time_value2;
int contador_teste = 0;
boolean teste = false;

void setup()
{
  //CAN setup
  Serial.begin(9600);
  newPiece1 = 0;
  newPieceCount1 = 0;
  newPiece2 = 0;
  newPieceCount2 = 0;
  while ( !Canbus.init(CANSPEED_500)) {
      delay(1000);
      Serial.println("CAN not connected");
  }
  Serial.println("CAN connected");

  //initializing times //apaguei umas coisas, verificar se precisam
  last_time_ping1 = millis();
  last_time_value1 = millis();
  last_time_ping2 = millis();
  last_time_value2 = millis();

  message.header.rtr = 0;
  message.header.length = 8; //formatted in DEC
  message.data[0] = 0x00;
  message.data[1] = 0x00;
  message.data[2] = 0x00;
  message.data[3] = 0x00;
  message.data[4] = 0x00;
  message.data[5] = 0x00;
  message.data[6] = 0x00;
  message.data[7] = 0x00;
}
 
void loop()
{
    delay(1000);
    //updates times
    time_x1 = millis();
    time_for_ping1 = time_x1 - last_time_ping1;
    time_for_value1 = time_x1 - last_time_value1;
    time_x2 = millis();
    time_for_ping2 = time_x2 - last_time_ping2;
    time_for_value2 = time_x2 - last_time_value2; 
      
    //reads incoming messages
    if (mcp2515_check_message()) {
      if (mcp2515_get_message(&message)) {
        can_messages.readMsg(message);
        if((can_messages.getMessageType() == 0x33) && 
          (can_messages.getNodeID() == C2NET_NODE_ID || can_messages.getNodeID()== 0xFF))
        {
          if (can_messages.getSensorID() == 0x01)
          {
              frequency_ping1 = can_messages.getPingFrequency()*1000;
          }
          if (can_messages.getSensorID() == 0x02)
              frequency_ping2 = can_messages.getPingFrequency()*1000;   
        }
        if((can_messages.getMessageType() == 0x41) && 
          (can_messages.getNodeID() == C2NET_NODE_ID || can_messages.getNodeID()== 0xFF))
        {
          if (can_messages.getSensorID() == 0x01)
          {
              frequency_value1 = can_messages.getPingFrequency()*1000;
          }
          if (can_messages.getSensorID() == 0x02)
              frequency_value2 = can_messages.getPingFrequency()*1000;   
        }
        if (can_messages.getMessageType() == REQUEST_ACTIVE_NODES_SENSOR_VALUE && 
          (can_messages.getNodeID() == C2NET_NODE_ID || can_messages.getNodeID()== 0xFF)) {
            
          if (can_messages.getSensorID() == 0x01)
              can_messages.send_msg_with_sensor_value(C2NET_NODE_ID, ULTRASONIC_SENSOR, ID_SENSOR1,
                                                  (newPieceCount1 & 0xFF00) >> 8, (newPieceCount1 & 0xFF));
          if (can_messages.getSensorID() == 0x02)
              can_messages.send_msg_with_sensor_value(C2NET_NODE_ID, ULTRASONIC_SENSOR, ID_SENSOR2,
                                                  (newPieceCount2 & 0xFF00) >> 8, (newPieceCount2 & 0xFF));             
          newPieceCount1 = 0;
          newPieceCount2 = 0;
        }
      }
    }

    //frequency ping for each sensor
    if (frequency_ping1 > 0 && (time_for_ping1 >= frequency_ping1)) {
      can_messages.send_ping(C2NET_NODE_ID, ID_SENSOR1);
      last_time_ping1 = millis();
      Serial.print("Mandei ping: ");
      Serial.println(frequency_ping1);
      time_for_ping1 = 0;
    }
    /*if (frequency_ping2 > 0 && (time_for_ping2>=frequency_ping2)) {
      can_messages.send_ping(C2NET_NODE_ID,ID_SENSOR2);
      Serial.println(frequency_ping2);
      last_time_ping2 = millis();
      time_for_ping2 = 0;
    }*/
    //frequency value for each sensor
    if (frequency_value1 > 0 && (time_for_value1 >= frequency_value1)) {
      can_messages.send_msg_with_sensor_value(C2NET_NODE_ID, ULTRASONIC_SENSOR, ID_SENSOR1,
                                                  (newPieceCount1 & 0xFF00) >> 8 , (newPieceCount1 & 0xFF));
      last_time_value1 = millis();
      Serial.print("Mandei valor: ");
      Serial.println(frequency_value1);
      time_for_value1 = 0;
      newPieceCount1 = 0;
    }
    /*
    if (frequency_value2 > 0 && (time_for_value2 >= frequency_value2)) {
      can_messages.send_msg_with_sensor_value(C2NET_NODE_ID, ULTRASONIC_SENSOR, ID_SENSOR2,
                                                  (newPieceCount2 & 0xFF00) >> 8 , (newPieceCount2 & 0xFF));
      last_time_value2 = millis();
      Serial.println(frequency_value2);
      time_for_value2 = 0;
      newPieceCount2 = 0;
    }*/
    
    //Sensor Measurements, in cm and inches
    microsec1 = ultrasonic1.timing();
    cmMsec1 = ultrasonic1.convert(microsec1, Ultrasonic::CM);
    Serial.print("Sensor 1: ");
    Serial.println(cmMsec1);
    if ( cmMsec1 < Threshold1) {
      if (newPiece1 == false) {
        newPiece1 = true;
        newPieceCount1++;
      }
    }else{
      newPiece1 = false;
    }
    delay(100);
    microsec2 = ultrasonic2.timing();
    cmMsec2 = ultrasonic2.convert(microsec2, Ultrasonic::CM);
    //Serial.print("Sensor 2: ");
    //Serial.println(cmMsec2);
    if (cmMsec2 < Threshold2) {
      if (newPiece2 == false) {
        newPiece2 = true;
        newPieceCount2++;
      }
    }else{
      newPiece2 = false;
    }

    //cycle delay
    delay(100);
    Serial.print("Piece Count 1: ");
    Serial.println(newPieceCount1);
    Serial.print("Piece Count 2: ");
    Serial.println(newPieceCount2);
}
