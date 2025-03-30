#include "converter/ENU_to_geo_converter.hpp"

#include <stdexcept>

#include "converter/ECEF_to_geo_converter.hpp"  // ECEFToGeoConverter の定義
#include "converter/ENU_to_ECEF_converter.hpp"  // ENUToECEFConverter の定義
#include "coordinate/ENU_coordinate.hpp"        // ENUCoordinate の定義
#include "coordinate/geo_coordinate.hpp"        // GeoCoordinate の定義

namespace trans_geo::conversion {

ENUToGeoConverter::ENUToGeoConverter(
    const trans_geo::ellipsoid::Ellipsoid& ellipsoid,
    const trans_geo::coordinate::GeoCoordinate& origin)
    : ellipsoid_(ellipsoid), origin_(origin) {
  // 内部変換器を生成
  enuToEcefConverter_ =
      std::make_unique<ENUToECEFConverter>(ellipsoid_, origin_);
  ecefToGeoConverter_ = std::make_unique<ECEFToGeoConverter>(ellipsoid_);
}

std::unique_ptr<trans_geo::interface::ICoordinate> ENUToGeoConverter::convert(
    const trans_geo::interface::ICoordinate& input) const {
  // 入力が ENUCoordinate であることを確認
  const auto* enu =
      dynamic_cast<const trans_geo::coordinate::ENUCoordinate*>(&input);
  if (!enu) {
    throw std::invalid_argument(
        "ENUToGeoConverter::convert expects input to be an ENUCoordinate.");
  }
  // まず ENU→ECEF 変換
  auto ecefCoord = enuToEcefConverter_->convert(input);
  // 次に ECEF→Geo 変換
  auto geoCoord = ecefToGeoConverter_->convert(*ecefCoord);
  return geoCoord;
}

}  // namespace trans_geo::conversion