#!/bin/bash

read -p "Enter the number of terms you want in the Fibonacci Series(terms>2):" num
first=0
second=1
echo $first
echo $second
for (( i=2;i<num;i++ ))
do
	third=$(( first+second ))
	echo $third
	first=$second
	second=$third
done

