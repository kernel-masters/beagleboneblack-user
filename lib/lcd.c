/*   LCd fle  */
/********************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h> 
#include <string.h> 
#include <assert.h>
#include <time.h>
/********************************/
#include "../inc/lcd.h"

#include "../inc/gpio.h"
/********************************/
FILE *F = NULL;
/********************************/


/********************************/
void KM_LCD_Init(void)
{
int i;
	Init_Lcd_Hardware();
	//usleep(30000);			//Tas=30mSec
	KM_GPIO_Write(LCD_EN,0);	//Clear EN
	KM_GPIO_Write(LCD_RS,0);	//Clear RS
	KM_GPIO_Write(LCD_RW,0);	//Clear RW
	usleep(10);			//Tas=3uSec
	KM_GPIO_Write(LCD_EN,1);	//Set	EN
	usleep(10);			//Tas=1uSec
	KM_GPIO_Write(LCD_EN,0);	//Clear	EN
	usleep(10);			//Tas=1uSec

	for(i=0;i<3;i++){
		LCD_Write_High_Nibble(HD44780_CMD_RESET);
		usleep(10);
	}	

	LCD_Write_High_Nibble(HD44780_CMD_FUNCTION );
	hd44780_function( HD44780_CONF_BUS, HD44780_CONF_LINES, HD44780_CONF_FONT );
	hd44780_display( HD44780_DISP_ON, HD44780_DISP_CURS_OFF, HD44780_DISP_BLINK_OFF );
	hd44780_clear();
	hd44780_entry( HD44780_ENTRY_ADDR_INC, HD44780_ENTRY_SHIFT_CURS );
	HD44780_ClrScr();
}
/*********************************/



/************************************************/
void LCD_Write_High_Nibble(unsigned char data )
{
	if(data & 0x10)
		KM_GPIO_Write(LCD_D4, 1); 
	else
		KM_GPIO_Write(LCD_D4, 0);
	if(data & 0x20) 
		KM_GPIO_Write(LCD_D5, 1);
	else 
        	KM_GPIO_Write(LCD_D5, 0);

	if(data & 0x40)
        	KM_GPIO_Write(LCD_D6, 1);
	else 
         	KM_GPIO_Write(LCD_D6, 0);

	if(data & 0x80)
        	KM_GPIO_Write(LCD_D7, 1);
	else 
         	KM_GPIO_Write(LCD_D7, 0);

	HD44780_EN(ON);
	Delay_HD44780(DELAY_EN);
	HD44780_EN(OFF);
}
/**************************************************/



/************************************************/
void LCD_Write_Low_Nibble(unsigned char data)
{
	if(data & 0x01)
        	KM_GPIO_Write(LCD_D4, 1); 
	else 
         	KM_GPIO_Write(LCD_D4, 0);
	if(data & 0x02)
        	KM_GPIO_Write(LCD_D5, 1);
	else
        	KM_GPIO_Write(LCD_D5, 0);
	if(data & 0x04)
        	KM_GPIO_Write(LCD_D6, 1);
	else
        	KM_GPIO_Write(LCD_D6, 0);
	if(data & 0x08)
        	KM_GPIO_Write(LCD_D7, 1);
	else
		KM_GPIO_Write(LCD_D7, 0);
	HD44780_EN(ON);
	Delay_HD44780(DELAY_EN);
	HD44780_EN(OFF);
}
/**************************************************/


/*************************************************/
void HD44780_EN(char Mode)
{
	if(Mode == ON){
		KM_GPIO_Write(LCD_EN,1);
	return;
	}
	KM_GPIO_Write(LCD_EN,0);
}
/************************************************/

void Delay_HD44780(unsigned int Delay)
{
	int i=0;
	for(i=0;i<Delay;i++)
		usleep(1000);
}

/**************************************************/


/* 4bit bus version */
void hd44780_write( unsigned char data )
{
	/* send the data bits - high nibble first */
	LCD_Write_High_Nibble( data );
	LCD_Write_Low_Nibble( data );
}
/**************************************************/


/**************************************************/
void hd44780_wr_cmd( unsigned char cmd )
{
	//hd44780_RS_Off();
	KM_GPIO_Write(LCD_RS,0);
	hd44780_write( cmd );
}



/**************************************************/
void hd44780_wr_data( unsigned char data )
{	//hd44780_RS_On();
	KM_GPIO_Write(LCD_RS,1);
	hd44780_write( data );
}
/**************************************************/



/***************************************************/
void Init_Lcd_Hardware(void)
{
	KM_GPIO_Export(LCD_D4);
	KM_GPIO_Export(LCD_D5);
	KM_GPIO_Export(LCD_D6);
	KM_GPIO_Export(LCD_D7);
	KM_GPIO_Export(LCD_RS);
	KM_GPIO_Export(LCD_RW);
	KM_GPIO_Export(LCD_EN);

	KM_GPIO_Direction(LCD_D4,1);//output direction
	KM_GPIO_Direction(LCD_D5,1);
	KM_GPIO_Direction(LCD_D6,1);
	KM_GPIO_Direction(LCD_D7,1);
	KM_GPIO_Direction(LCD_RS,1);
	KM_GPIO_Direction(LCD_RW,1);
	KM_GPIO_Direction(LCD_EN,1);

}
/*********************************/

void KM_LCD_Exit(void){
        KM_GPIO_Unexport(44);
        KM_GPIO_Unexport(45);
        KM_GPIO_Unexport(46);
        KM_GPIO_Unexport(47);
        KM_GPIO_Unexport(86);
        KM_GPIO_Unexport(87);
        KM_GPIO_Unexport(88);
}

/**************************************************/
void HD44780_PutChar(unsigned char c)
{
	hd44780_wr_data(c & 0xff);
}
/**************************************************/


/**************************************************/
void HD44780_GotoXY(unsigned char y, unsigned char x)
{
 unsigned char copy_y=0;
if(x > (HD44780_DISP_LENGTH-1))
	x = 0;
	if(y > (HD44780_DISP_ROWS-1))
		y = 0;
	switch(y) {
	case 0:  copy_y = 0x80; break;
	case 1:  copy_y = 0xc0; break;
	case 2:  copy_y = 0x94; break;
	case 3:  copy_y = 0xd4; break;
	 }
hd44780_wr_cmd(x + copy_y);
}
/**************************************************/




/**************************************************/
void HD44780_PutStr(char *str)
{
unsigned int i=0;
	do
	{
		HD44780_PutChar(str[i]);
		i++;
}	while(str[i]!='\0');
}
/**************************************************/


/**************************************************/
void HD44780_ClrScr(void)
{
	hd44780_wr_cmd(HD44780_CMD_CLEAR);
}
/**************************************************/



/**************************************************/
void HD44780_Str_XY(char X, char Y, char *Ptr)
{
	HD44780_GotoXY(X,Y);
	HD44780_PutStr(Ptr);
}
