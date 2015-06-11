#include <iostream>
#include <gtest/gtest.h>
#include <vector>

int Factorial(int n){
    if (n == 0) return 1;
    else return n*Factorial(n-1);
}; // Returns the factorial of n

    // Tests factorial of 0.
    TEST(FactorialTest, HandlesZeroInput) {
        EXPECT_EQ(1, Factorial(0));
    }
    // Tests factorial of positive numbers.
    TEST(FactorialTest, HandlesPositiveInput) {
        EXPECT_EQ(1, Factorial(1));
        EXPECT_EQ(2, Factorial(2));
        EXPECT_EQ(6, Factorial(3));
        EXPECT_EQ(40320, Factorial(8));
    }

//int main()
//{
//EXPECT_EQ(7, Factorial(1));
//}
