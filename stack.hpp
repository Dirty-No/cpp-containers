#ifndef STACK_HPP
# define STACK_HPP

#include "vector.hpp"

namespace ft {


template<
    class T,
    class Container = ft::vector<T>
> class stack {

	public:
		typedef Container container_type;
		typedef typename Container::value_type value_type;
		typedef typename Container::size_type size_type;
		typedef typename Container::reference reference;
		typedef typename Container::const_reference const_reference;

	protected:
		container_type c;
	public:

	explicit stack( const Container& cont = Container() ) : c(cont) {
	}

	stack( const stack& other ) : c(other.c) {
	}

	~stack(){}

	stack& operator=( const stack& other ) {
		if (this != &other) this->c = other.c;
		return *this;
	}

	reference top() {
		return this->c.back();
	}

	const_reference top() const {
		return this->c.back();
	}

	bool empty() const {
		return this->c.empty();
	}

	size_type size() const { return this->c.size(); }

	void push( const value_type& value ) { this->c.push_back(value); }

	void pop() { return this->c.pop_back(); }

	friend bool operator==(
		const ft::stack<T,Container>& lhs,
		const ft::stack<T,Container>& rhs ) {
			return lhs.c == rhs.c;
	}

	friend bool operator!=(
		const ft::stack<T,Container>& lhs,
		const ft::stack<T,Container>& rhs ) {
			return lhs.c != rhs.c;
	}

	friend bool operator<(
		const ft::stack<T,Container>& lhs,
		const ft::stack<T,Container>& rhs ) {
			return lhs.c < rhs.c;
	}

	friend bool operator<=(
		const ft::stack<T,Container>& lhs,
		const ft::stack<T,Container>& rhs ) {
			return lhs.c <= rhs.c;
	}

	friend bool operator>(
		const ft::stack<T,Container>& lhs,
		const ft::stack<T,Container>& rhs ) {
			return lhs.c > rhs.c;
	}

	friend bool operator>=(
		const ft::stack<T,Container>& lhs,
		const ft::stack<T,Container>& rhs ) {
			return lhs.c >= rhs.c;
	}

};
}

#endif
