#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>
#include "gesture_handlers.h"


int pipe_to_knn(struct file_descriptors files, char *x_mag, char *y_mag, char *z_mag) {

    int fd_data = files.data;

    char data[100];

    strcat(data, x_mag);
    strcat(data, ",");
    strcat(data, y_mag);
    strcat(data, ",");
    strcat(data, z_mag);

    if(write(fd_data, &data, sizeof(data)) < 0){
        printf("ERROR: Writing to data file\n");
        return 0;
    }

    return 1;
}