#ifndef WRIST_CONTROLLER_APP_KNN_H
#define WRIST_CONTROLLER_APP_KNN_H

struct sample_info {

    int number_of_features;
    int number_of_points;
    int number_of_classes;
    int number_of_dimensions;
    int count;
};

typedef struct points{
    double distance;
    int class;
} Point;

typedef struct raw_points{
    int class;
    double data_x[21];
    double data_y[21];
    double data_z[21];
} RPoint;

int compare(const void *, const void *);
int classify_knn(RPoint, RPoint *, int, int, int);
int classify_knn_internal(RPoint, RPoint *, int, int, int, int);
void calculate_frequencies(Point *, int *, int);
int determine_class(int *, int);
double minimum(double, double, double);
Point dtw(RPoint, RPoint, int);

#endif //WRIST_CONTROLLER_APP_KNN_H