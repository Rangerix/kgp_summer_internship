#!usr/bin/perl
#use List::Util qw(first_index);
open ASA, "ASA1.txt" or die "Cannot open file\n";
@lines=<ASA>;
foreach $line(@lines){
chomp;
system "cp $line prot.pdb;./accall<1accall.input;";
@arr=split(/\./,$line);
$arr[2]="rsa";
$line1=join('.',@arr);
system "cp prot.rsa $line1;rm prot.asa prot.rsa prot.log;rm prot.pdb;";
#print $line1;
#system "cp $line prot.pdb;./accall<1accall.input;cp prot.rsa $OUTDIR/$file.rsa;rm prot.asa prot.rsa prot.log;rm prot.pdb;";
}
#$DOCKDIR = '/home/Anupam/Desktop/naccess/input/cluspro.126961';  #Docking Directory containing docked proteins
#$OUTDIR = $ARGV[0];

#@files = <$DOCKDIR/*.pdb>;
#foreach $file (@files) {
#print $line;
#break;	
#}
