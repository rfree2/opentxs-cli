#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <cmath>
#include <iomanip>

#define ALLOWED_BOOST 1

#ifdef ALLOWED_BOOST
//#include <boost/multiprecision/cpp_int.hpp>
#endif


#ifdef ALLOWED_BOOST

//typedef boost::multiprecision::checked_int1024_t t_safenumber;

// typedef boost::multiprecision::number<cpp_int_backend<16384, 16384, signed_magnitude, checked, void> >    t_safenumber;

#else

//typedef long long double t_safenumber;

#endif
namespace nCppSafe {
/***

size_t size;
int b;
if ( (opsize_t)size < b ) // ok

*/


/***
@struct
@brief you cast an size_t to this type, and then resulting type is comparable to all build-in integral types without warning nor overflows.

This is a speciall struct that is suppose to:
- allow comparing integrals that can be negative

Use this ONLY for casting for ONLY comparsions.
*/

#define ATTR __attribute__((pure)) // TODO attribute operator?
struct xsize_t_struct {
	public: /* public to not mock around with friending templated operators */
		size_t m_v; // the value of size_t

	public:
		explicit xsize_t_struct(size_t size) : m_v(size) {} // only proper way to init this
		template <class TYPE> xsize_t_struct(TYPE integral)=delete; // this is blocked
		xsize_t_struct()=delete; // this is blocked too

		// TEST CASE it -2,-1,0,+1,+2  N * N tests
};

//template <class T_INT> bool operator< (T_INT value, xsize_t_struct size)   { std::cerr<<" {value="<<value<<"<"<<size.m_v<<" 1 } "; if (value<0) return 1; return value < size.m_v; }
//template <class T_INT> bool operator< (xsize_t_struct size, T_INT value)   { std::cerr<<" {xsize="<<size.m_v<<" < value="<<value<<" 2 } "; if (value<0) return 0; return size.m_v < value; }

template <class T_INT> bool operator< (T_INT value, xsize_t_struct size)   { if (value<0) return 1; return static_cast<size_t>(value) < size.m_v; }
template <class T_INT> bool operator< (xsize_t_struct size, T_INT value)   { if (value<0) return 0; return size.m_v < static_cast<size_t>(value); }

template <class T_INT> bool operator<= (T_INT value, xsize_t_struct size)  { if (value<0) return 1; return static_cast<size_t>(value) <= size.m_v; } // -100 <= 0
template <class T_INT> bool operator<= (xsize_t_struct size, T_INT value)  { if (value<0) return 0; return size.m_v <= static_cast<size_t>(value); } // 0 <= -100

template <class T_INT> bool operator> (T_INT value, xsize_t_struct size)   { if (value<0) return 0; return static_cast<size_t>(value) > size.m_v; }
template <class T_INT> bool operator> (xsize_t_struct size, T_INT value)   { if (value<0) return 1; return size.m_v > static_cast<size_t>(value); }

template <class T_INT> bool operator>= (T_INT value, xsize_t_struct size)  { if (value<0) return 0; return static_cast<size_t>(value) >= size.m_v; }
template <class T_INT> bool operator>= (xsize_t_struct size, T_INT value)  { if (value<0) return 1; return size.m_v >= static_cast<size_t>(value); }

template <class T_INT> bool operator!= (T_INT value, xsize_t_struct size)   { if (value<0) return 1; return static_cast<size_t>(value) != size.m_v; }
template <class T_INT> bool operator!= (xsize_t_struct size, T_INT value)   { if (value<0) return 1; return static_cast<size_t>(value) != size.m_v; }

template <class T_INT> bool operator== (T_INT value, xsize_t_struct size)   { if (value<0) return 0; return static_cast<size_t>(value) == size.m_v; }
template <class T_INT> bool operator== (xsize_t_struct size, T_INT value)   { if (value<0) return 0; return static_cast<size_t>(value) == size.m_v; }
// @TODO all op

#undef ATTR



typedef xsize_t_struct xsize_t; // TODO sfinae



}
 // nCppSafe

