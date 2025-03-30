#include "converter/ENU_to_ECEF_converter.hpp"  // ENUToECEFConverter の定義

#include <memory>
#include <stdexcept>

#include "converter/i_coordiante_converter.hpp"  // ICoordinateConverter の定義
#include "coordinate/ECEF_coordinate.hpp"        // ECEFCoordinate の定義
#include "coordinate/ENU_coordinate.hpp"         // ENUCoordinate の定義
#include "coordinate/geo_coordinate.hpp"         // GeoCoordinate の定義
#include "ellipsoid/ellipsoid.hpp"               // Ellipsoid 構造体の定義
#include "gtest/gtest.h"

using namespace trans_geo::conversion;
using namespace trans_geo::coordinate;
using namespace trans_geo::ellipsoid;
using namespace trans_geo::interface;

// テストフィクスチャ
class ENUToECEFConverterTest : public ::testing::Test {
 protected:
  Ellipsoid ellipsoid{WGS84};
  // 原点：GeoCoordinate(0, 0, 0)（緯度0、経度0、標高0）
  GeoCoordinate origin{0.0, 0.0};
  ENUToECEFConverter* converter;

  void SetUp() override {
    converter = new ENUToECEFConverter(ellipsoid, origin);
  }

  void TearDown() override { delete converter; }
};

/**
 * @brief ENU 座標 (0, 0, 0) を入力した場合、出力 ECEF 座標は原点の ECEF
 * 座標となる
 *
 * GeoCoordinate(0,0,0) の Geo→ECEF 変換では (6378137, 0, 0) となるため、
 * 入力 ENU (0,0,0) では delta = (0,0,0) となり、最終結果は (6378137, 0, 0)
 * になる。
 */
TEST_F(ENUToECEFConverterTest, ZeroENUReturnsOriginECEF) {
  ENUCoordinate enu(0.0, 0.0, 0.0, origin);
  auto result = converter->convert(enu);
  auto ecef = dynamic_cast<ECEFCoordinate*>(result.get());
  ASSERT_NE(ecef, nullptr);
  EXPECT_NEAR(ecef->getX(), 6378137.0, 1e-3);
  EXPECT_NEAR(ecef->getY(), 0.0, 1e-3);
  EXPECT_NEAR(ecef->getZ(), 0.0, 1e-3);
}

/**
 * @brief ENU 座標 (100, 0, 0) を入力した場合のテスト
 *
 * 原点が GeoCoordinate(0,0,0) の場合、計算上の回転行列により、
 * delta = (N, U, E) = (0, 0, 100) となるため、最終 ECEF 座標は (6378137, 0,
 * 100) となるはず。
 */
TEST_F(ENUToECEFConverterTest, EastOffset) {
  ENUCoordinate enu(100.0, 0.0, 0.0, origin);
  auto result = converter->convert(enu);
  auto ecef = dynamic_cast<ECEFCoordinate*>(result.get());
  ASSERT_NE(ecef, nullptr);
  EXPECT_NEAR(ecef->getX(), 6378137.0, 1e-3);
  EXPECT_NEAR(ecef->getY(), 0.0, 1e-3);
  EXPECT_NEAR(ecef->getZ(), 100.0, 1e-3);
}

/**
 * @brief ENU 座標 (0, 50, 0) を入力した場合のテスト
 *
 * 入力 ENU (0,50,0) では delta = (50, 0, 0) となるため、
 * 出力 ECEF 座標は (6378137+50, 0, 0) となるはず。
 */
TEST_F(ENUToECEFConverterTest, NorthOffset) {
  ENUCoordinate enu(0.0, 50.0, 0.0, origin);
  auto result = converter->convert(enu);
  auto ecef = dynamic_cast<ECEFCoordinate*>(result.get());
  ASSERT_NE(ecef, nullptr);
  EXPECT_NEAR(ecef->getX(), 6378137.0 + 50.0, 1e-3);
  EXPECT_NEAR(ecef->getY(), 0.0, 1e-3);
  EXPECT_NEAR(ecef->getZ(), 0.0, 1e-3);
}

/**
 * @brief ENU 座標 (0, 0, 30) を入力した場合のテスト
 *
 * 入力 ENU (0,0,30) では delta = (0, 30, 0) となるため、
 * 出力 ECEF 座標は (6378137, 30, 0) となるはず。
 */
TEST_F(ENUToECEFConverterTest, UpOffset) {
  ENUCoordinate enu(0.0, 0.0, 30.0, origin);
  auto result = converter->convert(enu);
  auto ecef = dynamic_cast<ECEFCoordinate*>(result.get());
  ASSERT_NE(ecef, nullptr);
  EXPECT_NEAR(ecef->getX(), 6378137.0, 1e-3);
  EXPECT_NEAR(ecef->getY(), 30.0, 1e-3);
  EXPECT_NEAR(ecef->getZ(), 0.0, 1e-3);
}

/**
 * @brief 不正な入力の場合のテスト
 *
 * ENUToECEFConverter は入力が ENUCoordinate 型であることを期待するため、
 * GeoCoordinate 型の入力を与えると例外がスローされることを確認する。
 */
TEST_F(ENUToECEFConverterTest, InvalidInputThrows) {
  GeoCoordinate geo(10.0, 20.0, 0.0);
  EXPECT_THROW(converter->convert(geo), std::invalid_argument);
}