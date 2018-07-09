# Tanoti

Tanoti is a BLAST guided reference based short read aligner. It is developed for maximising alignment in highly variable next generation sequence data sets (Illumina).

Dynamic memory management and batch processing make Tanoti consume very little memory and processing power. Tanoti's read alignment performance is superior to BWA and Bowtie in our comparisons on small viral genomes, giving greater depth with highly variable reads without losing accuracy of alignment.


## Installation:

Tanoti installation requires C program compiler. 

Download Tanoti source and enter into Tanoti directory

_cd Tanoti/src_

_bash compile_tanoti.sh_

This will create binaries in bin directory inside Tanoti.

Set path to bin directory (in .bashrc or in .profile) and start using it


## USAGE

### Paired end reads:
_tanoti –r reference.fa –i input1.fq input2.fq -o output.sam –p 1_

### Single end reads:
_tanoti –r reference.fa –i input1.fq -o output.sam_

Where:

    -r : reference genome(s) in FASTA format
    -i : input files in FASTQ format
    -o : output file name (default: FinalAssembly.sam)
    -p : paired-end reads 1/0 (default 0)

Optional parameters:

    -m : minimum match percentage of a read (default 50%)
    -u : include unmapped reads in the output 1/0 (default 0)
    -t : Keep temporary files
    -P : number of parallel BLAST searches (default 2/single end and 4/paired end. Don’t change this value 
         if you are unsure of it)
    -h : Help
