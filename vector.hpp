#ifndef  VECTOR_HPP
# define VECTOR_HPP
# include <memory> // std::allocator
# include <algorithm> // std::copy
# include "utils.hpp"

namespace ft
{
	// https://en.cppreference.com/w/cpp/container/vector
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
				// Allocate buffer
				this->_M_start = _M_allocate(__n);

				// Init end pos to start
				this->_M_finish = this->_M_start;

				// Init end of storage
				this->_M_end_of_storage = this->_M_start + __n;
			}

			// Allocates a new buffer and copies the range in it
			template <typename _ForwardIterator>
			pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last) {
				// Allocate new buffer
				pointer __result = this->_M_allocate(__n);

				// Copy data into buffer
				// https://stackoverflow.com/questions/30158192/difference-between-stduninitialized-copy-stdcopy
				std::uninitialized_copy(__first, __last, __result);

				// return new buffer
				return __result;
			}

			// Erase everything after pos and set the end to pos
			// pos must be before _M_finish
			void _M_erase_at_end(pointer __pos) {
				// WIP: Unsure if this check is better than __pos == _M_finish
				if (size_type __n = this->_M_finish - __pos)
				{
					// Destroy whole range
					ft::__destroy(__pos, this->_M_finish, this->_M_allocator);
					
					// Update end position
					this->_M_finish = __pos;
				}
			}

			// Shrink/Grow vector to __n and fill with __val
			void _M_fill_assign(size_type __n, const value_type& __val) {
				if (__n > capacity()) {
					// Be lazy and use the fill constructor, current vector will be cleaned with destructor
					vector __tmp(__n, __val, this->_M_allocator);
					__tmp.swap(*this);
				}
				else if (__n > size()) {
					// Fill initialized part
					std::fill(begin(), end(), __val);

					// Compute additional size
					const size_type __add = __n - size();

					// Fill uninitialized part
					this->_M_finish = ft::__uninitialized_fill_n_a(this->_M_finish,
					  __add, __val, this->_M_allocator);
				}
				else
					// Fill up to N and erase extra data
					_M_erase_at_end(std::fill_n(this->_M_start, __n, __val));
			}

			/// Safety check used only from at().
			void _M_range_check(size_type __n) const {
				if (__n >= this->size())
					__throw_out_of_range_fmt(__N("vector::_M_range_check: __n "
									"(which is %zu) >= this->size() "
									"(which is %zu)"),
								__n, this->size());
			}

		public:
			~vector() {
				// Simply deallocate the buffer
				_M_deallocate(this->_M_start, this->_M_end_of_storage - this->_M_start);
			}

			// Init to default values
			vector() : _M_start(), _M_finish(), _M_end_of_storage() { }

			// Allocate buffer and init pointers
			vector(size_type __n) {
				_M_create_storage(__n);
			}

			// Simple diff
			size_type capacity() const {
				return this->_M_end_of_storage - this->_M_start;
			}

			// Iterators are literally pointers
			iterator begin() {
				return iterator(this->_M_start);
			}

			// Get the cont_iterator
			const_iterator begin() const {
				return const_iterator(this->_M_start);
			}

			iterator end() {
				return iterator(this->_M_finish);
			}

			const_iterator end() const {
				return const_iterator(this->_M_finish);
			}

			// Simple diff
			size_type size() const {
				return size_type(this->_M_finish - this->_M_start);
			}

			// this shit's actually arbitrary af and requires compiler extensions
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

			// Copies a vector and handles garbage cleaning
			vector & operator=(const vector & __x) {
				// Self referencing check
				if (&__x != this) {
					// Get target len
					const size_type __xlen = __x.size();
					if (__xlen > capacity()) {
						// Get a new buffer with new data
						pointer __tmp =  _M_allocate_and_copy(__xlen, __x.begin(), __x.end());
						
						// Destroy current buffer
						__destroy(this->_M_start, this->_M_finish, this->_M_allocator);

						// Deallocate current buffer
						_M_deallocate(this->_M_start, this->_M_finish - this->_M_start);

						// Set _M_start to new_buffer start
						this->_M_start = __tmp;

						// Set end of storage to new buffer's end
						this->_M_end_of_storage = this->_M_start +__xlen;
					}
					else if (size() >= __xlen) {
						// Copy new objects and destroy out of bond ones
						__destroy(std::copy(__x.begin(), __x.end(), begin()), end(), this->_M_allocator);
					}
					else {
						// here, this->size is lower but capacity is higher
						// Copy to 'this' initialized part
						std::copy(__x._M_start, __x._M_start + size(), this->_M_start);

						// Copy to 'this' uninitialized part
						ft::__uninitialized_copy_a(__x._M_start + size(), __x._M_finish, this->_M_finish, this->_M_allocator);
					}
					this->_M_finish = this->_M_start + __xlen;
				}
				return *this;
			}

			// allocator getter
			allocator_type get_allocator() const {
				return this->_M_allocator;
			}

			// Fills __n bytes with __val starting from _M_start
			// https://en.cppreference.com/w/cpp/container/vector/assign
			void	assign(size_type __n, const value_type& __val){
				_M_fill_assign(__n, __val);
			}

			// simple swap, not sure if there's a better way
			void swap(vector& __x) {
				std::swap(this->_M_allocator, __x._M_allocator);
				std::swap(this->_M_end_of_storage, __x._M_end_of_storage);
				std::swap(this->_M_start, __x._M_start);
				std::swap(this->_M_finish, __x._M_finish);
			}

			reference at( size_type __n ) {
				// Throw pretty exception when out of range
				_M_range_check(__n);

				// Get elem
				return (*this)[__n];
			}

			const_reference at( size_type pos ) const {
				// Throw pretty exception when out of range
				_M_range_check(__n);

				// Get elem
				return (*this)[__n];
			}
			
			// No check no exception access operator
			reference operator[](size_type __n) {
				return this->_M_start[__n];
			}

			// No check no exception access operator
			const_reference operator[](size_type __n) const {
				return this->_M_start[__n];
			}

			reference front() {
				return *begin();
			}

			const_reference front() const {
				return *begin();
			}



			
	};
};

#endif // ! VECTOR_HPP