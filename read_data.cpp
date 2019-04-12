#include <stdio.h>
#include "daq.h"
#include <iostream>
#include <fstream>
using namespace std;

int main(){
// Part 1
float SAMPLING_RATE_HZ = 100;
float TIMEOUT_SEC = 1;
float VMIN = 0;
float VMAX = 5;
AI_Configuration(SAMPLING_RATE_HZ, TIMEOUT_SEC, VMIN, VMAX);
AO_Configuration(VMIN, VMAX);

float voltage = 0;
while(1){
	voltage=Read_Voltage();
	cout << voltage << endl;
}
// Part 2
ofstream myfile ("20Hz.txt");
float SAMPLING_RATE_HZ = 100;
float TIMEOUT_SEC = 1;
float VMIN = 0;
float VMAX = 5;
AI_Configuration(SAMPLING_RATE_HZ, TIMEOUT_SEC, VMIN, VMAX);
AO_Configuration(VMIN, VMAX);

float voltage = 0;
while(1){
	voltage=Read_Voltage();
	Write_Voltage(voltage);
	cout << voltage << endl;

	myfile << voltage <<endl;
}
myfile.close();
}
