#pragma warning disable
using System;
using System.Security.Cryptography;
using System.Text;
using System.Linq;

string GenerateRandomString(int length)
{
    const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    var random = new Random();
    return new string(Enumerable.Repeat(chars, length)
      .Select(s => s[random.Next(s.Length)]).ToArray());
}

byte[] m5HashFromSaltedString(string input, string salt)
{
    byte saltByte = Convert.ToByte(salt, 16);
    byte[] inputBytes = Encoding.UTF8.GetBytes(input);
    byte[] saltedInputBytes = inputBytes.Append(saltByte).ToArray();

    using (MD5 md5 = MD5.Create())
    {
        byte[] hashBytes = md5.ComputeHash(saltedInputBytes);

        return hashBytes;
    }
}

bool test()
{
    string inputString = "Hello World!";
    string saltString = "C5";

    byte[] hashBytes = m5HashFromSaltedString(inputString, saltString);
    byte[] expected = { 0xE6, 0xD9, 0xB0, 0xB9, 0xD1, 0x78, 0xB2, 0x40, 0x02, 0x89, 0xEB, 0xEA, 0x33, 0xE8, 0xB8, 0x82 };
    return hashBytes.SequenceEqual(expected);
}

Tuple<string, string> findTwoStringsWithSameHash(string saltString)
{
    Dictionary<string, string> attempts = new Dictionary<string, string>();

    while (true)
    {
        string randomString = GenerateRandomString(10);
        byte[] md5OfRandomString = m5HashFromSaltedString(randomString, saltString);
        byte[] hashBytes5 = md5OfRandomString.Take(5).ToArray();
        string md5OfRandomStringString = BitConverter.ToString(hashBytes5).Replace("-", "");

        if (attempts.Count == 0)
        {
            attempts.Add(md5OfRandomStringString, randomString);
        }
        else
        {
            if (attempts.ContainsKey(md5OfRandomStringString))
            {
                return new Tuple<string, string>(attempts[md5OfRandomStringString], randomString);
            }

            attempts.Add(md5OfRandomStringString, randomString);
        }
    }
}

string P2(string[] args)
{
    if (args.Length == 0) throw new ArgumentException("No salt provided");
    if (!test()) throw new Exception("Basic Hash Test failed");

    string saltString = args[0];
    var result = findTwoStringsWithSameHash(saltString);
    return result.Item1 + "," + result.Item2;
}

var result = P2(args);
Console.WriteLine(result);
