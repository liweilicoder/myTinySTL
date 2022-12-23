#ifndef MYTINYSTL_ALGORITHM_TEST_H_
#define MYTINYSTL_ALGORITHM_TEST_H_

#include <algorithm>
#include <functional>
#include <numeric>
#include "test.h"

namespace mystl {
namespace test {
namespace algorithm_test{
TEST(first)
{
    EXPECT_NE(2, 3);
    EXPECT_EQ(2, 4);
}

TEST(second)
{
    EXPECT_GT(2, 1);
    EXPECT_STRNE("123", "123");
}

} // namespace algorithm_test
} // namespace test
} // namespace mystl
#endif // !MYTINYSTL_ALGORITHM_TEST_H_
