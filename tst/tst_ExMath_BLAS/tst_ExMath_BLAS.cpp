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

TEST_CASE()
{
  using T1 = N::static_matrix_t<3, 2, int>;
  using T2 = N::static_matrix_t<2, 3, int>;

  T1 t1 = { 1, 2, 3, 4, 5, 6 };
  T2 t2 = { 1, 2, 3, 4, 5, 6 };

  auto e1 = t1 + N::transpose(t2);
  auto e2 = N::transpose(t1) + t2;

  REQUIRE(e1.number_of_rows == 3);
  REQUIRE(e1.number_of_columns == 2);
  REQUIRE(e2.number_of_rows == 2);
  REQUIRE(e2.number_of_columns == 3);

  REQUIRE(e1(0, 0) == 2);
  REQUIRE(e1(0, 1) == 6);
  REQUIRE(e1(1, 0) == 5);
  REQUIRE(e1(1, 1) == 9);
  REQUIRE(e1(2, 0) == 8);
  REQUIRE(e1(2, 1) == 12);

  REQUIRE(e2(0, 0) == 2);
  REQUIRE(e2(0, 1) == 5);
  REQUIRE(e2(0, 2) == 8);
  REQUIRE(e2(1, 0) == 6);
  REQUIRE(e2(1, 1) == 9);
  REQUIRE(e2(1, 2) == 12);
}

TEST_CASE()
{
  using T1 = N::static_matrix_t<3, 2, int>;
  using T2 = N::static_matrix_t<2, 3, int>;

  T1 t1 = { 1, 2, 3, 4, 5, 6 };
  T2 t2 = { 1, 2, 3, 4, 5, 6 };

  auto e1 = t1 - N::transpose(t2);
  auto e2 = N::transpose(t1) - t2;

  REQUIRE(e1.number_of_rows == 3);
  REQUIRE(e1.number_of_columns == 2);
  REQUIRE(e2.number_of_rows == 2);
  REQUIRE(e2.number_of_columns == 3);

  REQUIRE(e1(0, 0) == 0);
  REQUIRE(e1(0, 1) == -2);
  REQUIRE(e1(1, 0) == 1);
  REQUIRE(e1(1, 1) == -1);
  REQUIRE(e1(2, 0) == 2);
  REQUIRE(e1(2, 1) == 0);

  REQUIRE(e2(0, 0) == 0);
  REQUIRE(e2(0, 1) == 1);
  REQUIRE(e2(0, 2) == 2);
  REQUIRE(e2(1, 0) == -2);
  REQUIRE(e2(1, 1) == -1);
  REQUIRE(e2(1, 2) == 0);
}

TEST_CASE()
{
  using T1 = N::static_matrix_t<3, 2, int>;
  using T2 = N::static_matrix_t<2, 3, int>;
  using E1 = N::static_matrix_t<3, 3, int>;
  using E2 = N::static_matrix_t<2, 2, int>;

  T1 t1 = { 1, 2, 3, 4, 5, 6 };
  T2 t2 = { 1, 2, 3, 4, 5, 6 };

  auto e1 = t1 * t2;
  auto e2 = t2 * t1;

  REQUIRE(e1.number_of_rows == 3);
  REQUIRE(e1.number_of_columns == 3);
  REQUIRE(e2.number_of_rows == 2);
  REQUIRE(e2.number_of_columns == 2);

  REQUIRE(e1(0, 0) == 9);
  REQUIRE(e1(0, 1) == 12);
  REQUIRE(e1(0, 2) == 15);
  REQUIRE(e1(1, 0) == 19);
  REQUIRE(e1(1, 1) == 26);
  REQUIRE(e1(1, 2) == 33);
  REQUIRE(e1(2, 0) == 29);
  REQUIRE(e1(2, 1) == 40);
  REQUIRE(e1(2, 2) == 51);

  REQUIRE(e2(0, 0) == 22);
  REQUIRE(e2(0, 1) == 28);
  REQUIRE(e2(1, 0) == 49);
  REQUIRE(e2(1, 1) == 64);
}

TEST_CASE()
{
  using T1 = N::static_matrix_t<3, 3, double>;

  // clang-format off
  T1 t1 = { 0.964888535199277, 0.957166948242946, 0.141886338627215,
            0.157613081677548, 0.485375648722841, 0.421761282626275,
            0.970592781760616, 0.800280468888800, 0.915735525189067 };
  
  T1 t2 = { 0.347323286154839, -2.477781501973262,  1.087379322476138,
            0.860694397160436,  2.422267213509713, -1.248984310494387,
           -1.120308606567346,  0.509343239793394,  1.031012996914348 };
  // clang-format on

  auto e1 = N::inverse(t1);
  auto e2 = N::inverse(t2);

  for (N::index_t row = 0; row < T1::number_of_rows; row++)
    for (N::index_t col = 0; col < T1::number_of_columns; col++)
    {
      REQUIRE(e1(row, col) == Approx(t2(row, col)));
    }
  for (N::index_t row = 0; row < T1::number_of_rows; row++)
    for (N::index_t col = 0; col < T1::number_of_columns; col++)
    {
      REQUIRE(e2(row, col) == Approx(t1(row, col)));
    }
}
