/*MIT License

Copyright (c) 2020 Ben Roberts

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

https://github.com/benroberts999/StrongType
*/

#pragma once
#include <iostream>
#include <type_traits>

namespace qip {

  template <typename enumT, enumT enumV, typename BaseT> struct StrongType {
  private:
    static_assert(std::is_arithmetic<BaseT>::value,
      "StrongType only available for arithmetic types");
    static_assert(
      std::is_enum<enumT>::value,
      "StrongType must be instantiated with scoped enum (enum class)");
    using StrongT = StrongType<enumT, enumV, BaseT>; // type alias

public:
  BaseT value;

  explicit constexpr StrongType(BaseT tv) : value(tv) {}
  explicit constexpr operator BaseT() const { return value; }
  constexpr BaseT &as_base() { return value; }
  [[nodiscard]] constexpr BaseT as_base() const { return value; }

  using BaseType = BaseT; // makes 'BaseType' publicly accessible

  //! Provides operators for regular arithmetic operations
  constexpr StrongT &operator*=(const StrongT &rhs) {
    this->value *= rhs.value;
    return *this;
  }
  friend constexpr StrongT operator*(StrongT lhs, const StrongT &rhs) {
    return lhs *= rhs;
  }
  constexpr StrongT &operator/=(const StrongT &rhs) {
    this->value /= rhs.value;
    return *this;
  }
  friend constexpr StrongT operator/(StrongT lhs, const StrongT &rhs) {
    return lhs /= rhs;
  }
  constexpr StrongT &operator+=(const StrongT &rhs) {
    this->value += rhs.value;
    return *this;
  }
  friend constexpr StrongT operator+(StrongT lhs, const StrongT &rhs) {
    return lhs += rhs;
  }
  constexpr StrongT &operator-=(const StrongT &rhs) {
    this->value -= rhs.value;
    return *this;
  }
  friend constexpr StrongT operator-(StrongT lhs, const StrongT &rhs) {
    return lhs -= rhs;
  }

  //! Provide Base*Strong, Strong*Base oprators - allow scalar multiplication
  constexpr StrongT &operator*=(const BaseT &rhs) {
    this->value *= rhs;
    return *this;
  }
  friend constexpr StrongT operator*(StrongT lhs, const BaseT &rhs) {
    return lhs *= rhs;
  }
  friend constexpr StrongT operator*(const BaseT &lhs, StrongT rhs) {
    return rhs *= lhs;
  }
  //! Provide Strong/Base, but NOT Base/Strong (still scalar multiplication).
  // If StrongT is used for physical units, this will likely not be what you
  // want. In this case, just be explicit. Base/Strong is not scalar
  // multiplication.
  constexpr StrongT &operator/=(const BaseT &rhs) {
    this->value /= rhs;
    return *this;
  }
  friend constexpr StrongT operator/(StrongT lhs, const BaseT &rhs) {
    return lhs /= rhs;
  }

  //! Provides pre/post increment/decrement (++, --) operators
  constexpr StrongT &operator++() {
    ++value;
    return *this;
  }
  constexpr StrongT operator++(int) {
    StrongT result(*this);
    ++(*this);
    return result;
  }
  constexpr StrongT &operator--() {
    --value;
    return *this;
  }
  constexpr StrongT operator--(int) {
    StrongT result(*this);
    --(*this);
    return result;
  }

  //! Provides comparison operators
  friend constexpr bool operator==(const StrongT &lhs, const StrongT &rhs) {
    return lhs.value == rhs.value;
  }
  friend constexpr bool operator!=(const StrongT &lhs, const StrongT &rhs) {
    return !(lhs == rhs);
  }
  friend constexpr bool operator<(const StrongT &lhs, const StrongT &rhs) {
    return lhs.value < rhs.value;
  }
  friend constexpr bool operator>(const StrongT &lhs, const StrongT &rhs) {
    return rhs < lhs;
  }
  friend constexpr bool operator<=(const StrongT &lhs, const StrongT &rhs) {
    return !(rhs < lhs);
  }
  friend constexpr bool operator>=(const StrongT &lhs, const StrongT &rhs) {
    return !(lhs < rhs);
  }

  //! Provides operators for direct comparison w/ BaseT literal (rvalue).
  //! Note: Does not allow comparison with BaseT lvalue
  friend constexpr bool operator==(const StrongT &lhs, const BaseT &&rhs) {
    return lhs.value == rhs;
  }
  friend constexpr bool operator!=(const StrongT &lhs, const BaseT &&rhs) {
    return lhs.value != rhs;
  }
  friend constexpr bool operator<(const StrongT &lhs, const BaseT &&rhs) {
    return lhs.value < rhs;
  }
  friend constexpr bool operator>(const StrongT &lhs, const BaseT &&rhs) {
    return lhs.value > rhs;
  }
  friend constexpr bool operator<=(const StrongT &lhs, const BaseT &&rhs) {
    return lhs.value <= rhs;
  }
  friend constexpr bool operator>=(const StrongT &lhs, const BaseT &&rhs) {
    return lhs.value >= rhs;
  }
  friend constexpr bool operator==(const BaseT &&lhs, const StrongT &rhs) {
    return lhs == rhs.value;
  }
  friend constexpr bool operator!=(const BaseT &&lhs, const StrongT &rhs) {
    return lhs != rhs.value;
  }
  friend constexpr bool operator<(const BaseT &&lhs, const StrongT &rhs) {
    return lhs < rhs.value;
  }
  friend constexpr bool operator>(const BaseT &&lhs, const StrongT &rhs) {
    return lhs > rhs.value;
  }
  friend constexpr bool operator<=(const BaseT &&lhs, const StrongT &rhs) {
    return lhs <= rhs.value;
  }
  friend constexpr bool operator>=(const BaseT &&lhs, const StrongT &rhs) {
    return lhs >= rhs.value;
  }

  //! Provides iostream interface, works as it would for BaseT
  friend std::ostream &operator<<(std::ostream &os, const StrongT &rhs) {
    return os << rhs.value;
  }
  friend std::istream &operator>>(std::istream &is, StrongT &rhs) {
    return is >> rhs.value;
  }
};

} // namespace qip