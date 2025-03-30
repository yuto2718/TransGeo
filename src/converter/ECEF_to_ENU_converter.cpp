#include "converter/ECEF_to_ENU_converter.hpp"

#include <Eigen/Dense>
#include <cmath>
#include <stdexcept>

#include "coordinate/ECEF_coordinate.hpp"  // ECEFCoordinate の定義
#include "coordinate/ENU_coordinate.hpp"   // ENUCoordinate の定義
#include "coordinate/geo_coordinate.hpp"   // GeoCoordinate の定義
#include "utils/utils.hpp"                 // degToRad, etc.

namespace trans_geo::conversion {

ECEFToENUConverter::ECEFToENUConverter(
    const trans_geo::ellipsoid::Ellipsoid& ellipsoid,
    const trans_geo::coordinate::GeoCoordinate& origin)
    : ellipsoid_(ellipsoid), origin_(origin) {}

std::unique_ptr<trans_geo::interface::ICoordinate> ECEFToENUConverter::convert(
    const trans_geo::interface::ICoordinate& input) const {
  // 入力が ECEFCoordinate であることを確認
  const auto* ecef =
      dynamic_cast<const trans_geo::coordinate::ECEFCoordinate*>(&input);
  if (!ecef) {
    throw std::invalid_argument(
        "ECEFToENUConverter::convert expects input to be an ECEFCoordinate.");
  }

  // ECEF 座標値を取得
  std::vector<double> ecefValues = ecef->getValues();
  if (ecefValues.size() != 3) {
    throw std::invalid_argument("ECEFCoordinate must have exactly 3 values.");
  }
  double X = ecefValues[0];
  double Y = ecefValues[1];
  double Z = ecefValues[2];

  // 原点の GeoCoordinate から値を取得（緯度,
  // 経度は度、オプションの高度はメートル）
  std::vector<double> geoValues = origin_.getValues();
  if (geoValues.size() < 2) {
    throw std::invalid_argument(
        "Origin GeoCoordinate does not contain enough values.");
  }
  double lat_deg = geoValues[0];
  double lon_deg = geoValues[1];
  double h_origin = (geoValues.size() == 3) ? geoValues[2] : 0.0;

  // 緯度・経度をラジアンに変換（共通ユーティリティを利用）
  double lat = trans_geo::utils::degToRad(lat_deg);
  double lon = trans_geo::utils::degToRad(lon_deg);

  // 楕円体モデルパラメータ
  double a = ellipsoid_.a;
  double e2 = ellipsoid_.e2;

  // 原点の ECEF 座標を計算（Geo→ECEF 変換）
  double sinLat = std::sin(lat);
  double cosLat = std::cos(lat);
  double sinLon = std::sin(lon);
  double cosLon = std::cos(lon);
  double N = a / std::sqrt(1.0 - e2 * sinLat * sinLat);
  double X0 = (N + h_origin) * cosLat * cosLon;
  double Y0 = (N + h_origin) * cosLat * sinLon;
  double Z0 = ((1.0 - e2) * N + h_origin) * sinLat;

  // 差分ベクトル d = [X - X0, Y - Y0, Z - Z0]
  double dX = X - X0;
  double dY = Y - Y0;
  double dZ = Z - Z0;

  // 回転行列 (ECEF -> ENU)
  // R =
  // [ -sin(lon),              cos(lon),             0 ]
  // [ -sin(lat)*cos(lon),   -sin(lat)*sin(lon),   cos(lat) ]
  // [  cos(lat)*cos(lon),    cos(lat)*sin(lon),   sin(lat) ]
  Eigen::Matrix3d R;
  R(0, 0) = -sinLon;
  R(0, 1) = cosLon;
  R(0, 2) = 0.0;
  R(1, 0) = -sinLat * cosLon;
  R(1, 1) = -sinLat * sinLon;
  R(1, 2) = cosLat;
  R(2, 0) = cosLat * cosLon;
  R(2, 1) = cosLat * sinLon;
  R(2, 2) = sinLat;

  // ECEF 差分ベクトル
  Eigen::Vector3d dVec(dX, dY, dZ);

  // ENU 座標 = R * dVec
  Eigen::Vector3d enuVec = R * dVec;
  double E = enuVec(0);
  double N_val = enuVec(1);
  double U = enuVec(2);

  // 変換結果として、ENUCoordinate を生成
  return std::make_unique<trans_geo::coordinate::ENUCoordinate>(E, N_val, U,
                                                                origin_);
}

}  // namespace trans_geo::conversion