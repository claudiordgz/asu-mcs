import { ensureDirSync } from "https://deno.land/std/fs/mod.ts";

/**
 * Run with:
 * $ deno run --allow-read --allow-write --unstable --allow-net .\getData.ts
 */

interface IGitFile {
  name: string;
  download_url: string;
}

async function main() {
  ensureDirSync("./tmp");

  const dataPath =
    "https://api.github.com/repos/jiayuasu/Coursera-ASU-Database/contents/course1/assignment1/exampleinput/";
  const files = (await (await fetch(dataPath)).json()) as IGitFile[];
  const datFiles = files.filter((fileSchema) =>
    fileSchema.name.endsWith(".dat")
  );

  for (const fileSchema of datFiles) {
    const res = await fetch(fileSchema.download_url);
    const file = await Deno.open(`./tmp/${fileSchema.name}`, {
      create: true,
      write: true,
    });
    if (res) {
      await Deno.copy(res.body as any, file);
    }
    file.close();
  }
}

main();
