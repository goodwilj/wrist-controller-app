#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>
#include "gesture_handlers.h"


int pipe_to_knn(char *x_mag, char *y_mag, char *z_mag) {

    size_t len;

    FILE *fd_dataa = fopen("data.txt", "w");

    len = strlen(x_mag);
    len += strlen(y_mag);
    len += strlen(z_mag);
    len += 2;

    char *data = (char*) malloc(len);

    strcat(data, x_mag);
    strcat(data, ",");
    strcat(data, y_mag);
    strcat(data, ",");
    strcat(data, z_mag);

    fwrite(data, 1, len, fd_dataa);

    fclose(fd_dataa);
    free(data);
}