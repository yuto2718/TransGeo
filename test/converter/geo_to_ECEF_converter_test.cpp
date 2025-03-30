#include "converter/geo_to_ECEF_converter.hpp"  // GeoToECEFConverter の定義

#include <cmath>

#include "coordinate/ECEF_coordinate.hpp"  // ECEFCoordinate の定義
#include "coordinate/geo_coordinate.hpp"   // GeoCoordinate の定義
#include "ellipsoid/ellipsoid.hpp"         // Ellipsoid の定義
#include "gtest/gtest.h"

using namespace trans_geo::conversion;
using namespace trans_geo::coordinate;
using namespace trans_geo::ellipsoid;

/**
 * @brief GeoToECEFConverter 用のテストフィクスチャ
 */
class GeoToECEFConverterTest : public ::testing::Test {
 protected:
  Ellipsoid ellipsoid;
  GeoToECEFConverter* converter;

  GeoToECEFConverterTest() : ellipsoid(GRS80), converter(nullptr) {}

  void SetUp() override { converter = new GeoToECEFConverter(ellipsoid); }

  void TearDown() override { delete converter; }
};

/**
 * @brief 赤道上 (lat=0, lon=0, h=0) の場合のテスト
 *
 * GeoCoordinate (0, 0, 0) の変換結果は、ECEF 座標 (a, 0, 0) となるはずです。
 */
TEST_F(GeoToECEFConverterTest, ConvertAtEquator) {
  GeoCoordinate geo(0.0, 0.0);
  auto result = converter->convert(geo);
  auto ecef = dynamic_cast<ECEFCoordinate*>(result.get());
  ASSERT_NE(ecef, nullptr);
  EXPECT_NEAR(ecef->getX(), 6378137.0, 1e-3);
  EXPECT_NEAR(ecef->getY(), 0.0, 1e-3);
  EXPECT_NEAR(ecef->getZ(), 0.0, 1e-3);
}

/**
 * @brief 赤道上で高度がある場合のテスト
 *
 * GeoCoordinate (0, 0, 100) の変換結果は、ECEF 座標 (a+100, 0, 0)
 * となるはずです。
 */
TEST_F(GeoToECEFConverterTest, ConvertAtEquatorWithAltitude) {
  GeoCoordinate geo(0.0, 0.0, 100.0);
  auto result = converter->convert(geo);
  auto ecef = dynamic_cast<ECEFCoordinate*>(result.get());
  ASSERT_NE(ecef, nullptr);
  EXPECT_NEAR(ecef->getX(), 6378137.0 + 100.0, 1e-3);
  EXPECT_NEAR(ecef->getY(), 0.0, 1e-3);
  EXPECT_NEAR(ecef->getZ(), 0.0, 1e-3);
}

/**
 * @brief 緯度45度、経度45度 (h=0) の場合のテスト
 *
 * 正解値はGRS80を利用し
 * https://vldb.gsi.go.jp/sokuchi/surveycalc/surveycalc/transf.html　を利用し算出
 */
TEST_F(GeoToECEFConverterTest, ConvertAt45Degrees) {
  GeoCoordinate geo(45.0, 45.0, 0.0);
  auto result = converter->convert(geo);
  auto ecef = dynamic_cast<ECEFCoordinate*>(result.get());
  ASSERT_NE(ecef, nullptr);

  EXPECT_NEAR(ecef->getX(), 3194419.145, 1e-3);
  EXPECT_NEAR(ecef->getY(), 3194419.145, 1e-3);
  EXPECT_NEAR(ecef->getZ(), 4487348.409, 1e-3);
}

/**
 * @brief 不正な入力型の場合のテスト
 *
 * GeoToECEFConverter は GeoCoordinate 型の入力を期待しているため、
 * ECEFCoordinate 型など不正な入力の場合、例外がスローされることを検証します。
 */
TEST_F(GeoToECEFConverterTest, InvalidInputThrows) {
  // ECEFCoordinate を用いて GeoToECEFConverter::convert を呼び出す
  ECEFCoordinate ecef(1.0, 2.0, 3.0);
  EXPECT_THROW(converter->convert(ecef), std::invalid_argument);
}
