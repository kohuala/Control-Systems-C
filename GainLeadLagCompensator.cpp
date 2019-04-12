////// Guan, Huihua /////////

#include "daq.h"
#include <iostream>
using namespace std;

#define Bilinear
#define Forward
#define Backward

int main() {
      float SAMPLING_RATE_HZ = 1000;
      float TIMEOUT_SEC = 1;
      float VMIN = 0;
      float VMAX = 5;
      AI_Configuration(SAMPLING_RATE_HZ, TIMEOUT_SEC, VMIN, VMAX);
      AO_Configuration(VMIN, VMAX);

    float r, e_pre, e_pre2, ud_pre, ug_pre, prev_e=0 ;
    float K, zd, zg, pd, pg, ug, current, e;

    e_pre = 0; e = 0; e_pre2 = 0; ug =0; ud_pre = 0; ug_pre = 0; r = 0;

    //ask for a reference voltage

    cout << "r=";
    Write_Voltage(0);
    cin >> r;

    ud_pre = 0; float t = .001; float ui, ud;
    
// Compensator parameter values
    ui = 0; ud = 0; K = 5; zd = 4; pd =30; zg = 1; pg = .01;

    while (1) {

        current = Read_Voltage();
        e = r - current;
#ifdef Bilinear
		e = e*K;
		ud = ((2-t*pd)/(2+t*pd))*ud_pre + (pd/zd)*((2+t*zd)/(2+t*pd))*e + (pd/zd)*((-2+t*zd)/(2+t*pd))*e_pre;

		ug = ((2-t*pg)/(2+t*pg))*ug_pre + ((2+t*zg)/(2+t*pg))*ud + ((-2+t*zg)/(2+t*pg))*ud_pre;

		e_pre = e;
		ud_pre = ud;
		ug_pre=ug;
#endif

#ifdef Forward
		e = e*K;
		ug = (pd/zd)*e + (1-t*pd)*ug_pre + (pd/zd)*(-1+t*zd)*e_pre;
		ud = ug + (1-pg*t)*ud_pre + (-1+ zg*t)*ud_pre;

		e_pre = e;
		ud_pre = ud;
		ug_pre=ug;

#endif

#ifdef Backward

        ud = (1 / (zd*pd*t))*(zd*ud_pre + K*pd*e + K*pd*e_pre + K*pd*zd*t*e);

        ug = (1 / (1 + pg*t)) * (ug_pre + ud + zg*t*ud - ud_pre);
#endif

        if (ug > 5)ug=5;

        if (ug < -5)ug = -5;

        if (ug >= 5 || ug <= -5) {

           e_pre = e; ud_pre = 0; ug_pre = 0;

        }
        else {

            ud_pre = ud; ug_pre = ug;e_pre = e;

        }
        Write_Voltage(ug);}}
