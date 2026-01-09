#!/bin/bash
# -------------------------------------------------------
# Author: Arthur Damböck
# 2025/2026
# -------------------------------------------------------

# input/output files
INPUT_FILE="./frames_architecture.svg"
OUTPUT_FILE="./frames_architecture_ids.txt"
OUTPUT_CONSTANTS="./architecture_ids.h"

# extract all IDs from SVG file, filter unwanted ones, and get unique IDs
IDS=$( grep -E "id=.*" frames_architecture.svg | \
      grep -Ev "(path.*|.*[[:digit:]]?-[[:digit:]]|g[[:digit:]]+.*|linearG.*|swatch.*|stop.*|.*namedview.*|.*showgrid.*|.*[[:digit:]]{3}.*|.*layer.*|.*defs[[:digit:]].*|.*grid[[:digit:]].*)" \
      | sed -E 's/.*id="([^"]+)".*/\1/' \
      | sort -u
)

# store id-file
echo $IDS | sed -e "s/[[:space:]]/ \n/g" > $OUTPUT_FILE
# generate C-header file with #defines (constants)
(
echo "// Auto-generated ID header"
echo "#ifndef ARCHITECTURE_IDS_H"
echo "#define ARCHITECTURE_IDS_H"
echo
while IFS= read -r id; do
  # dropTarget to comp
  ACT_ID=$(echo "$id" | sed -E 's/dropTarget/comp/g')
  MACRONAME="ID_$(echo "$ACT_ID" | tr '[:lower:]-' '[:upper:]_')"
  MACROSTR="$( echo "$ACT_ID" | grep -e 'comp_register.*' | sed -E 's/comp_//g')"
  if test -z $MACROSTR ; then
    MACROSTR=$ACT_ID
  fi
  # skip lines
  LINE_ID=$(echo "$MACROSTR" | grep -e 'line.*')
  CONN_ID=$(echo "$MACROSTR" | grep -e 'con_.*')
  if test -z $LINE_ID && test -z $CONN_ID; then
    echo "#define $MACRONAME \"$MACROSTR\""
  fi

  done <<< "$IDS"

echo
echo "#endif // ARCHITECTURE_IDS_H"
) > $OUTPUT_CONSTANTS
