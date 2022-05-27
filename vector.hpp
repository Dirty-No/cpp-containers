#ifndef  VECTOR_HPP
# define VECTOR_HPP

# include <memory> // std::allocator
# include <algorithm> // std::copy
# include "utils.hpp"

namespace ft
{

	template<
		class T,
		class Allocator = std::allocator<T>
	> class vector {

		// Member types (C++98) https://en.cppreference.com/w/cpp/container/vector
		public:
			typedef	T value_type;
			typedef	Allocator allocator_type ;
			typedef	std::size_t size_type;
			typedef	std::ptrdiff_t difference_type;
			typedef	value_type & reference;
			typedef	const value_type& const_reference;
			typedef	typename Allocator::pointer pointer;
			typedef	typename Allocator::const_pointer const_pointer;
			typedef	value_type* iterator;
			typedef	const value_type * const_iterator;

		private:
			// https://en.wikipedia.org/wiki/Hungarian_notation#:~:text=s_wheels%C2%A0%3A%20static%20member%20of%20a%20class
			pointer _M_start;
			pointer _M_finish;
			pointer _M_end_of_storage;
			allocator_type _M_allocator;

			// https://en.cppreference.com/w/cpp/memory/allocator/deallocate
			void _M_deallocate(pointer __p, size_type __n) {
				if (__p)
					_M_allocator.deallocate(__p, __n);
			}

			pointer _M_allocate(size_type __n) {
				if (__n == 0)
					return pointer();
				return (_M_allocator.allocate(__n));
			}

			void _M_create_storage(size_type __n)
			{
				this->_M_start = _M_allocate(__n);
				this->_M_finish = this->_M_start;
				this->_M_end_of_storage = this->_M_start + __n;
			}

			template <typename _ForwardIterator>
			pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last) {
				pointer __result = this->_M_allocate(__n);
				// https://stackoverflow.com/questions/30158192/difference-between-stduninitialized-copy-stdcopy
				std::uninitialized_copy(__first, __last, __result);
				return __result;
			}

		public:
			~vector() {
				_M_deallocate(this->_M_start, this->_M_end_of_storage - this->_M_start);
			}

			vector() : _M_start(), _M_finish(), _M_end_of_storage() { }

			vector(size_type __n) {
				_M_create_storage(__n);
			}

			size_type capacity() const {
				return this->_M_end_of_storage - this->_M_start;
			}

			iterator begin() {
				return iterator(this->_M_start);
			}

			const_iterator begin() const {
				return const_iterator(this->_M_start);
			}

			iterator end() {
				return iterator(this->_M_finish);
			}

			const_iterator end() const {
				return const_iterator(this->_M_finish);
			}

			size_type size() const {
				return size_type(this->_M_finish - this->_M_start);
			}

			// this shit's actually arbitrary af
			size_type max_size() const {
				// Ported from c++?? to c++98 from stl_vector.hpp
				// std::distance(begin(), end()) cannot be greater than PTRDIFF_MAX,
				// and realistically we can't store more than PTRDIFF_MAX/sizeof(T)
				// (even if std::allocator_traits::max_size says we can).
				const size_type __diffmax
					= __gnu_cxx::__numeric_traits<difference_type>::__max / sizeof(T);
				const size_type __allocmax = this->_M_allocator.max_size();
				return (std::min)(__diffmax, __allocmax);
			}

			vector & operator=(const vector & __x) {
				if (&__x != this) {
					const size_type __xlen = __x.size();
					if (__xlen > capacity()) {
						pointer __tmp =  _M_allocate_and_copy(__xlen, __x.begin(), __x.end());
						__destroy(this->_M_start, this->_M_finish, this->_M_allocator);
						_M_deallocate(this->_M_start, this->_M_finish - this->_M_start);
						this->_M_start = __tmp;
						this->_M_end_of_storage = this->_M_start +__xlen;
					}
					else if (size() >= __xlen) {
						__destroy(std::copy(__x.begin(), __x.end(), begin()), end(), this->_M_allocator);
					}
					else {
						std::copy(__x._M_start, __x._M_start + size(), this->_M_start);
						ft::__uninitialized_copy_a(__x._M_start + size(), __x._M_finish, this->_M_finish, this->_M_allocator);
					}
					this->_M_finish = this->_M_start + __xlen;
				}
				return *this;
			}

			allocator_type get_allocator() const {
				return this->_M_allocator;
			}


	};
};

#endif // ! VECTOR_HPP