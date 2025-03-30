#pragma once

#include <memory>

#include "converter/i_coordiante_converter.hpp"  // ICoordinateConverter インターフェースの定義
#include "coordinate/geo_coordinate.hpp"  // GeoCoordinate の定義
#include "ellipsoid/ellipsoid.hpp"        // Ellipsoid 構造体の定義

namespace trans_geo::conversion {

/**
 * @brief GeoCoordinate から ENUCoordinate への変換クラス
 *
 * このクラスは、入力の GeoCoordinate をまず Geo→ECEF 変換し、
 * その後 ECEF→ENU 変換を適用することで、GeoCoordinate を ENUCoordinate
 * に変換します。
 *
 * 変換に必要な楕円体モデルと原点 (GeoCoordinate)
 * はコンストラクタインジェクションにより渡されます。
 */
class GeoToENUConverter : public ICoordinateConverter {
 public:
  /**
   * @brief コンストラクタ
   * @param ellipsoid 変換に利用する楕円体モデル（例: WGS84）
   * @param origin    変換の基準となる原点 (GeoCoordinate 型)
   */
  GeoToENUConverter(const trans_geo::ellipsoid::Ellipsoid& ellipsoid,
                    const trans_geo::coordinate::GeoCoordinate& origin);

  /**
   * @brief 入力の GeoCoordinate を ENUCoordinate に変換する
   *
   * @param input 変換対象の座標。GeoCoordinate 型であることが期待されます。
   * @return std::unique_ptr<trans_geo::interface::ICoordinate> 変換後の
   * ENUCoordinate オブジェクト
   * @throw std::invalid_argument 入力が GeoCoordinate でない場合
   */
  std::unique_ptr<trans_geo::interface::ICoordinate> convert(
      const trans_geo::interface::ICoordinate& input) const override;

 private:
  trans_geo::ellipsoid::Ellipsoid ellipsoid_;
  trans_geo::coordinate::GeoCoordinate origin_;
  // 内部で再利用する変換器（Geo→ECEF と ECEF→ENU）
  std::unique_ptr<ICoordinateConverter> geoToEcefConverter_;
  std::unique_ptr<ICoordinateConverter> ecefToEnuConverter_;
};

}  // namespace trans_geo::conversion