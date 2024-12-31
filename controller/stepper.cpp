
#include <cmath>
#include <print>

#include <pigpio.h>
#include <unistd.h>

#include <stepper.hpp>
#include <joystick.hpp>

using namespace std::string_view_literals;

static void pulse(stepper_motor* motor, direction dir)
{
    /*std::println("[Motor {}] PULSE {}",*/
    /*             (int)motor->ax,*/
    /*             dir == direction::right ? "RIGHT"sv : "LEFT"sv);*/
    if (motor->current_direction != dir) {
        motor->current_direction = dir;
        gpioTrigger(motor->dir_gpio, 10, 1);
    }
    gpioTrigger(motor->pulse_gpio, 10, 1);
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

        const auto sleep_time = (int)((500 - 10) * (1.0f - fabs(modifier))) + 10;
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
    std::println("here");
}

