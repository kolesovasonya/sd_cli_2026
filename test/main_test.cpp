#include "gtest/gtest.h"
#include "../main.cpp" 

TEST(GreetTest, BasicGreeting) {
    EXPECT_EQ(greet("World"), "Hello, World!");
}

TEST(GreetTest, SpecificName) {
    EXPECT_EQ(greet("John"), "Hello, John!");
}

TEST(GreetTest, EmptyName) {
    EXPECT_EQ(greet(""), "Hello, !");
}
