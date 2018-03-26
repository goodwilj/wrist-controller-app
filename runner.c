#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <strings.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include "gesture_handlers.h"
#include "utils.h"

double get_double(const char *str) {

    while (*str && !(isdigit(*str) || ((*str == '-' || *str == '+') && isdigit(*(str + 1)))))
        str++;

    return strtod(str, NULL);
}

void update_coordinates(double x_deg, double y_deg){

    int x = 0, y = 0;

    // change in position = (degrees/second * seconds) * pixel multiplier
    if (y_deg > 2 || y_deg < -2) x = (int) ((y_deg * 0.05) * 20);
    if (x_deg > 2 || x_deg < -2) y = (int) ((x_deg * 0.05) * 12);

    // print coordinates of now
    printf("X: %f, ", x_deg);
    printf("Y: %f\n", y_deg);

    move_mouse(x, y, 1);
}

/**
 * Split the gyroscope data into x and y coordinates
 * @param buf contains the data.
 * @return 1 for success and 0 for error.
 */
int split_packet(char *buf){

    double x_deg = 0, y_deg = 0;
    char *x_mag = (char *)malloc(10);
    char *y_mag = (char *)malloc(10);
    char *z_mag = (char *)malloc(10);

    char *block = strtok(buf,"\r\n");
    char *token = strtok(buf, ","); // split into tokens
    int count;

    while(block != NULL) {

        count = 0;
        while (token != NULL) {

            count++;
            if(count == 1)
                x_deg = get_double(token);
            else if (count == 2)
                y_deg = get_double(token);
            else if (count == 3)
                x_mag = token;
            else if (count == 4)
                y_mag = token;
            else if (count == 5)
                z_mag = token;

            token = strtok(NULL, ",");
        }
        block = strtok(NULL,"\r\n");
    }

    pipe_to_knn(x_mag, y_mag, z_mag);
    update_coordinates(x_deg, y_deg);

//    free(x_mag);
//    free(y_mag);
//    free(z_mag);

    return 1;
}

/**
 * Process to handle tracking all mouse movements.
 * @param files: struct to keep track of used file pointers for select
 * @param data: pointer to array to store the coordinates
 */
void track_mouse(struct file_descriptors files, unsigned char *data){

    // set select fields for system input
    fd_set s_rd, s_wr, s_ex;
    FD_ZERO(&s_rd);
    FD_ZERO(&s_wr);
    FD_ZERO(&s_ex);
    FD_SET(files.rd_sys, &s_rd);
    FD_SET(files.rd_bt, &s_rd);
    FD_SET(files.wr, &s_wr);

    while(1){

        if(select(files.max + 1, &s_rd, &s_wr, &s_ex, NULL) >= 0) {
            get_mouse_coordinates(data);
        }
    }
}

/**
 * Process to receive bluetooth packets from device and pass them off to handlers.
 * @param files: struct to hold file pointers for select
 * @param buf: pointer to array to store incoming data
 */
void process_input(struct file_descriptors files, char *buf) {

    // set select fields for bluetooth
    fd_set s_rd, s_wr, s_ex;
    FD_ZERO(&s_rd);
    FD_ZERO(&s_wr);
    FD_ZERO(&s_ex);
    FD_SET(files.rd_bt, &s_rd);
    FD_SET(files.wr, &s_wr);

    struct timespec time;
    time.tv_sec = 0;
    time.tv_nsec = 60000000; // 60ms poll time

    write_to_bluetooth(files.rd_bt, 1);

    while (1) {

        if (select(files.max + 1, &s_rd, &s_wr, &s_ex, NULL) >= 0) {
            memset(buf, 0, 40);

            // read packet from the device
            read_from_bluetooth(files.rd_bt, buf);

            // process the packet
            split_packet(buf);

            // sleep for 60 ms
            nanosleep(&time, NULL);

            // relay that the packet was received
            write_to_bluetooth(files.rd_bt, 1);
        }
    }
}

int main() {

    struct file_descriptors files;
    unsigned char data[3];
    char buf[40];

    printf("Creating device...\n");
    files =  create_device();

    center_cursor();

    pid_t id = fork();

    // process to track mouse movement
    if(id == 0) {
        track_mouse(files, data);
    }

    // process to handle system calls
    else {
        process_input(files, buf);
    }

    printf("\nDestroying device...\n");
    destroy_device();
    
    return 0;
}