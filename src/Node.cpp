#include <stdexcept>
#include <rjson/base.hpp>
#include <rjson/rjson.hpp>
#include <rjson/NodeImpl.hpp>
#include <rjson/ObjectValue.hpp>

using namespace rjson;

namespace rjson
{
  namespace detail
  {
    const_iterator::const_iterator(const ::rjson::Node& parentNode)
    : m_ParentNode(parentNode)
    {
    }

    bool
    const_iterator::operator!=(const const_iterator& value) const
    {
      return &m_ParentNode == &value.m_ParentNode && NULL != m_InternalIterator.get() && NULL != value.m_InternalIterator.get() && *m_InternalIterator != *value.m_InternalIterator;
    }

    const_iterator&
    const_iterator::operator++()
    {
      if (NULL != m_InternalIterator.get())
      {
        m_InternalIterator->operator++();
      }
      return *this;
    }

    const_iterator::const_iterator(const const_iterator& src)
    : ::rjson::config::noncopyable()
    , m_ParentNode(src.m_ParentNode)
    , m_InternalIterator((NULL != src.m_InternalIterator.get()) ? new ObjectValue_const_iterator(*src.m_InternalIterator) : NULL)
    {
    }

    const ::rjson::Node&
    const_iterator::operator*() const
    {
      return *this->operator->();
    }
    
    const ::rjson::Node*
    const_iterator::operator->() const
    {
      if (NULL == m_InternalIterator.get())
      {
        throw std::logic_error("Ivalid iterator");
      }
      return m_InternalIterator->operator->();
    }

  }
}

Node::Node()
: m_Impl(::rjson::detail::createNodeImpl())
{
}

Node::Node(const std::string& name, const Node& value)
: m_Name(name)
, m_Impl(::rjson::detail::createNodeImpl(value))
{
}

Node::Node(const Node& src)
: ::rjson::config::noncopyable()
, m_Name(src.m_Name)
, m_Impl(src.m_Impl->clone())
{
}

Node&
Node::operator =(const rjson::Node &src)
{
  this->m_Impl = src.m_Impl->clone();
  return *this;
}

Node::~Node()
{
}

Node&
Node::operator=(double value)
{
  this->m_Impl->setValue(value);
  return *this;
}

Node&
Node::operator=(int64_t value)
{
  this->m_Impl->setValue(value);
  return *this;
}

Node&
Node::operator=(const std::string& value)
{
  this->m_Impl->setValue(value);
  return *this;
}

void
Node::push_back(const Node& value)
{
  this->m_Impl->push_back(value);
}

std::string
Node::getName() const
{
  return m_Name;
}

void
Node::setName(const std::string& value)
{
  m_Name = value;
}

ENodeType::type
Node::getType() const
{
  return this->m_Impl->getType();
}

bool
Node::isNumber() const
{
  return ENodeType::kINTEGER == this->getType() || ENodeType::kDOUBLE == this->getType();
}

bool
Node::operator==(double value) const
{
  return this->m_Impl->IsEqual(value);
}

bool
Node::operator==(int64_t value) const
{
  return this->m_Impl->IsEqual(value);
}

bool
Node::operator==(const std::string& value) const
{
  return this->m_Impl->IsEqual(value);
}

bool
Node::operator==(const Node& value) const
{
  bool ret = ENodeType::kUNDEFINED != this->getType() && ENodeType::kUNDEFINED != value.getType() && this->getType() == value.getType();
  if (ret)
  {
    switch(this->getType())
    {
    case ENodeType::kOBJECT:
      {
        ret = this->size() == value.size(); // some kind of optimazing
        for (const_iterator ci_this = this->begin(), ci_value = value.begin(); ret && ci_this != this->end() && ci_value != value.end(); ++ci_this, ++ci_this)
        {
          ret = *ci_this == *ci_value;
        }
      }
      break;
    case ENodeType::kINTEGER:
      ret = this->operator ==(value.asInt64());
      break;
   case ENodeType::kDOUBLE:
      ret = this->operator ==(value.asDouble());
      break;
   case ENodeType::kSTRING:
      ret = this->operator ==(value.asString());
      break;
    case ENodeType::kUNDEFINED:
    default:
      ;
    }
  }
  return ret;
}

int64_t
Node::asInt64() const
{
  return this->m_Impl->asInt64();
}

std::string
Node::asString() const
{
  return this->m_Impl->asString();
}

double
Node::asDouble() const
{
  return this->m_Impl->asDouble();
}

size_t
Node::size() const
{
  return this->m_Impl->size();
}

Node::const_iterator
Node::begin() const
{
  Node::const_iterator ret(*this);
  ret.m_InternalIterator = this->m_Impl->getBegin();
  return ret;
}

Node::const_iterator
Node::end() const
{
  Node::const_iterator ret(*this);
  ret.m_InternalIterator = this->m_Impl->getEnd();
  return ret;
}

namespace rjson
{
  bool operator==(int64_t value, const Node& node)
  {
    return node == value;
  }

  bool operator==(double value, const Node& node)
  {
    return node == value;
  }

  bool operator==(const std::string& value, const Node& node)
  {
    return node == value;
  }
}
