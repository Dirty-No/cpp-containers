#include "utils.hpp"
#include <iostream>

// Clean alternative to enable_if


/* FIRST METHOD (best) */
// Avoids requirements issues

void _to_be_resolved_dispatcher(ft::true_type) {
	std::cout << "true\n";
}

void _to_be_resolved_dispatcher(ft::false_type) {
	std::cout << "false\n";
}

// We need to know our variable's type to use ft::is_integral on it.
// Easier way to do this is to create a template dispactcher function this way:
template<typename T>
void to_be_resolved(const T & __x) {
	(void) __x;
	_to_be_resolved_dispatcher(ft::is_integral<T>());
}

/* SECOND METHOD (wrong)*/

// This one will cause compile issue if T doesnt meet the requirements of one of the branches
// (e.g.: iterators)
template <typename T>
void dynamic_dispatcher(const T &) {
	if (ft::is_integral<T>()) { // Value replaced at compile time
		std::cout << "true\n";
	}
	else
		std::cout << "false\n";
}

int main(void)
{
	/* FIRST METHOD */
	// Resolves to true
	to_be_resolved(50);

	// Resolves to false
	to_be_resolved(1.0);

	/* SECOND METHOD */

	dynamic_dispatcher(50);

	dynamic_dispatcher(1.0);
}