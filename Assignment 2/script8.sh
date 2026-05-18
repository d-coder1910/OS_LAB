#!/bin/bash

read -p "How many numbers you want to enter:" n
read -p "Enter number 1:" num
max=$num
min=$num

for(( i=2;i<=n;i++ ))
do
	read -p "Enter number $i: " num2
	if [[ num2 -gt max ]]
	then
		max=$num2
	fi

	if [[ num2 -lt min ]]
	then 
		min=$num2
	fi
done

echo "Maximum: $max"
echo "Minimum: $min"

