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

class generic_parser:


	def get_code_from_one_param(self,element,bits):
	
		""" Returns nbits,read,write,condition
		
			Parses the ELEMENT string and, if its a register, a number or a memory access,
			returns in READ a piece of C code that allows to get that value; in WRITE a
			piece of code that allows to store a value there (if applicable, None if not
			valid) and NONE in CONDITION.
			
			if ELEMENT is a condition (like check ZERO, CARRY...), it returns None in READ
			and WRITE, and in CONDITION a piece of C code which returns TRUE (1) if that
			condition is true, or FALSE (0) if it is false.
			
			When the string has an ambiguous size (like (nn) in LD A,(nn) ), this function
			will use BITS to know how many bits has to use. But if BITS is 0 it means
			that the main class still doesn't know how many bits the current instruction
			is using, so this function must return 0 in NBITS. But if it can determine
			the true bit size (both because it's a fixed size element, like a register,
			or because BITS contains a value different of 0) then it must returns that
			size.
			
			If the combination of ELEMENT and BITS is unknown, it must call
			
				self.unknown_param()
				
			to show an error and stop the parsing process
			"""
	
		raise TypeError('Abstract method z80parser.get_code_from_one_param called')


	def create_code(self,ncode,tst1,tst2,opcode,code_read,code_write,condition,bits):
		
		""" Writes C code for the opcode OPCODE, with the list CODE_READ containing 
			C code for read each one of the parameters, the list CODE_WRITE containing
			C code for write each one of the parameters, CONDITION containing a C code
			string to check if the condition is TRUE or FALSE (or containing NONE if
			the opcode is unconditional), and all parameters having a wide of BITS bits.
			NCODE is the decimal code asigned to this opcode, and TST1 and TST2 are the
			TStates used by this opcode to run (the first when the condition is True,
			and the second when the condition is False).
			
			If this method returns False the main class will assume that it hasn't
			added the C code to return the number of TStates, and will add a line with
			the code RETURN (TST1); after it; but if it returns True, the class will
			assume that the C code inserted already returns the number of TStates, and
			won't add the RETURN line.
			
			This is an abstract method, and must be implemented in the class
			derived from this one.
			
			If the combination of OPCODE and BITS is unknown, it must call
			
				self.unknown_opcode()
				
			to show an error and stop the parsing process
			
		"""
			
		raise TypeError('Abstract method z80parser.create_code called')


	def write_start_code(self,filename,file_out):
		
		""" Writes the first part of the C code for the file """

		raise TypeError('Abstract method z80parser.write_start_code called')


	def write_end_code(self,filename,file_out):
		
		""" Writes the first part of the C code for the file """

		raise TypeError('Abstract method z80parser.write_end_code called')


	def license(self):
		
		""" writes the license terms. It must be always the GPL """

		if self.copyright_year==None:
			raise TypeError('Copyright year not defined')
		
		if self.copyright_author==None:
			raise TypeError('Copyright author not defined')
		
		if self.copyright_program_name==None:
			raise TypeError('Copyright program name not defined')

		self.file_out.write('/*\n')
		self.file_out.write(' * Copyright (C) '+str(self.copyright_year)+' '+str(self.copyright_author)+'\n')
		self.file_out.write(' * This file is part of '+str(self.copyright_program_name)+'\n')
		self.file_out.write(' *\n')
		self.file_out.write(' * '+str(self.copyright_program_name)+' is free software; you can redistribute it and/or modify\n')
		self.file_out.write(' * it under the terms of the GNU General Public License as published by\n')
		self.file_out.write(' * the Free Software Foundation; either version 3 of the License, or\n')
		self.file_out.write(' * (at your option) any later version.\n')
		self.file_out.write(' *\n')
		self.file_out.write(' * '+str(self.copyright_program_name)+' is distributed in the hope that it will be useful,\n')
		self.file_out.write(' * but WITHOUT ANY WARRANTY; without even the implied warranty of\n')
		self.file_out.write(' * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n')
		self.file_out.write(' * GNU General Public License for more details.\n')
		self.file_out.write(' *\n')
		self.file_out.write(' * You should have received a copy of the GNU General Public License\n')
		self.file_out.write(' * along with this program.  If not, see <http://www.gnu.org/licenses/>.\n')
		self.file_out.write(' * \n')
		self.file_out.write(' */')


	def __init__(self,filename):
		
		self.line_number=0
		self.curr_element=""
		self.curr_bits=0
		self.curr_opcode=""
		self.curr_instruction=""
		self.filename=filename
		self.file_in=open(filename+".txt","r")
		self.file_out=open(filename+".c","w")

		self.license() # the code generated by z80free_gencode.py is ALWAYS GPL
		self.file_out.write('\n\n')
		self.write_start_code()
		self.file_out.write('\tswitch(opcode) {\n')
		while True:
			if self.proccess_line()==False:
				break
		self.file_out.write('\t}\n\n')			
		self.write_end_code()
		self.file_out.close()
		self.file_in.close()


	def proccess_line(self):
		
		while True:
			ncode,tst1,tst2,opcode,params=self.get_parts()
			if ncode==None:
				return False
			if tst1!=0:
				break
				
		code_read,code_write,condition,bits=self.get_code_from_params(params)
		self.file_out.write("\tcase "+str(ncode)+": // "+self.curr_instruction+"\n")
		self.curr_opcode=opcode
		self.curr_bits=bits
		tst_done=self.create_code(ncode,tst1,tst2,opcode,code_read,code_write,condition,bits)
		if tst_done==False:
			self.file_out.write("\t\treturn ("+str(tst1)+");\n")
		self.file_out.write("\tbreak;\n")


	def unknown_opcode(self):
		
		self.show_error("Unknown opcode "+str(self.curr_opcode)+" for "+str(self.curr_bits)+" bits wide while processing "+str(self.curr_instruction))


	def unknown_param(self):
		
		self.show_error("Unknown parameter "+str(self.curr_element)+" for "+str(self.curr_bits)+" bits wide while processing "+str(self.curr_instruction))


	def show_error(self,msg_error):

		print
		print "Syntax error while processing file "+self.filename+".txt"
		print "at line "+str(self.line_number)+" when processing instruction "+self.curr_instruction
		print msg_error
		print "Check the file and run again"
		print
		self.file_out.close()
		self.file_in.close()
		sys.exit(1)


	def get_parts(self):
		
		""" Gets the parts from a file line: number code, TStates OPCODE and parameters """
		
		while True:
			line=self.file_in.readline()
			self.line_number+=1
			if line=="":
				return None,0,0,"",""
			if (line[0]!="#") and (line!="\n"):
				break
	
		if line[-1]=="\n":
			line=line[:-1]
		l_ncode=int(line[0:2],16)
		pos=line[3:].find("\t") # find the separator between the TStates and the OPCODE
		if (line[3]=="\t") or (line[3]=="*") or (pos==-1): # if there are no TStates nor OPCODE, its an empty entry
			return l_ncode,0,0,"",""
	
		tstates=line[3:pos+3]
		pos2=tstates.find("/") # check if there are two TState values
		if pos2==-1:
			l_tst1=int(tstates)
			l_tst2=int(tstates)
		else:
			l_tst1=int(tstates[:pos2]) # tstates if the condition is True
			l_tst2=int(tstates[pos2+1:]) # tstates if the condition is False
		l_opcode=line[pos+4:]
		self.curr_instruction=l_opcode
		pos=l_opcode.find(" ")
		if pos!=-1: # there are parameters
			l_params=l_opcode[pos+1:]
			l_opcode=l_opcode[:pos]
		else:
			l_params=""
	
		return l_ncode,l_tst1,l_tst2,l_opcode,l_params
	
	
	def get_code_from_params(self,params):
		
		""" Returns the C code to read or write the params, and the code for the condition
			Return two lists, an string and an integer. The first list contains the C code to
			read each parameter; the second list the C code to write each parameter; the
			string returns the string which checks the condition (None if there's no condition)
			and the integer indicates when the operators are 8 or 16 bit wide.
			
			If it finds a set of parameters that can't parse, it prints it and exits
			
			It uses the virtual method get_code_from_one_param(element,bits) to parse
			each parameter
		"""
		
		bits=0
		
		if params=="":
			return [],[],None,0
		
		bits_found=False
		list_params=params.split(",")
		while bits_found==False:
			bits_found=True
			read_code=[]
			write_code=[]
			condition=None
			for element in list_params:
				# a parameter's bit wide can be ambiguous (like (nn) in LD A,(nn) or
				# LD HL,(nn) ) so we try to read it passing the currently known bit wide,
				# and the function must returns the true wide if it's unambiguous (like A, HL...)
				# the true wide if passed a correct wide, or 0 if the current bit wide is
				# unknown and the parameter is ambiguous. So in the case of
				# LD A,(nn) we do only one pass to get the wide, but in the case of
				# LD (nn),A we do two passes, because the first parameter is ambiguous
				# in size
				self.curr_bits=bits
				self.curr_element=element
				nbits,read,write,p_condition=self.get_code_from_one_param(element,bits)
				if p_condition!=None: # this parameter is a condition
					condition=p_condition
					continue
				if nbits==0:
					bits_found=False
				else:
					if bits==0:
						bits=nbits
					if bits!=nbits:
						self.show_error("Mixed bit wide parameters (8 and 16 bit parameters in the same line)")
				
				read_code.append(read)
				write_code.append(write)
			if (bits==0) and (bits_found==False):
				self.show_error("All parameters are ambiguous: "+str(params))
		
		return read_code,write_code,condition,bits


class z80_parser(generic_parser):

	def __init__(self,filename):
		
		self.copyright_year="2008-2009"
		self.copyright_author="Sergio Costas (Raster Software Vigo)"
		self.copyright_program_name="Z80Free"
		generic_parser.__init__(self, filename)


	def get_code_from_one_param(self,element,bits):
	
		""" Returns nbits,read,write,condition """
		
		if element=="AF":
			return 16,"processor->Rm.wr.AF","processor->Rm.wr.AF=@DATA@;",None
		if element=="AF'":
			return 16,"processor->Ra.wr.AF","processor->Ra.wr.AF=@DATA@;",None
		if element=="BC":
			return 16,"processor->Rm.wr.BC","processor->Rm.wr.BC=@DATA@;",None
		if element=="DE":
			return 16,"processor->Rm.wr.DE","processor->Rm.wr.DE=@DATA@;",None
		if element=="HL":
			return 16,"processor->Rm.wr.HL","processor->Rm.wr.HL=@DATA@;",None
		if element=="SP":
			return 16,"processor->Rm.wr.SP","processor->Rm.wr.SP=@DATA@;",None
		
		if element=="IX":
			return 16,"processor->Rm.wr.IX","processor->Rm.wr.IX=@DATA@;",None
		if element=="IY":
			return 16,"processor->Rm.wr.IY","processor->Rm.wr.IY=@DATA@;",None
		
		if element=="(IX+d)":
			if bits==0:
				return 0,None,None,None
			elif bits==8:
				if (self.codes=="DDCB") or (self.codes=="FDCB"):
					return 8,"Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))","Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),@DATA@);",None
				else:
					return 8,"Z80free_Rd(Z80free_addr_relative(processor,processor->Rm.wr.IX))","Z80free_Wr(Z80free_addr_relative(processor,processor->Rm.wr.IX),@DATA@);",None
		
		if element=="(IY+d)":
			if bits==0:
				return 0,None,None,None
			elif bits==8:
				if (self.codes=="DDCB") or (self.codes=="FDCB"):
					return 8,"Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IY,d1))","Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IY,d1),@DATA@);",None
				else:
					return 8,"Z80free_Rd(Z80free_addr_relative(processor,processor->Rm.wr.IY))","Z80free_Wr(Z80free_addr_relative(processor,processor->Rm.wr.IY),@DATA@);",None
		
		if element=="IX+d":
			if bits==0:
				return 0,None,None,None
			elif bits==8:
				return 8,"Z80free_addr_relative(processor,processor->Rm.wr.IX)",None,None

		if element=="IY+d":
			if bits==0:
				return 0,None,None,None
			elif bits==8:
				return 8,"Z80free_addr_relative(processor,processor->Rm.wr.IY)",None,None
		
		if element=="b(IX+d)":
			if (self.codes=="DDCB") or (self.codes=="FDCB"):
				return 8,"Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))","Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),@DATA@);",None
			else:
				return 8,"Z80free_Rd(Z80free_addr_relative(processor,processor->Rm.wr.IX))","Z80free_Wr(Z80free_addr_relative(processor,processor->Rm.wr.IX),@DATA@);",None


		if element=="b(IY+d)":
			if (self.codes=="DDCB") or (self.codes=="FDCB"):
				return 8,"Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IY,d1))","Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IY,d1),@DATA@);",None
			else:
				return 8,"Z80free_Rd(Z80free_addr_relative(processor,processor->Rm.wr.IY))","Z80free_Wr(Z80free_addr_relative(processor,processor->Rm.wr.IY),@DATA@);",None
		
		if element=="nn":
			return 16,"Z80free_read_param_16(processor)",None,None
		if element=="n":
			return 8,"Z80free_read_param_8(processor)",None,None
		
		if element=="(nn)":
			if bits==0:
				return 0,None,None,None
			elif bits==8:
				return 8,"Z80free_Rd(Z80free_read_param_16(processor))","Z80free_Wr(Z80free_read_param_16(processor),@DATA@);",None
			elif bits==16:
				return 16,"Z80free_read16(Z80free_read_param_16(processor))","Z80free_write16(Z80free_read_param_16(processor),@DATA@);",None
		
		if element=="(BC)":
			if bits==0:
				return 0,None,None,None
			elif bits==8:
				return 8,"Z80free_Rd(processor->Rm.wr.BC)","Z80free_Wr(processor->Rm.wr.BC,@DATA@);",None
			elif bits==16:
				return 16,"Z80free_read16(processor->Rm.wr.BC)","Z80free_write16(processor->Rm.wr.BC,@DATA@);",None
		if element=="(DE)":
			if bits==0:
				return 0,None,None,None
			elif bits==8:
				return 8,"Z80free_Rd(processor->Rm.wr.DE)","Z80free_Wr(processor->Rm.wr.DE,@DATA@);",None
			elif bits==16:
				return 16,"Z80free_read16(processor->Rm.wr.DE)","Z80free_write16(processor->Rm.wr.DE,@DATA@);",None
		if element=="(HL)":
			if bits==0:
				return 0,None,None,None
			elif bits==8:
				return 8,"Z80free_Rd(processor->Rm.wr.HL)","Z80free_Wr(processor->Rm.wr.HL,@DATA@);",None
			elif bits==16:
				return 16,"Z80free_read16(processor->Rm.wr.HL)","Z80free_write16(processor->Rm.wr.HL,@DATA@);",None
		if element=="(SP)":
			if bits==0:
				return 0,None,None,None
			elif bits==16:
				return 16,"Z80free_read16(processor->Rm.wr.SP)","Z80free_write16(processor->Rm.wr.SP,@DATA@);",None
		if element=="b(HL)":
			return 8,"Z80free_Rd(processor->Rm.wr.HL)","Z80free_Wr(processor->Rm.wr.HL,@DATA@);",None
		
		if element=="I":
			return 8,"Z80free_readI(processor)","processor->I=@DATA@;",None
		if element=="R":
			return 8,"Z80free_readR(processor)","Z80free_setR(processor,@DATA@);",None	
		
		if element=="A":
			return 8,"processor->Rm.br.A","processor->Rm.br.A=@DATA@;",None
		if element=="B":
			return 8,"processor->Rm.br.B","processor->Rm.br.B=@DATA@;",None
		if element=="C":
			return 8,"processor->Rm.br.C","processor->Rm.br.C=@DATA@;",None
		if element=="D":
			return 8,"processor->Rm.br.D","processor->Rm.br.D=@DATA@;",None
		if element=="E":
			return 8,"processor->Rm.br.E","processor->Rm.br.E=@DATA@;",None
		if element=="H":
			return 8,"processor->Rm.br.H","processor->Rm.br.H=@DATA@;",None
		if element=="L":
			return 8,"processor->Rm.br.L","processor->Rm.br.L=@DATA@;",None
		if element=="IXH":
			return 8,"processor->Rm.br.IXh","processor->Rm.br.IXh=@DATA@;",None
		if element=="IXL":
			return 8,"processor->Rm.br.IXl","processor->Rm.br.IXl=@DATA@;",None
		if element=="IYH":
			return 8,"processor->Rm.br.IYh","processor->Rm.br.IYh=@DATA@;",None
		if element=="IYL":
			return 8,"processor->Rm.br.IYl","processor->Rm.br.IYl=@DATA@;",None
		
		if element=="0H":
			return 16,"0",None,None
		if element=="0":
			return 8,"0",None,None
		if element=="1":
			return 8,"1",None,None
		if element=="2":
			return 8,"2",None,None
		if element=="3":
			return 8,"3",None,None
		if element=="4":
			return 8,"4",None,None
		if element=="5":
			return 8,"5",None,None
		if element=="6":
			return 8,"6",None,None
		if element=="7":
			return 8,"7",None,None
		if element=="8H":
			return 16,"8",None,None
		if element=="10H":
			return 16,"16",None,None
		if element=="18H":
			return 16,"24",None,None
		if element=="20H":
			return 16,"32",None,None
		if element=="28H":
			return 16,"40",None,None
		if element=="30H":
			return 16,"48",None,None
		if element=="38H":
			return 16,"56",None,None
		
		if element=="NZ":
			return 0,None,None,"(0==(F_Z&processor->Rm.br.F))"
		if element=="Z":
			return 0,None,None,"(F_Z&processor->Rm.br.F)"
		if element=="NC":
			return 0,None,None,"(0==(F_C&processor->Rm.br.F))"
		if element=="CF":
			return 0,None,None,"(F_C&processor->Rm.br.F)"
		if element=="PO":
			return 0,None,None,"(0==(F_PV&processor->Rm.br.F))"
		if element=="PE":
			return 0,None,None,"(F_PV&processor->Rm.br.F)"
		if element=="P":
			return 0,None,None,"(0==(F_S&processor->Rm.br.F))"
		if element=="M":
			return 0,None,None,"(F_S&processor->Rm.br.F)"
		
		self.unknown_param()
	

	def create_code(self,ncode,tst1,tst2,opcode,code_read,code_write,condition,bits):
		
		""" Returns True if it has written the RETURN for returning the TStates """
		
		if opcode=="NOP":
			return False
		
		if opcode=="LD2":
				self.file_out.write("\t\ttmp2="+code_read[0]+";\n")
				self.file_out.write("\t\tZ80free_Wr(tmp2,"+code_read[1]+");\n")
				return False
			
		if opcode=="LD":
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@",code_read[1])+"\n")
			return False

		if opcode=="INC":
			if bits==16:
				self.file_out.write("\t\t"+code_write[0].replace("@DATA@","Z80free_doIncDec16(processor,"+code_read[0]+",0)")+"\n")
				return False
			elif bits==8:
				self.file_out.write("\t\t"+code_write[0].replace("@DATA@","Z80free_doIncDec(processor,"+code_read[0]+",0)")+"\n")
				return False

		if opcode=="DEC":
			if bits==16:
				self.file_out.write("\t\t"+code_write[0].replace("@DATA@","Z80free_doIncDec16(processor,"+code_read[0]+",1)")+"\n")
				return False
			elif bits==8:
				self.file_out.write("\t\t"+code_write[0].replace("@DATA@","Z80free_doIncDec(processor,"+code_read[0]+",1)")+"\n")
				return False

		if opcode=="RLCA":
			self.file_out.write("\t\tprocessor->Rm.br.A=Z80free_doRLC(processor,0,processor->Rm.br.A);\n")
			return False
		
		if opcode=="RRCA":
			self.file_out.write("\t\tprocessor->Rm.br.A=Z80free_doRRC(processor,0,processor->Rm.br.A);\n")
			return False
		
		if opcode=="EX":
			self.file_out.write("\t\ttmp2="+code_read[0]+";\n")
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@",code_read[1])+"\n")
			self.file_out.write("\t\t"+code_write[1].replace("@DATA@","tmp2")+"\n")
			return False
		
		if opcode=="ADD":
			if bits==16:
				data="Z80free_doArithmetic16(processor,"+code_read[0]+","+code_read[1]+",0,0)"
				self.file_out.write("\t\t"+code_write[0].replace("@DATA@",data)+"\n")
				return False
			elif bits==8:
				data="Z80free_doArithmetic(processor,"+code_read[0]+","+code_read[1]+",0,0)"
				self.file_out.write("\t\t"+code_write[0].replace("@DATA@",data)+"\n")
				return False
		
		if opcode=="ADC":
			if bits==16:
				data="Z80free_doArithmetic16(processor,"+code_read[0]+","+code_read[1]+",1,0)"
				self.file_out.write("\t\t"+code_write[0].replace("@DATA@",data)+"\n")
				return False
			elif bits==8:
				data="Z80free_doArithmetic(processor,"+code_read[0]+","+code_read[1]+",1,0)"
				self.file_out.write("\t\t"+code_write[0].replace("@DATA@",data)+"\n")
				return False
		
		if opcode=="SUB":
			if bits==16:
				data="Z80free_doArithmetic16(processor,"+code_read[0]+","+code_read[1]+",0,1)"
				self.file_out.write("\t\t"+code_write[0].replace("@DATA@",data)+"\n")
				return False
			elif bits==8:
				data="Z80free_doArithmetic(processor,processor->Rm.br.A,"+code_read[0]+",0,1)"
				self.file_out.write("\t\tprocessor->Rm.br.A="+data+";\n")
				return False
			
		if opcode=="SBC":
			if bits==16:
				data="Z80free_doArithmetic16(processor,"+code_read[0]+","+code_read[1]+",1,1)"
				self.file_out.write("\t\t"+code_write[0].replace("@DATA@",data)+"\n")
				return False
			elif bits==8:
				data="Z80free_doArithmetic(processor,"+code_read[0]+","+code_read[1]+",1,1)"
				self.file_out.write("\t\t"+code_write[0].replace("@DATA@",data)+"\n")
				return False
		
		if opcode=="DJNZ":
			self.file_out.write("\t\ttmp1="+code_read[0]+";\n")
			self.file_out.write("\t\tprocessor->Rm.br.B--;\n")
			self.file_out.write("\t\tif (processor->Rm.br.B) {\n")
			self.file_out.write("\t\t\tZ80free_jump_relative(processor,tmp1);\n")
			self.file_out.write("\t\t\treturn ("+str(tst1)+");\n")
			self.file_out.write("\t\t} else {\n")
			self.file_out.write("\t\t\treturn ("+str(tst2)+");\n")
			self.file_out.write("\t\t}\n")
			return True
		
		if opcode=="RLA":
			self.file_out.write("\t\tprocessor->Rm.br.A=Z80free_doRL(processor,0,processor->Rm.br.A);\n")
			return False
		
		if opcode=="RRA":
			self.file_out.write("\t\tprocessor->Rm.br.A=Z80free_doRR(processor,0,processor->Rm.br.A);\n")
			return False
		
		if opcode=="DAA":
			self.file_out.write("\t\tZ80free_doDAA(processor);\n")
			return False
		
		if opcode=="CPL":
			self.file_out.write("\t\tprocessor->Rm.br.A=255-processor->Rm.br.A;\n")
			self.file_out.write("\t\tZ80free_adjustFlags(processor,processor->Rm.br.A);\n")
			self.file_out.write("\t\tZ80free_setFlag(processor,F_H);\n")
			self.file_out.write("\t\tZ80free_setFlag(processor,F_N);\n")
			return False
		
		if opcode=="SCF":
			self.file_out.write("\t\tZ80free_setFlag(processor,F_C);\n")
			self.file_out.write("\t\tZ80free_resFlag(processor,F_H);\n")
			self.file_out.write("\t\tZ80free_resFlag(processor,F_N);\n")
			self.file_out.write("\t\tZ80free_adjustFlags(processor,processor->Rm.br.A);\n")
			return False

		if opcode=="CCF":
			self.file_out.write("\t\tZ80free_valFlag(processor,F_C,(~processor->Rm.br.F)&F_C);\n")
			self.file_out.write("\t\tZ80free_resFlag(processor,F_N);\n")
			self.file_out.write("\t\tZ80free_adjustFlags(processor,processor->Rm.br.A);\n")
			return False
		
		if opcode=="HALT":
			self.file_out.write("\t\tprocessor->PC--;\n")
			self.file_out.write("\t\tprocessor->HALT=1;\n")
			return False
		
		if opcode=="AND":
			if bits==8:
				self.file_out.write("\t\tZ80free_doAND(processor,"+code_read[0]+");\n")
				return False
		
		if opcode=="OR":
			if bits==8:
				self.file_out.write("\t\tZ80free_doOR(processor,"+code_read[0]+");\n")
				return False
		
		if opcode=="XOR":
			if bits==8:
				self.file_out.write("\t\tZ80free_doXOR(processor,"+code_read[0]+");\n")
				return False
			
		if opcode=="CP":
			if bits==8:
				self.file_out.write("\t\ttmp1="+code_read[0]+";\n")
				self.file_out.write("\t\tZ80free_doArithmetic(processor,processor->Rm.br.A,tmp1,0,1);\n")
				self.file_out.write("\t\tZ80free_adjustFlags(processor,tmp1);\n")
				return False
		
		if opcode=="JR":
			self.file_out.write("\t\ttmp1="+code_read[0]+";\n")
			if condition==None: # no condition, always jump
				self.file_out.write("\t\tZ80free_jump_relative(processor,tmp1);\n")
				return False
			else:
				self.file_out.write("\t\tif "+condition+" {\n")
				self.file_out.write("\t\t\tZ80free_jump_relative(processor,tmp1);\n")
				self.file_out.write("\t\t\treturn ("+str(tst1)+");\n")
				self.file_out.write("\t\t} else {\n")
				self.file_out.write("\t\t\treturn ("+str(tst2)+");\n")
				self.file_out.write("\t\t}\n")
				return True
		
		if opcode=="RET":
			if condition==None:
				self.file_out.write("\t\tprocessor->PC=Z80free_doPop(processor);\n")
				return False
			else:
				self.file_out.write("\t\tif "+condition+" {\n")
				self.file_out.write("\t\t\tprocessor->PC=Z80free_doPop(processor);\n")
				self.file_out.write("\t\t\treturn ("+str(tst1)+");\n")
				self.file_out.write("\t\t} else {\n")
				self.file_out.write("\t\t\treturn ("+str(tst2)+");\n")
				self.file_out.write("\t\t}\n")
				return True
			
		if opcode=="POP":
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","Z80free_doPop(processor)")+"\n")
			return False
		
		if opcode=="JP":
			self.file_out.write("\t\ttmp2="+code_read[0]+";\n")
			if condition==None:
				self.file_out.write("\t\tprocessor->PC=tmp2;\n")
				return False
			else:
				self.file_out.write("\t\tif "+condition+" {\n")
				self.file_out.write("\t\t\tprocessor->PC=tmp2;\n")
				self.file_out.write("\t\t\treturn ("+str(tst1)+");\n")
				self.file_out.write("\t\t} else {\n")
				self.file_out.write("\t\t\treturn ("+str(tst2)+");\n")
				self.file_out.write("\t\t}\n")
				return True
		
		if opcode=="CALL":
			self.file_out.write("\t\ttmp2="+code_read[0]+";\n")
			if condition==None:
				self.file_out.write("\t\tZ80free_doPush(processor,processor->PC);\n")
				self.file_out.write("\t\tprocessor->PC=tmp2;\n")
				return False
			else:
				self.file_out.write("\t\tif "+condition+" {\n")
				self.file_out.write("\t\t\tZ80free_doPush(processor,processor->PC);\n")
				self.file_out.write("\t\t\tprocessor->PC=tmp2;\n")
				self.file_out.write("\t\t\treturn ("+str(tst1)+");\n")
				self.file_out.write("\t\t} else {\n")
				self.file_out.write("\t\t\treturn ("+str(tst2)+");\n")
				self.file_out.write("\t\t}\n")
				return True
		
		if opcode=="PUSH":
			self.file_out.write("\t\tZ80free_doPush(processor,"+code_read[0]+");\n")
			return False
		
		if opcode=="RST":
			self.file_out.write("\t\tZ80free_doPush(processor,processor->PC);\n")
			self.file_out.write("\t\tprocessor->PC="+code_read[0]+";\n")
			return False
		
		if opcode=="EXX":
			self.file_out.write("\t\ttmp2=processor->Rm.wr.BC;\n")
			self.file_out.write("\t\tprocessor->Rm.wr.BC=processor->Ra.wr.BC;\n")
			self.file_out.write("\t\tprocessor->Ra.wr.BC=tmp2;\n")
			self.file_out.write("\t\ttmp2=processor->Rm.wr.DE;\n")
			self.file_out.write("\t\tprocessor->Rm.wr.DE=processor->Ra.wr.DE;\n")
			self.file_out.write("\t\tprocessor->Ra.wr.DE=tmp2;\n")
			self.file_out.write("\t\ttmp2=processor->Rm.wr.HL;\n")
			self.file_out.write("\t\tprocessor->Rm.wr.HL=processor->Ra.wr.HL;\n")
			self.file_out.write("\t\tprocessor->Ra.wr.HL=tmp2;\n")
			return False
		
		if opcode=="IN":
			if bits==8:
				self.file_out.write("\t\ttmp2=((word)processor->Rm.br.A)<<8;\n")
				self.file_out.write("\t\tprocessor->Rm.br.A=Z80free_In(tmp2+((word)Z80free_read_param_8(processor)));\n")
				return False

		if opcode=="IN_BC":
			if bits==8:
				self.file_out.write("\t\t"+code_write[0].replace("@DATA@","Z80free_In(processor->Rm.wr.BC)")+"\n")
				self.file_out.write("\t\tZ80free_adjustFlagSZP (processor,"+code_read[0]+");\n")
				self.file_out.write("\t\tZ80free_resFlag(processor,F_H|F_N);\n")
				return False
			elif bits==0:
				self.file_out.write("\t\ttmp1=Z80free_In(processor->Rm.wr.BC);\n")
				self.file_out.write("\t\tZ80free_adjustFlagSZP (processor,tmp1);\n")
				self.file_out.write("\t\tZ80free_resFlag(processor,F_H|F_N);\n")
				return False

		if opcode=="OUT":
			if bits==8:
				self.file_out.write("\t\ttmp1=processor->Rm.br.A;\n")
				self.file_out.write("\t\ttmp2=((word)tmp1)<<8;\n")
				self.file_out.write("\t\tZ80free_Out(tmp2+((word)Z80free_read_param_8(processor)),tmp1);\n")
				return False


		if opcode=="OUT_BC":
			if bits==8:
				self.file_out.write("\t\tZ80free_Out(processor->Rm.wr.BC,"+code_read[0]+");\n")
				return False
		
		if opcode=="DI":
			self.file_out.write("\t\tprocessor->IFF1=0;\n")
			self.file_out.write("\t\tprocessor->IFF2=0;\n")
			return False
		
		if opcode=="EI":
			self.file_out.write("\t\tprocessor->IFF1=2; /* this allows to delay one instruction the interrupts*/\n")
			self.file_out.write("\t\tprocessor->IFF2=1;\n")
			return False
		
		if opcode=="NEG":
			self.file_out.write("\t\tprocessor->Rm.br.A=Z80free_doArithmetic(processor,0,processor->Rm.br.A,0,1);\n")
			return False
		
		if opcode=="RETN":
			self.file_out.write("\t\tprocessor->IFF1=(processor->IFF2 ? 2 : 0);\n")
			self.file_out.write("\t\tprocessor->PC=Z80free_doPop(processor);\n")
			return False
		
		if opcode=="RETI":
			self.file_out.write("\t\tprocessor->IFF1=2;\n")
			self.file_out.write("\t\tprocessor->IFF2=1;\n")
			self.file_out.write("\t\tprocessor->PC=Z80free_doPop(processor);\n")
			return False
		
		if opcode=="IM":
			self.file_out.write("\t\tprocessor->IM="+code_read[0]+";\n")
			return False
		
		if opcode=="RLD":
			self.file_out.write("\t\tZ80free_doRLD(processor);\n")
			return False

		if opcode=="RRD":
			self.file_out.write("\t\tZ80free_doRRD(processor);\n")
			return False
		
		if opcode=="LDI":
			self.file_out.write("\t\ttmp1=Z80free_Rd(processor->Rm.wr.HL++);\n")
			self.file_out.write("\t\tZ80free_Wr(processor->Rm.wr.DE++,tmp1);\n")
			self.file_out.write("\t\ttmp1+=processor->Rm.br.A;\n")
			self.file_out.write("\t\tZ80free_valFlag(processor,F_3,tmp1&0x08);\n")
			self.file_out.write("\t\tZ80free_valFlag(processor,F_5,tmp1&0x02);\n")
			self.file_out.write("\t\tprocessor->Rm.wr.BC--;\n")
			self.file_out.write("\t\tZ80free_resFlag(processor,F_H|F_N);\n")
			self.file_out.write("\t\tif (processor->Rm.wr.BC)\n")
			self.file_out.write("\t\t\tZ80free_setFlag(processor,F_PV);\n")
			self.file_out.write("\t\telse\n")
			self.file_out.write("\t\t\tZ80free_resFlag(processor,F_PV);\n")
			return False
		
		if opcode=="CPI":
			self.file_out.write("\t\ttmp2=F_C&processor->Rm.br.F;\n")
			self.file_out.write("\t\ttmp1=Z80free_doArithmetic(processor,processor->Rm.br.A,Z80free_Rd(processor->Rm.wr.HL++),0,1);\n")
			self.file_out.write("\t\tif (processor->Rm.br.F&F_H)\n")
			self.file_out.write("\t\t\ttmp1--;\n")
			self.file_out.write("\t\tZ80free_valFlag(processor,F_3,tmp1&0x08);\n")
			self.file_out.write("\t\tZ80free_valFlag(processor,F_5,tmp1&0x02);\n")
			self.file_out.write("\t\tZ80free_setFlag(processor,F_N);\n")
			self.file_out.write("\t\tZ80free_valFlag(processor,F_C,tmp2);\n")
			self.file_out.write("\t\tprocessor->Rm.wr.BC--;\n")
			self.file_out.write("\t\tif (processor->Rm.wr.BC)\n")
			self.file_out.write("\t\t\tZ80free_setFlag(processor,F_PV);\n")
			self.file_out.write("\t\telse\n")
			self.file_out.write("\t\t\tZ80free_resFlag(processor,F_PV);\n")
			return False

		if opcode=="INI":
			self.file_out.write("\t\t/*INI, IND, INIR and INDR first decrement B and then uses it*/\n")
			self.file_out.write("\t\tZ80free_Wr(processor->Rm.wr.HL,Z80free_In(processor->Rm.wr.BC));\n")
			self.file_out.write("\t\tprocessor->Rm.wr.HL++;\n")
			self.file_out.write("\t\tprocessor->Rm.br.B=Z80free_doIncDec(processor,processor->Rm.br.B,1);\n")
			self.file_out.write("\t\tZ80free_valFlag(processor,F_N, 1);\n")
			self.file_out.write("\t\tZ80free_valFlag(processor,F_Z, (processor->Rm.br.B == 0));\n")
			return False
		
		if opcode=="OUTI":
			self.file_out.write("\t\t/*OUTI, OUTD, OTIR and OTDR first decrement B and then uses it*/\n")
			self.file_out.write("\t\tprocessor->Rm.br.B=Z80free_doIncDec(processor,processor->Rm.br.B,1);\n")
			self.file_out.write("\t\tZ80free_Out(processor->Rm.wr.BC,Z80free_Rd(processor->Rm.wr.HL));\n")
			self.file_out.write("\t\tprocessor->Rm.wr.HL++;\n")
			return False

		if opcode=="LDD":
			self.file_out.write("\t\ttmp1=Z80free_Rd(processor->Rm.wr.HL--);\n")
			self.file_out.write("\t\tZ80free_Wr(processor->Rm.wr.DE--,tmp1);\n")
			self.file_out.write("\t\ttmp1+=processor->Rm.br.A;\n")
			self.file_out.write("\t\tZ80free_valFlag(processor,F_3,tmp1&0x08);\n")
			self.file_out.write("\t\tZ80free_valFlag(processor,F_5,tmp1&0x02);\n")
			self.file_out.write("\t\tprocessor->Rm.wr.BC--;\n")
			self.file_out.write("\t\tZ80free_resFlag(processor,F_H|F_N);\n")
			self.file_out.write("\t\tif (processor->Rm.wr.BC)\n")
			self.file_out.write("\t\t\tZ80free_setFlag(processor,F_PV);\n")
			self.file_out.write("\t\telse\n")
			self.file_out.write("\t\t\tZ80free_resFlag(processor,F_PV);\n")
			return False

		if opcode=="CPD":
			self.file_out.write("\t\ttmp2=F_C&processor->Rm.br.F;\n")
			self.file_out.write("\t\ttmp1=Z80free_doArithmetic(processor,processor->Rm.br.A,Z80free_Rd(processor->Rm.wr.HL--),0,1);\n")
			self.file_out.write("\t\tif (processor->Rm.br.F&F_H)\n")
			self.file_out.write("\t\t\ttmp1--;\n")
			self.file_out.write("\t\tZ80free_valFlag(processor,F_3,tmp1&0x08);\n")
			self.file_out.write("\t\tZ80free_valFlag(processor,F_5,tmp1&0x02);\n")
			self.file_out.write("\t\tZ80free_setFlag(processor,F_N);\n")
			self.file_out.write("\t\tZ80free_valFlag(processor,F_C,tmp2);\n")
			self.file_out.write("\t\tprocessor->Rm.wr.BC--;\n")
			self.file_out.write("\t\tif (processor->Rm.wr.BC)\n")
			self.file_out.write("\t\t\tZ80free_setFlag(processor,F_PV);\n")
			self.file_out.write("\t\telse\n")
			self.file_out.write("\t\t\tZ80free_resFlag(processor,F_PV);\n")
			return False

		if opcode=="IND":
			self.file_out.write("\t\tZ80free_Wr(processor->Rm.wr.HL,Z80free_In(processor->Rm.wr.BC));\n")
			self.file_out.write("\t\tprocessor->Rm.br.B=Z80free_doIncDec(processor,processor->Rm.br.B,1);\n")
			self.file_out.write("\t\tprocessor->Rm.wr.HL--;\n")
			return False
		
		if opcode=="OUTD":
			self.file_out.write("\t\tprocessor->Rm.br.B=Z80free_doIncDec(processor,processor->Rm.br.B,1);\n")
			self.file_out.write("\t\tZ80free_Out(processor->Rm.wr.BC,Z80free_Rd(processor->Rm.wr.HL));\n")
			self.file_out.write("\t\tprocessor->Rm.wr.HL--;\n")
			return False

		if opcode=="LDIR":
			self.file_out.write("\t\ttmp1=Z80free_Rd(processor->Rm.wr.HL++);\n")
			self.file_out.write("\t\tZ80free_Wr(processor->Rm.wr.DE++,tmp1);\n")
			self.file_out.write("\t\tprocessor->Rm.wr.BC--;\n")
			self.file_out.write("\t\tif (processor->Rm.wr.BC) {\n")
			self.file_out.write("\t\t\tprocessor->PC-=2;\n")
			self.file_out.write("\t\t\treturn ("+str(tst1)+");\n")
			self.file_out.write("\t\t} else {\n")
			self.file_out.write("\t\t\tZ80free_resFlag(processor,F_H|F_N|F_PV);\n")
			self.file_out.write("\t\ttmp1+=processor->Rm.br.A;\n")
			self.file_out.write("\t\tZ80free_valFlag(processor,F_3,tmp1&0x08);\n")
			self.file_out.write("\t\tZ80free_valFlag(processor,F_5,tmp1&0x02);\n")
			self.file_out.write("\t\t\treturn ("+str(tst2)+");\n")
			self.file_out.write("\t\t}\n")
			return True

		if opcode=="CPIR":
			self.file_out.write("\t\ttmp2=F_C&processor->Rm.br.F;\n")
			self.file_out.write("\t\tZ80free_doArithmetic(processor,processor->Rm.br.A,Z80free_Rd(processor->Rm.wr.HL++),0,1);\n")
			self.file_out.write("\t\tprocessor->Rm.wr.BC--;\n")
			self.file_out.write("\t\tif ((processor->Rm.wr.BC)&&(!(processor->Rm.br.F&F_Z))) {\n")
			self.file_out.write("\t\t\tprocessor->PC-=2;\n")
			self.file_out.write("\t\t\tZ80free_valFlag(processor,F_C,tmp2);\n")
			self.file_out.write("\t\t\treturn ("+str(tst1)+");\n")
			self.file_out.write("\t\t} else {\n")
			self.file_out.write("\t\t\tZ80free_resFlag(processor,F_H|F_PV|F_3|F_5);\n")
			self.file_out.write("\t\t\tZ80free_setFlag(processor,F_N);\n")
			self.file_out.write("\t\tZ80free_valFlag(processor,F_C,tmp2);\n")
			self.file_out.write("\t\tif (processor->Rm.wr.BC)\n")
			self.file_out.write("\t\t\tZ80free_setFlag(processor,F_PV);")
			self.file_out.write("\t\t\treturn ("+str(tst2)+");\n")
			self.file_out.write("\t\t}\n")
			return True

		if opcode=="INIR":
			self.file_out.write("\t\tZ80free_Wr(processor->Rm.wr.HL,Z80free_In(processor->Rm.wr.BC));\n")
			self.file_out.write("\t\tprocessor->Rm.br.B=Z80free_doIncDec(processor,processor->Rm.br.B,1);\n")
			self.file_out.write("\t\tprocessor->Rm.wr.HL++;\n")
			self.file_out.write("\t\tif (processor->Rm.br.B) {\n")
			self.file_out.write("\t\t\tprocessor->PC-=2;\n")
			self.file_out.write("\t\t\treturn ("+str(tst1)+");\n")
			self.file_out.write("\t\t} else {\n")
			self.file_out.write("\t\t\treturn ("+str(tst2)+");\n")
			self.file_out.write("\t\t}\n")
			return True
		
		if opcode=="OTIR":
			self.file_out.write("\t\tprocessor->Rm.br.B=Z80free_doIncDec(processor,processor->Rm.br.B,1);\n")
			self.file_out.write("\t\tZ80free_Out(processor->Rm.wr.BC,Z80free_Rd(processor->Rm.wr.HL));\n")
			self.file_out.write("\t\tprocessor->Rm.wr.HL++;\n")
			self.file_out.write("\t\tif (processor->Rm.br.B) {\n")
			self.file_out.write("\t\t\tprocessor->PC-=2;\n")
			self.file_out.write("\t\t\treturn ("+str(tst1)+");\n")
			self.file_out.write("\t\t} else {\n")
			self.file_out.write("\t\t\treturn ("+str(tst2)+");\n")
			self.file_out.write("\t\t}\n")
			return True

		if opcode=="LDDR":
			self.file_out.write("\t\ttmp1=Z80free_Rd(processor->Rm.wr.HL--);\n")
			self.file_out.write("\t\tZ80free_Wr(processor->Rm.wr.DE--,tmp1);\n")
			self.file_out.write("\t\tprocessor->Rm.wr.BC--;\n")
			self.file_out.write("\t\tif (processor->Rm.wr.BC) {\n")
			self.file_out.write("\t\t\tprocessor->PC-=2;\n")
			self.file_out.write("\t\t\treturn ("+str(tst1)+");\n")
			self.file_out.write("\t\t} else {\n")
			self.file_out.write("\t\t\tZ80free_resFlag(processor,F_H|F_N|F_PV);\n")
			self.file_out.write("\t\ttmp1+=processor->Rm.br.A;\n")
			self.file_out.write("\t\tZ80free_valFlag(processor,F_3,tmp1&0x08);\n")
			self.file_out.write("\t\tZ80free_valFlag(processor,F_5,tmp1&0x02);\n")
			self.file_out.write("\t\t\treturn ("+str(tst2)+");\n")
			self.file_out.write("\t\t}\n")
			return True

		if opcode=="CPDR":
			self.file_out.write("\t\ttmp2=F_C&processor->Rm.br.F;\n")
			self.file_out.write("\t\tZ80free_doArithmetic(processor,processor->Rm.br.A,Z80free_Rd(processor->Rm.wr.HL--),0,1);\n")
			self.file_out.write("\t\tprocessor->Rm.wr.BC--;\n")
			self.file_out.write("\t\tif ((processor->Rm.wr.BC)&&(!(processor->Rm.br.F&F_Z))) {\n")
			self.file_out.write("\t\t\tprocessor->PC-=2;\n")
			self.file_out.write("\t\t\tZ80free_valFlag(processor,F_C,tmp2);\n")
			self.file_out.write("\t\t\treturn ("+str(tst1)+");\n")
			self.file_out.write("\t\t} else {\n")
			self.file_out.write("\t\t\tZ80free_resFlag(processor,F_H|F_PV|F_3|F_5);\n")
			self.file_out.write("\t\t\tZ80free_setFlag(processor,F_N);\n")
			self.file_out.write("\t\tZ80free_valFlag(processor,F_C,tmp2);\n")
			self.file_out.write("\t\tif (processor->Rm.wr.BC)\n")
			self.file_out.write("\t\t\tZ80free_setFlag(processor,F_PV);")
			self.file_out.write("\t\t\treturn ("+str(tst2)+");\n")
			self.file_out.write("\t\t}\n")
			return True

		if opcode=="INDR":
			self.file_out.write("\t\tZ80free_Wr(processor->Rm.wr.HL,Z80free_In(processor->Rm.wr.BC));\n")
			self.file_out.write("\t\tprocessor->Rm.br.B=Z80free_doIncDec(processor,processor->Rm.br.B,1);\n")
			self.file_out.write("\t\tprocessor->Rm.wr.HL--;\n")
			self.file_out.write("\t\tif (processor->Rm.br.B) {\n")
			self.file_out.write("\t\t\tprocessor->PC-=2;\n")
			self.file_out.write("\t\t\treturn ("+str(tst1)+");\n")
			self.file_out.write("\t\t} else {\n")
			self.file_out.write("\t\t\treturn ("+str(tst2)+");\n")
			self.file_out.write("\t\t}\n")
			return True
		
		if opcode=="OTDR":
			self.file_out.write("\t\tprocessor->Rm.br.B=Z80free_doIncDec(processor,processor->Rm.br.B,1);\n")
			self.file_out.write("\t\tZ80free_Out(processor->Rm.wr.BC,Z80free_Rd(processor->Rm.wr.HL));\n")
			self.file_out.write("\t\tprocessor->Rm.wr.HL--;\n")
			self.file_out.write("\t\tif (processor->Rm.br.B) {\n")
			self.file_out.write("\t\t\tprocessor->PC-=2;\n")
			self.file_out.write("\t\t\treturn ("+str(tst1)+");\n")
			self.file_out.write("\t\t} else {\n")
			self.file_out.write("\t\t\treturn ("+str(tst2)+");\n")
			self.file_out.write("\t\t}\n")
			return True

		if opcode=="RLC":
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","Z80free_doRLC(processor,1,"+code_read[0]+")")+"\n")
			return False
		
		if opcode=="LD_RLC":
			self.file_out.write("\t\ttmp1=Z80free_doRLC(processor,1,"+code_read[1]+");\n")
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","tmp1")+"\n")
			self.file_out.write("\t\t"+code_write[1].replace("@DATA@","tmp1")+"\n")
			return False
		
		if opcode=="RRC":
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","Z80free_doRRC(processor,1,"+code_read[0]+")")+"\n")
			return False
		
		if opcode=="LD_RRC":
			self.file_out.write("\t\ttmp1=Z80free_doRRC(processor,1,"+code_read[1]+");\n")
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","tmp1")+"\n")
			self.file_out.write("\t\t"+code_write[1].replace("@DATA@","tmp1")+"\n")
			return False
		
		if opcode=="RL":
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","Z80free_doRL(processor,1,"+code_read[0]+")")+"\n")
			return False
		
		if opcode=="LD_RL":
			self.file_out.write("\t\ttmp1=Z80free_doRL(processor,1,"+code_read[1]+");\n")
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","tmp1")+"\n")
			self.file_out.write("\t\t"+code_write[1].replace("@DATA@","tmp1")+"\n")
			return False
		
		if opcode=="RR":
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","Z80free_doRR(processor,1,"+code_read[0]+")")+"\n")
			return False
		
		if opcode=="LD_RR":
			self.file_out.write("\t\ttmp1=Z80free_doRR(processor,1,"+code_read[1]+");\n")
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","tmp1")+"\n")
			self.file_out.write("\t\t"+code_write[1].replace("@DATA@","tmp1")+"\n")
			return False
		
		if opcode=="SLA":
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","Z80free_doSL(processor,0,"+code_read[0]+")")+"\n")
			return False
		
		if opcode=="LD_SLA":
			self.file_out.write("\t\ttmp1=Z80free_doSL(processor,0,"+code_read[1]+");\n")
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","tmp1")+"\n")
			self.file_out.write("\t\t"+code_write[1].replace("@DATA@","tmp1")+"\n")
			return False
		
		if opcode=="SRA":
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","Z80free_doSR(processor,1,"+code_read[0]+")")+"\n")
			return False
		
		if opcode=="LD_SRA":
			self.file_out.write("\t\ttmp1=Z80free_doSR(processor,1,"+code_read[1]+");\n")
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","tmp1")+"\n")
			self.file_out.write("\t\t"+code_write[1].replace("@DATA@","tmp1")+"\n")
			return False
		
		if opcode=="SLL":
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","Z80free_doSL(processor,1,"+code_read[0]+")")+"\n")
			return False
		
		if opcode=="LD_SLL":
			self.file_out.write("\t\ttmp1=Z80free_doSL(processor,1,"+code_read[1]+");\n")
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","tmp1")+"\n")
			self.file_out.write("\t\t"+code_write[1].replace("@DATA@","tmp1")+"\n")
			return False
		
		if opcode=="SRL":
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","Z80free_doSR(processor,0,"+code_read[0]+")")+"\n")
			return False
		
		if opcode=="LD_SRL":
			self.file_out.write("\t\ttmp1=Z80free_doSR(processor,0,"+code_read[1]+");\n")
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","tmp1")+"\n")
			self.file_out.write("\t\t"+code_write[1].replace("@DATA@","tmp1")+"\n")
			return False
		
		if opcode=="BIT":
			self.file_out.write("\t\tZ80free_doBIT(processor,"+code_read[0]+","+code_read[1]+");\n")
			return False
		
		if opcode=="RES":
			self.file_out.write("\t\t"+code_write[1].replace("@DATA@","Z80free_doSetRes(processor,0,"+code_read[0]+","+code_read[1]+")")+"\n")
			return False
		
		if opcode=="LD_RES":
			self.file_out.write("\t\ttmp1=Z80free_doSetRes(processor,0,"+code_read[1]+","+code_read[2]+");\n")
			self.file_out.write("\t\t"+code_write[2].replace("@DATA@","tmp1")+"\n")
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","tmp1")+"\n")
			return False
		
		if opcode=="SET":
			self.file_out.write("\t\t"+code_write[1].replace("@DATA@","Z80free_doSetRes(processor,1,"+code_read[0]+","+code_read[1]+")")+"\n")
			return False
		
		if opcode=="LD_SET":
			self.file_out.write("\t\ttmp1=Z80free_doSetRes(processor,1,"+code_read[1]+","+code_read[2]+");\n")
			self.file_out.write("\t\t"+code_write[2].replace("@DATA@","tmp1")+"\n")
			self.file_out.write("\t\t"+code_write[0].replace("@DATA@","tmp1")+"\n")
			return False
		
		if opcode=="DEFAULT":
			self.file_out.write("\t\treturn (Z80free_codes(processor,opcode));\n")
			return True
		
		self.unknown_opcode()


class parser_XX(z80_parser):
	
	def __init__(self,filename):

		self.codes="XX"		
		z80_parser.__init__(self,filename)


	def write_start_code(self):
		
		""" Writes the first part of the C code for the file """

		self.file_out.write('#include "Z80free.h"\n\n')
		self.file_out.write('int '+self.filename+' (Z80FREE *processor,byte opcode) {\n')
		self.file_out.write('\tstatic byte tmp1;\n')
		self.file_out.write('\tstatic word tmp2;\n')
		self.file_out.write('\n')
		
		
	def write_end_code(self):
		
		""" writes the last part of the C code for the file """
		
		self.file_out.write('\treturn -1;\n')
		self.file_out.write('}\n')


class parser_CB(z80_parser):
	
	def __init__(self,filename):
		
		self.codes="CB"
		z80_parser.__init__(self,filename)


	def write_start_code(self):
		
		""" Writes the first part of the C code for the file """
		
		self.file_out.write('#include "Z80free.h"\n\n')
		self.file_out.write('int '+self.filename+' (Z80FREE *processor,byte opcode) {\n')
		#self.file_out.write('\tstatic byte tmp1;\n')
		#self.file_out.write('\tstatic word tmp2;\n')
		self.file_out.write('\n')
		
		
	def write_end_code(self):
		
		""" writes the last part of the C code for the file """
		
		self.file_out.write('\treturn -1;\n')
		self.file_out.write('}\n')


class parser_ED(z80_parser):
	
	def __init__(self,filename):

		self.codes="ED"		
		z80_parser.__init__(self,filename)


	def write_start_code(self):
		
		""" Writes the first part of the C code for the file """
	
		self.file_out.write('#include "Z80free.h"\n\n')
		self.file_out.write('int '+self.filename+' (Z80FREE *processor,byte opcode) {\n')
		self.file_out.write('\tstatic byte tmp1;\n')
		self.file_out.write('\tstatic byte tmp2;\n')
		self.file_out.write('\n')
		
		
	def write_end_code(self):
		
		""" writes the last part of the C code for the file """
		
		self.file_out.write('\treturn -1;\n')
		self.file_out.write('}\n')


class parser_DD(z80_parser):
	
	def __init__(self,filename):

		self.codes="DD"		
		z80_parser.__init__(self,filename)


	def write_start_code(self):
		
		""" Writes the first part of the C code for the file """
	
		self.file_out.write('#include "Z80free.h"\n\n')
		self.file_out.write('int '+self.filename+' (Z80FREE *processor,byte opcode) {\n')
		self.file_out.write('\tstatic byte tmp1;\n')
		self.file_out.write('\tstatic word tmp2;\n')
		self.file_out.write('\n')
		
		
	def write_end_code(self):
		
		""" writes the last part of the C code for the file """
		
		self.file_out.write('\treturn -1;\n')
		self.file_out.write('}\n')


class parser_FD(z80_parser):
	
	def __init__(self,filename):

		self.codes="FD"		
		z80_parser.__init__(self, filename)


	def write_start_code(self):
		
		""" Writes the first part of the C code for the file """
		
		self.file_out.write('#include "Z80free.h"\n\n')
		self.file_out.write('int '+self.filename+' (Z80FREE *processor,byte opcode) {\n')
		self.file_out.write('\tstatic byte tmp1;\n')
		self.file_out.write('\tstatic word tmp2;\n')
		self.file_out.write('\n')
		
		
	def write_end_code(self):
		
		""" writes the last part of the C code for the file """
		
		self.file_out.write('\treturn -1;\n')
		self.file_out.write('}\n')


class parser_DDCB(z80_parser):
	
	def __init__(self,filename):

		self.codes="DDCB"		
		z80_parser.__init__(self, filename)


	def write_start_code(self):
		
		""" Writes the first part of the C code for the file """
		
		self.file_out.write('#include "Z80free.h"\n\n')
		self.file_out.write('int '+self.filename+' (Z80FREE *processor,byte d1) {\n')
		self.file_out.write('\tstatic byte tmp1;\n')
		#self.file_out.write('\tstatic word tmp2;\n')
		self.file_out.write('\tstatic byte opcode;\n')
		self.file_out.write('\topcode=Z80free_Rd(processor->PC++);\n')
		self.file_out.write('\n')
		
		
	def write_end_code(self):
		
		""" writes the last part of the C code for the file """
		
		self.file_out.write('\treturn -1;\n')
		self.file_out.write('}\n')


class parser_FDCB(z80_parser):
	
	def __init__(self,filename):

		self.codes="FDCB"		
		z80_parser.__init__(self, filename)


	def write_start_code(self):
		
		""" Writes the first part of the C code for the file """
		
		self.file_out.write('#include "Z80free.h"\n\n')
		self.file_out.write('int '+self.filename+' (Z80FREE *processor,byte d1) {\n')
		self.file_out.write('\tstatic byte tmp1;\n')
		#self.file_out.write('\tstatic word tmp2;\n')
		self.file_out.write('\tstatic byte opcode;\n')
		self.file_out.write('\topcode=Z80free_Rd(processor->PC++);\n')
		self.file_out.write('\n')
		
		
	def write_end_code(self):
		
		""" writes the last part of the C code for the file """
		
		self.file_out.write('\treturn -1;\n')
		self.file_out.write('}\n')


aparser=parser_XX("Z80free_codes")
aparser=parser_CB("Z80free_codesCB")
aparser=parser_DD("Z80free_codesDD")
aparser=parser_DDCB("Z80free_codesDDCB")
aparser=parser_ED("Z80free_codesED")
aparser=parser_FD("Z80free_codesFD")
aparser=parser_FDCB("Z80free_codesFDCB")

