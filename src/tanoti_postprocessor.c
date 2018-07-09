/*
Program for
Sreenu Vattipally
MRC-CVR, University of Glasgow
Glasgow, UK
22/Apr/2013
*/

#include "tanoti.h"

int main (int argc, char **argv){ 
FILE *input, *output;
char buffer[MAX_LINE_LEN+1], Ref_Name[MAX_REF_LEN], Read_Seq[MAX_READ_LEN],\
Ref_Seq[MAX_READ_LEN], RefGen[50], ID[20];
int Read_Num=0, Read_Len=0, Hsp_Num=0, Ref_Len=0, Read_From=0,\
Read_To=0, Ref_From=0, Ref_To=0, Identity=0, Align_Len=0, print_flag=0, MATCH_PERCENTAGE=50, IDENTITY=90;

	if((input = fopen(argv[1], "r"))!=NULL){ 
        MATCH_PERCENTAGE=atoi(argv[2]); 
        IDENTITY=atoi(argv[5]); 
		sprintf(ID, "%d", getppid()); 
		strcpy(RefGen, "tnt_8_"); strcat(RefGen, argv[3]); strcat(RefGen, argv[4]); strcat(RefGen, ID); 
		output=fopen(RefGen,"a");
		while (!feof(input)) { 
			fgets(buffer,MAX_LINE_LEN,input); if(feof(input)) break; 
			if(strstr(buffer,"<Iteration_query-def>")) Read_Num=GetBlastInt(buffer);
			if(strstr(buffer,"<Iteration_query-len>")) Read_Len=GetBlastInt(buffer);
			if(strstr(buffer,"<Hsp_num>")) Hsp_Num=GetBlastInt(buffer);
			if(strstr(buffer,"<Hsp_query-from>")) Read_From=GetBlastInt(buffer);
			if(strstr(buffer,"<Hsp_query-to>")) Read_To=GetBlastInt(buffer);
			if(strstr(buffer,"<Hsp_hit-from>")) Ref_From=GetBlastInt(buffer);
			if(strstr(buffer,"<Hsp_hit-to>")) Ref_To=GetBlastInt(buffer);
			if(strstr(buffer,"<Hsp_identity>")) Identity=GetBlastInt(buffer);
			if(strstr(buffer,"<Hsp_align-len>")) Align_Len=GetBlastInt(buffer);
			if(strstr(buffer,"<Hsp_qseq>")) GetBlastStr(Read_Seq,buffer);
			if(strstr(buffer,"<Hsp_hseq>")) GetBlastStr(Ref_Seq, buffer);
			if(strstr(buffer,"<Hit_def>")) GetBlastStr(Ref_Name, buffer);
			if(strstr(buffer,"<Hit_len>")) Ref_Len=GetBlastInt(buffer);

			if(strstr(buffer,"</Hsp>")){  						// End of a hit
				if((Align_Len >= (Read_Len*MATCH_PERCENTAGE)/100) && Identity >=IDENTITY){        // Check if the alignemt is greater than or equal to the match percentage
				//if(Align_Len >= (Read_Len*MATCH_PERCENTAGE)/100){		// Check if the alignemt is greater than or equal to the match percentage
					if(Hsp_Num==1 && print_flag==0){
						printf("%d\t%d\t%d\t%d\t%d\t%s\t%s\t%s\t",Read_Num,Ref_From,Ref_To,Read_From,Read_To,Ref_Name,Read_Seq,Ref_Seq);
						fprintf(output,"@SQ\tSN:%s\tLN:%d\n",Ref_Name,Ref_Len);
						print_flag++;
					}
					if(Hsp_Num > 1){
                        if(print_flag==0){
						printf("%d\t%d\t%d\t%d\t%d\t%s\t%s\t%s\t",Read_Num,Ref_From,Ref_To,Read_From,Read_To,Ref_Name,Read_Seq,Ref_Seq);
						fprintf(output,"@SQ\tSN:%s\tLN:%d\n",Ref_Name,Ref_Len);
						print_flag++;
						}
                    //  else
					//	printf("TX:%d:%d:%d ",Read_From,Ref_From,(Identity*100)/Read_Len); 
                    //  printf("MULTIPLE ");
					}
				}
				Ref_From=Ref_To=Read_From=Read_To=Align_Len=Identity=0;
				Ref_Seq[0]='\0'; Read_Seq[0]='\0';
			}

			if(strstr(buffer,"</Iteration>") && Read_Num!=0){
				if(print_flag==0) printf("%d\t0\t0\t0\t0\t#\t#\t#\t0\n",Read_Num);
				if(print_flag==1) printf("###\t1\n");
				if(print_flag >1) printf("MULTIPLE\t%d\n",print_flag);
				Ref_Seq[0]='\0'; Read_Seq[0]='\0';
				Read_Num=Ref_From=Ref_To=Read_From=Read_To=Align_Len=Identity=Hsp_Num=print_flag=Read_Len=0;
			}
		} 
		// If no mapped read at the end
		if(print_flag==0) fprintf(output,"@SQ\tSN:%s\tLN:%d\n",Ref_Name,Ref_Len);
		fclose(output);
	}
}
