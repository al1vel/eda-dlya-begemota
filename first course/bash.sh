#!/bin/bash

total_lines=0

for file in *.cpp *.h; do
    if [ -f "$file" ]; then
        lines=$(wc -l < "$file")
        total_lines=$((total_lines + lines))
    fi
done

echo "Total lines: $total_lines"
