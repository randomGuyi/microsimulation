#!/bin/bash

SEARCH_DIR=${1:-.}
FILES=$(find "$SEARCH_DIR" -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.cc" \) -not -name "ui_*")

TMP_FILE=$(mktemp)

for file in $FILES; do
    current_ns=$(grep -E '^namespace [a-zA-Z0-9_:]+' "$file" | sed 's/namespace //;s/ {.*//;s/ //g' | head -n 1)

    if [ -n "$current_ns" ]; then
        grep -E "(using namespace [a-zA-Z0-9_:]+|[a-zA-Z0-9_:]+::[a-zA-Z0-9_]+)" "$file" | while read -r line; do
            
            if [[ "$line" == *"using namespace"* ]]; then
                dep=$(echo "$line" | sed -E 's/.*using namespace ([a-zA-Z0-9_:]+).*/\1/')
            else
                dep=$(echo "$line" | sed -E 's/.*[^a-zA-Z0-9_:][&]*([a-zA-Z0-9_:]+)::[a-zA-Z0-9_]+.*/\1/')
            fi

            dep=$(echo "$dep" | tr -d ';&*()')

            if [[ -n "$dep" ]] && [[ "$dep" != "$current_ns" ]] && [[ "$dep" == *"::"* ]]; then
                case "$dep" in
                    std*|boost*|Qt*|this|public|private|protected|return|gui_*) ;; # Ignorieren
                    *) echo "$current_ns|$dep" >> "$TMP_FILE" ;;
                esac
            fi
        done
    fi
done

echo "Namespace-Abhängigkeiten:"
echo "--------------------------------------------------"

sort -u "$TMP_FILE" | awk -F'|' '
    {
        if ($1 != last) {
            print "namespace " $1;
            last = $1;
        }
        print "     uses namespace " $2;
    }
'

rm "$TMP_FILE"
