#!/bin/bash

read -p "How many numbers you want to enter:" n

sum=0
for (( i=0;i<n;i++ ))
do
	read -p "Enter Number:" num
	sum=$(( sum+num ))
done

avg=$(( sum/n ))

echo "Average of the given numbers is: $avg"

