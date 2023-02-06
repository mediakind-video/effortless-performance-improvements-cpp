#!/bin/bash

set -euo pipefail

if [[ 1 -ne "$(cat /sys/devices/system/cpu/intel_pstate/no_turbo)" ]]
then
    (
        echo "Disable CPU scaling before running the script."
        echo "echo 1 | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo"
    ) 1>&2
    exit 1
fi

labels=(""
        "baseline"
        "map::find()"
        "unordered\\\\_map"
        "tokenize reserve"
        "tokenize string\\\\_view"
        "no-temp concatenate\\\\_tokens"
        "tokenize reuse vector"
        "no iostream"
        "unordered\\\\_map<string\\\\_view>"
        "replace batch"
        "robin\\\\_map"
        "custom to\\\\_string()"
        "replace w/o new strings")

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")"; pwd)"
max_sample_size="${1:-100000000}"
runs=10

tmp_dir="$(mktemp -d)/plot-$(date --iso-8601=seconds | tr ':' '-')"
mkdir -p "$tmp_dir"

clean_up()
{
    local working_directory
    working_directory="$(pwd)"
    local plot_dir
    plot_dir="$(basename "$tmp_dir")"

    cd "$tmp_dir/.."

    tar cfz "$working_directory/$plot_dir".tgz "$plot_dir"
    cd - >/dev/null

    rm -fr "last-plot"
    tar xf "$plot_dir.tgz"
    mv "$plot_dir" "last-plot"

    rm -fr "$tmp_dir"
}

trap clean_up EXIT

service_count="$(find "$script_dir"/build/release/ \
                      -maxdepth 1 \
                      -name "run_service_*" \
                   | wc -l)"

for (( n=10; n <= max_sample_size; n *= 10 ))
do
    for service in "$script_dir"/build/release/run_service_*
    do
        i="$(echo "$service" | sed 's/.*_\([0-9]\+$\)/\1/')"

        echo -n "service $service, n=$n ."

        t="$(zcat "$script_dir/samples/sample-$n.txt.gz" \
                 | "$service" - /dev/null \
                 | grep ^time \
                 | cut -d' ' -f2)"

        local_runs="$runs"
        if [[ "$n" -le 10000 ]]
        then
            local_runs=$((runs * 2))
        fi

        for (( r=1; r < local_runs; ++r ))
        do
            echo -n .
            tr="$(zcat "$script_dir/samples/sample-$n.txt.gz" \
                      | "$service" - /dev/null \
                      | grep ^time \
                      | cut -d' ' -f2)"
            if [[ "$tr" -lt "$t" ]]
            then
                t="$tr"
            fi
        done

        echo
        echo "$n $t" >> "$tmp_dir/data-$i.txt"
    done
done

cp "$tmp_dir/data-0.txt" "$tmp_dir/join-left.txt"

for (( i=1; i != service_count; ++i))
do
    LANG=C join "$tmp_dir"/join-left.txt "$tmp_dir/data-$i.txt" \
        > "$tmp_dir"/join-tmp.txt
    mv "$tmp_dir"/join-tmp.txt "$tmp_dir/join-left.txt"
done

mv "$tmp_dir/join-left.txt" "$tmp_dir/all.txt"

generate_absolute_plot_subcommands()
{
    local n="$1"

    for (( i=2; i <= n; ++i ))
    do
        printf '   , "" using 1:%s title "%s" with linespoints \\\n' \
               $((i + 1)) "${labels[$i]}"
    done
}

generate_absolute_plot_commands()
{
    for (( i=1; i < service_count; ++i ))
    do
        cat <<EOF
set output script_directory."absolute-$i.png"
plot script_directory."/all.txt" using 1:2 title "${labels[1]}" with lines \
$(generate_absolute_plot_subcommands "$i")

EOF
    done
}

generate_relative_plot_subcommands()
{
    local n="$1"

    for (( i=1; i <= n; ++i ))
    do
        printf '   , "" using 1:($%s/$2) title "%s" with linespoints \\\n' \
               $((i + 2)) "${labels[$((i + 1))]}"
    done
}

generate_relative_plot_commands()
{
    for (( i=1; i < service_count; ++i ))
    do
        cat <<EOF
set output script_directory."relative-$i.png"
plot script_directory."all.txt" using 1:(1) title "baseline" with lines \
$(generate_relative_plot_subcommands "$i")

EOF
    done
}

cat - > "$tmp_dir/plot.gp" <<EOF
set terminal pngcairo font "Sans,12" linewidth 3 size 1280,720

script_directory = system("dirname ".ARG0)."/"

set key Left right reverse outside

set xlabel "Sample size"
set logscale x


set ylabel "Absolute processing time"

set title "Absolute processing time, best of $runs runs."

set xrange [1000:*]
set yrange [*:*]

$(generate_absolute_plot_commands)

set ylabel "Relative processing time"

set title "Relative performance, best of $runs runs."

set yrange [0:1.1]

$(generate_relative_plot_commands)
EOF

gnuplot "$tmp_dir/plot.gp"
