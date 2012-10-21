* About
=======
  This project is a test exercise.

* How to build and run unit_tests (Microsoft Visual Studio)
===========================================================
  - Download boost from http://boost.org. I prefer to keep all third party libraries near the project, but in this particular case I guess it is acceptable to omit boost in the repo to reduce project size.
  - Extract boost into some directory X. Let the "X/boost/shared_ptr.hpp" is a correct path.
  - Google test framework is already downloaded (from http://code.google.com/p/googletest/) into vendors/gtest
  - Extract gtest into some dir Y (I used "vendors/gtest/gtest" dir). Ensure that the following path is correct Y/fused-src/gtest/gtest.h (in my case it is vendors\gtest\gtest\gtest-1.6.0\fused-src\gtest/gtest.h).
  - Configure paths for projects:
    If you use MSVS 2008:
      Edit file msvs/vsprops/boost.directory.vsprops with a text editor or in MS Visual Studio 2008 (see note about "how to configure vsprops"). Set value of `AdditionalIncludeDirectories` to X.
      Edit file msvs/vsprops/gtest_common.vsprops with a text editor or in MS Visual Studio 2008 (see note about "how to configure vsprops"). Set value of "UserMacros" `gtest_directory` to Y.
    If you use MSVS 2010:
      Edit file msvs/vsprops/boost.directory.props ("props" NOT "vsprops" as for MSVS 2008) with a text editor or in MS Visual Studio 2008 (see note about "how to configure vsprops"). Set value of `AdditionalIncludeDirectories` to X.
      Edit file msvs/vsprops/gtest_common.props with a text editor or in MS Visual Studio 2008 (see note about "how to configure vsprops"). Set value of "UserMacros" `gtest_directory` to Y.
  - Now open solution, build and run.
    
* How to build and run unit_tests (linux and other)
===================================================
  If your clint code does not use boost and uses c++11 features (shared_ptr and make_shared)
    CXXFLAGS=-std=c++0x make rebuild
  else
    CXXFLAGS=-DREDUCED_JSON_USE_BOOST make rebuild

  If boost is unpacked to some directory XXX don't forget to add this XXX as include search directory
  CXXFLAGS="-std=c++0x -I/XXX" make rebuild

  Run: ./build/unit_tests 


* Note: how to configure vsprops
================================
  Go to menu `View`->`Other Windows`->`Property Manager`. Please, take into account that properties specified for $(Configuration) can be edited in `$(Configuration)|$(Platform)` properties set.
  For example for Debug Win32 related properties are stored in `Debug|Win32` "properties card".
  `common` properties are used by all configurations. If you change some settings in `common` or other vsprops included by `common` then the changes will be applied for all configurations. I recommend you after any changing of settings to reopen solution, thus close it and then open.
  Also before committing changes into some VCS, please, close the solution to ensure that everything is saved to the filesystem.

* Note: Using of boost
======================
  - Using of boost::variant
    It's the fastest way to implement reliable various type holder. There are few limitations but this type is hidden by private implementation, so the client code of this library does not directly depend on boost::variant. In this case boost::variant can be easily replaced by something else.
  - If your client code uses C++11 features it can be independent of boost (for example unit_tests for MSVS 2010 does not depend on boost). If client code does not support c++11 features (actually it's only shared_ptr and make_shared) then you have to globally define REDUCED_JSON_USE_BOOST (via compiler flag for preprocessor or in precompiled header).


* Note: Header include order
============================
  1. std
  2. rjson/base.hpp
  3. vendors
  4. rjson/*.hpp

* Note: include and include_private
===================================
  `include` is a folder with headers for user of this library. It contains all definitions required to use this lib in other projects.
  `include_private` is used only by this library.
  
* Note: default value of Node
=============================
  It's too inconvenient without `undefined` type. So I introduced `undefined` type with value `undefined`.
  
* Note: about optimization of structure
======================================
  I would like to have dummy structures without specific behaviour. So anybody can use them without reading the documentation.
  It means that deep copy ctr is preferable instead of using `clone` methods.
  
* Note: #pragma once
====================
  Almost all contemporary compilers efficiently support this directive and using `#ifndef ...` guards is error-prone.

* Note UNIT_TEST_REGISTER_LIBRARY and UNIT_TEST_PULL_MY_LIBRARY
===============================================================
  If you compile dir unit_tests/UnitTests as separate lib then some linkers do not link this lib (or object file). So no tests will be run.

* Note: Not implemented
=======================
  - parsing/serialization of `undefined` value
  - support of object property name surrounded by quotes {"property name" : 10}
  - support of \uXXXX symbols
  - and many many things
