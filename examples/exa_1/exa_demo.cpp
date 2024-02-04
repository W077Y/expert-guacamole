#include <ExMath.hpp>
#include <iostream>
#include <type_traits>

template <ExMath::matrix_readable_concept T> void print_matrix(const char* name, T const& m)
{
  std::cout << "\n" << name << " = [\n";
  for (ExMath::index_t r = 0; r < T::number_of_rows; r++)
  {
    for (ExMath::index_t c = 0; c < T::number_of_columns; c++)
    {
      std::cout << m(r, c) << ",";
    }
    std::cout << "\n";
  }
  std::cout << "]\n";
}

float fill_fnc(ExMath::index_t const& r, ExMath::index_t const& c) { return static_cast<float>(r + c); }

int main()
{
  std::cout << "This is a demo example!" << std::endl;

  ExMath::identity_matrix_t<3, 3, float> u3;
  print_matrix("m1", u3);

  {
    ExMath::static_matrix_t<3, 3, float> m;
    print_matrix("m2", m);
  }
  {
    ExMath::static_matrix_t<3, 3, float> m{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
    print_matrix("m2-a", m);
  }
  {
    ExMath::static_matrix_t<3, 3, float> m{ u3 };
    print_matrix("m3", m);
  }
  {
    ExMath::static_matrix_t<3, 3, float> m{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
    print_matrix("m4", m);
    m = u3;
    print_matrix("m5", m);
    m = fill_fnc;
    print_matrix("m6", m);
  }
  {
    ExMath::static_matrix_t<3, 4, float> m{ fill_fnc };
    print_matrix("m7", m);
  }
  {
    ExMath::static_matrix_t<3, 3, float> m{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
    auto                                 m0 = m + m + ExMath::identity_matrix_t<3, 3, float>();
    print_matrix("m7", m0);
  }
  {
    ExMath::static_matrix_t<3, 3, float> m{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
    m += ExMath::identity_matrix_t<3, 3, float>();
    print_matrix("m8", m);
  }
  {
    ExMath::static_matrix_t<3, 3, float> m{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
    auto                                 m0 = m - m - ExMath::identity_matrix_t<3, 3, float>();
    print_matrix("m9", m0);
  }
  {
    ExMath::static_matrix_t<3, 3, float> m{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
    m -= ExMath::identity_matrix_t<3, 3, float>();
    print_matrix("m10", m);
  }
  //{
  //  ExMath::matrix_t<float, 3, 3> m{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
  //  auto                          m0 = m * m;
  //  print_matrix("m11", m0);
  //}
  //{
  //  ExMath::matrix_t<float, 3, 3> m1{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
  //  ExMath::matrix_t<float, 1, 1> m2{ 2.0f };
  //  auto                          m0 = m2 * m1 * m2;
  //  print_matrix("m12", m0);
  //}
  //{
  //  ExMath::matrix_t<float, 3, 3> m1{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
  //  auto                          m0 = 2.0f * m1 * 2.0f;
  //  print_matrix("m12", m0);
  //}

  return 0;
}
