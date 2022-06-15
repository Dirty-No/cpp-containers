#include "utils.hpp"
#include <vector>
#include <utility>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include "pair.hpp"
#include<map>
#ifndef  RED_BLACK_HPP
# define RED_BLACK_HPP

namespace ft
{

const char ANSI_BLACK[] = "\x1b[30m";
const char ANSI_RED[] = "\x1b[31m";
const char ANSI_GREEN[] = "\x1b[32m";
const char ANSI_RESET[] = "\x1b[0m";

enum _Rb_color { _S_red = false, _S_black = true };

template<typename _Val>
struct _Rb_node {
	typedef _Rb_node* _Rb_pointer;
	typedef const _Rb_node* _Rb_const_pointer;

	_Rb_color	_M_color;
	_Rb_pointer	_M_parent;
	_Rb_pointer	_M_left;
	_Rb_pointer	_M_right;
	_Val		_M_value;

	_Rb_node() : _M_color(_S_red), _M_parent(0), _M_left(0), _M_right(0) {}
	
	_Rb_node(const _Val& __v) : _M_color(_S_red), _M_parent(0), _M_left(0), _M_right(0), _M_value(__v) {}

	static _Rb_pointer _S_minimum(_Rb_pointer __x) {
		if (!__x) return 0;
		while (__x->_M_left != 0) __x = __x->_M_left;
			return __x;
	}

	static _Rb_const_pointer _S_minimum(_Rb_const_pointer __x) {
		if (!__x) return 0;
		while (__x->_M_left != 0) __x = __x->_M_left;
			return __x;
	}

	static _Rb_pointer _S_maximum(_Rb_pointer __x) {
		if (!__x) return 0;
		while (__x->_M_right != 0) __x = __x->_M_right;
			return __x;
	}

	static _Rb_const_pointer _S_maximum(_Rb_const_pointer __x) {
		if (__x == 0) return 0;
		while (__x->_M_right != 0) __x = __x->_M_right;
			return __x;
	}
};

// Helper type offering value initialization guarantee on the compare functor.
template<typename _Key_compare>
struct _Rb_key_compare
{
	_Key_compare		_M_key_compare;

	_Rb_key_compare() : _M_key_compare()
	{ }

	_Rb_key_compare(const _Key_compare& __comp) : _M_key_compare(__comp)
	{ }

	template<typename _Val>
	bool operator()(const _Val& __lhs, const _Val& __rhs) const
	{
		return this->_M_key_compare(__lhs, __rhs);
	}
};

template<
    class _Val,
    class Compare = std::less<_Val>,
    class Allocator = std::allocator<_Val>
> struct _Rb_tree {
		typedef	_Val value_type;
		typedef	Allocator allocator_type ;
		typedef	std::size_t size_type;
		typedef	std::ptrdiff_t difference_type;
		typedef	value_type & reference;
		typedef	const value_type& const_reference;
		typedef	typename Allocator::pointer pointer;
		typedef	typename Allocator::const_pointer const_pointer;

		typedef  _Rb_node<value_type> _Rb_node_type;
		typedef _Rb_node_type* _Rb_pointer;
		typedef const _Rb_pointer _Rb_const_pointer;

		typedef	typename Allocator::template rebind<_Rb_node_type>::other _Rb_node_allocator;

		Allocator _M_allocator;
		_Rb_node_allocator _M_node_allocator;
		// Compare _M_key_compare;
		typedef _Rb_key_compare<Compare> key_compare;
		key_compare _M_key_compare;

		_Rb_pointer	_M_root;
		_Rb_pointer	_M_leftmost;
		_Rb_pointer	_M_rightmost;
		size_type	_M_size;

		struct _Rb_iterator_hook {
			_Rb_tree* _M_tree;
		};

		typedef	typename Allocator::template rebind<_Rb_iterator_hook>::other _Rb_hook_allocator;
		_Rb_hook_allocator _M_hook_allocator;

		_Rb_iterator_hook *_M_hook;

		_Rb_pointer _M_increment_node(_Rb_pointer __pos) {
			if (__pos == NULL) return this->_M_leftmost;
			if (__pos->_M_right)
				return  _Rb_node_type::_S_minimum(__pos->_M_right);
			
			_Rb_pointer __p = __pos->_M_parent;

			while (__p && __pos == __p->_M_right)
			{
				__pos = __p;
				__p = __p->_M_parent;
			}

			return __p;			
		}

		_Rb_const_pointer _M_increment_node(_Rb_const_pointer __x) const {
			_Rb_pointer __pos = (_Rb_pointer)__x;
			if (__pos == NULL) return this->_M_leftmost;
			if (__pos->_M_right)
			{
				return  _Rb_node_type::_S_minimum(__pos->_M_right);
			}
			
			_Rb_pointer __p = __pos->_M_parent;

			while (__p && __pos == __p->_M_right)
			{
				__pos = __p;
				__p = __p->_M_parent;
			}

			return __p;			
		}

		_Rb_pointer _M_decrement_node(_Rb_pointer __pos) {
			if (__pos == NULL)
				return this->_M_rightmost;
			
			if (__pos->_M_left)
				return _Rb_node_type::_S_maximum(__pos->_M_left);
			
			_Rb_pointer __p = __pos->_M_parent;

			while (__p && __pos == __p->_M_left)
			{
				__pos = __p;
				__p = __p->_M_parent;
			}
			
			return __p;
		}

		_Rb_const_pointer _M_decrement_node(_Rb_const_pointer __x) const {
			_Rb_pointer __pos = (_Rb_pointer)__x;
			if (__pos == NULL)
				return this->_M_rightmost;
			
			if (__pos->_M_left)
				return  _Rb_node_type::_S_maximum(__pos->_M_left);
			
			_Rb_pointer __p = __pos->_M_parent;

			while (__p && __pos == __p->_M_left)
			{
				__pos = __p;
				__p = __p->_M_parent;
			}
			
			return __p;
		}

		struct _Rb_iterator {
				typedef _Val value_type;
				typedef difference_type difference_type;
				typedef std::bidirectional_iterator_tag iterator_category;
				typedef _Val* pointer;
				typedef _Val& reference;

				typedef _Rb_tree::_Rb_pointer _Rb_pointer;

				_Rb_iterator_hook* _M_hook;
				_Rb_pointer _M_node;

				_Rb_iterator() : _M_hook(0), _M_node(0)
				{ }

				_Rb_iterator(_Rb_iterator_hook *__it_hook) : _M_hook(__it_hook), _M_node() 
				{}

				_Rb_iterator(_Rb_iterator_hook *__it_hook, _Rb_pointer _pos) 
					: _M_hook(__it_hook) , _M_node(_pos) {}

				_Rb_iterator(const _Rb_iterator &__x) :
				_M_hook(__x._M_hook), _M_node(__x._M_node) {}
				
				reference operator*() const {
					return *ft::__addressof(_M_node->_M_value);
				}

				pointer operator->() const {
					return ft::__addressof(_M_node->_M_value);
				}

				_Rb_iterator &operator++() {
					this->_M_node = 
						this->_M_hook->_M_tree->_M_increment_node(this->_M_node);
					// std::cout << "this: " << this->_M_node << std::endl;
					return *this;
				}
				
				_Rb_iterator operator++(int) {
					_Rb_iterator __tmp = *this;
					this->_M_node = 
						this->_M_hook->_M_tree->_M_increment_node(this->_M_node);
					return __tmp;
				}

				_Rb_iterator &operator--() {
					this->_M_node = 
						this->_M_hook->_M_tree->_M_decrement_node(this->_M_node);
					return *this;
				}
				
				_Rb_iterator operator--(int) {
					_Rb_iterator __tmp = *this;
					this->_M_node = 
						this->_M_hook->_M_tree->_M_decrement_node(this->_M_node);
					return __tmp;
				}

				friend bool
				operator==(const _Rb_iterator& __x, const _Rb_iterator& __y)
				{ return __x._M_node == __y._M_node; }

				friend bool
				operator!=(const _Rb_iterator& __x, const _Rb_iterator& __y)
				{ return __x._M_node != __y._M_node; }
		};

		//const iterator
		struct _Rb_const_iterator {
				typedef _Val value_type;
				typedef difference_type difference_type;
				typedef std::bidirectional_iterator_tag iterator_category;
				typedef const _Val* pointer;
				typedef const _Val& reference;

				typedef _Rb_tree::_Rb_const_pointer _Rb_const_pointer;
				typedef _Rb_tree::_Rb_pointer _Rb_pointer;


				const _Rb_iterator_hook *_M_hook;
				_Rb_pointer _M_node;

				_Rb_const_iterator() : _M_hook(0), _M_node(0)
				{ }

				_Rb_const_iterator(const _Rb_iterator_hook *_origin) : _M_hook(_origin), _M_node() 
				{}

				_Rb_const_iterator(const _Rb_iterator_hook *_tree, _Rb_const_pointer _pos) 
					: _M_hook(_tree) , _M_node(_pos) {}

				_Rb_const_iterator(const _Rb_const_iterator &__x) :
				_M_hook(__x._M_hook), _M_node(__x._M_node) {}

				_Rb_const_iterator(const _Rb_iterator &__x) :
				_M_hook(__x._M_hook), _M_node(__x._M_node) {}

				reference operator*() const {
					return *ft::__addressof(_M_node->_M_value);
				}

				pointer operator->() const {
					return ft::__addressof(_M_node->_M_value);
				}

				_Rb_const_iterator &operator++() {
					this->_M_node = 
						this->_M_hook->_M_tree->_M_increment_node(this->_M_node);
					return *this;
				}

				_Rb_const_iterator operator++(int) {
					_Rb_const_iterator __tmp = *this;
					this->_M_node = 
						this->_M_hook->_M_tree->_M_increment_node(this->_M_node);
					return __tmp;
				}

				_Rb_const_iterator &operator--() {
					this->_M_node = 
						this->_M_hook->_M_tree->_M_decrement_node(this->_M_node);
					return *this;
				}

				_Rb_const_iterator operator--(int) {
					_Rb_const_iterator __tmp = *this;
					this->_M_node = 
						this->_M_hook->_M_tree->_M_decrement_node(this->_M_node);
					return __tmp;
				}

				friend bool
				operator==(const _Rb_const_iterator& __x, const _Rb_const_iterator& __y)
				{ return __x._M_node == __y._M_node; }

				friend bool
				operator!=(const _Rb_const_iterator& __x, const _Rb_const_iterator& __y)
				{ return __x._M_node != __y._M_node; }
		};

		typedef _Rb_iterator iterator;
		typedef	_Rb_const_iterator const_iterator;
		typedef ft::reverse_iterator<iterator> reverse_iterator;
		typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

		_Rb_tree() : _M_allocator(Allocator()), 
		 _M_node_allocator(_Rb_node_allocator()), _M_key_compare(Compare()),
		 _M_root(NULL), _M_leftmost(_M_root), _M_rightmost(_M_root), _M_size(0)
		
		{
			this->_M_hook = this->_M_hook_allocator.allocate(1);
			this->_M_hook->_M_tree = this;
		}

		explicit _Rb_tree(
			const Compare& comp, const Allocator& alloc = Allocator()
		) : _M_allocator(alloc), _M_node_allocator(_Rb_node_allocator()), _M_key_compare(comp),
			_M_root(NULL), _M_leftmost(_M_root), _M_rightmost(_M_root) , _M_size(0)
		{
			this->_M_hook = this->_M_hook_allocator.allocate(1);
			this->_M_hook->_M_tree = this;
		}
		// todo range and copy construct

		iterator begin() {
			return iterator(this->_M_hook, this->_M_leftmost);
		}

		iterator end() {
			return iterator(this->_M_hook, NULL);
		}
		
		//const
		const_iterator begin() const {
			return const_iterator(this->_M_hook, this->_M_leftmost);
		}

		const_iterator end() const {
			return const_iterator(this->_M_hook, NULL);
		}

		//reverse
		reverse_iterator rbegin() {
			return reverse_iterator(end());
		}

		reverse_iterator rend() {
			return reverse_iterator(begin());
		}

		//const reverse
		const_reverse_iterator rbegin() const {
			return const_reverse_iterator(end());
		}

		const_reverse_iterator rend() const {
			return const_reverse_iterator(begin());
		}

		static void _S_set_parent(_Rb_pointer __node, _Rb_pointer __parent) {
			if (__node) __node->_M_parent = __parent;
		}

		static bool _S_is_red(_Rb_pointer __h) {
			return __h && __h->_M_color == _S_red;
		}

		_Rb_pointer _M_create_node(_Rb_pointer __parent, value_type __val) {
			_Rb_pointer __node = this->_M_node_allocator.allocate(1);
			this->_M_node_allocator.construct(__node, __val);
			__node->_M_parent = __parent;
			return __node;
		}

		_Rb_pointer _M_create_root(value_type __val) {
			this->_M_root = this->_M_node_allocator.allocate(1);
			this->_M_node_allocator.construct(this->_M_root, __val);
			this->_M_root->_M_left = NULL;
			this->_M_root->_M_right = NULL;
			this->_M_root->_M_parent = NULL;
			this->_M_root->_M_color = _S_black;
			this->_M_size = 1;
			this->_M_leftmost = this->_M_root;
			this->_M_rightmost = this->_M_root;
			return this->_M_root;
		}

		static _Rb_pointer	_S_rotate_left(_Rb_pointer __h) {
			// Left-leaning Red-Black Tree implementation

			_Rb_pointer __x = __h->_M_right;
			__h->_M_right = __x->_M_left;
			__x->_M_left = __h;
			__x->_M_color = __h->_M_color;
			__h->_M_color = _S_red;

			// Update parent pointers
			_S_set_parent(__x, __h->_M_parent);
			_S_set_parent(__x->_M_left, __x); // __x->_M_left == __h
			_S_set_parent(__x->_M_right, __x);
			_S_set_parent(__h->_M_left, __h);
			_S_set_parent(__h->_M_right, __h);

			return __x;
		}

		static _Rb_pointer _S_rotate_right(_Rb_pointer __h) {
			// Right-leaning Red-Black Tree implementation
			_Rb_pointer __x = __h->_M_left;
			__h->_M_left = __x->_M_right;
			__x->_M_right = __h;
			__x->_M_color = __h->_M_color;
			__h->_M_color = _S_red;

			// Update parent pointers
			_S_set_parent(__x, __h->_M_parent);
			_S_set_parent(__x->_M_left, __x);
			_S_set_parent(__x->_M_right, __x);
			_S_set_parent(__h->_M_left, __h);
			_S_set_parent(__h->_M_right, __h);

			return __x;
		}

		static void _S_flip_colors(_Rb_pointer __h) {
			__h->_M_color = __h->_M_color == _S_red ? _S_black : _S_red;
			__h->_M_left->_M_color = __h->_M_left->_M_color == _S_red ? _S_black : _S_red;
			__h->_M_right->_M_color = __h->_M_right->_M_color == _S_red ? _S_black : _S_red;
		}

		static _Rb_pointer _S_fixup(_Rb_pointer __h) {
			if (_S_is_red(__h->_M_right) && !_S_is_red(__h->_M_left))
				__h = _S_rotate_left(__h);
			if (_S_is_red(__h->_M_left) && _S_is_red(__h->_M_left->_M_left))
				__h = _S_rotate_right(__h);
			if (_S_is_red(__h->_M_left) && _S_is_red(__h->_M_right))
				_S_flip_colors(__h);

			return __h;
		}

		static _Rb_pointer _S_move_red_left(_Rb_pointer __h) {
			_S_flip_colors(__h);
			if (_S_is_red(__h->_M_right->_M_left)) {
				__h->_M_right = _S_rotate_right(__h->_M_right);
				__h = _S_rotate_left(__h);
				_S_flip_colors(__h);
			}
			return __h;
		}

		static _Rb_pointer _S_move_red_right(_Rb_pointer __h) {
			_S_flip_colors(__h);
			if (_S_is_red(__h->_M_left->_M_left)) {
				__h = _S_rotate_right(__h);
				_S_flip_colors(__h);
			}
			return __h;
		}

		void _M_swap_relink_nodes(_Rb_pointer __a, _Rb_pointer __b)
		{
			_Rb_pointer __ap = __a->_M_parent;
			_Rb_pointer __bp = __b->_M_parent;
			_Rb_pointer __al = __a->_M_left;
			_Rb_pointer __ar = __a->_M_right;
			_Rb_pointer __bl = __b->_M_left;
			_Rb_pointer __br = __b->_M_right;
			_Rb_color __ac = __a->_M_color;
			_Rb_color __bc = __b->_M_color;

			if (__a->_M_left == __b) {
				__a->_M_left = __b->_M_left;
				__b->_M_left = __a;
				__b->_M_parent = __a->_M_parent;
				__a->_M_parent = __b;

				__a->_M_right = __br;
				__b->_M_right = __ar;

				_S_set_parent(__a->_M_right, __a);
				_S_set_parent(__b->_M_right, __b);

				__a->_M_color = __bc;
				__b->_M_color = __ac;

				return;
			}
			if (__a->_M_right == __b) {
				__a->_M_right = __b->_M_right;
				__b->_M_right = __a;
				__b->_M_parent = __a->_M_parent;
				__a->_M_parent = __b;

				__a->_M_left = __bl;
				__b->_M_left = __al;

				_S_set_parent(__a->_M_left, __a);
				_S_set_parent(__b->_M_left, __b);

				__a->_M_color = __bc;
				__b->_M_color = __ac;

				return;
			}

			if (__b->_M_left == __a ||__b->_M_right == __a) {
				this->_M_swap_relink_nodes(__b, __a);
				return;
			}

			if (__a == this->_M_root)
				this->_M_root = __b;
			else if (__b == this->_M_root)
				this->_M_root = __a;
			
			if (__ap) {
				if (__ap->_M_left == __a)
					__ap->_M_left = __b;
				else if (__ap->_M_right == __a)
					__ap->_M_right = __b;
				else {
					throw std::runtime_error("_M_swap_relink_nodes: __a not found in __ap's children");
				}
			}
			if (__bp) {
				if (__bp->_M_left == __b)
					__bp->_M_left = __a;
				else if (__bp->_M_right == __b)
					__bp->_M_right = __a;
				else {
					throw std::runtime_error("_M_swap_relink_nodes: __b not found in __bp's children");
				}
			}

			_S_set_parent(__al, __b);
			_S_set_parent(__ar, __b);
			_S_set_parent(__bl, __a);
			_S_set_parent(__br, __a);

			__a->_M_parent = __bp;
			__b->_M_parent = __ap;

			__a->_M_left = __bl;
			__a->_M_right = __br;

			__b->_M_left = __al;
			__b->_M_right = __ar;

			__a->_M_color = __bc;
			__b->_M_color = __ac;
		}

		_Rb_pointer _M_insert_node(
			_Rb_pointer __h, _Rb_pointer &__new_node_ref,
			const value_type& __val) {

			if (!__h) {
				this->_M_size++;
				return (__new_node_ref = this->_M_create_node(NULL, __val));
			}
			if (this->_M_key_compare(__val, __h->_M_value)) {
				__h->_M_left = this->_M_insert_node(
					__h->_M_left,
					__new_node_ref,
					__val
				);
			
			_S_set_parent(__h->_M_left, __h);

			if (__h == this->_M_leftmost)
				this->_M_leftmost = __new_node_ref;
	
			//} else if (__val > __h->_M_value) {
			} else if (this->_M_key_compare(__h->_M_value, __val)) {
				__h->_M_right = this->_M_insert_node(
					__h->_M_right,
					__new_node_ref,
					__val
				);
				
				if (__h == this->_M_rightmost)
					this->_M_rightmost = __new_node_ref;
				
				_S_set_parent(__h->_M_right, __h);
			} else {
				// Key already exist, just return __h and do nothing
				__new_node_ref = __h;
			}

			return this->_S_fixup(__h);
		}

		ft::pair<iterator, bool> insert(const value_type& __val ) {
			if (!_M_root)
				return ft::make_pair(iterator(this->_M_hook, this->_M_create_root(__val)), true);

			_Rb_pointer __inserted_node = NULL;
			size_type __old_size = this->_M_size;
			
			// std::cout << "lol"	<< std::endl;
			this->_M_root = _M_insert_node(this->_M_root, __inserted_node, __val);
			// std::cout << "af"	<< std::endl;

			_S_set_parent(this->_M_root, NULL);
			_S_set_parent(this->_M_root->_M_left, this->_M_root);
			_S_set_parent(this->_M_root->_M_right, this->_M_root);

			this->_M_root->_M_color = _S_black;

			//awful
			// this->_M_leftmost = _Rb_node_type::_S_minimum(this->_M_root);
			// this->_M_rightmost = _Rb_node_type::_S_maximum(this->_M_root);
			assert_sane();

			return ft::make_pair(
				iterator(this->_M_hook, __inserted_node),
				__old_size != _M_size
			);
		}

		static bool _S_is_fixing_needed(_Rb_pointer __h) {
			if (__h == NULL)
				return false;
			if (_S_is_red(__h->_M_right) && !_S_is_red(__h->_M_left))
				return true;
			if (_S_is_red(__h->_M_left) && _S_is_red(__h->_M_left->_M_left))
				return true;
			return _S_is_red(__h->_M_left) && _S_is_red(__h->_M_right);
		}

		_Rb_pointer _M_recursive_fixup_from_top(_Rb_pointer __h,
			_Rb_pointer __new_node) {
			if (__h == NULL)
				return NULL;
			if (__h == __new_node)
				return __h;
			if (this->_M_key_compare(__h->_M_value, __new_node->_M_value)) {
				__h->_M_right = this->_M_recursive_fixup_from_top(
					__h->_M_right,
					__new_node
				);
				_S_set_parent(__h->_M_right, __h);
			} else {
				__h->_M_left = this->_M_recursive_fixup_from_top(
					__h->_M_left,
					__new_node
				);
				_S_set_parent(__h->_M_left, __h);
			}
			return _S_fixup(__h);
		}

		_Rb_pointer _M_recursive_fixup_from_bottom(_Rb_pointer __h,
			_Rb_pointer __new_node) {
			if (__h->_M_parent == NULL)
				return this->_M_root = _M_recursive_fixup_from_top(
					__h,
					__new_node
				);

			if (_S_is_fixing_needed(__h->_M_left))
				__h->_M_left = this->_M_recursive_fixup_from_top(
					__h->_M_left,
					__new_node
				);

			if (_S_is_fixing_needed(__h->_M_right))
				__h->_M_right = this->_M_recursive_fixup_from_top(
					__h->_M_right,
					__new_node
				);

			_Rb_pointer __grand_parent = __h->_M_parent->_M_parent;

			if (!_S_is_fixing_needed(__h)
				&& !_S_is_fixing_needed(__h->_M_parent)
				&& !_S_is_fixing_needed(__h->_M_parent->_M_left)
				&& !_S_is_fixing_needed(__h->_M_parent->_M_right)
				&& (!__grand_parent
					|| (
							!_S_is_fixing_needed(__grand_parent)
							&& !_S_is_fixing_needed(__grand_parent->_M_left)
							&& !_S_is_fixing_needed(__grand_parent->_M_right)
						)
					)
				)
					return __h;
			
			return this->_M_recursive_fixup_from_bottom(__h->_M_parent, __new_node);
		}

		
		_Rb_pointer _M_insert_hint(_Rb_pointer __hint,
			const value_type& __val) {
			_Rb_pointer __lower_bound = this->_M_decrement_node(__hint);
			_Rb_pointer __upper_bound = this->_M_increment_node(__hint);

			if
			(
				__hint
				&& !_M_key_compare(__hint->_M_value, __val)
				&& !_M_key_compare(__val, __hint->_M_value)
			)
				return __hint;

			if
			(
				__lower_bound 
				&& !_M_key_compare(__lower_bound->_M_value, __val)
				&& !_M_key_compare(__val, __lower_bound->_M_value)
			)
				return __lower_bound;

			if
			(
				__upper_bound
				&& !_M_key_compare(__upper_bound->_M_value, __val)
				&& !_M_key_compare(__val, __upper_bound->_M_value)
			)
				return __upper_bound;

			if
			(
				__upper_bound == NULL
				&& _M_key_compare(this->_M_rightmost->_M_value, __val)
			) {

				_Rb_pointer __new_node = NULL;

				this->_M_rightmost->_M_right =
					this->_M_insert_node(
						this->_M_rightmost->_M_right,
						__new_node,
						__val
					);
				
				_S_set_parent(this->_M_rightmost->_M_right, this->_M_rightmost);

				this->_M_recursive_fixup_from_bottom(__new_node, __new_node);

				this->_M_root->_M_color = _S_black;

				this->_M_rightmost = __new_node;

				return __new_node;
			}

			if
			(
				__lower_bound == NULL
				&& _M_key_compare(__val, this->_M_leftmost->_M_value)
			) {
				
				_Rb_pointer __new_node = NULL;

				this->_M_leftmost->_M_left =
					this->_M_insert_node(
						this->_M_leftmost->_M_left,
						__new_node,
						__val
					);
				
				_S_set_parent(this->_M_leftmost->_M_left, this->_M_leftmost);

				this->_M_recursive_fixup_from_bottom(__new_node, __new_node);

				this->_M_root->_M_color = _S_black;

				this->_M_leftmost = __new_node;

				return __new_node;
			}

			if 
			(
				__lower_bound && __upper_bound
				&& _M_key_compare(__lower_bound->_M_value, __val)
				&& _M_key_compare(__val, __upper_bound->_M_value)
			) {
				
				_Rb_pointer __new_node = NULL;
				return this->insert(__val).first._M_node;
				__hint->_M_right = this->_M_insert_node(
					__hint->_M_right,
					__new_node,
					__val
				);
				_S_set_parent(__hint->_M_right, __hint);
				this->_M_recursive_fixup_from_top(NULL, __new_node);
				this->_M_root->_M_color = _S_black;
				if (__new_node->_M_parent == NULL)
					this->_M_root = __new_node;
				return __new_node;
			}

			// Not in range, fallback to regular insert
			return this->insert(__val).first._M_node;
		}


		iterator insert( iterator __hint, const value_type& __val ) {
			// if (!_M_root)
			// 	return iterator(this->_M_hook, this->_M_create_root(__val));
			
			if (this->size() < 6)
				return this->insert(__val).first;


			iterator it(this->_M_hook,this->_M_insert_hint(__hint._M_node, __val));
			return it;
		}

		template<class _InputIterator>
		void insert (_InputIterator __first, _InputIterator __last) {
			if (__first == __last)
				return;

			if (!_M_root)
			{
				this->_M_create_root(*__first);
				++__first;
			}

			iterator __hint = this->begin();
			while (__first != __last) {
				__hint = insert(__hint, *__first);
				++__first;
			}
			this->assert_sane();
		}

		template< class _InputIterator >
		_Rb_tree( _InputIterator first, _InputIterator last,
		const Compare& comp = Compare(),
		const Allocator& alloc = Allocator() ) : _M_allocator(alloc), 
		 _M_node_allocator(_Rb_node_allocator()), _M_key_compare(comp),
		 _M_root(NULL), _M_leftmost(_M_root), _M_rightmost(_M_root) , _M_size(0)
		 {
			 this->_M_hook = this->_M_hook_allocator.allocate(1);
			 this->_M_hook->_M_tree = this;

			insert(first, last);
		 }

		
		// _M_clone_node
		_Rb_pointer _M_clone_node(_Rb_pointer __x)  {
			if (!__x)
				return NULL;
			_Rb_pointer __new = _M_create_node(NULL, __x->_M_value);
			__new->_M_color = __x->_M_color;
			__new->_M_left = _M_clone_node(__x->_M_left);
			__new->_M_right = _M_clone_node(__x->_M_right);
			_S_set_parent(__new->_M_left, __new);
			_S_set_parent(__new->_M_right, __new);
			return __new;
		}

		_Rb_tree(const _Rb_tree& other) : _M_allocator(other._M_allocator), 
		 _M_node_allocator(_Rb_node_allocator()), _M_key_compare(other._M_key_compare),
		 _M_root(this->_M_clone_node(other._M_root)), _M_leftmost(_Rb_node_type::_S_minimum(this->_M_root)),
		 _M_rightmost(_Rb_node_type::_S_maximum(this->_M_root) ), _M_size(other._M_size)
		 {

			this->_M_hook = this->_M_hook_allocator.allocate(1);
			this->_M_hook->_M_tree = this;
		 }


		// operator=
		_Rb_tree& operator=(const _Rb_tree& other) {
			if (this != &other) {
				this->clear();
				this->_M_root = this->_M_clone_node(other._M_root);
				this->_M_size = other._M_size;
				this->_M_leftmost = _Rb_node_type::_S_minimum(this->_M_root);
				this->_M_rightmost = _Rb_node_type::_S_maximum(this->_M_root);
			}
			return *this;
		}

		// _M_find_node
		_Rb_pointer _M_find_node(const value_type& __k) const {
			_Rb_pointer __y = this->_M_root;
			while (__y) {
				if (this->_M_key_compare(__k, __y->_M_value))
					__y = __y->_M_left;
				else if (this->_M_key_compare(__y->_M_value, __k))
					__y = __y->_M_right;
				else
					return __y;
			}
			return NULL;
		}

		//find
		iterator find(const value_type& __val) {
			_Rb_pointer __node = _M_find_node(__val);
			return iterator(this->_M_hook, __node);
		}

		const_iterator find(const value_type& __val) const {
			_Rb_const_pointer __node = _M_find_node(__val);
			return const_iterator(this->_M_hook, __node);
		}

		//operator[]
		reference operator[](const value_type& __val) {
			_Rb_pointer __node = _M_find_node(__val);
			if (!__node) {
				ft::pair<iterator, bool> __res = this->insert(__val);
				return *(__res.first);
			}
			this->assert_sane();
			return __node->_M_value;
		}

		//at
		reference at(const value_type& __val) {
			_Rb_pointer __node = _M_find_node(__val);
			if (!__node)
				throw std::out_of_range("out of range");
			return __node->_M_value;
		}
		

		static void _S_print_tree(_Rb_const_pointer __root, size_type __level, char __dir) {
			if (__root == NULL)
				return;
			_S_print_tree(__root->_M_left, __level + 1, 'l');
			for (size_type i = 0; i < __level; i++)
				(void)(0||printf("\t"));

			std::cout<<__dir<<(__root->_M_color == _S_black ? ANSI_BLACK : ANSI_RED)<<__root->_M_value<<ANSI_RESET<<std::endl;
			_S_print_tree(__root->_M_right, __level + 1, 'r');
		}

		static size_type _S_count_blacks_from_leaf(_Rb_pointer __leaf) {
			if (__leaf == NULL)
				return 0;
			
			size_type __count = 0;

			while (__leaf) {
				if (!_S_is_red(__leaf))
					__count++;
				__leaf = __leaf->_M_parent;
			}
			return __count;
		}


		// int count;
		// int isInitialized;
		typedef ft::pair<size_type, bool> _count_type;

		static bool _S_check_blacks_helper(_Rb_pointer __root,
			_count_type &__count_test) {
			if (__root == NULL)
				return false;
			
			size_type __count = _S_count_blacks_from_leaf(__root);

			if (__root->_M_left == NULL && __root->_M_right == NULL) {
				if (__count_test.second) { // is initialized
					if (__count_test.first != __count)
						return true;
				} else {
					__count_test.first = __count;
					__count_test.second = true;
				}
			}
			return _S_check_blacks_helper(__root->_M_left, __count_test)
			|| _S_check_blacks_helper(__root->_M_right, __count_test);
		}

		static bool _S_check_black_nodes_count(_Rb_pointer __root) {
			ft::pair<size_type,bool> __c(0,false);
			return _S_check_blacks_helper(__root, __c);
		}

		static bool _S_check_double_reds(_Rb_pointer __root) {
			if (__root == NULL)
				return false;
			if (_S_is_red(__root) && (_S_is_red(__root->_M_left) ||_S_is_red(__root->_M_right)))
				return true;
			return _S_check_double_reds(__root->_M_left) || _S_check_double_reds(__root->_M_right);
		}

		bool _M_check_parents_sanity(_Rb_pointer __root) const {
			if (__root == NULL)
				return false;
			if (__root->_M_parent == NULL)
				return (__root != this->_M_root) || (_M_check_parents_sanity(__root->_M_left) || _M_check_parents_sanity(__root->_M_right));
			if (__root->_M_parent->_M_left == __root->_M_parent->_M_right)
				return true;
			if (__root->_M_parent->_M_left == __root || __root->_M_parent->_M_right == __root)
				return _M_check_parents_sanity(__root->_M_left) || _M_check_parents_sanity(__root->_M_right);
			return true;
		}

		bool _M_check_rightmost_is_correct() const {
			if (this->_M_root == NULL)
				return false;
			
			return _Rb_node_type::_S_maximum(this->_M_root)  != this->_M_rightmost;
		}

		bool _M_check_leftmost_is_correct() const {
			if (this->_M_root == NULL)
				return false;
			return _Rb_node_type::_S_minimum(this->_M_root) != this->_M_leftmost;
		}

		bool _M_check_sanity() const {
			bool res = false;
			if ((res = this->_M_check_parents_sanity(this->_M_root)))
				(void)(0||printf("%s[KO] RBTree PARENTS ARE BROKEN%s\n", ANSI_RED, ANSI_RESET));
			if (_S_is_red(this->_M_root) && (res = 1))
				(void)(0||printf("%s[KO] RBTree VIOLATES RULE 2 (this->_M_root must be black)%s\n", ANSI_RED, ANSI_RESET));
			if (_S_check_double_reds(this->_M_root) && (res = 1))
				(void)(0||printf("%s[KO] RBTree VIOLATES RULE 3 (no double red nodes)%s\n", ANSI_RED, ANSI_RESET));
			if (_S_check_black_nodes_count(this->_M_root)  && (res = 1))
				(void)(0||printf("%s[KO] RBTree VIOLATES RULE 4 (same number of black nodes for all paths to a leave)%s\n", ANSI_RED, ANSI_RESET));
			if (_M_check_rightmost_is_correct() && (res = 1))
				(void)(0||printf("%s[KO] RBTree RIGHTMOST IS WRONG%s\n", ANSI_RED, ANSI_RESET));
			if (_M_check_leftmost_is_correct() && (res = 1))
				(void)(0||printf("%s[KO] RBTree LEFTMOST IS WRONG%s\n", ANSI_RED, ANSI_RESET));
			if (!res)
				(void)(1||printf("%s[OK] RBTree IS OK%s\n", ANSI_GREEN, ANSI_RESET));
			return !res;
		}

		bool sane() const {
			return this->_M_check_sanity();
		}

		void assert_sane() const {
			assert(this->sane());
		}

		void print() const {
			_S_print_tree(this->_M_root, 0, 'R');
		}

		void _M_clear_node(_Rb_pointer __node) {
			if (__node == NULL)
				return;
			_M_clear_node(__node->_M_left);
			_M_clear_node(__node->_M_right);

			// this->_M_allocator.destroy(ft::__addressof(__node->_M_value));
			// this->_M_allocator.deallocate(ft::__addressof(__node->_M_value), 1);
			this->_M_node_allocator.destroy(__node);
			this->_M_node_allocator.deallocate(__node, 1);
		}

		void clear() {
			this->_M_clear_node(this->_M_root);
			this->_M_root = NULL;
			this->_M_leftmost = NULL;
			this->_M_rightmost = NULL;
			this->_M_size = 0;
		}

		~_Rb_tree() {
			this->clear();
			this->_M_hook_allocator.deallocate(this->_M_hook, 1);
		}

		//key_comp
		key_compare key_comp() const {
			return this->_M_key_compare();
		}

		//empty
		bool empty() const {
			return this->_M_root == NULL;
		}

		//size
		size_type size() const {
			return this->_M_size;
		}

		//max_size
		size_type max_size() const {
			return (this->_M_node_allocator.max_size());
		}

		//swap
		void swap(_Rb_tree& __t) {
			std::swap(this->_M_root, __t._M_root);
			std::swap(this->_M_leftmost, __t._M_leftmost);
			std::swap(this->_M_rightmost, __t._M_rightmost);
			std::swap(this->_M_size, __t._M_size);
			std::swap(this->_M_key_compare, __t._M_key_compare);
			std::swap(this->_M_node_allocator, __t._M_node_allocator);
			std::swap(this->_M_hook, __t._M_hook);
			this->_M_hook->_M_tree = this;
			__t._M_hook->_M_tree = &__t;
		}

		//get_allocator
		allocator_type get_allocator() const {
			return this->_M_allocator;
		}

		//count
		size_type count(const value_type& __k) const {
			return this->find(__k) == this->end() ? 0 : 1;
		}

		//lower_bound
		iterator lower_bound(const value_type& __k) {
			iterator __it = iterator(this->_M_hook, this->_M_leftmost);
			while (__it != end() && this->_M_key_compare(*__it, __k))
				++__it;
			return __it;
		}

		//const
		const_iterator lower_bound(const value_type& __k) const {
			const_iterator __it = const_iterator(this->_M_hook, this->_M_leftmost);
			while (__it != end() && this->_M_key_compare(*__it, __k))
				++__it;
			return __it;
		}

		//upper_bound
		iterator upper_bound(const value_type& __k) {
			iterator __it = iterator(this->_M_hook, this->_M_leftmost);
			while (__it != end() && !this->_M_key_compare(__k, *__it))
				++__it;
			return __it;
		}

		//const
		const_iterator upper_bound(const value_type& __k) const {
			const_iterator __it = const_iterator(this->_M_hook, this->_M_leftmost);
			while (__it != end() && !this->_M_key_compare(__k, *__it))
				++__it;
			return __it;
		}

		//equal_range
		ft::pair<iterator, iterator> equal_range(const value_type& __k) {
			iterator __first = iterator(this->_M_hook, this->_M_leftmost);

			while (__first != end() && this->_M_key_compare(*__first, __k))
				++__first;
			iterator __last = __first;
			while (__last != end() && !this->_M_key_compare(__k, *__last))
				++__last;
			return ft::pair<iterator, iterator>(__first, __last);
		}

		//const
		ft::pair<const_iterator, const_iterator> equal_range(const value_type& __k) const {
			const_iterator __it = this->lower_bound(__k);
			const_iterator __it2 = this->upper_bound(__k);
			return ft::pair<const_iterator, const_iterator>(__it, __it2);
		}

		void _M_destroy_node(_Rb_pointer __node) {
			this->_M_node_allocator.destroy(__node);
			this->_M_node_allocator.deallocate(__node, 1);
		}

		//_M_erase_node
		_Rb_pointer _M_erase_node(_Rb_pointer __h, const value_type &__val) {
			if (this->_M_key_compare(__val, __h->_M_value)) {
			// if (__val < __h->_M_value) {
				if (!_S_is_red(__h->_M_left) && !_S_is_red(__h->_M_left->_M_left))
					__h = _S_move_red_left(__h);
				
				const _Rb_pointer __old_left = __h->_M_left;

				__h->_M_left = this->_M_erase_node(__h->_M_left, __val);
				_S_set_parent(__h->_M_left, __h);

				if (__old_left == this->_M_leftmost)
					this->_M_leftmost = _Rb_node_type::_S_minimum(__h);
			} else {
				if (_S_is_red(__h->_M_left))
					__h = _S_rotate_right(__h);
				// if (__val == __h->_M_value && __h->_M_right == NULL) { // to change to comp
				if (!this->_M_key_compare(__val, __h->_M_value)
					&& !this->_M_key_compare(__h->_M_value, __val)
					&& __h->_M_right == NULL) {
					this->_M_destroy_node(__h);
					this->_M_size--;
					return NULL;
				}
				if (!_S_is_red(__h->_M_right) && !_S_is_red(__h->_M_right->_M_left))
					__h = _S_move_red_right(__h);
				
				const _Rb_pointer __old_right = __h->_M_right;

				// if (__val == __h->_M_value) {// to change to comp
				if (!this->_M_key_compare(__h->_M_value, __val)
				 && !this->_M_key_compare(__val, __h->_M_value)) {
					_Rb_pointer __x = _Rb_node_type::_S_minimum(__h->_M_right);

					// std::cout << "RELINKING SHIT" << std::endl;
					this->_M_swap_relink_nodes(__h, __x);
					_Rb_pointer __tmp = __x;
					__x = __h;
					__h = __tmp;

					__h->_M_right = this->_M_erase_node(__h->_M_right, __x->_M_value);
				} else
					__h->_M_right = this->_M_erase_node(__h->_M_right, __val);
				_S_set_parent(__h->_M_right, __h);
				if (__old_right == this->_M_rightmost)
					this->_M_rightmost = __h->_M_right ? _Rb_node_type::_S_maximum(__h->_M_right) : __h;
			}
			return _S_fixup(__h);
		}

		size_type erase(const value_type &__val) {
			if (this->_M_find_node(__val) == NULL)
				return 0;
			const size_type __old_size = this->_M_size;
			this->_M_root = this->_M_erase_node(this->_M_root, __val);
			if (this->_M_root == NULL) {
				this->_M_leftmost = NULL;
				this->_M_rightmost = NULL;
			} else {
				this->_M_root->_M_color = _S_black;

				_S_set_parent(this->_M_root, NULL);
				_S_set_parent(this->_M_root->_M_left, this->_M_root);
				_S_set_parent(this->_M_root->_M_right, this->_M_root);

				//awful
				// this->_M_leftmost = _Rb_node_type::_S_minimum(this->_M_root);
				// this->_M_rightmost = _Rb_node_type::_S_maximum(this->_M_root);
			}
			this->assert_sane();

			return __old_size == this->_M_size ? 0 : 1;
		}

		void erase(iterator __pos) {
			if (__pos != this->end())
				this->erase(__pos._M_node->_M_value);
			this->assert_sane();
		}

		void erase(iterator __first, iterator __last) {
			while (__first != __last)
			{
				iterator __tmp = __first;
				++__tmp;

				this->erase(__first);
				__first = __tmp;
			}
			this->assert_sane();
		}
		
		
};

//comparison operators
template <class _Value, class _Compare, class _Alloc>
inline bool operator==(const _Rb_tree<_Value, _Compare, _Alloc>& __x, const _Rb_tree<_Value, _Compare, _Alloc>& __y) {

	// for (typename _Rb_tree<_Value, _Compare, _Alloc>::const_iterator __it = __x.begin(); __it != __x.end(); ++__it) {
	// 	std::cout << __it->first << "@@@";
	// }
	// std::cout << std::endl;

	// for (typename _Rb_tree<_Value, _Compare, _Alloc>::const_iterator __it = __y.begin(); __it != __y.end(); ++__it) {
	// 	std::cout << __it->first << "###";
	// }
	// std::cout << std::endl;

	return __x.size() == __y.size() && std::equal(__x.begin(), __x.end(), __y.begin());
}

//operator!=
template <class _Value, class _Compare, class _Alloc>
inline bool operator!=(const _Rb_tree<_Value, _Compare, _Alloc>& __x, const _Rb_tree<_Value, _Compare, _Alloc>& __y) {
	return !(__x == __y);
}

//operator<
template <class _Value, class _Compare, class _Alloc>
inline bool operator<(const _Rb_tree<_Value, _Compare, _Alloc>& __x, const _Rb_tree<_Value, _Compare, _Alloc>& __y) {
	return std::lexicographical_compare(__x.begin(), __x.end(), __y.begin(), __y.end());
}

//operator>
template <class _Value, class _Compare, class _Alloc>
inline bool operator>(const _Rb_tree<_Value, _Compare, _Alloc>& __x, const _Rb_tree<_Value, _Compare, _Alloc>& __y) {
	return __y < __x;
}

//operator<=
template <class _Value, class _Compare, class _Alloc>
inline bool operator<=(const _Rb_tree<_Value, _Compare, _Alloc>& __x, const _Rb_tree<_Value, _Compare, _Alloc>& __y) {
	return !(__y < __x);
}

//operator>=
template <class _Value, class _Compare, class _Alloc>
inline bool operator>=(const _Rb_tree<_Value, _Compare, _Alloc>& __x, const _Rb_tree<_Value, _Compare, _Alloc>& __y) {
	return !(__x < __y);
}

//std::swap
template <class _Value, class _Compare, class _Alloc>
inline void swap(_Rb_tree<_Value, _Compare, _Alloc>& __x, _Rb_tree<_Value, _Compare, _Alloc>& __y) {
	__x.swap(__y);
}

} // namespace ft
/*
int main(void) {
	ft::_Rb_tree<int> lol;

	lol.insert(27);

	ft::_Rb_tree<int>::iterator lolit = lol.begin();
	printf("%p\n", lolit._M_node);
	printf("%d\n", *lolit);
	lol.print();
	for (int i = 0; i < 150; i++)
	{
		lol.insert(i);
		std::cout << "------------------------------------------\n";
		// lol.print();
	}
	assert(lol._M_rightmost->_M_value == 149);
	lol.print();
	std::cout << lol._M_leftmost->_M_value << std::endl;

	std::cout << "---------ICNREMENT ------------\n";	
	for (ft::_Rb_tree<int>::iterator it = lol.begin(); it != lol.end(); it++) {
		std::cout << "it: " << *it << std::endl;
	}

	std::cout << "\n---------DECREMENT ------------\n";	
	std::cout << *(--lol.end()) << std::endl;
	for (ft::_Rb_tree<int>::iterator it = --lol.end(); it != lol.end(); it--) {
		std::cout << "it: " << *it << std::endl;
	}

	//find
	std::cout << "\n---------FIND ------------\n";
	ft::_Rb_tree<int>::iterator it = lol.find(27);
	std::cout << "it: " << *it << std::endl;
	assert(*it == 27);

	std::cout << "\n---------FIND CONST ------------\n";
	ft::_Rb_tree<int>::const_iterator cit = lol.find(27);
	std::cout << "cit: " << *cit << std::endl;
	assert(*cit == 27);

	std::cout << "\n---------CONST BEGIN/END------------\n";
	for (ft::_Rb_tree<int>::const_iterator cit = lol.begin(); cit != lol.end(); cit++) {
		std::cout << "cit: " << *cit << std::endl;
	}
	// const decrement
	std::cout << "\n---------CONST DECREMENT ------------\n";
	for (ft::_Rb_tree<int>::const_iterator cit = --lol.end(); cit != lol.end(); cit--) {
		std::cout << "cit: " << *cit << std::endl;
	}

	std::cout << "\n--------- INSERT RANGE ------------\n";
	std::vector<int> v;
	for (int i = 0; i < 100; i++)
		v.push_back(i);
	lol.insert(v.begin(), v.end());
	// lol.print();

	std::cout << "\n--------- REVERSE ITERATOR ------------\n";
	for (ft::_Rb_tree<int>::reverse_iterator rit = lol.rbegin(); rit != lol.rend(); rit++) {
		std::cout << "rit: " << *rit << std::endl;
	}

	std::cout << "\n--------- CONST REVERSE ITERATOR ------------\n";
	for (ft::_Rb_tree<int>::const_reverse_iterator crit = lol.rbegin(); crit != lol.rend(); crit++) {
		std::cout << "crit: " << *crit << std::endl;
	}

	std::cout << "\n--------- MAX SIZE ------------\n";
	std::cout << lol.max_size() << std::endl;

	std::cout << "\n--------- COMPARISON OPERATORS ------------\n";
	ft::_Rb_tree<int> lol2;
	lol2.insert(27);
	std::cout << (lol == lol2) << std::endl;
	std::cout << (lol != lol2) << std::endl;
	std::cout << (lol < lol2) << std::endl;
	std::cout << (lol > lol2) << std::endl;
	std::cout << (lol <= lol2) << std::endl;
	std::cout << (lol >= lol2) << std::endl;

	std::cout << "\n--------- SWAP ------------\n";
	ft::_Rb_tree<int> lol3;
	lol3.insert(27);
	lol.swap(lol3);

	std::cout << "\n--------- ERASE ------------\n";
	lol.erase(lol.find(27));
	// lol.print();

	std::cout << "\n--------- ERASE ITERATOR ------------\n";
	lol.erase(lol.begin());

	std::cout << "\n--------- ERASE RANGE ------------\n";
	lol.erase(lol.begin(), lol.end());
	assert(lol.empty());

	std::cout << "\n--------- MAP ------------\n";
	ft::map<int, int> map4;
	map4.insert(ft::make_pair(27, 27));
	map4.insert(ft::make_pair(28, 28));
	map4.insert(ft::make_pair(29, 29));
	map4.insert(ft::make_pair(30, 30));

	std::cout << "map4.size(): " << map4.size() << std::endl;
	std::cout << "map4.max_size(): " << map4.max_size() << std::endl;
	std::cout << "map4.empty(): " << map4.empty() << std::endl;
	std::cout << "map4.find(27): " << map4.find(27)->first << std::endl;
	std::cout << "map4.find(28): " << map4.find(28)->first << std::endl;
	std::cout << "map4.find(29): " << map4.find(29)->first << std::endl;
	std::cout << "map4.find(30): " << map4.find(30)->first << std::endl;

	std::cout << "\n--------- MAP CONST ------------\n";
	ft::map<int, int> const map5 = map4;
	std::cout << "map5.size(): " << map5.size() << std::endl;
	std::cout << "map5.max_size(): " << map5.max_size() << std::endl;
	std::cout << "map5.empty(): " << map5.empty() << std::endl;
	std::cout << "map5.find(27): " << map5.find(27)->first << std::endl;

	std::cout << "\n--------- MAP ITERATOR ------------\n";
	for (ft::map<int, int>::iterator it = map4.begin(); it != map4.end(); it++) {
		std::cout << "it: " << it->first << " " << it->second << std::endl;
	}

	std::cout << "\n--------- MAP CONST ITERATOR ------------\n";
	for (ft::map<int, int>::const_iterator it = map4.begin(); it != map4.end(); it++) {
		std::cout << "it: " << it->first << " " << it->second << std::endl;
	}

	std::cout << "\n--------- MAP REVERSE ITERATOR ------------\n";
	for (ft::map<int, int>::reverse_iterator it = map4.rbegin(); it != map4.rend(); it++) {
		std::cout << "it: " << it->first << " " << it->second << std::endl;
	}

	std::cout << "\n--------- MAP CONST REVERSE ITERATOR ------------\n";
	for (ft::map<int, int>::const_reverse_iterator it = map4.rbegin(); it != map4.rend(); it++) {
		std::cout << "it: " << it->first << " " << it->second << std::endl;
	}

	std::cout << "\n--------- MAP INSERT ------------\n";
	for (int i = 0; i < 100; i++)
		map4.insert(ft::make_pair(i, i));
		
	std::cout << "map4.size(): " << map4.size() << std::endl;
	std::cout << "map4.max_size(): " << map4.max_size() << std::endl;
	std::cout << "map4.empty(): " << map4.empty() << std::endl;
	std::cout << "map4.find(27): " << map4.find(27)->first << std::endl;


	std::cout << "\n--------- MAP VALUE COMPARE ------------\n";
	ft::map<int, int> map6;
	ft::map<int, int>::value_compare vc(map6.value_comp());
	(void)vc;



}
*/
#endif // ! RED_BLACK_HPP
