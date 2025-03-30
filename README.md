# TransGeo Coordinate Transformation Library

TransGeo は、地理座標の変換を行う C++ ライブラリです。

## 特徴

- **複数の座標系サポート**  
  Geo、ECEF、ENU など複数の座標系を実装。
- **GoogleTest による単体テスト**  
  各コンポーネントの正確性とエラーハンドリングをテスト済みです。

## ディレクトリ構成

- **converter/**  
  各変換器の実装
- **coordinate/**  
  座標系クラスの実装。
- **ellipsoid/**  
  楕円体モデル（WGS84 など）の定義。
- **utils/**  
  度・ラジアン変換、補助量計算などの共通ユーティリティ関数。
- **tests/**  
  各モジュールの単体テスト（GoogleTest 

## 依存ライブラリ

- **Eigen3**  
  Eigen は線形代数、行列・ベクトル計算、数値ソルバーなどを提供するヘッダオンリーの C++ テンプレートライブラリです。  
  本プロジェクトでは、変換処理で Eigen を利用しています。
  Eigen の公式サイト ([http://eigen.tuxfamily.org/](http://eigen.tuxfamily.org/)) から最新バージョンがダウンロード可能です。
  
- **GoogleTest**
GoogleTest (gtest) は C++ 用のユニットテストフレームワークであり、本プロジェクトでは各モジュールの単体テストに利用しています。

## 使用例

```C++
#include "converter.hpp"
#include "coordinate.hpp"
#include "ellipsoid/ellipsoid.hpp"
#include "utils/utils.hpp"

// Geo 座標の例（緯度, 経度, 標高）
trans_geo::coordinate::GeoCoordinate geo(35.6895, 139.6917, 50.0);

// Geo→ECEF 変換器の生成
auto geoToEcefConverter = trans_geo::conversion::GeoToECEFConverter(WGS84);

// 変換実行
auto ecefCoord = geoToEcefConverter.convert(geo);
```
