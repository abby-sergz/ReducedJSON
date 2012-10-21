#pragma once
#include <vector>
#include <rjson/base.hpp>

namespace rjson
{
  class Node;
  namespace detail
  {
    typedef std::vector< ::rjson::Node> TValues;
    class ObjectValue
    {
    public:
      ObjectValue();
      explicit
      ObjectValue(const ::rjson::Node& value);
      void push_back(const ::rjson::Node& value);
      bool operator==(const ObjectValue& value) const;
      size_t size() const;
      TValues::const_iterator begin() const;
      TValues::const_iterator end() const;
    private:
      TValues m_Values;
    };

    struct ObjectValue_const_iterator
    {
      ObjectValue_const_iterator(const TValues::const_iterator& value);
      bool operator!=(const ObjectValue_const_iterator& value) const;
      ObjectValue_const_iterator& operator++(); // prefix increment
      const ::rjson::Node* operator->() const;
    private:
      ObjectValue_const_iterator operator++(int); // postfix increment, not implemented
    private:
      TValues::const_iterator m_RealIterator;
    };


  }
}
