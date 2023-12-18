#include <stdio.h>
#include <stdlib.h>

void write16(FILE* f,int v) {
	char c[2];
	c[0]=v>>8;
	c[1]=v&0xFF;
	fwrite(c,2,1,f);
}

int main(int argc,char** argv) {
	if(argc!=2) {
		printf("Usage:\n");
		printf("ea5split <ea5_file>\n");
		return(1);
	}

	FILE* f;
	f=fopen(argv[1],"rb");
	if(f==NULL) {
		printf("Cannot open \"%s\"\n",argv[1]);
		return(1);
	}
	fseek(f,0,SEEK_END);
	int l=ftell(f);
	fseek(f,0,SEEK_SET);
	
	if(l>0x2000) {
		unsigned char* data=malloc(l);
		fread(data,l,1,f);
		fclose(f);
		
		int load_address=data[4]*256+data[5];
		int end_address=load_address+l-6-1;
		if(end_address>0xFFFF || end_address>0x3FFF && end_address<0xA000) {
			printf("Program loaded outside expansion RAM\n");
			free(data);
			return(1);
		}
		
		char* ext=strchr(argv[1],'.');
		char* name=malloc(strlen(argv[1])+1);
		strcpy(name,argv[1]);
		if(ext!=NULL)name[ext-argv[1]]='\0';


		int files_number=(l-6)/(0x2000-6);
		if((l-6)%(0x2000-6))files_number++;
		int name_l=strlen(name);
		char c=name[name_l-1]+files_number-1;
		if(c>'Z' && c<'a' || c>'z') {
			name[name_l]='1';
			name[name_l+1]='\0';
		}
		
		char* filename=malloc(strlen(name)+strlen(ext));
		
		int i,l2,offset;
		offset=6;
		for(i=0;i<files_number;i++) {
			strcpy(filename,name);
			strcat(filename,ext);
			
			l2=l-offset;
			if(l2>0x2000-6)l2=0x2000-6;
			
			f=fopen(filename,"wb");
			if(i==files_number-1)write16(f,0x000);
			else write16(f,0xFFFF);
			write16(f,l2);
			write16(f,load_address);
			fwrite(data+offset,l2,1,f);
			fclose(f);
			
			load_address+=l2;
			offset+=l2;
			name[strlen(name)-1]++;
		}
		
		free(name);
		free(data);
		free(filename);
	}
	else fclose(f);

	return(0);
}
