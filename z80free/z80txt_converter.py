#!/usr/bin/env python

import sys

fichero1=open(sys.argv[1],"r")
fichero2=open(sys.argv[2],"w")

def escribe_linea(thefile,nlinea3,pp2,pp3):

	nlinea2=hex(nlinea3)[2:].upper()
	while len(nlinea2)<2:
		nlinea2="0"+nlinea2

	thefile.write(nlinea2+"\t"+pp2+"\t"+pp3+"\n")

contador2=0
for linea in fichero1:
	if linea[-1]=="\n":
		linea=linea[:-1]
	if linea=="":
		continue
	contador=0
	nlinea=""
	for caracter in linea:
		if caracter=='\t':
			nlinea+=(" "*(8-contador))
			contador=-1
		else:
			nlinea+=caracter
		contador+=1
		if contador==8:
			contador=0
	
	if len(nlinea)<49:
		nlinea+=" "*(49-len(nlinea))
		
	p1=nlinea[:9]
	p2=nlinea[16:40]
	p3=nlinea[40:48]
	
	while (p1!="") and (p1[-1]==" "):
		p1=p1[:-1]
	while (p2!="") and (p2[-1]==" "):
		p2=p2[:-1]
	while (p3!="") and (p3[-1]==" "):
		p3=p3[:-1]
	
	try:
		if ((p1[:4].upper()=="FDCB") or (p1[:4].upper()=="DDCB")) and (len(p1)>8):
			nlinea=int(p1[7:],16)
		elif (len(p1)>3) and ((p1[:2].upper()=="CB") or (p1[:2].upper()=="ED") or (p1[:2].upper()=="DD") or (p1[:2].upper()=="FD")):
			nlinea=int(p1[2:4],16)
		else:
			nlinea=int(p1[:2],16)
	except:
		print p1
	
	while contador2<nlinea:
		escribe_linea(fichero2,contador2,"","")
		contador2+=1
	if p3=="":
		p3="*"
	escribe_linea(fichero2,nlinea,p3,p2)
	contador2+=1
	
fichero2.close()
fichero1.close()