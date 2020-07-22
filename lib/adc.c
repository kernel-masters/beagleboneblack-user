#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "../inc/lcd.h"

void ADC_CHECK(void)
{

        int fd,fd1;
        int i=0, j=0,n=12,count=5;
        char c,buff[5],buff1[5];
        int ret;
        int Dout=0;
        float Vref= 1.8;
        float Step_Size,Vin;
        float temp;
        char Rx_Buff[10];
         fd= open("/sys/bus/iio/devices/iio:device0/in_voltage0_raw",O_RDONLY);
  if (-1 == fd) {
                fprintf(stderr, "Failed to open you have to load updated modules !\n");
                return(-1);
        }
           fd1= open("/sys/bus/iio/devices/iio:device0/in_voltage1_raw",O_RDONLY);
  if (-1 == fd1) {
                fprintf(stderr, "Failed to open you have to load updated modules !\n");
                return(-1);
        }
while(count--){
j=0;
Dout=0;
sleep(2);

buff[3]='\0';
buff1[4]='\0';
lseek( fd, 0,SEEK_SET);
	if((ret= read(fd, buff, 3)) <0){
                perror("read error:");
                close(fd);
                exit(1);
  }
printf("Buff:%s\n",buff);
	while(j !=3)
	{
		Dout =(Dout*10)+((buff[j] - 0x30));
		i++;
		j++;
	}
//---------------------------DAC--------------------

//printf(" Dout:%d\n", Dout);
Step_Size =Vref /4096;

Vin= Dout * Step_Size;
//printf("Step_Size:%.5f\n",Step_Size );
//printf("input volatege = %.2f\n", Vin);
temp = Vin *100 ;              //use y= mx+c 

printf("temp: %.2f\n", temp);
sprintf(Rx_Buff,"%f",temp);
HD44780_ClrScr();
HD44780_Str_XY(1,0,"temp:");
HD44780_Str_XY(1,7,Rx_Buff);

lseek( fd1, 0,SEEK_SET);
	if((ret= read(fd1, buff1, 4)) <0){
                perror("read error:");
                close(fd1);
                exit(1);
  	}
printf("potentio:%s\n",buff1);
HD44780_Str_XY(0,0,"potenio:");
HD44780_Str_XY(0,9,buff1);

                      
}}
