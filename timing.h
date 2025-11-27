#pragma once

#include <chrono>

template <typename F>
double measure_ms(F&& f) {
    auto start = chrono::steady_clock::now();
    f();
    auto end = chrono::steady_clock::now();
    chrono::duration<double, milli> diff = end - start;
    return diff.count();
}
