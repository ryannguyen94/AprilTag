void init_actuator(void);

void set_pwm(unsigned short channel, float pwm);

void set_raw_pwm(unsigned short channel, int pwm);

void init_throttle(unsigned short channel);

void stop(unsigned short channel);

void brake(unsigned short channel);

void disable(void);
