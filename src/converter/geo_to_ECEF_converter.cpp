#include "converter/geo_to_ECEF_converter.hpp"

#include <Eigen/Dense>
#include <cmath>
#include <stdexcept>

#include "coordinate/ECEF_coordinate.hpp"  // ECEFCoordinate の定義
#include "coordinate/geo_coordinate.hpp"   // GeoCoordinate の定義
#include "utils/utils.hpp"

namespace trans_geo::conversion {

GeoToECEFConverter::GeoToECEFConverter(
    const trans_geo::ellipsoid::Ellipsoid& ellipsoid)
    : ellipsoid_(ellipsoid) {}

std::unique_ptr<trans_geo::interface::ICoordinate> GeoToECEFConverter::convert(
    const trans_geo::interface::ICoordinate& input) const {
  // 入力が GeoCoordinate であることを確認
  const auto* geo =
      dynamic_cast<const trans_geo::coordinate::GeoCoordinate*>(&input);
  if (!geo) {
    throw std::invalid_argument(
        "GeoToECEFConverter::convert expects input to be a GeoCoordinate.");
  }

  // GeoCoordinate から値を取得（単位:
  // 緯度・経度は度、オプションの高度はメートル）
  std::vector<double> geoValues = geo->getValues();
  double lat_deg = geoValues[0];
  double lon_deg = geoValues[1];
  double h = (geoValues.size() == 3) ? geoValues[2] : 0.0;

  // 度 -> ラジアン変換
  double lat = trans_geo::utils::degToRad(lat_deg);
  double lon = trans_geo::utils::degToRad(lon_deg);

  // 楕円体モデルパラメータ
  double a = ellipsoid_.a;
  double e2 = ellipsoid_.e2;

  // 補助量 N (prime vertical radius of curvature)
  double sinLat = std::sin(lat);
  double cosLat = std::cos(lat);
  double N_val = trans_geo::utils::calcN(a, e2, lat);

  // ECEF 座標計算
  double x = (N_val + h) * cosLat * std::cos(lon);
  double y = (N_val + h) * cosLat * std::sin(lon);
  double z = ((1.0 - e2) * N_val + h) * sinLat;

  return std::make_unique<trans_geo::coordinate::ECEFCoordinate>(x, y, z);
}

}  // namespace trans_geo::conversion