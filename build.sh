#!/bin/bash
set -e

# ビルドディレクトリ作成
mkdir -p build
cd build

# CMakeで構成＆ビルド（Ninjaがあれば使う）
if command -v ninja &> /dev/null; then
    cmake -G Ninja ..
    ninja
else
    cmake ..
    make -j$(nproc)
fi

echo "✅ Build complete!"