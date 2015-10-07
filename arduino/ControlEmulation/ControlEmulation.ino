//Control Channel emulation 
// This program emulates an RC reciever with PWM outputs
// using the servo library. 
// There are 4 channels of control: 
// aileron = roll
// elevator = pitch
// throttle = hight or power in motors.
// rudder = yaw 
//  
// Created by Serred Sep/19/15
#include <Servo.h> 
Servo aileron_channel; 
Servo elevator_channel; 
Servo throttle_channel; 
Servo rudder_channel; 

const int _max = 130; //max on ail, ele, rudder.
const int _min = 56; //min on ail,ele,rudder
const int _max_t = 128;//max on throttle
const int _min_t = 59; //min on throttle

int aileron_value = 93; //93 is the middle value.
int elevator_value = 93; 
int throttle_value = 59; //59 is idle and 128 is full
int rudder_value = 93; 

String to_print= "";

void setup()
{
  aileron_channel.attach(3);
  elevator_channel.attach(5);
  throttle_channel.attach(6);
  rudder_channel.attach(9);
  Serial.begin(9600); 
}

void loop()
{
  if(Serial.available() >0) 
  {

    int inByte = Serial.read(); 
    _inc_channel_out(inByte);

  }

  aileron_channel.write(aileron_value); 
  elevator_channel.write(elevator_value); 
  throttle_channel.write(throttle_value);
  rudder_channel.write(rudder_value); 
}

void _inc_channel_out(int inByte)
{
  switch(inByte)
  {
    case 'a':
      if(aileron_value < _max) aileron_value +=1;
      Serial.print("aileron_value: ");
      Serial.print(aileron_value);
      Serial.println(" right");
      break;
    case 'z':
      if(aileron_value > _min) aileron_value -=1;
      Serial.print("aileron_value: ");
      Serial.print(aileron_value);
      Serial.println(" left");
      break;
    case 's':
      if(elevator_value < _max) elevator_value +=1;
      Serial.print("elevator_value: ");
      Serial.print(elevator_value);
      Serial.println(" back");
      break;
    case 'x':
      if(elevator_value > _min) elevator_value -=1;
      Serial.print("elevator_value: ");
      Serial.print(elevator_value);
      Serial.println(" forward"); 
      break; 
    case 'd':
      if(throttle_value < _max_t) throttle_value +=1;
      Serial.print("throttle_value: ");
      Serial.print(throttle_value);
      Serial.println(" +t");
      break;
    case 'c':
      if(throttle_value > _min_t) throttle_value -=1;
      Serial.print("throttle_value: ");
      Serial.print(throttle_value);
      Serial.println(" -t");
      break;
    case 'f':
      if(rudder_value < _max) rudder_value +=1;
      Serial.print("rudder_value: ");
      Serial.print(rudder_value);
      Serial.println(" left");
      break;
    case 'v':
      if(rudder_value > _min) rudder_value -=1;
      Serial.print("rudder_value: "); 
      Serial.print(rudder_value);
      Serial.println(" right");
      break;
    default:
      break;
  }
}
