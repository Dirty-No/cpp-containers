#include "vector.hpp"
#include <vector>
#include <iostream>
int main(void)
{
	ft::vector<int> v;
	ft::vector<int> v2;
	std::vector<int> vv;

	std::cout << v.max_size() << " " << vv.max_size() << std::endl;
	v = v2;
}