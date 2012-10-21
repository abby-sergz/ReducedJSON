#include <vector>
#include <string>
#include <stdexcept>
#include <rjson/base.hpp>
#include <boost/variant.hpp>
#include <boost/make_shared.hpp>
#include <rjson/rjson.hpp>
#include <rjson/NodeImpl.hpp>
#include <rjson/ConvertValue.hpp>
#include <rjson/ObjectValue.hpp>

namespace
{

  class Undefined
  {
  public:
    bool operator==(const Undefined& ) const
    {
      return false;
    }
  };
}

namespace rjson
{
  namespace detail
  {
    ObjectValue::ObjectValue()
    {
    }

    ObjectValue::ObjectValue(const ::rjson::Node& value)
    {
      this->push_back(value);
    }

    void
    ObjectValue::push_back(const ::rjson::Node& value)
    {
      m_Values.push_back(value);
    }

    bool
    ObjectValue::operator ==(const ObjectValue& value) const
    {
      return m_Values == value.m_Values;
    }

    size_t
    ObjectValue::size() const
    {
      return m_Values.size();
    }

    TValues::const_iterator
    ObjectValue::begin() const
    {
      return m_Values.begin();
    }

    TValues::const_iterator
    ObjectValue::end() const
    {
      return m_Values.end();
    }

    ObjectValue_const_iterator::ObjectValue_const_iterator(const TValues::const_iterator& value)
    : m_RealIterator(value)
    {
    }

    bool
    ObjectValue_const_iterator::operator!=(const ObjectValue_const_iterator& value) const
    {
      return m_RealIterator != value.m_RealIterator;
    }

    ObjectValue_const_iterator&
    ObjectValue_const_iterator::operator++()
    {
      ++m_RealIterator;
      return *this;
    }

    const ::rjson::Node*
    ObjectValue_const_iterator::operator->() const
    {
      return m_RealIterator.operator->();
    }
  }
}

namespace
{

  struct GetNodeType : boost::static_visitor< ::rjson::ENodeType::type>
  {
    ::rjson::ENodeType::type operator()(int64_t) const
    {
      return ::rjson::ENodeType::kINTEGER;
    }

    ::rjson::ENodeType::type operator()(double ) const
    {
      return ::rjson::ENodeType::kDOUBLE;
    }

    ::rjson::ENodeType::type operator()(const std::string&) const
    {
      return ::rjson::ENodeType::kSTRING;
    }

    ::rjson::ENodeType::type operator()(const ::rjson::detail::ObjectValue&) const
    {
      return ::rjson::ENodeType::kOBJECT;
    }

    ::rjson::ENodeType::type operator()(const ::Undefined&) const
    {
      return ::rjson::ENodeType::kUNDEFINED;
    }
  };

  struct ConvertToDouble : boost::static_visitor<double>
  {
    double operator()(int64_t value) const
    {
      return ::rjson::helper::ConvertValue<double>(value);
    }

    double operator()(double value) const
    {
      return value;
    }

    double operator()(const std::string& value) const
    {
      return ::rjson::helper::ConvertValue<double>(value);
    }

    double operator()(const ::rjson::detail::ObjectValue& value) const
    {
      if (value.size() != 1)
      {
        throw std::domain_error("Convertsion from `Object` to `double` is impossible.");
      }
      return value.begin()->asDouble();
    }

    double operator()(const ::Undefined& /*value*/) const
    {
      throw std::domain_error("Convertsion from `Undefined` to `double` is impossible.");
    }
  };

  struct ConvertToInt64 : boost::static_visitor<int64_t>
  {
    int64_t operator()(int64_t value) const
    {
      return value;
    }

    int64_t operator()(double value) const
    {
      return ::rjson::helper::ConvertValue<int64_t>(value);
    }

    int64_t operator()(const std::string& value) const
    {
      return ::rjson::helper::ConvertValue<int64_t>(value.c_str());
    }

    int64_t operator()(const ::rjson::detail::ObjectValue& value) const
    {
      if (value.size() != 1)
      {
        throw std::domain_error("Convertsion from `ObjectValue` to `int64_t` is impossible.");
      }
      return value.begin()->asInt64();
    }

    int64_t operator()(const ::Undefined& /*value*/) const
    {
      throw std::domain_error("Convertsion from `Undefined` to `int64_t` is impossible.");
    }
  };

  struct ConvertToString : boost::static_visitor<std::string>
  {
    std::string operator()(int64_t value) const
    {
      return ::rjson::helper::ConvertValue<std::string>(value);
    }

    std::string operator()(double value) const
    {
      return ::rjson::helper::ConvertValue<std::string>(value);
    }

    const std::string& operator()(const std::string& value) const
    {
      return value;
    }

    std::string operator()(const ::rjson::detail::ObjectValue& value) const
    {
      if (value.size() != 1)
      {
        throw std::domain_error("Convertsion from `ObjectValue` to `std::string` is impossible.");
      }
      return value.begin()->asString();
    }

    std::string operator()(const ::Undefined& /*value*/) const
    {
      throw std::domain_error("Convertsion from `Undefined` to `int64_t` is impossible.");
    }
  };

  struct GetElementsNumber : boost::static_visitor<size_t>
  {
    template<typename T>
    size_t operator()(const T&) const
    {
      return 0;
    }

    size_t operator()(const ::rjson::detail::ObjectValue& value) const
    {
      return value.size();
    }

  };

  struct NodeImpl : rjson::detail::INodeImpl
  {
  public:
    typedef boost::variant<double, int64_t, std::string, ::rjson::detail::ObjectValue, Undefined> TValue;

    NodeImpl();
    NodeImpl(const ::rjson::Node& value);

    // INodeImpl -- BEGIN --
#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4481) //warning C4481: nonstandard extension used: override specifier 'override'
#endif

    virtual ::rjson::detail::TINodeImplPtr clone() const RJSON_OVERRIDE;
    virtual void setValue(double value) RJSON_OVERRIDE;
    virtual void setValue(int64_t value) RJSON_OVERRIDE;
    virtual void setValue(const std::string& value) RJSON_OVERRIDE;
    virtual void push_back(const ::rjson::Node& value) RJSON_OVERRIDE;
    virtual bool IsEqual(double value) const RJSON_OVERRIDE;
    virtual bool IsEqual(int64_t value) const RJSON_OVERRIDE;
    virtual bool IsEqual(const std::string& value) const RJSON_OVERRIDE;
    virtual ::rjson::ENodeType::type getType() const RJSON_OVERRIDE;
    virtual int64_t asInt64() const RJSON_OVERRIDE;
    virtual std::string asString() const RJSON_OVERRIDE;
    virtual double asDouble() const RJSON_OVERRIDE;
    virtual size_t size() const RJSON_OVERRIDE;
    virtual ::rjson::config::shared_ptr< ::rjson::detail::ObjectValue_const_iterator> getBegin() const RJSON_OVERRIDE;
    virtual ::rjson::config::shared_ptr< ::rjson::detail::ObjectValue_const_iterator> getEnd() const RJSON_OVERRIDE;
#ifdef WIN32
#pragma warning(pop)
#endif
    // INodeImpl  -- END --
    void setValue(const ::rjson::detail::ObjectValue& value);
    template<typename Visitor>
    typename Visitor::result_type apply_visitor(const Visitor& visitor)
    {
      return boost::apply_visitor(visitor, m_Value);
    }
  private:
    TValue m_Value;
  };

  struct EnsureObjectValue : boost::static_visitor< ::rjson::detail::ObjectValue&>, ::rjson::config::noncopyable
  {
    EnsureObjectValue(NodeImpl& nodeImpl)
    : m_NodeImpl(nodeImpl)
    {
    }

    template<typename T>
    ::rjson::detail::ObjectValue& operator()(const T&) const
    {
      m_NodeImpl.setValue(::rjson::detail::ObjectValue());
      return boost::apply_visitor(*this, m_NodeImpl);
    }
    
    ::rjson::detail::ObjectValue& operator()(::rjson::detail::ObjectValue& value) const
    {
      return value;
    }

  private:
    NodeImpl& m_NodeImpl;
  };

  struct CreateIterator : boost::static_visitor< ::rjson::config::shared_ptr< ::rjson::detail::ObjectValue_const_iterator> >, ::rjson::config::noncopyable
  {
    CreateIterator(bool isBegin)
    : m_IsBegin(isBegin)
    {
    }

    template<typename T>
    result_type operator()(const T&) const
    {
      return result_type();
    }
    
    result_type operator()(const ::rjson::detail::ObjectValue& value) const
    {
      result_type ret = ::rjson::config::make_shared<result_type::element_type>(m_IsBegin ? value.begin() : value.end());
      return ret;
    }
  private:
    bool m_IsBegin;
  };

  NodeImpl::NodeImpl()
  : m_Value(Undefined())
  {
  }

  //NodeImpl::NodeImpl(const ::rjson::Node& value)
  //: m_Value(::rjson::detail::ObjectValue(value))
  //{
  //}

  NodeImpl::NodeImpl(const ::rjson::Node& value)
  : m_Value(Undefined())
  {
    if (::rjson::ENodeType::kOBJECT == value.getType())
    {
      ::rjson::detail::ObjectValue objectValue;
      for (::rjson::Node::const_iterator ci = value.begin(); ci != value.end(); ++ci)
      {
        objectValue.push_back(*ci);
      }
      m_Value = objectValue;
    }
    else
    if (::rjson::ENodeType::kSTRING == value.getType())
    {
      m_Value = value.asString();
    }
    else
    if (::rjson::ENodeType::kINTEGER == value.getType())
    {
      m_Value = value.asInt64();
    }
    else
    if (::rjson::ENodeType::kDOUBLE == value.getType())
    {
      m_Value = value.asDouble();
    }
  }


  ::rjson::detail::TINodeImplPtr
  NodeImpl::clone() const
  {
    return ::rjson::detail::TINodeImplPtr(new NodeImpl(*this));
  }

  void
  NodeImpl::setValue(double value)
  {
    m_Value = value;
  }

  void
  NodeImpl::setValue(int64_t value)
  {
    m_Value = value;
  }

  void
  NodeImpl::setValue(const std::string& value)
  {
    m_Value = value;
  }

  void
  NodeImpl::push_back(const ::rjson::Node& value)
  {
    EnsureObjectValue visitor(*this);
    boost::apply_visitor(visitor, m_Value).push_back(value);
  }

  bool
  NodeImpl::IsEqual(double value) const
  {
    TValue v(value);
    return m_Value == v;
  }

  bool
  NodeImpl::IsEqual(int64_t value) const
  {
    TValue v(value);
    return m_Value == v;
  }

  bool
  NodeImpl::IsEqual(const std::string& value) const
  {
    TValue v(value);
    return m_Value == v;
  }

  ::rjson::ENodeType::type
  NodeImpl::getType() const
  {
    GetNodeType visitor;
    return boost::apply_visitor(visitor, m_Value);
  }

  int64_t
  NodeImpl::asInt64() const
  {
    ConvertToInt64 visitor;
    return boost::apply_visitor(visitor, m_Value);
  }

  std::string
  NodeImpl::asString() const
  {
    ConvertToString visitor;
    return boost::apply_visitor(visitor, m_Value);
  }

  double
  NodeImpl::asDouble() const
  {
    ConvertToDouble visitor;
    return boost::apply_visitor(visitor, m_Value);
  }

  size_t
  NodeImpl::size() const
  {
    GetElementsNumber visitor;
    return boost::apply_visitor(visitor, m_Value);
  }

  ::rjson::config::shared_ptr< ::rjson::detail::ObjectValue_const_iterator>
  NodeImpl::getBegin() const
  {
    CreateIterator visitor(/*begin = */true);
    return boost::apply_visitor(visitor, m_Value);
  }

  ::rjson::config::shared_ptr< ::rjson::detail::ObjectValue_const_iterator>
  NodeImpl::getEnd() const
  {
    CreateIterator visitor(/*begin = */false /*thus `end`*/);
    return boost::apply_visitor(visitor, m_Value);
  }

  void
  NodeImpl::setValue(const rjson::detail::ObjectValue& value)
  {
    m_Value = value;
  }
}

namespace rjson
{
  namespace detail
  {
    TINodeImplPtr createNodeImpl()
    {
      return TINodeImplPtr(new NodeImpl());
    }

    TINodeImplPtr createNodeImpl(const Node& value)
    {
      return TINodeImplPtr(new NodeImpl(value));
    }
  }
}
