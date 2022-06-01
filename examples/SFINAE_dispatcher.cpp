#include "utils.hpp"
#include <iostream>

// Clean alternative to enable_if

void to_be_resolved(ft::true_type) {
	std::cout << "true\n";
}

void to_be_resolved(ft::false_type) {
	std::cout << "false\n";
}


// We need to know our variable's type to use ft::is_integral on it.
// Easier way to do this is to create a template dispactcher function this way:
template<typename T>
void to_be_resolved_dispatcher(const T & __x) {
	(void) __x;
	to_be_resolved(ft::is_integral<T>());
}

int main(void)
{
	// Resolves to true
	to_be_resolved_dispatcher(50);

	// Resolves to false
	to_be_resolved_dispatcher(1.0);
}