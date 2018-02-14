#include <stdio.h>
#include <stdlib.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "gesture_handlers.h"
#include "bluetooth_handlers.h"



int main(void)
{
    printf("Starting program...\n");

    scan_for_device();
//    create_device();
//    move_mouse(-5,5);
//    mouse_right_click();
//    destroy_device();

    return 0;
}