#include <iostream>
#ifdef WIN32
#include <conio.h>
#endif
#include <unit_tests/unit_tests.hpp>

UNIT_TEST_PULL_MY_LIBRARY(TestFromEmail)
UNIT_TEST_PULL_MY_LIBRARY(VariousObjects)
UNIT_TEST_PULL_MY_LIBRARY(ConstructObject)
//UNIT_TEST_PULL_MY_LIBRARY(UTF8RussianFile)

int
main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  int ret = RUN_ALL_TESTS();
  //getch();
  return ret;
}
