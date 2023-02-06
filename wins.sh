#!/bin/bash

set -euo pipefail

echo "Wins (percents) relative to baseline"

< "$1"/all.txt \
  awk '{
       printf("%9d", $1);
       for (i = 3; i <= NF; ++i) {
         r=100 * ($i-$2)/$2;
         printf(" %3d", int(r + ((r > 0) ? 0.5 : -0.5)))
       }
       printf("\n");
     }' | tail -n +3 | (
    max=(-100 -100 -100 -100 -100 -100 -100 -100 -100 -100 -100 -100)
    min=(0 0 0 0 0 0 0 0 0 0 0 0)
    while IFS=$'\n' read -r line
    do
        a=($line)
        echo "$line"
        for i in {1..12}
        do
            if [[ "${a[$i]}" -lt "${min[$((i-1))]}" ]]
            then
                min[$((i-1))]=${a[$i]}
            fi
            if [[ "${a[$i]}" -gt "${max[$((i-1))]}" ]]
            then
                max[$((i-1))]=${a[$i]}
            fi
        done
    done
    echo "max      $(printf ' %3d' "${max[@]}")"
    echo "min      $(printf ' %3d' "${min[@]}")"
)

echo "Wins (percents) relative to previous"

< "$1"/all.txt \
  awk '{
       printf("%9d", $1);
       for (i = 3; i <= NF; ++i) {
         r=100 * ($i-$(i-1))/$(i-1);
         printf(" %3d", int(r + ((r > 0) ? 0.5 : -0.5)))
       }
       printf("\n");
     }' | tail -n +3 | (
    max=(-100 -100 -100 -100 -100 -100 -100 -100 -100 -100 -100 -100)
    min=(0 0 0 0 0 0 0 0 0 0 0 0)
    while IFS=$'\n' read -r line
    do
        a=($line)
        echo "$line"
        for i in {1..12}
        do
            if [[ "${a[$i]}" -lt "${min[$((i-1))]}" ]]
            then
                min[$((i-1))]=${a[$i]}
            fi
            if [[ "${a[$i]}" -gt "${max[$((i-1))]}" ]]
            then
                max[$((i-1))]=${a[$i]}
            fi
        done
    done
    echo "max      $(printf ' %3d' "${max[@]}")"
    echo "min      $(printf ' %3d' "${min[@]}")"
)
