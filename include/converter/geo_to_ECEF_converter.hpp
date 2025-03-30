#pragma once

#include <memory>

#include "converter/i_coordiante_converter.hpp"
#include "ellipsoid/ellipsoid.hpp"  // Ellipsoid 構造体の定義

namespace trans_geo::conversion {

/**
 * @brief GeoCoordinate から ECEFCoordinate への変換クラス
 *
 * このクラスは、GeoCoordinate（地理座標）を楕円体モデルに基づいて
 * ECEFCoordinate（地球中心・地球固定座標）に変換します。
 * 楕円体モデルはコンストラクタインジェクションにより渡されます。
 */
class GeoToECEFConverter : public ICoordinateConverter {
 public:
  /**
   * @brief コンストラクタ
   *
   * @param ellipsoid 変換に利用する楕円体モデル
   */
  explicit GeoToECEFConverter(const trans_geo::ellipsoid::Ellipsoid& ellipsoid);

  /**
   * @brief GeoCoordinate を ECEFCoordinate に変換する
   *
   * @param input 変換対象の座標。GeoCoordinate 型であることが期待されます。
   * @return std::unique_ptr<trans_geo::interface::ICoordinate> 変換後の
   * ECEFCoordinate オブジェクト
   * @throw std::invalid_argument 入力が GeoCoordinate でない場合
   */
  std::unique_ptr<trans_geo::interface::ICoordinate> convert(
      const trans_geo::interface::ICoordinate& input) const override;

 private:
  trans_geo::ellipsoid::Ellipsoid ellipsoid_;
};

}  // namespace trans_geo::conversion