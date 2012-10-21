#include <iostream>
#include <string>
#include <unit_tests/unit_tests.hpp>

UNIT_TEST_REGISTER_LIBRARY(TestFromEmail)

using std::cout;
using std::endl;

TEST(TestSingleValues, TestFromEmail)
{
  {
    std::string str = "23";

    ::rjson::Node node = ::rjson::read(str);
    EXPECT_EQ(23, node.asInt64());
  }

  {
    std::string str = "\"John Doe\"";

    ::rjson::Node node = ::rjson::read(str);
    EXPECT_EQ("John Doe", node.asString());
  }

  {
    std::string str =  "{ item: \"Gadget\", price: 499.99 }";
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
    std::string str = "{ id: 2, data: { top: \"white\", bottom: \"blue\" } }";
    ::rjson::Node node = ::rjson::read(str);
    ASSERT_EQ(2u, node.size());
    ::rjson::Node::const_iterator ci = node.begin();
    EXPECT_EQ("id", ci->getName());
    EXPECT_EQ(2, ci->asInt64());
    ++ci;
    EXPECT_EQ("data", ci->getName());
    ASSERT_EQ(2u, ci->size());
    ::rjson::Node::const_iterator ci_level_1 = ci->begin();
    EXPECT_EQ("top", ci_level_1->getName());
    EXPECT_EQ("white", ci_level_1->asString());
    ++ci_level_1;
    EXPECT_EQ("bottom", ci_level_1->getName());
    EXPECT_EQ("blue", ci_level_1->asString());
    
  }

}
