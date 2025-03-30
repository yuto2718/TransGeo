#include "converter/ECEF_to_geo_converter.hpp"  // ECEFToGeoConverter の定義

#include <cmath>

#include "converter/geo_to_ECEF_converter.hpp"  // ラウンドトリップ用に Geo→ECEF 変換器を利用
#include "coordinate/ECEF_coordinate.hpp"  // ECEFCoordinate の定義
#include "coordinate/geo_coordinate.hpp"   // GeoCoordinate の定義
#include "ellipsoid/ellipsoid.hpp"         // Ellipsoid 構造体の定義
#include "gtest/gtest.h"

using namespace trans_geo::conversion;
using namespace trans_geo::coordinate;
using namespace trans_geo::ellipsoid;

// テストフィクスチャ
class ECEFToGeoConverterTest : public ::testing::Test {
 protected:
  Ellipsoid ellipsoid{WGS84};
  ECEFToGeoConverter* converter;

  void SetUp() override { converter = new ECEFToGeoConverter(ellipsoid); }

  void TearDown() override { delete converter; }
};

/**
 * @brief Equator (lat=0, lon=0, h=0) のテスト
 *
 * 入力として ECEFCoordinate(a, 0, 0) を与えると、
 * GeoCoordinate は (0°, 0°, 0) に変換されるはずです。
 */
TEST_F(ECEFToGeoConverterTest, ConvertAtEquator) {
  double a = ellipsoid.a;  // 6378137.0
  ECEFCoordinate ecef(a, 0.0, 0.0);
  auto geo_ptr = converter->convert(ecef);
  auto geo = dynamic_cast<GeoCoordinate*>(geo_ptr.get());
  ASSERT_NE(geo, nullptr);

  // 緯度 0°, 経度 0°、高度 0 が期待される
  EXPECT_NEAR(geo->getLatitude(), 0.0, 1e-6);
  EXPECT_NEAR(geo->getLongitude(), 0.0, 1e-6);
  EXPECT_NEAR(geo->getAltitude().value_or(0.0), 0.0, 1e-3);
}

/**
 * @brief Equator with altitude のテスト
 *
 * 入力として ECEFCoordinate(a+100, 0, 0) を与えると、
 * GeoCoordinate の高度が約 100 m となるはずです。
 */
TEST_F(ECEFToGeoConverterTest, ConvertAtEquatorWithAltitude) {
  double a = ellipsoid.a;  // 6378137.0
  double h = 100.0;
  ECEFCoordinate ecef(a + h, 0.0, 0.0);
  auto geo_ptr = converter->convert(ecef);
  auto geo = dynamic_cast<GeoCoordinate*>(geo_ptr.get());
  ASSERT_NE(geo, nullptr);

  EXPECT_NEAR(geo->getLatitude(), 0.0, 1e-6);
  EXPECT_NEAR(geo->getLongitude(), 0.0, 1e-6);
  // 高度がほぼ 100 m になる
  EXPECT_NEAR(geo->getAltitude().value_or(0.0), 100.0, 1e-2);
}

/**
 * @brief ラウンドトリップテスト
 *
 * 既知の GeoCoordinate (45°, 45°, 0) を Geo→ECEF 変換し、その結果を ECEF→Geo
 * 変換して 元の GeoCoordinate に近い値が得られるか検証します。
 */
TEST_F(ECEFToGeoConverterTest, RoundTripTest) {
  // 元の GeoCoordinate
  GeoCoordinate originalGeo(45.0, 45.0, 0.0);
  // Geo→ECEF 変換器（同じ楕円体モデルを使用）
  GeoToECEFConverter geo2ecef(ellipsoid);
  auto ecef_ptr = geo2ecef.convert(originalGeo);
  auto ecef = dynamic_cast<ECEFCoordinate*>(ecef_ptr.get());
  ASSERT_NE(ecef, nullptr);

  // ECEF→Geo 変換
  auto convertedGeo_ptr = converter->convert(*ecef);
  auto convertedGeo = dynamic_cast<GeoCoordinate*>(convertedGeo_ptr.get());
  ASSERT_NE(convertedGeo, nullptr);

  // 角度は小数点以下2桁程度の精度で一致すれば十分（反復法の収束誤差などを考慮）
  EXPECT_NEAR(convertedGeo->getLatitude(), 45.0, 1e-4);
  EXPECT_NEAR(convertedGeo->getLongitude(), 45.0, 1e-4);
  EXPECT_NEAR(convertedGeo->getAltitude().value_or(0.0), 0.0, 1e-2);
}

/**
 * @brief 不正な入力の場合のテスト
 *
 * ECEFToGeoConverter は入力が ECEFCoordinate 型であることを期待するため、
 * 例えば GeoCoordinate 型の入力では例外がスローされることを検証します。
 */
TEST_F(ECEFToGeoConverterTest, InvalidInputThrows) {
  GeoCoordinate geo(10.0, 20.0, 0.0);
  EXPECT_THROW(converter->convert(geo), std::invalid_argument);
}