#pragma once

#include <string>
#include <vector>

#include "coordinate/interface.hpp"  // trans_geo::interface::ICoordinate の定義

namespace trans_geo::coordinate {
/**
 * @brief Earth-Centered, Earth-Fixed (ECEF) 座標系クラス
 *
 * このクラスは、地球中心・地球固定座標系 (ECEF) の座標値（x, y,
 * z）を保持します。 trans_geo::interface::ICoordinate
 * インターフェースを実装し、
 * 座標値の取得／設定や文字列表現の生成などの基本機能を提供します。
 */
class ECEFCoordinate : public trans_geo::interface::ICoordinate {
 public:
  /**
   * @brief コンストラクタ
   * @param x X座標（メートル単位）
   * @param y Y座標（メートル単位）
   * @param z Z座標（メートル単位）
   */
  explicit ECEFCoordinate(double x, double y, double z) noexcept;

  /**
   * @brief 座標値を取得する
   *
   * 座標値は [x, y, z] の順で返されます。
   *
   * @return std::vector<double> 座標値のベクトル
   */
  std::vector<double> getValues() const override;

  /**
   * @brief 座標値を設定する
   *
   * 入力ベクトルは [x, y, z] の形式であり、必ず要素数が3でなければなりません。
   *
   * @param values [x, y, z] を含むベクトル
   * @throw std::invalid_argument ベクトルのサイズが3でない場合
   */
  void setValues(const std::vector<double>& values) override;

  /**
   * @brief 座標情報の文字列表現を返す
   *
   * 座標値を固定小数点形式で出力し、 "[x, y, z]" の形式の文字列を返します。
   *
   * @return std::string 座標の文字列表現
   */
  std::string toString() const override;

  /**
   * @brief X座標を取得する
   * @return double X座標（メートル単位）
   */
  double getX() const noexcept;

  /**
   * @brief Y座標を取得する
   * @return double Y座標（メートル単位）
   */
  double getY() const noexcept;

  /**
   * @brief Z座標を取得する
   * @return double Z座標（メートル単位）
   */
  double getZ() const noexcept;

  /**
   * @brief X座標を設定する
   * @param x X座標（メートル単位）
   */
  void setX(double x) noexcept;

  /**
   * @brief Y座標を設定する
   * @param y Y座標（メートル単位）
   */
  void setY(double y) noexcept;

  /**
   * @brief Z座標を設定する
   * @param z Z座標（メートル単位）
   */
  void setZ(double z) noexcept;

  /**
   * @brief 仮想クローン
   *
   * この ECEFCoordinate オブジェクトの完全なコピーを生成します。
   *
   * @return std::unique_ptr<trans_geo::interface::ICoordinate>
   * このオブジェクトのコピー
   */
  std::unique_ptr<trans_geo::interface::ICoordinate> clone() const override;

 private:
  double x_;  ///< X座標（メートル単位）
  double y_;  ///< Y座標（メートル単位）
  double z_;  ///< Z座標（メートル単位）
};
}  // namespace trans_geo::coordinate