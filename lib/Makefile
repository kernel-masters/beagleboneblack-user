
all:
	arm-linux-gnueabihf-gcc -c gpio.c
	arm-linux-gnueabihf-gcc -c lcd.c
	arm-linux-gnueabihf-gcc -c adc.c
	arm-linux-gnueabihf-gcc -c time_get.c
	arm-linux-gnueabihf-gcc -c time_set.c
	ar rc kmlib.a gpio.o lcd.o adc.o time_get.o time_set.o
