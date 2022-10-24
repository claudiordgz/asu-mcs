import argparse
import os
import string

def get_decrypted_text(key, encrypted_text):
    decrypted_text = ""
    for encrypted_char in encrypted_text:
       p = encrypted_char - ord('A')
       c = chr(ord('A') + ((p - key) % 26))
       decrypted_text = decrypted_text + c
    return decrypted_text

def main(args):
    encrypting = not args.decrypt
    if encrypting:
        plaintext = open(args.encrypt, "rb").read()
        keybytes = bytes(os.urandom(1))
        keyrotate = int(keybytes[0] % 25) + 1
        print("Key is " + string.ascii_uppercase[keyrotate])
        ciphertext = ""

        for i in range(0, len(plaintext)):
            if chr(plaintext[i]) in string.ascii_uppercase:
                p = plaintext[i] - ord('A')
                c = chr(ord('A') + ((p + keyrotate) % 26))
                ciphertext = ciphertext + c
            else:
                print("Incorrect input character %s." % plaintext[i])

        with open(args.outfile, "wb") as output:
            output.write(ciphertext.encode("ascii"))
            output.close()
    else:
        ciphertext = open(args.decrypt, "rb").read()
        encrypted_text = ciphertext
        k = open("caesarkey.txt", "r").read()[-1]
        key = ord(k) - ord('A')
        print(k, key)
        decrypted_text = get_decrypted_text(key, encrypted_text)
        plaintext = open('plaintext.txt', 'w')
        plaintext.write(decrypted_text)
        plaintext.close()

if __name__ == "__main__":
    # Handle command-line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--decrypt", help='Decrypt a file', required=False)
    parser.add_argument("-e", "--encrypt", help='Encrypt a file', required=False)
    parser.add_argument("-o", "--outfile", help='Output file', required=False)

    args = parser.parse_args()
    main(args)