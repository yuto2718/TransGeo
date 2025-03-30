#include "converter/ENU_to_ECEF_converter.hpp"

#include <Eigen/Dense>
#include <cmath>
#include <stdexcept>

#include "coordinate/ECEF_coordinate.hpp"  // ECEFCoordinate の定義
#include "coordinate/ENU_coordinate.hpp"   // ENUCoordinate の定義
#include "coordinate/geo_coordinate.hpp"   // GeoCoordinate の定義
#include "utils/utils.hpp"                 // degToRad, etc.

namespace trans_geo::conversion {

ENUToECEFConverter::ENUToECEFConverter(
    const trans_geo::ellipsoid::Ellipsoid& ellipsoid,
    const trans_geo::coordinate::GeoCoordinate& origin)
    : ellipsoid_(ellipsoid), origin_(origin) {}

std::unique_ptr<trans_geo::interface::ICoordinate> ENUToECEFConverter::convert(
    const trans_geo::interface::ICoordinate& input) const {
  // 入力が ENUCoordinate であることを確認
  const auto* enu =
      dynamic_cast<const trans_geo::coordinate::ENUCoordinate*>(&input);
  if (!enu) {
    throw std::invalid_argument(
        "ENUToECEFConverter::convert expects input to be an ENUCoordinate.");
  }

  // ENU 座標値を取得 (単位: メートル)
  double E = enu->getEast();
  double N = enu->getNorth();
  double U = enu->getUp();

  // 原点の GeoCoordinate から値を取得（緯度・経度は度、標高はメートル）
  std::vector<double> geoValues = origin_.getValues();
  if (geoValues.size() < 2) {
    throw std::invalid_argument(
        "Origin GeoCoordinate does not contain enough values.");
  }
  double lat_deg = geoValues[0];
  double lon_deg = geoValues[1];
  double h_origin = (geoValues.size() == 3) ? geoValues[2] : 0.0;

  // 緯度・経度をラジアンに変換
  double lat = trans_geo::utils::degToRad(lat_deg);
  double lon = trans_geo::utils::degToRad(lon_deg);

  // 楕円体モデルパラメータ
  double a = ellipsoid_.a;
  double e2 = ellipsoid_.e2;

  // 原点の ECEF 座標を計算（Geo → ECEF）
  double sinLat = std::sin(lat);
  double cosLat = std::cos(lat);
  double sinLon = std::sin(lon);
  double cosLon = std::cos(lon);
  double N_val = a / std::sqrt(1.0 - e2 * sinLat * sinLat);
  double X0 = (N_val + h_origin) * cosLat * cosLon;
  double Y0 = (N_val + h_origin) * cosLat * sinLon;
  double Z0 = ((1.0 - e2) * N_val + h_origin) * sinLat;

  // ENU → ECEF の変換のため、原点の緯度・経度から回転行列を構成する
  // ECEF_to_ENU の回転行列 R は下記の通り:
  // [ -sin(lon),              cos(lon),              0 ]
  // [ -sin(lat)*cos(lon),  -sin(lat)*sin(lon),   cos(lat) ]
  // [  cos(lat)*cos(lon),   cos(lat)*sin(lon),   sin(lat) ]
  // その転置 R^T が ENU → ECEF 変換行列となる
  Eigen::Matrix3d R;
  R(0, 0) = -sinLon;
  R(0, 1) = -sin(lat) * cosLon;
  R(0, 2) = cos(lat) * cosLon;
  R(1, 0) = cosLon;
  R(1, 1) = -sin(lat) * sinLon;
  R(1, 2) = cos(lat) * sinLon;
  R(2, 0) = 0.0;
  R(2, 1) = cos(lat);
  R(2, 2) = sin(lat);

  // ENU → ECEF の変換行列は R^T
  Eigen::Matrix3d R_transpose = R.transpose();

  // ENU 座標ベクトル
  Eigen::Vector3d enuVec(E, N, U);
  // ECEF 偏差ベクトル = R^T * (ENU)
  Eigen::Vector3d delta = R_transpose * enuVec;

  // 最終的な ECEF 座標 = 原点の ECEF 座標 + delta
  double X = X0 + delta(0);
  double Y = Y0 + delta(1);
  double Z = Z0 + delta(2);

  return std::make_unique<trans_geo::coordinate::ECEFCoordinate>(X, Y, Z);
}

}  // namespace trans_geo::conversion
