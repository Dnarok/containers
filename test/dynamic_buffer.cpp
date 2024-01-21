#include <gtest/gtest.h>
#include "containers/dynamic_buffer.hpp"

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
    static_assert(requires (dynamic_buffer<int>&A) { A.resize(std::size_t{}); });
    dynamic_buffer<int> buffer1 = { 0, 1, 2, 3, 4, 5 };
    dynamic_buffer<int> buffer2{ buffer1 };
    int* buffer2_data = buffer2.data;
    dynamic_buffer<int> buffer3 = { 0, 1, 2, 3, 4, 5, 0, 0, 0 };

    buffer2.resize(9);
    EXPECT_EQ(buffer2.size, 9);
    EXPECT_NE(buffer2.data, buffer2_data);
    EXPECT_NE(buffer2, buffer1);
    EXPECT_EQ(buffer2, buffer3);

    dynamic_buffer<int> buffer4 = { 0, 1, 2, 3 };
    buffer4.resize(0);
    EXPECT_EQ(buffer4, dynamic_buffer<int>{});
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

TEST(DynamicBuffer, ClaimPointer)
{
    static_assert(std::is_constructible_v<dynamic_buffer<int>, int*, std::size_t>);
    int* pointer = new int[3] { 123, 124, 125 };
    dynamic_buffer<int> buffer{ pointer, 3 };

    EXPECT_EQ(buffer.size, 3);
    EXPECT_EQ(buffer.data, pointer);
    
    for (std::size_t i = 0; i < buffer.size; ++i)
    {
        EXPECT_EQ(buffer[i], 123 + i);
    }
};

TEST(DynamicBufferIterator, InputIterator)
{
    static_assert(std::input_iterator<dynamic_buffer_iterator<int>>);
    dynamic_buffer<int> buffer = { 0, 1, 2, 3, 4, 5 };
    dynamic_buffer_iterator<int> iterator1{ buffer.data };

    // std::weakly_incrementable
    //     std::movable
    //         std::move_constructible
    dynamic_buffer_iterator<int> iterator2{ std::move(iterator1) };
    EXPECT_EQ(iterator2.data, buffer.data);

    //         std::assignable_from<T>
    dynamic_buffer_iterator<int> iterator3{};
    iterator3 = dynamic_buffer_iterator<int>{ buffer.data };
    EXPECT_EQ(iterator3.data, buffer.data);

    //         std::swappable
    dynamic_buffer_iterator<int> iterator4{ buffer.data + 1 };
    swap(iterator3, iterator4);
    EXPECT_EQ(iterator3.data, buffer.data + 1);

    //     ++i -> iterator&
    dynamic_buffer_iterator<int> iterator5{ buffer.data };
    int* iterator5_data = iterator5.data;
    dynamic_buffer_iterator<int>& iterator5_reference = ++iterator5;
    ++iterator5_reference;
    EXPECT_EQ(iterator5.data, iterator5_data + 2);
    EXPECT_EQ(std::addressof(iterator5), std::addressof(iterator5_reference));

    //     i++ -> iterator
    dynamic_buffer_iterator<int> iterator6{ buffer.data };
    EXPECT_EQ((iterator6++).data, buffer.data);
    EXPECT_EQ(iterator6.data, buffer.data + 1);

    // *i -> reference
    dynamic_buffer_iterator<int> iterator7{ buffer.data };
    int& iterator7_value = *iterator7;
    EXPECT_EQ(iterator7_value, 0);
    iterator7_value = 10;
    EXPECT_EQ(buffer[0], 10);
    ++iterator7;
    EXPECT_EQ(*iterator7, 1);
    *iterator7 = 11;
    EXPECT_EQ(buffer[1], 11);
};

TEST(DynamicBufferIterator, ForwardIterator)
{
    static_assert(std::forward_iterator<dynamic_buffer_iterator<int>>);
    dynamic_buffer<int> buffer = { 0, 1, 2, 3, 4, 5, 6 };
    dynamic_buffer_iterator<int> iterator1{ buffer.data };

    // std::regular
    //     std::copyable
    //         std::copy_constructible
    dynamic_buffer_iterator<int> iterator2{ iterator1 };
    EXPECT_EQ(iterator2.data, iterator1.data);

    //         std::default_initializable
    //         std::assignable_from<const T&>
    dynamic_buffer_iterator<int> iterator3{};
    EXPECT_EQ(iterator3.data, nullptr);
    iterator3 = iterator2;
    EXPECT_EQ(iterator3.data, iterator2.data);

    //     std::equality_comparable
    EXPECT_EQ(iterator3, iterator2);
};

TEST(DynamicBufferIterator, BidirectionalIterator)
{
    static_assert(std::bidirectional_iterator<dynamic_buffer_iterator<int>>);
    dynamic_buffer<int> buffer = { 0, 1, 2, 3, 4, 5 };

    // --i -> iterator&
    dynamic_buffer_iterator<int> iterator1{ buffer.data + 3 };
    int* iterator1_data = iterator1.data;
    dynamic_buffer_iterator<int>& iterator1_reference = --iterator1;
    --iterator1_reference;
    EXPECT_EQ(iterator1.data, iterator1_data - 2);
    EXPECT_EQ(std::addressof(iterator1), std::addressof(iterator1_reference));
    
    // i-- -> iterator
    dynamic_buffer_iterator<int> iterator2{ buffer.data + 1 };
    EXPECT_EQ((iterator2--).data, buffer.data + 1);
    EXPECT_EQ(iterator2.data, buffer.data);
};

TEST(DynamicBufferIterator, RandomAccessIterator)
{
    static_assert(std::random_access_iterator<dynamic_buffer_iterator<int>>);
    dynamic_buffer<int> buffer = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // i <=> j -> strong_ordering
    dynamic_buffer_iterator<int> iterator1{ buffer.data + 2 };
    dynamic_buffer_iterator<int> iterator2{ buffer.data + 6 };
    EXPECT_EQ(iterator1 <=> iterator2, std::strong_ordering::less);
    EXPECT_EQ(iterator2 <=> iterator1, std::strong_ordering::greater);
    EXPECT_EQ(iterator1 <=> dynamic_buffer_iterator<int>{}, std::strong_ordering::greater);

    // i - j -> difference_type
    EXPECT_EQ(iterator1 - iterator2, -4);
    EXPECT_EQ(iterator2 - iterator1, 4);

    // i += n -> iterator&
    dynamic_buffer_iterator<int> iterator3{ buffer.data };
    dynamic_buffer_iterator<int>& iterator3_reference = iterator3 += 3;
    EXPECT_EQ(*iterator3, 3);
    EXPECT_EQ(*iterator3_reference, *iterator3);
    iterator3 += -3;
    EXPECT_EQ(*iterator3, 0);

    // i + n -> iterator
    dynamic_buffer_iterator<int> iterator4{ buffer.data };
    EXPECT_EQ(*(iterator4 + 3), 3);
    EXPECT_EQ(*iterator4, 0);
    EXPECT_EQ(*(3 + iterator4), *(iterator4 + 3));

    // i -= n -> iterator&
    dynamic_buffer_iterator<int> iterator5{ buffer.data + 3};
    dynamic_buffer_iterator<int>& iterator5_reference = iterator5 -= 3;
    EXPECT_EQ(*iterator5, 0);
    EXPECT_EQ(*iterator5_reference, *iterator5);
    iterator5 -= -3;
    EXPECT_EQ(*iterator5, 3);

    // i - n -> iterator
    dynamic_buffer_iterator<int> iterator6{ buffer.data + 5 };
    EXPECT_EQ(*(iterator6 - 3), 2);
    EXPECT_EQ(*iterator6, 5);

    // i[n] -> reference
    dynamic_buffer_iterator<int> iterator7{ buffer.data };
    for (std::size_t i = 0; i < buffer.size; ++i)
    {
        EXPECT_EQ(iterator7[i], i);
    }
    iterator7[0] = 3;
    EXPECT_EQ(buffer[0], 3);
    iterator7[2] = 500;
    EXPECT_EQ(buffer[2], 500);
};

TEST(DynamicBufferIterator, ContiguousIterator)
{
    struct value
    {
        int x;
    };

    static_assert(std::contiguous_iterator<dynamic_buffer_iterator<value>>);
    dynamic_buffer<value> buffer{ value{ 0 }, value{ 1 }, value{ 2 }, value{ 3 } };
    dynamic_buffer_iterator<value> iterator{ buffer.data };
    EXPECT_EQ(iterator->x, 0);
    EXPECT_EQ((iterator + 2)->x, 2);
    ++iterator;
    EXPECT_EQ(iterator->x, 1);
    EXPECT_EQ((iterator + 2)->x, 3);
};