
typedef struct {
    int _fd;

    float lx;
    float ly;
    float rx;
    float ry;

    float r2;
    float l2;

    char cross;
    char triangle;
    char square;
    char circle;

    char up;
    char left;
    char down;
    char right;
} joystick_context;

void joystick_init(joystick_context* context, int num);
void joystick_update(joystick_context* context);

