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
#include "ml_lib/knn.h"



int main(void)
{
    printf("Starting program...\n");

    //scan_for_device();
//    create_device();
//    move_mouse(-5,5);
//    mouse_right_click();
//    destroy_device();
    printf("%d", test_function(8, 6));
    double test[5] = {1.3, 4.5, 8.23, 8.0, 7.0};
    record_training_data(test, 3);
    RPoint r = {1, 3.0, 3.0, 3.0};
    classify_knn(r);
    return 0;
}