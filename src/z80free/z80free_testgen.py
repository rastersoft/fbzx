#!/usr/bin/env python
# -*- coding: UTF-8 -*-

# Copyright 2008-2009 (C) Raster Software Vigo (Sergio Costas)

# This file is part of Z80Free
#
# Z80Free is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# Z80Free is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import sys

infile=open("tests.in","r")
expected=open("tests.expected","r")

outfile=open("tests.z80free","w")

maximo=0

while True:
    # read the test
    while True:
        test_name=""
        test_name=infile.readline()
        if test_name!="\n":
            break
    if test_name=="": # EOF
        break
    print test_name
    outfile.write(test_name)
    linea=infile.readline()
    while linea[:2]!="-1":
        outfile.write(linea)
        linea=infile.readline()
    outfile.write("-1\n")
    
    while True:
        test_name2=expected.readline()
        if test_name2!="\n":
            break
    if test_name2=="": #EOF
        continue
    if (test_name2!=test_name):
        print "error, %(entrada)s y %(salida)s no coinciden"%{"entrada":test_name,"salida":test_name2}
        sys.exit(1) 
    
    port=[]
    while True: # jump over event lines
        linea=expected.readline()
        if linea[0]!=' ':
            break
        pos=linea.find("PR")
        if pos==-1:
            continue
        port.append(linea[pos+3:])

    print linea
    for element in port:
        outfile.write(element)
        outfile.write(" ")
    outfile.write("-1\n")
    outfile.write(linea) # registers
    linea=expected.readline()
    print linea
    while (linea!="\n") and (linea!=""):
        print linea
        outfile.write(linea)
        linea=expected.readline()
    outfile.write("-1\n\n")

infile.close()
expected.close()
outfile.close()        
