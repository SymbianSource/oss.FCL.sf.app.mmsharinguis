rem
rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
rem All rights reserved.
rem This component and the accompanying materials are made available
rem under the terms of "Eclipse Public License v1.0"
rem which accompanies this distribution, and is available
rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
rem
rem Initial Contributors:
rem Nokia Corporation - initial contribution.
rem
rem Contributors:
rem
rem Description:
rem

@goto invoke_perl

#!perl
#line 5

################################################################################################################
# This Script will be used to count errors and warnings in abld
################################################################################################################

#	Packages needed
use strict;					# strict naming rules
use Getopt::Long;		# parameter handling
use Cwd;						# Directory operations
Getopt::Long::Configure( "bundling_override","ignore_case_always" );

my($param_help,
	 $param_dest);
my($ecount)=0;
my($wcount)=0;
my($destination)=cwd;

# read commandline parameters
my $result = GetOptions('help'			=> \$param_help,						# Display help info												
												'dest=s' 		=> \$param_dest);					  # Destination directory												
if(defined $param_dest){
	$destination=$param_dest ;		
	}
else{
	print "\n Note: No results destination directory specified.\n";
	$destination=~s/\//\\/g;
	$destination=$destination."\\";
	}	
	
$destination.="\Build";
mkdir($destination);
print " Note: Results will be copied to $destination\n";
open SUMMARY,">$destination\\summary.txt" or die "Can't Open $!";
system("bldmake bldfiles");
display_critical_error() if($?!=0);   
system("abld reallyclean winscw urel");
print "\n *** Building for winscw urel and logging in file $destination\\winscw_log.txt *** \n";
system("abld build winscw urel >$destination\\winscw_log.txt");
display_critical_error() if($?!=0);   
print "\n *** End of Building for winscw urel *** \n";
open LOGFILE,"$destination\\winscw_log.txt" or die "Can't Open $!";
print "\n *** Writing summary for winscw *** \n";
while(<LOGFILE>){
 if($_=~/error/){
 $ecount++;
 }
 if($_=~/warning/){
 $wcount++;
 }
}
close LOGFILE;
print SUMMARY "WINSCW build Errors\t=$ecount \n";
print SUMMARY "WINSCW build Warnings\t=$wcount \n";
$ecount=0;
$wcount=0;
print "\n *** Building for armv5 urel and logging in $destination\\armv5_log.txt *** \n";
system("abld reallyclean armv5 urel");
system("abld build armv5 urel >$destination\\armv5_log.txt");
display_critical_error() if($?!=0);
print "\n *** End of Building for armv5 urel *** \n";
open LOGFILE,"$destination\\armv5_log.txt" or die "Can't Open $!";
print "\n *** Writing summary for armv5 *** \n ";
while(<LOGFILE>){
 if($_=~/error/){
 $ecount++;
 }
 if($_=~/warning/){
 $wcount++;
 }
}
close LOGFILE;
print SUMMARY "ARMV5 build Errors\t=$ecount \n";
print SUMMARY "ARMV5 build Warnings\t=$wcount \n";
close LOGFILE;
close SUMMARY;
print "\n *** DONE *** \n";

sub display_critical_error {  
  print "\n-> Critical Error Occured : ".$?."\n" if(!@_[0]);  
  print "\n-> Critical Error Occured : ".@_[0]."\n" if(@_[0]);  	  
  exit 1;
  }	
__END__

:invoke_perl
@perl -x -S buildcheck.cmd %1 %2 %3 %4 %5 %6 %7 %8 %9
