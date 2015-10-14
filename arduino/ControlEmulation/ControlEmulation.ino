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
Servo aux_channel;

const int _max = 130; //max on ail, ele, rudder.
const int _min = 56; //min on ail,ele,rudder
const int _max_t = 128;//max on throttle
const int _min_t = 59; //min on throttle
const int _arm_delay = 25;//to simulate mechanical delay

int aileron_value = 93; //93 is the middle value.
int elevator_value = 93; 
int throttle_value = 96; //59 is idle and 128 is full
int rudder_value = 93; 
int aux_value = 93;
int _pwm_value = 0; 
String command;

void setup()
{
  aileron_channel.attach(3);
  elevator_channel.attach(5);
  throttle_channel.attach(6);
  rudder_channel.attach(9);
  aux_channel.attach(10);
  
  aileron_channel.write(aileron_value); 
  elevator_channel.write(elevator_value);
  throttle_channel.write(throttle_value);
  rudder_channel.write(rudder_value); 
  aux_channel.write(aux_value);
  
  Serial.begin(9600); 
}

void loop()
{
  _read_command();
}


void _read_command()
{
  while(Serial.available())
  {
    char c = Serial.read(); 
    
    if(int(c) ==13) //for arduino SM use c=='\n'
    {
      _parse_command(command);
      command = "";
    }
    else
    {
      command += c;
    }
  }  
}
void _parse_command(String com)
{
  String part1; 
  String part2; 
  
  int underscore_index = com.indexOf("_");
  
  if(underscore_index > 0)
  {
    part1 = com.substring(0,underscore_index);
    part2 = com.substring(underscore_index + 1);
    
    int value = part2.toInt();
    
    if(!part1.equals("TH"))
    {
      _pwm_value = min(max(value,_min),_max); //trim value
    }else{
      
      _pwm_value = min(max(value,_min_t),_max_t);//trim value to 
                                                 //throttle vals.
    }    
    
     _write_to_channel(part1,_pwm_value);
   
  }
  else
  {
    int length = com.length(); 
    
    if(length == 3) //use ==4 for arduino
    {
      part1 = com; //use com.substring(0,3) for arduino
      if(part1.equals("ARM"))
      {

        //To arm rudder value must read 55 and throttle must read 58
        
        Serial.println("Arming"); 

        rudder_value = 55;   //make sure rudder is at middle pos
        throttle_value = 58; //make sure throttle is at middle pos
        aux_value = 130;     //value to turn on aux channel.

        aux_channel.write(aux_value);           //turn on aux.
        rudder_channel.write(rudder_value);     //Set rd to full right
        throttle_channel.write(throttle_value); //Set th to idle
        
        delay(2000);
        
        //default values
        rudder_value = 93;
        throttle_value = 96; 
        
        //reset to default values leaving aux on
        throttle_channel.write(throttle_value);
        rudder_channel.write(rudder_value);
        
      } 
      else if(part1.equals("DRM"))
      {
        
        //To arm rudder value must read 130 and throttle must read 58        
        
        Serial.println("Disarming"); 
        
        rudder_value = 130;   //make sure the sticks are at middle pos
        throttle_value = 58; //make sure throttle is at middle pos
        aux_value = 130;     //value to turn on aux.
        
        throttle_channel.write(throttle_value); //set th to idle
        rudder_channel.write(rudder_value);     //set rd to full right
        aux_channel.write(aux_value); //turn on aux channel.
        
        delay(2000);
        
        //default values
        rudder_value = 93;
        throttle_value = 93; 
        
        //reset to default values
        throttle_channel.write(throttle_value);
        rudder_channel.write(rudder_value);
        
      }      
      else if(part1.equals("CAL"))
      {
        //To calibrate turn of power on kk2 board
        //Send command CAL via serial 
        //Quickly connect power back on 
        //Press button 1 and 4 on kk2 board
        //The porgram should calibrate 
        //Indicated by all fo the ESC on
        Serial.println("Calibrating");
        
        delay(3000);
        
        aileron_value = 93;
        elevator_value = 93; 
        throttle_value = _max_t;
        rudder_value = 93;
        aux_value = 93;
        
        aileron_channel.write(aileron_value);
        elevator_channel.write(elevator_value); 
        throttle_channel.write(_max_t);
        rudder_channel.write(rudder_value);
        aux_channel.write(aux_value);
        
        
        Serial.print("max t = ");
        Serial.println(throttle_value);
        
        for(int i=1; i<=69; i++)
        {
          throttle_value -=1;
          throttle_channel.write(throttle_value);
          delay(_arm_delay);
        }
 
        delay(2000);
        Serial.print("min t = ");
        Serial.println(throttle_value);
      }
      else {_print_error(0);}
    } else
    {
      _print_error(0);
    }
    
  }
}

void _write_to_channel(String channel, int pwm)
{ 
  if(channel.equals("AI"))
  {
    aileron_value = pwm;
    aileron_channel.write(pwm);
  }
  else if(channel.equals("EL"))
  {
    elevator_value = pwm;
    elevator_channel.write(pwm);
  }
  else if(channel.equals("TH"))
  {
    throttle_value = pwm;
    throttle_channel.write(pwm);
  }
  else if(channel.equals("RD"))
  {
    rudder_value = pwm;
    rudder_channel.write(pwm); 
  }
  else if(channel.equals("AX"))
  {
    aux_value = pwm; 
    aux_channel.write(pwm);
  }
  else
  {
    _print_error(0); //Error 0 is incorrect command
    return;
  }
  
  Serial.print(channel);
  Serial.print("_");
  Serial.println(pwm);
}

void _print_error(int e)
{
  switch(e)
  {
    case 0:
          Serial.println("Incorrect command"); 
          break; 
    case 1:
          Serial.println("Some");
          break; 
    default:
          break;
  }
}
