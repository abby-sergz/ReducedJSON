
CXX:=g++

BUILD_DIR=build
OUTPUT_RESULT=${BUILD_DIR}/unit_tests

GTEST_DIR=vendors/gtest/gtest/gtest-1.6.0/


build:
	@install -d ${BUILD_DIR}
	@$(MAKE) -C ./ compile
	@$(MAKE) -C ./ successmessage


SOLUTION_DIR=./

OBJECTS+=src/ConvertValue.o
OBJECTS+=src/Node.o
OBJECTS+=src/NodeImpl.o
OBJECTS+=src/Reader.o
OBJECTS+=src/Writer.o

UT_OBJECTS+=unit_tests/gtest-all.o
UT_OBJECTS+=unit_tests/main.uto
UT_OBJECTS+=unit_tests/UnitTests/TestsFromEmail.uto
UT_OBJECTS+=unit_tests/UnitTests/VariousObjects.uto
UT_OBJECTS+=unit_tests/UnitTests/ConstructObject.uto
UT_OBJECTS+=unit_tests/UnitTests/UTF8File.uto

#PASSED_CXXFLAGS:=$(CXXFLAGS) -DREDUCED_JSON_USE_BOOST
PASSED_CXXFLAGS:=$(CXXFLAGS)

INCLUDE+=-I./include -I./include_private
CXXFLAGS+=-c $(INCLUDE) ${PASSED_CXXFLAGS}
CXXFLAGS+=-Wdeprecated -Wall -pedantic -Wextra -Wno-long-long

UT_CXXFLAGS+=$(PASSED_CXXFLAGS) -c -I./include -I./unit_tests/include -I$(GTEST_DIR)/fused-src

unit_tests/gtest-all.o:
	$(CXX) $(UT_CXXFLAGS)  $(GTEST_DIR)/fused-src/gtest/gtest-all.cc -o $@

%.o:%.cpp
	@$(CXX) $(CXXFLAGS) $< -o $@

%.uto:%.cpp
	@$(CXX) $(UT_CXXFLAGS) $< -o $@

compile: $(OBJECTS) $(UT_OBJECTS)
	$(CXX) $(OBJECTS) $(UT_OBJECTS) -o ${OUTPUT_RESULT} -lpthread

clean:
	rm -rf $(OBJECTS) $(UT_OBJECTS)
	rm -rf ${BUILD_DIR}

rebuild:
	@$(MAKE) -C ./ clean
	@$(MAKE) -C ./ build

successmessage:
	@echo "*****************************************"
	@echo "*     Build successfully finished       *"
	@echo "*****************************************"








.PHONY: build clean rebuild
