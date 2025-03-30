#!/bin/bash
set -e

echo "🧹 Cleaning build/ directory..."
rm -rf build

# --all オプションで .cache も消す
if [[ "$1" == "--all" ]]; then
    echo "🧹 Cleaning .cache/ directory..."
    rm -rf .cache
fi

echo "✅ Clean complete."