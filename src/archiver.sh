#!/bin/bash

if ! [[ -d "../build/s21_calc.app" ]]; then
    make install
    tar -cvf $1 *
else
    tar -cvf $1 *
fi
