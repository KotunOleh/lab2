#include <iostream>
#include <vector>
#include <random>
#include <execution>
#include "timeit"

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
    std::cout << "\nResearching std library algorithm on data size: " << data.size() << "\n";
    
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

int main() {
    auto list = randVectorGenerator(1000000);
    stdLibraryAlgo(list);
    cin.get();
}

