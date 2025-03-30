#include "coordinate/ECEF_coordinate.hpp"  // ECEFCoordinate クラスのヘッダ

#include <stdexcept>
#include <string>
#include <vector>

#include "gtest/gtest.h"

using namespace trans_geo::coordinate;

namespace trans_geo::coordinate::test {
/**
 * @brief ECEFCoordinate 用のテストフィクスチャ
 */
class ECEFCoordinateTest : public ::testing::Test {
 protected:
  ECEFCoordinate* coord;  ///< テスト対象の ECEFCoordinate オブジェクト

  void SetUp() override {
    // 初期値として [1000.0, 2000.0, 3000.0] を設定
    coord = new ECEFCoordinate(1000.0, 2000.0, 3000.0);
  }

  void TearDown() override { delete coord; }
};

/**
 * @brief コンストラクタと getter のテスト
 */
TEST_F(ECEFCoordinateTest, ConstructorAndGetters) {
  EXPECT_DOUBLE_EQ(coord->getX(), 1000.0);
  EXPECT_DOUBLE_EQ(coord->getY(), 2000.0);
  EXPECT_DOUBLE_EQ(coord->getZ(), 3000.0);
}

/**
 * @brief getValues() のテスト
 */
TEST_F(ECEFCoordinateTest, GetValues) {
  std::vector<double> values = coord->getValues();
  ASSERT_EQ(values.size(), 3);
  EXPECT_DOUBLE_EQ(values[0], 1000.0);
  EXPECT_DOUBLE_EQ(values[1], 2000.0);
  EXPECT_DOUBLE_EQ(values[2], 3000.0);
}

/**
 * @brief 有効な 3 要素ベクトルによる setValues() のテスト
 */
TEST_F(ECEFCoordinateTest, SetValuesValid) {
  std::vector<double> newValues = {4000.0, 5000.0, 6000.0};
  EXPECT_NO_THROW(coord->setValues(newValues));
  std::vector<double> values = coord->getValues();
  EXPECT_DOUBLE_EQ(values[0], 4000.0);
  EXPECT_DOUBLE_EQ(values[1], 5000.0);
  EXPECT_DOUBLE_EQ(values[2], 6000.0);
}

/**
 * @brief 不正なサイズのベクトルによる setValues() の例外テスト
 */
TEST_F(ECEFCoordinateTest, SetValuesInvalid) {
  std::vector<double> invalidValues = {
      7000.0, 8000.0};  // サイズが2、例外が発生するはず
  EXPECT_THROW(coord->setValues(invalidValues), std::invalid_argument);
}

/**
 * @brief toString() の出力形式のテスト
 */
TEST_F(ECEFCoordinateTest, ToStringTest) {
  std::string expected = "[1000.000000, 2000.000000, 3000.000000]";
  EXPECT_EQ(coord->toString(), expected);
}

/**
 * @brief 個別 setter の動作確認テスト
 */
TEST_F(ECEFCoordinateTest, IndividualSetters) {
  coord->setX(1111.0);
  coord->setY(2222.0);
  coord->setZ(3333.0);
  EXPECT_DOUBLE_EQ(coord->getX(), 1111.0);
  EXPECT_DOUBLE_EQ(coord->getY(), 2222.0);
  EXPECT_DOUBLE_EQ(coord->getZ(), 3333.0);
}

/**
 * @brief clone() メソッドのテスト
 *
 * clone()
 * により生成されたオブジェクトが正しく元オブジェクトと同じ状態で生成され、
 * なおかつディープコピーであること（クローン側の変更が元に影響しないこと）を検証します。
 */
TEST_F(ECEFCoordinateTest, CloneTest) {
  // clone() によりコピーを生成
  auto clonePtr = coord->clone();
  ASSERT_NE(clonePtr, nullptr)
      << "clone() により生成されたポインタが nullptr です。";

  // 元のオブジェクトと clone()
  // により生成されたオブジェクトの文字列表現が同一であることを確認
  EXPECT_EQ(clonePtr->toString(), coord->toString());

  // 動的キャストにより ECEFCoordinate 型に変換して各アクセサを利用
  ECEFCoordinate* clonedCoord = dynamic_cast<ECEFCoordinate*>(clonePtr.get());
  ASSERT_NE(clonedCoord, nullptr) << "clone() により生成されたオブジェクトが "
                                     "ECEFCoordinate 型ではありません。";

  // クローン側の値を変更し、元のオブジェクトと異なる状態になることを確認
  clonedCoord->setX(4000.0);
  clonedCoord->setY(5000.0);
  clonedCoord->setZ(6000.0);
  EXPECT_NE(clonedCoord->toString(), coord->toString())
      << "クローン側の変更が元のオブジェクトに影響しています。";
}
}  // namespace trans_geo::coordinate::test