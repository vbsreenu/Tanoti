#include "tanoti.h"

int main(int argc, char **argv){ 
    
    FILE *input; 
    int i=0, j=0, k=0,insertion=0, deletion=0, match=0, ReadLen=0, BaseCount=0, HitCount=0, MapQ=0,count=0, flag=0;
	char c, temp[TempSize], buffer[MAX_LINE_LEN+1], RefName[TempSize], **ReadNames, **ReadSeq, **SeqQual,HitReadSeq[TagLen], HitRefSeq[TagLen];
	int RefStart=0, RefEnd=0, ReadNum=0,ReadStart=0,ReadEnd=0, UnMapped=0;

    if(argc!=6) { printf("Wrong parameters\n"); exit(0); }
	UnMapped=atoi(argv[5]);

	// Memory Allocation
	MemAllocate (&ReadNames, LibSize, TagLen); 
	MemAllocate (&ReadSeq, LibSize, TagLen); 
	MemAllocate (&SeqQual, LibSize, TagLen); 
	FileRead(argv[1],&ReadNames); 
	FileRead(argv[2],&ReadSeq); 
	FileRead(argv[3],&SeqQual);

	// Reading Reference Name and Length
	//GetRefNameLen(argv[4],RefName,&BaseCount); 
	if(argv[1][strlen(argv[1])-1]=='1' && argv[1][strlen(argv[1])-2]=='_'){ printf("@PG\tID:Tanoti\tPN:Tanoti Assembler\tVN:1.0\n");} 

    // Reading Processed BLAST output 
    if((input = fopen(argv[4], "r"))!=NULL){ 


		while (!feof(input)) { 
		fgets(buffer,MAX_LINE_LEN,input); if(feof(input)) break;
		ReadNum=GetInt(buffer,1); RefStart=GetInt(buffer,2); RefEnd=GetInt(buffer,3); ReadStart=GetInt(buffer,4); ReadEnd=GetInt(buffer,5); GetStr(RefName,buffer,6); GetStr(HitReadSeq,buffer,7); GetStr(HitRefSeq,buffer,8); HitCount=GetInt(buffer,10);

		// For mapped reads
		if (HitCount>0){

        printf("%s\t",ReadNames[ReadNum-1]);

        if(ReadStart > ReadEnd) { printf("16\t"); } else { printf("0\t"); }
		MapQ = GetMapQ(HitCount);
        printf("%s\t%d\t%d\t",RefName,RefStart,MapQ); 
        
        ReadLen=strlen(ReadSeq[ReadNum-1]);

        // Calculating CIGAR value 
		// Positive strand match 
		if (ReadStart < ReadEnd) 
			PrintCIGAR(HitReadSeq, HitRefSeq, ReadStart, ReadEnd,ReadLen,1); 
		// Negative strand Match 
		else 
			PrintCIGAR(HitReadSeq, HitRefSeq, ReadStart, ReadEnd,ReadLen,-1);
        
        printf("\t*\t0\t0\t");

        // Printing  Sequence and Qualities 
		// Positive strand 
		if(ReadStart < ReadEnd) PrintSeqQual(ReadSeq[ReadNum-1], SeqQual[ReadNum-1],1); 
		// Negative strand 
		else PrintSeqQual(ReadSeq[ReadNum-1], SeqQual[ReadNum-1],-1); 
        printf("\n"); 

		}
		// This is for unmapped reads
		else {
			if (UnMapped==1) {
				printf("%s\t4\t*\t0\t0\t*\t*\t0\t0\t%s\t%s\n",ReadNames[ReadNum-1],ReadSeq[ReadNum-1],SeqQual[ReadNum-1]);
			}
		}
	} 
	fclose(input); 
	} 
// Free Memory 
MemFree(&ReadNames, LibSize);
MemFree(&ReadSeq, LibSize);
MemFree(&SeqQual, LibSize);
}
