%.rel : %.c %.h
	sdcc -c $<

OBJS = *.asm *.hex *.ihx *.lk *.lst *.map *.mem *.rel *.rst *.sym

all : main.hex

main.hex : main.ihx
	packihx main.ihx > main.hex

main.ihx : main.c myLib.lib
	sdcc main.c -l myLib.lib

myLib.lib : SEG_7.rel KEYPAD.rel
	sdar -o myLib.lib -rc SEG_7.rel KEYPAD.rel

clean :
	del $(OBJS) myLib.lib