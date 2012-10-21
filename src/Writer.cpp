#include <iostream>
#include <stdexcept>
#include <rjson/rjson.hpp>

using std::endl;

using ::rjson::ENodeType;
using ::rjson::Node;

namespace
{
  void write_impl(const Node& src, std::ostream& os, bool isDebug = false)
  {
    bool use_comma = false;
    if (src.size() > 0)
    {
      os << "{" << endl;
      for (Node::const_iterator ci = src.begin(); ci != src.end(); ++ci)
      {
        if (use_comma)
        {
          os << ", " << endl;
        }
        switch(ci->getType())
        {
        case ENodeType::kOBJECT:
          os << ci->getName() << ":" << endl;
          write(*ci, os);
          break;
        case ENodeType::kINTEGER:
          os << ci->getName() << (isDebug ? ": (integer)" : ":") << ci->asInt64() << endl;
          break;
        case ENodeType::kDOUBLE:
          os << ci->getName() << (isDebug ? ": (double)" : ":") << ci->asDouble() << endl;
          break;
        case ENodeType::kSTRING:
          os << ci->getName() << (isDebug ? ": (string)\"" : ":\"") << ci->asString() << "\"" << endl;
          break;
        case ENodeType::kUNDEFINED:
          os << ci->getName() << (isDebug ? ": (undefined)" : ":") << endl;
          break;
        default:
          throw std::logic_error("Wrong node type value");
        }
        use_comma = true;
      }
      os << "}" << endl;
    }
    else
    {
      switch(src.getType())
      {
      case ENodeType::kOBJECT:
        os << "{" << endl;
        os << "}" << endl;
        break;
      case ENodeType::kINTEGER:
        os << src.getName() << (isDebug ? ": (integer)" : ":") << src.asInt64() << endl;
        break;
      case ENodeType::kDOUBLE:
        os << src.getName() << (isDebug ? ": (double)" : ":") << src.asDouble() << endl;
        break;
      case ENodeType::kSTRING:
        os << src.getName() << (isDebug ? ": (string)\"" : ":\"") << src.asString() << "\"" << endl;
        break;
      case ENodeType::kUNDEFINED:
        os << src.getName() << (isDebug ? ": (undefined)" : ":") << endl;
        break;
      default:
        throw std::logic_error("Wrong node type value");
      }
    }
  }

}

namespace rjson
{

void write(const Node& src, std::ostream& os)
{
  ::write_impl(src, os);
}

void write_debug(const Node& src, std::ostream& os)
{
  ::write_impl(src, os, /*isDebug = */ true);
}

}
