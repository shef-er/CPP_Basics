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
#include <cstdint>
#include <functional>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include <cmath>
#include <utility>

#include <bits/stdc++.h>
using namespace std;

const size_t N = 4;

typedef pair<int, int> Vertex;
typedef vector< vector<double> > Matrix;


// final_path[] stores the final solution ie, the
// path of the salesman.
vector<size_t> final_path(N+1);

// visited[] keeps track of the already visited nodes
// in a particular path
vector<bool> visited(N, 0);

// Stores the final minimum weight of shortest tour.
double final_res = DBL_MAX;





void
print(Matrix &input)
{
    cout << "Matrix:" << endl;
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
            printf("%4.2f ", input[i][j]);
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
    Matrix result(N, vector<double>(N, 0) );

    for (size_t i = 0; i < N; i++)
        for (size_t j = 1; j < N; j++)
            if (i < j) {
               result[i][j] = dist(input[i], input[j]);
               result[j][i] = result[i][j];
            }

    return result;
}

// Function to find the minimum edge cost
// having an end at the vertex i
int
firstMin(Matrix &adj, size_t i)
{
    double min = DBL_MAX;
    for (size_t j = 0; j < N; j++)
        if (adj[i][j] < min && i != j)
            min = adj[i][j];

    return min;
}

// Function to find the second minimum edge cost
// having an end at the vertex i
int
secondMin(Matrix &adj, size_t i)
{
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


// function that takes as arguments:
// curr_bound -> lower bound of the root node
// curr_weight-> stores the weight of the path so far
// level-> current level while moving in the search
//         space tree
// curr_path[] -> where the solution is being stored which
//                would later be copied to final_path[]
void
TSPRec(Matrix &adj, double curr_bound, double curr_weight, size_t level,
       vector<int> curr_path)
{
    // base case is when we have reached level N which
    // means we have covered all the nodes once
    if (level == N)
    {
        // check if there is an edge from last vertex in
        // path back to the first vertex
        if (adj[curr_path[level-1]][curr_path[0]] != 0)
        {
            // curr_res has the total weight of the
            // solution we got
            double curr_res = curr_weight +
                              adj[curr_path[level-1]][curr_path[0]];

            // Update final result and final path if
            // current result is better.
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

    // for any other level iterate for all vertices to
    // build the search space tree recursively
    for (size_t i = 0; i < N; i++)
    {
        // Consider next vertex if it is not same (diagonal
        // entry in adjacency matrix and not visited
        // already)
        if (adj[curr_path[level-1]][i] != 0 && visited[i] == false)
        {
            double temp = curr_bound;
            curr_weight += adj[curr_path[level-1]][i];

            // different computation of curr_bound for
            // level 2 from the other levels
            if (level==1)
              curr_bound -= ((firstMin(adj, curr_path[level-1]) +
                              firstMin(adj, i))/2);
            else
              curr_bound -= ((secondMin(adj, curr_path[level-1]) +
                              firstMin(adj, i))/2);

            // curr_bound + curr_weight is the actual lower bound
            // for the node that we have arrived on
            // If current lower bound < final_res, we need to explore
            // the node further
            if (curr_bound + curr_weight < final_res)
            {
                curr_path[level] = i;
                visited[i] = true;

                // call TSPRec for the next level
                TSPRec(adj, curr_bound, curr_weight, level+1, curr_path);
            }

            // Else we have to prune the node by resetting
            // all changes to curr_weight and curr_bound
            curr_weight -= adj[curr_path[level-1]][i];
            curr_bound = temp;

            // Also reset the visited array
            visited.assign(visited.size(), false);
            for (size_t j = 0; j <= level-1; j++)
                visited[curr_path[j]] = true;
        }
    }
}

// This function sets up final_path[]
void
TSP(Matrix &adj)
{
    // Calculate initial lower bound for the root node
    // using the formula 1/2 * (sum of first min +
    // second min) for all edges.
    // Also initialize the curr_path and visited array
    double curr_bound = 0;
    vector<int> curr_path(N+1, -1);
    visited.assign(curr_path.size(), false);

    // Compute initial bound
    for (size_t i = 0; i < N; i++)
        curr_bound += (firstMin(adj, i) + secondMin(adj, i));

    // We start at vertex 1 so the first vertex
    // in curr_path[] is 0
    visited[0] = true;
    curr_path[0] = 0;

    // Call to TSPRec for curr_weight equal to
    // 0 and level 1
    TSPRec(adj, curr_bound, 0, 1, curr_path);
}


int
main()
{

    vector<Vertex> vertex {
        make_pair(1,1), // 0
        make_pair(5,8), // 1
        make_pair(6,3), // 2
        make_pair(9,1)  // 3
    };

    Matrix adj = vertex2Adjency(vertex);
    print(adj);

    TSP(adj);

    printf("Lowest cost : %4.2f\n", final_res);
    printf("Path: ");

    for (size_t i = 0; i <= N; i++)
        printf("%zu ", final_path[i]);
    cout << endl;

    return 0;
}

