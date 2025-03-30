#include "coordinate/geo_coordinate.hpp"  // GeoCoordinate クラスのヘッダ

#include "gtest/gtest.h"

using namespace trans_geo::coordinate;

namespace trans_geo::coordinate::test {
/**
 * @brief GeoCoordinate 用のテストフィクスチャ
 */
class GeoCoordinateTest : public ::testing::Test {
 protected:
  GeoCoordinate* coord2;  ///< 2要素座標オブジェクト（緯度・経度のみ）
  GeoCoordinate* coord3;  ///< 3要素座標オブジェクト（緯度・経度・高度）

  void SetUp() override {
    // 2要素の場合
    coord2 = new GeoCoordinate(35.6895, 139.6917);
    // 3要素の場合
    coord3 = new GeoCoordinate(35.6895, 139.6917, 100.0);
  }

  void TearDown() override {
    delete coord2;
    delete coord3;
  }
};

/**
 * @brief 2要素座標の getValues() のテスト
 */
TEST_F(GeoCoordinateTest, GetValuesTwoElement) {
  std::vector<double> values = coord2->getValues();
  ASSERT_EQ(values.size(), 2);
  EXPECT_DOUBLE_EQ(values[0], 35.6895);
  EXPECT_DOUBLE_EQ(values[1], 139.6917);
  EXPECT_EQ(coord2->getAltitude(), std::nullopt);
}

/**
 * @brief 3要素座標の getValues() のテスト
 */
TEST_F(GeoCoordinateTest, GetValuesThreeElement) {
  std::vector<double> values = coord3->getValues();
  ASSERT_EQ(values.size(), 3);
  EXPECT_DOUBLE_EQ(values[0], 35.6895);
  EXPECT_DOUBLE_EQ(values[1], 139.6917);
  EXPECT_DOUBLE_EQ(values[2], 100.0);
  ASSERT_TRUE(coord3->getAltitude().has_value());
  EXPECT_DOUBLE_EQ(coord3->getAltitude().value(), 100.0);
}

/**
 * @brief setValues() で2要素のベクトルを設定するテスト
 */
TEST_F(GeoCoordinateTest, SetValuesTwoElement) {
  std::vector<double> newValues = {40.0, 120.0};
  EXPECT_NO_THROW(coord2->setValues(newValues));
  std::vector<double> values = coord2->getValues();
  ASSERT_EQ(values.size(), 2);
  EXPECT_DOUBLE_EQ(values[0], 40.0);
  EXPECT_DOUBLE_EQ(values[1], 120.0);
  EXPECT_EQ(coord2->getAltitude(), std::nullopt);
}

/**
 * @brief setValues() で3要素のベクトルを設定するテスト
 */
TEST_F(GeoCoordinateTest, SetValuesThreeElement) {
  std::vector<double> newValues = {40.0, 120.0, 200.0};
  EXPECT_NO_THROW(coord3->setValues(newValues));
  std::vector<double> values = coord3->getValues();
  ASSERT_EQ(values.size(), 3);
  EXPECT_DOUBLE_EQ(values[0], 40.0);
  EXPECT_DOUBLE_EQ(values[1], 120.0);
  EXPECT_DOUBLE_EQ(values[2], 200.0);
  ASSERT_TRUE(coord3->getAltitude().has_value());
  EXPECT_DOUBLE_EQ(coord3->getAltitude().value(), 200.0);
}

/**
 * @brief 不正なサイズのベクトルを渡した場合の例外テスト
 */
TEST_F(GeoCoordinateTest, SetValuesInvalidSize) {
  std::vector<double> invalidValues = {10.0};
  EXPECT_THROW(coord2->setValues(invalidValues), std::invalid_argument);
}

/**
 * @brief toString() の出力フォーマットのテスト
 */
TEST_F(GeoCoordinateTest, ToStringTest) {
  std::string expected = "[35.689500, 139.691700, 100.000000]";
  EXPECT_EQ(coord3->toString(), expected);
}

/**
 * @brief 個別アクセサ（getter/setter）の動作確認
 */
TEST_F(GeoCoordinateTest, AccessorTest) {
  coord2->setLatitude(10.0);
  coord2->setLongitude(20.0);
  coord2->setAltitude(30.0);
  EXPECT_DOUBLE_EQ(coord2->getLatitude(), 10.0);
  EXPECT_DOUBLE_EQ(coord2->getLongitude(), 20.0);
  ASSERT_TRUE(coord2->getAltitude().has_value());
  EXPECT_DOUBLE_EQ(coord2->getAltitude().value(), 30.0);
}

/**
 * @brief 2要素座標オブジェクトの clone() メソッドのテスト
 */
TEST_F(GeoCoordinateTest, Clone2Element) {
  auto clone2 = coord2->clone();
  // clone() で生成されたオブジェクトが null でないことを確認
  ASSERT_NE(clone2, nullptr);
  // clone() されたオブジェクトの文字列表現が元と同じであることを確認
  EXPECT_EQ(clone2->toString(), coord2->toString());

  // 動的キャストにより GeoCoordinate 型に変換して個別のアクセサを使用
  GeoCoordinate* clone2Ptr = dynamic_cast<GeoCoordinate*>(clone2.get());
  ASSERT_NE(clone2Ptr, nullptr);
  // clone の緯度を変更
  clone2Ptr->setLatitude(40.0);
  // clone と元の toString() が異なることを確認（深いコピーであること）
  EXPECT_NE(clone2->toString(), coord2->toString());
}

/**
 * @brief 3要素座標オブジェクトの clone() メソッドのテスト
 */
TEST_F(GeoCoordinateTest, Clone3Element) {
  auto clone3 = coord3->clone();
  ASSERT_NE(clone3, nullptr);
  EXPECT_EQ(clone3->toString(), coord3->toString());

  GeoCoordinate* clone3Ptr = dynamic_cast<GeoCoordinate*>(clone3.get());
  ASSERT_NE(clone3Ptr, nullptr);
  // clone の高度を変更
  clone3Ptr->setAltitude(150.0);
  EXPECT_NE(clone3->toString(), coord3->toString());
}
}  // namespace trans_geo::coordinate::test