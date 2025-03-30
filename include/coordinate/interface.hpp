#pragma once

#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace trans_geo::interface {

/**
 * @brief 座標系の基本インターフェース
 *
 * すべての座標系クラスで共通して利用する操作を定義します。
 */
class ICoordinate {
 public:
  virtual ~ICoordinate() = default;

  /**
   * @brief 座標値を取得する
   *
   * 例: [x, y, z] または [緯度, 経度, 高度] の形式で返します。
   *
   * @return std::vector<double> 座標値のベクトル
   */
  virtual std::vector<double> getValues() const = 0;

  /**
   * @brief 座標値を設定する
   *
   * 例: [x, y, z] または [緯度, 経度, 高度] の形式で受け取ります。
   *
   * @param values 設定する座標値のベクトル
   */
  virtual void setValues(const std::vector<double>& values) = 0;

  /**
   * @brief 座標情報の文字列表現を返す
   *
   * デバッグやログ出力の目的で、座標値を固定小数点形式で出力します。
   *
   * @return std::string 座標の文字列表現
   */
  virtual std::string toString() const {
    auto values = getValues();
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6) << "[";
    for (size_t i = 0; i < values.size(); ++i) {
      oss << values[i];
      if (i != values.size() - 1) {
        oss << ", ";
      }
    }
    oss << "]";
    return oss.str();
  }

  /**
   * @brief 仮想クローン
   *
   * このオブジェクトの完全なコピーを生成します。
   *
   * @return std::unique_ptr<ICoordinate> このオブジェクトのコピー
   */
  virtual std::unique_ptr<ICoordinate> clone() const = 0;
};

/**
 * @brief 原点情報が必要な座標系向けの拡張インターフェース
 *
 * ENU 座標系など、原点情報を必要とする座標系に対応します。
 */
class ICoordinateWithOrigin : public ICoordinate {
 public:
  virtual ~ICoordinateWithOrigin() = default;

  /**
   * @brief 原点座標を取得する
   *
   * @return std::unique_ptr<ICoordinate> 原点座標のコピー
   */
  virtual std::unique_ptr<ICoordinate> getOrigin() const = 0;

  /**
   * @brief 原点座標を設定する
   *
   * @param origin 原点座標オブジェクト（コピー渡し）
   * @throw std::invalid_argument origin の getValues() のサイズが 2 または 3
   * でない場合
   */
  virtual void setOrigin(const ICoordinate& origin) = 0;
};
}  // namespace trans_geo::interface