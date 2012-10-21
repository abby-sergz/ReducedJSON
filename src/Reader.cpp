#include <iostream>
#include <cctype>
#include <cmath>
#include <sstream>
#include <rjson/rjson.hpp>

namespace
{

  const char kEND_OF_LINE  = 0;
  const char kNUMBER_MINUS = '-';
  const char kDOT          = '.';
  const char kCOMMA        = ',';
  const char kQM           = '"';
  const char kSLASH        = '\\';
  const char kOPEN_BRACE   = '{';
  const char kCLOSE_BRACE  = '}';
  const char kCOLON        = ':';

  class Parser : ::rjson::config::noncopyable
  {
  public:
    Parser(std::istream& is);
    void parseTo(::rjson::Node& node);
  private:
    void ParseNumber(::rjson::Node& node);
    void ParseString(::rjson::Node& node);
    void ParseObject(::rjson::Node& node);
    bool getNextChar();
    void EatSpaces();
    void parseToImmediatly(::rjson::Node& node);
    std::string readTill(bool (*isTerminatorFunc)(char c));
  private:
    char m_Char;
    std::istream& m_is;
  };

  Parser::Parser(std::istream& is)
  : m_Char(0)
  , m_is(is)
  {
  }

  void
  Parser::parseTo(::rjson::Node& node)
  {
    if(this->getNextChar() && kEND_OF_LINE != m_Char)
    {
      this->parseToImmediatly(node);
    }
  }

  void
  Parser::parseToImmediatly(::rjson::Node& node)
  {
    this->EatSpaces();
    if(kNUMBER_MINUS == m_Char || isdigit(m_Char))
    {
      this->ParseNumber(node);
    }
    else
    if (kQM == m_Char)
    {
      this->ParseString(node);
    }
    else
    if (kOPEN_BRACE == m_Char)
    {
      this->ParseObject(node);
      kEND_OF_LINE != m_Char && this->getNextChar(); // try read next after '}'
    }
    // if there is some simbols they will be ignored
  }

  void
  Parser::ParseNumber(::rjson::Node& node)
  {
    double num = 0;
    bool neg = kNUMBER_MINUS == m_Char;
    int fractionPartOrder = 0;
    bool isError = false;

    if (neg)
    {
      if (this->getNextChar() && kEND_OF_LINE != m_Char)
      {
        if (isdigit(m_Char))
        {
          if (neg)
          {
            num -= (m_Char - '0');
          }
          else
          {
            num += (m_Char - '0');
          }
        }
        else
        if (kDOT == m_Char)
        {
          ++fractionPartOrder;
        }
        else
        {
          isError = true;
        }
      }
    }
    else
    {
      num += m_Char - '0';
    }

    while(!isError && this->getNextChar() && kEND_OF_LINE != m_Char)
    {
      if (isdigit(m_Char))
      {
        if (0 == fractionPartOrder)
        {
          num *= 10;
          if (neg)
          {
            num -= m_Char - '0';
          }
          else
          {
            num += m_Char - '0';
          }
        }
        else
        {
          if (neg)
          {
            num -= (m_Char - '0')/pow(10., fractionPartOrder);
          }
          else
          {
            num += (m_Char - '0')/pow(10., fractionPartOrder);
          }
          ++fractionPartOrder;
        }
      }
      else
      if (kDOT == m_Char)
      {
        ++fractionPartOrder;
      }
      else
      {
        break;
      }
    }

    if (0 == fractionPartOrder) // integer
    {
      node = static_cast<int64_t>(num);
    }
    else
    {
      node = num;
    }
  }

  void
  Parser::ParseString(::rjson::Node& node)
  {
    std::stringstream ss;
    bool isExist = false;
    while(this->getNextChar() && kEND_OF_LINE != m_Char && !isExist)
    {
      if (kSLASH == m_Char)
      {
        if (this->getNextChar() && kEND_OF_LINE != m_Char)
        {
          if (kSLASH == m_Char)
          {
            ss << kSLASH;
          }
          else
          if (kQM == m_Char)
          {
            ss << kQM;
          }
          else
          {
            ss << kSLASH << m_Char;
          }
        }
      }
      else
      {
        if (kQM == m_Char)
        {
          node = ss.str();
          isExist = true;
        }
        else
        {
          ss << m_Char;
        }
      }
    }
  }

  bool isSpaceOrColon(char c)
  {
    return kCOLON == c || isspace(c);
  }

  bool isQM(char c)
  {
    return kQM == c;
  }

  void
  Parser::ParseObject(::rjson::Node& node)
  {
    do
    {
      if(this->getNextChar() && kEND_OF_LINE != m_Char)
      {
        this->EatSpaces();
      }
      std::string name;
      if (kQM == m_Char)
      {
        this->getNextChar(); // fetch the next symbol
        name = this->readTill(&isQM);
        kEND_OF_LINE != m_Char && this->getNextChar(); // read closing "
      }
      else
      {
        name = this->readTill(&isSpaceOrColon);
      }
      std::stringstream ss;

      if (kCOLON != m_Char)
      {
        this->EatSpaces();
      }
      
      if (kCOLON == m_Char)
      {
        if(this->getNextChar() && kEND_OF_LINE != m_Char)
        {
          this->EatSpaces();
        }
        ::rjson::Node node_value;
        this->parseToImmediatly(node_value);
        node.push_back(::rjson::Node(name, node_value));
        this->EatSpaces();
      }
    }
    while(kCOMMA == m_Char);
  }

  void
  Parser::EatSpaces()
  {
    while(isspace(m_Char) && this->getNextChar() && kEND_OF_LINE != m_Char);
  }

  bool
  Parser::getNextChar()
  {
    bool ret = m_is.good();
    if (ret)
    {
      m_is.get(m_Char);
      ret = m_is.good();
    }
    return ret;
  }

  std::string
  Parser::readTill(bool (*isTerminatorFunc)(char c))
  {
    std::stringstream ss;
    bool isExit = false;
    do
    {
      if (kEND_OF_LINE != m_Char)
      {
        if (NULL != isTerminatorFunc)
        {
          if (!(*isTerminatorFunc)(m_Char))
          {
            ss << m_Char;
          }
          else
          {
            isExit = true;
          }
        }
        else
        {
          ss << m_Char;
        }
      }
      else
      {
        isExit = true;
      }
    }
    while(!isExit && this->getNextChar());
    return ss.str();
  }

}

namespace rjson
{
  Node read(std::istream& is)
  {
    Node ret;
    Parser p(is);

    p.parseTo(ret);

    return ret;
  }

  Node read(const std::string& str)
  {
    std::stringstream ss;
    ss.str(str);
    return read(ss);
  }
}