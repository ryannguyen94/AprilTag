/*
 * calibrate.cpp
 * 
 * Copyright 2018  <pi@raspberrypi>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <iostream>
#include <sys/time.h>
#include "actuator.h"
#include <bcm2835.h>

using namespace std;

int main(int argc, char **argv)
{
	init_actuator();
	set_pwm(0, 0);
	
	while (1) {
		int channel, pwm;
		cout << "Enter channel to calibrate: " << endl;
		cin >> channel;
		if (channel != 0 && channel != 1) break;
		while (1) {
			cout << "Enter pwm: " << endl;
			cin >> pwm;
			if (pwm == -9999) break;
			set_raw_pwm(channel, pwm);
		}
	}
	
	return 0;
}

