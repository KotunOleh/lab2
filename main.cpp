//Kotun Oleh. Variant 4. Compiler: MSVC

#include <iostream>
#include <vector>
#include <random>
#include <execution>
#include <thread>
#include "timeit"
#include <iomanip>
#include <numeric>

using namespace std;

vector<int> randVectorGenerator(size_t size) {
    vector<int> data(size);
    mt19937 gen(random_device{}());
    uniform_int_distribution<> distrib(0, 1000000);
    generate(data.begin(), data.end(), [&]() { return distrib(gen); });
    return data;
}

bool isEven(int a) {return a % 2 == 0;}

void stdLibraryAlgo(const vector<int>& data) {
    cout << "\nResearching std library algorithm on data size: " << data.size() << "\n";
    
    long long count_result;
    cout << "- Without policies: ";
    timeit([&]() {
        count_result = count_if(data.begin(), data.end(), isEven);
    });

    cout << "- With execution::seq policy: ";
    timeit([&]() {
        count_result = count_if(execution::seq, data.begin(), data.end(), isEven);
    });

    cout << "- With execution::par policy: ";
    timeit([&]() {
        count_result = count_if(execution::par, data.begin(), data.end(), isEven);
    });

    cout << "- With execution::par_unseq policy: " << ": ";
    timeit([&]() {
        count_result = count_if(execution::par_unseq, data.begin(), data.end(), isEven);
    });
    
    cout << "Amount of even nums: " << count_result << "\n";
}

long long parallel_count_if(const vector<int>& data, size_t k) {
    if (k == 0) return 0;
    const size_t total_size = data.size();
    if (total_size == 0) return 0;

    vector<long long> partial_counts(k, 0);

    {
        vector<jthread> threads;
        threads.reserve(k);

        size_t chunk_size = total_size / k;
        size_t remainder = total_size % k;
        auto it_start = data.cbegin();

        for (size_t i = 0; i < k; ++i) {
            size_t current_chunk_size = chunk_size + (i < remainder ? 1 : 0);
            if (current_chunk_size == 0) continue;

            auto it_end = it_start;
            advance(it_end, current_chunk_size);
            
            threads.emplace_back([&partial_counts, i, it_start, it_end]() {
                partial_counts[i] = count_if(it_start, it_end, isEven);
            });

            it_start = it_end;
        }

    }
    return accumulate(partial_counts.begin(), partial_counts.end(), 0LL);
}

void test_parallel_count_if(const vector<int>& data) {
    cout << "\nResearch parallel algorithms on data size " << data.size() << "\n";
    
    const unsigned int hardware_threads = thread::hardware_concurrency();
    cout << "Amount of hardware threads: " << hardware_threads << "\n\n";

    long long count_result = 0;

    size_t max_k = min(32u, hardware_threads * 2);
    if (max_k == 0) max_k = 16; 
    for (size_t k = 1; k <= max_k; ++k) {
        cout << "K = " << k << ": ";
    
        timeit([&]() {
            count_result = parallel_count_if(data, k);
        });
    }
    cout << "Amount of even nums: " << count_result << "\n";
}

int main() {
    const vector<size_t> vector_sizes = {100000, 1000000, 10000000};

    for (const auto size : vector_sizes) {
        cout << "\nTesting for: " << size << "\n";
        
        auto data_vector = randVectorGenerator(size);
        
        stdLibraryAlgo(data_vector);
        test_parallel_count_if(data_vector);
    }
    cout << "\n\nAll tests complete. Press Enter to exit..." << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    cin.get();

    return 0; }

