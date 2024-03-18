#include <iostream>
#include <vector>
#include <algorithm>
#include <future>
#include <iterator>

template<typename Iterator, typename Function>
void parallel_for_each(Iterator begin, Iterator end, Function func, size_t num_threads) {
    size_t size = std::distance(begin, end);

    const size_t threshold = 1000;

    if (size < threshold) {
        std::for_each(begin, end, func);
    }
    else {
        size_t half_size = size / 2;

        std::future<void> future = std::async(std::launch::async, [&] {
            parallel_for_each(begin, begin + half_size, func, num_threads);
            });

        parallel_for_each(begin + half_size, end, func, num_threads);

        future.get();
    }
}

int main() {
    std::vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    parallel_for_each(vec.begin(), vec.end(), [](int& n) {
        n *= 2;
        }, 4);

    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}

