#pragma once

#if defined(REDUCED_JSON_USE_BOOST)
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#else
#include <memory>
#endif

#if WIN32 || __cplusplus > 199711L
#define RJSON_OVERRIDE override 
#else
#define RJSON_OVERRIDE
#endif


namespace rjson
{
  namespace config
  {
    #if defined(REDUCED_JSON_USE_BOOST)
    using boost::shared_ptr;
    using ::boost::make_shared;
	
    #else
    using ::std::shared_ptr;
    using ::std::make_shared;
    #endif

	// copied from http://www.boost.org/doc/libs/1_51_0/boost/noncopyable.hpp
	namespace noncopyable_  // protection from unintended ADL
	{
	  class noncopyable
	  {
	   protected:
		  noncopyable() {}
		  ~noncopyable() {}
	   private:  // emphasize the following members are private
		  noncopyable( const noncopyable& );
		  const noncopyable& operator=( const noncopyable& );
	  };
	}

	typedef noncopyable_::noncopyable noncopyable;

  }
}
