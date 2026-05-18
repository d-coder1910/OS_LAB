#!/bin/bash

read -p "Enter the number:" num

if [[ $num -eq 1 ]]
then
	echo "$num is not prime"

elif [[ $num -eq 2 ]]
then 
	 echo "$num is prime"
else 
	prime=1
	for (( i=3;i<=$num/2;i++ ))
	do
		if (( num%i == 0 ))
		then 
			prime=0
			break
		fi
	done
	
	if [[ $prime -eq 1 ]]
	then 
		echo "$num is prime"
	else
		echo "$num is not prime"
	fi

fi 

