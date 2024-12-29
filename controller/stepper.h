#pragma once

typedef struct {
    int pulse_gpio;
    int dir_gpio;
} stepper_motor_t;

typedef enum {
    DIRECTION_LEFT,
    DIRECTION_RIGHT
} direction_t;

void stepper_pulse(stepper_motor_t* motor, direction_t direction, int rpm);

