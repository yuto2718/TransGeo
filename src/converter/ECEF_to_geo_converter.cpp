#include "converter/ECEF_to_geo_converter.hpp"

#include <Eigen/Dense>
#include <cmath>
#include <stdexcept>

#include "coordinate/ECEF_coordinate.hpp"  // ECEFCoordinate の定義
#include "coordinate/geo_coordinate.hpp"   // GeoCoordinate の定義
#include "utils/utils.hpp"

namespace trans_geo::conversion {

ECEFToGeoConverter::ECEFToGeoConverter(
    const trans_geo::ellipsoid::Ellipsoid& ellipsoid)
    : ellipsoid_(ellipsoid) {}

std::unique_ptr<trans_geo::interface::ICoordinate> ECEFToGeoConverter::convert(
    const trans_geo::interface::ICoordinate& input) const {
  // 入力が ECEFCoordinate であることを確認
  const auto* ecef =
      dynamic_cast<const trans_geo::coordinate::ECEFCoordinate*>(&input);
  if (!ecef) {
    throw std::invalid_argument(
        "ECEFToGeoConverter::convert expects input to be an ECEFCoordinate.");
  }

  // ECEF 座標値を取得
  std::vector<double> ecefValues = ecef->getValues();
  if (ecefValues.size() != 3) {
    throw std::invalid_argument("ECEFCoordinate must have exactly 3 values.");
  }
  double X = ecefValues[0];
  double Y = ecefValues[1];
  double Z = ecefValues[2];

  // 楕円体パラメータ
  double a = ellipsoid_.a;
  double e2 = ellipsoid_.e2;

  // 中間変数 p: X-Y 平面上の距離
  double p = std::sqrt(X * X + Y * Y);

  // 経度は atan2 で直接求める（ラジアン）
  double lon = std::atan2(Y, X);

  // 初期値として緯度を求める（簡易初期値）
  double lat = std::atan2(Z, p * (1.0 - e2));
  double lat_prev = 0.0;
  int iter = 0, maxIter = 100;
  constexpr double tol = 1e-12;
  // 反復法で緯度を収束させる
  while (std::fabs(lat - lat_prev) > tol && iter < maxIter) {
    lat_prev = lat;
    double sinLat = std::sin(lat);
    double N = a / std::sqrt(1.0 - e2 * sinLat * sinLat);
    lat = std::atan2(Z + e2 * N * sinLat, p);
    iter++;
  }
  // 補助量 N の再計算
  double sinLat = std::sin(lat);
  double N = a / std::sqrt(1.0 - e2 * sinLat * sinLat);
  // 高度 h の計算
  double h = p / std::cos(lat) - N;

  // ラジアン -> 度変換
  double lat_deg = trans_geo::utils::radToDeg(lat);
  double lon_deg = trans_geo::utils::radToDeg(lon);

  return std::make_unique<trans_geo::coordinate::GeoCoordinate>(lat_deg,
                                                                lon_deg, h);
}

}  // namespace trans_geo::conversion