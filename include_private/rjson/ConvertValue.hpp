#pragma once

#include <string>
#include <inttypes.h>
#include <rjson/base.hpp>

namespace rjson
{
namespace helper
{
  namespace detail
  {
    template<typename TTarget, typename TSource>
    struct ReturnTypeSelector;
    
    template<>
    struct ReturnTypeSelector<const std::string&, const std::string&>
    {
      typedef const std::string& type;
    };

    template<>
    struct ReturnTypeSelector<int64_t, const std::string&>
    {
      typedef int64_t type;
    };

    template<>
    struct ReturnTypeSelector<double, const std::string&>
    {
      typedef double type;
    };

    template<>
    struct ReturnTypeSelector<std::string, int64_t>
    {
      typedef std::string type;
    };

    template<>
    struct ReturnTypeSelector<double, int64_t>
    {
      typedef double type;
    };

    template<>
    struct ReturnTypeSelector<int64_t, double>
    {
      typedef int64_t type;
    };

    template<>
    struct ReturnTypeSelector<std::string, double>
    {
      typedef std::string type;
    };

    template<typename X>
    struct ReturnTypeSelector<X&, const std::string&>; // We've just converted from a string and there is only temporary variable 'X' type.

  }

  template<typename T>
  typename detail::ReturnTypeSelector<T, const std::string&>::type ConvertValue(const std::string& src);

  template<typename T>
  typename detail::ReturnTypeSelector<T, int64_t>::type ConvertValue(int64_t src);

  template<typename T>
  typename detail::ReturnTypeSelector<T, double>::type ConvertValue(double src);

    template<>
    int64_t ConvertValue<int64_t>(const std::string& src);

    template<>
    double ConvertValue<double>(int64_t value);

    template<>
    std::string ConvertValue<std::string>(int64_t value);

    template<>
    int64_t ConvertValue<int64_t>(double value);

    template<>
    std::string ConvertValue<std::string>(double value);

    template<>
    double ConvertValue<double>(const std::string& src);
} // namespace helper
} // namespace rjson