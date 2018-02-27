#include <stdio.h>
#include <stdlib.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include "gesture_handlers.h"
#include "bluetooth_handlers.h"
#include "ml_lib/knn.h"
#include "data/csvParse.h"


int main(void)
{
    printf("Starting program...\n");
    clock_t start = clock();
    //scan_for_device();
//    create_device();
//    move_mouse(-5,5);
//    mouse_right_click();
//    destroy_device();
    //printf("%d", test_function(8, 6));
    //double test[5] = {1.3, 4.5, 8.23, 8.0, 7.0};
    //record_training_data(test, 3);
    //6.8,3.2,5.9,2.3 ---> answer : 3
    //5.0,3.6,1.4,0.2 --> answer : 1
    //5.6,2.9,3.6,1.3 --> answer: 2
    RPoint r[30];
    extract_data(r, 30, 4, "data/testData.csv");
    int correct = 0;
    int total = 0;
    int prediction = 0;
    for(int i = 0; i < 30; i++){
        prediction = classify_knn(r[i]);
        total++;
        if(prediction == r[i].class){
            correct++;
        }
    }
    double reliability = (double)correct/(double)total;
    clock_t end = clock();
    float seconds = (float)(end - start)/CLOCKS_PER_SEC;
    printf("Time to complete : %fs\n", seconds);
    printf("Percent Correct = %.1f%%", reliability*100);
    return 0;
}