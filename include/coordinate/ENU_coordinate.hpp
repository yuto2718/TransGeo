#pragma once

#include <memory>
#include <string>
#include <vector>

#include "coordinate/interface.hpp"  // trans_geo::interface::ICoordinate, ICoordinateWithOrigin の定義

namespace trans_geo::coordinate {
/**
 * @brief 東・北・上 (ENU) 座標系クラス
 *
 * このクラスは、ローカル座標系である ENU (East, North, Up) 座標系を表現します。
 * ENU 座標系は、指定された原点に対する東、北、上の座標値を保持します。
 * 原点は trans_geo::interface::ICoordinate
 * 型のオブジェクトで表現され、GeoCoordinate などを利用可能です。 本クラスは
 * trans_geo::interface::ICoordinateWithOrigin インターフェースを実装します。
 */
class ENUCoordinate : public trans_geo::interface::ICoordinateWithOrigin {
 public:
  /**
   * @brief コンストラクタ
   *
   * @param east 東方向の座標値（メートル単位）
   * @param north 北方向の座標値（メートル単位）
   * @param up 上方向の座標値（メートル単位）
   * @param origin 原点座標オブジェクト。原点の座標は、ICoordinate の
   * getValues() により 2 もしくは 3 要素で表現される必要があります。
   * @throw std::invalid_argument origin の getValues() のサイズが 2 または 3
   * でない場合
   */
  ENUCoordinate(double east, double north, double up,
                const trans_geo::interface::ICoordinate& origin);

  /**
   * @brief ENU 座標値を取得する
   *
   * 座標値は [east, north, up] の順で返されます。
   *
   * @return std::vector<double> 座標値のベクトル
   */
  std::vector<double> getValues() const override;

  /**
   * @brief ENU 座標値を設定する
   *
   * 入力ベクトルは [east, north, up] の形式であり、必ず要素数が 3
   * でなければなりません。
   *
   * @param values [east, north, up] を含むベクトル
   * @throw std::invalid_argument ベクトルのサイズが 3 でない場合
   */
  void setValues(const std::vector<double>& values) override;

  /**
   * @brief 座標情報の文字列表現を返す
   *
   * ENU 座標値を固定小数点形式で出力し、"[east, north, up]"
   * の形式の文字列を返します。
   *
   * @return std::string 座標の文字列表現
   */
  std::string toString() const override;

  /**
   * @brief 原点座標を取得する
   *
   * 原点は trans_geo::interface::ICoordinate 型のオブジェクトで表現されます。
   *
   * @return std::shared_ptr<trans_geo::interface::ICoordinate>
   * 原点座標オブジェクト
   */
  std::unique_ptr<trans_geo::interface::ICoordinate> getOrigin() const override;

  /**
   * @brief 原点座標を設定する
   *
   * 原点は trans_geo::interface::ICoordinate
   * 型のオブジェクトで表現されなければなりません。
   *
   * @param origin 原点座標オブジェクト
   * @throw std::invalid_argument origin の getValues() のサイズが 2 または 3
   * でない場合
   */
  void setOrigin(const trans_geo::interface::ICoordinate& origin) override;

  /**
   * @brief 東方向の座標値を取得する
   * @return double 東方向の座標値（メートル単位）
   */
  double getEast() const noexcept;

  /**
   * @brief 北方向の座標値を取得する
   * @return double 北方向の座標値（メートル単位）
   */
  double getNorth() const noexcept;

  /**
   * @brief 上方向の座標値を取得する
   * @return double 上方向の座標値（メートル単位）
   */
  double getUp() const noexcept;

  /**
   * @brief 東方向の座標値を設定する
   * @param east 東方向の座標値（メートル単位）
   */
  void setEast(double east) noexcept;

  /**
   * @brief 北方向の座標値を設定する
   * @param north 北方向の座標値（メートル単位）
   */
  void setNorth(double north) noexcept;

  /**
   * @brief 上方向の座標値を設定する
   * @param up 上方向の座標値（メートル単位）
   */
  void setUp(double up) noexcept;

  /**
   * @brief 仮想クローン
   *
   * この ENUCoordinate オブジェクトのコピーを生成します。
   *
   * @return std::unique_ptr<trans_geo::interface::ICoordinate>
   * このオブジェクトのコピー
   */
  std::unique_ptr<trans_geo::interface::ICoordinate> clone() const override;

 private:
  double east_;   ///< 東方向の座標値（メートル単位）
  double north_;  ///< 北方向の座標値（メートル単位）
  double up_;     ///< 上方向の座標値（メートル単位）
  std::shared_ptr<trans_geo::interface::ICoordinate>
      origin_;  ///< 原点座標オブジェクト
};
}  // namespace trans_geo::coordinate