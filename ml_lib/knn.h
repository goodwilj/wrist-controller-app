//
// Created by nbabha on 2/16/18.
//

#ifndef WRIST_CONTROLLER_APP_KNN_H
#define WRIST_CONTROLLER_APP_KNN_H



int test_function(int, int);
double * process_data_internal(double *, int );
void record_training_data(double *, int);
struct points{
    double distance;
    int class;
};
struct raw_points{
    int class;
    double data[3];
};
typedef struct points Point;
typedef struct raw_points RPoint;
Point manhattan_distance(RPoint, RPoint, int);
Point euclidean_distance(RPoint, RPoint, int);
int compare(Point, Point);
Point * sort(Point *);
int classify_knn(RPoint);
int classify_knn_internal(RPoint, RPoint *, int);

#endif //WRIST_CONTROLLER_APP_KNN_H