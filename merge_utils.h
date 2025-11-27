#pragma once

#include <vector>
#include <cstddef>

using namespace std; 

vector<int> generate_vector(size_t n, unsigned int seed);

vector<int> generate_sorted_vector(size_t n, unsigned int seed);


double merge_plain(const vector<int>& a,
                   const vector<int>& b);

double merge_seq(const vector<int>& a,
                 const vector<int>& b);

double merge_par(const vector<int>& a,
                 const vector<int>& b);

double merge_par_unseq(const vector<int>& a,
                       const vector<int>& b);

double custom_parallel_merge(const vector<int>& a,
                             const vector<int>& b,
                             int K);
