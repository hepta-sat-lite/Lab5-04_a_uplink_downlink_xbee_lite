#include "mbed.h"
#include "LITE_CDH.h"
#include "LITE_EPS.h"
#include "LITE_SENSOR.h"
#include "LITE_COM.h"

RawSerial pc(USBTX,USBRX,9600);
LITE_CDH cdh(PB_5, PB_4, PB_3, PA_8, "sd", PA_3);
LITE_EPS eps(PA_0,PA_4);
LITE_SENSOR sensor(PA_7,PB_7,PB_6);
LITE_COM com(PA_9,PA_10,9600);

int main()
{
    eps.turn_on_regulator();//turn on 3.3V conveter
    cdh.turn_on_analogSW();//turn on transceiver
    int i = 0,rcmd=0,cmdflag=0;
    float bt,ax,ay,az;
    com.printf("Count Up!\r\n");
    while(1) {
        com.printf("num = %d\r\n",i);
        i++;
        wait(1.0);
        com.xbee_receive(&rcmd,&cmdflag);
        pc.printf("rcmd=%d, cmdflag=%d\r\n",rcmd, cmdflag);
        if (cmdflag == 1) {
            if (rcmd == 'a') {
                sensor.set_up();
                pc.printf("Command Get %d\r\n",rcmd);
                com.printf("HEPTA Uplink OK\r\n");
                pc.printf("===================\r\n");
                pc.printf("Accel sensing Mode\r\n");
                pc.printf("===================\r\n");
                for(int ii = 0; ii < 10; ii++) {
                    sensor.sen_acc(&ax,&ay,&az);
                    eps.vol(&bt);
                    com.printf("AX = %f\r\n",ax);
                    com.printf("AY = %f\r\n",ay);
                    com.printf("AZ = %f\r\n",az);
                    com.printf("V = %f\r\n",bt);
                    wait(0.5);
                }
            }
            com.initialize();
        }
    }
}