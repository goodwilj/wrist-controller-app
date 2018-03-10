#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

double components_to_vector(double x, double y){
    return sqrt(x*x + y*y);
}

void accel_to_velocity(struct kinematics* vars, double a){

    // store old velocity
    double temp = vars->vf;

    //get final velocity
    vars->vf = vars->v0 + a*vars->t;

    // commit old velocity to old slot
    vars->v0 = temp;
}

void velocity_to_distance(struct kinematics* vars){
    vars->d = ((vars->v0 + vars->vf)/2.0) * vars->t;
}

double convert_to_negative(double num){
    return num * -1;
}
