#!/usr/local/bin/perl

# _verstr = "@(#)$Id: msgc.pl 48880 2009-03-17 19:59:28Z raghu@syncsort.com $"

# This perl wrapper modifies some auto-generated java mdata class files.
# Only GUI requires these modifications. 

##############################################################
# insert static int member into the auto generated java code #
##############################################################
sub updateJava 
{
    # parse arguements
    my $destination = shift(@_);
    my $className = shift(@_);
    my $addStaticValue = shift(@_);

    
    my $fileName = "$destination/$className.java";
    my $line = "";

    # read destination input file
    unless (open(IN, $fileName)){
#	print STDERR "Can't open input $fileName: $!\n";
	return;
    }

    my @lines = <IN>;
    close IN;

    foreach $line (@lines){
	if ($line =~ m/$addStaticValue/){
	    return;
	} 
    }

    # insert the static int member to the output file
    unless (open (OUT, '>' .  $fileName)){
#	print "fail to open output file\n";
	return;
    }

    my $pattern = "public class $className extends msgBase";
    foreach $line (@lines){
	print OUT $line;
	if ($line =~ m/$pattern/){
	    print OUT $addStaticValue;
	} 
    }
    close OUT;
}

##############################################################
# find out which java file needs updates and call updateJava #
##############################################################
sub postProcessX {
    my $destinations = shift(@_);   
    my $key = shift(@_);
    my @classNameLists = split(",", $fileListHash{$key});
    my $classNames;
    foreach $classNames (@classNameLists){
	my $addStaticValues = $fileContentsHash{$classNames};
	updateJava ($destinations, $classNames, $addStaticValues);    
    }
}

sub postProcess {
    my $destinations = shift(@_);   
    my @classNameLists = keys %fileContentsHash;
    my $classNames;
    foreach $classNames (@classNameLists){
	my $addStaticValues = $fileContentsHash{$classNames};
	updateJava ($destinations, $classNames, $addStaticValues);    
    }
}


################################################################
# fileContentsHash: store instered contents for each java file #
################################################################
%fileContentsHash = (
   "MDat_file_info" =>   "  public static int path_encoding = 65001;  public static int path_encoding_flag = 0;\n",
   "MDat_qsubdirs" =>   "  public static int dirid_encoding = 65001; public static int dirid_encoding_flag = 0;\n",
   "MDat_file_ver_info" =>   "  public static int path_encoding = 65001;  public static int path_encoding_flag = 0;\n",
   "MDat_report_file_versions" =>   "  public static int sellistname_encoding = 65001;  public static int sellistname_encoding_flag = 0;\n",
   "MDat_jobdef" =>   "  public static int defp_encoding = 65001;  public static int defp_encoding_flag = 0;\n",
   "MDat_seldir" =>   "  public static int defp_encoding = 65001;  public static int defp_encoding_flag = 0;\n",
   "MDat_group" =>   "  public static int groupname_encoding = 65001;  public static int groupname_encoding_flag = 0;\n",
   "MDat_catalog_search" =>   "  public static int directory_encoding = 65001;  public static int pattern_encoding = 65001; public static int directory_encoding_flag = 0;  public static int pattern_encoding_flag = 0;\n",  
);



##############################################################
# main routine:                                              # 
#   checks if need to update the java file after msgc_orig   #
##############################################################
@systemArgs = ("/bex/devtools/sun2/msgc", @ARGV);
system(@systemArgs);

while (@ARGV){
    if (shift(@ARGV) eq "-j"){
        postProcess(shift(@ARGV));
    }
}




