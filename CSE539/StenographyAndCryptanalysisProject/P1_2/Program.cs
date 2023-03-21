#pragma warning disable
using System;
using System.IO;
using System.Linq;
using System.Collections;
using System.Security.Cryptography;

double bruteForceSeed(string plainText, string cipherText)
{
  DateTimeOffset initialTime = new DateTimeOffset(2020, 7, 3, 11, 0, 0, TimeSpan.Zero);
  DateTimeOffset lastTime = new DateTimeOffset(2020, 7, 4, 11, 0, 0, TimeSpan.Zero);
  TimeSpan timeSpan = lastTime - initialTime;
  int totalMinutes = (int)timeSpan.TotalMinutes;
  int lowerBound = (int)(initialTime.UtcDateTime - new DateTime(1970, 1, 1)).TotalMinutes;
  int upperBound = lowerBound + totalMinutes;

  for (int seed = lowerBound; seed <= upperBound; seed++)
  {
    Random randomNumberGenerator = new Random(seed);
    byte[] key = BitConverter.GetBytes(randomNumberGenerator.NextDouble());
    string encryptedText = Encrypt(key, plainText);
    if (encryptedText == cipherText)
    {
      return seed;
    }
  }

  return -1;
}

string Encrypt(byte[] key, string secretString)
{
  DESCryptoServiceProvider csp = new DESCryptoServiceProvider();
  MemoryStream ms = new MemoryStream();
  CryptoStream cs = new CryptoStream(ms,
  csp.CreateEncryptor(key, key), CryptoStreamMode.Write);
  StreamWriter sw = new StreamWriter(cs);
  sw.Write(secretString);
  sw.Flush();
  cs.FlushFinalBlock();
  sw.Flush();
  return Convert.ToBase64String(ms.GetBuffer(), 0, (int)ms.Length);
}

string P1_2(string[] args)
{
  if (args.Length != 2)
  {
    throw new Exception("Error: Please pass in the plaintext and the ciphertext (e.g., \"Hello World\" \"RgdIKNgHn2Wg7jXwAykTlA==\")");
  }
  string plaintext = args[0];
  string ciphertext = args[1];
  double seed = bruteForceSeed(plaintext, ciphertext);
  return seed.ToString();
}

var result = P1_2(args);
Console.WriteLine(result);
