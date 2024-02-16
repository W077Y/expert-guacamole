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
  template <typename T> concept value_type_concept = requires() { typename T::value_type; };

  template <typename T> concept static_matrix_size_concept = requires()
  {
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

  template <typename T, typename value_type> concept readable_like_matrix_concept = requires(T const& obj, index_t const& idx_t)
  {
    {
      obj(idx_t, idx_t)
    } -> std::convertible_to<value_type>;
  };

  template <typename T, typename value_type> concept writeable_like_matrix_concept = requires(T & obj, index_t const& idx_t, typename T::value_type value)
  {
    {
      obj(idx_t, idx_t) = value
    } -> std::convertible_to<typename T::value_type&>;
  };

  template <typename T>
  concept readable_static_matrix_concept = value_type_concept<T> && static_matrix_size_concept<T> && readable_like_matrix_concept<T, typename T::value_type>;

  template <typename T>
  concept writeable_static_matrix_concept = value_type_concept<T> && static_matrix_size_concept<T> && writeable_like_matrix_concept<T, typename T::value_type>;

  template <static_matrix_size_concept T> constexpr bool is_scalar = T::number_of_rows == 1 && T::number_of_columns;

  template <static_matrix_size_concept T1, static_matrix_size_concept T2>
  constexpr bool is_same_size = T1::number_of_rows == T2::number_of_rows && T1::number_of_columns == T2::number_of_columns;

  template <typename Erg, typename Val>
  concept is_assignable = writeable_static_matrix_concept<Erg> && (readable_static_matrix_concept<Val> && is_same_size<Erg, Val>) ||
                          (!readable_static_matrix_concept<Val> && readable_like_matrix_concept<Val, typename Erg::value_type>);

}    // namespace ExMath

namespace ExMath
{
  namespace Internal
  {
    template <index_t rows, index_t columns> constexpr index_t calc_index_row_major(index_t const& row, index_t const& col) noexcept
    {
      return row * columns + col;
    }
    template <index_t rows, index_t columns> constexpr index_t calc_index_col_major(index_t const& row, index_t const& col) noexcept
    {
      return col * rows + row;
    }

    template <writeable_static_matrix_concept Erg, typename Val>
    requires readable_like_matrix_concept<Val, typename Erg::value_type> constexpr void assign(Erg& erg, Val const& rhs)
    {
      for (index_t col = 0; col < Erg::number_of_columns; col++)
        for (index_t row = 0; row < Erg::number_of_rows; row++)
          erg(row, col) = rhs(row, col);
    }

    template <writeable_static_matrix_concept Erg, typename Val>
    requires readable_like_matrix_concept<Val, typename Erg::value_type> constexpr void add_assign(Erg& erg, Val const& rhs)
    {
      for (index_t col = 0; col < Erg::number_of_columns; col++)
        for (index_t row = 0; row < Erg::number_of_rows; row++)
          erg(row, col) += rhs(row, col);
    }

    template <writeable_static_matrix_concept Erg, typename Val>
    requires readable_like_matrix_concept<Val, typename Erg::value_type> constexpr void sub_assign(Erg& erg, Val const& rhs)
    {
      for (index_t col = 0; col < Erg::number_of_columns; col++)
        for (index_t row = 0; row < Erg::number_of_rows; row++)
          erg(row, col) -= rhs(row, col);
    }

    template <writeable_static_matrix_concept Erg, typename Val> constexpr void mult_assign(Erg& erg, Val const& val)
    {
      for (index_t col = 0; col < Erg::number_of_columns; col++)
        for (index_t row = 0; row < Erg::number_of_rows; row++)
          erg(row, col) *= val;
    }

    template <writeable_static_matrix_concept Erg, typename Val> constexpr void div_assign(Erg& erg, Val const& rhs)
    {
      for (index_t col = 0; col < Erg::number_of_columns; col++)
        for (index_t row = 0; row < Erg::number_of_rows; row++)
          erg(row, col) /= rhs;
    }

    template <writeable_static_matrix_concept Erg, typename Lhs, typename Rhs>
    requires readable_like_matrix_concept<Lhs, typename Erg::value_type>&& readable_like_matrix_concept<Rhs, typename Erg::value_type> constexpr void
                                                                           add(Erg& erg, Lhs const& lhs, Rhs const& rhs)
    {
      for (index_t col = 0; col < Erg::number_of_columns; col++)
        for (index_t row = 0; row < Erg::number_of_rows; row++)
          erg(row, col) = lhs(row, col) + rhs(row, col);
    }

    template <writeable_static_matrix_concept Erg, typename Lhs, typename Rhs>
    requires readable_like_matrix_concept<Lhs, typename Erg::value_type>&& readable_like_matrix_concept<Rhs, typename Erg::value_type> constexpr void
                                                                           sub(Erg& erg, Lhs const& lhs, Rhs const& rhs)
    {
      for (index_t col = 0; col < Erg::number_of_columns; col++)
        for (index_t row = 0; row < Erg::number_of_rows; row++)
          erg(row, col) = lhs(row, col) - rhs(row, col);
    }

    template <writeable_static_matrix_concept Erg, typename Lhs, typename Rhs>
    requires readable_like_matrix_concept<Lhs, typename Erg::value_type>&& readable_like_matrix_concept<Rhs, typename Erg::value_type> constexpr void
                                                                           mult(Erg& erg, Lhs const& lhs, Rhs const& rhs)
    {
      using T = typename Erg::value_type;
      for (index_t col = 0; col < Erg::number_of_columns; col++)
        for (index_t row = 0; row < Erg::number_of_rows; row++)
        {
          T tmp = 0;
          for (index_t idx = 0; idx < Lhs::number_of_columns; idx++)
            tmp += lhs(row, idx) * rhs(idx, col);
          erg(row, col) = tmp;
        }
    }

    template <writeable_static_matrix_concept Erg, typename Val>
    requires readable_like_matrix_concept<Val, typename Erg::value_type> constexpr void scale(Erg& erg, Val const& val, typename Val::value_type const& scale)
    {
      for (index_t col = 0; col < Erg::number_of_columns; col++)
        for (index_t row = 0; row < Erg::number_of_rows; row++)
          erg(row, col) = val(row, col) * scale;
    }
  }    // namespace Internal
}    // namespace ExMath

namespace ExMath
{
  template <index_t rows, index_t columns, typename T> class static_matrix_t
  {
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
      return this->m_data[Internal::calc_index_row_major<number_of_rows, number_of_columns>(row, col)];
    }

    constexpr auto operator()(index_t const& row, index_t const& col) noexcept -> value_type&
    {
      return this->m_data[Internal::calc_index_row_major<number_of_rows, number_of_columns>(row, col)];
    }

    template <typename Rhs> requires is_assignable<static_matrix_t, Rhs> constexpr auto operator=(Rhs const& rhs) noexcept
    {
      Internal::assign(*this, rhs);
      return *this;
    };

  private:
    value_type m_data[number_of_elements]{};
  };

  template <index_t rows, index_t columns, typename T> class static_matrix_t<rows, columns, const T>
  {
  public:
    using value_type                            = std::remove_cvref_t<T>;
    static constexpr index_t number_of_rows     = rows;
    static constexpr index_t number_of_columns  = columns;
    static constexpr index_t number_of_elements = rows * columns;

    static_matrix_t(value_type const (&data)[number_of_elements])
        : m_data{ data }
    {
    }

    constexpr auto operator()(index_t const& row, index_t const& col) const noexcept -> value_type const&
    {
      return this->m_data[Internal::calc_index_row_major<number_of_rows, number_of_columns>(row, col)];
    }

  private:
    value_type const (&m_data)[number_of_elements]{};
  };

  template <index_t rows, index_t columns, typename T> class static_matrix_external_memory_t
  {
  public:
    using value_type                            = std::remove_cvref_t<T>;
    static constexpr index_t number_of_rows     = rows;
    static constexpr index_t number_of_columns  = columns;
    static constexpr index_t number_of_elements = rows * columns;

    static_matrix_external_memory_t(value_type (&data)[number_of_elements])
        : m_data{ data }
    {
    }

    constexpr auto operator()(index_t const& row, index_t const& col) const noexcept -> value_type const&
    {
      return this->m_data[Internal::calc_index_row_major<number_of_rows, number_of_columns>(row, col)];
    }

    constexpr auto operator()(index_t const& row, index_t const& col) noexcept -> value_type&
    {
      return this->m_data[Internal::calc_index_row_major<number_of_rows, number_of_columns>(row, col)];
    }

    template <typename Rhs> requires is_assignable<static_matrix_t, Rhs> constexpr auto operator=(Rhs const& rhs) noexcept
    {
      Internal::assign(*this, rhs);
      return *this;
    };

  private:
    value_type (&m_data)[number_of_elements]{};
  };

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

  template <readable_static_matrix_concept T> class transpose_view_t
  {
  public:
    using value_type                            = std::remove_cvref_t<typename T::value_type>;
    static constexpr index_t number_of_rows     = T::number_of_columns;
    static constexpr index_t number_of_columns  = T::number_of_rows;
    static constexpr index_t number_of_elements = T::number_of_elements;

    transpose_view_t(T const& obj)
        : m_obj{ obj }
    {
    }

    constexpr auto operator()(index_t const& row, index_t const& col) const noexcept -> value_type const& { return this->m_obj(col, row); }

  private:
    T const& m_obj;
  };

  template <index_t rows, index_t columns, typename T> using matrix_view_t = static_matrix_t<rows, columns, const T>;
}    // namespace ExMath

namespace ExMath
{
  template <typename Erg, typename Rhs>
  requires(writeable_static_matrix_concept<Erg>&& readable_static_matrix_concept<Rhs>&& is_same_size<Erg, Rhs>) constexpr auto
  operator+=(Erg& erg, Rhs const& rhs) noexcept
  {
    Internal::add_assign(erg, rhs);
    return erg;
  }

  template <typename Erg, typename Rhs>
  requires(writeable_static_matrix_concept<Erg>&& readable_static_matrix_concept<Rhs>&& is_same_size<Erg, Rhs>) constexpr auto
  operator-=(Erg& erg, Rhs const& rhs) noexcept
  {
    Internal::sub_assign(erg, rhs);
    return erg;
  }

  template <typename Erg> requires writeable_static_matrix_concept<Erg> constexpr auto operator*=(Erg& erg, typename Erg::value_type const& rhs) noexcept
  {
    Internal::mult_assign(erg, rhs);
    return erg;
  }

  template <typename Erg> requires writeable_static_matrix_concept<Erg> constexpr auto operator/=(Erg& erg, typename Erg::value_type const& rhs) noexcept
  {
    Internal::div_assign(erg, rhs);
    return erg;
  }

  template <typename Lhs, typename Rhs>
  requires(readable_static_matrix_concept<Lhs>&& readable_static_matrix_concept<Rhs>&& is_same_size<Lhs, Rhs>) constexpr auto operator+(Lhs const& lhs,
                                                                                                                                        Rhs const& rhs) noexcept
  {
    using Erg = static_matrix_t<Rhs::number_of_rows, Rhs::number_of_columns, typename Rhs::value_type>;
    Erg erg;
    Internal::add(erg, lhs, rhs);
    return erg;
  }

  template <typename Lhs, typename Rhs>
  requires(readable_static_matrix_concept<Lhs>&& readable_static_matrix_concept<Rhs>&& is_same_size<Lhs, Rhs>) constexpr auto operator-(Lhs const& lhs,
                                                                                                                                        Rhs const& rhs) noexcept
  {
    using Erg = static_matrix_t<Rhs::number_of_rows, Rhs::number_of_columns, typename Rhs::value_type>;
    Erg erg;
    Internal::sub(erg, lhs, rhs);
    return erg;
  }

  template <typename Lhs, typename Rhs>
  requires(readable_static_matrix_concept<Lhs>&& readable_static_matrix_concept<Rhs>&& Lhs::number_of_columns == Rhs::number_of_rows) constexpr auto
  operator*(Lhs const& lhs, Rhs const& rhs) noexcept
  {
    using Erg = static_matrix_t<Lhs::number_of_rows, Rhs::number_of_columns, typename Lhs::value_type>;
    Erg erg;
    Internal::mult(erg, lhs, rhs);
    return erg;
  }

  template <typename Val, typename Scl>
  requires(readable_static_matrix_concept<Val>&& readable_static_matrix_concept<Scl> && !is_scalar<Val> && is_scalar<Scl>) constexpr auto
  operator*(Val const& val, Scl const& scale) noexcept
  {
    using Erg = static_matrix_t<Val::number_of_rows, Val::number_of_columns, typename Val::value_type>;
    Erg erg;
    Internal::scale(erg, val, scale(0, 0));
    return erg;
  }

  template <typename Val, typename Scl>
  requires(readable_static_matrix_concept<Val>&& readable_static_matrix_concept<Scl> && !is_scalar<Val> && is_scalar<Scl>) constexpr auto
  operator*(Scl const& scale, Val const& val) noexcept
  {
    using Erg = static_matrix_t<Val::number_of_rows, Val::number_of_columns, typename Val::value_type>;
    Erg erg;
    Internal::scale(erg, val, scale(0, 0));
    return erg;
  }

  template <typename Val> requires(readable_static_matrix_concept<Val>) constexpr auto operator*(Val const& val, typename Val::value_type const& scale) noexcept
  {
    using Erg = static_matrix_t<Val::number_of_rows, Val::number_of_columns, typename Val::value_type>;
    Erg erg;
    Internal::scale(erg, val, scale);
    return erg;
  }

  template <typename Val> requires(readable_static_matrix_concept<Val>) constexpr auto operator*(typename Val::value_type const& scale, Val const& val) noexcept
  {
    using Erg = static_matrix_t<Val::number_of_rows, Val::number_of_columns, typename Val::value_type>;
    Erg erg;
    Internal::scale(erg, val, scale);
    return erg;
  }

  template <readable_static_matrix_concept T> constexpr auto transpose(T const& val) { return transpose_view_t<T>{ val }; }

  template <readable_static_matrix_concept T> requires(T::number_of_rows == T::number_of_columns) constexpr auto inverse(T const& value)
  {
    using value_type = typename T::value_type;
    using mat_t      = static_matrix_t<T::number_of_rows, T::number_of_columns, value_type>;

    auto swap_rows = [](mat_t& mat, index_t const& r1, index_t const& r2)
    {
      for (index_t col = 0; col < mat_t::number_of_columns; col++)
        std::swap(mat(r1, col), mat(r2, col));
    };

    auto div_row = [](mat_t& mat, value_type const& val, index_t const& row)
    {
      for (index_t col = 0; col < mat_t::number_of_columns; col++)
        mat(row, col) /= val;
    };

    auto sub_scl_rows = [](mat_t& mat, value_type const& val, index_t const& r1, index_t const& r2)
    {
      for (index_t col = 0; col < mat_t::number_of_columns; col++)
        mat(r1, col) -= mat(r2, col) * val;
    };

    mat_t erg = identity_matrix_t<mat_t::number_of_rows, mat_t::number_of_columns, value_type>();
    mat_t val = value;
    for (index_t row = 0; row < mat_t::number_of_rows; row++)
    {
      index_t    sel_idx = row;
      value_type fac     = val(row, row);
      for (index_t idx = row + 1; idx < mat_t::number_of_rows; idx++)
      {
        if (fac < val(idx, row))
        {
          sel_idx = idx;
          fac     = val(idx, row);
        }
      }

      if (sel_idx != row)
      {
        swap_rows(val, sel_idx, row);
        swap_rows(erg, sel_idx, row);
      }

      if (fac != 1.0)
      {
        div_row(val, fac, row);
        div_row(erg, fac, row);
      }

      for (index_t idx = 0; idx < mat_t::number_of_rows; idx++)
      {
        if (idx == row)
          continue;
        value_type const fac = val(idx, row);
        sub_scl_rows(val, fac, idx, row);
        sub_scl_rows(erg, fac, idx, row);
      }
    }
    return erg;
  }
}    // namespace ExMath

#endif
