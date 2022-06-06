#include "utils.hpp"
#include <iostream>

// https://en.cppreference.com/w/cpp/types/enable_if
// https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/types/enable_if.html

// Using enable_if allows you to avoid writing one (1) dispatcher function
// 	at the cost of an absolutely unreadable syntax
// 	hooray!

// Literally the most awful fucking syntax hack ever
template<typename T>
void to_be_resolved(
	T, typename ft::enable_if<ft::is_integral<T>::value >::type* = 0
)
{
	std::cout << "true\n";
}

// Why would they do this
template<typename T>
void to_be_resolved(
	T, typename ft::enable_if<!ft::is_integral<T>::value >::type* = 0
)
{
	std::cout << "false\n";
}

int main(void)
{
	to_be_resolved(1);
	to_be_resolved(0.5);
}
