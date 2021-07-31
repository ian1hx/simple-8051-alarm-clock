#include "8051.h"
#include "KEYPAD.h"

char getkey(){
	char i;
	char j;
	short tmp;

/*
The column loop. i is the index of the column that starts from 0
	i	0|1|2|3
		=======
		0|1|2|3
		4|5|6|7
		8|9|A|B
		C|D|E|F
*/
/*
The column loop. i is the index of the column that starts from 0
	j

	0|	0|1|2|3
	1|	4|5|6|7
	2|	8|9|A|B
	3|	C|D|E|F
*/	
		

	for(i=0;i<4;i++){	
		P1=~(0x80 >> i);
		while((P1 & 0x0f) != 0x0f){ //If some button is pressed
			tmp = P1;
			for(j=0;j<4;j++){
				if((~tmp & ((0x08) >> j)) << j == 0x08)

					return j*4+i;
			}

		}
	}
	return 16;

}