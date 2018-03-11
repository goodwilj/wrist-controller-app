//
// Created by nbabha on 2/16/18.
//

#ifndef WRIST_CONTROLLER_APP_KNN_H
#define WRIST_CONTROLLER_APP_KNN_H



double * process_data_internal(double *, int );
struct points{
    double distance;
    int class;
};
struct raw_points{
    int class;
    double data[4];
};
typedef struct points Point;
typedef struct raw_points RPoint;
Point manhattan_distance(RPoint, RPoint, int);
Point euclidean_distance(RPoint, RPoint, int);
int compare(const void *, const void *);
int classify_knn(RPoint, RPoint *, int, int, int);
int classify_knn_internal(RPoint, RPoint *, int, int, int, int);
void calculate_frequencies(Point *, int *, int);
int determine_class(int *, int);

#endif //WRIST_CONTROLLER_APP_KNN_H