#!/bin/bash

make re
counter=1
max=100
while [ $counter -le $max ]
do
	printf "\e[1;34mTest $counter\n\e[0m"
	./philo_two 4 410 200 200
	((counter++))
done