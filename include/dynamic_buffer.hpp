#pragma once

#include <memory>
#include <initializer_list>
#include <compare>

#include "contract.hpp"

/*
    A buffer whose length is decided at runtime.
    Can only change size when explicitly resized, and will always reallocate.
*/

struct uninitialized_t
{
    explicit uninitialized_t() = default;
};
constexpr inline uninitialized_t uninitialized{};

template <typename T, typename A = std::allocator<T>>
struct dynamic_buffer
{
    using traits = std::allocator_traits<A>;
    using allocator_type = typename traits::allocator_type;
    using value_type = typename traits::value_type;
    using size_type = typename traits::size_type;
    using pointer = typename traits::pointer;

    size_type size = 0;
    [[no_unique_address]] allocator_type allocator = {};
    pointer data = nullptr;

    constexpr dynamic_buffer() = default;
    constexpr dynamic_buffer(const dynamic_buffer& other);
    constexpr dynamic_buffer(dynamic_buffer&& other) noexcept;
    constexpr ~dynamic_buffer();
    constexpr auto operator =(dynamic_buffer other) noexcept -> dynamic_buffer&;

    constexpr dynamic_buffer(std::initializer_list<value_type> init);
    template <typename... Args>
    constexpr dynamic_buffer(size_type size, Args&&... arguments);
    constexpr dynamic_buffer(uninitialized_t, size_type size);

    constexpr auto operator [](size_type index)	      & -> value_type&;
    constexpr auto operator [](size_type index) const & -> const value_type&;
    constexpr auto operator [](size_type index)      && -> value_type&&;

    template <typename... Args>
    constexpr auto resize(size_type size, Args&&... arguments) -> void;
    constexpr auto resize(uninitialized_t, size_type size) -> void;
};

template <typename T, typename A>
constexpr auto swap(dynamic_buffer<T, A>& left, dynamic_buffer<T, A>& right) noexcept -> void
{
    using std::swap;

    swap(left.size, right.size);
    swap(left.data, right.data);

    if constexpr (dynamic_buffer<T, A>::traits::propagate_on_container_swap::value)
    {
        swap(left.allocator, right.allocator);
    }
};

template <typename T, typename A>
constexpr dynamic_buffer<T, A>::dynamic_buffer(const dynamic_buffer& other) :
    size{ other.size },
    allocator{ traits::select_on_container_copy_construction(other.allocator) },
    data{ traits::allocate(allocator, size) }
{
    contract;
        post(size == other.size);

    cond(data ? size > 0 : size == 0);
    for (size_t i = 0; i < size; ++i)
    {
        traits::construct(allocator, data + i, *(other.data + i));
    }
};

template <typename T, typename A>
constexpr dynamic_buffer<T, A>::dynamic_buffer(dynamic_buffer&& other) noexcept :
    dynamic_buffer{}
{
    swap(*this, other);
};

template <typename T, typename A>
constexpr dynamic_buffer<T, A>::~dynamic_buffer()
{
    contract;
        post(size == 0);
        post(data == nullptr);

    for (size_t i = 0; i < size; ++i)
    {
        traits::destroy(allocator, data + i);
    }

    traits::deallocate(allocator, data, size);
    size = 0;
    data = nullptr;
};

template <typename T, typename A>
constexpr auto dynamic_buffer<T, A>::operator =(dynamic_buffer other) noexcept -> dynamic_buffer&
{
    swap(*this, other);
    return *this;
};

template <typename T, typename A>
constexpr dynamic_buffer<T, A>::dynamic_buffer(std::initializer_list<value_type> init) :
    size{ init.size() },
    allocator{},
    data{ traits::allocate(allocator, size) }
{
    contract;
        post(size == init.size());

    for (size_type i = 0; i < size; ++i)
    {
        traits::construct(allocator, data + i, *(init.begin() + i));
    }
};

template <typename T, typename A>
template <typename... Args>
constexpr dynamic_buffer<T, A>::dynamic_buffer(size_type size, Args&&... arguments) :
    size{ size },
    allocator{},
    data{ traits::allocate(allocator, size) }
{
    for (size_type i = 0; i < size; ++i)
    {
        traits::construct(allocator, data + i, std::forward<Args>(arguments)...);
    }
};

template <typename T, typename A>
constexpr dynamic_buffer<T, A>::dynamic_buffer(uninitialized_t, size_type size) :
    size{ size },
    allocator{},
    data{ traits::allocate(allocator, size) }
{};

template <typename T, typename A>
constexpr auto dynamic_buffer<T, A>::operator [](size_type index) & -> value_type&
{
    contract;
        pre(index < size);

    return data[index];
};

template <typename T, typename A>
constexpr auto dynamic_buffer<T, A>::operator [](size_type index) const& -> const value_type&
{
    contract;
        pre(index < size);

    return data[index];
};

template <typename T, typename A>
constexpr auto dynamic_buffer<T, A>::operator [](size_type index) && -> value_type&&
{
    contract;
        pre(index < size);

    return std::move(data[index]);
};

template <typename T, typename A>
constexpr auto operator ==(const dynamic_buffer<T, A>& left, const dynamic_buffer<T, A>& right) noexcept -> bool
{
    if (left.size != right.size)
    {
        return false;
    }

    for (std::size_t i = 0; i < left.size; ++i)
    {
        if (left[i] != right[i])
        {
            return false;
        }
    }

    return true;
};

template <typename T, typename A>
constexpr auto operator <=>(const dynamic_buffer<T, A>& left, const dynamic_buffer<T, A>& right) noexcept -> std::partial_ordering
{
    if (not left.data or not right.data)
    {
        return std::partial_ordering::unordered;
    }

    if (left.size != right.size)
    {
        return left.size <=> right.size;
    }

    for (std::size_t i = 0; i < left.size; ++i)
    {
        auto value = left[i] <=> right[i];
        if (value != 0)
        {
            return value;
        }
    }

    return std::partial_ordering::equivalent;
};

template <typename T, typename A>
template <typename... Args>
constexpr auto dynamic_buffer<T, A>::resize(size_type new_size, Args&&... arguments) -> void
{
    if (size == new_size)
    {
        return;
    }

    const size_type limit = std::min(size, new_size);
    dynamic_buffer new_buffer(uninitialized, new_size);
    for (size_type i = 0; i < limit; ++i)
    {
        traits::construct(new_buffer.allocator, new_buffer.data + i, *(data + i));
    }
    for (size_type i = limit; i < new_size; ++i)
    {
        traits::construct(new_buffer.allocator, new_buffer.data + i, std::forward<Args>(arguments)...);
    }

    swap(*this, new_buffer);
};

template <typename T, typename A>
constexpr auto dynamic_buffer<T, A>::resize(uninitialized_t, size_type new_size) -> void
{
    if (size == new_size)
    {
        return;
    }

    const size_type limit = std::min(size, new_size);
    dynamic_buffer new_buffer(uninitialized, new_size);
    for (size_type i = 0; i < limit; ++i)
    {
        traits::construct(new_buffer.allocator, new_buffer.data + i, *(data + i));
    }

    swap(*this, new_buffer);
};
