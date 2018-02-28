//
// Created by nbabha on 2/16/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "knn.h"
#include "csvParse.h"

//primitive functions
int test_function(int a, int b) {
    return a + b;
}
int compare(const void *v1, const void *v2){
    Point *p1 = v1;
    Point *p2 = v2;
    if(p1->distance > p2->distance){
        return 1;
    }
    else if(p1->distance < p2->distance){
        return -1;
    }
    else{
        return 0;
    }
}
Point * sort(Point * list){
    return list;
}
//return manhattan distance b/w two raw points. r1 is the training data, r2 is the
//real time data up for classification
Point manhattan_distance(RPoint r1, RPoint r2, int size){
    double * data1 = r1.data;
    double * data2 = r2.data;
    Point p;
    double dist = 0.0;
    for(int i = 0; i < size; i++){
        dist = dist + (data1[i] - data2[i]);
    }
    p.distance = dist;
    p.class = r1.class;
    return p;
}
Point euclidean_distance(RPoint r1, RPoint r2, int size){
    double * data1 = r1.data;
    double * data2 = r2.data;
    Point p;
    double squared_distance = 0.0;
    double distance = 0.0;
    for(int i = 0; i < size; i++){
        //printf("data %f - %f", data1[i], data2[i]);
        squared_distance = squared_distance + pow((data1[i] - data2[i]), 2);
    }
    distance = sqrt(squared_distance);
    p.distance = distance;
    p.class = r1.class;
    return p;
}
//internal processing functions
double * process_data_internal(double * s, int size){
    //take the absolute value of
    for (int i; i < size; i++){
        s[i] = abs(s[i]);
    }
    //to do : implement some type of low-pass digital filter
}
void record_training_data(double * s, int class) {
    FILE *fp;
    fp = fopen("ml_lib/trainingData.txt", "a");
    fprintf(fp, "\nTraining Data");
    printf("testing....\n");
}

//classifier
int classify_knn(RPoint r){
    int numTrainingPoints = 120;
    RPoint training_data[numTrainingPoints];
    int k = 17;
    extract_data(training_data, numTrainingPoints, 4, "../data/trainingData.csv");
    return classify_knn_internal(r,training_data, numTrainingPoints, k);
}
//steps for classification
//1. Using input point, create Points from each RPoint of the training set
//2. Sort Points
//3. Take first k Points from the sorted list
//4. Analyze first k Points
int classify_knn_internal(RPoint r, RPoint * training, int num, int k){
    Point points[num];
    int numClasses = 3;
    int classes[4] = {0, 0, 0, 0};
    const int size_struct_points = 16;
    Point * sorted_points;
    for(int i = 0; i < num; i++){
        points[i] = euclidean_distance(training[i], r, 3);
    }
    qsort(points, num, size_struct_points, compare);
    for(int i = 0; i < k; i++) {
        //printf("Points : (%d, %f)\n", points[i].class, points[i].distance);
    }
    calculate_frequencies(points, classes, k);
    int determination = determine_class(classes, numClasses);
    printf("Class: %d\n", determination);

    return determination;
}
void calculate_frequencies(Point * sorted_points, int * classes, int k){
    for(int i = 0; i < k; i++){
        classes[sorted_points[i].class]++;
    }
}
int determine_class(int * classes, int num){
    int count = 0;
    int answer = 0;
    for(int i = 0; i < num + 1; i++){
        printf("Num of %ds = %d ", i, classes[i]);
        if(classes[i] > count){
            count = classes[i];
            answer = i;
        }
    }
    return answer;
}

