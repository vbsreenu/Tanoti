/*
Program for splitting FASTQ file into Names, Sequences (free seq and fasta), and Qualities files.
Used in TANOTI program.

Usage: ProgramName FastQFile

Sreenu Vattipally
MRC-CVR, University of Glasgow
Glasgow, UK

11/Apr/2013
*/

#include "tanoti.h"

int main (int argc, char **argv){

	FILE *input, *outName, *outSeq, *outSeqFA, *outQual, *outNumb;
	int flag=0, line=1, num=1, count=0, new_file=-1, seq_flag=1, name_flag=0, FA_index=0;
	char c, Name[20], Seq[20], SeqFA[20], Qual[20], FileNumb[20], ID[20], file_num[10], paired_end=1;
	
	if((input = fopen(argv[1], "r"))!=NULL){ 
		
		sprintf(ID, "%d", getppid()); 
		sprintf(file_num, "%d", num); 

		if (argc==3) paired_end=atoi(argv[2]);
		
		if (paired_end==1){
			strcpy(Name, "tnt_1_1_"); strcat(Name, ID); strcat(Name,"_"); strcat(Name,file_num);
			strcpy(Seq, "tnt_1_2_"); strcat(Seq, ID); strcat(Seq,"_");strcat(Seq,file_num);
			strcpy(SeqFA, "tnt_1_3_"); strcat(SeqFA, ID); strcat(SeqFA,"_");strcat(SeqFA,file_num);
			strcpy(Qual, "tnt_1_4_"); strcat(Qual, ID); strcat(Qual,"_");strcat(Qual,file_num);
		}

		else if (atoi(argv[2])==2){
			strcpy(Name, "tnt_2_1_"); strcat(Name, ID); strcat(Name,"_"); strcat(Name,file_num);
			strcpy(Seq, "tnt_2_2_"); strcat(Seq, ID); strcat(Seq,"_");strcat(Seq,file_num);
			strcpy(SeqFA, "tnt_2_3_"); strcat(SeqFA, ID); strcat(SeqFA,"_");strcat(SeqFA,file_num);
			strcpy(Qual, "tnt_2_4_"); strcat(Qual, ID); strcat(Qual,"_");strcat(Qual,file_num);
		}
		else { fclose(input); exit(0);}

		outName=fopen(Name, "w");
		outSeq=fopen(Seq, "w");
		outSeqFA=fopen(SeqFA, "w");
		outQual=fopen(Qual, "w");

		while((c = fgetc(input))) {
			if (feof(input)){ strcpy(FileNumb, "tnt_0_"); strcat(FileNumb, ID); outNumb=fopen(FileNumb, "w"); fprintf(outNumb,"%d\n",num); fclose(outNumb); break; }
			if(c=='@' && line==1) {flag=1; if(new_file!=-1) new_file=1; seq_flag=1; name_flag=0; FA_index++;}
			if (count%FILE_LIMIT==0 && new_file==1){ 
				new_file=0; FA_index=1;
				num++;
				file_num[0]='\0'; Name[0]='\0'; Seq[0]='\0'; Qual[0]='\0'; SeqFA[0]='\0'; 
				fclose(outName); fclose(outSeq); fclose(outSeqFA);fclose(outQual); 
				sprintf(ID, "%d", getppid()); 
				sprintf(file_num, "%d", num); 

			if (paired_end==1){
				strcpy(Name, "tnt_1_1_"); strcat(Name, ID); strcat(Name,"_"); strcat(Name,file_num);
				strcpy(Seq, "tnt_1_2_"); strcat(Seq, ID); strcat(Seq,"_");strcat(Seq,file_num);
				strcpy(SeqFA, "tnt_1_3_"); strcat(SeqFA, ID); strcat(SeqFA,"_");strcat(SeqFA,file_num);
				strcpy(Qual, "tnt_1_4_"); strcat(Qual, ID); strcat(Qual,"_");strcat(Qual,file_num);
			}

			else if (atoi(argv[2])==2){
				strcpy(Name, "tnt_2_1_"); strcat(Name, ID); strcat(Name,"_"); strcat(Name,file_num);
				strcpy(Seq, "tnt_2_2_"); strcat(Seq, ID); strcat(Seq,"_");strcat(Seq,file_num);
				strcpy(SeqFA, "tnt_2_3_"); strcat(SeqFA, ID); strcat(SeqFA,"_");strcat(SeqFA,file_num);
				strcpy(Qual, "tnt_2_4_"); strcat(Qual, ID); strcat(Qual,"_");strcat(Qual,file_num);
			}

			outName=fopen(Name, "w"); 
			outSeq=fopen(Seq, "w"); 
			outSeqFA=fopen(SeqFA, "w"); 
			outQual=fopen(Qual, "w");
		}
		if(flag==1 && line%4==1) { if(name_flag!=0) putc(c, outName); name_flag++;}
		if(flag==1 && line%4==2) { if (seq_flag==1) fprintf(outSeqFA,">%d\n",FA_index);  if(c!=' ') {putc(c, outSeq); putc(c,outSeqFA);} seq_flag=0;}
		if(flag==1 && line%4==0) { if(c!=' ') putc(c, outQual); if(c=='\n') {flag=0; line=1; count++; new_file=1;}}
		if(c=='\n' && flag==1) line++; 
	}
	fclose(input); fclose(outName); fclose(outSeq); fclose(outSeqFA); fclose(outQual); 
}
}
