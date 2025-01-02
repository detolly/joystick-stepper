
#include <print>
#include <thread>

#include <unistd.h>
#include <pigpio.h>

#include <joystick.hpp>
#include <stepper.hpp>

stepper_motor x{ 6, 13, 1000, axis::X };
stepper_motor y{ 19, 26, 1000, axis::Y };
joystick joy;

int main()
{
    std::println(stderr, "Starting motor controller");

    int rc = gpioInitialise();
    if (rc != 0) {
        std::printf("Failed to initialize gpio..\n");
        return -1;
    }

    gpioSetMode(6, PI_OUTPUT);
    gpioSetMode(13, PI_OUTPUT);
    gpioSetMode(19, PI_OUTPUT);
    gpioSetMode(26, PI_OUTPUT);

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
