//
// Created by nbabha on 2/16/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "knn.h"

//primitive functions
int test_function(int a, int b) {
    return a + b;
}
int compare(Point p1, Point p2){
    if(p1.distance > p2.distance){
        return 1;
    }
    return 0;
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
        printf("data1 : %f " , data1[i]);
        printf("data2 : %f\n" , data2[i]);
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
    RPoint trainingSet[2] = {
            {1, 3.5, 2.8, 2.4},
            {2, 4.0, 1.65, 2.5}
    };
    return classify_knn_internal(r,trainingSet, 2);
}
//steps for classification
//1. Using input point, create Points from each RPoint of the training set
//2. Sort Points
//3. Take first k Points from the sorted list
//4. Analyze first k Points
int classify_knn_internal(RPoint r, RPoint * training, int num){
    const int numDataPoints = 3;
    Point points[num];
    Point * sorted_points;
    for(int i = 0; i < num; i++){
        points[i] = euclidean_distance(training[i], r,numDataPoints);
    }
    printf("Points : (%d, %f) , (%d, %f)", points[0].class, points[0].distance, points[1].class, points[1].distance);
    sorted_points = sort(points);
    return 0;
}
