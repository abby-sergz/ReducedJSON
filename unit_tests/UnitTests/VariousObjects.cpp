#include <iostream>
#include <unit_tests/unit_tests.hpp>
#include <rjson/../../include_private/rjson/ConvertValue.hpp>

UNIT_TEST_REGISTER_LIBRARY(VariousObjects)

using std::cout;
using std::endl;
using ::rjson::Node;
using ::rjson::ENodeType;

namespace
{
  const int isWrite = 0;
}


TEST(BiggerObject, VariousObjects)
{
  std::string str = "{ id: 2, data: { top: \"white\", bottom: \"blue\" }, item :{name  :  \"product\",price:100.99} }";
  ::rjson::Node node = ::rjson::read(str);
  ASSERT_EQ(3u, node.size());
}

TEST(IntegerDouble, VariousObjects)
{
  {
    std::string str = "123";
    ::rjson::Node node = ::rjson::read(str);
    EXPECT_EQ(ENodeType::kINTEGER, node.getType());
    EXPECT_EQ(123, node.asInt64());
  }
  {
    std::string str = "123.";
    ::rjson::Node node = ::rjson::read(str);
    EXPECT_EQ(ENodeType::kDOUBLE, node.getType());
    EXPECT_DOUBLE_EQ(123., node.asDouble());
  }
  {
    std::string str = "123.123";
    ::rjson::Node node = ::rjson::read(str);
    EXPECT_EQ(ENodeType::kDOUBLE, node.getType());
    EXPECT_DOUBLE_EQ(123.123, node.asDouble());
  }

  {
    std::string str = "-123";
    ::rjson::Node node = ::rjson::read(str);
    EXPECT_EQ(ENodeType::kINTEGER, node.getType());
    EXPECT_EQ(-123, node.asInt64());
  }
  {
    std::string str = "-123.";
    ::rjson::Node node = ::rjson::read(str);
    EXPECT_EQ(ENodeType::kDOUBLE, node.getType());
    EXPECT_DOUBLE_EQ(-123., node.asDouble());
  }
  {
    std::string str = "-123.123";
    ::rjson::Node node = ::rjson::read(str);
    EXPECT_EQ(ENodeType::kDOUBLE, node.getType());
    EXPECT_DOUBLE_EQ(-123.123, node.asDouble());
  }

  {
    std::string str = "0.123";
    ::rjson::Node node = ::rjson::read(str);
    EXPECT_EQ(ENodeType::kDOUBLE, node.getType());
    EXPECT_DOUBLE_EQ(.123, node.asDouble());
  }
  {
    std::string str = "-0.123";
    ::rjson::Node node = ::rjson::read(str);
    EXPECT_EQ(ENodeType::kDOUBLE, node.getType());
    EXPECT_DOUBLE_EQ(-.123, node.asDouble());
  }

}

TEST(Construct_Serialize_Parse_IsEqual_1, VariousObjects)
{
  Node name;
  name.setName("name");
  name = "gadget";
  Node price;
  price.setName("price");
  price = 500 - 0.01;
  
  Node item;
  item.push_back(name);
  item.push_back(price);
  if (isWrite)
  {
    using std::cout;
    write(item, cout);
  }

  std::stringstream ss;
  write(item, ss);
  
  Node new_item = ::rjson::read(ss);
  if (isWrite)
  {
    using std::cout;
    cout <<"------------" << endl;
    write(new_item, cout);
  }
  EXPECT_EQ(item, new_item);
}


TEST(Construct_Serialize_Parse_IsEqual_2, VariousObjects)
{
  Node name;
  name.setName("name");
  name = "gadget";
  Node price;
  price.setName("price");
  price = 500 - 0.01;
  
  Node item;
  item.push_back(name);
  item.push_back(price);
  if (isWrite)
  {
    using std::cout;
    write(item, cout);
  }

  Node items;
  for (int64_t i = 0; i < 10; ++i)
  {
    item.setName("item$" + rjson::helper::ConvertValue<std::string>(i));
    items.push_back(item);
    if (i % 3 == 0)
    {
      Node x_item = item;
      x_item.setName("x_item");
      x_item.push_back(item);
      items.push_back(x_item);
    }
  }

  std::stringstream ss;
  write(items, ss);
  
  Node new_items = ::rjson::read(ss);
  if (isWrite)
  {
    using std::cout;
    cout <<"------------" << endl;
    write(new_items, cout);
  }
  EXPECT_EQ(items, new_items);
}

TEST(ValueEscaping, VariousObjects)
{
  {
    std::string str = "{ data: \"value\"}";
    ::rjson::Node xnode = ::rjson::read(str);
    ASSERT_EQ(1u, xnode.size());
    const Node& node = *xnode.begin();

    EXPECT_EQ("value", node);
  }

  { // val"ue
    std::string str = "{ data: \"val\\\"ue\"}";
    ::rjson::Node xnode = ::rjson::read(str);
    ASSERT_EQ(1u, xnode.size());
    const Node& node = *xnode.begin();

    EXPECT_EQ("val\"ue", node);
  }

  { // '\n' and others are not supported
    std::string str = "{ data: \"val\\nue\"}";
    ::rjson::Node xnode = ::rjson::read(str);
    ASSERT_EQ(1u, xnode.size());
    const Node& node = *xnode.begin();

    EXPECT_EQ("val\\nue", node);
  }
}

TEST(Spaces, VariousObjects)
{
  {
    std::string str = "  \n  23   ignored\n";

    ::rjson::Node node = ::rjson::read(str);
    EXPECT_EQ(23, node.asInt64());
  }

  {
    std::string str = "  \n  23ignored\n";

    ::rjson::Node node = ::rjson::read(str);
    EXPECT_EQ(23, node.asInt64());
  }

  {
    std::string str = "      \"John Doe\"ignored        ";

    ::rjson::Node node = ::rjson::read(str);
    EXPECT_EQ("John Doe", node.asString());
  }
  {
    std::string str = "      \"John Doe\"      ignored        ";

    ::rjson::Node node = ::rjson::read(str);
    EXPECT_EQ("John Doe", node.asString());
  }

  {
    std::string str =  "    {  item: \"Gadget\"      ,        price      :  499.99      }   ignored   ";
    ::rjson::Node node = ::rjson::read(str);
    ASSERT_EQ(2u, node.size());
    ::rjson::Node::const_iterator ci = node.begin();
    EXPECT_EQ("item", ci->getName());
    EXPECT_EQ("Gadget", ci->asString());
    ++ci;
    EXPECT_EQ("price", ci->getName());
    EXPECT_DOUBLE_EQ(499.99, ci->asDouble());
  }
  {
    std::string str =  "    {  item: \"Gadget\"      ,        price      :  499.99      }ignored     ";
    ::rjson::Node node = ::rjson::read(str);
    ASSERT_EQ(2u, node.size());
    ::rjson::Node::const_iterator ci = node.begin();
    EXPECT_EQ("item", ci->getName());
    EXPECT_EQ("Gadget", ci->asString());
    ++ci;
    EXPECT_EQ("price", ci->getName());
    EXPECT_DOUBLE_EQ(499.99, ci->asDouble());
  }
}

TEST(PropertyNameSurroundedByQuotes, VariousObjects)
{
  std::string str =  "{\" i t e:m{}\": \"Gadget\" ,price:499.99}";
  ::rjson::Node node = ::rjson::read(str);
  ASSERT_EQ(2u, node.size());
  ::rjson::Node::const_iterator ci = node.begin();
  EXPECT_EQ(" i t e:m{}", ci->getName());
}

