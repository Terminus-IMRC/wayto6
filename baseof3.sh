#!/bin/sh
for i in $(seq 1 34); do
	for j in $(seq $((i+1)) 35); do
		for k in $(seq $((j+1)) 36); do
			echo "$i $j $k"
		done
	done
done >baseof3.txt
