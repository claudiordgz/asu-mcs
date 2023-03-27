#pragma warning disable
using System;
using System.Text;
using System.Numerics;
using System.Diagnostics;

namespace P4
{

    class ParsedParams
    {
        public int p_e { get; set; }
        public int p_c { get; set; }
        public int q_e { get; set; }
        public int q_c { get; set; }
        public BigInteger cyphertext { get; set; }
        public BigInteger plaintext { get; set; }
    }

    class Program
    {
        public static ParsedParams parseArgs(string[] args)
        {
            int p_e = int.Parse(args[0]);
            int p_c = int.Parse(args[1]);
            int q_e = int.Parse(args[2]);
            int q_c = int.Parse(args[3]);
            BigInteger cyphertext = BigInteger.Parse(args[4]);
            BigInteger plaintext = BigInteger.Parse(args[5]);

            return new ParsedParams
            {
                p_e = p_e,
                p_c = p_c,
                q_e = q_e,
                q_c = q_c,
                cyphertext = cyphertext,
                plaintext = plaintext
            };
        }

        public static Tuple<BigInteger, BigInteger, BigInteger> extendedEuclideanAlgorithm(BigInteger a, BigInteger b)
        {
            if (b == 0) return Tuple.Create(a, BigInteger.One, BigInteger.Zero);

            BigInteger x2 = 1, x1 = 0, y2 = 0, y1 = 1;
            while (b > 0)
            {
                BigInteger q = a / b;
                BigInteger r = a - q * b;
                BigInteger x = x2 - q * x1;
                BigInteger y = y2 - q * y1;
                a = b;
                b = r;
                x2 = x1;
                x1 = x;
                y2 = y1;
                y1 = y;
            }
            return Tuple.Create(a, x2, y2);
        }

        public static string P4(string[] args)
        {
            if (args.Length != 6) throw new ArgumentException("Insufficient arguments");
            ParsedParams parsedParams = parseArgs(args);

            var p = BigInteger.Subtract(BigInteger.Pow(2, parsedParams.p_e), parsedParams.p_c);
            var q = BigInteger.Subtract(BigInteger.Pow(2, parsedParams.q_e), parsedParams.q_c);
            var phi = BigInteger.Multiply(p - 1, q - 1);

            BigInteger e = 65537;
            var extendedEuclideanAlgorithmResult = extendedEuclideanAlgorithm(e, phi);

            BigInteger d = extendedEuclideanAlgorithmResult.Item2;
            Debug.Assert(BigInteger.Multiply(d, e) % phi == 1, "Invalid d");

            var decryptedCyphertext = BigInteger.ModPow(parsedParams.cyphertext, d, BigInteger.Multiply(p, q));
            var encryptedText = BigInteger.ModPow(parsedParams.plaintext, e, BigInteger.Multiply(p, q));

            return $"{decryptedCyphertext},{encryptedText}";
        }

        public static void Main(string[] args)
        {
            var result = P4(args);
            Console.WriteLine(result);
        }
    }
}

