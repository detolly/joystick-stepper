
OPTS="-O3 -std=c++23 -I. -Ilibenjoy/src"
LINK_OPTS="-O3"

gcc $OPTS libenjoy/src/libenjoy.cpp -o libenjoy.o
gcc $OPTS libenjoy/src/libenjoy_linux.cpp -o libenjoy_linux.o

gcc $OPTS joystick.cpp -o joystick.o
gcc $OPTS stepper.cpp -o stepper.o
gcc $OPTS main.cpp -o main.o

gcc $LINK_OPTS main.o stepper.o joystick.o libenjoy.o libenjoy_linux.o -o controller
