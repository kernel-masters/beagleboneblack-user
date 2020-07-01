#include<stdio.h>
#include <time.h>
#include <unistd.h>
#include "../inc/gpio.h"
#include "../inc/lcd.h"
#define LOW  0
#define HIGH 1

#define PIN  11 /* P1-18 */
#define POUT 10 /* P1-07 */


/*************************************/

#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int file;
int adapter_nr = 1;
char filename[20];
int slave_addr = 0x68;
__u8 row;
__u8 register_add;
__s32 length;
__s32 res;
__u8 buf[130];
int i;
unsigned long funcs;
/***********************************/

void LCD_Testcase(void);
int GPIO_Testcase(void);
void KM_BBB_GPIO_Unexport(void);

enum Test_Case{
	EXIT=0,
	GPIO_TEST,
	LCD_TEST,
	UART_TEST,
	I2C_SCANNING,
	ADC_TEST,
	SPI2_ETHERNET,
	SELF_DIAG
};

void KM_BBB_GPIO_Unexport(void){
	KM_GPIO_Unexport(9);
	KM_GPIO_Unexport(10);
	KM_GPIO_Unexport(11);
	KM_GPIO_Unexport(26);
	KM_GPIO_Unexport(27);
}

void KM_BBB_GPIO_Init(void){
	KM_GPIO_Export(9); // BUZZER
	KM_GPIO_Direction(9,1);// Set output direction
	KM_GPIO_Export(10); // USER LED
	KM_GPIO_Direction(10,1);//set output direction
	KM_GPIO_Export(11); // ENTER SWITCH
	KM_GPIO_Direction(11,0); // set input direction
	KM_GPIO_Export(26);//  UP SWITCH
	KM_GPIO_Direction(26,0); // set input direction
	KM_GPIO_Export(27);// DOWN SWITCH
	KM_GPIO_Direction(27,0); // Set input direction
}

void KM_BBB_Button_Testcase(void){
 	printf("Press Enter Switch\n");
	
	while(KM_GPIO_Read(11)==1);
		 KM_GPIO_Write(9,1);	 //BUZZER ON
		 sleep(1);		
	         KM_GPIO_Write(9,0);     //BUZZER OFF
                 printf("Press Down Switch\n");
	while(KM_GPIO_Read(27)==1);             
                 KM_GPIO_Write(9,1);     //BUZZER ON
                 sleep(1);                 
        	 KM_GPIO_Write(9,0);     //BUZZER OFF
		 printf("Press Up Switch\n");
	while(KM_GPIO_Read(26)==1);
                 KM_GPIO_Write(9,1);     //BUZZER ON
                 sleep(1);                                  	
        	 KM_GPIO_Write(9,0);     //BUZZER OFF
}

void I2C_Scanning(void){
        int res;
        file = open("/dev/i2c-1", O_RDWR);
        if (file < 0) {
                printf("Sorry, could not open file /dev/i2c-%d\n",1);
                exit(1);
        }
	/*
        if (ioctl(file, I2C_FUNCS, &funcs) < 0) {
                printf("Sorry could not find the supported list of functions\n");
                exit(1);
        }
	for(slave_addr=0;slave_addr<127;slave_addr++)
	{
        if (ioctl(file,I2C_SLAVE, slave_addr) < 0) {
                printf("slave Add not set");
		sleep(1);
              
        }
	res = i2c_smbus_read_byte_data(file,0x00);
           if (res < 0) {
                         printf("Read error\n");
                        // exit(1);
                       }
	   else 
	   {
		   printf("Device is Found:%d\n",slave_addr);
		   slee(1);	
		   }*/
//}
}

void SPI2Ethernet(){

	system("ifconfig");
	system("ping www.google.com");

}

int main(int argc, char *argv[])
{
	int option;
	char yes='y';

	KM_BBB_GPIO_Init();//Switches, LED, Buzzer Initalization
	KM_LCD_Init();


while(yes == 'y')
{
	printf("\n1. GPIO Test Case\n");
	printf("2. LCD Test Case\n");
	printf("3. UART - 1,2,5 Test Case \n");
	printf("4. I2C Scanning\n");
	printf("5. ADC Test Case\n");
	printf("6. SPI2Ethernet\n");
	printf("7. KM-BBB Self Diagnostic\n");
	printf("0. Exit\n");
	scanf(" %d",&option);

	switch(option)
	{
		case GPIO_TEST:
			printf("1. GPIO Test\n");
			printf("2. KM-BBB Button Test:");
			scanf("%d",&option);
			if(option==1)	
				GPIO_Testcase();
			if(option==2)
				KM_BBB_Button_Testcase();
			break;

		case LCD_TEST:
			printf("Lcd Test Case\n");
			LCD_Testcase();
			break;
		
		case UART_TEST:
			break;

		case I2C_SCANNING:
			I2C_Scanning();
			break;

		case ADC_TEST:
			break;
		
		case SPI2_ETHERNET:
			SPI2Ethernet();
			break;

		case SELF_DIAG:
			break;
		
		case EXIT:
			KM_BBB_GPIO_Unexport();
			KM_LCD_Exit();
			exit(0);
			break;

		default :
			break;
	}//switch
	//printf("do you want continue main menu \'y\'\n");
	//scanf(" %c",&yes);

}//while
	KM_BBB_GPIO_Unexport();
	KM_LCD_Exit();
	return(0);
}//main


void LCD_Testcase(void){
	HD44780_ClrScr();
	HD44780_Str_XY(0,3,"WELCOME TO");
	HD44780_Str_XY(1,0,"KERNEL MASTERS");
}

int GPIO_Testcase(void)
{
	int gpio_pin,ret;
	char status[10],yes='y';
	
	while(yes == 'y')
	{
	printf("Pls Enter input or set or clear or toggle:");
	scanf("%s",&status);

	printf("Pls Enter gpio number:");
	scanf("%d",&gpio_pin);

	if(strcmp(status,"set")==0){
		printf("set function\n");
		KM_GPIO_Write(gpio_pin,1);
	}
	else if(strcmp(status,"clear")==0){
	        printf("clear function\n");
                KM_GPIO_Write(gpio_pin,0);
        }
	else if(strcmp(status,"input")==0){
		printf("input function\n");
                ret= KM_GPIO_Read(gpio_pin);
		printf("read value:%d\n",ret);
		if((ret==0)&&((gpio_pin==11)||(gpio_pin==26)||(gpio_pin==27)))
		{
			KM_GPIO_Write(9,1);
			sleep(2);
			KM_GPIO_Write(9,0);
		}
        }
	else if(strcmp(status,"toggle")==0) {
		printf("toggle function\n");
                ret= KM_GPIO_Read(gpio_pin);
		printf("read value:%d\n",ret);
		if (ret == 0)
		KM_GPIO_Write(gpio_pin,1);
		if (ret == 1)
		KM_GPIO_Write(gpio_pin,0);
	}
	else
	printf("wrong input\n");

	printf("Do you want continue press \'y\'\n\n");
	scanf(" %c",&yes);
}//while

}
