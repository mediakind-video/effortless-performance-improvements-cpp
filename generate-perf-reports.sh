#!/bin/bash

set -euo pipefail

if [[ -1 -ne "$(cat /proc/sys/kernel/perf_event_paranoid)" ]] \
       || [[ 0 -ne "$(cat /proc/sys/kernel/kptr_restrict)" ]]
then
    (
        echo "Disable perf event restrictions before running the script."
        echo "echo -1 | sudo tee /proc/sys/kernel/perf_event_paranoid"
        echo "echo 0 | sudo tee /proc/sys/kernel/kptr_restrict"
    ) 1>&2
    exit 1
fi

cd "$(dirname "${BASH_SOURCE[0]}")"

find ./build/release/ -maxdepth 1 -name "run_service_*" -executable \
    | while read -r s
do
    i="$(basename "$s" | tr '_' '\n' | tail -n 1)"
    echo "$i"

    zcat ./samples/sample-10000000.txt.gz \
        | perf record \
               -g \
               --call-graph dwarf \
               --output perf.data \
               "$s" \
               - \
               /dev/null
    perf report --stdio --input perf.data > "perf-report/service_$i.txt"
done
