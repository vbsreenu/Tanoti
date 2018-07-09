#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>

#define FILE_LIMIT      100000     /* Number of reads in each file, while splitting the large files */
#define LibSize         100010     /* This is the size of each library file ( >= FILE_LIMIT )*/
#define TagLen          500         /* Max Read length */
#define MAX_LINE_LEN    1028        /* Max line length in a file */
#define TempSize        500         /* To keep Reference name */
#define MAX_READ_LEN    1028        /* Max Read length */
#define MAX_REF_LEN     500         /* Max Reference name length */

// Used in Assembler
int GetInt(char *String, int Column){ 
	int i, j=0, flag=1, IntValue=0; char temp[50]; temp[0]='\0'; 
	for(i=0;;i++){ 
		if(String[i]=='\t' || String[i]=='\n') flag++; 
		if(flag > Column) { temp[j]='\0'; IntValue=atoi(temp);}
		if(flag==Column && String[i]!='\t' && String[i]!='\n') temp[j++]=String[i];
		if(i==strlen(String) || flag > Column) break;
	} return IntValue; 
}

// Used in Assembler
void GetStr(char *CharVar, char *String, int Column){ 
	int i, j=0, flag=1;
	for(i=0;;i++){ 
		if(String[i]=='\t' || String[i]=='\n') flag++; 
		if(flag > Column) { CharVar[j]='\0';}
		if(flag==Column && String[i]!='\t' && String[i]!='\n') CharVar[j++]=String[i];
		if(i==strlen(String) || flag > Column) break;
	}
	CharVar[j]='\0';
}


// Used in Assembler
// OMG... a function with a TRIPLE pointer :) 

int FileRead(char *FileName, char ***FileStore){ 
	FILE *input; int i=0,j=0; char c; 
	if((input = fopen(FileName, "r"))!=NULL){ 
		while((c = fgetc(input))) { 
			if (feof(input)) { (*FileStore)[i][j]='\0'; break; } 
			if(c=='\n') {(*FileStore)[i][j]='\0'; i++; j=0; continue;} 
			else (*FileStore)[i][j++]=c; 
		} fclose(input); 
	} return i;
}

// Used in Assembler
// Again some fun with TRIPLE pointers :D

void MemAllocate(char ***Ary, int Row, int Column)
{
	int i; 
	*Ary = (char **)malloc(sizeof(char *)*Row); 
	for(i = 0; i< Row; i++) (*Ary)[i] = (char *)malloc(sizeof(char) *Column);
}

void MemFree (char ***Ary, int Size){
	int i;
	for (i = 0; i < Size; i++) free((*Ary)[i]); 
	free(*Ary);
}

// Used in Post-processing
int GetBlastInt(char *String){ 
	int IntValue=0, i=0, j=0, flag=0;
	char temp[100]; 
	for(i=0;;i++){ 
		if(String[i]=='>') flag=1; 
		if(String[i]=='<' && flag==1) { temp[j]='\0'; IntValue=atoi(temp); break;} 
		if(i==strlen(String)) break;    // precautionary break if it escapes above  
		if(flag==1 && String[i]!='>') temp[j++]=String[i]; 
	} return IntValue; 
}

// Used in Post-processing
void GetBlastStr(char *CharVar, char *String){ 
	int i=0, j=0, flag=0; 
	for(i=0;;i++){ 
		if(String[i]=='>') flag=1;
		if(String[i]=='<' && flag==1) { CharVar[j]='\0'; break;} 
		if(i==strlen(String)) break; 
		if(flag==1 && String[i]!='>') CharVar[j++]=String[i]; 
	} 
}

void GetRefNameLen(char *FileName, char *RefName, int *BaseCount){
	FILE *input; 
	int i=0, j=0, flag=0;
	char c, temp[100];
	temp[0]='\0';

	if((input = fopen(FileName, "r"))!=NULL){ 
		while((c = fgetc(input))) { 
			if (feof(input)) break; 
			if(c=='\t') flag++; 
			if(flag==1) {RefName[i]='\0'; flag++;} 
			if(flag==3) {temp[j]='\0'; *BaseCount=atoi(temp);} 
			if(flag==0) RefName[i++]=c; 
			if(flag==2 && c!='\t') temp[j++]=c; 
		} fclose(input); 
	}
}

void PrintCIGAR(char *HitReadSeq, char *HitRefSeq, int ReadStart, int ReadEnd, int ReadLen, int strand){

	int k=0, insertion=0,deletion=0,match=0;
	 

	for(k=0;k<strlen(HitReadSeq);k++){ 
		// Print initial soft clipping 
		if (strand==-1)
			if (k==0 && ReadStart<ReadLen) printf("%dS",ReadLen-ReadStart);
		if (strand==1)
			if (k==0 && ReadStart>1) printf("%dS",ReadStart-1);

		// Match and mismatch 
		if(HitReadSeq[k]!='-' && HitRefSeq[k]!='-') { if(insertion>0){ printf("%dI", insertion); insertion=0; } if(deletion>0){ printf("%dD", deletion); deletion=0; } match++; } 

		// Deletion 
		if(HitReadSeq[k]=='-'){ if(match>0){ printf("%dM",match); match=0; } if(insertion>0){ printf("%dI",insertion); insertion=0; } deletion++; } 

		// Insertion 
		if(HitRefSeq[k]=='-'){ if(match>0){ printf("%dM",match); match=0; } if(deletion>0){ printf("%dD", deletion); deletion=0; } insertion++; } 

		// End of the sequence 
		if(k==strlen(HitReadSeq)-1){ 
			if(match>0){ printf("%dM",match); match=0; } 
			if(insertion>0){ printf("%dI", insertion); insertion=0; } 
			if(deletion>0){ printf("%dD", deletion); deletion=0; } 
			// Print final soft clipping 
			if (strand==-1)
				if(ReadEnd>1) printf("%dS",ReadEnd-1);
			if (strand==1)	
				if(ReadEnd<ReadLen)printf("%dS",ReadLen-ReadEnd);
		} 
	} // End of For 
}

void PrintSeqQual(char *Seq, char *Qual, int strand){ 

	int i;

	// If Negative strand 
	if(strand == -1){	
		for(i=strlen(Seq)-1;i>=0;i--){ if(Seq[i]=='A') printf("T"); else if(Seq[i]=='T') printf("A"); else if(Seq[i]=='G') printf("C"); else if(Seq[i]=='C') printf("G"); else printf("%c",Seq[i]); }
		printf("\t"); 
		for(i=strlen(Seq)-1;i>=0;i--) printf("%c",Qual[i]); 
	} 
	// If Positive strand 
	else printf("%s\t%s",Seq, Qual); 
}

int GetMapQ(int HitCount){

//  MapQ = -10 log10(P);
//	Tanoti uses MapQ values as specified in SAM format specification.  It yields 4 distinct values 
//	25 = unique mapping
//	3 = maps to 2 locations in the target
//	2 = maps to 3 locations
//	1 = maps to 4-9 locations
//	0 = maps to 10 or more locations.

// Where P = probability that this mapping is NOT the correct one. 
// Tanoti ignores the number of mismatches in this calculation and simply assume that if it maps to 2 locations then P = 0.5, 3 locations implies P = 2/3, 4 locations => P = 3/4 etc.
// Then MapQ = -10 log10(0.5) = 3; -10 log10(2/3) = 1.76 (rounds to 2); -10 log10(3/4) = 1.25 (rounds to 1), etc.

// See http://seqanswers.com/forums/showthread.php?t=3142 for more information.

	int MapQ=0;

        if(HitCount == 1 ) MapQ=25; 
		if(HitCount == 2 ) MapQ=3; 
		if(HitCount == 3 ) MapQ=2; 
		if(HitCount > 3  && HitCount < 10) MapQ=1; 
		if(HitCount >= 10) MapQ=0; 

		return MapQ;
}

int GetTLEN (int start1, int end1, int start2, int end2, int position){
int TLEN=0, min=0,max=0;

min=start1<start2?start1:start2;
max=end1>end2?end1:end2;

TLEN=(max-min)+1;

if (start1==start2 && position==2) TLEN*=-1;
if(position==1 && start1 > start2 ) TLEN*=-1;
if(position==2 && start2 > start1 ) TLEN*=-1;

return TLEN;
}

int GetFLAG(int start1, int end1, int start2, int end2, int count1, int count2, int position) {
/*  1 - Paired reads 
    2 - Mapped in Proper pair 
    4 - Read unmapped
    8 - Mate unmapped
    16 - Read reversed
    32 - Mate reversed
    64 - First in the Pair
    128 - Second in the pair */

	int FLAG=0;

	FLAG+=1; FLAG+=2;
	if (count1==0) FLAG+=4;
	if (count2==0) FLAG+=8;
	if (start1 > end1) FLAG+=16;
	if (start2 > end2) FLAG+=32;
	if (position==1) FLAG+=64;
	if (position==2) FLAG+=128;

	return FLAG;
}

int GetUFLAG(int start1, int end1, int start2, int end2, int count1, int count2, int position) {

	int FLAG=0;

	FLAG+=1; 
	if (count1==0) FLAG+=4;
	if (count2==0) FLAG+=8;
	if (start2 > end2) FLAG+=32;
	if (position==1) FLAG+=64;
	if (position==2) FLAG+=128;
	return FLAG;
}
