#ifndef WRIST_CONTROLLER_APP_UTILS_H
#define WRIST_CONTROLLER_APP_UTILS_H

struct kinematics {
    double v0;
    double vf;
    double t;
    double a;
    double d;
};

void accel_to_velocity(struct kinematics*, double acceleration);
void velocity_to_distance(struct kinematics*);
double components_to_vector(double x, double y);
double convert_to_negative(double num);

#endif //WRIST_CONTROLLER_APP_UTILS_H
