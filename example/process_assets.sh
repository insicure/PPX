#!/bin/bash

SRC_DIR="nitrofs_raw"
DEST_DIR="nitrofs"
EXCLUDE_DIR="$SRC_DIR/atlas_raw"

mkdir -p "$SRC_DIR/atlas"
for dir in $SRC_DIR/atlas_raw/*/; do
  if [ -d "$dir" ]; then
    echo "Packing atlas: $dir"
    sillypacker "$dir" "$SRC_DIR/atlas" > /dev/null 2>&1
  fi
done

# set sillyimage format here
declare -A format_map
format_map["rgb16.png"]="rgb16"
format_map["rgb256.png"]="rgb256"
format_map["indexed4.png"]="indexed4"
format_map["indexed16.png"]="indexed16"
format_map["indexed256.png"]="indexed256"
format_map["indexed32a8.png"]="indexed32a8"
format_map["indexed8a32.png"]="indexed8a32"

format_map["nds12_0.png"]="indexed4"
format_map["nds12_1.png"]="indexed4"
format_map["nds12_2.png"]="indexed4"
format_map["player.png"]="indexed16"
format_map["TitleScreen.png"]="indexed256"
format_map["TitleScreen2.png"]="indexed256"

find "$SRC_DIR" \( \
  -path "$SRC_DIR/atlas_raw" -o \
  -path "$SRC_DIR/original" -o \
  -path "$SRC_DIR/exclude-folder-here" \
\) -prune -o -type f -print | while read -r src_file; do
  rel_path="${src_file#$SRC_DIR/}"
  dest_path="$DEST_DIR/$rel_path"
  dest_dir="$(dirname "$dest_path")"

  mkdir -p "$dest_dir"

  if [[ "$src_file" == *.png ]]; then
    filename="$(basename "$src_file")"
    format="${format_map[$filename]:-rgb16}"
    
    echo "Converting sillyimg: $filename -> $format"
    sillyimg -i "$src_file" -o "$dest_dir" -f "$format" > /dev/null 2>&1
  else
    cp "$src_file" "$dest_path"
  fi
done
