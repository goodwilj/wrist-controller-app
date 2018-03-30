#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "gesture_handlers.h"
#include "ml_lib/knn.h"
#include "ml_lib/csvParse.h"

struct sample_info knn_info = { 7, 32, 3, 3, 0 };

int count = 0, ticks = 0, set = 1;
double avg_x = 0, avg_y = 0, avg_z = 0;

RPoint raw_point;
RPoint training_data[50];

double get_double(const char *str) {

    while (*str && !(isdigit(*str) || ((*str == '-' || *str == '+') && isdigit(*(str + 1)))))
        str++;

    return strtod(str, NULL);
}

int process_for_knn(double x, double y, double z){

    double abs_x = fabs(x);
    double abs_y = fabs(y);
    double abs_z = fabs(z);

//    printf("%lf, %lf, %lf\n", abs_x, abs_y, abs_z);

    raw_point.data_x[knn_info.count] = abs_x;
    raw_point.data_y[knn_info.count] = abs_y;
    raw_point.data_z[knn_info.count] = abs_z;
    int gesture = -1;

    if (knn_info.count++ == knn_info.number_of_features) {
        gesture = classify_knn(raw_point, training_data, knn_info.number_of_points, knn_info.number_of_features, knn_info.number_of_classes);

        knn_info.count -= 5;
        int len = sizeof(raw_point.data_x)/sizeof(raw_point.data_x[0]);
        for (int i = 0; i < len - 5; i++) {
            raw_point.data_x[i] = raw_point.data_x[i + 5];
            raw_point.data_y[i] = raw_point.data_y[i + 5];
            raw_point.data_z[i] = raw_point.data_z[i + 5];
        }
    }

    return gesture;
}

void update_coordinates(double x_deg, double y_deg){

    int x = 0, y = 0;

    // change in position = (degrees/second * seconds) * pixel multiplier
    if (y_deg > 2 || y_deg < -2) x = (int) ((y_deg * 0.05) * 28);
    if (x_deg > 2 || x_deg < -2) y = (int) ((x_deg * 0.05) * 16);

    // print coordinates of now
//    printf("X: %f, ", x_deg);
//    printf("Y: %f\n", y_deg);

    move_mouse(x, y, 1);
}

/**
 * Split the gyroscope data into x and y coordinates
 * @param buf contains the data.
 * @return 1 for success and 0 for error.
 */
int split_packet(char *buf){

    double x_deg = 0, y_deg = 0;
    double x_mag = 0, y_mag = 0, z_mag = 0;

    char *block = strtok(buf,"\r\n");
    char *token = strtok(buf, ","); // split into tokens

    while(block != NULL) {

        count = 0;
        while (token != NULL) {

            count++;
            if(count == 1)
                x_deg = get_double(token);
            else if (count == 2)
                y_deg = get_double(token);
            else if (count == 3)
                x_mag = get_double(token);
            else if (count == 4)
                y_mag = get_double(token);
            else if (count == 5)
                z_mag = get_double(token);

            token = strtok(NULL, ",");
        }
        block = strtok(NULL,"\r\n");
    }

    // cancel out extra components caused by movement
    if ((x_deg < 10 && x_deg > -10) && (y_deg < 10 && y_deg > -10)) {
        if (ticks != 4)
            ticks++;
    } else {
        ticks = 0;
    }

    // if the device is stable
    if (ticks == 4) {

        if (set) {
            avg_x = x_mag;
            avg_y = y_mag;
            avg_z = z_mag;
            set = 0;
        }

        int gesture = process_for_knn(x_mag - avg_x, y_mag - avg_y, z_mag - avg_z);
        handle_gesture(gesture);
    } else set = 1;

    update_coordinates(x_deg, y_deg);

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
    time.tv_nsec = 70000000; // 70ms poll time

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
    char *training_data_location = "../new_data/new_training_data.csv";

    printf("Getting training data...\n");
    get_training_set(training_data, knn_info.number_of_points, knn_info.number_of_features, training_data_location);

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