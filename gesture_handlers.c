#include "gesture_handlers.h"
#include <linux/uinput.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

int fd;

void emit(int fd, int type, int code, int val)
{
    struct input_event ie;

    ie.type = type;
    ie.code = code;
    ie.value = val;
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;

    write(fd, &ie, sizeof(ie));
}

int create_device(){

    struct uinput_setup usetup;
    if((fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK)) < 0)
        return -EINVAL;

    // enable mouse buttons and relative events
    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(fd, UI_SET_KEYBIT, KEY_C);
    ioctl(fd, UI_SET_KEYBIT, KEY_P);
    ioctl(fd, UI_SET_KEYBIT, KEY_RIGHTCTRL);
    ioctl(fd, UI_SET_KEYBIT, KEY_RIGHTALT);
    ioctl(fd, UI_SET_KEYBIT, KEY_FN_F4);

    ioctl(fd, UI_SET_EVBIT, EV_REL);
    ioctl(fd, UI_SET_RELBIT, REL_X);
    ioctl(fd, UI_SET_RELBIT, REL_Y);

    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_BLUETOOTH;
    usetup.id.vendor = 0x0001;
    usetup.id.product = 0x0001;
    strcpy(usetup.name, "Wristband Device");

    ioctl(fd, UI_DEV_SETUP, &usetup);
    ioctl(fd, UI_DEV_CREATE);

    sleep(1);
    return 1;
}

int destroy_device(){
    ioctl(fd, UI_DEV_DESTROY);
    close(fd);
}

void mouse_left_click(){
    emit(fd, EV_KEY, BTN_LEFT, 1);
    emit(fd, EV_SYN, SYN_REPORT, 0);
    emit(fd, EV_KEY, BTN_LEFT, 0);
    emit(fd, EV_SYN, SYN_REPORT, 0);
}

void mouse_right_click(){
    emit(fd, EV_KEY, BTN_RIGHT, 1);
    emit(fd, EV_SYN, SYN_REPORT, 0);
    emit(fd, EV_KEY, BTN_RIGHT, 0);
    emit(fd, EV_SYN, SYN_REPORT, 0);
}

void mouse_hold(){
    emit(fd, EV_KEY, BTN_LEFT, 1);
    emit(fd, EV_SYN, SYN_REPORT, 0);
}

void mouse_release(){
    emit(fd, EV_KEY, BTN_LEFT, 0);
    emit(fd, EV_SYN, SYN_REPORT, 0);
}

void copy(){
    emit(fd, EV_KEY, KEY_C, 0);
    emit(fd, EV_KEY, KEY_RIGHTCTRL, 0);
    emit(fd, EV_SYN, SYN_REPORT, 0);
    emit(fd, EV_KEY, KEY_C, 1);
    emit(fd, EV_KEY, KEY_RIGHTCTRL, 1);
    emit(fd, EV_SYN, SYN_REPORT, 0);
}

void paste(){
    emit(fd, EV_KEY, KEY_P, 0);
    emit(fd, EV_KEY, KEY_RIGHTCTRL, 0);
    emit(fd, EV_SYN, SYN_REPORT, 0);
    emit(fd, EV_KEY, KEY_P, 1);
    emit(fd, EV_KEY, KEY_RIGHTCTRL, 1);
    emit(fd, EV_SYN, SYN_REPORT, 0);
}

void close_window(){
    emit(fd, EV_KEY, KEY_RIGHTALT, 0);
    emit(fd, EV_KEY, KEY_FN_F4, 0);
    emit(fd, EV_SYN, SYN_REPORT, 0);
    emit(fd, EV_KEY, KEY_RIGHTALT, 1);
    emit(fd, EV_KEY, KEY_FN_F4, 1);
    emit(fd, EV_SYN, SYN_REPORT, 0);
}

void move_mouse(int x, int y){

    // TODO: need to calculate the speed the mouse moves from the accelerometer data

    int i = 100;
    while(i--){
        emit(fd, EV_REL, REL_X, x);
        emit(fd, EV_REL, REL_Y, y);
        emit(fd, EV_SYN, SYN_REPORT, 0);
        usleep(15000); // temporary
    }
}

void center_cursor(){

}

