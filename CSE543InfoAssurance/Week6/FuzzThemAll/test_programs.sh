#!/bin/bash

start=`date +%s%N`

prng_seed=$RANDOM
echo "PRNG seed: $prng_seed"

# make an array of all prog 
test_programs=("./test_programs/prog0" "./test_programs/prog1" "./test_programs/prog2")
test_programs_names=("prog_0" "prog_1" "prog_2")

for pi in ${!test_programs[@]}; do
	for i in {1..250}; do
    	j=$( expr "$i" '*' 100 )
  		path="${test_programs[$pi]}"
		name="${test_programs_names[$pi]}"
		path_and_executable="$path/$name"
		cp $path/seed ./seed

		./fuzzer2 $prng_seed $j | $path_and_executable 2>/dev/null || {
			status=$?
			if [ "$status" -eq 139 ]; then
			    echo "Crashed $path_and_executable with status=$status using prng_seed=$prng_seed, iter=$j" >> results.log
				break
			fi
		}
	done
done

rm ./seed
end=`date +%s%N`
printf "Done $j iterations in %0.2f seconds." $(echo "scale=2;($end-$start)/1000000000" | bc)
