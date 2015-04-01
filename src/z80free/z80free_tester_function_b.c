
int read_elements(Z80FREE *emulador) {

	int tstates,retval;
	unsigned short int v1,v2,v3,v4,v5,v6,v7,v8,v9,v10;

	Z80free_reset(emulador);

	retval=fscanf(infile,"%hx %hx %hx %hx",(unsigned short int *)&emulador->Rm.wr.AF,(unsigned short int *)&emulador->Rm.wr.BC,(unsigned short int *)&emulador->Rm.wr.DE,(unsigned short int *)&emulador->Rm.wr.HL);
	retval=fscanf(infile,"%hx %hx %hx %hx",&v7,&v8,&v9,&v10);
	emulador->Ra.wr.AF=v7;
	emulador->Ra.wr.BC=v8;
	emulador->Ra.wr.DE=v9;
	emulador->Ra.wr.HL=v10;
	retval=fscanf(infile,"%hx %hx %hx %hx",(unsigned short int *)&emulador->Rm.wr.IX,(unsigned short int *)&emulador->Rm.wr.IY,(unsigned short int *)&emulador->Rm.wr.SP,(unsigned short int *)&emulador->PC);
	retval=fscanf(infile,"%hx %hx %hx %hx %hx %hx %d",&v1,&v2,&v3,&v4,&v5,&v6,&tstates);

	emulador->I=v1;
	emulador->R=v2;
	emulador->R2=v2;
	emulador->IFF1=v3;
	emulador->IFF2=v4;
	emulador->IM=v5;
	emulador->HALT=v6;

	return (tstates);
}

int read_memory(char mode) {
	// mode=0 : store value in memory
	// mode=1 : compare value with memory

	int position,retval,value;
	

	retval=fscanf(infile,"%x",&position);
	if (position==-1) {
		return -1;
	}

	do {
		retval=fscanf(infile,"%x",&value);
		
		if (value!=-1) {
			if (mode) {
				if (memory[position]!=value) {
					printf("Error in memory, address %X. Expected value: %X, current value: %X\n",position,value,memory[position]);
				}
			} else {
				memory[position]= (byte) value;
			}
			position++;
		}
	} while(value!=-1);
	return 0;
}

int read_inports() {

	int retval, value, addr;
	

	portpos=0;
	elements=0;

	while (1) {
		retval=fscanf(infile,"%x",&addr);
		if (addr==-1) {
			break;
		}
		retval=fscanf(infile,"%x",&value); 
		portdirs[elements]=(word) addr;
		ports[elements]=(byte) value;
		elements++;
	}
	return 0;
}

