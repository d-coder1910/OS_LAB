#!/bin/bash

echo "First Argument" $1
echo "Second Argument" $2
echo "Third Argument" $3
echo "Fourth Argument" $4

echo "Total Arguments: $#"
echo "All Arguments (@): $@"
echo "All Arguments (*): $*"

echo "Process ID of the last Script $$"
echo "Exit Status of the last command $?"


