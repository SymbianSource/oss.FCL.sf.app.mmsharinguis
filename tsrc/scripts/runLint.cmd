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

@goto Invoke

#!perl
#
# This Script Runs PCLint Static Code Analysis tool .
#
# runLint.cmd


# Run from a directory containing a bld.inf

use strict;
use Cwd;

my($pclintDir)= "pclint";
mkdir $pclintDir;

# Get Code Type.  The value for this is blank for production code and test for test code
my $type = "";
my $filterFile = "normal.lnt";
my $filterFileDir = "C:\\APPS\\pclint_8.0q";
my $destination;
if($#ARGV >= 0 ) {
$destination = $ARGV[0] ;
}
else{
$destination= cwd ;
}
$destination.="\\".'PCLint';
system("mkdir $destination") ;
$filterFileDir=$ARGV[1] if($#ARGV >= 1 );
$filterFile=$ARGV[2] if($#ARGV >= 2 );
$type = $ARGV[3] if($#ARGV >= 3 );
if($ARGV[0]=~m/-h|-H/)
	{
	usage();
	exit 0;
	}

# Re generate winscw makefile
#system( "bldmake -k bldfiles" );
#system( "abld -k $type makefile winscw" );

#copy .lnt file to local folder 
system( "copy $filterFileDir\\*.lnt . " );

# Run Lint over each of the requested makefiles
# The make file is used to determine source, include paths; and #defines to use
open WHATCMD, "abld $type makefile winscw -w |";

open(SUMMARY,">$destination\\summary.txt") || die " Can't Open Summary FileName \n";

while( <WHATCMD> )
{
	chomp;
	my $makeFile = $_;
	my @temparray;	
	if( $makeFile =~ /winscw$/i )
	{
		@temparray=split(/\./,$makeFile);
		@temparray=split(/\\/,$temparray[0]);
		my $fileName = pop(@temparray);
		$fileName = $fileName."\.txt";
		print "\nRunning PCLint for Module $fileName \n";		
		my $optionFile = "current.lnt";
		unlink( $optionFile ) if( -f $optionFile );
		GenerateOptions( $makeFile, $optionFile );		
		system( "lint-nt.exe -i $filterFile current.lnt >$destination\\$fileName");
		unlink( $optionFile ) if( -f $optionFile );		
		print SUMMARY "Module Name\t: $fileName \n";
		print SUMMARY " Total Errors\t: ".countError($fileName,'error')."\n";
		print SUMMARY " Total Warnings\t: ".countError($fileName,'Warning')."\n";
	}
}
close SUMMARY;
system( "del *.lnt" );

sub GenerateOptions($$)
# generate PC-LINT options file from winscw makefile
{
	my ($makeFile, $optionFile) = @_;

	open INP, $makeFile or die "Can't open $makeFile";
	my $line;
	my $cwd;
	while(defined($line = <INP>))
	{
		chomp $line;
		if($line =~ /^# CWD (.+)/)
		{
			$cwd = $1;
			last;
		}
	}

	my $inc = "";
	my $defs = "";
	my $src = "";
	my $totLine = "";
	while(defined($line = <INP>))
	{
		chomp $line;
		if($line =~ /(.*)\\$/)
		{
			$totLine .= $1;
		}
		else
		{
			$totLine .= $line;
			if($inc eq "" && $totLine =~ /^\s*INCDIR\s*=(.+)/)
			{
				my @dirs = split /\s+-i\s+/, $1;
				foreach my $dir (@dirs)
				{
					$dir =~ s/-cwd source/ /g;
					$dir =~ s/-i-/ /g;
					$dir =~ s/^\s*//;
					$dir =~ s/\s*$//;

					my @forceIncList = split /\s+-include\s+/, $dir;
					my $isInc = 1;
					foreach my $force (@forceIncList)
					{
						$force =~ s/\"//g;
						if( $force )
						{
							$inc .= $isInc? "-i$force\n" : "-header($force)\n";
							$isInc = 0;
						}
					}
				}
			}
			elsif($defs eq "" && $totLine =~ /^\s*CWDEFS\s*=\s*(.+)/)
			{
				my @macs = split /\s*-d/, $1;
				foreach my $mac (@macs)
				{
					$mac =~ s/\"//g;
					$mac =~ s/^ //;
					$mac =~ s/\s.+$//g;
					$defs .= "-d$mac\n";
				}
			}
			elsif($totLine =~ /^\$\(EPOCBLDUREL\)\\.+\.o : (\S+)/)	# NB! this presumes that source file paths never include whitespace! if they do then (\S+) has to change to allow for dbl-quote packaging
			{
				$src .= "$1\n" if( $1 !~ /\.uid\.cpp$/i );
			}
			$totLine = "";
		}
	}

	open OUT, ">$optionFile" or die "can't create lint options file $optionFile";
	
	print OUT "$inc$defs-d_DEBUG\n-d_UNICODE\n$src\n";
	
	close OUT;
}

sub usage
	{
	print "\n\t Usage : \n";
	print "\t\t runLint <result_destination_directory> <pclint_directory> <filter_file> <option> \n ";	
	print "\t\t Option : The value for this is blank for production code and test for test code\n";	
	print "\t\t Example:runLint C:\\test C:\\APPS\\pclint_8.0q normal.lnt \n";
	print "\n\t Note:This script should be run from where bld.inf file available\n";		
	print "\t      If no arguments available then default will be used.\n";		
	print "\n\t runLint -h -> To print this help\n";		
	}


sub countError
{
my($fileName,$searchStr)= @_;
my($count) = 0;
open FILE , "$destination\\$fileName" or die " Can't open $fileName ";
my @lines = <FILE>;
foreach my $line (@lines)
	{
	if($line=~/: $searchStr/)
		{
		$count++;
		}
	}
close FILE;
return $count;
}


__END__

:Invoke
@perl -x -S M:\multimediasharing\internal\group\runLint.cmd %1 %2 %3 %4 %5 %6 %7 %8 %9
