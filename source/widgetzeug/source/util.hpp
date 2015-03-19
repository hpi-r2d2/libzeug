#pragma once

#include <cmath>
#include <memory>
#include <type_traits>

namespace widgetzeug
{

template <typename T>
const T & clamp(const T & value, const T & min, const T & max)
{
    return std::min(std::max(value, min), max);
}

template <typename T>
const T fract(const T & value)
{
    static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");
    return value - std::floor(value);
}

template <typename T>
const T mix(const T & x, const T & y, qreal a)
{
    return (1.0 - a) * x + a * y;
}

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

} // namespace widgetzeug
