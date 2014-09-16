#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <cmath>
#include <iomanip>

#ifndef USE_BOOST_FOR_TESTING
	#define USE_BOOST_FOR_TESTING USE_BOOST
#endif

#if USE_BOOST_FOR_TESTING
#include <boost/multiprecision/cpp_int.hpp>
#endif


#if USE_BOOST_FOR_TESTING
	typedef boost::multiprecision::checked_int1024_t t_safenumber;
#else
	typedef long double t_safenumber; // that should work too but the one with boost is nicer
	// e.g. do not throw warnings from the TESTING CODE (but that can be fixed)
#endif


namespace nCppSafe {

/***
Following operations must be fixed to
- not cause a warning
- and to be safe e.g. if the integral type has negative value.

signed long a; size_t b;
if ( a < b ) ... // error

if ( (xsize_t)a < b ) // ok
if ( (xsize_t)a <= b ) // ok
if ( (xsize_t)a > b ) // ok
if ( (xsize_t)a >= b ) // ok

if ( b < a(xsize_t) ) // ok
if ( b <= a(xsize_t) ) // ok
if ( b > a(xsize_t) ) // ok
if ( b >= a(xsize_t) ) // ok
*/

/***
@description This is a speciall struct that is suppose to:
- allow comparing integrals that can be negative with a size_t
- also possibly do a (partial) check is the value used makes sense to be used in size_t context

The full version of size_t but with sign.
Use this ONLY for casting for ONLY comparsions.
This type is allowed to give you garbaged value in other cases, e.g. if you would like to store large negative
number here, then it only guarantees to be able to confirm that will be smaller then your size_t (e.g. we could store here value like -1)
*/

#define ATTR // TODO attribute operator?
struct xsize_t_struct {
	private:
		size_t m_value; // the value (without size) minus zero is not allowed.
		bool m_minus; // 0=plus.  if value==0 then m_minus=0 always!

	public:
		xsize_t_struct() { }
		xsize_t_struct(size_t t) { m_minus=0; m_value=t; }

		void my_abort(const std::string & msg) {
			std::cout << "size_t arithmetics error in " << __FUNCTION__ << " " << __FILE__<<" : " << msg << std::endl;
			abort();
		}

		// TODO also for signed long int
		xsize_t_struct(signed long long int v) {
			if (v<0) {
				// we are not guaranteeing to store other values then needed for comparsing, so no need to check if -v is stored correctly
				m_minus = 1;
				m_value = 1; // -v  we just store it as representing "-1". we avoid "-v" in case if that could wrap around,
				// and result in minus=1,value=0 which would represent "-0" and that is not allowed (because other code
				// is not prepared to handle that correctly when comparing "-0" <= 0.
			}
			else {
				m_minus = 0;
				const size_t limit_max = std::numeric_limits<size_t>::max();
				unsigned long long int vu = v;
				if (vu > limit_max) {
					my_abort(std::string("Overflow: value of v=") + std::to_string(v) + std::string(" is too big to be placed in signed-size_t, "
						"you are probably doing something wrong if you were about to compare it to size_t then"));
				}
				m_value = v;
			}
		}

		template <class T_size_t>
		ATTR bool operator<=(T_size_t b) const  { if (m_minus) return 1; return m_value <= b; }
		template <class T_size_t>
		ATTR bool operator<(T_size_t b) const   { if (m_minus) return 1; return m_value < b; }
		template <class T_size_t>
		ATTR bool operator>=(T_size_t b) const  { if (m_minus) return 0; return m_value >= b; }
		template <class T_size_t>
		ATTR bool operator>(T_size_t b) const   { if (m_minus) return 0; return m_value > b; }
		template <class T_size_t>
		ATTR bool operator==(T_size_t b) const  { if (m_minus) return 0; return m_value == b; }
		template <class T_size_t>
		ATTR bool operator!=(T_size_t b) const  { if (m_minus) return 1; return m_value != b; }

		template <class T_size_t>
		friend ATTR bool operator<=(T_size_t b, xsize_t_struct a);
		template <class T_size_t>
		friend ATTR bool operator<(T_size_t b, xsize_t_struct a);
		template <class T_size_t>
		friend ATTR bool operator>=(T_size_t b, xsize_t_struct a);
		template <class T_size_t>
		friend ATTR bool operator>(T_size_t b, xsize_t_struct a);
		template <class T_size_t>
		friend ATTR bool operator==(T_size_t b, xsize_t_struct a);
		template <class T_size_t>
		friend ATTR bool operator!=(T_size_t b, xsize_t_struct a);

		private:
		template <class T_size_t>
		ATTR operator T_size_t() {
			// to catch any errors
			my_abort( std::string("Trying to convert the special signed-T_size_t object back to use it by value; "
				"That is not allowed, it shoul be used ONLY FOR COMPARSION with T_size_t."
				" value=")+std::to_string(m_value)+std::string(" back to T_size_t!"));
			//if (m_minus) my_abort( std::string("Trying to convert negative value=")+std::to_string(m_value)+std::string(" back to T_size_t!"));
			return 0; // m_value;
		}


};
#undef ATTR

#define ATTR // TODO attribute operator?
template <class T_size_t>
ATTR bool operator<=(T_size_t b, xsize_t_struct a)  { if (a.m_minus) return 0; return b <= a.m_value; }
template <class T_size_t>
ATTR bool operator<(T_size_t b, xsize_t_struct a)   { if (a.m_minus) return 0; return b < a.m_value; }
template <class T_size_t>
ATTR bool operator>=(T_size_t b, xsize_t_struct a)  { if (a.m_minus) return 1; return b >= a.m_value; }
template <class T_size_t>
ATTR bool operator>(T_size_t b, xsize_t_struct a)   { if (a.m_minus) return 1; return b > a.m_value; }
template <class T_size_t>
ATTR bool operator==(T_size_t b, xsize_t_struct a)  { if (a.m_minus) return 0; return b == a.m_value; }
template <class T_size_t>
ATTR bool operator!=(T_size_t b, xsize_t_struct a)  { if (a.m_minus) return 1; return b != a.m_value; }
#undef ATTR

typedef xsize_t_struct xsize_t; // TODO sfinae




} // nCppSafe

