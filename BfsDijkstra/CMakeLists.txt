cmake_minimum_required(VERSION 3.4)
project(dijkstra_alg)
add_subdirectory(pybind11)
pybind11_add_module(bfsDijkstraModule BfsDijkstra.cpp)