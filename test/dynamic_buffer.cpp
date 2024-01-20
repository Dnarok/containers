#include <gtest/gtest.h>
#include "dynamic_buffer.hpp"

TEST(DynamicBuffer, DefaultConstruction)
{
    static_assert(std::is_default_constructible_v<dynamic_buffer<int>>);
    dynamic_buffer<int> buffer{};

    EXPECT_EQ(buffer.size, 0);
    EXPECT_EQ(buffer.data, nullptr);
};

TEST(DynamicBuffer, InitializerListConstruction)
{
    static_assert(std::is_constructible_v<dynamic_buffer<int>, std::initializer_list<int>>);
    dynamic_buffer<int> buffer = { 0, 1, 2, 3, 4, 5 };

    EXPECT_EQ(buffer.size, 6);
    EXPECT_NE(buffer.data, nullptr);

    for (std::size_t i = 0; i < buffer.size; ++i)
    {
        EXPECT_EQ(buffer.data[i], i);
    }
};

TEST(DynamicBuffer, CopyConstruction)
{
    static_assert(std::is_copy_constructible_v<dynamic_buffer<int>>);
    dynamic_buffer<int> buffer1 = { 4, 3, 2, 1, 0 };
    dynamic_buffer<int> buffer2{ buffer1 };

    EXPECT_EQ(buffer2.size, buffer1.size);
    EXPECT_NE(buffer2.data, nullptr);
    EXPECT_NE(buffer2.data, buffer1.data);

    for (std::size_t i = 0; i < buffer2.size; ++i)
    {
        EXPECT_EQ(buffer2.data[i], buffer1.data[i]);
    }
};

TEST(DynamicBuffer, MoveConstruction)
{
    static_assert(std::is_move_constructible_v<dynamic_buffer<int>>);
    dynamic_buffer<int> buffer1 = { 10, 20, 30, 40, 50, 60 };
    dynamic_buffer<int> buffer2{ std::move(buffer1) };

    EXPECT_EQ(buffer1.size, 0);
    EXPECT_EQ(buffer1.data, nullptr);
    EXPECT_EQ(buffer2.size, 6);
    EXPECT_NE(buffer2.data, nullptr);

    for (std::size_t i = 0; i < buffer2.size; ++i)
    {
        EXPECT_EQ(buffer2.data[i], (i + 1) * 10);
    }
};

TEST(DynamicBuffer, SizeConstruction)
{
    static_assert(std::is_constructible_v<dynamic_buffer<int>, std::size_t>);
    dynamic_buffer<int> buffer(5);

    EXPECT_EQ(buffer.size, 5);
    EXPECT_NE(buffer.data, nullptr);

    for (std::size_t i = 0; i < buffer.size; ++i)
    {
        EXPECT_EQ(buffer.data[i], int{});
    }
};

TEST(DynamicBuffer, SizeWithArgumentsConstruction)
{
    static_assert(std::is_constructible_v<dynamic_buffer<int>, std::size_t, int>);
    dynamic_buffer<int> buffer(5, 10);

    EXPECT_EQ(buffer.size, 5);
    EXPECT_NE(buffer.data, nullptr);

    for (std::size_t i = 0; i < buffer.size; ++i)
    {
        EXPECT_EQ(buffer.data[i], 10);
    }
};

TEST(DynamicBuffer, UninitializedConstruction)
{
    static_assert(std::is_constructible_v<dynamic_buffer<int>, uninitialized_t, std::size_t>);
    dynamic_buffer<int> buffer(uninitialized, 5);

    EXPECT_EQ(buffer.size, 5);
    EXPECT_NE(buffer.data, nullptr);
};

TEST(DynamicBuffer, CopyAssignment)
{
    static_assert(std::is_copy_assignable_v<dynamic_buffer<int>>);
    dynamic_buffer<int> buffer1 = { 0, 2, 4, 6, 8, 10 };
    dynamic_buffer<int> buffer2{};
    buffer2 = buffer1;

    EXPECT_EQ(buffer2.size, buffer1.size);
    EXPECT_NE(buffer2.data, nullptr);
    EXPECT_NE(buffer2.data, buffer1.data);

    for (std::size_t i = 0; i < buffer2.size; ++i)
    {
        EXPECT_EQ(buffer2.data[i], buffer1.data[i]);
    }
};

TEST(DynamicBuffer, MoveAssignment)
{
    static_assert(std::is_move_assignable_v<dynamic_buffer<int>>);
    dynamic_buffer<int> buffer1 = { 0, 2, 4, 6, 8 };
    int* buffer1_data = buffer1.data;
    dynamic_buffer<int> buffer2 = {};
    buffer2 = std::move(buffer1);

    EXPECT_EQ(buffer2.size, 5);
    EXPECT_NE(buffer2.data, nullptr);
    EXPECT_EQ(buffer2.data, buffer1_data);

    for (std::size_t i = 0; i < buffer2.size; ++i)
    {
        EXPECT_EQ(buffer2.data[i], i * 2);
    }
};

TEST(DynamicBuffer, Swap)
{
    static_assert(std::is_swappable_v<dynamic_buffer<int>>);
    dynamic_buffer<int> buffer1 = { 0, 1, 2, 3, 4 };
    std::size_t buffer1_size = buffer1.size;
    int* buffer1_data = buffer1.data;
    dynamic_buffer<int> buffer2 = { 5, 6, 7, 8, 9, 10 };
    std::size_t buffer2_size = buffer2.size;
    int* buffer2_data = buffer2.data;

    swap(buffer1, buffer2);

    EXPECT_EQ(buffer1.size, buffer2_size);
    EXPECT_EQ(buffer2.size, buffer1_size);
    EXPECT_EQ(buffer1.data, buffer2_data);
    EXPECT_EQ(buffer2.data, buffer1_data);
};

TEST(DynamicBuffer, Subscript)
{
    static_assert(requires (dynamic_buffer<int>&A) { A[0]; });
    dynamic_buffer<int> buffer = { 0, 2, 4, 6, 8, 10 };

    for (std::size_t i = 0; i < buffer.size; ++i)
    {
        EXPECT_EQ(buffer[i], i * 2);
    }

    buffer[2] = 1000;
    EXPECT_EQ(*(buffer.data + 2), 1000);
};

TEST(DynamicBuffer, EqualityComparison)
{
    static_assert(std::equality_comparable<dynamic_buffer<int>>);
    dynamic_buffer<int> buffer1 = { 0, 1, 2, 3, 4, 5 };
    dynamic_buffer<int> buffer2 = { 0, 1, 2, 3, 4, 5 };
    dynamic_buffer<int> buffer3 = { 0, 1, 2, 3, 5, 4 };

    EXPECT_EQ(buffer1, buffer2);
    EXPECT_NE(buffer1, buffer3);
    EXPECT_NE(buffer2, buffer3);
};

TEST(DynamicBuffer, Spaceship)
{
    static_assert(std::three_way_comparable<dynamic_buffer<int>>);
    dynamic_buffer<int> buffer1 = { 0, 1, 2, 3, 4, 5 };
    dynamic_buffer<int> buffer2 = { 0, 1, 2, 3, 4, 5 };
    dynamic_buffer<int> buffer3 = { 0, 1, 2, 3, 4 };
    dynamic_buffer<int> buffer4 = { 0, 1, 2, 3, 4, 5, 6 };
    dynamic_buffer<int> buffer5 = { 0, 1, 2, 3, 5, 5 };
    dynamic_buffer<int> buffer6 = { 0, 0, 0, 0, 0, 0 };
    dynamic_buffer<int> buffer7{};

    EXPECT_EQ(buffer1 <=> buffer2, std::partial_ordering::equivalent);
    EXPECT_EQ(buffer1 <=> buffer3, std::partial_ordering::greater);
    EXPECT_EQ(buffer1 <=> buffer4, std::partial_ordering::less);
    EXPECT_EQ(buffer1 <=> buffer5, std::partial_ordering::less);
    EXPECT_EQ(buffer1 <=> buffer6, std::partial_ordering::greater);
    EXPECT_EQ(buffer1 <=> buffer7, std::partial_ordering::unordered);
};

TEST(DynamicBuffer, Resize)
{
    static_assert(requires (dynamic_buffer<int>& A) { A.resize(std::size_t{}); });
    dynamic_buffer<int> buffer1 = { 0, 1, 2, 3, 4, 5 };
    dynamic_buffer<int> buffer2{ buffer1 };
    int* buffer2_data = buffer2.data;
    dynamic_buffer<int> buffer3 = { 0, 1, 2, 3, 4, 5, 0, 0, 0 };
    
    buffer2.resize(9);
    EXPECT_EQ(buffer2.size, 9);
    EXPECT_NE(buffer2.data, buffer2_data);
    EXPECT_NE(buffer2, buffer1);
    EXPECT_EQ(buffer2, buffer3);
};

TEST(DynamicBuffer, ResizeWithArguments)
{
    static_assert(requires (dynamic_buffer<int>& A) { A.resize(std::size_t{}, int{});});
    dynamic_buffer<int> buffer1 = { 0, 1, 2, 3, 4, 5 };
    dynamic_buffer<int> buffer2{ buffer1 };
    int* buffer2_data = buffer2.data;
    dynamic_buffer<int> buffer3 = { 0, 1, 2, 3, 4, 5, 1000, 1000, 1000 };

    buffer2.resize(9, 1000);
    EXPECT_EQ(buffer2.size, 9);
    EXPECT_NE(buffer2.data, buffer2_data);
    EXPECT_NE(buffer2, buffer1);
    EXPECT_EQ(buffer2, buffer3);
};

TEST(DynamicBuffer, ResizeUninitialized)
{
    static_assert(requires (dynamic_buffer<int>& A) { A.resize(uninitialized_t{}, std::size_t{}); });
    dynamic_buffer<int> buffer1 = { 0, 1, 2, 3, 4, 5 };
    dynamic_buffer<int> buffer2 = { buffer1 };
    int* buffer2_data = buffer2.data;
    std::size_t buffer2_size = buffer2.size;

    buffer2.resize(uninitialized, 9);
    EXPECT_EQ(buffer2.size, 9);
    EXPECT_NE(buffer2.data, buffer2_data);

    for (std::size_t i = 0; i < buffer2_size; ++i)
    {
        EXPECT_EQ(buffer2[i], i);
    }

    // no real way to check "are the last three uninitialized memory."
};