#include <stdio.h>
#include <stdlib.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include "gesture_handlers.h"
#include "bluetooth_handlers.h"



int main(void) {

    struct file_descriptors files;

    printf("Creating device...\n");
    files =  create_device();

    printf("\nStarting routine...\n");

    signed char x, y;
    unsigned char data[3];

    // set select fields
    fd_set s_rd, s_wr, s_ex;
    FD_ZERO(&s_rd);
    FD_ZERO(&s_wr);
    FD_ZERO(&s_ex);
    FD_SET(files.rd, &s_rd);
    FD_SET(files.wr, &s_wr);

    // begin loop
    while(1){

        if(select(files.max + 1, &s_rd, &s_wr, &s_ex, NULL) < 0)
            return 0;
        else{
            get_mouse_coordinates(data);
            printf("x = %d, x = %d\n", data[1], data[2]);
        }
    }

    get_mouse_coordinates(data);


    printf("x = %d, y = %d", data[1], data[2]);
//
//    int i = 100;
//    while(i--)
//        move_mouse(-1,1, 1000);
//
//    sleep(1);

    printf("\nDestroying device...\n");
    destroy_device();

    return 0;
}