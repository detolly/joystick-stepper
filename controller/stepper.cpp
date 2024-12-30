
#include <cmath>
#include <print>
#include <string_view>

#include <unistd.h>

#include <stepper.hpp>
#include <joystick.hpp>

enum class direction
{
    left,
    right
};

using namespace std::string_view_literals;

static void pulse(stepper_motor* motor, direction dir)
{
    std::println("[Motor {}] PULSE {}", 
                 (int)motor->axis,
                 dir == direction::right ? "RIGHT"sv : "LEFT"sv);
    motor->current_step = (motor->current_step + (dir == direction::right ? 1 : -1)) % motor->steps_per_revolution;
}

static void stepper_step(stepper_motor* motor, joystick* joystick)
{
    const auto us_per_step = (1000000 / motor->steps_per_revolution);
    const auto modifier = (motor->axis == axis::X ? joystick->x_axis : joystick->y_axis);
    const auto sleep_time = us_per_step * (1.0f - std::fabs(modifier));
    
    pulse(motor, modifier < 0.0f ? direction::left : direction::right);
    usleep(sleep_time);
}

static void stepper_reset(stepper_motor* motor)
{
    
}

void stepper_thread(stepper_motor* motor, joystick* joystick)
{
    std::println(stderr, "Starting stepper thread");
    while (!motor->exit) {
        stepper_step(motor, joystick);
    }
}

