#pragma warning disable
using System;
using System.Security.Cryptography;
using System.Text;
using System.Linq;
using System.Numerics;

namespace P3
{
    class AES
    {
        public static byte[] encrypt(string input, byte[] key, byte[] iv)
        {
            using (AesManaged aes = new AesManaged())
            using (MemoryStream msEncrypt = new MemoryStream())
            using (CryptoStream csEncrypt = new CryptoStream(msEncrypt, aes.CreateEncryptor(key, iv), CryptoStreamMode.Write))
            {
                byte[] inputBytes = Encoding.UTF8.GetBytes(input);
                csEncrypt.Write(inputBytes, 0, inputBytes.Length);
                csEncrypt.FlushFinalBlock();
                return msEncrypt.ToArray();
            }
        }
        public static string decrypt(byte[] ciphertext, byte[] key, byte[] iv)
        {
            using (AesManaged aes = new AesManaged())
            using (MemoryStream msDecrypt = new MemoryStream(ciphertext))
            using (CryptoStream csDecrypt = new CryptoStream(msDecrypt, aes.CreateDecryptor(key, iv), CryptoStreamMode.Read))
            using (StreamReader srDecrypt = new StreamReader(csDecrypt))
            {
                return srDecrypt.ReadToEnd();
            }
        }
    }

    class ParsedParams
    {
        public byte[] IV { get; set; }
        public BigInteger g_e { get; set; }
        public BigInteger g_c { get; set; }
        public int N_e { get; set; }
        public int N_c { get; set; }
        public BigInteger x { get; set; }
        public BigInteger g_y_mod_n { get; set; }
        public byte[] C { get; set; }
        public string P { get; set; }
    }

    class Program
    {
        public static ParsedParams parseArgs(string[] args)
        {
            byte[] iv = args[0]
                  .Split(' ')
                  .Select(hexValue => Convert.ToByte(hexValue, 16))
                  .ToArray();

            BigInteger g_e, g_c, x, g_y_mod_n;
            int N_e, N_c;
            if (!BigInteger.TryParse(args[1], out g_e) || !BigInteger.TryParse(args[2], out g_c) ||
                !int.TryParse(args[3], out N_e) || !int.TryParse(args[4], out N_c) ||
                !BigInteger.TryParse(args[5], out x) || !BigInteger.TryParse(args[6], out g_y_mod_n))
            {
                throw new ArgumentException("Invalid parameters");
            }

            // Parse encrypted message C in hex
            byte[] encryptedMsg = args[7]
                  .Split(' ')
                  .Select(hexValue => Convert.ToByte(hexValue, 16))
                  .ToArray();

            // Parse plaintext message P as a string
            string plaintext = args[8];

            var parsedParams = new ParsedParams
            {
                IV = iv,
                g_e = g_e,
                g_c = g_c,
                N_e = N_e,
                N_c = N_c,
                x = x,
                g_y_mod_n = g_y_mod_n,
                C = encryptedMsg,
                P = plaintext
            };

            return parsedParams;
        }

        public static string P3(string[] args)
        {
            if (args.Length != 9) throw new ArgumentException("Insufficient arguments");
            ParsedParams parsedParams = parseArgs(args);

            var keyBytes = calculateKey(parsedParams);
            byte[] encryptedMsg = AES.encrypt(parsedParams.P, keyBytes, parsedParams.IV);
            string decryptedMsg = AES.decrypt(parsedParams.C, keyBytes, parsedParams.IV);

            string result = $"{decryptedMsg},{BitConverter.ToString(encryptedMsg).Replace("-", " ")}";
            return result;
        }

        public static byte[] calculateKey(ParsedParams parameters)
        {
            var n = BigInteger.Subtract(BigInteger.Pow(2, parameters.N_e), parameters.N_c);
            BigInteger key = BigInteger.ModPow(parameters.g_y_mod_n, parameters.x, n);
            return key.ToByteArray();
        }

        public static void Main(string[] args)
        {
            var result = P3(args);
            Console.WriteLine(result);
        }
    }
}

