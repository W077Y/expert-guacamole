#pragma once
#ifndef EXMATH_TRAITS_HPP
#define EXMATH_TRAITS_HPP

#include <cmath>
#include <concepts>
#include <type_traits>

namespace ExMath
{
  using index_t = uint32_t;
}

namespace ExMath
{
  template <typename T> concept matrix_base_concept = requires()
  {
    typename T::value_type;
    {
      T::number_of_rows
    } -> std::convertible_to<index_t>;
    {
      T::number_of_columns
    } -> std::convertible_to<index_t>;
    {
      T::number_of_elements
    } -> std::convertible_to<index_t>;
  };

  template <typename T, typename value_type> concept matrix_readable_like_concept = requires(T const& obj, index_t const& idx_t)
  {
    {
      obj(idx_t, idx_t)
    } -> std::convertible_to<value_type>;
  };

  template <typename T, typename value_type> concept matrix_writeable_like_concept = requires(T & obj, index_t const& idx_t, typename T::value_type value)
  {
    {
      obj(idx_t, idx_t) = value
    } -> std::convertible_to<typename T::value_type&>;
  };

  template <typename T> concept matrix_readable_concept = matrix_base_concept<T> && matrix_readable_like_concept<T, typename T::value_type>;

  template <typename T> concept matrix_writeable_concept = matrix_readable_concept<T> && matrix_writeable_like_concept<T, typename T::value_type>;

  template <matrix_base_concept T1, matrix_base_concept T2>
  constexpr bool is_same_size = T1::number_of_rows == T2::number_of_rows && T1::number_of_columns == T2::number_of_columns;

  template <typename Lhs, typename Rhs>
  concept is_assignable = matrix_writeable_concept<Lhs> && ((matrix_readable_concept<Rhs> && is_same_size<Lhs, Rhs>) ||
                                                            (!matrix_readable_concept<Rhs> && matrix_readable_like_concept<Rhs, typename Lhs::value_type>));
  ;

}    // namespace ExMath

namespace ExMath
{
  namespace Internal
  {
    template <matrix_writeable_concept Erg, typename Rhs>
    requires matrix_readable_like_concept<Rhs, typename Erg::value_type> constexpr void assign(Erg& erg, Rhs const& rhs)
    {
      for (index_t col = 0; col < Erg::number_of_columns; col++)
        for (index_t row = 0; row < Erg::number_of_rows; row++)
          erg(row, col) = rhs(row, col);
    }

    template <matrix_writeable_concept Erg, typename Rhs>
    requires matrix_readable_like_concept<Rhs, typename Erg::value_type> constexpr void add_assign(Erg& erg, Rhs const& rhs)
    {
      for (index_t col = 0; col < Erg::number_of_columns; col++)
        for (index_t row = 0; row < Erg::number_of_rows; row++)
          erg(row, col) += rhs(row, col);
    }

    template <matrix_writeable_concept Erg, typename Rhs>
    requires matrix_readable_like_concept<Rhs, typename Erg::value_type> constexpr void sub_assign(Erg& erg, Rhs const& rhs)
    {
      for (index_t col = 0; col < Erg::number_of_columns; col++)
        for (index_t row = 0; row < Erg::number_of_rows; row++)
          erg(row, col) -= rhs(row, col);
    }

    template <matrix_writeable_concept Erg, typename Lhs, typename Rhs>
    requires matrix_readable_like_concept<Lhs, typename Erg::value_type>&& matrix_readable_like_concept<Rhs, typename Erg::value_type> constexpr void
                                                                           add(Erg& erg, Lhs const& lhs, Rhs const& rhs)
    {
      for (index_t col = 0; col < Erg::number_of_columns; col++)
        for (index_t row = 0; row < Erg::number_of_rows; row++)
          erg(row, col) = lhs(row, col) + rhs(row, col);
    }

    template <matrix_writeable_concept Erg, typename Lhs, typename Rhs>
    requires matrix_readable_like_concept<Lhs, typename Erg::value_type>&& matrix_readable_like_concept<Rhs, typename Erg::value_type> constexpr void
                                                                           sub(Erg& erg, Lhs const& lhs, Rhs const& rhs)
    {
      for (index_t col = 0; col < Erg::number_of_columns; col++)
        for (index_t row = 0; row < Erg::number_of_rows; row++)
          erg(row, col) = lhs(row, col) - rhs(row, col);
    }
  }    // namespace Internal

  template <index_t rows, index_t columns, typename T> class identity_matrix_t
  {
  public:
    using value_type                            = std::remove_cvref_t<T>;
    static constexpr index_t number_of_rows     = rows;
    static constexpr index_t number_of_columns  = columns;
    static constexpr index_t number_of_elements = rows * columns;

    constexpr auto operator()(index_t const& row, index_t const& col) const noexcept -> value_type
    {
      if (row == col)
        return static_cast<value_type>(1);
      return static_cast<value_type>(0);
    }
  };

  template <index_t rows, index_t columns, typename T> class static_matrix_t
  {
    static constexpr index_t calc_index_row_major(index_t const& row, index_t const& col) noexcept { return row * columns + col; }
    static constexpr index_t calc_index_col_major(index_t const& row, index_t const& col) noexcept { return col * rows + row; }

  public:
    using value_type                            = std::remove_cvref_t<T>;
    static constexpr index_t number_of_rows     = rows;
    static constexpr index_t number_of_columns  = columns;
    static constexpr index_t number_of_elements = rows * columns;

    constexpr static_matrix_t() noexcept = default;

    template <typename Rhs> requires is_assignable<static_matrix_t, Rhs> constexpr static_matrix_t(Rhs const& rhs) noexcept { Internal::assign(*this, rhs); };

    template <typename... Us>
    requires std::is_convertible_v<std::common_type_t<Us...>, value_type> constexpr static_matrix_t(Us&&... args) noexcept
        : m_data{ std::forward<Us>(args)... }
    {
      static_assert(sizeof...(Us) == number_of_elements);
    };

    constexpr auto operator()(index_t const& row, index_t const& col) const noexcept -> value_type const&
    {
      return this->m_data[static_matrix_t::calc_index_row_major(row, col)];
    }

    constexpr auto operator()(index_t const& row, index_t const& col) noexcept -> value_type&
    {
      return this->m_data[static_matrix_t::calc_index_row_major(row, col)];
    }

    template <typename Rhs> requires is_assignable<static_matrix_t, Rhs> constexpr auto operator=(Rhs const& rhs) noexcept
    {
      Internal::assign(*this, rhs);
      return *this;
    };

  private:
    value_type m_data[number_of_elements]{};
  };

  template <typename Erg, typename Rhs>
  requires(matrix_writeable_concept<Erg>&& matrix_readable_concept<Rhs>&& is_same_size<Erg, Rhs>) constexpr auto operator+=(Erg& erg, Rhs const& rhs) noexcept
  {
    Internal::add_assign(erg, rhs);
    return erg;
  }

  template <typename Erg, typename Rhs>
  requires(matrix_writeable_concept<Erg>&& matrix_readable_concept<Rhs>&& is_same_size<Erg, Rhs>) constexpr auto operator-=(Erg& erg, Rhs const& rhs) noexcept
  {
    Internal::sub_assign(erg, rhs);
    return erg;
  }

  template <typename Lhs, typename Rhs>
  requires(matrix_readable_concept<Lhs>&& matrix_readable_concept<Rhs>&& is_same_size<Lhs, Rhs>) constexpr auto operator+(Lhs const& lhs,
                                                                                                                          Rhs const& rhs) noexcept
  {
    using Erg = static_matrix_t<Rhs::number_of_rows, Rhs::number_of_columns, typename Rhs::value_type>;
    Erg erg;
    Internal::add(erg, lhs, rhs);
    return erg;
  }

  template <typename Lhs, typename Rhs>
  requires(matrix_readable_concept<Lhs>&& matrix_readable_concept<Rhs>&& is_same_size<Lhs, Rhs>) constexpr auto operator-(Lhs const& lhs,
                                                                                                                          Rhs const& rhs) noexcept
  {
    using Erg = static_matrix_t<Rhs::number_of_rows, Rhs::number_of_columns, typename Rhs::value_type>;
    Erg erg;
    Internal::sub(erg, lhs, rhs);
    return erg;
  }

}    // namespace ExMath

#endif
