#include "merge_utils.h"
#include "timing.h"

#include <algorithm>
#include <execution>
#include <random>
#include <thread>

using namespace std; 

vector<int> generate_vector(size_t n, unsigned int seed) {
    mt19937 rng(seed);
    uniform_int_distribution<int> dist(0, 1'000'000);

    vector<int> v;
    v.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        v.push_back(dist(rng));
    }

    sort(v.begin(), v.end());
    return v;
}

double merge_plain(const vector<int>& a,
                   const vector<int>& b) {
    vector<int> result(a.size() + b.size());

    double time_ms = measure_ms([&]() {
        merge( 
            a.begin(), a.end(),
            b.begin(), b.end(),
            result.begin()
        );
    });

    return time_ms;
}

double merge_seq(const vector<int>& a,
                 const vector<int>& b) {
    vector<int> result(a.size() + b.size());

    double time_ms = measure_ms([&]() {
        merge(
            execution::seq,
            a.begin(), a.end(),
            b.begin(), b.end(),
            result.begin()
        );
    });

    return time_ms;
}

double merge_par(const vector<int>& a,
                 const vector<int>& b) {
    vector<int> result(a.size() + b.size());

    double time_ms = measure_ms([&]() {
        merge(
            execution::par,
            a.begin(), a.end(),
            b.begin(), b.end(),
            result.begin()
        );
    });

    return time_ms;
}

double merge_par_unseq(const vector<int>& a,
                       const vector<int>& b) {
    vector<int> result(a.size() + b.size());

    double time_ms = measure_ms([&]() {
        merge(
            execution::par_unseq,
            a.begin(), a.end(),
            b.begin(), b.end(),
            result.begin()
        );
    });

    return time_ms;
}

double custom_parallel_merge(const vector<int>& a,
                             const vector<int>& b,
                             int K) {
    const size_t n = a.size();
    const size_t m = b.size();

    vector<vector<int>> partial_results(K);

    vector<thread> threads;
    threads.reserve(K);

    auto time_ms = measure_ms([&]() {

        for (int i = 0; i < K; ++i) {
            size_t a_start = n * i / K;
            size_t a_end   = n * (i + 1) / K;

            size_t b_start = m * i / K;
            size_t b_end   = m * (i + 1) / K;

            threads.emplace_back([&, a_start, a_end, b_start, b_end, i]() {
                const size_t part_size =
                    (a_end - a_start) + (b_end - b_start);

                partial_results[i].resize(part_size);

                merge(
                    a.begin() + a_start, a.begin() + a_end,
                    b.begin() + b_start, b.begin() + b_end,
                    partial_results[i].begin()
                );
            });
        }

        for (auto& t : threads) {
            t.join();
        }

        vector<int> merged = partial_results[0];

        for (int i = 1; i < K; ++i) {
            vector<int> temp(merged.size() + partial_results[i].size());

            merge(
                merged.begin(), merged.end(),
                partial_results[i].begin(), partial_results[i].end(),
                temp.begin()
            );

            merged.swap(temp);
        }
    });

    return time_ms;
}
