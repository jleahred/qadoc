#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
Create a python script with input
Execute it, and writes a file with output
"""
VERSION = '0.0.0'

import sys, os


lines = sys.stdin.readlines()

initialize = []
code = []


# separate code from initialization

reading_code = False
for line in lines:
    line = line.strip()
    if reading_code == False:
        initialize.append(line)
    else:
        code.append(line)
    if line.startswith('file_name'):
        reading_code = True


# initialization
exec('\n'.join(initialize))

# generate file with code
if 'file_name' in locals():
    FILE = open(os.path.join('/tmp/adoc/', file_name), 'wt')
    FILE.writelines('\n'.join(code))
    FILE.close()


# execute file
os.chdir('/tmp/adoc/')
for r in run:
    pipe = os.popen(r[0])
    FILE = open(r[1], r[2])
    FILE.writelines(pipe.readlines())
    FILE.close
    


#temp_path = sys.argv[1]
#temp_python_full_path = os.path.join(temp_path, "temp.py")
#temp_text_full_path = os.path.join(temp_path, "temp.txt")
#FILE = open(temp_python_full_path,"wt")
#FILE.writelines(lines)
#FILE.close()

#pipe = os.popen('python ' + temp_python_full_path + ' ' + temp_path)
#FILE = open(temp_text_full_path ,"wt")
#FILE.writelines(pipe.readlines())
#FILE.close
#print ""
