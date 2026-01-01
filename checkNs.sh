#!/bin/bash

SEARCH_DIR=${1:-.}
# Liste der Dateien ohne Qt-generierte Files
FILES=$(find "$SEARCH_DIR" -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.cc" \) -not -name "ui_*")

# Temporäre Datei für die Zuordnung
TMP_FILE=$(mktemp)

for file in $FILES; do
    # 1. Den Namespace der aktuellen Datei finden
    current_ns=$(grep -E '^namespace [a-zA-Z0-9_:]+' "$file" | sed 's/namespace //;s/ {.*//;s/ //g' | head -n 1)

    if [ -n "$current_ns" ]; then
        # 2. Alle potenziellen Abhängigkeiten extrahieren
        # Sucht nach 'using namespace' ODER 'Word::Word'
        grep -E "(using namespace [a-zA-Z0-9_:]+|[a-zA-Z0-9_:]+::[a-zA-Z0-9_]+)" "$file" | while read -r line; do
            
            # Extraktion: 
            # a) Falls 'using namespace' vorkommt -> nimm das Wort danach
            # b) Sonst nimm den Teil vor dem letzten '::'
            if [[ "$line" == *"using namespace"* ]]; then
                dep=$(echo "$line" | sed -E 's/.*using namespace ([a-zA-Z0-9_:]+).*/\1/')
            else
                # Extrahiere Pfad vor dem :: und entferne davorstehende Sonderzeichen/Variablen
                dep=$(echo "$line" | sed -E 's/.*[^a-zA-Z0-9_:][&]*([a-zA-Z0-9_:]+)::[a-zA-Z0-9_]+.*/\1/')
            fi

            # Bereinigung: Semikolons, Ampersands, etc.
            dep=$(echo "$dep" | tr -d ';&*()')

            # Filter: Keine Keywords, kein Eigenbezug, muss :: enthalten (für Pfade)
            if [[ -n "$dep" ]] && [[ "$dep" != "$current_ns" ]] && [[ "$dep" == *"::"* ]]; then
                case "$dep" in
                    std*|boost*|Qt*|this|public|private|protected|return|gui_*) ;; # Ignorieren
                    *) echo "$current_ns|$dep" >> "$TMP_FILE" ;;
                esac
            fi
        done
    fi
done

# 3. Ausgabe formatieren
echo "Namespace-Abhängigkeiten:"
echo "--------------------------------------------------"

# Sortiere eindeutige Paare und gruppiere sie bei der Ausgabe
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
