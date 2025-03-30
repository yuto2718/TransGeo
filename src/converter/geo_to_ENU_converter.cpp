#include "converter/Geo_to_ENU_converter.hpp"

#include <stdexcept>

#include "converter/ECEF_to_ENU_converter.hpp"  // ECEFToENUConverter の定義
#include "converter/Geo_to_ECEF_converter.hpp"  // GeoToECEFConverter の定義
#include "coordinate/geo_coordinate.hpp"

namespace trans_geo::conversion {

GeoToENUConverter::GeoToENUConverter(
    const trans_geo::ellipsoid::Ellipsoid& ellipsoid,
    const trans_geo::coordinate::GeoCoordinate& origin)
    : ellipsoid_(ellipsoid), origin_(origin) {
  // 内部変換器を生成（コンストラクタインジェクションの楕円体モデルおよび原点を利用）
  geoToEcefConverter_ = std::make_unique<GeoToECEFConverter>(ellipsoid_);
  ecefToEnuConverter_ =
      std::make_unique<ECEFToENUConverter>(ellipsoid_, origin_);
}

std::unique_ptr<trans_geo::interface::ICoordinate> GeoToENUConverter::convert(
    const trans_geo::interface::ICoordinate& input) const {
  // 入力が GeoCoordinate であることを確認
  const auto* geo =
      dynamic_cast<const trans_geo::coordinate::GeoCoordinate*>(&input);
  if (!geo) {
    throw std::invalid_argument(
        "GeoToENUConverter::convert expects input to be a GeoCoordinate.");
  }
  // まず Geo→ECEF 変換
  auto ecefCoord = geoToEcefConverter_->convert(input);
  // 次に ECEF→ENU 変換
  auto enuCoord = ecefToEnuConverter_->convert(*ecefCoord);
  return enuCoord;
}

}  // namespace trans_geo::conversion