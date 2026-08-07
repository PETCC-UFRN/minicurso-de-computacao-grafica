#!/bin/bash

mkdir -p frames

cycles=2

scenes=(
    "scenes/boneco_1.xml"
    "scenes/boneco_2.xml"
    "scenes/boneco_3.xml"
    "scenes/boneco_2.xml"
    "scenes/boneco_3.xml"
)

frame_count=0

for ((i=0; i<cycles; i++)); do
    for ((j=0; j<${#scenes[@]}; j++)); do
        ./build/PEinT "${scenes[$j]}"
        mv boneco.png "frames/frame_$(printf "%03d" "$frame_count").png"
        ((frame_count++))
    done
done

frame_rate=3

ffmpeg -framerate $frame_rate \
    -i frames/frame_%03d.png \
    -filter_complex "split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse" \
    output.gif
