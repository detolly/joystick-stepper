
#include <pigpio.h>
#include <thread>
#include <print>

#include <unistd.h>

#include <joystick.hpp>
#include <stepper.hpp>

stepper_motor x{ 1, 2, 1000, axis::X };
stepper_motor y{ 3, 4, 1000, axis::Y };
joystick joy;

int main()
{
    std::println(stderr, "Starting motor controller");

    int rc = gpioInitialise();
    if (rc != 0) {
        std::println("Failed to initialize gpio..");
        return -1;
    }

    std::thread t_joystick{ joystick_thread, &joy };
    std::thread t_stepper_x{ stepper_thread, &x, &joy };
    std::thread t_stepper_y{ stepper_thread, &y, &joy };

    while(true) {
        if (!joy.l1_button || !joy.triangle_button) {
            usleep(16000);
            continue;
        }

        x.exit = true;
        y.exit = true;
        joy.exit = true;
        break;
    }

    t_joystick.join();
    t_stepper_x.join();
    t_stepper_y.join();

    return 0;
}
