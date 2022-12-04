FuzzThemAll Project

The Fuzzer is a Rust program, and the source is in the fuzzer_src directory. Dependency libraries are kept to a minimum to learn more about the fuzzing process. 
The binary is in the root directory, named fuzzer. To generate it, I perform the following:

$ cd fuzzer_src
$ cargo build --release
$ cp ./target/release/fuzzer ../fuzzer

It took a lot of work to develop a solid strategy because of my need for more knowledge of Rust. Nevertheless, I was pleased about the challenge, and I got to learn a lot. Reading the Arguments and getting the initial seed file was simple enough. This logic is in the function get_positional_arguments and get_seed inside the source. 

I started with a vector of u8 numbers. I manipulated these numbers to generate an array of Characters. I had to be careful because some characters would break the program. I tested out changing the entire String with a 13% probability or changing each character with a 13% probability. Overall, I was more pleased with changing the String because of execution speed, but due to comments on Slack, I erred on the safe side and went with the latter. 

The end loop was surprisingly simple:

fn main_fuzzer(prng: &mut ChaCha8Rng, cli_args: Cli) -> Vec<u8> {
    // copy seed_content to a new vector
    let mut fuzzed_value: Vec<u8> = cli_args.seed_content.clone();
    // iterate according to num_iterations
    for i in 0..cli_args.num_of_iterations {
        let itr = i + 1;
        // every 500 i, add 10 random characters to the seed
        // reduce the possibility of null chars to 1
        if itr % 500 == 0 {
            for _ in 0..1 {
                fuzzed_value.push(prng.gen_range(0..126));
            }
            for _ in 1..10 {
                fuzzed_value.push(prng.gen_range(48..126));
            }
        }
        for c in fuzzed_value.iter_mut() {
            // calculate random 0-100, mutate c if it is in the 13%
            // avoid invalid utf-8 characters
            let random = prng.gen_range(1..100);
            if random <= 13 {
                if *c % 200 == 0 {
                    *c = prng.gen_range(0..126);
                } else if *c % 3 == 1 {
                    *c = prng.gen_range(174..255);
                } else {
                    *c = prng.gen_range(65..126);
                }
            }
        }
    }
    return fuzzed_value;
}

Blank Characters, NULL Characters, new lines, and any related space posed a severe challenge. However, using long strings to crash the programs yielded faster results. I felt limited by the lack of command line arguments to my fuzzer. I would have liked to have the following functionality:
Force refresh. Every 500 iterations, add 10, but also switch everything to A or 0 and start over.
Only numbers. 
Allow special characters.
Chunk size
Unfortunately, for prog_6 and prog_7, I needed a little more time to develop a better strategy. So I solved for them using a seed file (seed_6 and seed_7, respectively). The seed file adds padding characters, which I imagined the fuzzer doing using probabilities similar to how we change the characters. 
To test the fuzzer, I wanted to use a Python Program using a subprocess. But this process was painfully slow. So instead, I wrote a batch script to generate a random quick PRNG seed and increase the num_of_iterations looking for a crash. This script is all_programs.sh. 
To run my programs, I would do the following:

$ ./all_programs.sh

A result would look like the following:

Crashed ./all_test_programs/prog_4 with status=139 using prng_seed=19278, iter=12000, seed=

The script would save results into results.log. It would hang now and then on 6, 7, 8, or 9. After going through the results, I would validate them using:

$ ./fuzzer 19278 12000 | ./all_test_programs_prog_4

test_programs.sh is the initial Bash File I created to deal with test_programs and the eventuality of using a seed file, which is something I had to do for prog_6 and prog_7.

