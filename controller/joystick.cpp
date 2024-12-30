
#include <print>
#include <unistd.h>

#include <joystick.hpp>
#include <libenjoy.h>

static void joystick_connected(joystick* joystick)
{
    unsigned int counter = 0;
    libenjoy_event ev;

    std::println(stderr, "Success!\n");
    std::println(stderr, "Axes: {} btns: {}\n", libenjoy_get_axes_num(joy),libenjoy_get_buttons_num(joy));

    while(!joystick->exit)
    {
        while(libenjoy_poll(joystick->ctx, &ev))
        {
            switch(ev.type)
            {
            case LIBENJOY_EV_AXIS:
                std::println(stderr, "{}: axis {} val {}\n", ev.joy_id, ev.part_id, ev.data);
                break;
            case LIBENJOY_EV_BUTTON:
                std::println(stderr, "{}: button {} val {}\n", ev.joy_id, ev.part_id, ev.data);
                break;
            case LIBENJOY_EV_CONNECTED:
                std::println(stderr, "{}: status changed: {}\n", ev.joy_id, ev.data);
                break;
            }
        }
        sleep(16);
        /*usleep(50000);*/
        /*counter += 50;*/
        /*if(counter >= 1000)*/
        /*{*/
        /*    libenjoy_enumerate(joystick->ctx);*/
        /*    counter = 0;*/
        /*}*/
    }

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
        sleep(1000);
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
        sleep(1000);
        return;
    }

    joystick_connected(joystick);

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

    libenjoy_close(ctx); 
}

