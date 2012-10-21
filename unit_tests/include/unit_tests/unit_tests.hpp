#pragma once

#include <gtest/gtest.h>
#include <rjson/rjson.hpp>

#ifndef UNIT_TEST_PULL_MY_LIBRARY
#define UNIT_TEST_PULL_MY_LIBRARY(LibName) namespace LibName{int PullInLib();}namespace{static int dummy_##LibName = LibName::PullInLib();}
#endif // UNIT_TEST_PULL_MY_LIBRARY

#ifndef UNIT_TEST_REGISTER_LIBRARY
#define UNIT_TEST_REGISTER_LIBRARY(LibName) namespace LibName{ int PullInLib() {return 0;} }
#endif // UNIT_TEST_REGISTER_LIBRARY