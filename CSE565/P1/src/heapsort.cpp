#include "heapsort.hpp"

#include <iostream>
#include <vector>
#include <queue>
#include <optional>
#include <stdexcept>


std::optional<int> get_max_index(const std::vector<int>& input_list, int right_index, int left_index, std::optional<int> top_index = std::nullopt) {
    int top = top_index.value_or(input_list.size());
    right_index = (right_index >= top) ? -1 : right_index;
    left_index = (left_index >= top) ? -1 : left_index;

    if (right_index == -1 && left_index == -1) {
        return std::nullopt;
    }

    if (right_index == -1) {
        return left_index;
    }

    int max_index = (input_list[right_index] > input_list[left_index]) ? right_index : left_index;
    return max_index;
}

void down_heapify(std::vector<int>& input_list, int parent_idx, std::optional<int> top_index = std::nullopt) {
    std::queue<int> q;
    q.push(parent_idx);

    while (!q.empty()) {
        int parent_index = q.front();
        q.pop();
        if (top_index.has_value() && parent_index >= top_index.value()) {
            return;
        }

        int left_index = (parent_index * 2) + 1;
        int right_index = (parent_index * 2) + 2;

        auto max_index_opt = get_max_index(input_list, right_index, left_index, top_index);
        if (!max_index_opt.has_value()) {
            continue;
        }

        int max_index = max_index_opt.value();
        if (input_list[max_index] > input_list[parent_index]) {
            std::swap(input_list[max_index], input_list[parent_index]);
            q.push(max_index);
        }
    }
}

void heapify(std::vector<int>& input_list) {
    int i = input_list.size() / 2;

    while (i > 0) {
        int right_index = i * 2;
        int left_index = (i * 2) - 1;
        int parent_index = (right_index - 1) / 2;

        auto max_index_opt = get_max_index(input_list, right_index, left_index);
        if (!max_index_opt.has_value()) {
            throw std::out_of_range("A child is expected here");
        }

        int max_index = max_index_opt.value();
        if (input_list[max_index] > input_list[parent_index]) {
            std::swap(input_list[max_index], input_list[parent_index]);
            down_heapify(input_list, max_index);
        }

        i -= 1;
    }
}

void heapsort(std::vector<int>& input_list) {
    heapify(input_list);
    int i = input_list.size() - 1;
    while (i >= 0) {
        std::swap(input_list[0], input_list[i]);
        down_heapify(input_list, 0, i);
        i -= 1;
    }
    std::reverse(input_list.begin(), input_list.end());
}
