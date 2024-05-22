#include "rearrange_digits.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include <numeric>
#include <iterator>

int str_list_to_int(const std::vector<std::string>& str_list) {
    std::string concatenated = std::accumulate(str_list.begin(), str_list.end(), std::string{});
    return std::stoi(concatenated);
}

std::pair<int, int> rearrange_digits(std::vector<int> input_list, void (*sort_fn)(std::vector<int>&)) {
    sort_fn(input_list);
    size_t n = input_list.size();
    size_t i = (n % 2 == 0) ? n / 2 : (n / 2) + 1;
    size_t j = n / 2;
    std::vector<std::string> results[2];
    
    for (const auto& el : input_list) {
        if (i >= j) {
            results[0].push_back(std::to_string(el));
            --i;
        } else {
            results[1].push_back(std::to_string(el));
            --j;
        }
    }
    return {str_list_to_int(results[0]), str_list_to_int(results[1])};
}
