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
#!perl
#line 5

#--------------------------------------------------------------------------------------------------
# This Script will be used to run EUnit testcases
# Example Usage in Nightly build script
# 	my $currentdir = cwd;
#		chdir "$ProjLocation\\mmsharing\\internal\\group";
#		system ("EunitRunner.pl -command=eu  -all -dest=$Plog -summary=$Plog\\scanlog.html 2>&1 >> $Plog\\EUnit.log");
#		system ("EunitRunner.pl -command=ctc -all -dest=$Plog -summary=$Plog\\scanlog.html  2>&1 >> $Plog\\CTC.log");
#		chdir $currentdir;
# Version : 1.0
#--------------------------------------------------------------------------------------------------

#	Perl packages used.
use File::Find;			# File operations
use XML::Simple;		# XML parsing
use File::Spec::Win32;  # File operartions
use Getopt::Long;		# parameter handling
use Cwd;						# Directory operations

# Parameters related to Epoc.
my($build_destination_dir)= "\\Epoc32\\release\\winscw\\udeb\\";
my($build_tools_dir)= "\\Epoc32\\tools\\";

# Parameters related to Digia Eunit.
my($eunit_xml_file_name)= "EUnit_Log.xml";
my($eunit_xml_file)= "\\Epoc32\\winscw\\C\\shared\\EUnit\\Logs\\".$eunit_xml_file_name;
my($eunitParams) = "/l xml /e S60AppEnv /w 40 /p All /t 50";
my($eunitParams2) = " /d alloc";
my($eunitApp) = "/epoc32/release/winscw/udeb/EUnitExeRunner.exe";
my($eunit_destination_dir)= "\\Epoc32\\release\\winscw\\udeb\\z\\sys\\bin\\";
my($unittest_result_file)="eunit_result.html";

# Parameters related to Codetest(ctc).
$coverageSymbols = "MON.sym";
$ctcCommandPart1 = "ctc-abld-winscw -i d -n";
$ctcCommandPart2 = "-- test build winscw udeb"; 
$ctcIgnoredPart1 = " -C \"EXCLUDE+*/internal/*.cpp";
$ctcIgnoredPart2 = "\"";
$coverageProfile = "profile.txt";
my($ctc_result_file)="./\CTCHTML/\indexF.html";

# Parameters related to filenames.
#my($overal_summary_file)="Overall_Summary.html";
my($overal_summary_file)="scanlog.html";

# Hack : This will be used if dll names are not retrived from build environment.
my(@default_subsystems)=("UT_AVAILABILITY",
                         "UT_AOPLUGIN",
                         "UT_SIPCRPLUGIN",
                         "UT_MUSAIWPROVIDER",
                         "UT_MUSWPADAPTER",
                         "UT_MUSMANAGERCLIENT",
                         "UT_MUSMANAGERSERVER",
                         "UT_MUSENGINE" );
my(@default_dll_filenames)=("UT_AVAILABILITY.DLL",
												    "UT_AOPLUGIN.DLL",
												    "UT_SIPCRPLUGIN.DLL",
												    "UT_MUSAIWPROVIDER.DLL",
												    "UT_MUSWPADAPTER.DLL",
												    "UT_MUSMANAGERCLIENT.DLL",
												    "UT_MUSMANAGERSERVER.DLL",
												    "UT_MUSENGINE.DLL" );
												    

my @uncompiled_sub_systems;# Holds list of uncompiled subsystems . Will be printed in report later.
my($currentdir)= cwd; # Current working directory.
my($results_destinationdir)=cwd;	# Results will be stored here.
my($results_destinationdir_log)=cwd; # Log prints will be stored here.
my ($unittest_log_file) = "EUnit.log"; # Eunit build process logs will be in this file.

# Parameters related to result prints
my($eunit_passpercent,
	 $eunit_failpercent,
	 $eunit_total_pass_count,
	 $eunit_total_fail_count,
	 $eunit_total_count,
	 $eunit_total_pass_percent);
my(@eunit_testsuite_name,
	 @eunit_testsuite_total_testcase,
	 @eunit_testsuite_pass_count);

# Parameters related to globally used variables.
my (@subsystems, 						# List of user selected sub systems
		@sourcefiles,						# List of user selected source files
		@exclude_sourcefiles,		# List of test source files.
		@makefiles,							# List of make files
		@dll_filenames);				# List of dll file names
		
my ($param_help,		 			  # To view help
		$param_command,		 		  # Command to Run
		$param_dest,						# Copy results to
		$param_all,							# Build all
		$param_noclean,					# Do not run abld clean
		$param_nobuild,					# Do not run abld build
		$param_nostub,					# Do not build stub		
		$param_exe,			        # Specify exe file to be run instead Eunirexerunner.exe
		$param_overall_summay,  # Overall Summary
		$param_version);				# Version		
my @gloabl_errors;
		
# Parameters for user choice.		
my($choice,
	 $user_selection,
	 $count);

#--------------------------------------------------------------------------------------------------
#																				Main
#--------------------------------------------------------------------------------------------------

#**************************************************************************************************
# Read commandline options.
#**************************************************************************************************
my $result = GetOptions('help'			=> \$param_help,						# Display help info
												'command=s'    => \$param_command,			# Command to run
												'dest=s' 		=> \$param_dest,					  # Destination directory
												'all'				=> \$param_all,       			# Run all												
 												'noclean'  	=> \$param_noclean,    			# Dont run reallyclean
												'nobuild'		=> \$param_nobuild, 		    # Dont run symbian build commands
												'nostub'		=> \$param_nostub, 				  # Dont build stupbs												
												'exe=s'     => \$param_exe,			        # Launch the exe specified.if nothing then epoc will be used
												'summary=s' => \$param_overall_summay,	# Launch the exe specified.if nothing then epoc will be used
												'version'   => \$param_version);				# Version Information

#Test
#my $mapped_drive = "\\\\Vagrp006\\groups5\\SP_IDO\\IPASDo_MSDo";
#$results_destinationdir = "$mapped_drive\\Builds\\multimediasharing\\3.2\\NightlyBuilds\\2008-wk8\\18-Monday\\Log\\EUnitAndCTC";
#exit 1;

#**************************************************************************************************
# Step 1 : Pre Cleanup.
#**************************************************************************************************
	run_system_command("del "."$eunit_destination_dir"."*.dll");  	
	run_system_command("del "."$eunit_xml_file");  	

#**************************************************************************************************
# Step 2 : Check pre-conditions.
#**************************************************************************************************
	if (! -e $build_destination_dir."EUnitExeRunner.exe") 
		{	
  	print "*** EUnit is not installed for this SDK ***\n"; 
  	exit 1;  
  	}

#************************************************************************************************** 
# Step 2 : Check command line parameters and configure if not present to default.
#**************************************************************************************************
	if(defined $param_overall_summay)
		{
		$overal_summary_file = 	$param_overall_summay ;
		}
	if ($param_command eq ('ctc'|'CTC'|'Ctc') )
		{
		$choice=1;
		}
	elsif ($param_command eq ('eu'|'EU'))
		{
		$choice=2;
		}
	else
		{
		usage();
		}
	$param_exe=$eunitApp if(!defined $param_exe);
	if(defined $param_dest)
		{
		$results_destinationdir=$param_dest ;		
		}
	else
		{
		print "\n Note: No results destination directory specified. Results will be copied to $currentdir/\Eunit\n";	
		}	
	$results_destinationdir=~s/\//\\/g;
	$results_destinationdir_log = "$results_destinationdir";# This is were logs are stored in nightly build env.
	run_system_command("mkdir $results_destinationdir\\EUnitAndCTC") if (! -e "$results_destinationdir\\EUnitAndCTC");
	$results_destinationdir="$results_destinationdir\\EUnitAndCTC";
	$param_noclean=1 if(defined $param_nobuild);
	$ENV{'CTC_DATA_PATH'}= "";	
	run_system_command("bldmake bldfiles") if (!defined $param_nobuild);

#************************************************************************************************** 
# Step 3 : Display subsystems to user. So he can make a choice for which one to compile
#          if command line parameter -all specified then it will not be shown.
#************************************************************************************************** 
	display_sybsystems(); 
	if(defined $param_all)
		{
		print "\nOptions -all specified.Building all components \n ";
		$user_selection=0;
		}
	else 
		{	
		print "\t Options selected : ";
		$user_selection = <STDIN>;
		chomp ($user_selection);
		exit 0 if $user_selection =~ /^[Qq]/;
		}

#************************************************************************************************** 	
# Step 4 : Build	subsystems.
#************************************************************************************************** 
	if(!$param_nostub)
		{	
		print "\n *** Building unittest stubs  ***\n ";
		run_system_command("abld reallyclean winscw udeb");
		run_system_command("abld build winscw udeb");
		print "\n *** End of building stubs ***\n ";	
	}
	if (!defined $param_nobuild)
		{
		print "\n *** Building test components ***\n";
		if($choice==1)
			{			
			$ENV{'CTC_DATA_PATH'}= $currentdir;	
			build_user_selected("$ctcCommandPart1  $coverageSymbols $ctcIgnoredPart1 $ctcIgnoredPart2 $ctcCommandPart2");		
			}
  	elsif($choice==2)
  		{
			build_user_selected("abld test build winscw udeb");
			}
		print "\n *** End of Building test components ***\n ";
		}	
	
#************************************************************************************************** 		
# Step 5 : Run EunitRunner.exe .
#************************************************************************************************** 	
	print "\n *** Running EUnit with selected subsystems ***\n";	
	chomp(@dll_filenames);
	if( scalar @dll_filenames eq 0 )
		{
		print "Can not find dll names automatically.Using default configured values\n";
		@dll_filenames = @default_dll_filenames;
		}
	copy_dlls(); # copy dlls to eunit directory
	if($choice==1)
		{  
		run_system_command("$param_exe "."$eunitParams");# Run without alloc failure  		   
		#run_system_command("$param_exe "."$eunitParams"."$eunitParams2");# Run with alloc failure   		   
		}
	elsif ($choice==2)
		{		
		run_system_command("$param_exe "."$eunitParams");  		   
		}

#************************************************************************************************** 		 
# Step 6 : Print Results.
#************************************************************************************************** 		
	print "\n *** Printing Reports ***\n";	
	if($choice==1)
		{	
		run_system_command("ctcpost $coverageSymbols $coverageDat -p $coverageProfile -T 70");		
		run_system_command("ctc2html -i $coverageProfile -t 70 -nsb");	
		run_system_command("xcopy  /R/F/Y/I CTCHTML $results_destinationdir\\CTCHTML");	
		print_overall_summary_ctc();		
		}
	elsif($choice==2)
		{	
		print_overall_summary_error();# Mainly needed for nightly build env and for eunit.		
		print_eunit_html_report();			
		print_overall_summary_eunit();		
		run_system_command("copy $eunit_xml_file $results_destinationdir\\$eunit_xml_file_name");		
		run_system_command("copy $unittest_result_file $results_destinationdir\\$unittest_result_file");		
		}
#This reports other script errors which might have harmed the flow of execution.
print_overall_summary_script_error();

#************************************************************************************************** 		 
# Step 7 : Post Cleanup.
#************************************************************************************************** 		 
	$ENV{'CTC_DATA_PATH'}= "";	
	run_system_command("del "."$eunit_destination_dir"."*.dll");  	
	run_system_command("del "."$eunit_xml_file");  	
	print "\n *** DONE ***\n";

#---------------------------------------Main Ends--------------------------------------------------

#--------------------------------------------------------------------------------------------------
# This sub routine displays all the subsystems to the user.
#--------------------------------------------------------------------------------------------------
sub display_sybsystems 
	{  
  get_subsystems();
  if(scalar @subsystems==0)
  	{
  	print "Can't find subsystem name automatically.Using default configured values\n" ;
  	@subsystems = @default_subsystems;  	
  	}  
  print "\n\tPlease choose an option listed below \n";   
  print "\n\tenter +/- to build backward or forward \n";   
  print "\t\t0.To build all \n";    	
  $count=1;
  foreach(@subsystems)
  	{   	
   	print "\t\t".$count++. "." .$_."\n";   	
   	}
  print "\t\tq.Exit \n";   
  }

#--------------------------------------------------------------------------------------------------
# Runs abld commands for the user selected choice.
#--------------------------------------------------------------------------------------------------
sub build_user_selected 
	{	 	
 	if($user_selection==0)
 		{  
  	for($count=0;$count<@subsystems;$count++)
  		{
  		run_system_command("abld test reallyclean winscw udeb @subsystems[$count]");
  		run_system_command("@_[0] @subsystems[$count]");  		 
  		get_dll_names(@makefiles[$count]);  				  				  				 				  				
  		}  	
  	}  	  		  		 
	elsif($user_selection=~/\+/)
		{ 				 			
  	for($count=substr($user_selection,0,1)-1;$count<@subsystems;$count++)
  		{
  		run_system_command("abld test reallyclean winscw udeb @subsystems[$count]");
  		run_system_command("@_[0] @subsystems[$count]"); 		 
  		get_dll_names(@makefiles[$count]);  				  				  				 				  				
  		}
  	}
  elsif($user_selection=~/\-/)
  	{  		  
  	 for($count=0;$count<substr($user_selection,0,1);$count++)
  	 	{  				
  		run_system_command("abld test reallyclean winscw udeb @subsystems[$count]");
  		run_system_command("@_[0] @subsystems[$count]");  		  				
  		get_dll_names(@makefiles[$count]);  				  				  				
  		}
  	}  	 
   else
   	{
    $count=0;    		
  	while($count<length $user_selection)
  		{
  	  my $str = substr ($user_selection,$count,1); 
  		run_system_command("abld test reallyclean winscw udeb @subsystems[$str-1]");
  		run_system_command("@_[0] @subsystems[$str-1]");  		
  		get_dll_names(@makefiles[$str-1]);  				  				  				  				
  		$count++;  		  				
  		}
  	}  	  		  		  	
	}
	
#--------------------------------------------------------------------------------------------------
# Displays critical error which is used for debugging purpose.
#--------------------------------------------------------------------------------------------------
sub display_critical_error
	{  
  print "\n ->ERROR OCCURED : "."$?"."\n"  if(!@_[0]);  
  print "\n ->ERROR OCCURED : ".@_[0]."\n" if(@_[0]);  	
  #exit 1;
  }
  
#--------------------------------------------------------------------------------------------------
# Calls system command and displays error in case.
#--------------------------------------------------------------------------------------------------
sub run_system_command
	{  
	print " \n -> Command : " .@_[0] ."\n";
	system(@_[0]);	
	push @gloabl_errors,"Error Occured for Command : @_[0] Error : $?" if($?!=0);
	display_critical_error() if($?!=0);  
	return $?;
	}
	
#--------------------------------------------------------------------------------------------------
# Prints Usage.
#--------------------------------------------------------------------------------------------------
sub usage	
	{
print<<ENDOFUSAGE;
 Usage :
      eunitrunner -command=ct|eu|mm -dest=resultdir	
        -ctc -> To run CTC.
        -eu -> To run EunitTestcases.
        -mm -> To run Manual Mode.Useful if you want to find the coverage inside your code.
        -resultdir->In where results will be copied.	
		    		
     Also you can specify some optional flags to change the behaviour of the script.
       -all -> To run all subsystems.
       -sourcefiles -> To Select Source files for instrumentation
       -noclean -> Don't run reallyclean
       -nobuild -> Dont run symbian build commands
       -nostub	 -> Dont build stubs
       -nosdkinst-> Dont instrument sdk
		
     Some miscellaneous Commands
       -version -> Version Information.
       -help -> Usage information.
	
  Examples :
       eunitrunner -command=ctc
       eunitrunner -command=eu C:/temp/results
       eunitrunner -command=eu C:/temp/results -all
       eunitrunner -command=ctc C:/temp/results\ -sourcefiles -nobuild -noclean -nostub
       
  Note:This script should be run from where bld.inf file available
  Note:Also you must specify TEMP environmental variable.(By default this exists in All windows system)
ENDOFUSAGE
	exit 0;
	}
	
#--------------------------------------------------------------------------------------------------
# Runs abld to make makefile and parse makefile to get subsystems.
#--------------------------------------------------------------------------------------------------
sub	get_subsystems()
	{	
	#Testing prupose to check how abld test makefile command runs.
	run_system_command("abld test makefile winscw -w");
	
	open WHATCMD, "abld test makefile winscw -w |";
	my @temparray;
	while(<WHATCMD>)
		{
		chomp;
		my $makeFile = $_;			
		if( $makeFile =~ /winscw$/i )
			{
			@temparray=split(/\.WINSCW/,$makeFile);			
			@temparray=split(/\\/,@temparray[0]);			
			push @subsystems,pop(@temparray);					
			push @makefiles,$makeFile;
			}
		}
	print "\n Makefiles = @makefiles\n";
	print "\n Subsystems = @subsystems\n";
	close WHATCMD;	
	}
	
#--------------------------------------------------------------------------------------------------
# Parses makefile to get source files. Useful if you want to run coverage for particular source file.
#--------------------------------------------------------------------------------------------------
sub get_source_files($)
	{
	open MAKE, @_[0] or die "Cant open $!";	
	my @temparray;
	while (<MAKE>)
		{
		my($line)=$_;						
		if($line=~/(EPOCBLDUREL).*.\.o.*.:.*.\.cpp/)
			{
			if($line!~/INTERNAL|internal/)
				{		
				@temparray=split(/:/,$line);
				@temparray=split(/\\/,@temparray[1]);					
				push @sourcefiles,pop @temparray;						
				}
			}			
		}
	close MAKE;	
	}
	
#--------------------------------------------------------------------------------------------------
# Parses makefile to get dll names.
#--------------------------------------------------------------------------------------------------
 sub get_dll_names($)
 	{ 	
	open MAKE, @_[0] or die "Cant open $_ $!";		
	my @temparray;
	my $dll_name;
	while (<MAKE>)
		{
		my($line)=$_;									
		chomp($line);
		if($line=~/Target.*.\.dll/)
			{				
			@temparray=split(/\s+/,$line);				
			$dll_name=pop @temparray;								
			chomp($dll_name);										
			push @dll_filenames,$dll_name;
			last;
			} 
		}
	close MAKE;	
	}
	
#--------------------------------------------------------------------------------------------------
# Parses eunit result xml file and prints it as html file.
#--------------------------------------------------------------------------------------------------
sub print_eunit_html_report	
	{
	my $simple = XML::Simple->new();	
	my $struct = eval {$simple->XMLin($eunit_xml_file, forcearray => 1,keyattr => [])};	
	push @gloabl_errors,"Invalid XML document Error : $@" if($@);			
	open(HTML_FILE, ">$unittest_result_file") ||(push @gloabl_errors,"Can not open $unittest_result_file for Eunit" and return);		
	print HTML_FILE "<TABLE BORDER CELLSPACING=\"2\" CELLPADDING=\"2\" WIDTH=\"80%\">\n";		
	print HTML_FILE "<CAPTION><H3><P ALIGN=Left>Overall Result:          Passed</H3></CAPTION>\n";										
	print HTML_FILE "<tr>\n";	
	print HTML_FILE "<th BGCOLOR=\"#CCCCCC\">Total TestCases</th>\n";
	print HTML_FILE "<th BGCOLOR=\"#00FF00\">Total Passed</th>\n";
	print HTML_FILE "<th BGCOLOR=\"#FF0000\">Total Failed</th>\n";
	print HTML_FILE "</tr>\n";	
	print HTML_FILE "<tr>\n";							
	print HTML_FILE "<td><P ALIGN=Center>      </td>\n";
	print HTML_FILE "<td><P ALIGN=Center>      </td>\n";
	print HTML_FILE "<td><P ALIGN=Center>      </td>\n";
	print HTML_FILE "</tr>\n";	
	print HTML_FILE "</table>\n";
	print HTML_FILE "                                                     <BR>";
	
	foreach my $dll(@{$struct->{dll}})
		{
	  my $dll_pass_count = 0;
		foreach my $mtestsuite(@{$dll->{testsuite}})
			{									
			foreach my $testsuite(@{$mtestsuite->{testsuite}})
				{				
				my($testcount)=1;						
				print HTML_FILE "<TABLE BORDER CELLSPACING=\"2\" CELLPADDING=\"2\" WIDTH=\"80%\">\n";
				print HTML_FILE "<CAPTION><H3><P ALIGN=Left>$testsuite->{name}</H3></CAPTION>\n";						
				print HTML_FILE "<tr>\n";	
				print HTML_FILE "<th>No</th>\n";
				print HTML_FILE "<th>Name</th>\n";
				print HTML_FILE "<th>Class</th>\n";
				print HTML_FILE "<th>Method</th>\n";
				print HTML_FILE "<th>Type</th>\n";
				print HTML_FILE "<th>Result</th>\n";
				print HTML_FILE "</tr>\n";					
				foreach my $testcase(@{$testsuite->{testcase}})
					{											
					print HTML_FILE "<tr>\n";							
					print HTML_FILE "<td BGCOLOR=\"#CCCCCC\"><P ALIGN=Left>$testcount</td>\n";
					print HTML_FILE "<td BGCOLOR=\"#CCCCCC\"><P ALIGN=Left>$testcase->{name}</td>\n";
					print HTML_FILE "<td BGCOLOR=\"#CCCCCC\"><P ALIGN=left>$testcase->{class}</td>\n";
					print HTML_FILE "<td BGCOLOR=\"#CCCCCC\"><P ALIGN=Left>$testcase->{method}</td>\n";
					print HTML_FILE "<td BGCOLOR=\"#CCCCCC\"><P ALIGN=Left>$testcase->{type}</td>\n";
					if($testcase->{result}->[0]->{status} eq "OK")
						{
						print HTML_FILE "<TD BGCOLOR=\"#00FF00\"><P ALIGN=Left>PASS</TD>\n";		
						$eunit_total_pass_count++;										
						$dll_pass_count++ ;
						}
					else
						{
						print HTML_FILE "<TD BGCOLOR=\"#FF0000\"><P ALIGN=Left>FAIL</TD>\n";
						$eunit_total_fail_count++;						
						}
					print HTML_FILE "</tr>\n";
					$testcount++;						
					}
				print HTML_FILE "</table>\n";
				print HTML_FILE "<BR>\n";				
				}			
			push @eunit_testsuite_name,$mtestsuite->{name};			
			push @eunit_testsuite_total_testcase,$mtestsuite->{size};
			push @eunit_testsuite_pass_count,$dll_pass_count;
			}		
		}	
	print HTML_FILE "</body><br></html>";	
	$eunit_total_count=$eunit_total_fail_count+$eunit_total_pass_count;
	seek HTML_FILE,301,0;
	print HTML_FILE "$eunit_total_count";
	seek HTML_FILE,334,0;
	print HTML_FILE "$eunit_total_pass_count";
	seek HTML_FILE,367,0;
	print HTML_FILE "$eunit_total_fail_count";
	if($eunit_total_pass_count>0 && $eunit_total_count>0)
		{
		$eunit_total_pass_percent=sprintf "%.2f", (($eunit_total_pass_count/$eunit_total_count)*100);	
		}
	else
		{
		$eunit_total_pass_percent=sprintf "%.2f","0.00";	
		}
	seek HTML_FILE,103,0;
	print HTML_FILE "$eunit_total_pass_percent%";		
	close (HTML_FILE);                   
	}
 
 #--------------------------------------------------------------------------------------------------
 # Prints overall summary in file specified in command line argument.
 #--------------------------------------------------------------------------------------------------
 sub print_overall_summary_eunit 
 	{
	open OVER_ALL_SUMMARY, ">>$overal_summary_file" or (push @gloabl_errors,"Can not open $overal_summary_file for Eunit" and return); 			
	print OVER_ALL_SUMMARY "<h1>Eunit Summary</h1>\n ";		
	print OVER_ALL_SUMMARY "<table cellspacing=\"0\"> \n ";
	print OVER_ALL_SUMMARY "<tr><td class=\"black\"><strong>PassRate&nbsp;%</strong>&nbsp;</td> \n ";
	print OVER_ALL_SUMMARY "<td class=\"black\"><strong>-</strong></td><td class=\"black\">&nbsp;<strong>Pass/&nbsp;all</strong>&nbsp;</td> \n ";
	print OVER_ALL_SUMMARY "<td width=\"115\">&nbsp;</td><td class=\"black\"><strong>Subsystem</strong></td></tr> \n ";
	print OVER_ALL_SUMMARY "<tr><td colspan=\"5\"><hr width=\"100%\"></td></tr> \n ";
	my($count)= 0;
	foreach $_(@eunit_testsuite_name)
		{
		my $dll_pass_percent = 0;
		my $dll_fail_percent = 0;
		my $dll_name = @eunit_testsuite_name[$count];
		my $dll_total = @eunit_testsuite_total_testcase[$count];
		my $dll_pass = @eunit_testsuite_pass_count[$count];
		if($dll_total>0 && $dll_pass>0)
			{
			$dll_pass_percent=sprintf "%.2f", (($dll_pass/$dll_total)*100);	
			}
		else
			{
			$dll_pass_percent=sprintf "%.2f","0.00 \n ";	
			}		
		$dll_fail_percent = 100 - $dll_pass_percent;
		print OVER_ALL_SUMMARY "<tr> \n ";
		print OVER_ALL_SUMMARY "<td align=\"right\" class=\"blue\">$dll_pass_percent&nbsp;%&nbsp;</td><td align=\"right\" class=\"blue\">&nbsp;</td> \n ";
		print OVER_ALL_SUMMARY "<td align=\"right\" class=\"blue\">&nbsp;$dll_pass/$dll_total&nbsp;</td> \n ";
		print OVER_ALL_SUMMARY "<td width=\"115\"><img border=\"1\" src=\"blue.gif\" width=\"$dll_pass_percent\" height=\"5\" alt=\"\"><img border=\"1\" src=\"red.gif\" width=\"$dll_fail_percent\" height=\"5\" alt=\"\"></td> \n ";
		print OVER_ALL_SUMMARY "<td><a class=\"blue\">$dll_name</a></td> \n ";
		print OVER_ALL_SUMMARY "</tr> \n ";
		$count ++;
		}	
	print OVER_ALL_SUMMARY "<tr><td colspan=\"5\"><hr width=\"100%\"></td></tr> \n ";
	print OVER_ALL_SUMMARY "<tr><td align=\"right\" class=\"black\"><strong>$eunit_total_pass_percent&nbsp;%</strong>&nbsp;</td> \n ";
	print OVER_ALL_SUMMARY "<td align=\"right\" class=\"black\"><strong>&nbsp;</strong></td> \n ";
	print OVER_ALL_SUMMARY "<td align=\"right\" class=\"black\"><strong>&nbsp;$eunit_total_pass_count/$eunit_total_count</strong>&nbsp;</td> \n ";
	my $total_eunit_fail_percent = 100 - $eunit_total_pass_percent;
	print OVER_ALL_SUMMARY "<td width=\"115\"><img border=\"1\" src=\"blue.gif\" width=\"$eunit_total_pass_percent\" height=\"5\" alt=\"\"><img border=\"1\" src=\"red.gif\" width=\"$total_eunit_fail_percent\" height=\"5\" alt=\"\"></td> \n ";
	print OVER_ALL_SUMMARY "<td class=\"black\"><strong>OVERALL</strong></td></tr> \n ";
	print OVER_ALL_SUMMARY "</table><br> \n ";	
	print OVER_ALL_SUMMARY "<a href=\"$results_destinationdir\\$unittest_result_file\">View compleate results : $results_destinationdir\\$unittest_result_file</a> \n ";	
	close OVER_ALL_SUMMARY;
  }

#--------------------------------------------------------------------------------------------------
#Prints overall summary of CTC in file specified in command line argument.
#--------------------------------------------------------------------------------------------------
sub print_overall_summary_ctc
 	{
 	
	open OVER_ALL_SUMMARY, ">>$overal_summary_file" or (push @gloabl_errors,"Can not open $overal_summary_file for CTC \n " and return );
  open CTC_RESULT,"$ctc_result_file";
 	@lines = <CTC_RESULT>;
 	print OVER_ALL_SUMMARY "<h1>CTC Summary</h1>\n ";	 	
 	print OVER_ALL_SUMMARY "<table cellspacing=\"0\"> \n";
  print OVER_ALL_SUMMARY "<tr><td class=\"black\"><strong>TER&nbsp;%</strong>&nbsp;</td>\n";
  print OVER_ALL_SUMMARY "<td class=\"black\"><strong>-</strong></td><td class=\"black\">&nbsp;<strong>covered/&nbsp;all</strong>&nbsp;</td>\n";
	print OVER_ALL_SUMMARY "<td width=\"115\">&nbsp;</td><td class=\"black\"><strong>Directory</strong></td></tr>\n";	
 	foreach $_(@lines) 
 		{
 		if ($_=~/hr width=/)
 			{
 			print OVER_ALL_SUMMARY $_;
 			} 		
 		if ($_=~/img border=/)
 			{ 		 				
 			print OVER_ALL_SUMMARY $_;
 			}
 		}  
 	print OVER_ALL_SUMMARY "</table><br>\n";
 	print OVER_ALL_SUMMARY "<a href=\"$results_destinationdir\\CTCHTML\\index.html\">View compleate results : $results_destinationdir\\CTCHTML\\index.html</a> \n ";	
 	close CTC_RESULT;
 	close OVER_ALL_SUMMARY;
}

#--------------------------------------------------------------------------------------------------
#Check uncompiled subsystems by checking the dll names.
#--------------------------------------------------------------------------------------------------
sub copy_dlls
 	{ 	
 	my $error = 0;
 foreach my $dll(@dll_filenames) 
 		{
 		$error = run_system_command("copy ".$build_destination_dir.$dll." ".$eunit_destination_dir.$dll);  
 		push @uncompiled_sub_systems,$dll if($error!=0)
 	  } 			
	}

#--------------------------------------------------------------------------------------------------
# Prints overall error summary if any compilation errors happen.
#--------------------------------------------------------------------------------------------------
 sub print_overall_summary_error 
 	{ 	
 	open OVER_ALL_SUMMARY, ">>$overal_summary_file" or (push @gloabl_errors,"Can not open $overal_summary_file for Eunit" and return) ;
 	if (scalar @uncompiled_sub_systems != 0)
 		{		
		print OVER_ALL_SUMMARY "<h1>Compilation Errors</h1>\n ";		
		my $count = 1;
		print OVER_ALL_SUMMARY "<h3>Following dll's were not found. Possibly because of build errors! \n</h3>";
		foreach $_(@uncompiled_sub_systems)
			{		
			print OVER_ALL_SUMMARY "<FONT COLOR=\"#FF0000\"><h4>$count. $_</h4></FONT>\n";
			$count++;		
			}	
		print OVER_ALL_SUMMARY "<a href=\"$results_destinationdir_log\\$unittest_log_file\">View Error details : $results_destinationdir_log\\$unittest_log_file</a> \n ";	
	 }	
	close OVER_ALL_SUMMARY;
  }
  
 #--------------------------------------------------------------------------------------------------
# Prints overall error summary if any compilation errors happen.
#--------------------------------------------------------------------------------------------------
 sub print_overall_summary_script_error 
 	{ 	
 	open OVER_ALL_SUMMARY, ">>$overal_summary_file" or return ;
 	if(scalar @gloabl_errors)
		{			
		print OVER_ALL_SUMMARY "<h1>Execution Errors</h1>\n ";	
		my $count = 1;
		foreach $_(@gloabl_errors)
			{
			print OVER_ALL_SUMMARY "<h3>$count.$_</h3>\n";
			$count++;		
			}
		print OVER_ALL_SUMMARY "<a href=\"$results_destinationdir_log\\$unittest_log_file\">View Error details : $results_destinationdir_log\\$unittest_log_file</a> \n ";	
		}
	close OVER_ALL_SUMMARY;
  }
  
# End of file.
