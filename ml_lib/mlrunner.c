//mlrunner.c
// Created by nbabha on 2/27/18.
//


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
#include "knn.h"
#include "csvParse.h"


int main(void)
{
    printf("Starting program...\n");
    clock_t start = clock();
    int correct = 0;
    int total = 0;
    int prediction = 0;
    int numTrainingPoints = 18;
    int numTestPoints = 1;
    int numFeatures = 21;
    int numClasses  = 2;
    int numDimensions = 3;

    RPoint r[numTestPoints];
    RPoint training_data[numTrainingPoints];
    get_training_set(r, numTestPoints, numFeatures, "../data/PreliminaryTestData.csv");
    get_training_set(training_data, numTrainingPoints, numFeatures, "../data/PreliminaryTrainingData.csv");
    for(int i = 0; i < numTestPoints; i++){
        normalize(r, numFeatures);
        prediction = classify_knn(r[i], training_data, numTrainingPoints, numFeatures, numClasses);
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