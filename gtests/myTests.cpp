//
// Created by Александр Широков on 25.07.2024.
//
#include <gtest/gtest.h>


int test_hello()
{
    return 1;
}


TEST(test_hello, sample_test)
{
    EXPECT_EQ(1, 1);
}