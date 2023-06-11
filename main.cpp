#include "mbed.h"
#include "HEPTA_EPS.h"
#include "HEPTA_CDH.h"
#include "HEPTA_SENSOR.h"
#include "HEPTA_COM.h"
HEPTA_CDH cdh(PB_5, PB_4, PB_3, PA_8, "sd");
HEPTA_EPS eps(PA_0,PA_4);
HEPTA_SENSOR sensor(PA_7,PB_7,PB_6,0xD0);
HEPTA_COM com(PA_9,PA_10,9600);
RawSerial sat(USBTX,USBRX,9600);
int main()
{
    sat.printf("Xbee Uplink Downlink Mode\r\n");
    int rcmd=0,cmdflag=0;
    float ax,ay,az;
    eps.turn_on_regulator();//turn on 3.3V conveter
    sensor.setup();
    for(int i=0;i<10;i++){
        com.xbee_receive(&rcmd,&cmdflag);
        com.printf("num=%d\r\n",i);
        if(cmdflag==1){
            if(rcmd=='a'){
                sat.printf("rcmd=%c,cmdflag=%d\r\n",rcmd,cmdflag);
                com.printf("Hepta-Sat Lite Uplink Ok\r\n");
                for(int i=0;i<10;i++){
                    sensor.sen_acc(&ax,&ay,&az);
                    com.printf("%f,%f,%f\r\n",ax,ay,az);
                    wait_ms(1000);
                }
            }
            com.initialize();
        }
        wait_ms(1000);
    }
}