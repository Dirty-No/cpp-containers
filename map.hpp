#ifndef MAP_HPP
# define MAP_HPP
#include "tree.hpp"
#include "pair.hpp"
namespace ft {

template <class _Key, class _T, class _Compare = std::less<_Key>, class _Alloc = std::allocator<ft::pair<const _Key, _T> > >
class map {
	public:
		typedef _Key key_type;
		typedef _T mapped_type;
		typedef ft::pair<const _Key, _T> value_type;
	private:
		// pair compare from _Compare
		template<class _Pair>
		struct _Pair_compare {
			_Compare _M_comp;
			_Pair_compare(_Compare __c) : _M_comp(__c) {}
			bool operator()(const _Pair& __x, const _Pair& __y) const {
				return _M_comp(__x.first, __y.first);
			}
		};
	public:
		typedef _Pair_compare<value_type> value_compare;
	private:
		
		// typedef _Rb_tree<ft::pair<_Key, _T>, _Compare, _Alloc> _Tree_type;
		// typedef _Rb_tree<ft::pair<_Key, _T>, _Pair_compare, _Alloc> _Tree_type;
		typedef _Rb_tree<value_type, _Pair_compare< value_type >, _Alloc> _Tree_type;
		_Tree_type _M_tree;

	public:

		typedef typename _Tree_type::size_type size_type;
		typedef typename _Tree_type::difference_type difference_type;
		typedef typename _Tree_type::pointer pointer;
		typedef typename _Tree_type::const_pointer const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef typename _Tree_type::iterator iterator;
		typedef typename _Tree_type::const_iterator const_iterator;
		typedef typename _Tree_type::reverse_iterator reverse_iterator;
		typedef typename _Tree_type::const_reverse_iterator const_reverse_iterator;

		typedef typename _Tree_type::allocator_type allocator_type;
		typedef _Compare key_compare;


		//constructors and destructor
		explicit map(const _Compare &__comp = _Compare(), const allocator_type &__a = allocator_type())
			: _M_tree(__comp, __a) {}
		
		template <class _InputIterator>
		map(_InputIterator __first, _InputIterator __last, const _Compare &__comp = _Compare(), const allocator_type &__a = allocator_type())
			: _M_tree(__first, __last, __comp, __a) {}
		
		map(const map &__x) : _M_tree(__x._M_tree) {}
		
		map(const map &__x, const allocator_type &__a) : _M_tree(__x._M_tree, __a) {}

		//destructor
		~map() {}

		//assignment operator
		map &operator=(const map &__x) {
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
		void swap(map &__x) {
			this->_M_tree.swap(__x._M_tree);
		}

		//find
		iterator find(const key_type &__x) {
			return this->_M_tree.find(value_type(__x, mapped_type()));
		}

		const_iterator find(const key_type &__x) const {
			return this->_M_tree.find(value_type(__x, mapped_type()));
		}

		//count
		size_type count(const key_type &__x) const {
			return this->_M_tree.count(value_type(__x, mapped_type()));
		}

		//lower_bound
		iterator lower_bound(const key_type &__x) {
			return this->_M_tree.lower_bound(value_type(__x, mapped_type()));
		}

		const_iterator lower_bound(const key_type &__x) const {
			return this->_M_tree.lower_bound(value_type(__x, mapped_type()));
		}

		//upper_bound
		iterator upper_bound(const key_type &__x) {
			return this->_M_tree.upper_bound(value_type(__x, mapped_type()));
		}

		const_iterator upper_bound(const key_type &__x) const {
			return this->_M_tree.upper_bound(value_type(__x, mapped_type()));
		}

		//equal_range
		ft::pair<iterator, iterator> equal_range(const key_type &__x) {
			return this->_M_tree.equal_range(value_type(__x, mapped_type()));
		}

		ft::pair<const_iterator, const_iterator> equal_range(const key_type &__x) const {
			return this->_M_tree.equal_range(value_type(__x, mapped_type()));
		}

		//operator[]
		mapped_type &operator[](const key_type &__x) {
			return this->_M_tree[value_type(__x, mapped_type())].second;
		}

		//at
		mapped_type &at(const key_type &__x) {
			return this->_M_tree.at(value_type(__x, mapped_type())).second;
		}

		const mapped_type &at(const key_type &__x) const {
			return this->_M_tree.at(value_type(__x, mapped_type())).second;
		}

		//erase
		void erase(iterator __position) {
			this->_M_tree.erase(__position);
		}

		size_type erase(const key_type &__x) {
			return this->_M_tree.erase(value_type(__x, mapped_type()));
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
	friend bool operator==(const map<_Key, _T, _Compare, _Alloc> &__x, const map<_Key, _T, _Compare, _Alloc> &__y) {
		return __x._M_tree == __y._M_tree;
	}

	// template <class _Key, class _T, class _Compare, class _Alloc>
	friend bool operator!=(const map<_Key, _T, _Compare, _Alloc> &__x, const map<_Key, _T, _Compare, _Alloc> &__y) {
		return __x._M_tree != __y._M_tree;
	}

	// template <class _Key, class _T, class _Compare, class _Alloc>
	friend bool operator<(const map<_Key, _T, _Compare, _Alloc> &__x, const map<_Key, _T, _Compare, _Alloc> &__y) {
		return __x._M_tree < __y._M_tree;
	}

	// template <class _Key, class _T, class _Compare, class _Alloc>
	friend bool operator<=(const map<_Key, _T, _Compare, _Alloc> &__x, const map<_Key, _T, _Compare, _Alloc> &__y) {
		return __x._M_tree <= __y._M_tree;
	}

	// template <class _Key, class _T, class _Compare, class _Alloc>
	friend bool operator>(const map<_Key, _T, _Compare, _Alloc> &__x, const map<_Key, _T, _Compare, _Alloc> &__y) {
		return __x._M_tree > __y._M_tree;
	}

	// template <class _Key, class _T, class _Compare, class _Alloc>
	friend bool operator>=(const map<_Key, _T, _Compare, _Alloc> &__x, const map<_Key, _T, _Compare, _Alloc> &__y) {
		return __x._M_tree >= __y._M_tree;
	}



};


}

namespace std
{

	//overload swap for map

  template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
  void swap(ft::map<_Key, _Tp, _Compare, _Alloc>& __x,
	 ft::map<_Key, _Tp, _Compare, _Alloc>& __y)
    {
		 __x.swap(__y);
	}

}

#endif // !MAP_HPP
