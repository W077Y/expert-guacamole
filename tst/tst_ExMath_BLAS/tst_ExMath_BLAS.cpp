#include <ExMath.hpp>
#include <ut_catch.hpp>

namespace N = ExMath;

TEST_CASE()
{
  using T = N::identity_matrix_t<1, 1, float>;

  static_assert(N::value_type_concept<T>);
  static_assert(N::static_matrix_size_concept<T>);
  static_assert(N::readable_like_matrix_concept<T, float>);
  static_assert(!N::writeable_like_matrix_concept<T, float>);
  static_assert(N::readable_static_matrix_concept<T>);
  static_assert(!N::writeable_static_matrix_concept<T>);

  static_assert(N::is_same_size<T, T>);
  static_assert(N::is_scalar<T>);
}

TEST_CASE()
{
  using T1 = N::matrix_view_t<1, 1, float>;
  using T2 = N::matrix_view_t<3, 3, float>;

  static_assert(N::value_type_concept<T1>);
  static_assert(N::static_matrix_size_concept<T1>);
  static_assert(N::readable_like_matrix_concept<T1, float>);
  static_assert(!N::writeable_like_matrix_concept<T1, float>);
  static_assert(N::readable_static_matrix_concept<T1>);
  static_assert(!N::writeable_static_matrix_concept<T1>);

  static_assert(N::is_same_size<T1, T1>);
  static_assert(N::is_scalar<T1>);

  static_assert(N::value_type_concept<T2>);
  static_assert(N::static_matrix_size_concept<T2>);
  static_assert(N::readable_like_matrix_concept<T2, float>);
  static_assert(!N::writeable_like_matrix_concept<T2, float>);
  static_assert(N::readable_static_matrix_concept<T2>);
  static_assert(!N::writeable_static_matrix_concept<T2>);

  static_assert(!N::is_same_size<T1, T2>);
  static_assert(!N::is_scalar<T2>);

  float data_t1[1] = { 1.0f };
  T1    t1{ data_t1 };

  float data_t2[9] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
  T2    t2{ data_t2 };

  REQUIRE(t1(0, 0) == 1.0f);

  REQUIRE(t2(0, 0) == 1.0f);
  REQUIRE(t2(0, 1) == 2.0f);
  REQUIRE(t2(0, 2) == 3.0f);
  REQUIRE(t2(1, 0) == 4.0f);
  REQUIRE(t2(1, 1) == 5.0f);
  REQUIRE(t2(1, 2) == 6.0f);
  REQUIRE(t2(2, 0) == 7.0f);
  REQUIRE(t2(2, 1) == 8.0f);
  REQUIRE(t2(2, 2) == 9.0f);
}

TEST_CASE()
{
  using T1 = N::static_matrix_external_memory_t<1, 1, float>;
  using T2 = N::static_matrix_external_memory_t<3, 3, float>;

  static_assert(N::value_type_concept<T1>);
  static_assert(N::static_matrix_size_concept<T1>);
  static_assert(N::readable_like_matrix_concept<T1, float>);
  static_assert(N::writeable_like_matrix_concept<T1, float>);
  static_assert(N::readable_static_matrix_concept<T1>);
  static_assert(N::writeable_static_matrix_concept<T1>);

  static_assert(N::is_same_size<T1, T1>);
  static_assert(N::is_scalar<T1>);

  static_assert(N::value_type_concept<T2>);
  static_assert(N::static_matrix_size_concept<T2>);
  static_assert(N::readable_like_matrix_concept<T2, float>);
  static_assert(N::writeable_like_matrix_concept<T2, float>);
  static_assert(N::readable_static_matrix_concept<T2>);
  static_assert(N::writeable_static_matrix_concept<T2>);

  static_assert(!N::is_same_size<T1, T2>);
  static_assert(!N::is_scalar<T2>);

  float data_t1[1] = { 1.0f };
  T1    t1{ data_t1 };

  float data_t2[9] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
  T2    t2{ data_t2 };

  REQUIRE(t1(0, 0) == 1.0f);

  REQUIRE(t2(0, 0) == 1.0f);
  REQUIRE(t2(0, 1) == 2.0f);
  REQUIRE(t2(0, 2) == 3.0f);
  REQUIRE(t2(1, 0) == 4.0f);
  REQUIRE(t2(1, 1) == 5.0f);
  REQUIRE(t2(1, 2) == 6.0f);
  REQUIRE(t2(2, 0) == 7.0f);
  REQUIRE(t2(2, 1) == 8.0f);
  REQUIRE(t2(2, 2) == 9.0f);

  t1 *= 2;
  t2 *= 2;

  REQUIRE(t1(0, 0) == 2.0f);

  REQUIRE(t2(0, 0) == 2.0f);
  REQUIRE(t2(0, 1) == 4.0f);
  REQUIRE(t2(0, 2) == 6.0f);
  REQUIRE(t2(1, 0) == 8.0f);
  REQUIRE(t2(1, 1) == 10.0f);
  REQUIRE(t2(1, 2) == 12.0f);
  REQUIRE(t2(2, 0) == 14.0f);
  REQUIRE(t2(2, 1) == 16.0f);
  REQUIRE(t2(2, 2) == 18.0f);

  t1 /= 2;
  t2 /= 2;

  REQUIRE(t1(0, 0) == 1.0f);

  REQUIRE(t2(0, 0) == 1.0f);
  REQUIRE(t2(0, 1) == 2.0f);
  REQUIRE(t2(0, 2) == 3.0f);
  REQUIRE(t2(1, 0) == 4.0f);
  REQUIRE(t2(1, 1) == 5.0f);
  REQUIRE(t2(1, 2) == 6.0f);
  REQUIRE(t2(2, 0) == 7.0f);
  REQUIRE(t2(2, 1) == 8.0f);
  REQUIRE(t2(2, 2) == 9.0f);
}

TEST_CASE()
{
  using T1 = N::static_matrix_t<3, 2, float>;
  using T2 = N::static_matrix_t<3, 2, const float>;

  static_assert(N::value_type_concept<T1>);
  static_assert(N::static_matrix_size_concept<T1>);
  static_assert(N::readable_like_matrix_concept<T1, float>);
  static_assert(N::writeable_like_matrix_concept<T1, float>);
  static_assert(N::readable_static_matrix_concept<T1>);
  static_assert(N::writeable_static_matrix_concept<T1>);

  static_assert(N::is_same_size<T1, T2>);
  static_assert(!N::is_scalar<T1>);

  static_assert(N::value_type_concept<T2>);
  static_assert(N::static_matrix_size_concept<T2>);
  static_assert(N::readable_like_matrix_concept<T2, float>);
  static_assert(!N::writeable_like_matrix_concept<T2, float>);
  static_assert(N::readable_static_matrix_concept<T2>);
  static_assert(!N::writeable_static_matrix_concept<T2>);

  static_assert(N::is_same_size<T2, T1>);
  static_assert(!N::is_scalar<T2>);
}

TEST_CASE()
{
  using T1 = N::identity_matrix_t<3, 3, float>;
  using T2 = N::static_matrix_t<3, 3, float>;

  static_assert(N::is_same_size<T1, T2>);
}

TEST_CASE()
{
  static_assert(N::readable_like_matrix_concept<decltype([](N::index_t const&, N::index_t const&) { return 0.0f; }), float>);
}
