//Compiler: g++

#include <iostream>
#include <vector>
#include <thread>
#include <iomanip>

#include "merge_utils.h"

using namespace std;

int main() {
   
    vector<size_t> sizes = {
        100'000,
        500'000,
        1'000'000,
        5'000'000
    };

    int base_seed1 = 42;
    int base_seed2 = 1337;

    for (size_t n : sizes) {
        cout << "=====================================" << endl;
        cout << "N = " << n << endl;

        unsigned int seed1 = base_seed1 + static_cast<int>(n);
        unsigned int seed2 = base_seed2 + static_cast<int>(n * 2);

        vector<int> a = generate_vector(n, seed1);
        vector<int> b = generate_vector(n, seed2);

        cout << "Data generated" << endl;

        double t_plain     = merge_plain(a, b);
        double t_seq       = merge_seq(a, b);
        double t_par       = merge_par(a, b);
        double t_par_unseq = merge_par_unseq(a, b);

        cout << fixed << setprecision(4);
        cout << "merge (no policy): " << t_plain     << " ms" << endl;
        cout << "merge (seq):       " << t_seq       << " ms" << endl;
        cout << "merge (par):       " << t_par       << " ms" << endl;
        cout << "merge (par_unseq): " << t_par_unseq << " ms" << endl;
         cout << endl;

        if (n == sizes.back()) {
            unsigned hw = thread::hardware_concurrency();
            if (hw == 0) hw = 4;

            cout << "Hardware threads: " << hw << endl;
            cout << "Custom parallel merge (K vs time) for N = "
                      << n << ":\n";
            cout << "K\tTime (ms)" << endl;

            double best_time = 1e18;
            int best_K = 1;

            for (int K = 1; K <= static_cast<int>(hw * 2); ++K) {
                double tK = custom_parallel_merge(a, b, K);
                cout << K << "\t" << tK << endl;

                if (tK < best_time) {
                    best_time = tK;
                    best_K = K;
                }
            }

            cout << endl << "Best K = " << best_K
                      << ", best time = " << best_time << " ms" << endl;
            cout << "K / hardware_threads = "
                      << static_cast<double>(best_K) / hw << endl;
        }

        cout << endl;
    }

    return 0;
}
