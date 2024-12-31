#pragma once

enum class axis
{
    X,
    Y
};

struct stepper_motor 
{
    unsigned int pulse_gpio{};
    unsigned int dir_gpio{};
    
    unsigned int current_step{};
    unsigned int steps_per_revolution{};

    axis ax{};

    bool exit{false};

    stepper_motor(unsigned int pulse_gpio, unsigned int dir_gpio, unsigned int steps_per_rev, axis ax)
        : pulse_gpio(pulse_gpio), dir_gpio(dir_gpio), steps_per_revolution(steps_per_rev), ax(ax)
    {}
};

struct joystick;
void stepper_thread(stepper_motor* motor, joystick* joystick);
