#!/bin/sh
#to run a file use command : bash filename.sh

#variable initialization
x=10
echo "x: $x"

#array initialization and fill nos in it
echo "enter array elements separated by spaces:"
read -a numbers

#printing array elements
echo "array elements: ${numbers[@]}"

#size of array
n=${#numbers[@]}

# Print array elements using a for loop
for ((i=0; i<n; i++)); 
do
    echo "${numbers[i]}"
done

#using a function
average() {
    sum=0
    for num in "${numbers[@]}"; 
    do
        sum=$((sum + num))
    done
    echo "Average: $((sum/n))"
}

#function call
average

#using if else statement
echo "Enter a number:"
read y
if [ $((y % 2)) -eq 0 ]; then
    echo "$y is divisible by 2"
else
    echo "$y is not divisible by 2"
fi

#using while loop
sum=0
while [ $y -gt 0 ]; 
do
	digit=$((y % 10))
	sum=$((sum + digit))
	y=$((y / 10))
done
echo "Sum of digits: $sum"