#pragma warning disable
using System;
using System.IO;
using System.Linq;
using System.Collections;

byte[] convertByteToBits(byte b)
{
  return Enumerable.Range(0, 4)
      .Select(i => (byte)((b >> (i * 2)) & 0b11))
      .Reverse()
      .ToArray();
}

byte[] stenography(byte[] inputBytes, byte[] bmpBytes)
{
  byte[] newBytes = new byte[bmpBytes.Length];

  int i = 0;
  foreach (byte dataByte in inputBytes)
  {
    byte[] dataBits = convertByteToBits(dataByte);
    foreach (byte dataBit in dataBits)
    {
      byte bmpByte = bmpBytes[i];
      // XOR dataBit and bmpByte
      byte newBmpByte = (byte)(bmpByte ^ dataBit);
      newBytes[i] = newBmpByte;
      i++;
    }
  }

  return newBytes;
}


byte[] P1_1(string[] args)
{
  byte[] bmpBytes = new byte[] {
      0x42,0x4D,0x4C,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x1A,0x00,0x00,0x00,0x0C,0x00,
      0x00,0x00,0x04,0x00,0x04,0x00,0x01,0x00,
      0x18,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
      0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,
      0x00,0x00,0xFF,0x00,0x00,0xFF,0xFF,0xFF,
      0xFF,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
      0xFF,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,
      0x00,0x00
    };

  if (args.Length != 1)
  {
    throw new Exception("Error: please pass in your input as a string after the 'dotnet run' command, e.g. 'dotnet run \"B1 FF FF CC 98 80 09 EA 04 48 7E C9\"'");
  }

  string hexString = args[0];
  byte[] inputBytes = hexString
      .Split(' ')
      .Select(hexValue => Convert.ToByte(hexValue, 16))
      .ToArray();

  if (inputBytes.Length > 12)
  {
    throw new Exception("Error: input has more than 12 bytes");
  }
  // separate the header from the bmp
  var startingIndexInBmpBytes = 26;
  var headerBytes = bmpBytes.Take(startingIndexInBmpBytes);
  var sliceBmpBytes = bmpBytes.Skip(startingIndexInBmpBytes).ToArray();
  byte[] newColorBytes = stenography(inputBytes, sliceBmpBytes);
  // slice the header and concat with newColorBytes
  var newBmpBytes = headerBytes.Concat(newColorBytes).ToArray();

  // Debug the input and output image
  // System.IO.File.WriteAllBytes("./input.bmp", bmpBytes);
  // System.IO.File.WriteAllBytes("./output.bmp", newBmpBytes);

  return newBmpBytes;
}
// BB E1 AC B6 37 4B 42 FB E8 79 A9 F2
var result = P1_1(args);
string formattedResult = BitConverter.ToString(result).Replace("-", " ");
Console.WriteLine(formattedResult);

