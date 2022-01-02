#include <gtest/gtest.h>
#include "../functions.hpp"

TEST(HelloTest, GtestSetup) {
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(1 + 3, 4);
}

TEST(FunctionsTest, pass_by_value) {
    int my_value = 0;
    pass_by_value(my_value);
    EXPECT_EQ(my_value, 0);
}

TEST(FunctionsTest, pass_by_reference) {
    int my_value = 0;
    pass_by_reference(my_value);
    EXPECT_EQ(my_value, 1);
}

TEST(FunctionsTest, pass_by_pointer) {
    int my_value = 0;
    pass_by_pointer(&my_value);
    EXPECT_EQ(my_value, 1);
}

TEST(FunctionsTest, pass_order_0) {
    int my_value = 0;

    pass_by_pointer(&my_value);
    EXPECT_EQ(my_value, 1);

    pass_by_value(my_value);
    EXPECT_EQ(my_value, 1);

    pass_by_reference(my_value);
    EXPECT_EQ(my_value, 2);

    pass_by_pointer(&my_value);
    EXPECT_EQ(my_value, 3);
}
