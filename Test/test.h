#ifndef MYTINYSTL_TEST_H_
#define MYTINYSTL_TEST_H_

// 一个简单的单元测试框架，定义了两个类 TestCase 和 UnitTest，以及一系列用于测试的宏

#include <ctime>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include "Lib/redbud/io/color.h"

namespace mystl {
namespace test {

#define green redbud::io::state::manual << redbud::io::hfg::green
#define red redbud::io::state::manual << redbud::io::hfg::red

#if defined(_MSC_VER)
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)
#endif
}// namespace test

namespace test {

//Class TestCase:封裝單個測試用例
class TestCase {
public:
    TestCase(const char* case_name) : testcase_name(case_name) {}
    
    // 一个纯虚函数，用于测试案例
    virtual void Run() = 0;
public:
    const char* testcase_name;   //测试案例的名称
    int         nTestResult;     // 测试案例的执行结果
    double      nFailed;         // 测试失败的案例数
    double      nPassed;         // 测试通过的案例数
};

//Class UnitTest:单元测试，把所有测试案例加入到 vector 中，依次执行测试案例
class UnitTest {
public:
    //获取一个案例
    static UnitTest* GetInstance();
    // 将案例依次加入 vector
    TestCase* RegisterTestCase(TestCase* testcase);
    void Run();

public:
    TestCase* CurrentTestCase;    // 当前执行的测试案例
    double    nPassed;            // 通过案例数
    double    nFailed;            // 失败案例数
protected:
    std::vector<TestCase*> test_cases_; // 保存案例集合
};

UnitTest* UnitTest::GetInstance() {
    static UnitTest instance;
    return &instance;
}

TestCase* UnitTest::RegisterTestCase(TestCase* testcase) {
    test_cases_.push_back(testcase);
    return testcase;
}

void UnitTest::Run() {
    for (auto it : test_cases_) {
        TestCase* testcase = it;
        CurrentTestCase = testcase;
        testcase->nTestResult = 1;
        testcase->nFailed = 0;
        testcase->nPassed = 0;
        std::cout << green << "============================================\n";
        std::cout << green << " Run TestCase:" << testcase->testcase_name << "\n";
        testcase->Run();
        if (testcase->nFailed == 0) std::cout << green;
        else std::cout << red;
        std::cout << " " << testcase->nPassed << " / " << testcase->nFailed + testcase->nPassed
            << " Cases passed. (" << testcase->nPassed / (testcase->nFailed + testcase->nPassed) * 100
            << "% )\n";
        std::cout << green << " End Testcase:" << testcase->testcase_name << "\n";
        if (testcase->nTestResult) ++nPassed;
        else ++nFailed;
    }
    std::cout << green << "============================================\n";
    std::cout << green << " Total TestCase : " << nPassed + nFailed << "\n";
    std::cout << green << " Total Passed : " << nPassed << "\n";
    std::cout << red << "Total Failed : " << nFailed << "\n";
    std::cout << green << " " << nPassed << " / " << nFailed + nPassed
        << " TestCases passed. (" << nPassed / (nFailed + nPassed) * 100 << "% )\n";
}

// 测试案例的类名，替换为 test_cast_TEST
#define TESTCASE_NAME(testcase_name) testcase_name##_TEST

// 使用宏定义掩盖复杂的测试样例封装过程，把 TEXT 中的测试案例放到单元测试中
#define MYTINYSTL_TEST_(testcase_name)                          \
class TESTCASE_NAME(testcase_name) : public TestCase {          \
public:                                                         \
    TESTCASE_NAME(testcase_name)(const char* case_name)         \
        : TestCase(case_name) {};                               \
    virtual void Run();                                         \
private:                                                        \
    static TestCase* const testcase_;                           \
};                                                              \
                                                                \
TestCase* const TESTCASE_NAME(testcase_name)::testcase_ =       \
    UnitTest::GetInstance()->RegisterTestCase(                  \
        new TESTCASE_NAME(testcase_name)(#testcase_name));      \
void TESTCASE_NAME(testcase_name)::Run()
/*
Run()后边没有写实现，是为了用宏定义将测试用例放入到 Run 的实现里，例如：
TEST(AddTestDemo)
{
EXPECT_EQ(3, Add(1, 2));
EXPECT_EQ(2, Add(1, 1));
}
上述代码将 { EXPECT_EQ(3, Add(1, 2)); EXPECT_EQ(2, Add(1, 1)); } 接到 Run() 的后面
*/



/**************************************************************************/
// 简单测试的宏定义
// 断言 : 宏定义形式为 EXPECT_* ，符合验证条件的，案例测试通过，否则失败
// 使用一系列的宏来封装验证条件，分为以下几大类 :

/*
真假断言
EXPECT_TRUE  验证条件: Condition 为 true
EXPECT_FALSE 验证条件: Condition 为 false
Example:
bool isPrime(int n);         一个判断素数的函数
EXPECT_TRUE(isPrime(2));     通过
EXPECT_FALSE(isPrime(4));    通过
EXPECT_TRUE(isPrime(6));     失败
EXPECT_FALSE(isPrime(3));    失败
*/
#define EXPECT_TRUE(Condition) do {                                 \
    if (Condition) {                                                \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
        std::cout << green << " EXPECT_TRUE succeeded!\n";          \
    } else {                                                        \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
        std::cout << red << " EXPECT_TRUE failed!\n";               \
    }                                                               \
} while(0)                                                          

#define EXPECT_FALSE(Condition) do {                                \
    if (!Condition) {                                               \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
        std::cout << green << " EXPECT_FALSE succeeded!\n";         \
    }                                                               \
    else {                                                          \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
        std::cout << red << "  EXPECT_FALSE failed!\n";             \
    }                                                               \
} while(0)

/*
比较断言
EXPECT_EQ(v1, v2) 验证条件: v1 == v2
EXPECT_NE(v1, v2) 验证条件: v1 != v2
EXPECT_LT(v1, v2) 验证条件: v1 <  v2
EXPECT_LE(v1, v2) 验证条件: v1 <= v2
EXPECT_GT(v1, v2) 验证条件: v1 >  v2
EXPECT_GE(v1, v2) 验证条件: v1 >= v2

Note:
1. 参数应满足 EXPECT_*(Expect, Actual)的格式，左边是期望值，右边是实际值
2. 在断言失败时，会将期望值与实际值打印出来
3. 参数值必须是可通过断言的比较操作符进行比较的，参数值还必须支持 << 操作符来
将值输入到 ostream 中
4. 这些断言可以用于用户自定义型别，但必须重载相应的比较操作符（如 == 、< 等）
5. EXPECT_EQ 对指针进行的是地址比较。即比较的是它们是否指向相同的内存地址，
而不是它们指向的内容是否相等。如果想比较两个 C 字符串(const char*)的值，
请使用 EXPECT_STREQ 。特别一提的是，要验证一个 C 字符串是否为空(NULL)，
请使用 EXPECT_STREQ(NULL, c_str)。但是要比较两个 string 对象时，
应该使用 EXPECT_EQ

Example:
EXPECT_EQ(3, foo());
EXPECT_NE(NULL, pointer);
EXPECT_LT(len, v.size());
*/
#define EXPECT_EQ(v1, v2) do {                                      \
    if (v1 == v2) {                                                 \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
        std::cout << green << " EXPECT_EQ succeeded!\n";            \
    } else {                                                        \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
        std::cout << red << " EXPECT_EQ failed!\n";                 \
        std::cout << red << " Expect:" << v1 << "\n";               \
        std::cout << red << " Actual:" << v2 << "\n";               \
    }                                                               \
} while(0)  

#define EXPECT_NE(v1, v2) do {                                      \
    if (v1 != v2) {                                                 \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
        std::cout << green << " EXPECT_NE succeeded!\n";            \
    } else {                                                        \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
        std::cout << red << " EXPECT_NE failed!\n";                 \
        std::cout << red << " Expect:" << v1 << "\n";               \
        std::cout << red << " Actual:" << v2 << "\n";               \
    }                                                               \
} while(0)

#define EXPECT_LT(v1, v2) do {                                      \
    if (v1 < v2) {                                                  \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
        std::cout << green << " EXPECT_LT succeeded!\n";            \
    } else {                                                        \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
        std::cout << red << " EXPECT_LT failed!\n";                 \
        std::cout << red << " Expect:" << v1 << "\n";               \
        std::cout << red << " Actual:" << v2 << "\n";               \
    }                                                               \
} while(0)

#define EXPECT_LE(v1, v2) do {                                      \
    if (v1 <= v2) {                                                 \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
        std::cout << green << " EXPECT_LE succeeded!\n";            \
    } else {                                                        \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
        std::cout << red << " EXPECT_LE failed!\n";                 \
        std::cout << red << " Expect:" << v1 << "\n";               \
        std::cout << red << " Actual:" << v2 << "\n";               \
    }                                                               \
} while(0)

#define EXPECT_GT(v1, v2) do {                                      \
    if (v1 > v2) {                                                  \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
        std::cout << green << " EXPECT_GT succeeded!\n";            \
    } else {                                                        \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
        std::cout << red << " EXPECT_GT failed!\n";                 \
        std::cout << red << " Expect:" << v1 << "\n";               \
        std::cout << red << " Actual:" << v2 << "\n";               \
    }                                                               \
} while(0)

#define EXPECT_GE(v1, v2) do {                                      \
    if (v1 >= v2) {                                                 \
        UnitTest::GetInstance()->CurrentTestCase->nPassed++;        \
        std::cout << green << " EXPECT_GE succeeded!\n";            \
    } else {                                                        \
        UnitTest::GetInstance()->CurrentTestCase->nTestResult = 0;  \
        UnitTest::GetInstance()->CurrentTestCase->nFailed++;        \
        std::cout << red << " EXPECT_GE failed!\n";                 \
        std::cout << red << " Expect:" << v1 << "\n";               \
        std::cout << red << " Actual:" << v2 << "\n";               \
    }                                                               \
} while(0)

}// namespace test
}// namespace mystl


#endif //! MYTINYSTL_TEST_H_