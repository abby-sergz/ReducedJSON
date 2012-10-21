#include <iostream>
#include <unit_tests/unit_tests.hpp>

UNIT_TEST_REGISTER_LIBRARY(ConstructObject)

using ::rjson::Node;
using ::rjson::ENodeType;

namespace
{
  const int isWrite = 0;
}


TEST(OneValueType, ConstructObject)
{
  {
    Node node;
    EXPECT_EQ(ENodeType::kUNDEFINED, node.getType());
  }

  {
    Node node;
    int64_t v = 10;
    node = v;
    EXPECT_EQ(ENodeType::kINTEGER, node.getType());
    EXPECT_EQ(v, node);
  }

  {
    Node node;
    double v = 10;
    node = v;
    EXPECT_EQ(ENodeType::kDOUBLE, node.getType());
    EXPECT_EQ(v, node);
  }

  {
    Node node;
    std::string v = "xxx";
    node = v;
    EXPECT_EQ(ENodeType::kSTRING, node.getType());
    EXPECT_EQ(v, node);
  }
}


TEST(ObjectValueType, ConstructObject)
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

  ASSERT_EQ(2u, item.size());
  EXPECT_EQ(name, *item.begin());
  EXPECT_EQ(price, *(++item.begin()));

}


