#pragma once

#include <optional>
#include <string>
#include <vector>

#include "coordinate/interface.hpp"  // trans_geo::interface::ICoordinate の定義

namespace trans_geo::coordinate {
/**
 * @brief 地理座標系クラス
 *
 * 緯度、経度、および（オプションで）高度を保持するクラスです。
 * このクラスは trans_geo::interface::ICoordinate インターフェースを実装し、
 * 座標値の取得・設定の基本機能を提供します。
 */
class GeoCoordinate : public trans_geo::interface::ICoordinate {
 public:
  /**
   * @brief コンストラクタ（高度なし）
   * @param latitude 緯度（度単位）
   * @param longitude 経度（度単位）
   */
  explicit GeoCoordinate(double latitude, double longitude) noexcept;

  /**
   * @brief コンストラクタ（高度あり）
   * @param latitude 緯度（度単位）
   * @param longitude 経度（度単位）
   * @param altitude 高度（例：メートル単位）
   */
  explicit GeoCoordinate(double latitude, double longitude,
                         double altitude) noexcept;

  /**
   * @brief 座標値を取得する
   *
   * 座標値は [緯度, 経度] もしくは [緯度, 経度, 高度] の形式で返されます。
   *
   * @return std::vector<double> 座標値のベクトル
   */
  std::vector<double> getValues() const override;

  /**
   * @brief 座標値を設定する
   *
   * ベクトルの要素数が2の場合は [緯度, 経度] として、
   * 3の場合は [緯度, 経度, 高度] として設定します。
   * それ以外の場合は std::invalid_argument を投げます。
   *
   * @param values 設定する座標値のベクトル
   * @throw std::invalid_argument ベクトルのサイズが2または3でない場合
   */
  void setValues(const std::vector<double>& values) override;

  /**
   * @brief 座標情報の文字列表現を返す
   *
   * デバッグやログ出力の目的で、座標値を固定小数点形式で出力します。
   *
   * @return std::string 座標の文字列表現
   */
  std::string toString() const override;

  /**
   * @brief 緯度を取得する
   * @return double 緯度（度単位）
   */
  double getLatitude() const noexcept;

  /**
   * @brief 経度を取得する
   * @return double 経度（度単位）
   */
  double getLongitude() const noexcept;

  /**
   * @brief 高度を取得する
   *
   * 高度が設定されていない場合は std::nullopt が返ります。
   *
   * @return std::optional<double> 高度（オプション）
   */
  std::optional<double> getAltitude() const noexcept;

  /**
   * @brief 緯度を設定する
   * @param latitude 緯度（度単位）
   */
  void setLatitude(double latitude) noexcept;

  /**
   * @brief 経度を設定する
   * @param longitude 経度（度単位）
   */
  void setLongitude(double longitude) noexcept;

  /**
   * @brief 高度を設定する
   * @param altitude 高度（例：メートル単位）
   */
  void setAltitude(double altitude) noexcept;

  /**
   * @brief 仮想クローン
   *
   * このオブジェクトの完全なコピーを生成します。
   *
   * @return std::unique_ptr<trans_geo::interface::ICoordinate>
   * このオブジェクトのコピー
   */
  std::unique_ptr<trans_geo::interface::ICoordinate> clone() const override;

 private:
  double latitude_;   ///< 緯度（度単位）
  double longitude_;  ///< 経度（度単位）
  std::optional<double> altitude_;  ///< 高度（オプション、例：メートル単位）
};
}  // namespace trans_geo::coordinate
