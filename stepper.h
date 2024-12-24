
typedef struct {
    int pulse_gpio;
    int dir_gpio;
} stepper_motor;

enum Direction {
    DIRECTION_LEFT,
    DIRECTION_RIGHT
};

void stepper_pulse(stepper_motor* motor, Direction direction, int rpm);

