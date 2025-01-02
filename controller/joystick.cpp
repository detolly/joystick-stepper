#include <print>

#include <unistd.h>

#include <joystick.hpp>
#include <libenjoy.h>

static void joystick_connected(joystick* joystick)
{
    unsigned int counter = 0;
    libenjoy_event ev;

    std::println(stderr, "Success!");
    std::println(stderr, "Axes: {} btns: {}", libenjoy_get_axes_num(joystick->joy),libenjoy_get_buttons_num(joystick->joy));

    bool disconnected = false;

    while(!joystick->exit && !disconnected)
    {
        while(libenjoy_poll(joystick->ctx, &ev))
        {
            switch(ev.type)
            {
            case LIBENJOY_EV_AXIS:
                if (ev.part_id == 0) {
                    joystick->x_axis = (float)ev.data / (float)32767;
                } else if (ev.part_id == 1) {
                    joystick->y_axis = -(float)ev.data / (float)32767;
                }
                break;
            case LIBENJOY_EV_BUTTON:
                break;
            case LIBENJOY_EV_CONNECTED:
                std::println(stderr, "{}: status changed: {}", ev.joy_id, ev.data);
                disconnected = true;
                break;
            }
        }
        usleep(1000);
    }

    std::println(stderr, "Controller Disconnected!");
}

static void joystick_connect(joystick* joystick)
{
    libenjoy_joy_info_list *info;

    // Updates internal list of joysticks. If you want auto-reconnect
    // after re-plugging the joystick, you should call this every 1s or so
    libenjoy_enumerate(joystick->ctx);
    info = libenjoy_get_info_list(joystick->ctx);

    if(info->count == 0)
    {
        libenjoy_free_info_list(info);
        sleep(1);
        return;
    }

    std::println("Opening joystick {}...", info->list[0]->name);
    joystick->joy = libenjoy_open_joystick(joystick->ctx, info->list[0]->id);
    joystick->id = info->list[0]->id;

    if(!joystick->joy)
    {
        std::println(stderr, "Joystick {} failed", joystick->id);
        joystick->id = -1;
        libenjoy_free_info_list(info);
        sleep(1);
        return;
    }

    joystick->connected = true;
    std::println("Joystick connected: {}", joystick->connected);
    joystick_connected(joystick);
    joystick->connected = false;

    libenjoy_close_joystick(joystick->joy);
    libenjoy_free_info_list(info);
}

void joystick_thread(joystick* joystick)
{
    std::println(stderr, "Starting joystick thread");

    libenjoy_context* ctx = libenjoy_init();
    joystick->ctx = ctx;

    while(!joystick->exit) {
        joystick_connect(joystick);
    }

    joystick->ctx = nullptr;
    libenjoy_close(ctx); 
}

