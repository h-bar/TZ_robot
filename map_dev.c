#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>

static int map_dev() {
	int fd = open("/dev/mem",O_RDWR|O_SYNC);
	if(fd < 0) {
		printf("Can't open /dev/mem\n");
		return 1;
	}

	gpio = mmap(0, 4*1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, BCM2837_GPIO_BASE);
	pwm = mmap(0, 4*1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, BCM2837_PWM_BASE);
	clk = mmap(0, 4*1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, BCM2837_CLK_BASE) ;
	
	if(gpio == NULL || pwm == NULL || clk == NULL) {
		printf("Can't mmap devices\n");
		return 1;
	}
	return 0;
}