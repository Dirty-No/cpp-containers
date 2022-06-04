#ifndef  VECTOR_HPP
# define VECTOR_HPP
# include <memory> // std::allocator
# include <algorithm> // std::copy, std::max, std::min
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
            typedef ft::reverse_iterator<iterator> reverse_iterator;
            typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        private:
            // https://en.wikipedia.org/wiki/Hungarian_notation#:~:text=s_wheels%C2%A0%3A%20static%20member%20of%20a%20class
            pointer _M_start;
            pointer _M_finish;
            pointer _M_end_of_storage;
            allocator_type _M_allocator;

            // Throws an exception if __n > max_size.
            // _M_allocator muse be initialized!
            size_type _M_init_max_len_check(size_type __n)
            {
                if (__n > max_size())
                    __throw_length_error(
                        "cannot create ft::vector larger than max_size()"
                    );
                return __n;
            }

            size_type _M_check_len(size_type __n, const char* __s) const {
                // Ensure current length + __n < max_size
                if (max_size() - size() < __n)
                    __throw_length_error(__s);

                // Double length when __n < size()
                // else, return __n
                const size_type __len = size() + (std::max)(size(), __n);

                // Ensure __len doesn't get bigger than max_size
                return (__len < size() || __len > max_size()) ?
                    max_size() : __len;
            }


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

            void _M_clear_and_deallocate() {
                // Destroy all elements
                _M_erase_at_end(this->_M_start);

                // Deallocate the buffer
                _M_deallocate(
                    this->_M_start,
                    this->_M_end_of_storage - this->_M_start
                );

                // Reset positions
                this->_M_finish = pointer();
                this->_M_start = pointer();
                this->_M_end_of_storage = pointer();
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

                    // Fill uninitialized/unconstructed part
                    this->_M_finish = ft::__uninitialized_fill_n_a(
                        this->_M_finish,
                        __add,
                        __val,
                        this->_M_allocator
                    );
                }
                else
                    // Fill up to N and erase extra data
                    _M_erase_at_end(std::fill_n(this->_M_start, __n, __val));
            }

            // Safety check used only from at().
            void _M_range_check(size_type __n) const {
                if (__n >= this->size())
                    __throw_out_of_range_fmt(
                        "ft::vector::_M_range_check: __n "
                        "(which is %zu) >= this->size() "
                        "(which is %zu)",
                        __n,
                        this->size()
                    );
            }

            // Realloc and insert a single element
            void _M_realloc_insert(iterator __position, const value_type& __x)
            {
                // Ensure len isnt bigger than max_size
                const size_type __len = _M_check_len(
                    size_type(1),
                    "ft::vector::_M_realloc_insert"
                );

                // Save current start and finish position
                pointer __old_start = this->_M_start;
                pointer __old_finish = this->_M_finish;

                // Get number of elements before inserted position
                const size_type __elems_before = __position - begin();

                // Allocate new buffer and init start and finish positions
                pointer __new_start(this->_M_allocate(__len));
                pointer __new_finish(__new_start);

                try {
                    // Construct inserted element
                    this->_M_allocator.construct(
                        __new_start + __elems_before,
                        __x
                    );

                    // Re-copy everything before inserted element
                    // This is a new memory block so no elements are constructed
                    __new_finish = pointer();
                    __new_finish = ft::__uninitialized_copy_a(
                        __old_start,
                        __position,
                        __new_start,
                        this->_M_allocator
                    );

                    // Increment __new_finish past inserted element
                    ++__new_finish;

                    // Re-copy everything after inserted element
                    // This is a new memory block so no elements are constructed
                    __new_finish = ft::__uninitialized_copy_a(
                        __position,
                        __old_finish,
                        __new_finish,
                        this->_M_allocator
                    );
                }
                catch(...) {
                    // This is executed when memory allocation
                    // 	or elements constructors throw an exception

                    if (!__new_finish) {
                        // __uninitialized_copy has failed but new elem inserted
                        // Destroy inserted element
                        this->_M_allocator.destroy(__new_start + __elems_before);
                    }
                    else {
                        // At least one copy has succeded
                        // We have to destroy all copied elements
                        ft::__destroy(
                            __new_start,
                            __new_finish,
                            this->_M_allocator
                        );
                    }
                    // Deallocate new_buffer
                    _M_deallocate(__new_start, __len);

                    // Throw exception again
                    throw;
                }
                // Destroy old elements
                ft::__destroy(__old_start, __old_finish, this->_M_allocator);

                // Deallocate old buffer
                _M_deallocate(__old_start, __old_finish, this->_M_allocator);

                // Set new position member values
                this->_M_start = __new_start;
                this->_M_finish = __new_finish;
                this->_M_end_of_storage = __new_start + __len;
            }

            // Input iterator range (read only, increment only)
            // https://palm-screw-745.notion.site/InputIterators-b70026303c2e4c68b9177b529b1d0308
            template<typename _InputIterator>
            void _M_range_initialize(_InputIterator __first,
            _InputIterator __last, std::input_iterator_tag) {
                try {
                    // Push_back every elem naively
                    for (; __first != __last; ++__first) {
                        push_back(*__first);
                    }
                }
                catch(...) {
                    // Destroy all constructed elements
                    clear();

                    // throw exception again
                    throw;
                }
            }

            // Forward iterator range (read/write, only increment)
            // https://palm-screw-745.notion.site/ForwardIterator-c79f03dc37e446d8b9ee76bbe54e034d
            template<typename _InputIterator>
            void _M_range_initialize(_InputIterator __first,
            _InputIterator __last, std::forward_iterator_tag) {
                /*
                    Get number of elements in range.
                     std::distance will do:
                        - a simple __last - __ first on random access iterators (O(1))
                        - count with a for loop on input_iterators (O(n))
                    https://en.cppreference.com/w/cpp/iterator/distance
                */
                const size_type __n = std::distance(__first, __last);
                
                // Allocate new buffer
                this->_M_start = _M_allocate(_M_init_max_len_check(__n));

                // Set end of storage pointer
                this->_M_end_of_storage = this->_M_start + __n;

                // Copy all objects to new buffer and set finish pointer
                this->_M_finish = ft::__uninitialized_copy_a(
                    __first,
                    __last,
                    this->_M_start,
                    this->_M_allocator
                );
            }

            template<typename _InputIterator>
            void _M_initialize_dispatch(
                _InputIterator __first,
                _InputIterator __last,
                ft::false_type)
            {
                _M_range_initialize(
                    __first,
                    __last,
                    ft::__iterator_category(__first)
                );
            }

            // Resolves range construct ambiguity
            // https://cplusplus.github.io/LWG/issue438
            template<typename _Integer>
            void _M_initialize_dispatch(
                _Integer __n,
                _Integer __value,
                ft::true_type)
            {
                // Allocate new buffer
                this->_M_start = _M_allocate(_M_init_max_len_check(
                    static_cast<size_type>(__n)));
                
                // Set end of storage position
                this->_M_end_of_storage =
                    this->_M_start + static_cast<size_type>(__n);

                // _M_fill_initialize(static_cast<size_type>(__n), __value);
                
                // Construct new elements with __value
                this->_M_finish = ft::__uninitialized_fill_n_a(
                    this->_M_start,
                    static_cast<size_type>(__n),
                    __value,
                    this->_M_allocator
                );
            }

            // Range assign implementation for forward_iterators (read/write, increment)
            // https://palm-screw-745.notion.site/ForwardIterator-c79f03dc37e446d8b9ee76bbe54e034d
            template<typename _ForwardIterator>
            void _M_assign_aux(_ForwardIterator __first,
                _ForwardIterator __last, std::forward_iterator_tag) {
                
                /*
                    Get number of elements in range.
                     std::distance will do:
                        - a simple __last - __ first on random access iterators (O(1))
                        - count with a for loop on input_iterators (O(n))
                    https://en.cppreference.com/w/cpp/iterator/distance
                */
                const size_type __len = std::distance(__first, __last);

                if (__len > capacity()) {
                    // Not enough allocated memory, so we have to realloc

                    // Ensure new len is in numeric limits bounds
                    _M_init_max_len_check(__len);

                    // Allocate new buffer and copy in-range elements in it
                    pointer __tmp(_M_allocate_and_copy(__len, __first, __last));

                    // Destroy and deallocate all old elements 
                    _M_clear_and_deallocate();

                    // Set new position values
                    this->_M_start = __tmp;
                    this->_M_finish = this->_M_start + __len;
                    this->_M_end_of_storage = this->_M_finish;
                }
                else if (size() >= __len) {
                    // We have enough initialized memory to std::copy everything

                    // Copy range into new buffer and then destroy every
                    // remaining elements after new end.
                    _M_erase_at_end(std::copy(__first, __last, this->_M_start));
                }
                else {
                    // We have enough allocated memory
                    // (initialized + uninitialized) to store every element

                    // Init the last elem to copy in initialized memory
                    _ForwardIterator __last_to_copy_in_init_mem = __first;
                    
                    // Advance the iterator to the last elem to copy in init mem
                    std::advance(__last_to_copy_in_init_mem, size());

                    // Copy everything that has to go in initialized memory
                    std::copy(
                        __first,
                        __last_to_copy_in_init_mem,
                        this->_M_start
                    );

                    // Copy everything that has to go in uninitialized memory
                    ft::__uninitialized_copy_a(
                        __last_to_copy_in_init_mem,
                        __last,
                        this->_M_finish,
                        this->_M_allocator
                    );
                }
            }

            /* Needs insert(iterator __position, const value_type& __x)
                 
                InputIterators inserts are absolutely awful to use with vector
                
                Since we cannot know range sizes beforehand
                    (std::distance will break everything in all cases)
                     we basically have no other choices than to use
                     O(n²) algorithms (worst case) for all cases
                     the best we can do is amortize them.

                If you, as the user, have a clue about the range size,
                    you should use reserve(_range_max_size_) beforehand
                    this avoids reallocations and all the mess it implies
            */
            template<typename _InputIterator>
            void _M_range_insert(iterator __pos, _InputIterator __first,
                _InputIterator __last, std::input_iterator_tag) {
                // Choose algorithm depending on position (end / elsewhere)
                if (__pos == end()) {
                    // Simply insert everything past the end
                    // Insert will handle reallocating when needed 
                    for (; __first != __last; ++__first)
                        insert(end(), *__first);
                }
                else if (__first != __last) {
                    /*
                    
                        Copy everything in a second vector before inserting it
                            in "this"
                            
                    We do not simply insert the range directly in place
                        because it would mean having to move everything at each
                        insert + reallocating each time we get past capacity()
                        which would mean our worst case complexity
                        would be something along the lines of:

                        O(_range_size² * after_size + _range_size + _this_size_)
                                            (atrocious)

                    Hence, the following implementation guarantees
                         a worst case of something ~like:
                         
                            O(_range_size_² + _this_size_)
                                        (best)
                    */

                        vector __tmp(__first, __last, this->_M_allocator);
                        insert(__pos, __tmp.begin(), __tmp.end());
                    }
            }

            // Here, we can get the range size beforehand, so our
            // 	algorithms can get a bit more sophisticated
            template <typename _ForwardIterator>
            void _M_range_insert(iterator __position, _ForwardIterator __first,
                _ForwardIterator __last, std::forward_iterator_tag) {
                    if (__first != __last) {
                    // Get range size
                    const size_type __n = std::distance(__first, __last);

                    // Choose algorithm depending on current unused allocd mem
                    if (
                        size_type(this->_M_end_of_storage - this->_M_finish)
                            >= __n
                    )
                    {
                        // We have enough allocated memory past finish pos
                        
                        // Get number of elements after insert position
                        const size_type __elems_after = end() - __position;

                        // Save old finish position
                        pointer __old_finish(this->_M_finish);

                        if (__elems_after > __n) {
                            
                            // First, move everything that will go to uninit mem
                            /*                                  
                                                ---> (copy direction)                          
                                           1    2   3   4      __n = 4                                               
                                           /\  /\  /\  /\                                                  
                                          /  \/  \/  \/  \     i: initialized                                           
                                         /   /\  /\  /\   \    u: uninitialized                                           
                                        /   /  \/  \/  \   \                                                                  
                                       /   /   /\  /\   \   \                                                                
                                      /   /   /  \/  \   \   \                                                                       
                                     /   /   /   /\   \   \   \                                                                       
                                    /   /   /   /  \   \   \   \                                                                     
                              ... | i | i | i | i | u | u | u | u 

                                    numbers represent copy order
                            */
                            ft::__uninitialized_copy_a(
                                this->_M_finish - __n,
                                this->_M_finish,
                                this->_M_finish,
                                this->_M_allocator
                            );

                            // Set new finish position
                            this->finish += __n;

                            /* 
                                Move everything that goes to initialized mem
                                
                                We have __elems_after > __n, that means that
                                    there is (__elems_after - __n) elements that
                                    overlaps in the range.

                                We do not have enough unused space to do
                                    a simple std::copy
                             
                                We have two choices:

                                simply copy the whole range backwards
                                         ----> (copy direction)
                                          (__n = 4)
                          |        copied into       |    copied into |
                          |          used init       |    initialized |
                          |            memory        |    unused mem  |                        
                          |11  10   9   8   7  6   5 |  4   3   2   1 | 
                           /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\                                                           
                          /  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \                                    
                         /   /\  /\  /\  /\  /\  /\  /\  /\  /\  /\   \                                      
                        /   /  \/  \/  \/  \/  \/  \/  \/  \/  \/  \   \                                     
                       /   /   /\  /\  /\  /\  /\  /\  /\  /\  /\   \   \                                     
                      /   /   /  \/  \/  \/  \/  \/  \/  \/  \/  \   \   \                                   
                     /   /   /   /\  /\  /\  /\  /\  /\  /\  /\   \   \   \                                 
                    /   /   /   /  \/  \/  \/  \/  \/  \/  \/  \   \   \   \                                                                                                    
                  | m | m | m | m | m | m | m | m | m | m | m | i | i | i | i |
                  |  future data  |       overlap             |     old data

                                m: data to move
                                i: initialized unused data (copied previously)

                            We can also: (2)

                                std::copy everything by groups of x (mod __n)
                                
                                         ----> (copy direction)
                                          (__n = 4)
                          |        copied into       |    copied into |
                          |          used init       |    initialized |
                          |            memory        |    unused mem  |                        
                          |9  10   11 | 5   6  7   8 |  1   2   3   4 | 
                           /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\                                                           
                          /  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \                                    
                         /   /\  /\  /\  /\  /\  /\  /\  /\  /\  /\   \                                      
                        /   /  \/  \/  \/  \/  \/  \/  \/  \/  \/  \   \                                     
                       /   /   /\  /\  /\  /\  /\  /\  /\  /\  /\   \   \                                     
                      /   /   /  \/  \/  \/  \/  \/  \/  \/  \/  \   \   \                                   
                     /   /   /   /\  /\  /\  /\  /\  /\  /\  /\   \   \   \                                 
                    /   /   /   /  \/  \/  \/  \/  \/  \/  \/  \   \   \   \                                                                                                    
                  | m | m | m | m | m | m | m | m | m | m | m | i | i | i | i |
                  |  future data  |       overlap             |     old data

                                m: data to move
                                i: initialized unused data (copied previously)

                            Both methods are equivalent in complexity
                            (1) is definitely simpler, being a single call to
                                std::copy_backwards, that's what gcc's STL chose 

                            Note: just realized that (2) is actually just
                                a fancy copy_backward, 
                                (1) is basically (2) with x = 1
                            
                                Let's implement (1) then.
                            */

                            std::copy_backward(
                                __position,
                                __old_finish - __n,
                                __old_finish);

                            // Actually insert the new data
                            std::copy(__first, __last, __position);
                        } else {
                            /* ( _elems_after <= __n )
                                    
                                __n = 5, p: __position; __elem_after = 2

                                          [ copy (2) ]
                                             /\  /\                                       
                                            /  \/  \
                                           /   /\   \
                                          /   /  \   \                          
                                         /   /    \   \                          
                                        /   /      \   \                          
                                       /   /        \   \                          
                                      /   /          \   \                      
                                     /   /            \   \                    
                                    /   /              \   \                 
                                p | i | i | u | u | u | u | u | u 
                                  | 1   2 | 3   4   5 |
                                  |cpy (3)|  copy (1) | 

                                  numbers represents elem order in 
                                    (__fist,__last) range
                            */

                            // Get lower bound of (1)/ upper bound of (3)
                            _ForwardIterator __mid = __first;
                            std::advance(__mid, __elems_after);

                            
                            // Perform copy (1)
                            ft::__uninitialized_copy_a(
                                __mid,
                                __last,
                                this->_M_finish,
                                this->_M_allocator
                            );

                            // Set finish position to end of (1)
		                    this->_M_finish += __n - __elems_after;

                            // Perform copy (2)
                            ft::__uninitialized_copy_a(
                                __position,
                                __old_finish,
                                this->_M_finish,
                                this->_M_allocator
                            );

                            // Set finish position to end of (2)
                            this->_M_finish += __elems_after;

                            // Perform copy (3)
                            std::copy(__first, __mid, __position);
                        }
                    } else {
                        // We do not have enough capacity, we have to realloc
                        
                        // Ensure new len is allocable
                        const size_type __len = _M_check_len(
                            __n,
                            "ft::vector::_M_range_insert"
                        );

                        // Allocate new buffer and set position pointers
                        pointer __new_start(this->_M_allocate(__len));
                        pointer __new_finish(__new_start);
                        
                        try 
                        {
                            // Copy back everything before __position
                            __new_finish = ft::__uninitialized_copy_a(
                                this->_M_start,
                                __position,
                                __new_start,
                                this->_M_allocator
                            );

                            // Copy the entire passed range
                            __new_finish = ft::__uninitialized_copy_a(
                                __first,
                                __last,
                                __new_finish,
                                this->_M_allocator
                            );

                            // Copy back everything after __position
                            __new_finish = ft::__uninitialized_copy_a(
                                __position,
                                this->_M_finish,
                                __new_finish,
                                this->_M_allocator
                            );
                        }
                        catch (...) {
                            // Destroy all constructed object
                            ft::__destroy(
                                __new_start,
                                __new_finish,
                                this->_M_allocator
                            );

                            // Deallocate new buffer
                            _M_deallocate(__new_start, __len);

                            // Throw exception again
                            throw;
                        }
                        
                        // Destroy and deallocate old data
                        _M_clear_and_deallocate();

                        // Set position values to new buffer
                        this->_M_start = __new_start;
                        this->_M_finish = __new_finish;
                        this->_M_end_of_storage = __new_start + __len;
                    }
                }
            }

            // Basically the same as _M_range_insert but with fill instead of copy
            void _M_fill_insert(iterator __position,
                size_type __n, const value_type& __x) {
                if (__n != 0) {
                    // Choose algorithm depending on current unused allocd mem
                    if (
                        size_type(this->_M_end_of_storage - this->_M_finish)
                            >= __n
                    )
                    {
                        // We have enough allocated memory past finish pos
                        
                        // Get number of elements after insert position
                        const size_type __elems_after = end() - __position;

                        // Save old finish position
                        pointer __old_finish(this->_M_finish);

                        if (__elems_after > __n) {
                            
                            // First, move everything that will go to uninit mem
                            /*                                  
                                                ---> (copy direction)                          
                                           1    2   3   4      __n = 4                                               
                                           /\  /\  /\  /\                                                  
                                          /  \/  \/  \/  \     i: initialized                                           
                                         /   /\  /\  /\   \    u: uninitialized                                           
                                        /   /  \/  \/  \   \                                                                  
                                       /   /   /\  /\   \   \                                                                
                                      /   /   /  \/  \   \   \                                                                       
                                     /   /   /   /\   \   \   \                                                                       
                                    /   /   /   /  \   \   \   \                                                                     
                              ... | i | i | i | i | u | u | u | u 

                                    numbers represent copy order
                            */
                            ft::__uninitialized_copy_a(
                                this->_M_finish - __n,
                                this->_M_finish,
                                this->_M_finish,
                                this->_M_allocator
                            );

                            // Set new finish position
                            this->finish += __n;

                            /* 
                                Move everything that goes to initialized mem
                                
                                We have __elems_after > __n, that means that
                                    there is (__elems_after - __n) elements that
                                    overlaps in the range.

                                We do not have enough unused space to do
                                    a simple std::copy
                             
                                We have two choices:

                                simply copy the whole range backwards
                                         ----> (copy direction)
                                          (__n = 4)
                          |        copied into       |    copied into |
                          |          used init       |    initialized |
                          |            memory        |    unused mem  |                        
                          |11  10   9   8   7  6   5 |  4   3   2   1 | 
                           /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\                                                           
                          /  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \                                    
                         /   /\  /\  /\  /\  /\  /\  /\  /\  /\  /\   \                                      
                        /   /  \/  \/  \/  \/  \/  \/  \/  \/  \/  \   \                                     
                       /   /   /\  /\  /\  /\  /\  /\  /\  /\  /\   \   \                                     
                      /   /   /  \/  \/  \/  \/  \/  \/  \/  \/  \   \   \                                   
                     /   /   /   /\  /\  /\  /\  /\  /\  /\  /\   \   \   \                                 
                    /   /   /   /  \/  \/  \/  \/  \/  \/  \/  \   \   \   \                                                                                                    
                  | m | m | m | m | m | m | m | m | m | m | m | i | i | i | i |
                  |  future data  |       overlap             |     old data

                                m: data to move
                                i: initialized unused data (copied previously)

                            We can also: (2)

                                std::copy everything by groups of x (mod __n)
                                
                                         ----> (copy direction)
                                          (__n = 4)
                          |        copied into       |    copied into |
                          |          used init       |    initialized |
                          |            memory        |    unused mem  |                        
                          |9  10   11 | 5   6  7   8 |  1   2   3   4 | 
                           /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\                                                           
                          /  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \                                    
                         /   /\  /\  /\  /\  /\  /\  /\  /\  /\  /\   \                                      
                        /   /  \/  \/  \/  \/  \/  \/  \/  \/  \/  \   \                                     
                       /   /   /\  /\  /\  /\  /\  /\  /\  /\  /\   \   \                                     
                      /   /   /  \/  \/  \/  \/  \/  \/  \/  \/  \   \   \                                   
                     /   /   /   /\  /\  /\  /\  /\  /\  /\  /\   \   \   \                                 
                    /   /   /   /  \/  \/  \/  \/  \/  \/  \/  \   \   \   \                                                                                                    
                  | m | m | m | m | m | m | m | m | m | m | m | i | i | i | i |
                  |  future data  |       overlap             |     old data

                                m: data to move
                                i: initialized unused data (copied previously)

                            Both methods are equivalent in complexity
                            (1) is definitely simpler, being a single call to
                                std::copy_backwards, that's what gcc's STL chose 

                            Note: just realized that (2) is actually just
                                a fancy copy_backward, 
                                (1) is basically (2) with x = 1
                            
                                Let's implement (1) then.
                            */

                            std::copy_backward(
                                __position,
                                __old_finish - __n,
                                __old_finish
                            );

                            // Actually insert the new data
                            std::fill(__position, __position + __n, __x);
                        } else {
                            /* ( _elems_after <= __n )
                                    
                                __n = 5, p: __position; __elem_after = 2

                                          [ copy (2) ]
                                             /\  /\                                       
                                            /  \/  \
                                           /   /\   \
                                          /   /  \   \                          
                                         /   /    \   \                          
                                        /   /      \   \                          
                                       /   /        \   \                          
                                      /   /          \   \                      
                                     /   /            \   \                    
                                    /   /              \   \                 
                                p | i | i | u | u | u | u | u | u 
                                  | 1   2 | 3   4   5 |
                                  |cpy (3)|  copy (1) | 

                                  numbers represents elem order in 
                                    (__fist,__last) range
                            */
            
                            // Perform copy (1)
                            ft::__uninitialized_fill_n_a(
                                __n - __elems_after,
                                __x,
                                this->_M_finish,
                                this->_M_allocator
                            );

                            // Set finish position to end of (1)
		                    this->_M_finish += __n - __elems_after;

                            // Perform copy (2)
                            ft::__uninitialized_copy_a(
                                __position,
                                __old_finish,
                                this->_M_finish,
                                this->_M_allocator
                            );

                            // Set finish position to end of (2)
                            this->_M_finish += __elems_after;

                            // Perform copy (3)
                            std::fill(__position, __old_finish, __x);
                        }
                    } else {
                        // We do not have enough capacity, we have to realloc
                        
                        // Ensure new len is allocable
                        const size_type __len = _M_check_len(
                            __n,
                            "ft::vector::_M_fill_insert"
                        );

                        const size_type __elems_before = __position - begin();

                        // Allocate new buffer and set position pointers
                        pointer __new_start(this->_M_allocate(__len));
                        pointer __new_finish(__new_start);
                        
                        try 
                        {
                           ft::__uninitialized_fill_n_a(
                               __new_start + __elems_before,
                               __n,
                               __x,
                               this->_M_allocator
                           );

                            __new_finish = pointer();

                            // Copy the entire passed range
                            __new_finish = ft::__uninitialized_copy_a(
                                this->_M_start,
                                __position,
                                __new_start,
                                this->_M_allocator
                            );

                            __new_finish += __n;

                            // Copy back everything after __position
                            __new_finish = ft::__uninitialized_copy_a(
                                __position,
                                this->_M_finish,
                                __new_finish,
                                this->_M_allocator
                            );


                        }
                        catch (...) {
                            if (!__new_finish)
                                ft::__destroy(
                                    __new_start + __elems_before,
                                    __new_start + __elems_before + __n,
                                    this->_M_allocator
                                );
                            else
                                // Destroy all constructed object
                                ft::__destroy(
                                    __new_start,
                                    __new_finish,
                                    this->_M_allocator
                                );

                            // Deallocate new buffer
                            _M_deallocate(__new_start, __len);

                            // Throw exception again
                            throw;
                        }
                        
                        // Destroy and deallocate old data
                        _M_clear_and_deallocate();

                        // Set position values to new buffer
                        this->_M_start = __new_start;
                        this->_M_finish = __new_finish;
                        this->_M_end_of_storage = __new_start + __len;
                    }
                }
            }

            // Needs _M_range_insert

            template<typename _InputIterator>
            void _M_assign_aux(_InputIterator __first, _InputIterator __last,
            	std::input_iterator_tag) {
                
                // Init current position for iterating
                pointer __cur(this->_M_start);

                for (;
                    __first != __last && __cur != this->_M_finish;
                    ++__cur,
                    (void)++__first;
                ) {
                    if (__first == __last) {
                        _M_erase_at_end(__cur);
                    }
                    else {
                        _M_range_insert(
                            end(),
                            __first,
                            __last,
                            ft::__iterator_category(__first);
                        );
                    }
                }
            }

            // To inline
            void _M_no_realloc_insert(iterator __position, const T& __x) {
                // We'll have to move everything after __position by one,
                // so we have to construct one new elem after finish
                // Optimize by constructing with the value that 
                //	would be copied anyway
                this->_M_allocator.construct(
                    this->_M_finish,
                    this->_M_finish[-1],
                );

                // Offset finish position by one
                T __x_copy = __x;

                // Copied ranges overlap,
                // end of destination is outside of the source range, so we
                // have to use copy_backward
                // See Notes: 
                // 	https://en.cppreference.com/w/cpp/algorithm/copy_backward
                std::copy_backward(
                    __position
                    this->_M_impl._M_finish - 2,
                    this->_M_impl._M_finish - 1
                );

                // Actually copy the passed element into insertion position
                *__position = __x_copy;
            }

            template<typename _Integer>
            void _M_assign_dispatch(_Integer __n, _Integer __val, __true_type)
            {
                _M_fill_assign(__n, __val); 
            }

            template<typename _InputIterator>
            void _M_assign_dispatch(
                _InputIterator __first, _InputIterator __last, __false_type)
            {
                _M_assign_aux(
                    __first,
                    __last,
                    ft::__iterator_category(__first)
                );
            }

            template <typename _InputIterator>
            void _M_insert_dispatch(iterator __pos, _InputIterator __first, 
                _InputIterator __last, ft::false_type) {
                _M_range_insert(__pos,
                    __first,
                    __last,
                    ft::__iterator_category(__first)
                );
            }

            template <typename _InputIterator>
            void _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __val,
			   ft::true_type) {
                _M_fill_insert(__pos, __n, __val);
            }

        public:
            ~vector() {
                // Destroy all elements and deallocate the buffer
                _M_clear_and_deallocate();
            }

            // Init to default values
            vector() : _M_start(), _M_finish(), _M_end_of_storage() { }

            explicit vector(allocator_type __a) {
                this->_M_allocator = __a;
            }

            // Fill constructor
            explicit vector(size_type __n, const value_type& __value = value_type(),
                const allocator_type& __a = allocator_type())
            : _M_allocator(__a) {
                // Ensure __n is lower than max_size()
                _M_init_max_len_check(__n);

                // Allocate memory and init _M_start, _M_finish and _M_end_of_storage
                _M_create_storage(__n);

                // Fill the value and set _M_finish.
                this->_M_finish = ft::__uninitialized_fill_n_a(
                    this->_M_start,
                    __n,
                    __value,
                    this->_M_allocator
                );
            }

            // Copy constructor
            vector(const vector& __x) {
                // Allocate buffer and init start/finish/end_of_storage pointers
                _M_create_storage(__x.size);

                // Copy allocator
                this->_M_allocator = __x.get_allocator();

                // Copy __x's data into is' uninitialized buffer
                this->_M_finish = ft::__uninitialized_copy_a(
                    __x.begin(),
                    __x.end(),
                    this->_M_start,
                    this->_M_allocator
                );
            }

            template<typename _InputIterator>
            vector(_InputIterator __first, _InputIterator __last,
                 const allocator_type& __a = allocator_type()) {
                this->_M_allocator = __a;

                /*
                    Check whether it's an integral type. 
                    If so, it's not an iterator and this must actually resolve
                    to vector(size_type, const value_type&);
                
                    This is used to fix _this_ ambiguity:
                        https://cplusplus.github.io/LWG/issue438
                
                    Yes, it's a dirty hack that kinda spaghettifies the code,
                    since this template function really isn't _meant_ to be
                    used for resolving vector(size_type, const value_type&);
                        in any way.
                */

                _M_initialize_dispatch(
                    __first,
                    __last,
                    ft::is_integral<_InputIterator>()
                );
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

            // https://en.cppreference.com/w/cpp/container/vector/rbegin
            reverse_iterator rbegin() {
                return reverse_iterator(end()) ;
            }

            const_reverse_iterator rbegin() const {
                return const_reverse_iterator(end());
            }

            // https://en.cppreference.com/w/cpp/container/vector/rend
            reverse_iterator rend() {
                return reverse_iterator(begin());
            }

            const_reverse_iterator rend() const {
                return const_reverse_iterator(begin());
            }

            // Simple diff
            size_type size() const {
                return size_type(this->_M_finish - this->_M_start);
            }

            // this shit's actually arbitrary af
            size_type max_size() const {
                // Ported from c++?? to c++98 from stl_vector.hpp
                // std::distance(begin(), end()) cannot be greater than PTRDIFF_MAX,
                // and realistically we can't store more than PTRDIFF_MAX/sizeof(T)
                // (even if std::allocator_traits::max_size says we can).
                
                // Using type_traits implemented by GCC to make this work
                // TODO: Use std::numeric_limits
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
                        pointer __tmp = _M_allocate_and_copy(
                            __xlen,
                            __x.begin(),
                            __x.end()
                        );
                        
                        // Destroy current buffer
                        __destroy(
                            this->_M_start,
                            this->_M_finish,
                            this->_M_allocator
                        );

                        // Deallocate current buffer
                        _M_deallocate(
                            this->_M_start,
                            this->_M_finish - this->_M_start
                        );

                        // Set _M_start to new_buffer start
                        this->_M_start = __tmp;

                        // Set end of storage to new buffer's end
                        this->_M_end_of_storage = this->_M_start +__xlen;
                    }
                    else if (size() >= __xlen) {
                        // Copy new objects and destroy out of bond ones
                        __destroy(
                            std::copy(
                                __x.begin(),
                                __x.end(),
                                begin()
                            ),
                            end(),
                            this->_M_allocator
                        );
                    }
                    else {
                        // here, this->size is lower but capacity is higher
                        // Copy to 'this' initialized part
                        std::copy(
                            __x._M_start,
                            __x._M_start + size(),
                            this->_M_start
                        );

                        // Copy to 'this' uninitialized part
                        ft::__uninitialized_copy_a(
                            __x._M_start + size(),
                            __x._M_finish,
                            this->_M_finish,
                            this->_M_allocator
                        );
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

            template<typename _InputIterator>
            void assign(_InputIterator __first, _InputIterator __last)
            {
                /* big comment
                    Check whether it's an integral type. 
                    If so, it's not an iterator and this must actually resolve
                    to void	assign(size_type __n, const value_type& __val);

                
                    This is the same issue that happens with range constructor:
                        vector(
                            _InputIterator __first,
                            _InputIterator __last,
                             const allocator_type& __a
                        )

                        https://cplusplus.github.io/LWG/issue438

                    Still a dirty hack, the right function isn't _actually_
                        resolved, it's just dispatched to an identical one.

                    I wonder if this subject's authors actually expected that
                        students would encounter this type of pretty tricky
                        issues.
                        
                    I really feel like they didn't test their subject
                        themselves at all. still a good exercise tho, too bad
                        everyone's just not handling those cases at all.

                    That's a big ass comment lol, since we're here, I guess it's
                    OK to fit an ascii of MGSV Senator Armstrong in here
                                                                                                                        
                                                        ...                                         
STANDING HERE                               .:~7Y5PGGBB#####BBBBGP555YJ7~:                          
I REALIZE                              .^7YG#&@@@@@@@@@@@@@@@@@@@@@@@@@@@&BY^                       
YOU WERE JUST LIKE ME              .^7P#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@BY!                    
TRYING TO MAKE HISTORY           ~Y#@@@@@@@@@@@@@&#BGPPP5YJJ?7!!!77??JYPG#&@@@@@B!                  
BUT WHO'S TO JUDGE             !G@@@@@@@@@@@@@G?^:.                      .:^7YG#@@P.                
THE RIGHT FROM WRONG         ^G@@@@@@@@@@@@@B7~^~:                             .~5@&^               
WHEN OUR GUARD IS DOWN     .5@@@@@@@@@@@@@P??77!?G.                               :YB~              
I THINK WE'LL BOTH AGREE  ^#@@@@@@@@@@@@#~      :J.                                 ^#~             
                         7&@@@@@@@@@@@@G.                                            ?B             
THAT VIOLENCE           ?@@@@@@@@@@@@@G.                                              B?            
BREEDS VIOLENCE        ?@@@@@@@@@@@@@B.                                               ^#^           
BUT IN THE END        7@@@@@@@@@@@@@@^                                                 PP           
IT HAS TO BE THIS WAY &@@@@@@@@@@@@@7            ....                                  7#           
                     P@@@@@@@@@@@@@P          ^~~~^^^~~~~^:::::::..                     B!          
                    :&@@@@@@@@@@@@&:          :^~~~~^^^~!7!~^^^^^^~~~^^:..::::::::.     PY          
                    7@@@@@@@@@@@@@J          .^:..       .:^^^^^^~~!~!77!!7!!!!!!~~..   5G          
                    Y@@@@@@@@@@@@&.                              .^??  ...  .::.::^^~.  7B          
                    Y@@@@@@@@@@@@J          .~?JYYYYYJJ?77!!!~~^:. .?.     :^:          .#:         
                    Y@@@@@@@@@@@@^      . .5&@@@@@@@@@@@@@@@@@@@@&BJ~      :. ~?5PPPPPPPP&J         
                    Y@@@@@@@@@@@@^    :?? ?@@@&#BGGPP5PPGGG#&@@@@@#B:       .G@@@@@@@@@@@@@G^       
                    J@@@@@@@@@@@&.   :YY!~Y&@&:.^~^^^^^^~~^:^^~!7?YG!       Y@@&@&#BGPP555YBP       
                    !@@@@@@@@@@@&5PGB&&@@@@@@&.:!^^~~???J7~^.     .J#!^!????#B~!~7??Y55J???BP^:     
                   .7&&@@@@@@@@@@@@@&@#BGPGGB@~~7!!~~?JJJ?!7JJ^   :Y&@@@@&&@@57J?7~:. .. .5P~??Y?   
                 .J#B5?7B@@@@@@5?YY7!J.      ?G^..          .~!   7&J!^:...^GGYBGGP7J??~. B!  .YG   
                 GJ:.:Y5:P@&BYJ!!!~~^         ~J?JJJJJ?!~^::.  ..7#~        Y&:     ..^^: #PG#@#:   
                !#     JB:#~         :!:.           .:^!7???JJJ?J?Y.       .J7GY?7!^:.    G#G@P.    
                ?P  ^  ~G P7        ~777!^                     :7?J^        Y~:~^~!7JJJJ?7#5Y?      
                ?G ^7  !G ~~       .J!?7:                     ~7:           ^?!       ..::5G.       
                ^&::?  .Y:         7Y?!.                  :~75!               :J^         !B        
                 YP ?~          .~J5!                 .:!7~:77 .^!!~:         ..5?.       ~#.       
                  GY J:        ^5Y!.                ^!!^:   :!!5GBG5PJ:     7#@BJ^!7^     !B        
                  :#! J .!    :GJ                 ^7~.               ^~~~~~!YY?7.  .~?:   !B        
                   ~#^^7 ?~   YY                  .                                  .?^  ?B        
                    7#.?~ 7! ~Y!                :7 !:....:::^^~~~^^~~^^^^^^^:.         J. JP        
                     YG !~!^:5Y:                :G JJ5J7?77!7?~~!!^^~~^^~^^~!!~~~^::.^^!! JP        
                      GJ   .BG!                  P: ^!7?J7!~!?::J~.^Y^:7?.^Y^~7!^!JY?Y^.^ GJ        
                      .&5JJ5P?.                  J?   .:^^~!!!!77!!77~~!!~!?~~7J??7~:    ^&:        
                      55...^7!~                  .G:            .::::^^^:^^^^^^::.   ?:  55         
                     YP   ^YY7?    :!^:           !P        .~!777!!!77!!^         .J^  ^#:         
                    JG.  :JJ^J?~   .J~7            JJ       .~:.      ..^!?!       7^   GJ          
                   7#.   ^?? ^??7   ~?7!            J.                             7.  JG           
                  !#^    :JJ  .^~.   ^?77                                             !#:           
                 ^#~      !G:         :7~!                             : .:.         7B:            
                ^B?      .?5:                                         :P !?~  ~~    JG:             
             .!YBB     .!YY7                                          :P.JJ^  ~7  ~PJ.              
          .~J57^#~    !5Y!.                                           Y7?Y^  .!?Y5J:                
        !YY7^  ~7     ~^                                        ^^^:.!&^7J!YPGYG#.                  
     ^?Y?:                                                      ^!77!!7~~!7~^..#B!^^^~^^::::..      
   !5Y~                      .^:                                       .~^    ?Y^5#G5?777???JJJ??7^ 
  !Y^                        :~7!^                                   .~?!.    .   .^7JJJ!     .:^!~ 
                               .~7~                                 .!!^               :^           
                                 ~?:                            .~7!..:                             
                                  ?~                          :!7~:                                 
                                  :                          :~!.                                   
                */
                _M_assign_dispatch(
                    __first,
                    __last,
                    ft::is_integral<_InputIterator>()
                );
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

            const_reference at( size_type __n ) const {
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

            // Get first elem
            reference front() {
                return *begin();
            }

            // Get first elem
            const_reference front() const {
                return *begin();
            }

            reference back() {
                return end()[-1];
            }

            const_reference back() const {
                return end()[-1];
            }

            // True if empty; false if not
            bool empty() const {
                return begin() == end();
            }

            // Return underlying buffer
            T *data() {
                return empty() ? 0 : this->_M_start;
            }

            void clear() {
                // Destroy all elements
                _M_erase_at_end(this->_M_start);
            }

            void reserve(size_type __n) {
                // Ensure new size is allocable
                if (__n > this->max_size())
                    __throw_length_error("ft::vector::reserve");
                if (this->capacity() < __n) {
                    const size_type __old_size = size();

                    // Allocate new buffer and copy existing data in it
                    pointer __tmp = _M_allocate_and_copy(
                        __n,
                        this->_M_start,
                        this->_M_finish
                    );

                    // Destroy old data
                    ft::__destroy(
                        this->_M_start,
                        this->_M_finish,
                        this->_M_allocator
                    );

                    // Deallocate entire old buffer
                    _M_deallocate(
                        this->_M_start,
                        this->_M_end_of_storage - this->_M_start
                    );

                    // Set start to new allocated buffer
                    this->_M_start = __tmp;

                    // Set _finish to end of existing elements
                    this->_M_finish = __tmp + __old_size;

                    // Set end_of_storage to end of allocated memory
                    this->_M_end_of_storage = this->_M_start + __n;
                }
            }

            void push_back(const value_type& __x) {
                if (this->_M_finish != this->_M_end_of_storage) {
                    this->_M_allocator.construct(this->_M_finish, __x);
                    ++this->_M_finish;
                }
                else
                    _M_realloc_insert(end(), __x);
            }

            iterator insert(iterator __position, const value_type& __x) {
                // Get number of elements before insertion position
                const size_type __n = __position - begin();

                // Choose insert algorithm to use depending on insert position
                if (this->_M_finish != this->_M_end_of_storage) {
                    // We have enough allocated memory to store the new elem
                    if (__position == end()) {
                        // Simply construct the new elem at the end
                        this->_M_allocator.construct(this->_M_finish, __x);

                        // Set new finish position
                        ++this->_M_finish;
                    }
                    else {
                        // We need to move everything after pos without realloc
                        this->_M_no_realloc_insert(__position, __x);
                    }
                }
                else {
                    // Not enough allocated space, so we need to realloc
                    _M_realloc_insert(__position, __x);
                }

                // Return new position
                return iterator(this->_M_start + __n);
            }
        
            template <typename _InputIterator>
            void insert(iterator __position,
                _InputIterator __first, _InputIterator __last) {
                _M_insert_dispatch(
                    __first,
                    __last,
                    ft::is_integral<_InputIterator>()
                );
            }

            iterator erase(iterator __position) {
                if (__position + 1 != end()) {
                    std::copy(__position + 1, end(), __position);
                }
                --this->_M_finish;
                this->_M_allocator.destroy(this->_M_finish);
                return __position;
            }

            iterator erase(iterator __first, iterator __last) {
                if (__first != __last) {
                    if (__last != end()) {
                        std::copy(__last, end(), __first);
                    }
                    _M_erase_at_end(__first + end() - __last);
                }
                return __first;
            }

    };

    template<typename _Tp, typename _Alloc>
    inline bool operator==(const vector<_Tp, _Alloc>& __x,
        const vector<_Tp, _Alloc>& __y) {
        return (__x.size() == __y.size()
            && std::equal(__x.begin(), __x.end(), __y.begin()));
    }

    template<typename _Tp, typename _Alloc>
    inline bool operator!=(const vector<_Tp, _Alloc>& __x,
        const vector<_Tp, _Alloc>& __y) {
        return !(__x == __y);
    }

    
    // Based on operator<
    template<typename _Tp, typename _Alloc>
    inline bool operator>(const vector<_Tp, _Alloc>& __x,
        const vector<_Tp, _Alloc>& __y) {
            return __y < __x;
    }

    // Based on operator<
    template<typename _Tp, typename _Alloc>
    inline bool operator<=(const vector<_Tp, _Alloc>& __x,
        const vector<_Tp, _Alloc>& __y) {
            return !(__y < __x);
    }

    // Based on operator<
    template<typename _Tp, typename _Alloc>
    inline bool operator>=(const vector<_Tp, _Alloc>& __x,
        const vector<_Tp, _Alloc>& __y) {
            return !(__x < __y);
    }
};

#endif // ! VECTOR_HPP
