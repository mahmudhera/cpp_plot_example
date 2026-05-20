#include <iostream>
#include <vector>
#include <random>
#include <string>

#include "external/matplotlib-cpp/matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main(int argc, char* argv[])
{
    // Default parameters
    int n_groups = 3;
    int samples_per_group = 200;
    std::string output_file = "violinplot.png";

    // Parse command line arguments
    if (argc > 1)
        n_groups = std::stoi(argv[1]);

    if (argc > 2)
        samples_per_group = std::stoi(argv[2]);

    if (argc > 3)
        output_file = argv[3];

    // Random number generators
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<std::vector<double>> data;

    // Generate distributions
    for (int g = 0; g < n_groups; ++g)
    {
        double mean = g * 2.0;

        std::normal_distribution<double> dist(mean, 1.0);

        std::vector<double> group_data;

        for (int i = 0; i < samples_per_group; ++i)
        {
            group_data.push_back(dist(gen));
        }

        data.push_back(group_data);
    }

    // Create figure
    plt::figure_size(1200, 780);

    // matplotlib-cpp raw Python call
    plt::detail::_interpreter::get();

    PyObject* pyplot = PyImport_ImportModule("matplotlib.pyplot");

    if (!pyplot)
    {
        std::cerr << "Failed to import matplotlib.pyplot\n";
        return 1;
    }

    PyObject* violinplot_func = PyObject_GetAttrString(pyplot, "violinplot");

    PyObject* py_data = PyList_New(data.size());

    for (size_t i = 0; i < data.size(); ++i)
    {
        PyObject* inner_list = PyList_New(data[i].size());

        for (size_t j = 0; j < data[i].size(); ++j)
        {
            PyList_SetItem(inner_list, j, PyFloat_FromDouble(data[i][j]));
        }

        PyList_SetItem(py_data, i, inner_list);
    }

    PyObject* args = PyTuple_Pack(1, py_data);

    PyObject_CallObject(violinplot_func, args);

    // Labels
    plt::title("Violin Plot");
    plt::xlabel("Group");
    plt::ylabel("Value");

    // Save figure
    plt::save(output_file);

    std::cout << "Saved violin plot to: " << output_file << std::endl;

    // Cleanup
    Py_DECREF(args);
    Py_DECREF(py_data);
    Py_DECREF(violinplot_func);
    Py_DECREF(pyplot);

    return 0;
}