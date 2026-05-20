#include <iostream>
#include <vector>
#include <cmath>
#include <string>

#include "external/matplotlib-cpp/matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main(int argc, char* argv[])
{
    // Default parameters
    int n_points = 100;
    double frequency = 1.0;
    std::string output_file = "plot.png";

    // Parse command line arguments
    if (argc > 1)
        n_points = std::stoi(argv[1]);

    if (argc > 2)
        frequency = std::stod(argv[2]);

    if (argc > 3)
        output_file = argv[3];

    std::vector<double> x;
    std::vector<double> y;

    for (int i = 0; i < n_points; ++i)
    {
        double xv = i * 0.1;
        double yv = std::sin(frequency * xv);

        x.push_back(xv);
        y.push_back(yv);
    }

    // Plot
    plt::figure_size(1200, 780);
    plt::plot(x, y);

    plt::title("Sine Wave");
    plt::xlabel("x");
    plt::ylabel("sin(freq * x)");

    // Save figure
    plt::save(output_file);

    std::cout << "Saved plot to: " << output_file << std::endl;

    return 0;
}