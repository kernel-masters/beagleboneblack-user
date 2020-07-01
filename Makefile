all:km-bbb-app

km-bbb-app:src/main.o 
	arm-linux-gnueabihf-gcc   -static src/main.o  lib/kmlib.a -o  km-bbb-app

src/main.o:src/main.c
	make -C src 

kmlib:
	make -C  lib 

modules:modules/gpio
	make -C modules/gpio

clean:
	make -C modules/gpio clean
	rm src/*.o km-bbb-app lib/*.a lib/*.o 2> /dev/null
