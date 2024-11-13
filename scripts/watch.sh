#!/bin/bash

trap 'echo "Error, waiting for rebuild..."; sleep 10' ERR

cmake -B build -S . && cmake --build build && ./build/cpp_api
