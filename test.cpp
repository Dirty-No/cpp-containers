#ifndef TEST_HPP
# define TEST_HPP
#define ASSERT_EQUAL(a, b) if (!(a == b)) { std::cout << "Error: " << #a << " != " << #b << std::endl; std::cout << "a: " << a << std::endl; std::cout << "b: " << b << std::endl; assert(a == b);}

#ifndef NS
# define NS ft

#include <map>
#include <string>
#include <iostream>
#include <stack>
#include <vector>
#include "vector.hpp"
#include "stack.hpp"
#include "pair.hpp"
#include "map.hpp"

#define VECTOR_HPP
#define STACK_HPP
#define PAIR_HPP
#define MAP_HPP

#endif // !NS


#ifdef MAP_HPP
template<typename K, typename T>
std::ostream& operator<<(std::ostream& os, const std::map<K, T>& m)
{
	os << "{";
	for (typename std::map<K,T>::const_iterator it = m.begin(); it != m.end(); ++it) {
		os << it->first << ": " << it->second << ", ";
	}
	os << "}";
	return os;
}

template<typename K, typename T>
std::ostream& operator<<(std::ostream& os, const ft::map<K, T>& m)
{
	os << "{";
	for (typename ft::map<K,T>::const_iterator it = m.begin(); it != m.end(); ++it) {
		os << it->first << ": " << it->second << ", ";
	}
	os << "}";
	return os;
}
#endif

#ifdef VECTOR_HPP
//vector print
template<typename T>
std::ostream& operator<<(std::ostream& os, const NS::vector<T>& v)
{
	os << "{";
	for (typename NS::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
		os << *it << ", ";
	}
	os << "}";
	return os;
}
#endif

#include <sstream>


namespace ft
{
	template <typename T>
	std::string
		to_string(T a)
	{
		std::ostringstream temp;

		temp << a;
		return (temp.str());
	}
}

#ifdef STACK_HPP
static int
test_stack()
{
	NS::stack <int> s;

	assert(s.empty());
	s.push(1);
	assert(!s.empty());
	assert(s.top() == 1);
	s.push(2);
	assert(s.top() == 2);
	assert(s.size() == 2);
	s.push(3);
	assert(s.top() == 3);
	assert(s.size() == 3);
	s.pop();
	assert(s.top() == 2);
	assert(s.size() == 2);
	s.pop();
	assert(s.top() == 1);
	assert(s.size() == 1);
	s.pop();
	assert(s.empty());
	assert(s.size() == 0);
	//lexicographical comparaison
	s.push(1);
	s.push(2);
	s.push(3);
	NS::stack <int> s2;
	s2.push(1);
	s2.push(2);
	s2.push(3);
	assert(s == s2);
	assert(s >= s2);
	assert(s <= s2);
	assert(!(s != s2));
	s2.pop();
	assert(s != s2);
	s2.push(2);
	assert(s > s2);
	assert(s >= s2);
	assert(!(s < s2));
	assert(!(s <= s2));
	assert(s2 < s);
	s2.pop();
	s2.push(4);
	assert(s < s2);
	assert(s2 > s);
	assert(s <= s2);
	assert(s2 >= s);
	assert(s != s2);
	assert(s2 != s);
	assert(!(s2 == s));
	assert(!(s == s2));
	return 0;
}
#endif

#ifdef VECTOR_HPP
template <class T>
static void
	init_vector(T& vref)
{
	vref.push_back(1);
	vref.push_back(2);
	vref.push_back(3);
}

static int
	mli_vector() {

	NS::vector<int> vct(7);
	NS::vector<int> vct_two(4);
	NS::vector<int> vct_three;
	NS::vector<int> vct_four;

	for (unsigned long int i = 0; i < vct.size(); ++i)
		vct[i] = (vct.size() - i) * 3;
	for (unsigned long int i = 0; i < vct_two.size(); ++i)
		vct_two[i] = (vct_two.size() - i) * 5;


	vct_three.assign(vct.begin(), vct.end());
	assert(vct_three.size() == vct.size());
	vct.assign(vct_two.begin(), vct_two.end());
	assert(vct.size() == vct_two.size());
	
	vct_two.assign(2, 42);
	assert(vct_two.size() == 2);

	vct_four.assign(4, 21);
	assert(vct_four.size() == 4);

	vct_four.assign(6, 84);
	assert(vct_four.size() == 6);

	vct.assign(5, 53);
	assert(vct.size() == 5);
	
	vct_two.assign(vct_three.begin(), vct_three.begin() + 3);

	assert(vct_two.size() == 3);

	return (0);

}

static int
	test_vector()
{

	//cut insert
	NS::vector<int> v17(5);
	NS::vector<int> v18;
	const int cut = 3;
	for (NS::vector<int>::size_type i = 0; i < v17.size(); i++)
		v17[i] = (v17.size() - i) * 7;
	assert(v17.size() == 5);
	assert(v17[0] == 35);
	assert(v17[1] == 28);
	assert(v17[2] == 21);
	assert(v17[3] == 14);
	assert(v17[4] == 7);
	v18.insert(v18.begin(), v17.begin(), v17.begin() + cut);
	assert(v18.size() == cut);
	assert(v18[0] == 35);
	assert(v18[1] == 28);
	assert(v18[2] == 21);
	std::cout << v17 << std::endl << "            |" << std::endl;
	std::cout << v18 << std::endl;
	v18.insert(v18.begin(), v17.begin() + cut, v17.end());
	std::cout << " = " << v18 << std::endl;

	assert(v18.size() == v17.size());
	assert(v18[0] == 14);
	assert(v18[1] == 7);
	assert(v18[2] == 35);
	assert(v18[3] == 28);
	assert(v18[4] == 21);
	v18.insert(v18.end(), v17.begin(), v17.begin() + cut);
	assert(v18.size() == 8);
	assert(v18[0] == 14);
	assert(v18[1] == 7);
	assert(v18[2] == 35);
	assert(v18[3] == 28);
	assert(v18[4] == 21);
	assert(v18[5] == 35);
	assert(v18[6] == 28);
	assert(v18[7] == 21);
	std::cout << "before insert 42          " << v18 << std::endl;
	v18.insert(v18.end(), 42);
	std::cout << "after insert 42           " << v18 << std::endl;
	v18.insert(v18.begin() + 5, 84);
	std::cout << "after insert 84 after 21  " << v18 << std::endl;
	std::cout << "v18.size() = " << v18.size() << std::endl;
	assert(v18.size() == 10);
	assert(v18[0] == 14);
	assert(v18[1] == 7);
	assert(v18[2] == 35);
	assert(v18[3] == 28);
	assert(v18[4] == 21);
	assert(v18[5] == 84);
	assert(v18[6] == 35);
	assert(v18[7] == 28);
	assert(v18[8] == 21);
	assert(v18[9] == 42);	

	//comparison
	NS::vector<int> v15(4);
	NS::vector<int> v16(4);
	v16.resize(10);
	v15[2]=42;
	std::cout << v15 << std::endl;
	std::cout << v16 << std::endl;
	assert(v16.size() == 10);
	assert(v15[2] == 42);
	assert(!(v15 == v16));
	assert(v15 != v16);
	assert(!(v15 < v16));
	assert(!(v15 <= v16));
	assert(v15 > v16);
	assert(v15 >= v16);
	//end insert
	NS::vector<std::string> v12;
	v12.push_back("hello");
	v12.push_back("world");
	v12.push_back("!");
	v12.insert(v12.end(), "ola");
	assert(v12.size() == 4);
	assert(v12[3] == "ola");

	NS::vector<int> v13;
	for (unsigned long int i = 0; i < v13.size(); ++i)
		v13[i] = (v13.size() - i) * 3;
	NS::vector<int> v14;
	v14.insert(v14.end(), 42);
	assert(v14.size() == 1);
	assert(v14[0] == 42);
	v14.insert(v14.begin(), 2 , 21);
	assert(v14.size() == 3);
	ASSERT_EQUAL(v14[0], 21);
	ASSERT_EQUAL(v14[1], 21);
	ASSERT_EQUAL(v14[2], 42);



	NS::vector <int> v;
	assert(v.empty());
	v.push_back(1);
	assert(!v.empty());
	assert(v.size() == 1);
	assert(v[0] == 1);
	v.push_back(2);
	assert(v.size() == 2);
	assert(v[0] == 1);
	assert(v[1] == 2);
	v.push_back(3);
	assert(v.size() == 3);
	assert(v[0] == 1);
	assert(v[1] == 2);
	assert(v[2] == 3);
	v.pop_back();
	assert(v.size() == 2);
	assert(v[0] == 1);
	assert(v[1] == 2);
	v.pop_back();
	assert(v.size() == 1);
	assert(v[0] == 1);
	v.pop_back();
	assert(v.empty());
	assert(v.size() == 0);
	//lexicographical comparaison
	init_vector(v);
	NS::vector <int> v2;
	v2.push_back(1);
	v2.push_back(2);
	v2.push_back(3);
	assert(v == v2);
	assert(v >= v2);
	assert(v <= v2);
	assert(!(v != v2));
	v2.pop_back();
	assert(v != v2);
	v2.push_back(2);
	assert(v > v2);
	assert(v >= v2);
	assert(!(v < v2));
	assert(!(v <= v2));
	assert(v2 < v);
	v2.pop_back();
	v2.push_back(4);
	assert(v < v2);
	assert(v2 > v);
	assert(v <= v2);
	assert(v2 >= v);
	assert(v != v2);
	assert(v2 != v);
	assert(!(v2 == v));
	assert(!(v == v2));
	//max _size test
	assert(v.max_size() == v2.max_size());
	assert(v.max_size() > 0);
	//resize test
	v.resize(10);
	assert(v.size() == 10);
	v.resize(5);
	assert(v.size() == 5);
	v.resize(10);
	assert(v.size() == 10);
	v.resize(0);
	assert(v.size() == 0);
	assert(v.empty());
	//reserve test
	v.reserve(10);
	assert(v.capacity() >= 10);
	v.reserve(5);
	assert(v.capacity() >= 10);
	v.reserve(20);
	assert(v.capacity() >= 20);
	v.reserve(0);
	assert(v.capacity() >= 20);
	v.reserve(1);
	assert(v.capacity() >= 20);
	//assign test
	v.assign(10, 1);
	assert(v.size() == 10);
	assert(v[0] == 1);
	v.assign(10, 2);
	assert(v.size() == 10);
	assert(v[0] == 2);
	v.assign(10, 3);
	assert(v.size() == 10);
	assert(v[0] == 3);
	v.assign(0, 4);
	assert(v.size() == 0);
	assert(v.empty());
	//insert test
	init_vector(v);
	v.insert(v.begin(), 0);
	assert(v.size() == 4);
	assert(v[0] == 0);
	assert(v[1] == 1);
	assert(v[2] == 2);
	assert(v[3] == 3);
	v.insert(v.begin() + 1, 0);
	assert(v.size() == 5);
	assert(v[0] == 0);
	assert(v[1] == 0);
	assert(v[2] == 1);
	assert(v[3] == 2);
	assert(v[4] == 3);
	v.insert(v.begin() + 2, 0);
	assert(v.size() == 6);
	assert(v[0] == 0);
	assert(v[1] == 0);
	assert(v[2] == 0);
	assert(v[3] == 1);
	assert(v[4] == 2);
	assert(v[5] == 3);

	//erase test
	v.erase(v.begin());
	std::cout << v << std::endl;
	assert(v.size() == 5);
	assert(v[0] == 0);
	assert(v[1] == 0);
	assert(v[2] == 1);
	assert(v[3] == 2);
	assert(v[4] == 3);
	v.erase(v.begin() + 1);
	assert(v.size() == 4);
	assert(v[0] == 0);
	assert(v[1] == 1);
	assert(v[2] == 2);
	assert(v[3] == 3);
	v.erase(v.begin() + 2);
	assert(v.size() == 3);
	assert(v[0] == 0);
	assert(v[1] == 1);
	assert(v[2] == 3);
	v.erase(v.begin() + 1);
	assert(v.size() == 2);
	assert(v[0] == 0);
	assert(v[1] == 3);
	v.erase(v.begin() + 1);
	assert(v.size() == 1);
	assert(v[0] == 0);
	v.erase(v.begin());
	assert(v.size() == 0);
	assert(v.empty());
	//clear test
	init_vector(v);
	v.clear();
	v2.clear();
	assert(v.size() == 0);
	assert(v.empty());
	assert(v == v2);
	assert(v >= v2);
	assert(v <= v2);
	assert(!(v != v2));
	//swap test
	init_vector(v);
	v2.push_back(4);
	v2.push_back(5);
	v2.push_back(6);
	v.swap(v2);
	assert(v.size() == 3);
	assert(v[0] == 4);
	assert(v[1] == 5);
	assert(v[2] == 6);
	assert(v2.size() == 3);
	init_vector(v2);
	//move test
	v.clear();
	v2.clear();
	init_vector(v);
	v2.push_back(4);
	v2.push_back(5);
	v2.push_back(6);
	v.swap(v2);
	assert(v.size() == 3);
	assert(v[0] == 4);
	assert(v[1] == 5);
	assert(v[2] == 6);
	assert(v2.size() == 3);
	init_vector(v2);
	//move assign test
	v.clear();
	v2.clear();
	init_vector(v);
	v2.push_back(4);
	v2.push_back(5);
	v2.push_back(6);
	v.swap(v2);
	assert(v.size() == 3);
	assert(v[0] == 4);
	assert(v[1] == 5);
	assert(v[2] == 6);
	assert(v2.size() == 3);
	init_vector(v2);
	//move assign test
	v.clear();
	v2.clear();
	init_vector(v);
	v2.push_back(4);
	v2.push_back(5);
	v2.push_back(6);
	v.swap(v2);
	assert(v.size() == 3);
	assert(v[0] == 4);
	assert(v[1] == 5);
	assert(v[2] == 6);
	assert(v2.size() == 3);
	init_vector(v2);
	//move assign test
	v.clear();
	v2.clear();
	init_vector(v);
	v2.push_back(4);
	v2.push_back(5);
	v2.push_back(6);
	v.swap(v2);
	assert(v.size() == 3);
	assert(v[0] == 4);
	assert(v[1] == 5);
	assert(v[2] == 6);
	assert(v2.size() == 3);
	init_vector(v2);
	// at test
	v.clear();
	init_vector(v);
	assert(v.at(0) == 1);
	//_capacity test
	assert(v.capacity() <= v.max_size());
	// insert test
	v.clear();
	init_vector(v);
	v.insert(v.begin() + 1, 4);
	assert(v.size() == 4);
	assert(v[0] == 1);
	assert(v[1] == 4);
	assert(v[2] == 2);
	assert(v[3] == 3);
	v.insert(v.begin() + 1, 5);
	assert(v.size() == 5);
	assert(v[0] == 1);
	assert(v[1] == 5);
	assert(v[2] == 4);
	assert(v[3] == 2);
	assert(v[4] == 3);
	//end insert test
	v.clear();
	init_vector(v);
	v.insert(v.end(), 4);
	assert(v.size() == 4);
	assert(v[0] == 1);
	assert(v[1] == 2);
	assert(v[2] == 3);
	assert(v[3] == 4);
	v.insert(v.end(), 5);
	assert(v.size() == 5);
	assert(v[0] == 1);
	assert(v[1] == 2);
	assert(v[2] == 3);
	assert(v[3] == 4);
	assert(v[4] == 5);
	//front test
	v.clear();
	init_vector(v);
	assert(v.front() == 1);
	//back test
	v.clear();
	init_vector(v);
	assert(v.back() == 3);
	//assign test
	v.clear();
	init_vector(v);
	v.assign(4, 4);
	assert(v.size() == 4);
	assert(v[0] == 4);
	assert(v[1] == 4);
	assert(v[2] == 4);
	assert(v[3] == 4);
	v.assign(4, 5);
	assert(v.size() == 4);
	assert(v[0] == 5);
	assert(v[1] == 5);
	assert(v[2] == 5);
	assert(v[3] == 5);
// test vector with strings
	NS::vector<std::string> v3;
	std::string hello = "hello";
	std::string world = "world";
	std::string dot = "!";

	v3.push_back(hello);
	std::cout << hello <<std::endl;
	v3.push_back("world");
	v3.push_back("!");
	assert(v3.size() == 3);
	assert(v3[0] == "hello");
	assert(v3[1] == "world");
	assert(v3[2] == "!");
	v3.push_back("hello");
	v3.push_back("world");
	v3.push_back("!");
	assert(v3.size() == 6);
	//compare
	NS::vector<std::string> v4;
	v4.push_back("hello");
	v4.push_back("world");
	v4.push_back("!");
	v4.push_back("hello");
	v4.push_back("world");
	v4.push_back("!");
	assert(v4 == v3);
	assert(!(v4 != v3));
	//comparisons
	assert(v4 <= v3);
	assert(v4 >= v3);
	assert(!(v4 < v3));
	assert(!(v4 > v3));
	//iterators tests
	v4.clear();
	v4.push_back("hello");
	v4.push_back("world");
	v4.push_back("!");
	v4.push_back("hello");
	v4.push_back("world");
	v4.push_back("!");
	NS::vector<std::string>::iterator it = v4.begin();
	assert(*it == "hello");
	++it;
	assert(*it == "world");
	++it;
	assert(*it == "!");
	++it;
	assert(*it == "hello");
	++it;
	assert(*it == "world");
	++it;
	assert(*it == "!");
	++it;
	assert(it == v4.end());
	--it;
	assert(*it == "!");
	--it;
	assert(*it == "world");
	--it;
	assert(*it == "hello");
	--it;
	assert(*it == "!");
	--it;
	assert(*it == "world");
	--it;
	assert(*it == "hello");
	//reverse iterators
	NS::vector<std::string>::reverse_iterator rit = v4.rbegin();
	assert(*rit == "!");
	++rit;
	assert(*rit == "world");
	++rit;
	assert(*rit == "hello");
	++rit;
	assert(*rit == "!");
	++rit;
	assert(*rit == "world");
	++rit;
	assert(*rit == "hello");
	++rit;
	assert(rit == v4.rend());
	--rit;
	assert(*rit == "hello");
	--rit;
	assert(*rit == "world");
	--rit;
	assert(*rit == "!");
	--rit;
	assert(*rit == "hello");
	--rit;
	assert(*rit == "world");
	--rit;
	assert(*rit == "!");
	//lexicographic
	v4.clear();
	v4.push_back("hello");
	v4.push_back("world");
	v4.push_back("!");
	v3.clear();
	v3.push_back("hello");
	v3.push_back("world");
	v3.push_back("!");
	assert(v4 == v3);
	assert(v4 >= v3);
	assert(v3 <= v4);
	assert(v4 <= v3);
	assert(v3 >= v4);
	assert(v4 <= v4);
	assert(v4 >= v4);
	assert(v3 <= v3);
	assert(v3 >= v3);
	v4.push_back("oii");
	assert(v4 > v3);
	assert(v3 < v4);
	assert(v4 != v3);
	//insert with input iterators 
	NS::vector<std::string> v7;
	v7.push_back("hello");
	v7.push_back("world");
	v7.push_back("!");

	NS::vector<std::string> v8;
	//insert with input iterators
	v8.insert(v8.begin(), v7.begin(), v7.end());
	assert(v8.size() == 3);
	assert(v8[0] == "hello");
	assert(v8[1] == "world");
	assert(v8[2] == "!");
	assert(v8 == v7);
	std::cout << "vector insertion with input iter test passed" << std::endl;

	//-------------------------
	//v5 size_t
	NS::vector<size_t> v5;
	for (size_t i = 0; i <= 10000000; i++)
	{
		v5.push_back(i);
	}
	assert(v5.size() == 10000001);
	assert(v5[10000000] == 10000000);
	NS::vector<size_t>::iterator it5 = v5.begin(), ite5 = v5.end();
	assert((ite5 - it5) == 10000001);
	for (; it5 != ite5; ++it5)
			*it5 = (ite5 - it5);


	NS::vector<int> v9;
	v9.reserve(100);
	v9.insert(v9.begin(), int(100000), int(0));
	//std::cout << v9.size() << std::endl;
	assert(v9.size() == 100000);
	//std::cout << v9[100000 - 1] << std::endl;
	assert(v9[100000 - 1] == 0);
	//------------------------
	//same with strings
	NS::vector<std::string> v6;
	for (size_t i = 0; i <= 10000000; i++)
	{
		v6.push_back(ft::to_string(i));
	}
	assert(v6.size() == 10000001);
	assert(v6[10000000] == "10000000");
	NS::vector<std::string>::iterator it6 = v6.begin(), ite6 = v6.end();
	assert((ite6 - it6) == 10000001);
	size_t index = 0;
	for (; it6 != ite6; ++it6)
			assert(*it6 == ft::to_string(index++));

	//string vector erase test
	NS::vector<std::string> v10;
	v10.push_back("hello");
	v10.push_back("world");
	v10.push_back("!");

	v10.erase(v10.begin());
	assert(v10.size() == 2);
	v10.insert(v10.begin(), "hellerkewklrf,wemngfmnegbmegmrbj,grn,bgrkgr,bngtn,");
	assert(v10[0] == "hellerkewklrf,wemngfmnegbmegmrbj,grn,bgrkgr,bngtn,");
	//			void assign(typename ft::enable_if<!std::numeric_limits<InputIt>::is_integer, InputIt>::type first, InputIt last )

	NS::vector<std::string> v11;
	v11.assign(v10.begin(),v10.end());
	assert(v11.size() == v10.size());
	assert(v11 == v10);

	v12 = NS::vector<std::string>(v11);
	assert(v12.size() == v11.size());
	assert(v12 == v11);
	v11 = v12;
	assert(v11.size() == v12.size());
	assert(v11 == v12);
	v12 = v11;
	assert(v12.size() == v11.size());
	assert(v12 == v11);
	v11 = NS::vector<std::string>(v12);
	assert(v11.size() == v12.size());
	assert(v11 == v12);
	v11 =  NS::vector<std::string>(v4);
	v4 = v11;

	std::cout << "starting weird insert tests" << std::endl;
	v4.insert(v4.begin() + v4.size() / 2, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
	std::cout << v4.size() << std::endl;
	std::cout << v4 << std::endl;
	//insert v4 into itself
	v4.insert(v4.begin() + v4.size() / 2, v4.begin(), v4.end());
	std::cout << v4.size() << std::endl;
	std::cout << v4 << std::endl;
	//again
	v4.insert(v4.begin() + v4.size() / 2, v4.begin(), v4.end());
	std::cout << v4.size() << std::endl;
	std::cout << v4 << std::endl;
	//AGAIN
	v4.insert(v4.begin() + v4.size() / 2, v4.begin(), v4.end());
	std::cout << v4.size() << std::endl;
	std::cout << v4 << std::endl;
	// AAAAGAIN
	v4.insert(v4.begin(), v4.begin(), v4.end());
	std::cout << v4.size() << std::endl;
	std::cout << v4 << std::endl;
	v4.insert(v4.begin(), 950, std::string("xDD"));
	std::cout << v4.size() << std::endl;
	std::cout << v4 << std::endl;

	v4.erase(v4.begin() + v4.size() /4, v4.begin() + v4.size() / 2);
	std::cout << v4.size() << std::endl;
	std::cout << v4 << std::endl;
	v4.erase(v4.begin() + v4.size() / 4, v4.begin() + v4.size() / 2);
	std::cout << v4.size() << std::endl;
	std::cout << v4 << std::endl;
	v4.erase(v4.begin() + v4.size() / 4, v4.end());
	std::cout << v4.size() << std::endl;
	std::cout << v4 << std::endl;
	v4.erase(v4.begin(), v4.end());
	std::cout << v4.size() << std::endl;
	std::cout << v4 << std::endl;

	// assign tests
	NS::vector<int> v19;
	int arr19[] = { 4, 3, 2, 7, 9, 2};
	const NS::vector<int>::size_type arr19_size = sizeof(arr19) / sizeof(*arr19);

	v19.assign(arr19, arr19 + arr19_size);

	for (size_t i = 0; i < arr19_size; i++)
	{
		assert(arr19[i] == v19[i]);
	}

	assert(v19.size() == arr19_size);
	v19.assign(arr19_size, 42);
	assert(v19.size() == arr19_size);

	for (size_t i = 0; i < arr19_size; i++)
	{
		assert(v19[i] == 42);
	}

	int arr20[] = { 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3,
	2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2,
	7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7,
	9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7,
	9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7,
	9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2,
	7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3,
	2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4,
	3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2,
	4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9,
	2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7,
	9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2,
	7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3,
	2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2,
	7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7,
	9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9,
	2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2,
	4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4,
	3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3,
	2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2,
	7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7,
	9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9,
	2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2,
	4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4,
	3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3,
	2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7,
	9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2,
	4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3,
	2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7,
	9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2,
	4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3,
	2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7,
	9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2,
	4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3,
	2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7,
	9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2,
	4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3,
	2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7,
	9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2,
	4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3,
	2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7,
	9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2,
	4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3,
	2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7,
	9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2,
	4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3,
	2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7,
	9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2,
	4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3,
	2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7,
	9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2,
	4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2, 4, 3, 2, 7, 9, 2
	};
	const NS::vector<int>::size_type arr20_size = sizeof(arr20) / sizeof(*arr20);
	v19.assign(arr20, arr20 + arr20_size);
	assert(v19.size() == arr20_size);
	for (size_t i = 0; i < arr19_size; i++)
	{
		assert(arr20[i] == v19[i]);
	}

	v19.assign(9999999, 42);
	assert(v19.size() == 9999999);
	for (NS::vector<int>::iterator it = v19.begin(); it != v19.end(); it++)
	{
		assert(*it == 42);
	}

	v19.assign(5, 13);
	assert(v19.size() == 5);

	NS::vector<int> v20;
	int arr21[] = { 21, 18, 15, 42, 42 , 42, 42 };

	v20.assign(arr21, arr21 + 3);

	assert(v20.size() == 3);

	for (size_t i = 0; i < 3; i++)
	{
		assert(v20[i] == arr21[i]);
	}

	v19.assign(v20.begin(), v20.begin() + 3);
	assert(v19.size() == v20.size());
	assert(v19.size() == 3);
	
	for (size_t i = 0; i < 3; i++)
	{
		assert(v19[i] == v20[i]);
	}


	NS::vector<int> v21;

	v21.assign(2, 42);

	v21.reserve(300);

	std::cout << "-- before --\n";
	v21.assign(v20.begin(), v20.begin() + 3);
	
	std::cout << v21.size() << std::endl;
	assert(v21.size() == 3);

	mli_vector();

	std::cout << "vector: Passed assign tests" << std::endl;
	return 0;
}

static int
	test_lexicographic()
{
	NS::vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	v.push_back(5);
	v.push_back(6);
	//lexicographic
	NS::vector<int> v2;
	v2.push_back(1);
	v2.push_back(2);
	v2.push_back(3);
	v2.push_back(4);
	v2.push_back(5);
	v2.push_back(6);
	//use lexicographical_compare
	assert (NS::lexicographical_compare(v.begin(), v.end(), v2.begin(), v2.end()) == false);
	assert (NS::lexicographical_compare(v2.begin(), v2.end(), v.begin(), v.end()) == false);
	v2.push_back(7);
	assert (NS::lexicographical_compare(v.begin(), v.end(), v2.begin(), v2.end()) == true);
	assert (NS::lexicographical_compare(v2.begin(), v2.end(), v.begin(), v.end()) == false);
	v2.pop_back();
	v2.pop_back();
	v2.push_back(8);
	assert (NS::lexicographical_compare(v.begin(), v.end(), v2.begin(), v2.end()) == true);
	assert (NS::lexicographical_compare(v2.begin(), v2.end(), v.begin(), v.end()) == false);
	return 0;
}
#endif

#ifdef MAP_HPP
/*
#undef NS
#define NS ft
*/
static int
	test_map()
{
	NS::map<int, int> m;
	m.insert(NS::make_pair(1, 1));
	m.erase(1);
	m.insert(NS::make_pair(1, 1));
	m.insert(NS::make_pair(2, 2));
	m.insert(NS::make_pair(3, 3));

	assert(m.size() == 3);

	assert(m[1] == 1);
	assert(m[2] == 2);
	assert(m[3] == 3);
	//erase
	std::cout << m << std::endl << "-------------------------" << std::endl;
	m.erase(1);
	std::cout << m << std::endl << "-------------------------" << std::endl;
	assert(m.size() == 2);
	assert(m[2] == 2);
	assert(m[3] == 3);
	//clear
	m.clear();
	//std::cout << m << std::endl << "--------" << std::endl;
	assert(m.size() == 0);
	//insert
	m.insert(NS::pair<int, int>(1, 1));
	m.insert(NS::pair<int, int>(2, 2));
	m.insert(NS::pair<int, int>(3, 3));
	std::cout << m.size() << std::endl;
	assert(m.size() == 3);
	assert(m[1] == 1);
	assert(m[2] == 2);
	assert(m[3] == 3);
	//erase
	m.erase(1);
	assert(m.size() == 2);
	assert(m[2] == 2);
	assert(m[3] == 3);
	//clear
	m.clear();
	assert(m.size() == 0);
	//string keys
	NS::map<std::string, int> m2;
	m2["hello"] = 1;
	m2["world"] = 2;
	m2["!"] = 3;
	//check
	assert(m2.size() == 3);
	assert(m2["hello"] == 1);
	assert(m2["world"] == 2);
	assert(m2["!"] == 3);
	//erase
	std::cout << m2 << std::endl << "-------------------------" << std::endl;
	m2.erase("hello");
	//assert find("hello") fails
	assert(m2.find("hello") == m2.end());
	std::cout << m2 << std::endl << "-------------------------" << std::endl;
	assert(m2.size() == 2);
	assert(m2["world"] == 2);
	assert(m2["!"] == 3);
	//clear
	m2.clear();
	assert(m2.size() == 0);
	//lexicographic
	m2.insert(NS::pair<std::string, int>("hello", 1));
	m2.insert(NS::pair<std::string, int>("world", 2));
	m2.insert(NS::pair<std::string, int>("!", 3));
	assert(m2.size() == 3);
	assert(m2["hello"] == 1);
	assert(m2["world"] == 2);
	assert(m2["!"] == 3);
	//compare
	NS::map<std::string, int> m3;
	m3.insert(NS::pair<std::string, int>("hello", 1));
	m3.insert(NS::pair<std::string, int>("world", 2));
	m3.insert(NS::pair<std::string, int>("!", 3));
	assert(m2 == m3);
	assert(m2 >= m3);
	assert(m3 <= m2);
	assert(m2 <= m3);
	assert(m2 <= m2);
	assert(m2 >= m2);
	assert(m3 >= m3);
	assert(m3 <= m3);
	assert(m3 >= m3);
	m3.insert(NS::pair<std::string, int>("oii", 4));
	//std::cout << "m2 \n" << m2 << std::endl;
	//std::cout << "m3 \n" << m3 << std::endl;
	//std::cout << "------------------------------------------------" << std::endl;
	/*std::cout << "{";
	for (NS::map<std::string, int>::iterator it = m2.begin(); it != m2.end(); it++)
		std::cout << it->first << ":" << it->second << ", ";
	std::cout << "}" << std::endl;
	std::cout << "{";
	for (NS::map<std::string, int>::iterator it = m3.begin(); it != m3.end(); it++)
		std::cout << it->first << ":" << it->second << ", ";
	std::cout << "}" << std::endl;*/
	assert(m3 < m2);
	assert(m2 > m3);
	assert(m3 != m2);
	assert(m2 != m3);
	m3.erase("oii");
	//std::cout << "m2 \n" << m2 << std::endl;
	//std::cout << "m3 \n" << m3 << std::endl;
	std::cout << "before ==" << std::endl;
	assert(m3 == m2);
	assert(!(m3 != m2));
	assert(m2 == m2);
	assert(m3 == m3);
	m3.clear();
	assert(m3 == m3);
	assert(m2 == m2);
	assert(m3 != m2);
	assert(m2 != m3);
	m3.insert(NS::pair<std::string, int>("oii", 4));
	assert(m3 != m2);
	assert(m2 == m2);
	assert(m3 == m3);
	m3.clear();
	m2.clear();
	//erase test
	m2.insert(NS::pair<std::string, int>("hello", 1));
	m2.insert(NS::pair<std::string, int>("world", 2));
	m2.insert(NS::pair<std::string, int>("!", 3));
	m2.erase("hello");
	assert(m2.size() == 2);
	//assert find fails
	assert(m2.find("hello") == m2.end());
	std::cout << "hello can't be found using find()" << std::endl;
	//assert you can't find it with iterator
	for (NS::map<std::string, int>::iterator it = m2.begin(); it != m2.end(); it++)
	{
		if (it->first == "hello")
		{
			std::cout << "hello found using iterator" << std::endl;
			assert(false);
		}
	}
	std::cout << "hello can't be found using iterator" << std::endl;
	assert(m2["world"] == 2);
	assert(m2["!"] == 3);
	m2.clear();
	assert(m2.size() == 0);
	//insert test
	m2.insert(NS::pair<std::string, int>("hello", 1));
	m2.insert(NS::pair<std::string, int>("world", 2));
	m2.insert(NS::pair<std::string, int>("!", 3));
	m2.insert(NS::pair<std::string, int>("hello", 4));
	assert(m2.size() == 3);
	assert(m2["hello"] == 1);
	assert(m2["world"] == 2);
	assert(m2["!"] == 3);
	m2.clear();
	m.clear();
	m3.clear();
	//m4 size_t size_t
	NS::map<size_t, size_t> m4;
	size_t size = 81790;
	for (size_t i = 0; i < size; i++)
	{
		m4.insert(NS::pair<size_t, size_t>(i, i));
	}
	//std::cout << m4 << std::endl << "-------------------------" << std::endl;
	std::cout << "m4" << std::endl;
//	std::cout << m4 << std::endl;
	assert(m4.size() == size);
	std::cout << "m2[" << m4.size() << "] \n" << m4[m4.size() - 1] << std::endl;
	assert(m4[m4.size() - 1] == size - 1);
	assert(m4[(m4.size() - 1)/2] == (size - 1)/2);
	assert(m4.find(m4.size() - 1) != m4.end());
	std::cout << m4.find(m4.size() - 1)->first << std::endl;
	//std::cout << m4.end()->first << std::endl;
	assert(m4.find(m4.size()) == m4.end());

	m4.clear();
	for (size_t i = 0; i < size; i++)
	{
		m4.insert(NS::pair<size_t, size_t>(i^0x4ACBD0E, i^0x4ACBD0E));
	}
	//std::cout << m4 << std::endl << "-------------------------" << std::endl;
	std::cout << "m4" << std::endl;
	//std::cout << m4 << std::endl;
	assert(m4.size() == size);
	assert(m4[(size/2)^0x4ACBD0E] == ((size/2) ^0x4ACBD0E));
//	std::map<std::string, std::string>::value_compare valcp((std::less<std::string>()));
	NS::map<size_t, size_t> m5;
	m5.insert(NS::pair<size_t, size_t>(88, 456));
	m5.insert(NS::pair<size_t, size_t>(99, 789));
	m5.insert(NS::pair<size_t, size_t>(11, 123));
	m5.insert(NS::pair<size_t, size_t>(22, 456));
	m5.insert(NS::pair<size_t, size_t>(33, 789));
	m5.insert(NS::pair<size_t, size_t>(44, 123));
	m5.insert(NS::pair<size_t, size_t>(1, 456));
	m5.insert(NS::pair<size_t, size_t>(2, 789));
	m5.insert(NS::pair<size_t, size_t>(0, 123));
	assert(m5.size() == 9);
	assert(m5[88] == 456);
	assert(m5[99] == 789);
	assert(m5[11] == 123);
	assert(m5[22] == 456);
	assert(m5[33] == 789);
	assert(m5[44] == 123);
	assert(m5[1] == 456);
	assert(m5[2] == 789);
	assert(m5[0] == 123);
	//iterator erase test
	m5.erase(m5.begin(), m5.end());	
	assert(m5.empty());
	//fille map with values
	for (size_t i = 0; i < 10000; i++)
	{
		m5.insert(NS::pair<size_t, size_t>(i^0xABD7, i));
	}
	assert(m5.size() == 10000);
	//remove stuff with iterator
	m5.erase(m5.begin(), m5.end());
	assert(m5.empty());
	//map6
	m5[0] = 0;
	NS::map<size_t, size_t> m6(m5);
	//operator[] test
	m6[1] = 1;
	m6[7] = 2;
	m6[95] = 3;
	m6[12] = 4;
	m6[13] = 5;
	//assert
	assert(m6[0] == 0);
	assert(m6[1] == 1);
	assert(m6[7] == 2);
	assert(m6[95] == 3);
	assert(m6[12] == 4);
	assert(m6[13] == 5);
	//clear
	m6.clear();
	return 0;
}

#endif

static int
	is_integral_test() 
{
	assert(ft::is_integral<int>::value == true);
	assert(ft::is_integral<unsigned int>::value == true);
	assert(ft::is_integral<long>::value == true);
	assert(ft::is_integral<unsigned long>::value == true);
	assert(ft::is_integral<char>::value == true);
	assert(ft::is_integral<unsigned char>::value == true);
	assert(ft::is_integral<signed char>::value == true);
	assert(ft::is_integral<wchar_t>::value == true);
	assert(ft::is_integral<bool>::value == true);
	assert(ft::is_integral<float>::value == false);
	assert(ft::is_integral<double>::value == false);
	assert(ft::is_integral<long double>::value == false);
	assert(ft::is_integral<std::string>::value == false);
	assert(ft::is_integral<NS::vector<int> >::value == false);
	assert((ft::is_integral<std::map<int, int> >::value == false));
	return 0;
}

#ifdef PAIR_HPP

static int
	pair_test()
{
	NS::pair<int, int> p(1, 2);
	assert(p.first == 1);
	assert(p.second == 2);
	return 0;
}

static int
	make_pair_test()
{
	NS::pair<int, int> p = NS::make_pair(1, 2);
	assert(p.first == 1);
	assert(p.second == 2);
	return 0;
}

static int
	swap_test()
{
	//swap vectors
	NS::vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	NS::vector<int> v2;
	v2.push_back(4);
	v2.push_back(5);
	v2.push_back(6);
	NS::swap(v1, v2);
	assert(v1.size() == 3);
	assert(v1[0] == 4);
	assert(v1[1] == 5);
	assert(v1[2] == 6);
	assert(v2.size() == 3);
	assert(v2[0] == 1);
	assert(v2[1] == 2);
	assert(v2[2] == 3);
	return 0;
}
#endif

#ifdef MAP_HPP

//map_order_test
static int map_order_test()
{
	NS::map<int, int> map;

	map.insert(NS::pair<int, int>(10, 1));
	std::cout << map << std::endl << "------------------------------------------------" << std::endl;
	map.insert(NS::pair<int, int>(20, 2));
	std::cout << map << std::endl << "------------------------------------------------" << std::endl;
	map.insert(NS::pair<int, int>(30, 3));
	std::cout << map << std::endl << "------------------------------------------------" << std::endl;
	map.insert(NS::pair<int, int>(4, 40));
	std::cout << map << std::endl << "------------------------------------------------" << std::endl;
	map.insert(NS::pair<int, int>(5, 50));
	std::cout << map << std::endl << "------------------------------------------------" << std::endl;
	map.insert(NS::pair<int, int>(6, 60));
	std::cout << map << std::endl << "------------------------------------------------" << std::endl;
	map.insert(NS::pair<int, int>(7, 70));
	std::cout << map << std::endl << "------------------------------------------------" << std::endl;
	for (size_t i = 0; i < map.size(); i++)
	{
		std::cout << "(" << i << "," << map[i] << ") ";
	}
	std::cout << std::endl;
	std::cout << "------------------------------------------------" << std::endl;
	NS::map<std::string, int> m3;
	std::cout << m3 << std::endl << "------------------------------------------------" << std::endl;
	m3.insert(NS::pair<std::string, int>("hello", 1));
	std::cout << m3 << std::endl << "------------------------------------------------" << std::endl;
	m3.insert(NS::pair<std::string, int>("world", 2));
	std::cout << m3 << std::endl << "------------------------------------------------" << std::endl;
	m3.insert(NS::pair<std::string, int>("!", 3));
	std::cout << m3 << std::endl << "------------------------------------------------" << std::endl;
	m3.insert(NS::pair<std::string, int>("oii", 4));
	std::cout << m3 << std::endl << "------------------------------------------------" << std::endl;

	return 0;
}

static int map_iter_test()
{
	NS::map<int, int> map;
	map.insert(NS::pair<int, int>(10, 1));
	map.insert(NS::pair<int, int>(20, 2));
	map.insert(NS::pair<int, int>(30, 3));
	map.insert(NS::pair<int, int>(4, 40));
	map.insert(NS::pair<int, int>(5, 50));
	map.insert(NS::pair<int, int>(6, 60));
	map.insert(NS::pair<int, int>(7, 70));
	std::cout << map << std::endl << "------------------------------------------------" << std::endl;
	NS::map<int, int>::iterator it = map.begin();
	NS::map<int, int>::iterator ite = map.end();
	while (it != ite)
	{
		std::cout << "(" << it->first << "," << it->second << ") ";
		it++;
	}
	std::cout << std::endl;
	std::cout << "------------------------------------------------" << std::endl;
	//rerverse iterator
	NS::map<int, int>::reverse_iterator rit = map.rbegin();
	NS::map<int, int>::reverse_iterator rite = map.rend();
	while (rit != rite)	
	{
		std::cout << "(" << rit->first << "," << rit->second << ") ";
		rit++;
	}
	std::cout << std::endl;
	std::cout << "------------------------------------------------" << std::endl;
	//const_iterator tests
	NS::map<int, int>::const_iterator cit = map.begin();
	NS::map<int, int>::const_iterator cite = map.end();
	while (cit != cite)
	{
		std::cout << "(" << cit->first << "," << cit->second << ") ";
		cit++;
	}
	std::cout << std::endl;
	std::cout << "passed const_iterator" << std::endl;
	//lower_bound test
	NS::map<int, int>::iterator it2 = map.lower_bound(10);
	//assert
	assert(it2->first == 10);
	assert(it2->second == 1);
	std::cout << "passed lower_bound" << std::endl;
	//upper_bound test
	NS::map<int, int>::iterator it3 = map.upper_bound(10);
	//assert
	assert(it3->first == 20);
	assert(it3->second == 2);
	std::cout << "passed upper_bound" << std::endl;
	//equal_range test
	NS::pair<NS::map<int, int>::iterator, NS::map<int, int>::iterator> p = map.equal_range(10);
	//assert
	assert(p.first->first == 10);
	assert(p.first->second == 1);
	//incremant
	p.first++;
	assert(p.first->first == 20);
	assert(p.first->second == 2);
	assert(p.second->first == p.first->first);
	assert(p.second->second == p.first->second);
	std::cout << "passed equal_range" << std::endl;

	//reverse_iterator
	NS::map<int, int>::reverse_iterator rit2 = map.rbegin();
	NS::map<int, int>::reverse_iterator rite2 = map.rend();
	while (rit2 != rite2)
	{
		std::cout << "(" << rit2->first << "," << rit2->second << ") ";
		rit2++;
	}
	std::cout << std::endl;
	rit2 = map.rbegin();
	rite2 = map.rend();
	while (rit2 != rite2)
	{
		std::cout << "(" << rit2->first << "," << rit2->second << ") ";
		++rit2;
	}
	std::cout << std::endl;
	std::cout << "reverse_iterator passed" << std::endl;

	//test upper bound again
	NS::map<int, int>::iterator it4 = map.upper_bound(10);
	//assert
	assert(it4->first == 20);
	assert(it4->second == 2);
	//test lower bound again
	NS::map<int, int>::iterator it5 = map.lower_bound(10);
	//assert
	assert(it5->first == 10);
	assert(it5->second == 1);
	//clear
	map.clear();
	//test lower bound again
	NS::map<int, int>::iterator it6 = map.lower_bound(10);
	//assert
	assert(it6 == map.end());
	//test upper bound again
	NS::map<int, int>::iterator it7 = map.upper_bound(10);
	//assert
	assert(it7 == map.end());
	//fill map with stuff
	map.insert(NS::pair<int, int>(10, 1));
	map.insert(NS::pair<int, int>(20, 2));
	map.insert(NS::pair<int, int>(30, 3));
	map.insert(NS::pair<int, int>(4, 40));
	map.insert(NS::pair<int, int>(5, 50));
	map.insert(NS::pair<int, int>(6, 60));
	map.insert(NS::pair<int, int>(-50, 10));
	map.insert(NS::pair<int, int>(-40, 20));
	map.insert(NS::pair<int, int>(-30, 30));
	map.insert(NS::pair<int, int>(25, 40));
	//test lower bound again
	NS::map<int, int>::iterator it8 = map.lower_bound(10);
	//assert
	assert(it8->first == 10);
	assert(it8->second == 1);
	//test upper bound again
	NS::map<int, int>::iterator it9 = map.upper_bound(10);
	//assert
	assert(it9->first == 20);
	assert(it9->second == 2);
	//test equal range again
	NS::pair<NS::map<int, int>::iterator, NS::map<int, int>::iterator> p2 = map.equal_range(10);
	//assert
	assert(p2.first->first == 10);
	assert(p2.first->second == 1);
	//test copy constructor
	NS::map<int, int> map2(map);
	//assert
	assert(map2.size() == map.size());
	//test lower bound again
	NS::map<int, int>::iterator it10 = map2.lower_bound(10);
	//assert
	assert(it10->first == 10);
	assert(it10->second == 1);
	//test upper bound again
	NS::map<int, int>::iterator it11 = map2.upper_bound(10);
	//assert
	assert(it11->first == 20);
	assert(it11->second == 2);
	//test equal range again
	NS::pair<NS::map<int, int>::iterator, NS::map<int, int>::iterator> p3 = map2.equal_range(10);
	//assert
	assert(p3.first->first == 10);
	assert(p3.first->second == 1);
	//test swap

	const NS::map<int, int>::size_type old_size1 = map.size();
	const NS::map<int, int>::size_type old_size2 = map2.size();
	map2.swap(map);
	
	//assert
	assert(map2.size() == old_size1);
	assert(map.size() == old_size2);
	//test lower bound again
	NS::map<int, int>::iterator it12 = map.lower_bound(10);
	//assert
	assert(it12->first == 10);
	assert(it12->second == 1);
	//test upper bound again
	NS::map<int, int>::iterator it13 = map.upper_bound(10);
	//assert
	assert(it13->first == 20);
	assert(it13->second == 2);
	//test equal range again
	NS::pair<NS::map<int, int>::iterator, NS::map<int, int>::iterator> p4 = map.equal_range(10);
	//assert
	assert(p4.first->first == 10);
	assert(p4.first->second == 1);
	//test assignment operator
	map2 = map;
	//assert
	assert(map2.size() == map.size());
	//test lower bound again
	NS::map<int, int>::iterator it14 = map2.lower_bound(10);
	//assert
	assert(it14->first == 10);
	assert(it14->second == 1);
	//test upper bound again
	NS::map<int, int>::iterator it15 = map2.upper_bound(10);
	//assert
	assert(it15->first == 20);
	assert(it15->second == 2);
	//test equal range again
	NS::pair<NS::map<int, int>::iterator, NS::map<int, int>::iterator> p5 = map2.equal_range(10);
	//assert
	assert(p5.first->first == 10);
	assert(p5.first->second == 1);
	//test upper bound on uninizialied map
	NS::map<int, int> map3;
	//test lower bound on uninizialied map
	assert(map3.lower_bound(10) == map3.end());
	//test upper bound on uninizialied map
	assert(map3.upper_bound(10) == map3.end());
	//test equal range on uninizialied map
	assert(map3.equal_range(10).first == map3.end());
	//test swap on uninizialied map
	map3.swap(map);
	//test std::swap
	std::swap(map, map2);
	map.clear();
	//iter trough map2
	NS::map<int, int>::iterator it16 = map2.begin();
	while (it16 != map2.end())
	{
		std::cout << "(" << it16->first << "," << it16->second << ") ";
		++it16;
	}
	return 0;
}
#endif

#define ENTROPY "2727"

#endif
