#include "ellipse_perimeter.h"

#include <iostream>
#include <vector>

int main() {
    //initialise all weights to a random integer between 1 and 100 inclusive
    std::vector<long double> weights;
    srand(0);
    for (int i = 0; i < 5; i++) {
        long double r = rand() % 100 + 1;
        weights.push_back(r);
    }

    //run gradient descent
    gradient_descent(&weights, true, 10000, 0.023, 0.9995, 0.001);

    //output example equation usage
    std::cout << "\nPerimeter of ellipse with radii 3 and 2 using generated weights: " << ellipse_perimeter(3, 2, &weights);
    std::cout << "\nPerimeter of ellipse with radii 3 and 2 using default weights: " << ellipse_perimeter(3, 2);
    std::cout << "\nPerimeter of ellipse with radii 3 and 2 true value from google: " << "15.8654";

    return 0;
}