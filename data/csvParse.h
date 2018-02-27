//
// Created by nbabha on 2/26/18.
//



#ifndef WRIST_CONTROLLER_APP_CSVPARSE_H
#define WRIST_CONTROLLER_APP_CSVPARSE_H

#include <ml_lib/knn.h>

//feed this the array, the number of training points, and the number of measurements in each point
void extract_data(RPoint *, int, int, char *);

#endif //WRIST_CONTROLLER_APP_CSVPARSE_H



