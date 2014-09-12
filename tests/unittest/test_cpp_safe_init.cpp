#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <cmath>
#include <iomanip>

#include <test_cpp_safe_init.hpp>

namespace nUnitTest {

template <class T>
class fake_vector {
	public:
		size_t m_size;
		fake_vector() : m_size(0) { }
		void resize(size_t new_size) { m_size = new_size; }
		size_t size() const { return m_size; }
};

template <class T_VECTOR>
bool test_vector_size(long long int AI, size_t BV) {
	using namespace std;
	std::cout << setw(20) << AI << " vs vector size BV="<< setw(20) << BV <<"... " << std::flush;
	T_VECTOR tab;
	tab.resize(BV);

	bool all_ok=1;

	#define COMPARE(OPERATOR, OPERATOR_NAME) \
	do { \
		bool c1 = ( ((xsize_t_struct)AI)  OPERATOR  (tab.size()) ); \
		bool c2 = ( (long double)AI)      OPERATOR  ((long double)BV); \
		bool OK = (c1==c2); \
		if (OK) cout << "ok" << OPERATOR_NAME << " "; \
		else { cout <<"\nFAILED AI="<<AI<<"  " << "(" << OPERATOR_NAME << ")" << "  " << "BV="<<BV<<" c1="<<c1<<" c2(double)="<<c2<<"! "; } \
		all_ok = all_ok && OK; \
	} while(0)
	COMPARE( < , "<" );
	COMPARE( <= , "<=" );
	COMPARE( > , ">" );
	COMPARE( > , ">=" );
	COMPARE( == , "==" );
	COMPARE( != , "!=" );
	#undef COMPARE

	#define COMPARE(OPERATOR, OPERATOR_NAME) \
	do { \
		bool c1 = ( (tab.size())       OPERATOR  ((xsize_t_struct)AI) ); \
		bool c2 = ( ((long double)BV)  OPERATOR  ((long double)AI) ); \
		bool OK = (c1==c2); \
		if (OK) cout << "ok" << OPERATOR_NAME << " "; \
		else { cout <<"\nFAILED BV="<<BV<<"  " << "(" << OPERATOR_NAME << ")" << "  " << "AI="<<AI<<" c1="<<c1<<" c2(double)="<<c2<<"! "; } \
		all_ok = all_ok && OK; \
	} while(0)
	COMPARE( < , "<" );
	COMPARE( <= , "<=" );
	COMPARE( > , ">" );
	COMPARE( > , ">=" );
	COMPARE( == , "==" );
	COMPARE( != , "!=" );
	#undef COMPARE

	cout << "." << endl;

	if (!all_ok) abort();
	return all_ok;
}


void main3() {
	int long long x = -3;
	vector<int> tab={0,1,2};
	// if ( x < tab.size()) { FOO(); } //  warning: comparison between signed and unsigned integer expressions [-Wsign-compare]
	if ( (xsize_t_struct) x < tab.size()) { FOO(); }
	if ( (xsize_t_struct) x <= tab.size()) { FOO(); }
	if ( (xsize_t_struct) x > tab.size()) { FOO(); }
	if ( (xsize_t_struct) x >= tab.size()) { FOO(); }

	typedef t_safenumber integral;

	integral p31 = static_cast<long long int>(std::pow(2,31));
	integral p32 = static_cast<long long int>(std::pow(2,32));
	typedef long long int big;
	integral min1 = std::numeric_limits<big>::min();
	integral max1 = std::numeric_limits<big>::max();
	integral min2 = std::numeric_limits<big>::min();
	integral max2 = std::numeric_limits<big>::max();

	vector<integral> i_values = {
		min2,min2+1,min2+2,min1,min1+1,
		-p32-1, -p32, -p32+1, -p31-1, -p31, -p31+1,
		-2,-1,
		0,+1,+2,+3, +100,
		+p31-1,+p31,+p31+1, +p32-1,+p32,+p32+1,
		max1/2-1, max1/2, max2/2-1, max2/2, /* max2+... would go out */
		max1-1, max1, max2-1, max2, /* max2+... would go out */
		};

	vector<integral> v_values = {
		0,+1,+2,+3, +100,
		+p31-1,+p31,+p31+1, +p32-1,+p32,+p32+1,
		max1/2-1, max1/2, max2/2-1, max2/2, max2/2+1, max2/2+2,
		max1-1, max1, max2-1, max2, max2+1, max2+2
	};

	cout<<"Array i_values for the integral numbers:      ";  for (auto i : i_values) std::cout<<i<<" ";  std::cout << std::endl;
	cout<<"Array v_values for the vector size_t numbers: ";  for (auto v : v_values) std::cout<<v<<" ";  std::cout << std::endl;

	bool all_ok = 1;
	for (auto i : i_values) {
		for (auto v : v_values) {
			const bool v_huge = v > static_cast<long long int>(std::pow(2,16));
			if (!v_huge) {
				const bool ok = test_vector_size< vector<char> >( i.convert_to<long long int>() , v.convert_to<size_t>() );
				all_ok = all_ok && ok;
			} else {
				const bool ok = test_vector_size< fake_vector<char> >( i.convert_to<long long int>() , v.convert_to<size_t>() );
				all_ok = all_ok && ok;
			}
		}
	}

	if (!all_ok) {
		cout << "!!! SOME TESTS FAILED !!!" << endl;
	} else cout << "All good :)" << endl;

}

void cpp_safe_int_unittest()
{
	cout << " --- START --- " << endl << endl << endl;
	cout << "size=" << sizeof( size_t ) << endl;
	main3();
}


} // namespace nUnitTest


