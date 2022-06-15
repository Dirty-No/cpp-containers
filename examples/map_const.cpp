#include <map>
#include <stdio.h>

int main(void) {
	std::map<const int, const int> m;

	m.insert(std::make_pair(1, 2));
	m.insert(std::make_pair(3, 4));
	m.insert(std::make_pair(5, 6));
	

}