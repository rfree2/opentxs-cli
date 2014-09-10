/* See other files here for the LICENCE that applies here. */
/* This adds ways to safly handle integers, sizes, overflows etc */

#ifndef USE_BOOST
	#define USE_BOOST 0
#endif

#if USE_BOOST
  #include <boost/integer.hpp>
#else
#endif

namespace nCppSafe {

// full range of size_t, but with a sign
#if USE_BOOST
  typedef boost::int_t< sizeof(size_t)*8 +1 >::fast msize_t;
#else
  typedef signed long long int xsize_t;
  // TODO static assert that this type can handle minus MAX_SIZE too
#endif

} // namespace nCppSafe



