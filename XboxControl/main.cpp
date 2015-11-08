//Program that reads input from xbox control 
//then maps to corresponding control signals 
//for the quadrotor
//
//Created by serred 6/Nov/2015

#include<stdio.h>
#include<string>
#include<cstdlib> 
#include<iostream>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUFLEN 1024
#define PORT 5005

using namespace std; 

int map_value(int);

int main(int argc, char* argv[])
{
	FILE *control_stream = NULL;
	char stream[1000];

	string command; 

	int rudder_index; 
	int throttle_index; 
	int elevator_index; 
	int aileron_index; 

	string rudder_string; 
	string throttle_string; 
	string elevator_string; 
	string aileron_string; 

	int elevator_value; 
	int rudder_value; 
	int throttle_value; 
	int aileron_value; 

	int map_throttle; 
	int map_rudder; 
	int map_elevator; 
	int map_aileron;

	control_stream = popen("xboxdrv","r");
	if(control_stream == NULL)
	{
		perror("Error de conexion en control");
		exit(-1); 
	}

	fgets(stream,1000,control_stream);

	while(!feof(control_stream))
	{
		command = stream;

		throttle_index = command.find("Y1"); 
		rudder_index = command.find("X1"); 
		aileron_index = command.find("X2");
		elevator_index = command.find("Y2");

		if(throttle_index != -1)
		{
			throttle_string = command.substr(throttle_index+3,6);

			throttle_value = atoi(throttle_string.c_str());

			map_throttle = map_value(throttle_value); 

			cout << "map_throttle = " << map_throttle << "\t"; 

		}
		if(rudder_index != -1)
		{
			rudder_string = command.substr(rudder_index+3,6);
			
		        rudder_value = atoi(rudder_string.c_str());

			map_rudder = map_value(rudder_value); 

			cout << "map_rudder = " << map_rudder << "\t "; 
		}
		if(elevator_index != -1)
		{
			elevator_string = command.substr(elevator_index+3,6);

		 	elevator_value = atoi(elevator_string.c_str());

			map_elevator = map_value(elevator_value); 

			cout << "map_elevator = " << map_elevator << "\t"; 
		}
		if(aileron_index != -1)
		{
			aileron_string = command.substr(aileron_index+3,6);

			aileron_value = atoi(aileron_string.c_str());

			map_aileron = map_value(aileron_value);

			cout << "map_aileron = " << map_aileron << endl; 
		}

		


		fgets(stream,1000,control_stream); 		
	}

	pclose(control_stream); 

	return 0; 
}

int map_value(int v)
{
	return (v+32768)*72/65535 + 55;
}
