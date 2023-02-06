#!/bin/bash

set -euo pipefail

if [[ $# -ne 1 ]] || [[ "$1" = "-h" ]] || [[ "$1" = "--help" ]]
then
    echo "Pass the measures as the argument (e.g. ./last-plot/data-0.txt)."
    exit
fi

log="$(readlink --canonicalize "$1")"

cd "$(dirname "${BASH_SOURCE[0]}")"

while read -r lines time
do
    size="$(zcat "./samples/sample-$lines.txt" | wc -c)"

    if ((size < 1000))
    then
        psize="$size"
        size_unit="B"
    elif ((size < 1000 * 1000))
    then
        psize=$(((size + 512) / 1000))
        size_unit="KB"
    elif ((size < 1000 * 1000 * 1000))
    then
        psize=$(((size + 512 * 1000) / (1000 * 1000)))
        size_unit="MB"
    else
        psize=$(((size + 512 * 1000 * 1000) / (1000 * 1000 * 1000)))
        size_unit="GB"
    fi

    if ((time < 1000))
    then
       ptime="$time"
       time_unit="ns."
    elif ((time < 1000 * 1000))
    then
       ptime=$(((time + 500) / 1000))
       time_unit="µs."
    elif ((time < 1000 * 1000 * 1000))
    then
       ptime=$(((time + 500 * 1000) / (1000 * 1000)))
       time_unit="ms."
    elif ((time < 60 * 1000 * 1000 * 1000))
    then
       ptime=$(((time + 500 * 1000 * 1000) / (1000 * 1000 * 1000)))
       time_unit="s."
    else
       ptime=$(((time + 30 * 1000 * 1000 * 1000) / (60 * 1000 * 1000 * 1000)))
       time_unit="minutes"
    fi

    throughput=$(((size * 1000000000) / (time * 1000 * 1000)))

    printf '%s (~%s %s) | %s %s | %s MB/s.\n' \
           "$lines" \
           "$psize" \
           "$size_unit" \
           "$ptime" \
           "$time_unit" \
           "$throughput"
done < "$log"
