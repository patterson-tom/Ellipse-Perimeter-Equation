#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <cmath>
#include <math.h>
#include <algorithm>

#include "ellipse_perimeter.h"

//of the form y = pi * (wa + vb - sqrt(xa^2 + yab + zb^2)) where w,v,x,y,z are the weights
double hypothesis_function(double a, double b, std::vector<long double> *w) {
    return 3.141592654 * (w->at(0) * a + w->at(1) * b - sqrt(w->at(2) * a * a + w->at(3) * a * b + w->at(4) * b * b));
}

//the mean of the squares of the absolute errors
double cost_function(std::map<long double, long double> *ellipse_lengths, std::vector<long double> *weights) {
    long double sum = 0;
    for (std::map<long double, long double>::iterator it = ellipse_lengths->begin(); it != ellipse_lengths->end(); it++) {
        sum += pow(it->second - hypothesis_function(it->first, 1, weights), 2);
    }
    return sum / ellipse_lengths->size();
}

//returns the mean percentage error of all data
double percentage_error(std::map<long double, long double> *ellipse_lengths, std::vector<long double> *weights) {
    long double sum = 0;
    for (std::map<long double, long double>::iterator it = ellipse_lengths->begin(); it != ellipse_lengths->end(); it++) {
        double diff = abs(it->second - hypothesis_function(it->first, 1, weights));
        sum += diff / it->second;
    }
    return sum / ellipse_lengths->size();
}

//returns the derivate of the cost function with respect to weight j
double derivative(std::map<long double, long double> *ellipse_lengths, std::vector<long double> *w, int j) {

    long double sum = 0;
    for (std::map<long double, long double>::iterator it = ellipse_lengths->begin(); it != ellipse_lengths->end(); it++) {

        long double a = it->first, b = 1;
        long double h = hypothesis_function(a, b, w), diff = it->second - h;
        
        //each weight obviously has a different derivate so we switch on j to select the correct equation
        switch (j) {
            case 0: {
                sum += -a * diff;
                break;
            }
            case 1: {
                sum += -b * diff;
                break;
            }
            case 2: {
                long double denominator = sqrt(w->at(2) * a * a + w->at(3) * a * b + w->at(4) * b * b);
                sum += ((0.05 * a * a) / denominator) * diff;
                break;
            }
            case 3: {
                long double denominator = sqrt(w->at(2) * a * a + w->at(3) * a * b + w->at(4) * b * b);
                sum += ((0.5 * a * b) / denominator) * diff;
                break;
            }
            case 4: {
                long double denominator = sqrt(w->at(2) * a * a + w->at(3) * a * b + w->at(4) * b * b);
                sum += ((0.5 * b * b) / denominator) * diff;
                break;
            }
        }
    }

    return sum / ellipse_lengths->size();
}

//best weights seen: {42.3783, 15.4938, 1690.18, 1257.11, 169.231}
//parameters: {1000000, 0.023, 0.9995, 0.001}
//cost function: 0.000852572
//Mean percentage error: 0.111765%
void gradient_descent(std::map<long double, long double> *ellipse_lengths, std::vector<long double> *weights, 
                        int iterations, double learning_rate, double learning_rate_modifier, double min_learning_rate) {

    //loop through each iteration and within each iteration loop through and update each weight                        
    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < 5; j++) {

            //max() used as if weights are negative then we could end up trying to sqrt a negative
            weights->at(j) = std::max(0.L, weights->at(j) - learning_rate * derivative(ellipse_lengths, weights, j));
        }

        //hopefully helps to avoid local minima
        if (learning_rate > min_learning_rate) {
            learning_rate *= learning_rate_modifier;
        }
    }
}

void load_true_values(std::map<long double, long double> *ellipse_lengths) {
    //open file
    std::fstream fin;
    fin.open("./ellipse-lengths.csv", std::ios::in);
    std::string line, word;

    //loop through every line in file
    while (fin >> line) {

        std::stringstream ss(line);

        //first column is the ratio between the ellipses' two radii
        std::getline(ss, word, ',');
        long double ratio = std::stod(word);

        //second column is the true perimeter value
        std::getline(ss, word, ',');
        long double length = std::stod(word);

        (*ellipse_lengths)[ratio] = length;
    }
}

//actually uses the equation to calculate the perimeter of a given ellipse
long double ellipse_perimeter(double a, double b, std::vector<long double> *weights) {
    double max = std::max(a, b), min = std::min(a, b);
    return min * hypothesis_function(max / min, 1, weights);
}

//uses default weights
long double ellipse_perimeter(double a, double b) {
    double max = std::max(a, b), min = std::min(a, b);
    std::vector<long double> weights = {42.3783, 15.4938, 1690.18, 1257.11, 169.231};
    return min * hypothesis_function(max / min, 1, &weights);
}