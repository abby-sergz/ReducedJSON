#pragma once

#include <istream>
#include <string>
#include <inttypes.h>

#include <rjson/base.hpp>

namespace rjson
{
  class Node;

  struct ENodeType
  {
    enum type
    {
      kUNDEFINED = 0,
      kINTEGER      ,
      kDOUBLE       ,
      kSTRING       ,
      kOBJECT
    };
  };

  namespace detail
  {
    struct INodeImpl;
    struct ObjectValue_const_iterator;
    struct const_iterator : public ::rjson::config::noncopyable
    {
      friend class ::rjson::Node;
    public:
      bool operator!=(const const_iterator& value) const;
      const_iterator& operator++(); // prefix increment
      const_iterator(const const_iterator&);
      const ::rjson::Node& operator*() const;
      const ::rjson::Node* operator->() const;
    private:
      const_iterator(const ::rjson::Node& parentNode);
      const_iterator operator++(int); // postfix increment, not implemented
    private:
      const Node& m_ParentNode;
	  ::rjson::config::shared_ptr<ObjectValue_const_iterator> m_InternalIterator;
    };
  }

  class Node : public ::rjson::config::noncopyable
  {
    typedef ::rjson::config::shared_ptr<detail::INodeImpl> TNodeImpl;
  public:
    Node();
    Node(const std::string& name, const Node& value);
    Node(const Node& src);
  
    Node& operator=(const Node& src);
    ~Node();

    Node& operator=(double value);
    Node& operator=(int64_t value);
    Node& operator=(const std::string& value);

    void push_back(const Node& value);

    std::string getName() const;
    void setName(const std::string& value);
    ENodeType::type getType() const;
    bool isNumber() const; // return true if type is double or integer
    bool operator==(double value) const;
    bool operator==(int64_t value) const;
    bool operator==(const std::string& value) const;
    bool operator==(const Node& value) const;

    int64_t asInt64() const;
    std::string asString() const;
    double asDouble() const;

    size_t size() const;
    typedef detail::const_iterator const_iterator;
    const_iterator begin() const;
    const_iterator end() const;
  private:
    std::string m_Name;
    TNodeImpl m_Impl;
  };

  bool operator==(int64_t value, const Node& node);
  bool operator==(double value, const Node& node);
  bool operator==(const std::string& value, const Node& node);


  Node read(std::istream& is);
  Node read(const std::string& str);

  void write(const Node& src, std::ostream& os);
  void write_debug(const Node& src, std::ostream& os);

}
