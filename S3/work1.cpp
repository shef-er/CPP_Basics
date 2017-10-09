/*
 * =====================================================================================
 *
 *       Filename:  work1.cpp
 *
 *    Description:  List of neighbors <-> List of edges
 *
 *        Created:  09.10.2017 08:54:41
 *
 *         Author:  Ernest Shefer (shef-er@ya.ru)
 *
 * =====================================================================================
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
using namespace std;


//---- custom types ----//

template<typename T>
using edges_list = vector<pair<T, T>>;

// zero element is vertex and his neighbors starts from 1 index
template<typename T>
using neighbors_list = vector<vector<T>>;


//---- show list ----//

template <class T>
void
show_list(edges_list<T> input)
{
    cout << "## edges list" << endl;
    for ( auto &edge : input)
    {
        cout << "[" << edge.first << "," << edge.second << "]" << endl;
    }
    cout << endl;
}

template <class T>
void
show_list(neighbors_list<T> input)
{
    cout << "## neighbors list" << endl;
    for ( auto &vertex : input)
    {
        cout << vertex[0] << ": ";
        for ( auto neigh = vertex.begin()+1; neigh != vertex.end(); ++neigh )
        {
            cout << *neigh << " ";
        }
        cout << endl;
    }
    cout << endl;
}


//---- read from file ----//

template <class T>
edges_list<T>
read_edges(const string &file_name)
{
    edges_list<T> result;
    pair <T, T> edge;

    ifstream file(file_name);
    T line;

    while ( getline(file, line) )
    {
        istringstream iss(line);

        if ( !(iss >> edge.first >> edge.second) ) break;
        result.push_back(edge);
    }

    show_list(result);
    return result;
}

template <class T>
neighbors_list<T>
read_neighbors(const string &file_name)
{
    neighbors_list<T> result;

    ifstream file (file_name);
    string line;

    while ( getline(file, line) )
    {
        vector<T> neigh;
        istringstream iss (line);
        T vertex;

        while (iss >> vertex)
        {
            neigh.push_back(vertex);
        }
        result.push_back(neigh);
    }

    show_list(result);
    return result;
}


//---- displays ----//

template <class T>
bool
is_unique(pair<T, T> input, edges_list<T> edges)
{
    for ( auto &edge : edges)
        if ( (edge.first == input.first && edge.second == input.second) ||
             (edge.first == input.second && edge.second == input.first) )
            return false;

    return true;
}

template <class T>
bool
is_unique(T token, neighbors_list<T> neighbors)
{
    for ( auto &neigh : neighbors)
        if ( token == neigh[0] )
            return false;

    return true;
}

template <class T>
neighbors_list<T>
edges_to_neighbors(edges_list<T> input)
{
    neighbors_list<T> result;

    for ( auto &edge : input)
    {
        vector<T> neigh;
        T vertex;

        if ( is_unique(edge.first, result) )
            vertex = edge.first;
        else if ( is_unique(edge.second, result) )
            vertex = edge.second;
        else
            continue;

        neigh.push_back(vertex);
        for ( auto &bone : input)
        {
            if (bone.first == vertex)
                neigh.push_back(bone.second);
            else if (bone.second == vertex)
                neigh.push_back(bone.first);
        }

        result.push_back(neigh);
    }

    show_list(result);
    return result;
}

template <class T>
edges_list<T>
neighbors_to_edges(neighbors_list<T> input)
{
    edges_list<T> result;
    pair<T, T> edge;

    for ( auto &vertex : input)
    {
        for ( auto neigh = vertex.begin()+1; neigh != vertex.end(); ++neigh )
        {
            edge = make_pair(vertex[0], *neigh);
            if ( is_unique(edge, result) )
                result.push_back( edge );
        }
    }

    show_list(result);
    return result;
}



int
main()
{
    string mode = "0";

    cout << "choose mode:\n (0) edges to neighbors list\n (1) neighbors to edges list" << endl;
    cin >> mode;

    if (mode == "0") {
        edges_to_neighbors( read_edges<string>("edges.txt") );
    }
    else if (mode == "1") {
        neighbors_to_edges( read_neighbors<string>("neighbors.txt") );
    }

    return 0;
}

