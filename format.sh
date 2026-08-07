#!/bin/sh
set -e

find -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
