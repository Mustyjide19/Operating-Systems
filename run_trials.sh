#!/bin/bash
# This file is for Part 1 of the coursework
# Write your for loop below:

# Loop through 20 times
for ((i=1; i<=20; i++)); do
    # Define the output filename
    output_file="output$(printf '%02d' $i).txt"
    
    # Print the current file being processed
    echo "Processing $output_file"
    
    # Run the trial program and redirect its output to the current filename
    ./trial > "$output_file"
done
