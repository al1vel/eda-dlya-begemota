#include <gtest/gtest.h>
#include "main.cpp"
#include <exception>
#include <iostream>

class TestFoo : public ::testing::Test {

protected:

    Summator* summator_ptr = nullptr;
    void SetUp() override{
        std::cout << "TestFoo::SetUp" << std::flush << std::endl;
        summator_ptr = new Summator();
    }

    void TearDown() override{
        std::cout << "TestFoo::TearDown" << std::flush << std::endl;
        delete summator_ptr;
    }
};

TEST_F(TestFoo, test_add) {
    ASSERT_EQ(this->summator_ptr->sum(1, 2), 3);
}

TEST_F(TestFoo, test_div_throw) {
    ASSERT_THROW(this->summator_ptr->div(1, 0), std::exception);
}

TEST_F(TestFoo, test_div) {
    ASSERT_EQ(this->summator_ptr->div(1, 1), 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}