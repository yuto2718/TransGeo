#pragma once
#include <cmath>

namespace trans_geo::utils {

/**
 * @brief 度をラジアンに変換する
 * @param deg 角度（度）
 * @return double 角度（ラジアン）
 */
constexpr double degToRad(double deg) noexcept { return deg * M_PI / 180.0; }

/**
 * @brief ラジアンを度に変換する
 * @param rad 角度（ラジアン）
 * @return double 角度（度）
 */
constexpr double radToDeg(double rad) noexcept { return rad * 180.0 / M_PI; }

/**
 * @brief 補助量 N（prime vertical radius of curvature）を計算する
 *
 * @param a 長半径
 * @param e2 離心率²
 * @param lat 緯度（ラジアン）
 * @return double 補助量 N
 */
inline double calcN(double a, double e2, double lat) {
  return a / std::sqrt(1.0 - e2 * std::sin(lat) * std::sin(lat));
}

}  // namespace trans_geo::utils