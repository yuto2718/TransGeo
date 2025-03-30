#pragma once

#include <memory>

#include "coordinate/interface.hpp"  // trans_geo::interface::ICoordinate の定義

namespace trans_geo::conversion {

/**
 * @brief 座標変換器の抽象インターフェース
 *
 * すべての変換戦略は、このインターフェースの convert() を実装します。
 */
class ICoordinateConverter {
 public:
  /**
   * @brief 入力座標を変換する
   *
   * @param input 変換対象の座標オブジェクト
   * @return std::unique_ptr<trans_geo::interface::ICoordinate>
   * 変換後の座標オブジェクト
   */
  virtual std::unique_ptr<trans_geo::interface::ICoordinate> convert(
      const trans_geo::interface::ICoordinate& input) const = 0;

  virtual ~ICoordinateConverter() = default;
};
}  // namespace trans_geo::conversion