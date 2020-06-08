import { ensureDirSync } from "https://deno.land/std/fs/mod.ts";

/**
 * Run with:
 * $ deno run --allow-read --allow-write --unstable --allow-net .\getData.ts
 */

async function main() {
  ensureDirSync("./tmp");

  const dataPath = "http://files.grouplens.org/datasets/movielens/ml-10m.zip";
  const res = await fetch(dataPath);
  const file = await Deno.open(`./tmp/ml-10m.zip`, {
    create: true,
    write: true,
  });
  if (res) {
    await Deno.copy(res.body as any, file);
  }
  file.close();
}

main();
