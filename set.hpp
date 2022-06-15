#ifndef SET_HPP
# define SET_HPP
#include <functional>
#include <memory>
#include "tree.hpp"
namespace ft {
	
template <class _Key, class _Compare = std::less<_Key>, class _Alloc = std::allocator<_Key> >
class set {

	public:
	// Member types definition
	typedef _Key key_type;
	typedef _Key value_type;
	typedef _Compare key_compare;
	typedef _Compare value_compare;
	typedef _Alloc allocator_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef typename _Alloc::pointer pointer;
	typedef typename _Alloc::const_pointer const_pointer;
	private:
	// Tree definition
	typedef _Rb_tree<value_type, value_compare, allocator_type> _Tree_type;
	_Tree_type _M_tree;
	public:
	typedef typename _Tree_type::size_type size_type;
	typedef typename _Tree_type::const_iterator iterator;
	typedef typename _Tree_type::const_iterator const_iterator;
	typedef typename _Tree_type::const_reverse_iterator reverse_iterator;
	typedef typename _Tree_type::const_reverse_iterator const_reverse_iterator;
	

		//constructors and destructor
		explicit set(const _Compare &__comp = _Compare(), const allocator_type &__a = allocator_type())
			: _M_tree(__comp, __a) {}
		
		template <class _InputIterator>
		set(_InputIterator __first, _InputIterator __last, const _Compare &__comp = _Compare(), const allocator_type &__a = allocator_type())
			: _M_tree(__first, __last, __comp, __a) {}
		
		set(const set &__x) : _M_tree(__x._M_tree) {}
		
		set(const set &__x, const allocator_type &__a) : _M_tree(__x._M_tree, __a) {}

		//destructor
		~set() {}

		//assignment operator
		set &operator=(const set &__x) {
			if (this != &__x)
				_M_tree = __x._M_tree;
			return *this;
		}

		//get_allocator
		allocator_type get_allocator() const {
			return this->_M_tree.get_allocator();
		}

		//key_comp
		key_compare key_comp() const {
			return key_compare();
		}

		//value_comp
		value_compare value_comp() const {
			return _Compare();
		}

		//begin
		iterator begin() {
			return this->_M_tree.begin();
		}

		const_iterator begin() const {
			return this->_M_tree.begin();
		}

		//end
		iterator end() {
			return this->_M_tree.end();
		}

		const_iterator end() const {
			return this->_M_tree.end();
		}

		//rbegin
		reverse_iterator rbegin() {
			return this->_M_tree.rbegin();
		}

		const_reverse_iterator rbegin() const {
			return this->_M_tree.rbegin();
		}

		//rend
		reverse_iterator rend() {
			return this->_M_tree.rend();
		}

		const_reverse_iterator rend() const {
			return this->_M_tree.rend();
		}

		//size
		size_type size() const {
			return this->_M_tree.size();
		}

		//max_size
		size_type max_size() const {
			return this->_M_tree.max_size();
		}

		//empty
		bool empty() const {
			return this->_M_tree.empty();
		}

		//clear
		void clear() {
			this->_M_tree.clear();
		}

		//swap
		void swap(set &__x) {
			this->_M_tree.swap(__x._M_tree);
		}

		//find
		iterator find(const key_type &__x) {
			return this->_M_tree.find(__x);
		}

		const_iterator find(const key_type &__x) const {
			return this->_M_tree.find(__x);
		}

		//count
		size_type count(const key_type &__x) const {
			return this->_M_tree.count(__x);
		}

		//lower_bound
		iterator lower_bound(const key_type &__x) {
			return this->_M_tree.lower_bound(__x);
		}

		const_iterator lower_bound(const key_type &__x) const {
			return this->_M_tree.lower_bound(__x);
		}

		//upper_bound
		iterator upper_bound(const key_type &__x) {
			return this->_M_tree.upper_bound(__x);
		}

		const_iterator upper_bound(const key_type &__x) const {
			return this->_M_tree.upper_bound(__x);
		}

		//equal_range
		ft::pair<iterator, iterator> equal_range(const key_type &__x) {
			return this->_M_tree.equal_range(__x);
		}

		ft::pair<const_iterator, const_iterator> equal_range(const key_type &__x) const {
			return this->_M_tree.equal_range(__x);
		}

		//operator[]
		key_type &operator[](const key_type &__x) {
			return this->_M_tree[__x].second;
		}

		//at
		key_type &at(const key_type &__x) {
			return this->_M_tree.at(__x).second;
		}

		const key_type &at(const key_type &__x) const {
			return this->_M_tree.at(__x).second;
		}

		//erase
		void erase(iterator __position) {
			this->_M_tree.erase(*__position);
		}

		size_type erase(const key_type &__x) {
			return this->_M_tree.erase(__x);
		}

		void erase(iterator __first, iterator __last) {
			this->_M_tree.erase(__first, __last);
		}

		//insert
		ft::pair<iterator, bool> insert(const value_type &__x) {
			// std::cout << "insert(value)" << std::endl;
			return this->_M_tree.insert(__x);
		}

		iterator insert(iterator __position, const value_type &__x) {
			// std::cout << "insert(pos, value)" << std::endl;
			return this->_M_tree.insert(__position, __x);
		}

		template <class _InputIterator>
		void insert(_InputIterator __first, _InputIterator __last) {
			// std::cout << "insert(first, last)" << std::endl;
			this->_M_tree.insert(__first, __last);
		}
		

	//comparison operators
	// template <class _Key, class _Tp, class _Compare, class _Alloc>
	friend bool operator==(const set<_Key, _Compare, _Alloc> &__x, const set<_Key, _Compare, _Alloc> &__y) {
		return __x._M_tree == __y._M_tree;
	}

	// template <class _Key, class _T, class _Compare, class _Alloc>
	friend bool operator!=(const set<_Key, _Compare, _Alloc> &__x, const set<_Key, _Compare, _Alloc> &__y) {
		return __x._M_tree != __y._M_tree;
	}

	// template <class _Key, class _T, class _Compare, class _Alloc>
	friend bool operator<(const set<_Key, _Compare, _Alloc> &__x, const set<_Key, _Compare, _Alloc> &__y) {
		return __x._M_tree < __y._M_tree;
	}

	// template <class _Key, class _T, class _Compare, class _Alloc>
	friend bool operator<=(const set<_Key, _Compare, _Alloc> &__x, const set<_Key, _Compare, _Alloc> &__y) {
		return __x._M_tree <= __y._M_tree;
	}

	// template <class _Key, class _T, class _Compare, class _Alloc>
	friend bool operator>(const set<_Key, _Compare, _Alloc> &__x, const set<_Key, _Compare, _Alloc> &__y) {
		return __x._M_tree > __y._M_tree;
	}

	// template <class _Key, class _T, class _Compare, class _Alloc>
	friend bool operator>=(const set<_Key, _Compare, _Alloc> &__x, const set<_Key, _Compare, _Alloc> &__y) {
		return __x._M_tree >= __y._M_tree;
	}



};
}

namespace std
{

	//overload swap for set

  template<typename _Key, typename _Compare, typename _Alloc>
  void swap(ft::set<_Key, _Compare, _Alloc>& __x,
	 ft::set<_Key, _Compare, _Alloc>& __y)
    {
		 __x.swap(__y);
	}


};


#endif // !SET_HPP