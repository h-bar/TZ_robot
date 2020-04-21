all: motor led
	gcc -o motor.out motor.c
	gcc -o led.out led.c
