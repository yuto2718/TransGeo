#include "converter/Geo_to_ENU_converter.hpp"  // GeoToENUConverter の定義

#include <memory>
#include <stdexcept>

#include "coordinate/ECEF_coordinate.hpp"  // ECEFCoordinate の定義（不正入力テスト用）
#include "coordinate/ENU_coordinate.hpp"  // ENUCoordinate の定義
#include "coordinate/geo_coordinate.hpp"  // GeoCoordinate の定義
#include "ellipsoid/ellipsoid.hpp"        // Ellipsoid 構造体の定義
#include "gtest/gtest.h"

using namespace trans_geo::conversion;
using namespace trans_geo::coordinate;
using namespace trans_geo::ellipsoid;
using namespace trans_geo::interface;

// テストフィクスチャ
class GeoToENUConverterTest : public ::testing::Test {
 protected:
  Ellipsoid ellipsoid{WGS84};
  // 原点として GeoCoordinate(0, 0, 0) を設定（緯度0、経度0、標高0）
  GeoCoordinate origin{0.0, 0.0, 0.0};
  GeoToENUConverter* converter;

  void SetUp() override {
    converter = new GeoToENUConverter(ellipsoid, origin);
  }
  void TearDown() override { delete converter; }
};

/**
 * @brief 入力 GeoCoordinate が原点と一致する場合、出力 ENU 座標は (0,0,0)
 * となる
 */
TEST_F(GeoToENUConverterTest, ZeroInputReturnsZeroENU) {
  GeoCoordinate geo_input(0.0, 0.0, 0.0);
  auto result = converter->convert(geo_input);
  auto enu = dynamic_cast<ENUCoordinate*>(result.get());
  ASSERT_NE(enu, nullptr);
  EXPECT_NEAR(enu->getEast(), 0.0, 1e-6);
  EXPECT_NEAR(enu->getNorth(), 0.0, 1e-6);
  EXPECT_NEAR(enu->getUp(), 0.0, 1e-6);
}

/**
 * @brief 入力 GeoCoordinate が原点と異なる場合、出力 ENU 座標が非ゼロとなる
 *
 * ここでは入力を GeoCoordinate(1.0, 1.0, 0.0) とし、変換結果が全軸で 0
 * ではないことを検証する。
 */
TEST_F(GeoToENUConverterTest, NonZeroInputProducesNonZeroENU) {
  GeoCoordinate geo_input(1.0, 1.0, 0.0);
  auto result = converter->convert(geo_input);
  auto enu = dynamic_cast<ENUCoordinate*>(result.get());
  ASSERT_NE(enu, nullptr);
  double sum = std::abs(enu->getEast()) + std::abs(enu->getNorth()) +
               std::abs(enu->getUp());
  EXPECT_GT(sum, 0.0);
}

/**
 * @brief 不正な入力型の場合、例外がスローされることを検証する
 *
 * GeoToENUConverter は GeoCoordinate 型の入力を期待するため、
 * ECEFCoordinate 型の入力を与えると例外 (std::invalid_argument)
 * が発生するはず。
 */
TEST_F(GeoToENUConverterTest, InvalidInputThrows) {
  ECEFCoordinate ecef(1000.0, 2000.0, 3000.0);
  EXPECT_THROW(converter->convert(ecef), std::invalid_argument);
}