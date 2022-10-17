#include "../gradient_descent.h"
#include <iostream>
#include <map>
#include <functional>
#include <cmath>
#include <string>

using namespace std;

// To help write if(verbose)
#define iv if(verbose)

bool verbose = false;
long double f(long double* x);
void debug(string name, vector<long double> x);
long double linear_regression(int length_of_data, const long double x[], const long double y[], long double args[]);
long double generic_linear(long double x, long double* args);
function<long double(long double*)> sqr_error(int length_of_data, const long double x[], const long double y[], 
    function<long double(long double, long double*)> func);
long double generic_linearv2(const long double* x, long double* args);
long double paraboloid(const long double* x, long double* args);

int main(int argc, char** argv){
    int argument_number = 0;
    char* ptr = argv[0];

    // A command map.
    map<string, function<void(char**)> > commands;
    commands["-v"] = [](char** args) -> void {verbose = true;};

    // Check if next argument is in argv:
    while(++argument_number < argc){
        if(commands.count(argv[argument_number])){
            commands[argv[argument_number]](argv + argument_number);
        }
    }


    // --------------------- FINDING MIN OF QUADRATIC ---------------------
    gradient_descent gradd(f, 1);
    gradd.set_verbose(false);
    gradd.set_iterations(5000);

    // INITIAL GUESS = 0.
    iv cout << "\n####### FINDING MIN OF QUADRATIC #######\n";
    iv cout << "f(x) = x(x - 20) + 20  --  Global min at x = 10.\n\n";
    iv cout << "TEST #1\n";
    iv debug("Initial guess: ", gradd.get_guess());
    gradd.run();
    iv debug("After gradient descent: ", gradd.get_guess());
    iv cout << "\n";
    assert(pow(gradd.get_guess()[0] - 10, 2) < 0.1);
    
    // INITIAL GUESS = -10.
    gradd.set_guess(new long double[1]{-10});
    iv cout << "TEST #2\n";
    iv debug("Initial guess: ", gradd.get_guess());
    gradd.run();
    iv debug("After gradient descent: ", gradd.get_guess());
    iv cout << "\n";
    assert(pow(gradd.get_guess()[0] - 10, 2) < 0.1);

    // INITIAL GUESS = -100.
    gradd.set_guess(new long double[1]{-100});
    iv cout << "TEST #3\n";
    iv debug("Initial guess: ", gradd.get_guess());
    gradd.run();
    iv debug("After gradient descent: ", gradd.get_guess());
    iv cout << "\n";
    assert(pow(gradd.get_guess()[0] - 10, 2) < 0.1);

    // INITIAL GUESS = 200.
    gradd.set_guess(new long double[1]{200});
    iv cout << "TEST #4\n";
    iv debug("Initial guess: ", gradd.get_guess());
    gradd.run();
    iv debug("After gradient descent: ", gradd.get_guess());
    iv cout << "\n";
    assert(pow(gradd.get_guess()[0] - 10, 2) < 0.1);



    // --------------------- LINEAR REGRESSION EXAMPLE ---------------------
    // x = 1, 5, 3, 20
    // y = 3, 10, 6, 25
    // Predicted regression: y = 1.12x + 2.87
    iv cout << "\n####### LINEAR REGRESSION #######\n";
    iv cout << "Predicted regression: y = 1.12x + 2.87\n\n";
    long double x[] = {1, 5, 3, 20};
    long double y[] = {3, 10, 6, 25};
    int length_of_data = 4;

    gradient_descent grd2([x, y, length_of_data](long double args[]) 
        -> long double{return linear_regression(length_of_data, x, y, args);}, 2);
    grd2.set_iterations(5000);
    iv cout << "TEST #1 - using linear_regression function\n";
    iv debug("Initial guess: ", grd2.get_guess());
    grd2.run();
    iv debug("After gradient descent: ", grd2.get_guess());
    assert(abs(grd2.get_guess()[0] - 1.12) < 0.01);
    assert(abs(grd2.get_guess()[1] - 2.87) < 0.01);
    iv cout << "\n";

    gradient_descent grd3(sqr_error(length_of_data, x, y, generic_linear), 2);
    grd3.set_iterations(5000);
    iv cout << "TEST #2 - using a generic sqr_error function with 1 dimensions\n";
    iv debug("Initial guess: ", grd3.get_guess());
    grd3.run();
    iv debug("After gradient descent: ", grd3.get_guess());
    assert(abs(grd3.get_guess()[0] - 1.12) < 0.01);
    assert(abs(grd3.get_guess()[1] - 2.87) < 0.01);
    iv cout << "\n";

    const long double** x2 = new const long double*[4];
    x2[0] = new long double[1]{1};
    x2[1] = new long double[1]{5};
    x2[2] = new long double[1]{3};
    x2[3] = new long double[1]{20};
    gradient_descent grd4(MSE(length_of_data, x2, y, generic_linearv2), 2);
    grd4.set_iterations(5000);
    iv cout << "TEST #3 - using a generic version 2 sqr_error\n";
    iv debug("Initial guess: ", grd4.get_guess());
    grd4.run();
    iv debug("After gradient descent: ", grd4.get_guess());
    assert(abs(grd4.get_guess()[0] - 1.12) < 0.01);
    assert(abs(grd4.get_guess()[1] - 2.87) < 0.01);
    iv cout << "\n";

    iv cout << "\n####### SOLVING MINIMUM #######\n";
    iv cout << "Predicted min: (0, 0)\n\n";
    length_of_data = 2;
    gradient_descent grd5([](long double* args) -> long double{return paraboloid(new const long double[0]{}, args);}, 2);
    grd5.set_guess(new long double[2]{10, 10});
    grd5.set_iterations(5000);
    iv debug("Initial guess: ", grd5.get_guess());
    grd5.run();
    iv debug("After gradient descent: ", grd5.get_guess());
    assert(abs(grd5.get_guess()[0]) < 0.01);
    assert(abs(grd5.get_guess()[1]) < 0.01);
}

void debug(string name, vector<long double> x){
    cout << name;
    for(auto i = x.begin(); i != x.end(); ++i){
        cout << *i << ", ";
    } 
    cout << "\n";
}

long double paraboloid(const long double x[], long double args[]){
    // ignore x.
    return args[0] * args[0] + 2 * args[1] * args[1];
}

long double linear_regression(int length_of_data, const long double x[], const long double y[], long double args[]){
    long double sum_of_error_squared = 0;

    for(int i = 0; i < length_of_data; ++i){
        sum_of_error_squared += pow(y[i] - (x[i] * args[0] + args[1]), 2);
    }

    return sum_of_error_squared;
}

function<long double(long double*)> sqr_error(int length_of_data, const long double x[], const long double y[], 
    function<long double(long double, long double*)> func){
    return [length_of_data, x, y, func](long double args[]) -> long double {
        long double sum_of_error_squared = 0;

        for(int i = 0; i < length_of_data; ++i){
            sum_of_error_squared += pow(y[i] - func(x[i], args), 2);
        }

        return sum_of_error_squared;
    };
}


long double generic_linear(long double x, long double* args){
    return x * args[0] + args[1];
}

long double generic_linearv2(const long double* x, long double* args){
    return x[0] * args[0] + args[1];
}

long double f(long double x[]){
    return x[0] * (x[0] - 20) + 20;
}

