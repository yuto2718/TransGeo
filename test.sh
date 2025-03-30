#!/bin/bash
set -e

./build.sh

# ビルドディレクトリに移動
cd build

# 論理コア数を取得（macOS/Linux 両対応）
NUM_CORES=$(nproc 2>/dev/null || sysctl -n hw.logicalcpu)

# テスト実行（CTest使用）
ctest --output-on-failure -j "$NUM_CORES"

echo "✅ All tests passed!"