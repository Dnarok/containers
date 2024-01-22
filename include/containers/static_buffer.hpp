#pragma once

#include <memory>
#include <initializer_list>
#include <compare>

#include "contract.hpp"

/*
    A buffer whose length is decided at compile time.
    Cannot be resized. 
*/

template <typename T, std::size_t N, typename A = std::allocator<T>>
struct static_buffer
{

};