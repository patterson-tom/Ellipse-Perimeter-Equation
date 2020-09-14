#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <cmath>
#include <math.h>
#include <algorithm>
#include <stdlib.h>   
#include <time.h>

//of the form pi * (wa + vb - sqrt(xa^2 + yab + zb^2)) where w,v,x,y,z are the weights
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

//returns the mean percentage accuracy of all data
double percentage_accuracy(std::map<long double, long double> *ellipse_lengths, std::vector<long double> *weights) {
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

void gradient_descent(std::map<long double, long double> *ellipse_lengths, std::vector<long double> *weights, 
                        int iterations = 1000, double learning_rate = 0.01, double learning_rate_modifier = 1) {

    //loop through each iteration and within each iteration loop through and update each weight                        
    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < 5; j++) {

            //max() used as if weights are negative then we could end up trying to sqrt a negative
            weights->at(j) = std::max(0.L, weights->at(j) - learning_rate * derivative(ellipse_lengths, weights, j));
        }

        //hopefully helps to avoid local minima
        learning_rate *= learning_rate_modifier;
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

int main() {
    //load true values of ellipse perimeters
    std::map<long double, long double> ellipse_lengths;
    load_true_values(&ellipse_lengths);

    //initialise all weights to a random integer between 1 and 100 inclusive
    std::vector<long double> weights;
    srand(0);
    for (int i = 0; i < 5; i++) {
        long double r = rand() % 100 + 1;
        weights.push_back(r);
    }

    //run gradient descent
    gradient_descent(&ellipse_lengths, &weights, 20000, 0.026, 0.9994);

    //output results
    std::cout << "Final weights: ";
    for (int i = 0; i < 5; i++) {
        std::cout << weights.at(i) << ", ";
    }
    std::cout << "\nCost function value: " << cost_function(&ellipse_lengths, &weights) << std::endl;
    std::cout << "Mean percentage accuracy: " <<percentage_accuracy(&ellipse_lengths, &weights) << std::endl;

    return 0;
}