all: motor led
	gcc -o motor motor.c
	gcc -o led led.c