#include "tanoti.h"

int main(int argc, char **argv){ 
    
FILE *RefFile, *input1, *input2; 
int i=0, j=0, k=0,insertion=0, deletion=0, match=0, ReadLen=0, BaseCount=0, FLAG=0, MapQ=0,TLEN=0,count1,count2,\
	UnMapped=0,RefStart1=0, RefEnd1=0, ReadNum1=0,ReadStart1=0,ReadEnd1=0, HitCount1=0,\
	RefStart2=0, RefEnd2=0, ReadNum2=0,ReadStart2=0,ReadEnd2=0, HitCount2=0;

char c, temp[TempSize], buffer1[MAX_LINE_LEN+1], buffer2[MAX_LINE_LEN+1],\
	RefName1[TempSize], RefName2[TempSize],\
	**ReadNames1, **ReadSeq1, **SeqQual1,\
	**ReadSeq2, **SeqQual2,\
	HitReadSeq1[TagLen], HitRefSeq1[TagLen],\
	HitReadSeq2[TagLen], HitRefSeq2[TagLen];

// Check input file number
if(argc!=10) { printf("Wrong parameters %d\n",argc); exit(0); }

// Should unmapped reads be included in the final alignment?
// Default is NO
UnMapped=atoi(argv[9]);

// Memory Allocation
// First parsed file.
MemAllocate (&ReadNames1, LibSize, TagLen); 
MemAllocate (&ReadSeq1, LibSize, TagLen); 
MemAllocate (&SeqQual1, LibSize, TagLen); 
// Get Names, Reads and Qualities from the first file
FileRead(argv[1],&ReadNames1); 
FileRead(argv[2],&ReadSeq1); 
count1=FileRead(argv[3],&SeqQual1);

// Second parsed file.
MemAllocate (&ReadSeq2, LibSize, TagLen); 
MemAllocate (&SeqQual2, LibSize, TagLen); 
// Get Names, Reads and Qualities from the second file
FileRead(argv[5],&ReadSeq2); 
count2=FileRead(argv[6],&SeqQual2);

// If unequal paired ends
if(count1!=count2){
    printf("ERROR: Unequal paired end records.\n");
    MemFree(&ReadNames1, LibSize);
    MemFree(&ReadSeq1, LibSize);
    MemFree(&SeqQual1, LibSize);
    MemFree(&ReadSeq2, LibSize);
    MemFree(&SeqQual2, LibSize);
    exit(0);
}

// Reading Reference Name and Length
//GetRefNameLen(argv[7],RefName,&BaseCount);

// Print Header for the first time.
if(argv[1][strlen(argv[1])-1]=='1' && argv[1][strlen(argv[1])-2]=='_')
	printf("@PG\tID:Tanoti\tPN:Tanoti Assembler\tVN:1.0\n");


// Reading Processed BLAST output 
if((input1 = fopen(argv[7], "r"))!=NULL &&\
	(input2 = fopen(argv[8], "r"))!=NULL){


	while (!feof(input1)) { 
	fgets(buffer2,MAX_LINE_LEN,input2); if(feof(input2)) break;
	fgets(buffer1,MAX_LINE_LEN,input1); 

	ReadNum1=GetInt(buffer1,1); RefStart1=GetInt(buffer1,2); RefEnd1=GetInt(buffer1,3); ReadStart1=GetInt(buffer1,4); ReadEnd1=GetInt(buffer1,5); GetStr(RefName1,buffer1,6); GetStr(HitReadSeq1,buffer1,7); GetStr(HitRefSeq1,buffer1,8); HitCount1=GetInt(buffer1,10);
	ReadNum2=GetInt(buffer2,1); RefStart2=GetInt(buffer2,2); RefEnd2=GetInt(buffer2,3); ReadStart2=GetInt(buffer2,4); ReadEnd2=GetInt(buffer2,5); GetStr(RefName2,buffer2,6); GetStr(HitReadSeq2,buffer2,7); GetStr(HitRefSeq2,buffer2,8); HitCount2=GetInt(buffer2,10);


	//First Read in the Read pair 
	// For mapped reads
	if (HitCount1>0){ 

		FLAG=GetFLAG(ReadStart1,ReadEnd1,ReadStart2,ReadEnd2,HitCount1, HitCount2,1);
		MapQ = GetMapQ(HitCount1);

		printf("%s\t%d\t%s\t%d\t%d\t",ReadNames1[ReadNum1-1],FLAG,RefName1,RefStart1,MapQ);
        
        ReadLen=strlen(ReadSeq1[ReadNum1-1]);

        // Calculating CIGAR value 
		// Positive strand match
        if (ReadStart1 < ReadEnd1)
			PrintCIGAR(HitReadSeq1, HitRefSeq1, ReadStart1, ReadEnd1,ReadLen,1); 
        // Negative strand Match 
        else
			PrintCIGAR(HitReadSeq1, HitRefSeq1, ReadStart1, ReadEnd1,ReadLen,-1); 

		// Print RNEXT and PNEXT. RNEXT is set to "=" because we are using a single reference. PNEXT is set to zero if nomatch or multiple matches.
		if (HitCount2==1) {
			if(strcmp(RefName1,RefName2)==0) printf("\t=\t%d\t", RefStart2);
			else printf("\t%s\t%d\t",RefName2,RefStart2);
		}
		else printf("\t*\t0\t");

		// Print TLEN
		if(HitCount1==1 && HitCount2==1){
			if(strcmp(RefName1,RefName2)==0) TLEN=GetTLEN(RefStart1,RefEnd1,RefStart2,RefEnd2,1);
			else TLEN=0;
		}
		else TLEN=0;

		printf("%d\t",TLEN);

        // Printing  Sequence and Qualities
		// Positive strand
        if(ReadStart1 < ReadEnd1) PrintSeqQual(ReadSeq1[ReadNum1-1], SeqQual1[ReadNum1-1],1); 
        // Negative strand
        else PrintSeqQual(ReadSeq1[ReadNum1-1], SeqQual1[ReadNum1-1],-1); 
        printf("\n"); 
		}
		
		// This is for unmapped reads
	else { 
		if (UnMapped==1) {
			FLAG=GetUFLAG(ReadStart1,ReadEnd1,ReadStart2,ReadEnd2,HitCount1,HitCount2,1);
				printf("%s\t%d\t*\t0\t0\t*\t*\t0\t0\t%s\t%s\n",ReadNames1[ReadNum1-1],FLAG,ReadSeq1[ReadNum1-1],SeqQual1[ReadNum1-1]);
			}
	}

// Second Read in the Read Pair
	// For mapped reads
	if (HitCount2>0){ 

		FLAG=GetFLAG(ReadStart2,ReadEnd2,ReadStart1,ReadEnd1,HitCount2, HitCount1,2);
		MapQ = GetMapQ(HitCount2);

		printf("%s\t%d\t%s\t%d\t%d\t",ReadNames1[ReadNum2-1],FLAG,RefName2,RefStart2,MapQ);

        ReadLen=strlen(ReadSeq2[ReadNum2-1]);

        // Calculating CIGAR value 
		// Positive strand match
        if (ReadStart2 < ReadEnd2)
			PrintCIGAR(HitReadSeq2, HitRefSeq2, ReadStart2, ReadEnd2,ReadLen,1); 
        // Negative strand Match 
        else
			PrintCIGAR(HitReadSeq2, HitRefSeq2, ReadStart2, ReadEnd2,ReadLen,-1); 

		// Print RNEXT and PNEXT. RNEXT is set to "=" because we are using a single reference. PNEXT is set to zero if nomatch or multiple matches.
		if (HitCount1==1){ 
			if(strcmp(RefName1,RefName2)==0) printf("\t=\t%d\t", RefStart1);
			else printf("\t%s\t%d\t",RefName1,RefStart1);
		}
		else printf("\t*\t0\t");

		// Print TLEN
		if(HitCount1==1 && HitCount2==1){
			if(strcmp(RefName1,RefName2)==0) TLEN=GetTLEN(RefStart1,RefEnd1,RefStart2,RefEnd2,2);
			else TLEN=0;
		}
		else TLEN=0;

		printf("%d\t",TLEN);

        // Printing  Sequence and Qualities
		// Positive strand
        if(ReadStart2 < ReadEnd2) PrintSeqQual(ReadSeq2[ReadNum2-1], SeqQual2[ReadNum2-1],1); 
        // Negative strand
        else PrintSeqQual(ReadSeq2[ReadNum2-1], SeqQual2[ReadNum2-1],-1); 
        printf("\n"); 
		}
		
		// This is for unmapped reads
	//else { if (UnMapped==1) { printf("%s\t4\t*\t0\t0\t*\t*\t0\t0\t%s\t%s\n",ReadNames1[ReadNum2-1],ReadSeq2[ReadNum2-1],SeqQual2[ReadNum2-1]); } }
	else { 
		if (UnMapped==1) {
			FLAG=GetUFLAG(ReadStart2,ReadEnd2,ReadStart1,ReadEnd1,HitCount2,HitCount1,2);
				printf("%s\t%d\t*\t0\t0\t*\t*\t0\t0\t%s\t%s\n",ReadNames1[ReadNum2-1],FLAG,ReadSeq2[ReadNum2-1],SeqQual2[ReadNum2-1]);
			}
	}


		}
		fclose(input1); fclose(input2); 
	} 

// Free Memory 
MemFree(&ReadNames1, LibSize);
MemFree(&ReadSeq1, LibSize);
MemFree(&SeqQual1, LibSize);
MemFree(&ReadSeq2, LibSize);
MemFree(&SeqQual2, LibSize);
}
