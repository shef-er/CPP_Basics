/*
 * =====================================================================================
 *
 *       Filename:  work2.cpp
 *
 *    Description:  Для набора точек на плоскости (евклидовы расстояния между точками)
 *                  решить задачу коммивояжёра методом ветвей и границ.
 *
 *        Created:  06.12.2017 23:45:51
 *
 *         Author:  Ernest Shefer (shef-er@ya.ru)
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <iostream>
#include <functional>
#include <vector>
#include <queue>
#include <stack>
#include <utility>
#include <limits>
#include <cstdint>
#include <cstdio>
#include <cfloat>
#include <cmath>

using namespace std;

typedef pair<int, int> Vertex;
typedef vector< vector<double> > Matrix;


vector<size_t> final_path;
vector<bool> visited;
double final_res = DBL_MAX;


void
print(Matrix &input)
{
    cout << "Matrix:" << endl;
    for (auto &i : input)
    {
        for (auto &j : i)
            printf("%6.2f ", j);
        cout << endl;
    }
    cout << endl;
}


double
dist(Vertex &a, Vertex &b)
{
    return sqrt( pow(a.first - b.first, 2) + pow(a.second - b.second, 2) );
}


Matrix
vertex2Adjency(vector<Vertex> &input)
{
    size_t N = input.size();
    Matrix result(N, vector<double>(N, 0) );

    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < N; j++)
            if (i < j) {
               result[i][j] = dist(input[i], input[j]);
               result[j][i] = result[i][j];
            }

    print(result);
    return result;
}


double
firstMin(Matrix &adj, size_t i)
{
    size_t N = adj.size();
    double min = DBL_MAX;

    for (size_t j = 0; j < N; j++)
        if (adj[i][j] < min && i != j)
            min = adj[i][j];

    return min;
}


double
secondMin(Matrix &adj, size_t i)
{
    size_t N = adj.size();
    double first = DBL_MAX;
    double second = DBL_MAX;

    for (size_t j = 0; j < N; j++)
    {
        if (i == j)
            continue;

        if (adj[i][j] <= first)
        {
            second = first;
            first = adj[i][j];
        }
        else if (adj[i][j] <= second && adj[i][j] != first)
            second = adj[i][j];
    }

    return second;
}


void
TSPRec(Matrix &adj, double curr_bound, double curr_weight, size_t level,
       vector<int> curr_path)
{
    size_t N = adj.size();

    if (level == N)
    {
        if (adj[curr_path[level-1]][curr_path[0]] != 0)
        {
            double curr_res = curr_weight +
                              adj[curr_path[level-1]][curr_path[0]];

            if (curr_res < final_res)
            {
                for (size_t i = 0; i < N; i++)
                    final_path[i] = curr_path[i];

                final_path[N] = curr_path[0];
                final_res = curr_res;
            }
        }
        return;
    }

    for (size_t i = 0; i < N; i++)
    {
        if (adj[curr_path[level-1]][i] != 0 && visited[i] == false)
        {
            double temp = curr_bound;
            curr_weight += adj[curr_path[level-1]][i];

            if (level==1)
              curr_bound -= ((firstMin(adj, curr_path[level-1]) +
                              firstMin(adj, i))/2);
            else
              curr_bound -= ((secondMin(adj, curr_path[level-1]) +
                              firstMin(adj, i))/2);

            if (curr_bound + curr_weight < final_res)
            {
                curr_path[level] = i;
                visited[i] = true;

                TSPRec(adj, curr_bound, curr_weight, level+1, curr_path);
            }

            curr_weight -= adj[curr_path[level-1]][i];
            curr_bound = temp;

            visited.assign(visited.size(), false);
            for (size_t j = 0; j <= level-1; j++)
                visited[curr_path[j]] = true;
        }
    }
}


void
TSP(vector<Vertex> &nodes)
{
    size_t N = nodes.size();
    Matrix adj = vertex2Adjency(nodes);
    double curr_bound = 0;
    vector<int> curr_path(N+1, -1);

    final_path.reserve(N+1);
    visited.reserve(N);
    visited.assign(curr_path.size(), false);

    for (size_t i = 0; i < N; i++)
        curr_bound += (firstMin(adj, i) + secondMin(adj, i));

    visited[0] = true;
    curr_path[0] = 0;

    TSPRec(adj, curr_bound, 0, 1, curr_path);

    printf("Lowest cost: %4.2f\n", final_res);
    printf("Path: ");
    for (size_t i = 0; i <= N; i++)
        printf("%zu ", final_path[i]);
    cout << endl;
}


int
main()
{
    vector<Vertex> nodes {
        make_pair(1,1),  // 0
        make_pair(5,8),  // 1
        make_pair(6,3),  // 2
        make_pair(0,11), // 3
        make_pair(9,1)   // 4
    };

    TSP(nodes);

    return 0;
}

