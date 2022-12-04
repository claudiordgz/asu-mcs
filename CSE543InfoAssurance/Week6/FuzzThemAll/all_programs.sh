#!/bin/bash

start=`date +%s%N`

prng_seed=$RANDOM
echo "PRNG seed: $prng_seed"

# make an array of all prog 
test_programs_names=("prog_0" "prog_1" "prog_2" "prog_3" "prog_4" "prog_5" "prog_7" "prog_6" "prog_8" "prog_9")

for pi in ${!test_programs_names[@]}; do 
	path="./all_test_programs"
	name="${test_programs_names[$pi]}"
	path_and_executable="$path/$name"
	echo "Processing $path_and_executable"
	possible_seed_file="seed_$pi"
	path_to_seed="$path/$possible_seed_file"
	if test -f "$path_to_seed"; then
		cp $path_to_seed ./seed
	else 
		path_to_seed=""
	fi
	for i in {1..20}; do
		j=$( expr "$i" '*' 1000 )

		./fuzzer $prng_seed $j | $path_and_executable 2>/dev/null || {
			status=$?
			if [ "$status" -eq 139 ]; then
				echo "Crashed $path_and_executable with status=$status using prng_seed=$prng_seed, iter=$j, seed=$path_to_seed" >> results.log
				break
			fi
		}
	done
	rm -f ./seed
done

end=`date +%s%N`
printf "Done $j iterations in %0.2f seconds." $(echo "scale=2;($end-$start)/1000000000" | bc)
