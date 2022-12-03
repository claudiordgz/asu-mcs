use itertools::join;
use rand::prelude::*;
use rand_chacha::ChaCha8Rng;
use std::env;
use std::fs::File;
use std::io::prelude::*;
use std::io::BufReader;

struct Cli {
    _prng_seed: u32,
    num_of_iterations: u32,
    seed_content: Vec<u8>,
}

fn get_positional_arguments(args: Vec<String>) -> (u32, u32) {
    let mut prng_seed: u32 = 64;
    let mut num_of_iterations: u32 = 1;

    // Parse command line arguments
    if args.len() > 1 {
        prng_seed = args[1].parse().unwrap();
    }
    if args.len() > 2 {
        num_of_iterations = args[2].parse().unwrap();
    }

    return (prng_seed, num_of_iterations);
}

fn get_seed(prng: &mut ChaCha8Rng) -> Vec<u8> {
    let seed_file = "./seed";
    //let seed_file = "../test_programs/prog0/seed";
    // read file and if it doesn't exist create a random Vec<u8>
    let mut seed_content: Vec<u8> = Vec::new();
    let file = File::open(seed_file);
    match file {
        Ok(file) => {
            let mut reader = BufReader::new(file);
            reader.read_to_end(&mut seed_content).unwrap();
        }
        Err(_) => {
            for _ in 10..32 {
                seed_content.push(prng.gen());
            }
        }
    }
    return seed_content;
}

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
                if *c < 126 {
                    *c += 1;
                } else {
                    *c = 48;
                }
            }
        }
    }
    return fuzzed_value;
}

fn main() {
    let args: Vec<String> = env::args().collect();
    // ./fuzzer prng_seed num_of_iterations
    // both args are optional but always in that order
    let (prng_seed, num_of_iterations) = get_positional_arguments(args);
    let mut prng = ChaCha8Rng::seed_from_u64(prng_seed as u64);
    let file = get_seed(&mut prng);
    let cli = Cli {
        _prng_seed: prng_seed,
        num_of_iterations,
        seed_content: file,
    };

    let fuzzed_value = main_fuzzer(&mut prng, cli);

    // print fuzzed value as copy
    // println!("Fuzzed value: {}", join(&fuzzed_value, ", "));
    // convert to char array and join
    let fuzzed_value = fuzzed_value
        .iter()
        .map(|x| *x as char)
        .collect::<Vec<char>>();
    let fuzzed_value = join(fuzzed_value, "");
    print!("{}", fuzzed_value);
    return;
}
