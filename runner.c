#include <stdio.h>
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

// TODO: tweak stuctures so that the movement is reset each time. Cursor shouldnt be forced into corner.
struct kinematics x_movement ={ .v0 = 0, .t = 0.01, .d = 0 };
struct kinematics y_movement ={ .v0 = 0, .t = 0.01, .d = 0 };

void update_coordinates(double x_accel, double y_accel){

    // convert acceleration to velocity for x & y components
    accel_to_velocity(&x_movement, x_accel);
    accel_to_velocity(&y_movement, y_accel);

    // convert velocity to distance for x & y components
    velocity_to_distance(&x_movement);
    velocity_to_distance(&y_movement);
}

int split_packet(char *buf){

    double x_accel = 0, y_accel = 0, z_accel = 0;
    char position[10];

    char *token = strtok(buf, ","); // split into tokens
    int count = 0;
    while (token != NULL) {

        count++;
        if(count == 1)
            x_accel = atof(token);
        else if (count == 2)
            y_accel = atof(token);
        else if (count == 3)
            z_accel = atof(token);
        else if (count == 4)
            strcpy(position, token);

        token = strtok(NULL, ",");
    }

    update_coordinates(x_accel, y_accel);

    // coordinates to be used
    int x = (int)convert_to_negative(x_movement.d*10);
    int y = (int)convert_to_negative(y_movement.d*10);
    int speed = (int)components_to_vector(x_movement.vf, y_movement.vf);

    move_mouse(x, y, speed); // set the new coordinates relative to the old position
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
    time.tv_nsec =  10000000; // 10ms poll time

    while (1) {

        if (select(files.max + 1, &s_rd, &s_wr, &s_ex, NULL) >= 0) {
            memset(buf, 0, 1000);
            read_from_bluetooth(files.rd_bt, buf);
            printf("%s", buf);

            split_packet(buf);

            nanosleep(&time, NULL);
        }
    }
}

int main(void) {

    struct file_descriptors files;
    unsigned char data[3];
    char buf[1000];

    printf("Creating device...\n");
    files =  create_device();

    // center_cursor();

    pid_t id = fork();

    // process to track mouse movement
    if(id == 0) {
        track_mouse(files, data);
    }

    // process to handle system calls
    else {
        process_input(files, buf);
    }

//    process_input(files, buf);

    printf("\nDestroying device...\n");
    destroy_device();
    
    return 0;
}