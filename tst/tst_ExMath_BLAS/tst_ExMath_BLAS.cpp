#include <ExMath.hpp>
#include <ut_catch.hpp>

namespace N = ExMath;


float fill_fnc(N::index_t const& r, N::index_t const& c) { return static_cast<float>(r + c); }

TEST_CASE()
{
  using T = N::identity_matrix_t<1, 1, float>;

  static_assert(N::matrix_base_concept<T>);
  static_assert(N::matrix_readable_like_concept<T, float>);
  static_assert(N::matrix_readable_concept<T>);

  static_assert(N::is_same_size<T, T>);
}

TEST_CASE()
{
  using T = N::static_matrix_t<3, 2, float>;

  static_assert(N::matrix_base_concept<T>);
  static_assert(N::matrix_readable_like_concept<T, float>);
  static_assert(N::matrix_readable_concept<T>);
  static_assert(N::matrix_writeable_concept<T>);

  static_assert(N::is_same_size<T, T>);
}

TEST_CASE()
{
  using T1 = N::identity_matrix_t<3, 3, float>;
  using T2 = N::static_matrix_t<3, 3, float>;

  static_assert(N::is_same_size<T1, T2>);
}

TEST_CASE()
{
  static_assert(N::matrix_readable_like_concept<decltype(fill_fnc), float>);
}
