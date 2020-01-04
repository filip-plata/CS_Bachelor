import os
import glob
import base64
import tarfile

from Crypto.Hash import SHA256
from Crypto.Util.number import bytes_to_long, long_to_bytes
from Crypto.PublicKey import RSA

import wannacry

AES_KEYS = "packets_AES.data"
PUBLIC_KEY = "pub.pem"
ENCRYPTED_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                             "Moje dokumenty")

#########################################################

# https://stackoverflow.com/questions/4798654/modular-multiplicative-inverse-function-in-python

def egcd(a, b):

    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        return (g, x - (b // a) * y, y)

def modinv(a, m):
    g, x, y = egcd(a, m)
    if g != 1:
        raise Exception('modular inverse does not exist')
    else:
        return x % m


###############################################################


def encrypted_files_asc():
    """Encrypted files sorted by creation time"""
    return sorted(
        glob.glob("./Moje dokumenty/*.txt"),
        key = lambda f: os.path.getmtime(f))


def modulus():
    with open(PUBLIC_KEY, "r") as f:
        return RSA.importKey(f.read()).n


def encrypted_keys():
    """Keys read in from extracted data from packets"""
    with open(AES_KEYS, "r") as f:
        return [x.strip().replace(":", "").decode("hex") for x in f.readlines()]


def get_aes_base():
    """Obtain first unencrypted message send to server,
       as explained in README file"""
    keys = encrypted_keys()
    N = modulus()

    keys = map(lambda x: bytes_to_long(x.decode('base64')), keys)

    base_times_6 = ((keys[2] - keys[0]) - 2*(keys[1] - keys[0]) - 6) % N
    base_times_18 = ((keys[3] - keys[0]) - 3*(keys[1] - keys[0]) - 24) % N

    assert (base_times_6*3 - base_times_18) % N == 0
    
    inv_6 = modinv(6, N)
    inv_18 = modinv(18, N)

    assert ((base_times_6 * inv_6) - (base_times_18 * inv_18)) % N == 0
    
    return (base_times_6 * inv_6) % N
    

def decrypt(args):
    """Using decrypter from wannacry module for convienience"""
    decrypter = wannacry.Decrypter()
    decrypter.process(
        os.path.join(os.path.dirname(os.path.abspath(__file__)), ENCRYPTED_DIR),
        zip(args[::2], args[1::2]))


def extract(tar_url, extract_path='.'):
    tar = tarfile.open(tar_url, 'r')
    for item in tar:
        tar.extract(item, extract_path)
        if item.name.find(".tgz") != -1 or item.name.find(".tar") != -1:
            extract(item.name, "./" + item.name[:item.name.rfind('/')])


def main():
    extract("Moje dokumenty.tgz")
    for_decryption = []
    aes_base = get_aes_base()
    count = 0
    
    for f in encrypted_files_asc():
        for_decryption.append(os.path.basename(f).split('.')[0])
        for_decryption.append(base64.b64encode(SHA256.new(long_to_bytes(aes_base + count)).digest()))
        count += 1

    print(for_decryption)
    decrypt(for_decryption)


if __name__ == "__main__":
    main()
