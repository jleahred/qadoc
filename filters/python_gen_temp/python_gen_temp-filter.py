#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
Create a python script with input
Execute it, and writes a file with output
"""
VERSION = '0.0.0'

import sys, os


temp_path = sys.argv[1]
lines = sys.stdin.readlines()
temp_python_full_path = os.path.join(temp_path, "temp.py")
temp_text_full_path = os.path.join(temp_path, "temp.txt")
FILE = open(temp_python_full_path,"wt")
FILE.writelines(lines)
FILE.close()

pipe = os.popen('python ' + temp_python_full_path + ' ' + temp_path)
FILE = open(temp_text_full_path ,"wt")
FILE.writelines(pipe.readlines())
FILE.close
print ""
