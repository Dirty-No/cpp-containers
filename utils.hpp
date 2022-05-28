#ifndef _FT_UTILS_HPP
# define _FT_UTILS_HPP

#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <new>
#include <typeinfo>
#include <stdarg.h>
#include <stdio.h>

namespace ft {

// https://code.woboq.org/gcc/libstdc++-v3/src/c++11/functexcept.cc.html
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


/* why do not they just do that */
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


}

#endif