GTEST_INC_DIR=/opt/googletest/googletest/include
GTEST_LIB_DIR=/opt/googletest/build/lib

SRCS = ../util/Utils.cpp
GTEST_SRCS = NetworkProbe_Test.cpp
all:
	g++ -o NetworkProbe_Test $(GTEST_SRCS) $(SRCS) -isystem ${GTEST_INC_DIR} -L${GTEST_LIB_DIR} -pthread -lgtest
	./NetworkProbe_Test
