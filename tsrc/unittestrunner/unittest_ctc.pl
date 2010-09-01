#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:
#
#!/usr/bin/perl

use File::Copy;
use Cwd;
use Getopt::Long;
use XML::Parser::Expat;

#---------------------------------------Initialization------------------------------------------------------#

$projectrootname = "/"; #is set to correct at run-time 
$projectdrive = "z:"; #is set to correct at run-time 

$eunitUnitTestRunner = "eunittest_ctc.pl";
$scriptLocation = "mmsharinguis/tsrc/unittestrunner";
$coverageResultsDirDefault = "mmsharinguis/tsrc/unittestrunner/results/";
$buildResults = "BuildResults.txt";
$coverageDat = "MON.dat";
$coverageSymbols = "MON.sym";
$coverageProfile = "profile.txt";

$eunitResultsFile = "EUnitLog.xml";
$coverageResultsFile = "CTCHTML/index.html";
$finalResultsFile = "AllResults.html";
$cssFile = "CTCHTML/ctc.css";
$cssLink = "<link rel=\"stylesheet\"";

$xmlDllLine = "dll";
$xmlResultLine = "result";
$xmlCaseFailed = "FAIL";
$xmlCasePassed = "OK";
$outputString = "";
$outputFileBodyStart = "<body";

$totalCount = 0;
$passedCount = 0;
$failedCount = 0;

# from command line
my ($param_noclean,
		$eunitDlls,
		$allocFailure,
		$coverageResultsDir);

#---------------------------------------Main Start----------------------------------------------------------#

# read command line parameters
my $result = GetOptions("noclean" 			  => \$param_noclean,
												"eunitdlls:s"	    => \$eunitDlls,
												"allocfailure:i"  => \$allocFailure,
												"results:s"       => \$coverageResultsDir,
												"help"						=> \$help);
												
$startdir = cwd;

if (defined $help)
{
	print_help();
	exit;
}

# find project root
findProjectDriveAndRoot();

# create directory for results
if ( defined $coverageResultsDir )
{
    if ($coverageResultsDir =~ /^(.:)/)
    {
        print("Drive name given in results dir arg\n");
    }
    else
    {  
        $coverageResultsDir = "$projectdrive" . "/" . "$coverageResultsDir";
    }
}
else
{
    $coverageResultsDir = "$projectdrive$projectrootname$coverageResultsDirDefault";
}
print ("Writing results to $coverageResultsDir \n\n");
if (mkdir("$coverageResultsDir", 0755))
{
    print("The results directory was created successfully! \n");
} 
else 
{
    print("The results directory already exists. \n");
} 

# set target for intrumentation result
$ENV{'CTC_DATA_PATH'}= "$coverageResultsDir";	

my $eunit_command = "$eunitUnitTestRunner -drive=$projectdrive -root=$projectrootname -results=$coverageResultsDir -eunitdlls=$eunitDlls -allocfailure=$allocFailure";
if (!defined $param_noclean)
{
	# clear previous results
	print("Cleaning previous results. \n");
	unlink("$coverageResultsDir$buildResults");
	unlink("$coverageResultsDir$coverageDat");
	unlink("$coverageResultsDir$coverageSymbols");
	unlink("$coverageResultsDir$coverageProfile");
	unlink("$coverageResultsDir$finalResultsFile");
}
else
{ 	
	$eunit_command .= " -noclean";
}


# run eunit tests
system("$eunit_command");
chdir($startdir);
	

# create textual output
chdir("$coverageResultsDir");
doSystemCall("ctcpost $coverageSymbols $coverageDat -p $coverageProfile -T 70");
doSystemCall("ctc2html -i $coverageProfile -t 70");


# clear target for intrumentation result
$ENV{'CTC_DATA_PATH'}= "";	

# combine all results
#
open(COVERAGE, "$coverageResultsDir$coverageResultsFile") or die("file $coverageResultsFile not found!\n");
@coverageFileContent = <COVERAGE>;

# eunit

$eunitOutputString = "<span class=\"head1\">EUnit Results</span><br><br>\n";

if ( -e "$coverageResultsDir$eunitResultsFile" )
{
	open(EUNITRESULTS,  "$coverageResultsDir$eunitResultsFile") or die ("Couldn't open $projectdrive$projectrootname$coverageResultsDir$eunitResultsFile!\n");
	$parser = new XML::Parser::Expat;
	$parser->setHandlers('Start' => \&sh,
                     'End'   => \&eh,
                     'Char'  => \&ch);
	$totalCount = 0;
	$passedCount = 0;
	$failedCount = 0;
		
	$parser->parse(*EUNITRESULTS);
		
	close(EUNITRESULTS);
}
else
{
	print ("file $eunitResultsFile not found!\n");
}


# append coverage information after eunit results

for ($j = 0; $j <= $#coverageFileContent; $j++)
	{
		$currentLine = @coverageFileContent[$j];
		
		if ($currentLine =~ /$cssLink/)
		{
			if ( open(CSSFILE,  "$coverageResultsDir$cssFile") )
			{
				# append css styles to results html
				$outputString .= "<style type=\"text/css\"> body {";
				@cssFileContent = <CSSFILE>;
				my($line);
				foreach $line (@cssFileContent)
				{ 
					$outputString .= $line;
				}
				$outputString .= "}</style>\n";
				close(CSSFILE);
			}
		}
		else
		{
			$outputString .= $currentLine;

			if ($currentLine =~ /$outputFileBodyStart/)
			{
				$outputString .= $eunitOutputString;
			}
		}
		
	}
	
open(NEWRESULTSFILE, "> $coverageResultsDir$finalResultsFile") or die "Cannot open final results file\n";
print NEWRESULTSFILE "$outputString";
close(NEWRESULTSFILE);
close(OUTPUT);
close(COVERAGE);

chdir($startdir);

#---------------------------------------Main Ends-------------------------------------------------------------#

sub findProjectDriveAndRoot()
{
		$tempStartDir = $startdir;
		if ($tempStartDir =~ m/$scriptLocation/)
		{
			# extract project root
			for ($tempStartDir) {
				 s/$scriptLocation+$//;
    	}
    	# divide it to drive and root
    	if ($tempStartDir =~ /^(.:)(.*)/)
    	{
    		$projectdrive = $1;
    		$projectrootname = $2;
			}
			print "project drive: $projectdrive \n";
			print "project root: $projectrootname \n";
		}
		else
		{
			print "cannot determine project drive and root, use defaults!\n";
		}
}

sub sh
{
  my ($p, $el, %atts) = @_;
	if ( $el eq $xmlDllLine )
	{
		$eunitOutputString .= "<span class=\"head3\">";
		$dllName = %atts->{name};
		@dllNameParts = split m!(\\)!, $dllName;
		$dllName = $dllNameParts[$#dllNameParts];
		$eunitOutputString .= "$dllName => </span>";
		$totalCount = %atts->{size};
	}
	
	if ( $el eq $xmlResultLine )
	{
		$status = %atts->{status};
		if ( $status eq $xmlCasePassed )
		{
			$passedCount++;
		}
		else
		{
			$failedCount++;
		}
	}
}

sub eh
{
	my ($p, $el) = @_;
	if ( $el eq $xmlDllLine )
	{
		$totalCount = $passedCount + $failedCount;
		if ( $failedCount > 0 || $totalCount == 0 )
		{
			$eunitOutputString .= "<span class=\"red\">Testcases passed/run: $passedCount/$totalCount </span><br>\n";
		}
		else
		{
			$eunitOutputString .= "<span class=\"blue\">Testcases passed/run: $passedCount/$totalCount </span><br>\n";
		}
		$passedCount = 0;	
		$failedCount = 0;
	}
}

sub ch
{
	my ($p, $el) = @_;
	#print ("$el\n");
}

sub doSystemCall
{
	#print("\nDoing system call: $_[0]\n");
	#system($_[0]);
	system("echo Doing system call: $_[0] >> $coverageResultsDir$buildResults");
	system("$_[0] >> $coverageResultsDir$buildResults 2>&1");
}

sub print_help
{
	print("\n*************************************************************\n\n");
	print("Script runs by default all multimediasharing eunit tests\n");
	print("and creates build, test and coverage results to:\n\n");
	print("    mmsharing/src/internal/unittestrunner/results/\n\n");
	print("Own configuration file (list of components to be processed)\n");
	print("can be used as well:\n\n");
	print("    >unittest_ctc.pl -eunitdlls=myowneunitcomponents.txt\n\n");
	print("The text file should follow the syntax of default configuration file\n");
	print("mmsharing/src/internal/unittestrunner/eunitdlls_ctc.txt\n\n");
	print("Alloc failure simulation can be turned on with following command:\n\n");
	print("    >unittest_ctc.pl -allocfailure=2\n\n");
	print("Any number will turn eunit alloc on.\n");
	print("When running just some set of components you might want to \"merge\" the results");
	print("with existing coverage results.\n");
	print("That is possible with command:\n\n");
	print("    >unittest_ctc.pl -eunitdlls=myowneunitcomponents.txt -noclean\n");
	print("\n*************************************************************\n");
}
