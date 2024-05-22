#include <iostream>
#include "heapsort.hpp"

using namespace std;

void print_vector(const std::vector<int>& vec) {
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}


int main () 
{
	std::vector<int> input_list = {4, 10, 3, 5, 1};
	heapsort(input_list);
	print_vector(input_list);
	return 0;
}
