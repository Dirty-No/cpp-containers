#include <iostream>
#include <memory>
#include <stdio.h>

struct base_print_constructor {
	base_print_constructor() {
		std::cout << "base_print_constructor" << std::endl;
	}
};

struct derived_print_constructor {
	derived_print_constructor() {
		std::cout << "derived_print_constructor" << std::endl;
	}
};

struct print_struct_base
{
	base_print_constructor p;
	int a;
	int b;
};


template<typename T>
struct print_struct : print_struct_base
{
	derived_print_constructor q;
	T c;

	print_struct() : c(0) {
	}
};

template<typename T>
struct test_struct {

	print_struct_base *s;
	
	test_struct() {
		s = std::allocator<print_struct<T>>().allocate(1);
		std::allocator<print_struct_base>().construct(s, print_struct_base());
	}

	void construct_derived() {
		std::allocator<print_struct<T>>().construct(static_cast<print_struct<T> *>(s), print_struct<T>());
	}
};


template<typename T>
void modifying_function(print_struct<T> *s) {
	s->a = 10;
	s->b = 20;
}

int main(void) {
	test_struct<const int> t;
	t.s->a = 1;
	t.s->b = 2;

	printf("before construct_derived: %d %d\n\n", t.s->a, t.s->b);

	t.construct_derived();

	printf("\nafter construct_derived: %d %d\n", t.s->a, t.s->b);

	print_struct<const int> *s = static_cast<print_struct<const int> *>(t.s);
	modifying_function(s);

	return 0;
}