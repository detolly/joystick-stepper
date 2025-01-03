
#include <cmath>
#include <print>

#include <pigpio.h>
#include <unistd.h>

#include <stepper.hpp>
#include <joystick.hpp>

using namespace std::string_view_literals;

static void pulse(stepper_motor* motor, direction dir)
{
    gpioWrite(motor->dir_gpio, dir == direction::left ? 1 : 0);
    gpioWrite(motor->pulse_gpio, 1);
    usleep(10);
    gpioWrite(motor->pulse_gpio, 0);
    motor->current_step = (motor->current_step + (dir == direction::right ? 1 : -1)) % motor->steps_per_revolution;
}

static void stepper_step(stepper_motor* motor, joystick* joystick)
{
    constexpr static auto threshold = 0.05f;

    while (true) {
        const auto modifier = motor->ax == axis::X ? joystick->x_axis : joystick->y_axis;
        if (fabs(modifier) < threshold)
            break;

        pulse(motor, modifier < 0.0f ? direction::left : direction::right);

        const auto sleep_time = (int)((100 - 3) * (1.0f - fabs(modifier))) + 3;
        int sleep_count{ 0 };
        while (sleep_count++ < sleep_time) {
            const auto new_modifier = (motor->ax == axis::X ? joystick->x_axis : joystick->y_axis);
            if (fabs(fabs(new_modifier) - fabs(modifier)) > 0.2)
                break;
            usleep(1000);
        }

        std::println(stderr, "Modifier: {}, sleep_time: {}", modifier, sleep_time);
    }
    usleep(1000);
}

static void stepper_reset(stepper_motor* motor)
{
    
}

void stepper_thread(stepper_motor* motor, joystick* joystick)
{
    std::println(stderr, "Starting stepper thread");
    while (!motor->exit) {
        if (joystick->connected) {
            stepper_step(motor, joystick);
        }
    }
}

