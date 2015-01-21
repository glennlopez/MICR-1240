/*---------------------------------------------------------------------------------
888b     d8888888888 .d8888b. 8888888b.     d888   .d8888b.     d8888  .d8888b. 
8888b   d8888  888  d88P  Y88b888   Y88b   d8888  d88P  Y88b   d8P888 d88P  Y88b
88888b.d88888  888  888    888888    888     888         888  d8P 888 888    888
888Y88888P888  888  888       888   d88P     888       .d88P d8P  888 888    888
888 Y888P 888  888  888       8888888P"      888   .od888P" d88   888 888    888
888  Y8P  888  888  888    888888 T88b 888888888  d88P"     8888888888888    888
888   "   888  888  Y88b  d88P888  T88b      888  888"            888 Y88b  d88P
888       8888888888 "Y8888P" 888   T88b   8888888888888888       888  "Y8888P" 

Subject:					Embeded Systems 1
Instructor:				Ken Patzel
Program:					Lab 1
Programmer:				Glenn Lopez
Description:			Interface a Character LCD to a 4 bit Z8 (Zilog)
---------------------------------------------------------------------------------*/

#include <ez8.h>

void init_ports(void);
void delay(unsigned int);

void init_lcd(void);
void soft_reset(void);
void cmd_write(unsigned char);
void data_write(unsigned char);
void lcd_ready(void);
unsigned char rd_busy(void);

void main(void)
{
init_ports();
init_lcd();
}	

/*-------------------------------------------------------*/

void init_ports(void)
{
	
	PEADDR=0X02;
	PECTL=0X00; //no alt function
	PEADDR=0X01;
	PECTL=0X00; //all output
	PEADDR=0X03;
	PECTL=0X00; //set to pushpull
	PEADDR=0X00;
return;	
}

/*-------------------------------------------------------*/

void delay(unsigned int count)
{

	unsigned int delay;
	
	for (count; count !=0; count--)
	{
		for (delay=0xA7; delay !=0; delay--)
		{
			;
		}
	}
	//while(time>0)   
	//{
	//	time--; //decrement time variable
	//	delay=0xA7; //assign value to variable delay
	//	while(delay>0) //exit loop when delay equals zero
	//	{
	//		delay--; //decrement delay
	//	}
	//}
return;	
}

/*-------------------------------------------------------*/
void init_lcd(void)
{
soft_reset();
	
lcd_ready();
cmd_write(0x28);
lcd_ready();
cmd_write(0x0F);
lcd_ready();
cmd_write(0x06);
lcd_ready();
cmd_write(0x01);
}

/*-------------------------------------------------------*/

void soft_reset(void)
{

unsigned int time;
	
time=16;
delay(time);
PEOUT=0x30;
PEOUT=0x38;
PEOUT=0x30;
	
time=5;
delay(time);
PEOUT=0x30;
PEOUT=0x38;
PEOUT=0x30;	

time=1;
delay(time);
PEOUT=0x30;
PEOUT=0x38;
PEOUT=0x30;


PEOUT=0x20;
PEOUT=0x28;
PEOUT=0x20;
}

/*-------------------------------------------------------*/

void cmd_write(unsigned char controlval)
{
	
	char highnib, lownib;
	
	highnib=controlval&0xF0;
	lownib=controlval<<4;
	
	PEOUT=0x00; //write first nib
	PEOUT=highnib;
	highnib=highnib|0x08;
	PEOUT=highnib;
	highnib=highnib&0xF7;
	PEOUT=highnib;
	
	PEOUT=0x00; //write 2nd nib
	PEOUT=lownib;
	lownib=lownib|0x08;
	PEOUT=lownib;
	lownib=lownib&0xF7;
	PEOUT=lownib;
}
/*-------------------------------------------------------*/

void data_write(unsigned char dataval)
{
	
	char highnib, lownib;
	
	highnib=dataval&0xF0;
	lownib=dataval<<4;
	
	PEOUT=0x04; //write first nib
	PEOUT=highnib;
	PEOUT=highnib|0x0C;
	PEOUT=highnib&0xF7;
	
	PEOUT=0x04; //write 2nd nib
	PEOUT=lownib;
	PEOUT=lownib|0x0C;
	PEOUT=lownib&0xF7;
}
/*-------------------------------------------------------*/

void lcd_ready(void)
{
	
	
	PEADDR=0X01;
	PECTL=0XF0; //4 lines are now I/P
	PEADDR=0X00;
	
	while(rd_busy())
		{
			rd_busy();
		}
		
	PEADDR=0X01;
	PECTL=0X00; //all output
	PEADDR=0X00;
	
}
/*-------------------------------------------------------*/

unsigned char rd_busy(void)
{
	char busyflag;
	unsigned char busy;
	busyflag=0x00;
	
	PEOUT=0x02; //set to read
	PEOUT=0x0A; //enable
	busyflag=PEIN;
	PEOUT=0x02;
	//PEOUT=0x0A; //pulse to read second nib
	//PEOUT=0x02;
	
	busyflag=busyflag&0x80;
	if (busyflag==0x80)
		busy=1;
	else 
		busy=0;
	
	return(busy);
}
/*-------------------------------------------------------*/