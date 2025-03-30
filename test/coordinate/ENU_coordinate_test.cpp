#include "coordinate/ENU_coordinate.hpp"  // ENUCoordinate クラスの定義

#include "coordinate/geo_coordinate.hpp"  // GeoCoordinate（ICoordinate の具体実装）の定義
#include "coordinate/interface.hpp"
#include "gtest/gtest.h"

using namespace trans_geo::coordinate;
using namespace trans_geo::interface;

namespace trans_geo::coordinate::test {
/**
 * @brief ENUCoordinate 用のテストフィクスチャ
 */
class ENUCoordinateTest : public ::testing::Test {
 protected:
  // テスト用の原点として GeoCoordinate を利用（3要素: 緯度, 経度, 高度）
  std::unique_ptr<ICoordinate> origin;
  std::unique_ptr<ENUCoordinate> enu;

  void SetUp() override {
    // 例えば、東京付近の座標を原点として設定
    origin = std::make_unique<GeoCoordinate>(35.0, 139.0, 50.0);
    enu = std::make_unique<ENUCoordinate>(100.0, 200.0, 300.0, *origin);
  }
};  // namespace trans_geo::coordinate::test

/**
 * @brief コンストラクタと各 getter の動作確認
 */
TEST_F(ENUCoordinateTest, ConstructorAndGetters) {
  EXPECT_DOUBLE_EQ(enu->getEast(), 100.0);
  EXPECT_DOUBLE_EQ(enu->getNorth(), 200.0);
  EXPECT_DOUBLE_EQ(enu->getUp(), 300.0);

  auto origClone = enu->getOrigin();
  auto origVals = origClone->getValues();
  // 初期原点は GeoCoordinate により 3 要素である前提
  ASSERT_EQ(origVals.size(), 3);
  EXPECT_DOUBLE_EQ(origVals[0], 35.0);
  EXPECT_DOUBLE_EQ(origVals[1], 139.0);
  EXPECT_DOUBLE_EQ(origVals[2], 50.0);
}

/**
 * @brief getValues() のテスト
 */
TEST_F(ENUCoordinateTest, GetValues) {
  std::vector<double> vals = enu->getValues();
  ASSERT_EQ(vals.size(), 3);
  EXPECT_DOUBLE_EQ(vals[0], 100.0);
  EXPECT_DOUBLE_EQ(vals[1], 200.0);
  EXPECT_DOUBLE_EQ(vals[2], 300.0);
}

/**
 * @brief toString() の出力形式テスト
 */
TEST_F(ENUCoordinateTest, ToStringTest) {
  std::string expected = "[100.000000, 200.000000, 300.000000]";
  EXPECT_EQ(enu->toString(), expected);
}

/**
 * @brief setValues() の有効な入力テスト
 */
TEST_F(ENUCoordinateTest, SetValuesValid) {
  std::vector<double> newVals = {400.0, 500.0, 600.0};
  EXPECT_NO_THROW(enu->setValues(newVals));
  std::vector<double> vals = enu->getValues();
  EXPECT_DOUBLE_EQ(vals[0], 400.0);
  EXPECT_DOUBLE_EQ(vals[1], 500.0);
  EXPECT_DOUBLE_EQ(vals[2], 600.0);
}

/**
 * @brief setValues() の不正な入力による例外テスト
 */
TEST_F(ENUCoordinateTest, SetValuesInvalid) {
  std::vector<double> invalidVals = {700.0,
                                     800.0};  // サイズが 2 なら例外が発生
  EXPECT_THROW(enu->setValues(invalidVals), std::invalid_argument);
}

/**
 * @brief 原点アクセサ（getOrigin() / setOrigin()）の動作確認
 */
TEST_F(ENUCoordinateTest, OriginAccessors) {
  // 初期原点の検証
  auto origClone = enu->getOrigin();
  auto origVals = origClone->getValues();
  ASSERT_EQ(origVals.size(), 3);
  EXPECT_DOUBLE_EQ(origVals[0], 35.0);
  EXPECT_DOUBLE_EQ(origVals[1], 139.0);
  EXPECT_DOUBLE_EQ(origVals[2], 50.0);

  // 新たな原点として 2 要素の GeoCoordinate
  // を設定（2要素版コンストラクタが用意されている前提） ※ もし 2
  // 要素版が無い場合は、3 要素版で高さを 0 とするなどの対応が必要です。
  auto newOrigin = std::make_unique<GeoCoordinate>(36.0, 140.0);
  EXPECT_NO_THROW(enu->setOrigin(*newOrigin));
  auto updatedOrigin = enu->getOrigin();
  auto updatedVals = updatedOrigin->getValues();
  ASSERT_EQ(updatedVals.size(), 2);
  EXPECT_DOUBLE_EQ(updatedVals[0], 36.0);
  EXPECT_DOUBLE_EQ(updatedVals[1], 140.0);
}

/**
 * @brief clone() メソッドの動作確認
 */
TEST_F(ENUCoordinateTest, CloneTest) {
  // clone() によりコピーを生成
  std::unique_ptr<ICoordinate> enuCloneBase = enu->clone();
  // clone() の戻り値は ICoordinate 型となるが、toString() や getValues()
  // で比較可能
  EXPECT_EQ(enuCloneBase->toString(), enu->toString());

  auto valsOriginal = enu->getValues();
  auto valsClone = enuCloneBase->getValues();
  ASSERT_EQ(valsOriginal.size(), valsClone.size());
  for (size_t i = 0; i < valsOriginal.size(); i++) {
    EXPECT_DOUBLE_EQ(valsOriginal[i], valsClone[i]);
  }

  // 原点についても、clone() で取得したオブジェクトは同じ値を持つことを確認
  auto enuCastesd = dynamic_cast<ENUCoordinate*>(enuCloneBase.get());
  auto origVals = enu->getOrigin()->getValues();
  auto cloneOrigVals = enuCastesd->getOrigin()->getValues();
  ASSERT_EQ(origVals.size(), cloneOrigVals.size());
  for (size_t i = 0; i < origVals.size(); i++) {
    EXPECT_DOUBLE_EQ(origVals[i], cloneOrigVals[i]);
  }

  // clone 後、クローン側の値を変更しても元のオブジェクトに影響がないことを確認
  enuCloneBase->setValues({700.0, 800.0, 900.0});
  EXPECT_NE(enuCloneBase->toString(), enu->toString());
}
}  // namespace trans_geo::coordinate::test
