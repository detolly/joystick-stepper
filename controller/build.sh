
CXX_OPTS="-std=c++23 -I. -Ilibenjoy/src -c"
CC_OPTS="-I. -Ilibenjoy/src -c"
LINK_OPTS=""

gcc $CC_OPTS libenjoy/src/libenjoy.c -o libenjoy.o && \
gcc $CC_OPTS libenjoy/src/libenjoy_linux.c -o libenjoy_linux.o && \
g++ $CXX_OPTS joystick.cpp -o joystick.o && \
g++ $CXX_OPTS stepper.cpp -o stepper.o && \
g++ $CXX_OPTS main.cpp -o main.o && \
g++ $LINK_OPTS main.o stepper.o joystick.o libenjoy.o libenjoy_linux.o -o controller
