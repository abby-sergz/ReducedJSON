#pragma once

#include <inttypes.h>
#include <rjson/base.hpp>

namespace rjson
{
  namespace detail
  {
    typedef ::rjson::config::shared_ptr<INodeImpl> TINodeImplPtr;
    struct ObjectValue_const_iterator;
    struct INodeImpl
    {
      virtual ~INodeImpl(){}
      virtual TINodeImplPtr clone() const = 0;
      virtual void setValue(double value) = 0;
      virtual void setValue(int64_t value) = 0;
      virtual void setValue(const std::string& value) = 0;
      virtual void push_back(const Node& value) = 0;
      virtual bool IsEqual(double value) const = 0;
      virtual bool IsEqual(int64_t value) const = 0;
      virtual bool IsEqual(const std::string& value) const = 0;
      virtual ::rjson::ENodeType::type getType() const = 0;
      virtual int64_t asInt64() const = 0;
      virtual std::string asString() const = 0;
      virtual double asDouble() const = 0;
      virtual size_t size() const = 0;
      virtual ::rjson::config::shared_ptr<ObjectValue_const_iterator> getBegin() const = 0;
      virtual ::rjson::config::shared_ptr<ObjectValue_const_iterator> getEnd() const = 0;
    };
    
    TINodeImplPtr createNodeImpl();
    TINodeImplPtr createNodeImpl(const Node& value);
  }
}
