//// Guan, Huihua /////

#include "daq.h"
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string.h>
using namespace std;

int main() {
float64 deg = 0.0;
float64 SAMPLING_RATE_HZ = 1400;
int32 TIMEOUT_SEC = 5;
float64 VMIN = -6;
float64 VMAX = 6;

//create an output stream
ofstream results;                      

//open the file for output
results.open ("Results.txt"); 

//writes to output file
results << "Position r\t" << "     u output of the PID" << "     potentiometer reading " << endl; 

//initializes DAQ input	
AI_Configuration(SAMPLING_RATE_HZ, TIMEOUT_SEC, VMIN, VMAX); 

//Initializes DAQ output
AO_Configuration(VMIN, VMAX); 


float64 transferFunc, u, v, up, ui, ud=0, error, vout, previous_ui = 0, previous_ud = 0, previous_error = 0, previous_v = 0, fc = 200, wc = 1256, filter;


float64 ud_filt, ud_filt_previous = 0, T = 1/SAMPLING_RATE_HZ;
float64 tc = 0.08, kc = 4, ti = tc/2, td = tc/8, r;
float64 k = 0.6*kc;

cout << "Please enter a degree position you want the motor to go to:"<< endl;
Write_Voltage(0);
cin >> deg;
	
for(int i = 0; i < 2000; i++)
	while(1){
	   {
		r = (deg*5)/180;       //Reference
		vout = Read_Voltage(); //Read Voltage
		error = r - vout;    //Calculate Error
		up = k*error;        //Proportional


////////////////// Bilinear Transformation /////////////////////////////////////////////

		v  = previous_v + (T/2)*(previous_error + error);
		ud = -previous_ud + ((2/T)*(error-previous_error)); //Derivative
		ud_filt = ((2 -T*wc)*ud_filt_previous + T*wc*ud           +T*wc*previous_ud)/(T*wc+2); //Filter
		
//////////////////// Backward //////////////////////////////////
		
		v= T*error+previous_v;
		ud=(1/T)*(error - previous_error);
		ud_filt= (ud_filt_previous + T*wc*ud)/(1 + T*wc);
		
///////////////////Forward/////////////////////////////////////
		v = T*previous_error + previous_v;
		previous_ud = (1/T)*(error - previous_error);
		ud_filt = previous_ud*T*wc - (T*wc)*ud_filt_previous + ud_filt_previous; //Filter
		
//////////////////////////////////////////////////////////////////////////
		u= k*(error + (1/ti)*v + td*ud);
		previous_error = error; //Anti windup

		previous_ud = ud;
		ud_filt_previous = ud_filt;

			if( u > 6 || u <-6){
				v = previous_v;
				u= k*(error + (1/ti)*v+ td*ud);
			
				if( u > 6){
				u = 6;}
			else if( u < -6){
				u = -6;}
			}
			else
			{
				previous_v = v;
			}
		Write_Voltage(u);
	   }
results << r  << '\t' << '\t'<< '\t' << u << '\t' << '\t' << '\t' << vout << endl;  } u = 0;}
