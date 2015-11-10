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
#include<sys/types.h> 
#include<unistd.h>
#include<netinet/in.h>
#include<netdb.h>
#include<memory.h> 
#include<ifaddrs.h> 
#include<net/if.h>
#include<errno.h> 
#include<stdlib.h> 
#include<sstream>


using namespace std; 

int map_value(int);

int resolvehelper(const char* hostname, int family, const char* service, sockaddr_storage* pAddr)
{
    int result;
    addrinfo* result_list = NULL;
    addrinfo hints = {};
    hints.ai_family = family;
    hints.ai_socktype = SOCK_DGRAM; // without this flag, getaddrinfo will return 3x the number of addresses (one for each socket type).
    result = getaddrinfo(hostname, service, &hints, &result_list);
    if (result == 0)
    {
        //ASSERT(result_list->ai_addrlen <= sizeof(sockaddr_in));
        memcpy(pAddr, result_list->ai_addr, result_list->ai_addrlen);
        freeaddrinfo(result_list);
    }

    return result;
}

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

    	int result = 0;
        int sock = socket(AF_INET, SOCK_DGRAM, 0);

        char szIP[100];

        sockaddr_in addrListen = {}; // zero-int, sin_port is 0, which picks a random port for bind.
        addrListen.sin_family = AF_INET;
        result = bind(sock, (sockaddr*)&addrListen, sizeof(addrListen));
        if (result == -1)
        {
           int lasterror = errno;
           std::cout << "error: " << lasterror;
           exit(1);
        }


        sockaddr_storage addrDest = {};
        result = resolvehelper("10.20.6.174", AF_INET, "5005", &addrDest);
        if (result != 0)
        {
           int lasterror = errno;
           std::cout << "error: " << lasterror;
           exit(1);
        }

        
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

		stringstream ss; 

		throttle_index = command.find("Y1"); 
		rudder_index = command.find("X1"); 
		aileron_index = command.find("X2");
		elevator_index = command.find("Y2");

		if(throttle_index != -1)
		{
			throttle_string = command.substr(throttle_index+3,6);

			throttle_value = atoi(throttle_string.c_str());

			map_throttle = map_value(throttle_value); 

			ss << "TH_" << map_throttle << " ";

		}
		if(rudder_index != -1)
		{
			rudder_string = command.substr(rudder_index+3,6);
			
		        rudder_value = atoi(rudder_string.c_str());

			map_rudder = map_value(rudder_value); 

			ss << "RD_" << map_rudder<< " ";
		}
		if(elevator_index != -1)
		{
			elevator_string = command.substr(elevator_index+3,6);

		 	elevator_value = atoi(elevator_string.c_str());

			map_elevator = map_value(elevator_value); 

			ss << "EL_" << map_elevator << " ";
		}
		if(aileron_index != -1)
		{
			aileron_string = command.substr(aileron_index+3,6);

			aileron_value = atoi(aileron_string.c_str());

			map_aileron = map_value(aileron_value);

			ss << "AI_" << map_aileron<< "\n";
		}

		


		fgets(stream,1000,control_stream); 		

		string str = ss.str();

		const char* msg = str.c_str();
                size_t msg_length = strlen(msg);

                result = sendto(sock, msg, msg_length, 0, (sockaddr*)&addrDest, sizeof(addrDest));

	}

	pclose(control_stream); 

	return 0; 
}

int map_value(int v)
{
	return (v+32768)*72/65535 + 55;
}
