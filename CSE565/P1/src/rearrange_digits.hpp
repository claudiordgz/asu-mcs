#ifndef REARRANGE_DIGITS_HPP
#define REARRANGE_DIGITS_HPP

#include <vector>
#include <utility>

std::pair<int, int> rearrange_digits(std::vector<int> input_list, void (*sort_fn)(std::vector<int>&));
int str_list_to_int(const std::vector<std::string>& str_list);

#endif 