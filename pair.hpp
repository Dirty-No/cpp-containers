#ifndef PAIR_HPP
# define PAIR_HPP

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

	


};

#endif
