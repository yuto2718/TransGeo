#pragma once

#include <memory>

#include "converter/i_coordiante_converter.hpp"  // ICoordinateConverter インターフェースの定義
#include "coordinate/geo_coordinate.hpp"  // GeoCoordinate の定義
#include "ellipsoid/ellipsoid.hpp"        // Ellipsoid 構造体の定義

namespace trans_geo::conversion {

/**
 * @brief ECEF から ENU への変換クラス
 *
 * このクラスは、ECEF
 * 座標（地球中心・地球固定座標）を、指定された原点（GeoCoordinate 型）を基準に
 * ローカルな ENU 座標に変換します。
 * 楕円体モデルはコンストラクタインジェクションで渡されます。
 */
class ECEFToENUConverter : public ICoordinateConverter {
 public:
  /**
   * @brief コンストラクタ
   *
   * @param ellipsoid 変換に利用する楕円体モデル（例: WGS84）
   * @param origin    変換の基準となる原点（GeoCoordinate 型）
   */
  ECEFToENUConverter(const trans_geo::ellipsoid::Ellipsoid& ellipsoid,
                     const trans_geo::coordinate::GeoCoordinate& origin);

  /**
   * @brief 入力の ECEFCoordinate を ENUCoordinate に変換する
   *
   * @param input 変換対象の座標。ECEFCoordinate 型であることが期待されます。
   * @return std::unique_ptr<trans_geo::interface::ICoordinate> 変換後の
   * ENUCoordinate オブジェクト
   * @throw std::invalid_argument 入力が ECEFCoordinate
   * でない場合、または値の取得に失敗した場合
   */
  std::unique_ptr<trans_geo::interface::ICoordinate> convert(
      const trans_geo::interface::ICoordinate& input) const override;

 private:
  trans_geo::ellipsoid::Ellipsoid ellipsoid_;
  trans_geo::coordinate::GeoCoordinate origin_;
};

}  // namespace trans_geo::conversion