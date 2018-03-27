//csvParse.c
// Created by nbabha on 2/26/18.
//Code taken from getline linux manual : http://man7.org/linux/man-pages/man3/getline.3.html
//Code taken from tutorialspoint for string delimiters : https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
//Data attempted from : https://archive.ics.uci.edu/ml/datasets/EEG+Eye+State
#include "csvParse.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//function for extracting data from CSV file and filling Raw Points array from it
void extract_data(RPoint * raw_points, int numPoints, int numReadings, char * file){

    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    FILE * data;
    char * token;

    //open CSV file
    data = fopen(file, "r");
    if(data == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    //iterate through each line in CSV file
    for(int i = 0; i < numPoints; i++){

        read = getline(&line, &len, data);
        token = strtok(line, ",");
        double entry = 0.0;

        //separate each line of CSV by comma delimiter, insert data values
        //into array of raw points passed as parameter
        for(int j = 0; j < numReadings; j++) {
            entry = strtod(token, NULL);
            raw_points[i].data_x[j] = entry;
            token = strtok(NULL, ",");
        }

        //last value of each line of CSV will be the class that data is labeled as
        int class = atoi(token);
        raw_points[i].class = class;
    }

    free(line);
    fclose(data);
}
//function for extracting data with multiple feature vectors from csv file and filling
//array of Raw Points
void extract_data_multiple(RPoint * raw_points, int numPoints, int numReadings, int numDimensions, char * file){

    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    FILE * data;
    char * token;

    //open CSV file
    data = fopen(file, "r");
    if(data == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    //iterate through each line in CSV file
    for(int i = 0; i < numPoints; i++){
        read = getline(&line, &len, data);
        token = strtok(line, ",");
        double entry = 0.0;

        //separate each line of CSV by comma delimiter, insert data values
        //into array of raw points passed as parameter
        for (int j = 0; j < numReadings; j++) {
            entry = strtod(token, NULL);
            //printf("Number : %d Reading : %f\n", j, entry);
            raw_points[i].data_x[j] = entry;
            token = strtok(NULL, ",");
        }

        read = getline(&line, &len, data);
        token = strtok(line, ",");
        for (int j = 0; j < numReadings; j++) {
            entry = strtod(token, NULL);
            raw_points[i].data_y[j] = entry;
            token = strtok(NULL, ",");
        }

        read = getline(&line, &len, data);
        token = strtok(line, ",");
        for (int j = 0; j < numReadings; j++) {
            entry = strtod(token, NULL);
            raw_points[i].data_z[j] = entry;
            token = strtok(NULL, ",");
        }

        //last value of each line of CSV will be the class that data is labeled as
        int class = atoi(token);
        //printf("Class of this point : %d", class);
        raw_points[i].class = class;
    }

    free(line);
    fclose(data);
}

