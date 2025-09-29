#!/bin/bash

echo "=== 编译项目 ==="
make

echo "=== 运行内存检查 ==="
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./space_miner << EOF
q
EOF

echo "=== 测试完成 ==="
