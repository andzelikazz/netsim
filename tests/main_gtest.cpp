#include "gtest/gtest.h"
#include "gmock/gmock.h"

int main_tests(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}