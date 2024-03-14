#include <iostream>
#include <vector>
#include <algorithm>
#include <future>
#include <iterator>


template<typename Iterator, typename Function>
void parallel_for_each(Iterator begin, Iterator end, Function func, size_t num_threads) {
    
    size_t size = std::distance(begin, end);

   
    size_t block_size = (size + num_threads - 1) / num_threads;

    
    std::vector<std::future<void>> futures(num_threads);

   
    for (size_t i = 0; i < num_threads; ++i) {
        Iterator block_begin = begin + i * block_size;
        Iterator block_end = (i == num_threads - 1) ? end : block_begin + block_size;

        futures[i] = std::async(std::launch::async, [=] {
            std::for_each(block_begin, block_end, func);
            });
    }

 
    for (auto& future : futures) {
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
