#ifndef _FT_UTILS_HPP
# define _FT_UTILS_HPP

#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <new>
#include <typeinfo>
#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>

namespace ft {

// https://code.woboq.org/gcc/libstdc++-v3/src/c++11/functexcept.cc.html
// Exception helpers
void
  __throw_bad_exception()
  { throw (std::bad_exception()); }
  void
  __throw_bad_alloc()
  { throw (std::bad_alloc()); }
  void
  __throw_bad_cast()
  { throw (std::bad_cast()); }
  void
  __throw_bad_typeid()
  { throw (std::bad_typeid()); }
  void
  __throw_logic_error(const char* __s __attribute__((unused)))
  { throw (std::logic_error(__s)); }
  void
  __throw_domain_error(const char* __s __attribute__((unused)))
  { throw (std::domain_error(__s)); }
  void
  __throw_invalid_argument(const char* __s __attribute__((unused)))
  { throw (std::invalid_argument(__s)); }
  void
  __throw_length_error(const char* __s __attribute__((unused)))
  { throw (std::length_error(__s)); }
  void
  __throw_out_of_range(const char* __s __attribute__((unused)))
  { throw (std::out_of_range(__s)); }
  void
  __throw_out_of_range_fmt(const char* __fmt, ...)
  {
    const size_t __len = __builtin_strlen(__fmt);
    // We expect at most 2 numbers, and 1 short string. The additional
    // 512 bytes should provide more than enough space for expansion.
    const size_t __alloca_size = __len + 512;
    char *const __s = static_cast<char*>(__builtin_alloca(__alloca_size));
    va_list __ap;
    va_start(__ap, __fmt);
    snprintf(__s, __alloca_size, __fmt, __ap);
    throw (std::out_of_range(__s));
    va_end(__ap);  // Not reached.
  }
  void
  __throw_runtime_error(const char* __s __attribute__((unused)))
  { throw (std::runtime_error(__s)); }
  void
  __throw_range_error(const char* __s __attribute__((unused)))
  { throw (std::range_error(__s)); }
  void
  __throw_overflow_error(const char* __s __attribute__((unused)))
  { throw (std::overflow_error(__s)); }
  void
  __throw_underflow_error(const char* __s __attribute__((unused)))
  { throw (std::underflow_error(__s)); }


template<class T, T v>
struct integral_constant {
    const static T value = v;
    typedef T value_type;
    typedef integral_constant type; // using injected-class-name
    operator value_type() const { return value; }
    // constexpr value_type operator()() const noexcept { return value; } // since c++14
};

/// The type used as a compile-time boolean with true value.
typedef integral_constant<bool, true>     true_type;

/// The type used as a compile-time boolean with false value.
typedef integral_constant<bool, false>    false_type;

// Type traits helpers from STL
#define _FT_DEFINE_SPEC_0_HELPER                          \
  template<>

#define _FT_DEFINE_SPEC_1_HELPER                          \
  template<typename _Tp>

#define _FT_DEFINE_SPEC_2_HELPER                          \
  template<typename _Tp, typename _Cp>

#define _FT_DEFINE_SPEC(_Order, _Trait, _Type, _Value)    \
  _FT_DEFINE_SPEC_##_Order##_HELPER                       \
    struct _Trait<_Type>                               \
    : public integral_constant<bool, _Value> { };

template<typename>
    struct __is_integral_helper
    : public false_type { };

  _FT_DEFINE_SPEC(0, __is_integral_helper, bool, true)
  _FT_DEFINE_SPEC(0, __is_integral_helper, char, true)
  _FT_DEFINE_SPEC(0, __is_integral_helper, signed char, true)
  _FT_DEFINE_SPEC(0, __is_integral_helper, unsigned char, true)
#ifdef _GLIBCXX_USE_WCHAR_T
  _FT_DEFINE_SPEC(0, __is_integral_helper, wchar_t, true)
#endif
  _FT_DEFINE_SPEC(0, __is_integral_helper, short, true)
  _FT_DEFINE_SPEC(0, __is_integral_helper, unsigned short, true)
  _FT_DEFINE_SPEC(0, __is_integral_helper, int, true)
  _FT_DEFINE_SPEC(0, __is_integral_helper, unsigned int, true)
  _FT_DEFINE_SPEC(0, __is_integral_helper, long, true)
  _FT_DEFINE_SPEC(0, __is_integral_helper, unsigned long, true)
  _FT_DEFINE_SPEC(0, __is_integral_helper, long long, true)
  _FT_DEFINE_SPEC(0, __is_integral_helper, unsigned long long, true)

/// remove_const
template<typename _Tp>
struct remove_const
{ typedef _Tp     type; };

template<typename _Tp>
struct remove_const<_Tp const>
{ typedef _Tp     type; };

/// remove_volatile
template<typename _Tp>
struct remove_volatile
{ typedef _Tp     type; };

template<typename _Tp>
struct remove_volatile<_Tp volatile>
{ typedef _Tp     type; };

/// remove_cv
template<typename _Tp>
struct remove_cv
{
	typedef typename
	remove_const<typename remove_volatile<_Tp>::type>::type     type;
};


/*	What i would like to do, but I can't because some f**ker thougth it was a good idea to
	make us reimplment C++11 in f*****g C++98
	https://stackoverflow.com/questions/10747810/what-is-the-difference-between-typedef-and-using-in-c11#:~:text=A%20typedef%2Dname%20can%20also,introduced%20by%20the%20typedef%20specifier.
*/
// // __remove_cv_t (std::remove_cv_t for C++11).
// template<typename _Tp>
// using __remove_cv_t = typename remove_cv<_Tp>::type;

/// is_integral
template<typename _Tp>
struct is_integral
: public __is_integral_helper<typename remove_cv<_Tp>::type>::type
{ };

/* s**** used by STL for some reason */
// # ifndef __ft_builtin_addressof
// #  define __ft_builtin_addressof(obj) (&const_cast<char&>(reinterpret_cast<const volatile char&>(obj)))
// # endif


/* why do they not just do that */
# ifndef __ft_builtin_addressof
#  define __ft_builtin_addressof(obj) &obj
# endif

template<typename _Tp>
inline  _Tp*	__addressof(_Tp& __r) {
	return __ft_builtin_addressof(__r);
}

/**
 * Destroy the object pointed to by a pointer type.
 */
template<typename _Tp>
inline void _Destroy(_Tp* __pointer)
{
	__pointer->~_Tp();
}

template<typename _ForwardIterator, typename _Allocator>
void	__destroy(_ForwardIterator __first, _ForwardIterator __last, _Allocator& __alloc)
{
	for (; __first != __last; ++__first)
  	__alloc.destroy(ft::__addressof(*__first));
}

//unitialized copy with alloc arg
template<typename _InputIterator, typename _ForwardIterator,
    typename _Allocator>
_ForwardIterator __uninitialized_copy_a(_InputIterator __first, _InputIterator __last,
  _ForwardIterator __result, _Allocator& __alloc)
{
  _ForwardIterator __cur = __result;
  try
  {
    for (; __first != __last; ++__first, (void)++__cur)
    __alloc.construct(ft::__addressof(*__cur), *__first);
    return __cur;
  }
  catch(...)
  {
    __destroy(__result, __cur, __alloc);
    throw;
  }
}

//unitialized fill with allocator and n bytes
template<typename _ForwardIterator, typename _Size, typename _Tp, typename _Allocator>
_ForwardIterator __uninitialized_fill_n_a(_ForwardIterator __first, _Size __n,
  const _Tp& __x, _Allocator& __alloc) {
  _ForwardIterator __cur = __first;
  try
  {
    for (; __n > 0; --__n, (void) ++__cur)
      __alloc.construct(ft::__addressof(*__cur), __x);
    return __cur;
  }
  catch(...)
  {
    std::_Destroy(__first, __cur, __alloc);
    throw;
  }
}

/* ITERATOR TRAITS */
// https://stackoverflow.com/questions/6742008/what-are-the-typical-use-cases-of-an-iterator-trait

//  struct for "real iterators" a.k.a. not pointer
//  Types are forwarded from the classes definitions
//  iterator_traits provides an uniform interface to use pointers
//    and user-defined iterators in the same way 
template <class T>
struct iterator_traits {
  typedef typename T::value_type value_type;
  typedef typename T::difference_type difference_type;
  typedef typename T::iterator_category iterator_category;
  typedef typename T::pointer pointer;
  typedef typename T::reference reference;
};

// pointer specialization
template <typename T>
struct iterator_traits<T*>
{
  typedef std::random_access_iterator_tag iterator_category;
  typedef T                               value_type;
  typedef T*                              pointer;
  typedef T&                              reference;
  typedef std::ptrdiff_t                  difference_type;
};

/* REVERSE ITERATOR */
// https://en.cppreference.com/w/cpp/iterator/reverse_iterator
template<class _Iterator>
class reverse_iterator {
  public:
    typedef _Iterator iterator_type;
    typedef ft::iterator_traits<_Iterator>::iterator_category iterator_category;
    typedef ft::iterator_traits<_Iterator>::value_type value_type;
    typedef ft::iterator_traits<_Iterator>::difference_type difference_type;
    typedef ft::iterator_traits<_Iterator>::pointer pointer;
    typedef ft::iterator_traits<_Iterator>::reference reference;
  protected:
    _Iterator current;

  private:

    // reverse_iterator must work uniformely with iterators and pointers
    // so we have to run different code to ensure compatibilty
    // this is used for operator->()

    // When the given type is a pointer, return the pointer
    template<typename _Tp>
    static _Tp* _S_to_pointer(_Tp* __p) const { return __p; }

    // When the given type is an iterator, return the operator->()
    template <typename _Tp>
    static _Tp* _S_to_pointer(_Tp __t) { return __t.operator->(); }

  public:
    reverse_iterator() : current() { }

    explicit reverse_iterator(const reverse_iterator & __x) : current(__x.current) {}

    template <typename _Iter>
    reverse_iterator(const reverse_iterator<_Iter>& __x) : current(__x.base()) { }

    iterator_type base() const { return current; }

    // https://en.cppreference.com/w/cpp/iterator/reverse_iterator/operator*
    // Returns a reference to the element previous to current.
    reference operator*() const {
      _Iterator __tmp = current;
      return *--__tmp;
    }

    // Returns a pointer to the element previous to current.
    pointer operator->() const {
        _Iterator __tmp = current;
        --__tmp;
        return _S_to_pointer(__tmp);
     }

    // increment / decrement operators
    // https://docs.microsoft.com/en-us/cpp/cpp/increment-and-decrement-operator-overloading-cpp?view=msvc-170

    // prefix increment operator
    reverse_iterator& operator++() {
      --current;
      return *this;
    }

    // postfix increment operator
    reverse_iterator operator++(int) {
      reverse_iterator __tmp = *this;
      --current;
      return __tmp;
    }

    // prefix decrement operator
    reverse_iterator operator--() { 
      ++current;
      return *this;
    }

    // postfix decrement operator
    reverse_iterator operator--(int) {
      reverse_iterator __tmp = *this;
      ++current;
      return *this;
    }

  // The underlying iterator must be a Random Access Iterator.
  reverse_iterator operator+(difference_type __n) const {
    return reverse_iterator(current - __n);
  }

  // The underlying iterator must be a Random Access Iterator.
  reverse_iterator& operator+=(difference_type __n) {
    current -= __n;
    return *this;
  }
  
  // The underlying iterator must be a Random Access Iterator.
  reverse_iterator operator-(difference_type __n) const {
    return reverse_iterator(current + __n);
  }

  // The underlying iterator must be a Random Access Iterator.
  reverse_iterator& operator-=(difference_type __n) {
    current += __n;
    return *this;
  }

  // The underlying iterator must be a Random Access Iterator.
  reference operator[](difference_type __n) const {
    return (*this)[__n];
  }

  template<typename _Iterator>
  inline bool operator==(const reverse_iterator<_Iterator>& __x,
        const reverse_iterator<_Iterator>& __y)
  { return __x.base() == __y.base(); }

  template<typename _Iterator>
  inline bool operator<(const reverse_iterator<_Iterator>& __x,
        const reverse_iterator<_Iterator>& __y)
    { return __y.base() < __x.base(); }

  template<typename _Iterator>
  inline bool operator!=(const reverse_iterator<_Iterator>& __x,
          const reverse_iterator<_Iterator>& __y)
    { return !(__x == __y); }

  template<typename _Iterator>
  inline bool operator>(const reverse_iterator<_Iterator>& __x,
        const reverse_iterator<_Iterator>& __y)
    { return __y < __x; }

  template<typename _Iterator>
  inline bool operator<=(const reverse_iterator<_Iterator>& __x,
          const reverse_iterator<_Iterator>& __y)
    { return !(__y < __x); }

  template<typename _Iterator>
  inline bool operator>=(const reverse_iterator<_Iterator>& __x,
          const reverse_iterator<_Iterator>& __y)
    { return !(__x < __y); }

  // Comparison of reverse_iterator to const reverse_iterator.
  template<typename _IteratorL, typename _IteratorR>
  inline bool operator==(const reverse_iterator<_IteratorL>& __x,
          const reverse_iterator<_IteratorR>& __y)
    { return __x.base() == __y.base(); }

  template<typename _IteratorL, typename _IteratorR>
  inline bool operator<(const reverse_iterator<_IteratorL>& __x,
        const reverse_iterator<_IteratorR>& __y)
    { return __y.base() < __x.base(); }

  template<typename _IteratorL, typename _IteratorR>
  inline bool operator!=(const reverse_iterator<_IteratorL>& __x,
          const reverse_iterator<_IteratorR>& __y)
    { return !(__x == __y); }

  template<typename _IteratorL, typename _IteratorR>
  inline bool operator>(const reverse_iterator<_IteratorL>& __x,
        const reverse_iterator<_IteratorR>& __y)
    { return __y < __x; }

  template<typename _IteratorL, typename _IteratorR>
  inline bool operator<=(const reverse_iterator<_IteratorL>& __x,
          const reverse_iterator<_IteratorR>& __y)
    { return !(__y < __x); }

  template<typename _IteratorL, typename _IteratorR>
  inline bool operator>=(const reverse_iterator<_IteratorL>& __x,
          const reverse_iterator<_IteratorR>& __y)
    { return !(__x < __y); }

  template<typename _Iterator>
    inline typename reverse_iterator<_Iterator>::difference_type
    operator-(const reverse_iterator<_Iterator>& __x,
	      const reverse_iterator<_Iterator>& __y)
    { return __y.base() - __x.base(); }

  template<typename _IteratorL, typename _IteratorR>
    inline typename reverse_iterator<_IteratorL>::difference_type
    operator-(const reverse_iterator<_IteratorL>& __x,
	      const reverse_iterator<_IteratorR>& __y)
    { return __y.base() - __x.base(); }

  template<typename _Iterator>
    inline reverse_iterator<_Iterator>
    operator+(typename reverse_iterator<_Iterator>::difference_type __n,
	      const reverse_iterator<_Iterator>& __x)
    { return reverse_iterator<_Iterator>(__x.base() - __n); }

  template<typename _Iterator>
  reverse_iterator& operator=(const reverse_iterator<_Iterator>& other) {
      if (&other != this) {
        this->current = other.base();
      }
      return *this;
  }

};

/* ENABLE_IF */
// https://en.cppreference.com/w/cpp/types/enable_if
// it is c++11 but annoying s*** subject wants you to do it anyway
// https://stackoverflow.com/questions/61557539/how-can-i-use-my-custom-enable-if-in-c98
template<bool B, class T = void>
struct enable_if {};
 
template<class T>
struct enable_if<true, T> { typedef T type; };

/* LEXICOGRAPHICAL COMPARE */
// https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare
template<class InputIt1, class InputIt2, class Compare>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                             InputIt2 first2, InputIt2 last2,
                             Compare comp)
{
    for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 ) {
        if (comp(*first1, *first2)) return true;
        if (comp(*first2, *first1)) return false;
    }
    return (first1 == last1) && (first2 != last2);
}

}

#endif