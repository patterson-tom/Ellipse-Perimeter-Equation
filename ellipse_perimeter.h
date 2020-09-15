#pragma once

#include <vector>
#include <map>

double hypothesis_function(double a, double b, std::vector<long double> *w);

double cost_function(std::map<long double, long double> *ellipse_lengths, std::vector<long double> *weights);
double percentage_error(std::map<long double, long double> *ellipse_lengths, std::vector<long double> *weights);

double derivative(std::map<long double, long double> *ellipse_lengths, std::vector<long double> *w, int j);

void gradient_descent(std::map<long double, long double> *ellipse_lengths, std::vector<long double> *weights, 
                        int iterations = 1000, double learning_rate = 0.01, double learning_rate_modifier = 1, double min_learning_rate = 0);

void load_true_values(std::map<long double, long double> *ellipse_lengths);

long double ellipse_perimeter(double a, double b, std::vector<long double> *weights);
long double ellipse_perimeter(double a, double b);


