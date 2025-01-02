
#include <libenjoy.h>

struct joystick
{
    int id{-1};
    libenjoy_joystick* joy{nullptr};
    libenjoy_context* ctx{nullptr};

    bool l1_button{ false };
    bool triangle_button{ false };

    float x_axis{ 0 };
    float y_axis{ 0 };

    bool exit{ false };
    bool connected{ false };
};

void joystick_thread(joystick* joystick);

