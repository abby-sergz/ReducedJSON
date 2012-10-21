#include <sstream>
#include <rjson/ConvertValue.hpp>

namespace rjson
{
  namespace helper
  {
    template<>
    int64_t ConvertValue<int64_t>(const std::string& src)
    {
  #ifdef WIN32
      return _atoi64(src.c_str());
  #else
      return atoll(src.c_str());
  #endif
    }

    template<>
    double ConvertValue<double>(int64_t value)
    {
      return static_cast<double>(value);
    }

    template<>
    std::string ConvertValue<std::string>(int64_t value)
    {
      std::stringstream ss;
      ss << value;
      return ss.str();
    }

    template<>
    int64_t ConvertValue<int64_t>(double value)
    {
      return static_cast<int64_t>(value);
    }

    template<>
    std::string ConvertValue<std::string>(double value)
    {
      std::stringstream ss;
      ss << value;
      return ss.str();
    }

    template<>
    double ConvertValue<double>(const std::string& src)
    {
      return atof(src.c_str());
    }
  }
}