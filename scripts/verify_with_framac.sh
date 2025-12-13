#!/bin/bash
FILE=src/legacy/http_parser_vuln.c
echo "=== Frama-C: HTTP parser ==="
frama-c -eva "$FILE"
frama-c -rte "$FILE" -then -eva
frama-c -wp "$FILE" -wp-rte
