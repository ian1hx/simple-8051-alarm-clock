#include "8051.h"
#include "SEG_7.h"
char number[16] ={
				0xc0, //0
				0xf9, //1
				0xa4, //2
				0xb0, //3
				0x99, //4
				0x92, //5
				0x82, //6
				0xf8, //7
				0x80, //8
				0x90, //9s
				}; 
void delay(){
	unsigned d1,d2;
	for (d1=0;d1<30;d1++)
		for(d2=0;d2<30;d2++);
}

void display(char arr[4])
{
	P0 = number[arr[3]]; //(show  &  0xf )
	//P2 = 0xE0;	//1110 0000
	P2_7 = P2_6 = P2_5 = 1;
	P2_4 = 0;
	delay();
	P0 = number[arr[2]];
	//P2 = 0xD0;	//1101 0000
	P2_7 = P2_6 = P2_4 = 1;
	P2_5 = 0;
	delay();
	P0 = number[arr[1]];
	//P2 = 0xB0;	//1011 0000
	P2_7 = P2_5 = P2_4 = 1;
	P2_6 = 0;
	delay();
	P0 = number[arr[0]];
	//P2 = 0x70;	//0111 0000
	P2_5 = P2_6 = P2_4 = 1;
	P2_7 = 0;
	delay();
}

void display_setting(char arr[4], char pos, __bit setting_flag_blink)
{
	P0 = number[arr[3]]; //(show  &  0xf )
	//P2 = 0xE0;	//1110 0000
	P2_7 = P2_6 = P2_5 = 1;
	if(pos == 3)
		P2_4 = setting_flag_blink;
	else
		P2_4 = 0;
	delay();
	P0 = number[arr[2]];
	//P2 = 0xD0;	//1101 0000
	P2_7 = P2_6 = P2_4 = 1;
	if(pos == 2)
		P2_5 = setting_flag_blink;
	else
		P2_5 = 0;
	delay();
	P0 = number[arr[1]];
	//P2 = 0xB0;	//1011 0000
	P2_7 = P2_5 = P2_4 = 1;
	if(pos == 1)
		P2_6 = setting_flag_blink;
	else
		P2_6 = 0;
	delay();
	P0 = number[arr[0]];
	//P2 = 0x70;	//0111 0000
	P2_5 = P2_6 = P2_4 = 1;
	if(pos == 0)
		P2_7 = setting_flag_blink;
	else
		P2_7 = 0;
	delay();
}