#include "actuator.h"

#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <bcm2835.h>
#include <stdio.h>

int file_i2c;

unsigned char buffer[60] = {0};
//~ unsigned char buffer1[60] = {0};
//~ unsigned char buffer2[60] = {0};
//~ unsigned char buffer3[60] = {0};

#define MODE1				0x00
#define MODE2				0x01
#define SUBADR1           	0x02
#define SUBADR2           	0x03
#define SUBADR3           	0x04
#define PRESCALE           	0xFE
#define LED0_ON_L          	0x06
#define LED0_ON_H          	0x07
#define LED0_OFF_L         	0x08
#define LED0_OFF_H         	0x09
#define ALL_LED_ON_L       	0xFA
#define ALL_LED_ON_H       	0xFB
#define ALL_LED_OFF_L      	0xFC
#define ALL_LED_OFF_H      	0xFD

// Bits:
#define RESTART            	0x80
#define SLEEP              	0x10
#define ALLCALL            	0x01
#define INVRT              	0x10
#define OUTDRV             	0x04

//DEADZONE
#define MIN_DRIVE			1200
#define MAX_DRIVE			1300
#define MIN_TURN			409
#define	MAX_TURN			1750

static float clamp (float value, float max, float min);

void init_actuator() {
	//----- OPEN THE I2C BUS -----
	char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return;
	}

	int addr = 0x40;          //<<<<<The I2C address of the slave
	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		//ERROR HANDLING; you can check errno to see what went wrong
		return;
	}
	
	//----- WRITE BYTES -----
	
	buffer[0] = MODE2;
	buffer[1] = OUTDRV;
	
	if (write(file_i2c, buffer, 2) != 2) {
		printf("Failed to write mode2\n");
	}
	
	buffer[0] = MODE1;
	buffer[1] = ALLCALL;
	
	if (write(file_i2c, buffer, 2) != 2) {
		printf("Failed to write mode1\n");
	}
	
	uint8_t readBuf[10] = {0};
	if (read(file_i2c, readBuf, 1) != 1) {
		printf("Fail to read\n");
	} else {
		printf("Mode 1 Data:  %i\n", readBuf[10]);
	}
	
	readBuf[0] &= ~SLEEP;
	if (write(file_i2c, readBuf, 1) != 1) {
		printf("Failed to wake up mode 1\n");
	}
}

void set_pwm(unsigned short channel, float pwm) {
	uint16_t _PWM = 0;
	channel *= 4;
	
	pwm = clamp(pwm, 1.0, 0.0);
	
	if (pwm == 0) {
		_PWM = 0;
	} else if (channel == 0) {
		_PWM = MIN_DRIVE + pwm * (MAX_DRIVE - MIN_DRIVE);
	} else {
		_PWM = MIN_TURN + pwm * (MAX_TURN - MIN_TURN);
	}
	printf("PWM is %i\n", _PWM);
	uint8_t length = 2;
	buffer[0] = LED0_ON_L + channel;
	buffer[1] = 0;
	if (write(file_i2c, buffer, length) != length)	
	{
		printf("Failed to write to the i2c bus.\n");
	}
	
	buffer[0] = LED0_ON_H + channel;
	buffer[1] = 0;
	if (write(file_i2c, buffer, length) != length)	
	{
		printf("Failed to write to the i2c bus.\n");
	}
	
	buffer[0] = LED0_OFF_L + channel;
	buffer[1] = _PWM & 0xFF;
	if (write(file_i2c, buffer, length) != length)	
	{
		printf("Failed to write to the i2c bus.\n");
	}
	
	buffer[0] = LED0_OFF_H + channel;
	buffer[1] = _PWM >> 8;
	if (write(file_i2c, buffer, length) != length)
	{
		printf("Failed to write to the i2c bus.\n");
	}
		
	
}

static float clamp ( float value, float max, float min) {
	if (value < min) {
		value = min;
	} else if (value > max) value = max;
	return value;
}

