#!/bin/bash

# Input- und Output-Dateien
INPUT_FILE="./frames_architecture.svg"
OUTPUT_FILE="./frames_architecture_ids.txt"
OUTPUT_CONSTANTS="./architecture_ids.h"

IDS=$( grep -E "id=.*" frames_architecture.svg | \
      grep -Ev "(path.*|.*[[:digit:]]?-[[:digit:]]|g[[:digit:]]+.*|linearG.*|swatch.*|stop.*|.*namedview.*|.*showgrid.*|.*[[:digit:]]{3}.*|.*layer.*|.*defs[[:digit:]].*|.*grid[[:digit:]].*)" \
      | sed -E 's/.*id="([^"]+)".*/\1/'
)


echo $IDS | sed -e "s/[[:space:]]/ \n/g" > $OUTPUT_FILE

(
echo "// Auto-generated ID header"
echo "#ifndef ARCHITECTURE_IDS_H"
echo "#define ARCHITECTURE_IDS_H"
echo

while IFS= read -r id; do
  macro=$(echo "$id" | tr '[:lower:]-' '[:upper:]_')
  echo "#define $macro \"$id\""
  done <<< "$IDS"

echo
echo "#endif // ARCHITECTURE_IDS_H"
) > $OUTPUT_CONSTANTS
