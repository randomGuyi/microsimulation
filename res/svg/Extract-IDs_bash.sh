#!/usr/bin/env bash

# Input- und Output-Dateien
INPUT_FILE="./frames_architecture.svg"
OUTPUT_FILE="./frames_architecture_ids.txt"

# IDs extrahieren, filtern und schreiben
grep -o 'id="[^"]*"' "$INPUT_FILE" |
    sed 's/id="\([^"]*\)"/\1/' |
    grep -Ev '^(g[0-9]+)$' \
        -e '^(path[0-9].*)$' \
        -e '^(svg[0-9]+)$' \
        -e '^true$' \
        -e '^false$' \
        -e '^(defs[0-9]+)$' \
        -e '^(namedview[0-9]+)$' \
        -e '^(grid[0-9]+)$' \
        -e '^(swatch[0-9]+)$' \
        -e '^(stop[0-9]+)$' \
        -e '^(linearGradient[0-9]+)$' \
        -e '^(layer[0-9]+)$' \
        | sed '/^\s*$/d' \
        > "$OUTPUT_FILE"

echo "IDs gespeichert: $OUTPUT_FILE"

