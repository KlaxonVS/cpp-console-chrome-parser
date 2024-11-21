#!/bin/bash


g++ cpp_ws.cpp -std=c++11 -o cpp_ws.exe -I C:\MinGW\include -L C:\MinGW\lib -I C:\dev\vcpkg\installed\x64-windows\include -L C:\dev\vcpkg\installed\x64-windows\lib -lws2_32 -lboost_thread -lboost_system  -lpthread
