#include "converter/ENU_to_geo_converter.hpp"  // ENUToGeoConverter の定義

#include <cmath>
#include <memory>
#include <stdexcept>

#include "coordinate/ENU_coordinate.hpp"  // ENUCoordinate の定義
#include "coordinate/geo_coordinate.hpp"  // GeoCoordinate の定義
#include "ellipsoid/ellipsoid.hpp"        // Ellipsoid 構造体の定義
#include "gtest/gtest.h"

using namespace trans_geo::conversion;
using namespace trans_geo::coordinate;
using namespace trans_geo::ellipsoid;
using namespace trans_geo::interface;

/**
 * @brief ENU_to_geo_converter 用のテストフィクスチャ
 */
class ENUToGeoConverterTest : public ::testing::Test {
 protected:
  // WGS84 楕円体モデル（a=6378137.0, f=1/298.257223563）
  Ellipsoid ellipsoid{6378137.0, 1.0 / 298.257223563};
  // 原点として GeoCoordinate(0, 0, 0) を設定（緯度0、経度0、標高0）
  GeoCoordinate origin{0.0, 0.0, 0.0};
  ENUToGeoConverter* converter;

  void SetUp() override {
    converter = new ENUToGeoConverter(ellipsoid, origin);
  }
  void TearDown() override { delete converter; }
};

/**
 * @brief 入力 ENU 座標が原点と一致する場合、出力 Geo 座標は (0,0,0) となる
 *
 * 原点 GeoCoordinate(0,0,0) の場合、Geo→ECEF 変換結果は (6378137,0,0)
 * となりますが、 ENU 座標 (0,0,0) による差分はゼロとなり、結果として Geo
 * 座標は原点と同一となるはずです。
 */
TEST_F(ENUToGeoConverterTest, ZeroInputReturnsOriginGeo) {
  // 入力 ENU 座標 (0,0,0)
  ENUCoordinate enu(0.0, 0.0, 0.0, origin);
  auto result = converter->convert(enu);
  auto geo = dynamic_cast<GeoCoordinate*>(result.get());
  ASSERT_NE(geo, nullptr);
  // 結果は原点と同じ (0,0,0) となる
  EXPECT_NEAR(geo->getLatitude(), 0.0, 1e-6);
  EXPECT_NEAR(geo->getLongitude(), 0.0, 1e-6);
  EXPECT_NEAR(geo->getAltitude().value_or(0.0), 0.0, 1e-3);
}

/**
 * @brief 入力 ENU 座標に非ゼロのオフセットがある場合のテスト
 *
 * ここでは、入力 ENU 座標として (50, 20, 10) を与え、その結果として出力 Geo
 * 座標が 原点から適切な偏差を持つ値となることを検証します。 ※
 * 具体的な値は変換計算に依存するため、ここでは出力が原点と異なる（すなわちオフセットが反映される）ことを検証します。
 */
TEST_F(ENUToGeoConverterTest, NonZeroInputProducesOffsetGeo) {
  ENUCoordinate enu(50.0, 20.0, 10.0, origin);
  auto result = converter->convert(enu);
  auto geo = dynamic_cast<GeoCoordinate*>(result.get());
  ASSERT_NE(geo, nullptr);
  // 原点 (0,0,0) とは異なる値になることを確認（実際の値は計算結果に依存）
  double lat = geo->getLatitude();
  double lon = geo->getLongitude();
  double h = geo->getAltitude().value_or(0.0);
  EXPECT_FALSE(std::fabs(lat) < 1e-6 && std::fabs(lon) < 1e-6 &&
               std::fabs(h) < 1e-6);
}

/**
 * @brief 不正な入力型の場合のテスト
 *
 * ENUToGeoConverter は入力が ENUCoordinate 型であることを期待するため、
 * GeoCoordinate 型の入力を与えると例外 (std::invalid_argument)
 * が発生するはずです。
 */
TEST_F(ENUToGeoConverterTest, InvalidInputThrows) {
  GeoCoordinate geo(10.0, 20.0, 0.0);
  EXPECT_THROW(converter->convert(geo), std::invalid_argument);
}