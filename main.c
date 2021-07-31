#include "8051.h"
#include "KEYPAD.h"
#include "SEG_7.h"

#define TIMER0_HBYTE 0x3c //(65536 - 50000) / 256
#define TIMER0_LBYTE 0xb0 //(65536 - 50000) % 256
#define TIMER1_HBYTE 0xff
#define TIMER1_LBYTE 0xa0

char counter_timer0 = 0;
char counter_timer0_blink = 0;
unsigned counter_timer1 = 0;
__bit setting_flag_blink = 0;
__bit alarm_set = 0;
char beep_count = 0;
char *alarm_time;
char *tmp_time;
char time[4] = {0, 0, 0, 0}; //hour_L, hour_R, min_L, min_R

void setTIME(__bit);

__bit set_mode = 0; // 0 for setting time, 1 for setting alarm


int main(){
	TH0 = TIMER0_HBYTE;
	TL0 = TIMER0_LBYTE;
	TH1 = TIMER1_LBYTE;
	TL1 = TIMER1_HBYTE;
	TMOD = 0x11; // for timer 0 and 1
	TCON = 0x10; // control for timer 0 and 1
				 // first set TR1=0
	IE = 0x8a;
	while(1){
		set_mode = 0;
		display(time);

		if(getkey() == 12){ //Press c to set time.
			P2_7 = P2_6 = P2_5 = P2_4 = 1;	//The SEG_7 will be temporarily turned off before you release the button.
			while(getkey() != 16){};
			setTIME(set_mode);
		}

		else if(getkey() == 13){ //Press d to set alarm.
			set_mode = 1;
			alarm_set = 0;
			P2_7 = P2_6 = P2_5 = P2_4 = 1;	//The SEG_7 will be temporarily turned off before you release the button.
			while(getkey() != 16){};
			setTIME(set_mode);
			alarm_set = 1;
		}
		else if(getkey() == 15){	//The alarm stops ringing.
			TR1 = 0;
			//press_alarm_flag = 1;
			alarm_set = 0;
		}

		else if(getkey() == 10){		//Pause
			TR0 = 0 ;
		}

		else if(getkey() == 11){		//Resume
			TR0 = 1 ;
		}

		if(alarm_set == 1 ){ //The alarm rings.
			if(	time[0] == *(alarm_time) 		&&	time[1] == *(alarm_time + 1) 	&&
				time[2] == *(alarm_time + 2)	&&	time[3] == *(alarm_time + 3) 	){
				TR1 = 1;

			}
		}	
	}
	// return 0; //unreachable
}

void setTIME(__bit mode){
	char digit_position = 0;   //0 is for the leftmost digit, and 3  is for the rightmost digit.
	char tmp_key = 16;
	char tmp_time[4];
	tmp_time[0]=time[0];
	tmp_time[1]=time[1];
	tmp_time[2]=time[2];
	tmp_time[3]=time[3];



	TR1 = 0;	//Turn off the alarm when it's in the setting mode.
	while(1){
		display_setting(tmp_time, digit_position, setting_flag_blink);
		tmp_key = getkey();
		if(tmp_key != 16 && tmp_key < 10){	//If some valid button has been pressed.
			switch(digit_position){			//Illegal input handling.
				case 0:
					if (tmp_key > 2)	break;
					tmp_time[digit_position++] = tmp_key;
					break;
				case 1:
					if (tmp_time[0] == 2 && tmp_key >3)	break;
					tmp_time[digit_position++] = tmp_key;
					break;
				case 2:
					if (tmp_key > 5)	break;
					tmp_time[digit_position++] = tmp_key;
					break;
				case 3:
					if (tmp_time[2] == 5 && tmp_key >9)	break;
					tmp_time[digit_position++] = tmp_key;
					break;
			}
			while(getkey() != 16){	//When the user has released the button.
				display_setting(tmp_time, digit_position, setting_flag_blink);
			}
		}
		if(digit_position == 4) break; //The user has set all the digits.	
	}
	if(mode == 0){
		time[0]=tmp_time[0];
		time[1]=tmp_time[1];
		time[2]=tmp_time[2];
		time[3]=tmp_time[3];
		TR0 = 0;
		TR0 = 1;
		TH0 = TIMER0_HBYTE;
		TL0 = TIMER0_LBYTE;
	}
	else{
		alarm_time[0]=tmp_time[0];		
		alarm_time[1]=tmp_time[1];
		alarm_time[2]=tmp_time[2];
		alarm_time[3]=tmp_time[3];	
	}

}

void timer_isr_0 (void) __interrupt(1) __using (0){ // 使用第 0 組暫存器

	counter_timer0++;
	counter_timer0_blink++;
	if(counter_timer0_blink == 5){
		setting_flag_blink = !setting_flag_blink;
		counter_timer0_blink = 0;
	}
	if(counter_timer0 == 20){
		time[3]++;
		if(time[3] > 9){
			time[3] = 0;
			time[2]++;
		}
		if(time[2] > 5){
			time[2] = 0;
			time[1]++;
		}
		if(time[1] > 9){
			time[1] = 0;
			time[0]++;
		}
		if(time[1] == 4 && time[0] == 2){
			time[0] = 0;
			time[1] = 0;
		}
		counter_timer0 = 0;
	}


	
	TH0 = TIMER0_HBYTE;
	TL0 = TIMER0_LBYTE;
}

void timer_isr_1 (void) __interrupt(3) __using (1){ // 使用第 1 組暫存器

	


	if(alarm_set == 1)
	{
		counter_timer1++;
		// if(beep_count < 4){
			if(counter_timer1 < 400){
				P2_0 = !P2_0;
			}
			else if(counter_timer1 > 1200){
				counter_timer1 = 0;
				// beep_count++;
			}
			
	 	}
	// 	else if(counter_timer1 > 2400){
	// 		counter_timer1 = 0;
	// 		beep_count = 0;
	// 	}
	// }

	TH1 = TIMER1_HBYTE;
	TL1 = TIMER1_LBYTE;
}