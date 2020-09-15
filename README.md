# Ellipse Perimeter Equation
There exists no equation for the perimeter of an ellipse (excluding infinite series). This project uses gradient descent to find an approximate equation of the form `area = pi * (wa + vb - sqrt(xa^2 + yab + zb^2))` where `w,v,x,y,z` are the weights generated in this project.

## Example
A full example of running and finding an equation can be seen in `main.cpp`, although most of the necessary code is repeated in the instructions below.

## Usage

### Running the equation
To simply run the equation using the default equation, use the method `ellipse_perimeter`:
```c++
std::cout << "Perimeter of ellipse with radii 3 and 2 using default equation: " << ellipse_perimeter(3, 2);
```

### Finding an equation
To run the gradient descent algorithm in order to try and find a better equation, use the `gradient_descent` method. You will need to pass in a `std::vector<long double>` object to write the results to.

For example,
```c++
//initialise all weights to a random integer between 1 and 100 inclusive
std::vector<long double> weights;
srand(0);
for (int i = 0; i < 5; i++) {
    long double r = rand() % 100 + 1;
    weights.push_back(r);
}

//run gradient descent
gradient_descent(&weights);
```

There are also a number of optional arguments to `gradient_descent`.
In this order, they are:
* `bool show_results` - Whether to print the results of the algorithm, defaults to `false`
* `int iterations` - The number of iterations to run through, defaults to `1000`
* `double learning_rate` - The learning rate to use (affects how far to adjust the weights each iteration), defaults to `0.01`
* `double learning_rate_modifier` - How much to decrease `learning_rate` by after each iteration, e.g `0.99` decreases `learning_rate` by 1% each iteration. Can help to avoid local minima, defaults to `1`
* `double min_learning_rate` - The minimum value for `learning_rate`, used to stop `learning_rate_modifier` from decreasing `learning_rate` too far, defaults to `0`

For example, the parameters used to generate the default are as follows:
```c++
gradient_descent(&weights, true, 1000000, 0.023, 0.9995, 0.001);
```

### Running the equation with custom weights
Once you have generated new weights with `gradient_descent`, you can run the equation using them as follows:
```c++
std::cout << "Perimeter of ellipse with radii 3 and 2 using default equation: " << ellipse_perimeter(3, 2, &weights);
```

## Notes
* More accurate approximations exist than the one found in this project, and can be found [here](https://en.wikipedia.org/wiki/Ellipse#Circumference).
* `ellipse_lengths.csv` is a slightly modified version of an list of true perimeter lengths by radii ratios created by [Matt Parker](https://www.youtube.com/user/standupmaths). You can find the original version [here](https://www.dropbox.com/s/55ffmx82zk1ezon/ellipse-lengths.xlsx?dl=0).
