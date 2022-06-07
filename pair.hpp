#ifndef PAIR_HPP
# define PAIR_HPP

namespace ft {
// https://en.cppreference.com/w/cpp/utility/pair
template<
    class T1,
    class T2
> struct pair {

public:
	typedef T1 first_type;
	typedef T2 second_type;

	T1 first;
	T2 second;

	pair() : first(), second() {}
	pair( const T1& __x, const T2& __y ) : first(__x), second(__y) { }
	
	template< class U1, class U2 >
	pair( const pair<U1, U2>& __p ) : first(__p.first), second(__p.second) { }

	pair& operator=( const pair& __x ) {
		this->first = __x.first;
		this->second = __x.second;
	}

};

template< class T1, class T2 >
bool operator==(const ft::pair<T1,T2>& __x, const ft::pair<T1,T2>& __y ) {	
	return __x.first == __y.first && __x.second == __y.second;
}


template< class T1, class T2 >
bool operator!=(const ft::pair<T1,T2>& __x, const ft::pair<T1,T2>& __y ) {	
	return __x.first != __y.first && __x.second != __y.second;
}

template< class T1, class T2 >
bool operator<(const ft::pair<T1,T2>& __x, const ft::pair<T1,T2>& __y ) {	
	return __x.first < __y.first 
		|| (__x.first == __y.first && __x.second < __y.second);
}

template< class T1, class T2 >
bool operator<=(const ft::pair<T1,T2>& __x, const ft::pair<T1,T2>& __y ) {	
	return __x.first < __y.first 
		|| (__x.first <= __y.first && __x.second <= __y.second);
}

template< class T1, class T2 >
bool operator>(const ft::pair<T1,T2>& __x, const ft::pair<T1,T2>& __y ) {	
	return __x.first > __y.first 
		|| (__x.first == __y.first && __x.second > __y.second);
}

template< class T1, class T2 >
bool operator>=(const ft::pair<T1,T2>& __x, const ft::pair<T1,T2>& __y ) {	
	return __x.first > __y.first 
		|| (__x.first == __y.first && __x.second >= __y.second);
}

template< class T1, class T2 >
ft::pair<T1,T2> make_pair( T1 __x, T2 __u ) { return ft::pair<T1, T2>(__x, __u); }

}

#endif
