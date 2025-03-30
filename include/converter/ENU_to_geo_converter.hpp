#pragma once

#include <memory>

#include "converter/i_coordiante_converter.hpp"  // ICoordinateConverter インターフェースの定義
#include "coordinate/geo_coordinate.hpp"  // GeoCoordinate の定義
#include "ellipsoid/ellipsoid.hpp"        // Ellipsoid 構造体の定義

namespace trans_geo::conversion {

/**
 * @brief ENU から Geo への変換クラス
 *
 * このクラスは、入力の ENU 座標（ENUCoordinate 型）を、内部で
 * ENU→ECEF 変換および ECEF→Geo 変換を順次適用することで、Geo 座標に変換します。
 *
 * 楕円体モデルおよび変換の基準となる原点 (GeoCoordinate 型)
 * はコンストラクタインジェクションにより渡されます。
 */
class ENUToGeoConverter : public ICoordinateConverter {
 public:
  /**
   * @brief コンストラクタ
   *
   * @param ellipsoid 変換に利用する楕円体モデル（例: WGS84）
   * @param origin    変換の基準となる原点 (GeoCoordinate 型)
   */
  ENUToGeoConverter(const trans_geo::ellipsoid::Ellipsoid& ellipsoid,
                    const trans_geo::coordinate::GeoCoordinate& origin);

  /**
   * @brief 入力の ENUCoordinate を GeoCoordinate に変換する
   *
   * @param input 変換対象の座標。ENUCoordinate 型であることが期待されます。
   * @return std::unique_ptr<trans_geo::interface::ICoordinate> 変換後の
   * GeoCoordinate オブジェクト
   * @throw std::invalid_argument 入力が ENUCoordinate でない場合
   */
  std::unique_ptr<trans_geo::interface::ICoordinate> convert(
      const trans_geo::interface::ICoordinate& input) const override;

 private:
  trans_geo::ellipsoid::Ellipsoid ellipsoid_;
  trans_geo::coordinate::GeoCoordinate origin_;
  // 内部で再利用する変換器
  std::unique_ptr<ICoordinateConverter> enuToEcefConverter_;
  std::unique_ptr<ICoordinateConverter> ecefToGeoConverter_;
};

}  // namespace trans_geo::conversion