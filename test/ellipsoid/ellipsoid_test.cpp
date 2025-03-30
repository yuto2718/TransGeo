#include "ellipsoid/ellipsoid.hpp"  // Ellipsoid 構造体の定義があるファイル

#include <cmath>

#include "gtest/gtest.h"

namespace trans_geo::ellipsoid::test {
// WGS84 パラメータの場合のテスト
TEST(EllipsoidTest, WGS84Parameters) {
  // WGS84 の長半径および扁平率
  double a = 6378137.0;
  double f = 1.0 / 298.257223563;
  trans_geo::ellipsoid::Ellipsoid ellipsoid(a, f);

  // 各メンバの値が正しいことを検証
  EXPECT_DOUBLE_EQ(ellipsoid.a, a);
  EXPECT_DOUBLE_EQ(ellipsoid.f, f);

  // 離心率²の計算結果 (2f - f²)
  double expected_e2 = 2 * f - f * f;
  EXPECT_NEAR(ellipsoid.e2, expected_e2, 1e-12);
}

// f = 0 (球体モデル) の場合のテスト
TEST(EllipsoidTest, SphericalModel) {
  double a = 6378137.0;
  double f = 0.0;
  trans_geo::ellipsoid::Ellipsoid ellipsoid(a, f);

  // 球体の場合、扁平率 f=0 なので離心率² は 0 となる
  EXPECT_DOUBLE_EQ(ellipsoid.e2, 0.0);
}
}  // namespace trans_geo::ellipsoid::test