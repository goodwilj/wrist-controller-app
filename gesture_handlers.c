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

int fd_uinput;
int fd_mice;
int max_fd;

void emit(int fd_uinput, int type, int code, int val)
{
    struct input_event ie;
    struct file_descriptors files;

    ie.type = type;
    ie.code = code;
    ie.value = val;
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;

    write(fd_uinput, &ie, sizeof(ie));
}

int connect_to_bluetooth(){

    int fd = open("/dev/rfcomm0", O_RDONLY);
    if(fd < 0 ){
        printf("ERROR: could not connect to bluetooth\n");
        return 0;
    }

    return fd;
}

int read_from_bluetooth(int fd, char *buf){

    if(read(fd, buf, sizeof(buf)) < 0){
        printf("ERROR: Reading from device\n");
        return 0;
    }

    return 1;
}

struct file_descriptors create_device(){

    struct uinput_setup usetup;
    struct file_descriptors files;

    // open file descriptor to uinput
    if((fd_uinput = open("/dev/uinput", O_WRONLY | O_NONBLOCK)) < 0){
        printf("ERROR: Opening dev/uinput...\n");
        return files;
    }

    // open file description to input/mice
    if((fd_mice = open("/dev/input/mice", O_RDONLY | O_NONBLOCK)) < 0){
        printf("ERROR: Opening dev/input/mice...\n");
        return files;
    }

    if(fd_uinput > fd_mice)
        max_fd = fd_uinput;
    else
        max_fd = fd_mice;

    // enable mouse buttons and relative events
    ioctl(fd_uinput, UI_SET_EVBIT, EV_KEY);
    ioctl(fd_uinput, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(fd_uinput, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(fd_uinput, UI_SET_KEYBIT, KEY_C);
    ioctl(fd_uinput, UI_SET_KEYBIT, KEY_P);
    ioctl(fd_uinput, UI_SET_KEYBIT, KEY_RIGHTCTRL);
    ioctl(fd_uinput, UI_SET_KEYBIT, KEY_RIGHTALT);
    ioctl(fd_uinput, UI_SET_KEYBIT, KEY_FN_F4);

    ioctl(fd_uinput, UI_SET_EVBIT, EV_REL);
    ioctl(fd_uinput, UI_SET_RELBIT, REL_X);
    ioctl(fd_uinput, UI_SET_RELBIT, REL_Y);

//    ioctl(fd_uinput, UI_SET_EVBIT, EV_ABS);
    ioctl(fd_uinput, UI_SET_ABSBIT, ABS_X);
    ioctl(fd_uinput, UI_SET_ABSBIT, ABS_Y);

    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_BLUETOOTH;
    usetup.id.vendor = 0x0001;
    usetup.id.product = 0x0001;
    strcpy(usetup.name, "Wristband Device");

    ioctl(fd_uinput, UI_DEV_SETUP, &usetup);
    ioctl(fd_uinput, UI_DEV_CREATE);

    sleep(1);

    files.rd = fd_mice;
    files.wr = fd_uinput;
    files.max = max_fd;

    return files;
}

int destroy_device(){
    ioctl(fd_uinput, UI_DEV_DESTROY);
    close(fd_uinput);
}

void mouse_left_click(){
    emit(fd_uinput, EV_KEY, BTN_LEFT, 1);
    emit(fd_uinput, EV_SYN, SYN_REPORT, 0);
    emit(fd_uinput, EV_KEY, BTN_LEFT, 0);
    emit(fd_uinput, EV_SYN, SYN_REPORT, 0);
}

void mouse_right_click(){
    emit(fd_uinput, EV_KEY, BTN_RIGHT, 1);
    emit(fd_uinput, EV_SYN, SYN_REPORT, 0);
    emit(fd_uinput, EV_KEY, BTN_RIGHT, 0);
    emit(fd_uinput, EV_SYN, SYN_REPORT, 0);
}

void mouse_hold(){
    emit(fd_uinput, EV_KEY, BTN_LEFT, 1);
    emit(fd_uinput, EV_SYN, SYN_REPORT, 0);
}

void mouse_release(){
    emit(fd_uinput, EV_KEY, BTN_LEFT, 0);
    emit(fd_uinput, EV_SYN, SYN_REPORT, 0);
}

void copy(){
    emit(fd_uinput, EV_KEY, KEY_C, 0);
    emit(fd_uinput, EV_KEY, KEY_RIGHTCTRL, 0);
    emit(fd_uinput, EV_SYN, SYN_REPORT, 0);
    emit(fd_uinput, EV_KEY, KEY_C, 1);
    emit(fd_uinput, EV_KEY, KEY_RIGHTCTRL, 1);
    emit(fd_uinput, EV_SYN, SYN_REPORT, 0);
}

void paste(){
    emit(fd_uinput, EV_KEY, KEY_P, 0);
    emit(fd_uinput, EV_KEY, KEY_RIGHTCTRL, 0);
    emit(fd_uinput, EV_SYN, SYN_REPORT, 0);
    emit(fd_uinput, EV_KEY, KEY_P, 1);
    emit(fd_uinput, EV_KEY, KEY_RIGHTCTRL, 1);
    emit(fd_uinput, EV_SYN, SYN_REPORT, 0);
}

void close_window(){
    emit(fd_uinput, EV_KEY, KEY_RIGHTALT, 0);
    emit(fd_uinput, EV_KEY, KEY_FN_F4, 0);
    emit(fd_uinput, EV_SYN, SYN_REPORT, 0);
    emit(fd_uinput, EV_KEY, KEY_RIGHTALT, 1);
    emit(fd_uinput, EV_KEY, KEY_FN_F4, 1);
    emit(fd_uinput, EV_SYN, SYN_REPORT, 0);
}

void move_mouse(int x, int y, int ms){

    // TODO: need to calculate the speed the mouse moves from the accelerometer data

    emit(fd_uinput, EV_REL, REL_X, x);
    emit(fd_uinput, EV_REL, REL_Y, y);
    emit(fd_uinput, EV_SYN, SYN_REPORT, 0);
    usleep(ms); // temporary
}

// data[1] = x-coord
// data[2] = y-coord
int get_mouse_coordinates(unsigned char *buf){

    read(fd_mice, buf, 3);
}

/*
 * x-coords: 0 -> 960
 * y-coords: 0 -> 540
 */
void center_cursor(){

    move_mouse(-1920, -1080, 0);
    move_mouse(480, 270, 0);
}

