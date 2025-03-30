// converter/ECEF_to_ENU_converter_test.cpp
#include "converter/ECEF_to_ENU_converter.hpp"  // ECEFToENUConverter の定義

#include <memory>
#include <stdexcept>

#include "coordinate/ECEF_coordinate.hpp"  // ECEFCoordinate の定義
#include "coordinate/ENU_coordinate.hpp"   // ENUcoordinateの定義
#include "coordinate/geo_coordinate.hpp"   // GeoCoordinate の定義
#include "ellipsoid/ellipsoid.hpp"         // Ellipsoid 構造体の定義
#include "gtest/gtest.h"

using namespace trans_geo::conversion;
using namespace trans_geo::coordinate;
using namespace trans_geo::ellipsoid;
using namespace trans_geo::interface;

/**
 * @brief ECEFToENUConverter 用のテストフィクスチャ
 */
class ECEFToENUConverterTest : public ::testing::Test {
 protected:
  // 原点として GeoCoordinate(0, 0, 0) を設定（緯度0、経度0、標高0）
  GeoCoordinate origin{0.0, 0.0};
  ECEFToENUConverter* converter;

  void SetUp() override { converter = new ECEFToENUConverter(WGS84, origin); }

  void TearDown() override { delete converter; }
};

/**
 * @brief 原点と一致する場合のテスト
 *
 * 原点 GeoCoordinate(0,0,0) の ECEF 座標は (a, 0, 0) となる（a = 6378137.0）。
 * このため、入力 ECEFCoordinate(a,0,0) を与えると、出力 ENU 座標は (0,0,0)
 * となるはずです。
 */
TEST_F(ECEFToENUConverterTest, ConvertAtOrigin) {
  ECEFCoordinate ecef(WGS84.a, 0.0, 0.0);
  auto result = converter->convert(ecef);
  // 変換結果は ENUCoordinate 型となることを期待
  auto enu = dynamic_cast<ENUCoordinate*>(result.get());
  ASSERT_NE(enu, nullptr);
  // ENU 座標がすべて 0 であることを検証
  EXPECT_NEAR(enu->getEast(), 0.0, 1e-3);
  EXPECT_NEAR(enu->getNorth(), 0.0, 1e-3);
  EXPECT_NEAR(enu->getUp(), 0.0, 1e-3);
}

/**
 * @brief 原点から東方向のオフセットのテスト
 *
 * 原点の ECEF 座標は (a,0,0) なので、入力 ECEFCoordinate(a+100, 0, 0)
 * を与えると、 差分は (100, 0, 0) となる。回転行列により ENU 座標は (0, 0, 100)
 * となるはずである。
 */
TEST_F(ECEFToENUConverterTest, ConvertEastOffset) {
  ECEFCoordinate ecef(WGS84.a + 100.0, 0.0, 0.0);
  auto result = converter->convert(ecef);
  auto enu = dynamic_cast<ENUCoordinate*>(result.get());
  ASSERT_NE(enu, nullptr);
  // 期待値：東、北成分は 0、上成分は 100 m
  EXPECT_NEAR(enu->getEast(), 0.0, 1e-3);
  EXPECT_NEAR(enu->getNorth(), 0.0, 1e-3);
  EXPECT_NEAR(enu->getUp(), 100.0, 1e-3);
}

/**
 * @brief 不正な入力の場合のテスト
 *
 * ECEFToENUConverter は入力が ECEFCoordinate 型であることを期待するため、
 * GeoCoordinate 型の入力を与えると例外がスローされることを確認する。
 */
TEST_F(ECEFToENUConverterTest, InvalidInputThrows) {
  GeoCoordinate geo(10.0, 20.0, 0.0);
  EXPECT_THROW(converter->convert(geo), std::invalid_argument);
}
