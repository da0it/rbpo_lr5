#!/bin/bash
FILE=tests/formal/http_parser_harness.c
echo "=== ESBMC: HTTP parser ==="

# Общая проверка
esbmc "$FILE" --unwind 50 --overflow-check --bounds-check --pointer-check

# Отдельно: только границы
esbmc "$FILE" --unwind 50 --bounds-check --no-pointer-check --no-div-by-zero-check

# Отдельно: только указатели
esbmc "$FILE" --unwind 50 --pointer-check --no-bounds-check --no-div-by-zero-check
