#include "utils/utils.hpp"

#include <cmath>

#include "gtest/gtest.h"

using namespace trans_geo::utils;

namespace trans_geo::utils::test {
/**
 * @brief degToRad() のテスト
 *
 * 度をラジアンに変換する関数が正しく動作するか検証します。
 */
TEST(CoordinateUtilsTest, DegToRad) {
  // 0度は 0 ラジアン
  EXPECT_DOUBLE_EQ(degToRad(0.0), 0.0);
  // 180度は π ラジアン
  EXPECT_DOUBLE_EQ(degToRad(180.0), M_PI);
  // 360度は 2π ラジアン
  EXPECT_NEAR(degToRad(360.0), 2 * M_PI, 1e-12);
  // 90度は π/2 ラジアン
  EXPECT_NEAR(degToRad(90.0), M_PI / 2, 1e-12);
}

/**
 * @brief radToDeg() のテスト
 *
 * ラジアンから度への変換が正しく行われるか検証します。
 */
TEST(CoordinateUtilsTest, RadToDeg) {
  // 0ラジアン -> 0度
  EXPECT_DOUBLE_EQ(radToDeg(0.0), 0.0);
  // π/2 ラジアン -> 90度
  EXPECT_NEAR(radToDeg(M_PI / 2), 90.0, 1e-12);
  // π ラジアン -> 180度
  EXPECT_DOUBLE_EQ(radToDeg(M_PI), 180.0);
  // 2π ラジアン -> 360度
  EXPECT_NEAR(radToDeg(2 * M_PI), 360.0, 1e-12);
}

/**
 * @brief calcN() のテスト
 *
 * 補助量 N
 * の計算が、与えられた楕円体パラメータおよび緯度に対して正しく行われるか検証します。
 */
TEST(CoordinateUtilsTest, CalcN) {
  // WGS84 のパラメータ
  double a = 6378137.0;
  double f = 1.0 / 298.257223563;
  double e2 = 2 * f - f * f;

  // 緯度 0 [ラジアン] では、N = a
  EXPECT_DOUBLE_EQ(calcN(a, e2, 0.0), a);

  // 緯度 45度 (π/4 ラジアン) の場合
  double lat = M_PI / 4;
  double expectedN = a / std::sqrt(1.0 - e2 * std::sin(lat) * std::sin(lat));
  EXPECT_NEAR(calcN(a, e2, lat), expectedN, 1e-6);

  // 緯度 90度 (π/2 ラジアン) の場合、sin(π/2) = 1 なので N = a / sqrt(1 - e2)
  lat = M_PI / 2;
  expectedN = a / std::sqrt(1.0 - e2);
  EXPECT_NEAR(calcN(a, e2, lat), expectedN, 1e-6);
}
};  // namespace trans_geo::utils::test