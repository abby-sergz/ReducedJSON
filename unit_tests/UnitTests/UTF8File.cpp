#include <fstream>
#include <unit_tests/unit_tests.hpp>


UNIT_TEST_REGISTER_LIBRARY(UTF8RussianFile)

using std::fstream;
using ::rjson::Node;

TEST(RWFile, UTF8RussianFile)
{
  fstream fin("./UTF8RussianFile.test", ::std::ios::binary | ::std::ios::in);
  fstream fout("./UTF8RussianFile.test.out", ::std::ios::binary| ::std::ios::trunc | ::std::ios::out);
  bool x = (fin.good() && fout.good());
  EXPECT_TRUE(x);
  if (x)
  {
    Node n = ::rjson::read(fin);
    write(n, fout);
  }
  
  fout.close();
  fin.close();
}