#pragma once

#include <memory>

#include "converter/i_coordiante_converter.hpp"
#include "ellipsoid/ellipsoid.hpp"

namespace trans_geo::conversion {
/**
 * @brief ECEFCoordinate から GeoCoordinate への変換クラス
 *
 * このクラスは、ECEF 座標を地理座標 (GeoCoordinate)
 * に変換する戦略クラスです。
 * 楕円体モデルはコンストラクタインジェクションにより渡されます。
 */
class ECEFToGeoConverter : public ICoordinateConverter {
 public:
  /**
   * @brief コンストラクタ
   * @param ellipsoid 変換に利用する楕円体モデル（例: WGS84）
   */
  explicit ECEFToGeoConverter(const trans_geo::ellipsoid::Ellipsoid& ellipsoid);

  /**
   * @brief 入力の ECEFCoordinate を GeoCoordinate に変換する
   *
   * @param input 変換対象の座標。ECEFCoordinate 型であることが期待されます。
   * @return std::unique_ptr<trans_geo::interface::ICoordinate> 変換後の
   * GeoCoordinate オブジェクト
   * @throw std::invalid_argument 入力が ECEFCoordinate
   * でない場合、または値の取得に失敗した場合
   */
  std::unique_ptr<trans_geo::interface::ICoordinate> convert(
      const trans_geo::interface::ICoordinate& input) const override;

 private:
  trans_geo::ellipsoid::Ellipsoid ellipsoid_;
};

}  // namespace trans_geo::conversion