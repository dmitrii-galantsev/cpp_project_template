#!/usr/bin/env bash

FILES=$(find . -iregex '.*\.\(cc\|h\|hh\|cpp\|c\)$' -and -not -iregex '\./\(tests\|build\)/.*')
clang-tidy --format-style=file -p build/ --fix-notes --fix-errors $FILES
