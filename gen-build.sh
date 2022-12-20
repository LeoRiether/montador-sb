#!/bin/bash

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B build
mv build/compile_commands.json ./
