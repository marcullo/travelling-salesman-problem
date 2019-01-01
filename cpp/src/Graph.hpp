#ifndef TRAVELLING_SALESMAN_PROBLEM_GRAPH_H
#define TRAVELLING_SALESMAN_PROBLEM_GRAPH_H

#include <vector>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

namespace tsp
{

class Graph
{
    std::vector<std::vector<double>> costs;
    bool generated;

public:
    /*
     * @brief Load graph from file.
     *
     * @details Example:
     *          [in]               [graph size: 3]
     *          3                  [costs:]
     *          0.0 100.0 90.0     0<->1: 100.0
     *          100.0 0.0 70.0     0<->2: 90.0
     *          90.0 70.0 0.0      1<->2: 70.0
     *
     *          The first line includes graph size. Next lines include
     *          costs from the vertex (which id corresponds to
     *          line_number - 1) to other vertices (which ids
     *          corresponds to numbers of values within a line).
     *
     *          The double value informs about mentioned cost.
     *          Values are separated by a single space.
     */
    Graph(std::string filename) : generated(false) {
        std::ifstream in{filename};

        if (!in.is_open()) {
            throw std::invalid_argument("Make sure file exists!");
        }

        int size = 0;
        in >> size;
        if (!size) {
            throw std::out_of_range("Trivial graph is not being taken into consideration");
        }

        for (int i = 0; i < size; i++) {
            costs.emplace_back();
        }

        int row = 0;
        while (!in.eof()) {
            double cost;
            in >> cost;
            auto &row_costs = costs.at(row);
            row_costs.push_back(cost);
            if (row_costs.size() == (size_t) size) {
                row++;
            }
            if (row == size) {
                break;
            }
        }
    }

    /*
     * @brief Generate graph using random doubles.
     */
    Graph(int size, bool round_to_int = false) {
        if (!size) {
            throw std::out_of_range("Trivial graph is not being taken into consideration");
        }

        const double max_cost = 10.0;
        double cost;

        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        for (int i = 0; i < size; i++) {
            costs.emplace_back(size);
        }

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if ( i + j > 2 * (size - 1)) {
                    continue;
                }
                cost = (j == i) ? 0.0 : (double) std::rand() / RAND_MAX * max_cost;
                cost = round_to_int ? (int) cost : cost;
                costs[i][j] = cost;
                costs[j][i] = cost;
            }
        }

        generated = true;
    }

    bool isGenerated() const {
        return generated;
    }

    int getSize() const {
        return costs.size();
    }

    const std::vector<std::vector<double>>& getCosts() const {
        return costs;
    }

    void show() const {
        using std::cout;
        using std::endl;

        cout << "Cost matrix of graph:" << endl;
        for (const auto& row: costs) {
            for (const auto cost: row) {
                printf("%.2lf%4s", cost, "");
            } cout << endl;
        }
    }
};

} // tsp

#endif //TRAVELLING_SALESMAN_PROBLEM_GRAPH_H
