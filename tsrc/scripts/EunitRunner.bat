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
# This Script will be used to run EUnit testcases with or without codetest 
# Instrumentation.
################################################################################################################

#	Packages needed
#use strict;					# strict naming rules
use File::Find;			# File operations
use XML::Simple;		# XML parsing
File::Spec::Win32;  # File operartions
use Getopt::Long;		# parameter handling
use Cwd;						# Directory operations
Getopt::Long::Configure( "bundling_override","ignore_case_always" );

#	Global variables

# System defined location of files
my($eunit_destination_dir)= "\\Epoc32\\release\\winscw\\udeb\\z\\sys\\bin\\";
my($build_destination_dir)= "\\Epoc32\\release\\winscw\\udeb\\";
my($build_tools_dir)= "\\Epoc32\\tools\\";
my($eunit_xml_file)= "\\Epoc32\\winscw\\C\\DigiaEUnit\\Logs\\DigiaEUnit_log.xml";

# Default names
my($codetest_workspace)="codetest.idb";
my($unittest_result_file)="eunit_result.html";
my($currentdir)= cwd;
my($results_destinationdir)=cwd;	

# Variables
my (@subsystems, 						# List of user selected sub systems
		@sourcefiles,						# List of user selected source files
		@makefiles,							# List of make files
		@dll_filenames);				# List of dll file names
		
my ($param_help,		 			  # To extract all zip files
		$param_command,		 		  # Command to Run
		$param_dest,						# Copy results to
		$param_all,							# Destination directory
		$param_selectedsource,	# Debug version enables data dumber		
		$param_noclean,					# Debug builds
		$param_nobuild,					# Creates an Environment		
		$param_nostub,					# Dont build stubs
		$param_nosdkinst,			  # Dont Instrument SDK
		$param_exe,			        # Launch the exe specified
		$param_version);				# Version		
		
my($choice,
	$user_selection,
	$count);

#---------------------------------------Main Start----------------------------------------------------------#


# read commandline parameters
my $result = GetOptions('help'			=> \$param_help,						# Display help info
												'command=s' => \$param_command,					# Command to run
												'dest=s' 		=> \$param_dest,					  # Destination directory
												'all'				=> \$param_all,       			# Run all
												'sourcefiles'	=> \$param_selectedsource,# To Select Source files												
 												'noclean'  	=> \$param_noclean,    			# Dont run reallyclean
												'nobuild'		=> \$param_nobuild, 		    # Dont run symbian build commands
												'nostub'		=> \$param_nostub, 				  # Dont build stupbs
												'nosdkinst' => \$param_nosdkinst,			  # Dont instrument SDK. It is already done
												'exe=s'     => \$param_exe,			            # Launch the exe specified.if nothing then epoc will be used
												'version'   => \$param_version);				# Version Information
#Parse user options
usage()if(!defined $param_command);                          # command is manadatory
$choice=1 if $param_command eq ('ct'|'CT');									 # set choice 1 if ct ( codtest)
$param_exe=$build_destination_dir.'EUnitExeRunner' if(!defined $param_exe);

#Checking for EUnit. Cant proceed without Eunit.
if (! -e $build_destination_dir."EUnitExeRunner.exe") {	
  print "*** EUnit is not installed for this SDK. Install EUnit by using Installation Manager from SymSEE ***\n";
  exit 1;  
  }
  
if($param_command eq ('eu'|'EU')){							  					 # set choice 2 if eu ( Eunit)
	unset_codetest_commands();                                 # unset the codetest commands
	$choice=2;
	}
$choice=3 if $param_command eq ('mm'|'MM');					         # set choice 3 if mm ( manual codtest run)
if(defined $param_dest){
	$results_destinationdir=$param_dest ;		
	}
else{
	print "\n Note: No results destination directory specified.Results will be copied to Current Directory\n";
	$results_destinationdir=~s/\//\\/g;
	$results_destinationdir=$results_destinationdir."\\";
	}	

$param_noclean=1 if(defined $param_nobuild);
run_system_command("bldmake bldfiles") if (!defined $param_nobuild);
display_sybsystems();                                       # display list of subsystems

if(defined $param_all){
	print "\nOptions -all specified.Building all components \n ";
	$user_selection=0;
	}
else {	
	print "\t Options selected : ";
	$user_selection = <STDIN>;chomp ($user_selection);
	exit 0 if $user_selection =~ /^[Qq]/;
	}

if(!$param_nostub){
	print "\n\n *** Building stubs first ***\n\n ";
	system("abld reallyclean winscw udeb");
	system("abld build winscw udeb");
	print "\n\n *** End of building stubs ***\n\n ";	
	}

print "\n\n *** Building test components ***\n\n";
build_user_selected();
print "\n\n *** End of Building test components ***\n\n ";

chomp(@dll_filenames);
foreach my $dll(@dll_filenames){   	
		run_system_command("copy ".$build_destination_dir.$dll." ".$eunit_destination_dir.$dll);     	 	
  	}  	

#Move Codetest.idb to temp directory so that codetest.py automation script can use it
if($choice==1){
	open DATFILE , ">$ENV{'TEMP'}/unitrunner.dat";
	my(@drive);
	@drive=split(/:/,$currentdir);
	if($param_exe=~/EUnitExeRunner/)
	  {
	  print DATFILE @drive[0].":".$build_destination_dir.$param_exe."\n";	 	
	  print DATFILE "$param_exe /L xml /E S60AppEnv /T 100 "."@dll_filenames"."\n";
	  }
	else
	  {
	  print DATFILE @drive[0].":".$build_tools_dir.$param_exe."\n";
	  }	
	close DATFILE;  		  	
	run_system_command("move codetest.idb $ENV{'TEMP'}/codetest.idb");	
	print "\n\n *** Running codetest python script ***\n\n";
	run_system_command("$ENV{'AMC_HOME'}/bin/ctmgr --cli codetest.py %1 %2 %3 %4 %5 %6 %7 %8 %9");		
	#Move Log files to user destination directory
	print "\n\n *** Moving Results ***\n\n";
	$results_destinationdir.="\\CodeTest";
	system("mkdir $results_destinationdir");
	system("move $ENV{'TEMP'}\\codetest*.html $results_destinationdir");
	system("move $ENV{'TEMP'}\\codetest*.txt $results_destinationdir");		
	system("move $ENV{'TEMP'}\\codetest*.idb $results_destinationdir");
	}
elsif($choice==2)	{
	#Run Eunit TestReport		
	print "\n\n *** Running EUnit with selected subsystems ***\n\n";
	if($param_exe=~/EUnitExeRunner/)
		  {
		  run_system_command("$param_exe /L xml /E S60AppEnv /T 100 "."@dll_filenames");   	
		  }
		else
		  {		  
		  run_system_command("$param_exe");   
		  print " Press a key to continue \n";
		  <STDIN>;	
		  }
	print "\n\n *** Printing Reports ***\n\n";
	$results_destinationdir.="\\EUnit";
	system("mkdir $results_destinationdir");
	print_html_report();	
	system("copy $eunit_xml_file $results_destinationdir");
	}
elsif($choice==3)	{
	#Run Eunit with instrumentation		
	do {
		print "\n Please start CodetestManager and Target Server \n";
		print " And load swic source codetest.idb\n";
		print " And Press Y/N to Continue or Exit ! ";
		$user_selection = <STDIN>;
		chomp ($user_selection);
		}while($user_selection !~ /^[nN||yY]/);
		exit 0 if $user_selection =~ /^[nN]/;
		print "\n\n *** Running EUnit with instrumented subsystems ***\n\n";
		if($param_exe=~/EUnitExeRunner/)
		  {
		  run_system_command("$param_exe /L xml /E S60AppEnv /T 100 "."@dll_filenames");   	
		  }
		else
		  {
		  run_system_command("$param_exe"); 
		  print " Press a key to continue \n";
		  <STDIN>;	  	
		  }
	}

print "\n\n *** DONE ***\n\n";

#---------------------------------------Main Ends-------------------------------------------------------------#


sub display_sybsystems {  
   get_subsystems();
   display_critical_error("Can't find a subsystem.Please Check your bld.inf file!") if(scalar @subsystems==0);   
   print "\n\tPlease choose an option listed below \n";   
   print "\n\tenter +/- to build backward or forward \n";   
   print "\t\t0.To build all \n";    	
   $count=1;
   foreach(@subsystems){   	
   	print "\t\t".$count++. "." .$_."\n";   	
   	}
   print "\t\tq.Exit \n\n";   
  }

sub build_user_selected {	  	
 	 if($user_selection==0){
  		system("abld test reallyclean winscw udeb") if(!defined $param_noclean or !defined $param_nobuild);
  		run_system_command("abld test build winscw udeb")	if(!defined $param_nobuild);  		
  		foreach my $subsystem (@makefiles){
  			get_dll_names($subsystem);
  			get_source_files($subsystem)if($choice==1);  						
  			}
  		if($choice==1 or $choice==3){
  			print "\n\n *** Setting the CodeTest Options *** \n\n";
  			set_codetest_commands(); 
  			print "\n\n *** Building again with Instrumentation *** \n\n";
  			system("abld test reallyclean winscw udeb"); # better to clean before running codetest
  			run_system_command("abld test build winscw udeb");	
  			print "\n\n *** End of Building with Instrumentation *** \n\n";
  			print "\n\n *** UnSetting the CodeTest Options *** \n\n";
  			unset_codetest_commands(); 			
  			}  	
  		splice(@sourcefiles, 0, scalar @sourcefiles); # delete sourcefiles
  		}  	  		  		 
		elsif($user_selection=~/\+/){ 				 			
  			for($count=substr($user_selection,0,1)-1;$count<@subsystems;$count++){
  				system("abld test reallyclean winscw udeb @subsystems[$count]")if(!defined $param_noclean or !defined $param_nobuild);
  				run_system_command("abld test build winscw udeb @subsystems[$count]")if(!defined $param_nobuild);  		  				
  				get_dll_names(@makefiles[$count]);
  				if($choice==1 or $choice==3){
  					get_source_files(@makefiles[$count]);		
  					print "\n\n *** Setting the CodeTest Options *** \n\n";
  					set_codetest_commands(); 
  					print "\n\n *** Building again with Instrumentation *** \n\n";	
  				 	system("abld test reallyclean winscw udeb @subsystems[$count]");
  					run_system_command("abld test build winscw udeb @subsystems[$count]");
  					print "\n\n *** End of Building with Instrumentation *** \n\n";
  					}
  				print "\n\n *** UnSetting the CodeTest Options *** \n\n" if(!defined $param_nobuild);  ;	
  				unset_codetest_commands() if(!defined $param_nobuild);  
  				splice(@sourcefiles, 0, scalar @sourcefiles); # delete sourcefiles
  				}
  		}
  	elsif($user_selection=~/\-/) {  		  
  		  for($count=0;$count<substr($user_selection,0,1);$count++)	{  				
  				system("abld test reallyclean winscw udeb @subsystems[$count]")if(!defined $param_noclean or !defined $param_nobuild);
  				run_system_command("abld test build winscw udeb @subsystems[$count]")if(!defined $param_nobuild);  		
  				get_dll_names(@makefiles[$count]);
  				if($choice==1 or $choice==3){
  					get_source_files(@makefiles[$count]);	
  					print "\n\n *** Setting the CodeTest Options *** \n\n";		
  				 	set_codetest_commands(); 			
  				 	print "\n\n *** Building again with Instrumentation *** \n\n";			
  				 	system("abld test reallyclean winscw udeb @subsystems[$count]");
  					run_system_command("abld test build winscw udeb @subsystems[$count]");
  					print "\n\n *** End of Building with Instrumentation *** \n\n";
  					}
  				print "\n\n *** UnSetting the CodeTest Options *** \n\n" if(!defined $param_nobuild);
  				unset_codetest_commands() if(!defined $param_nobuild);
  				splice(@sourcefiles, 0, scalar @sourcefiles); # delete sourcefiles
  				}
  		  }  	 
    else	{
    		$count=0;
  			while($count<length $user_selection)	{
  				my $str = substr ($user_selection,$count,1); 
  				system("abld test reallyclean winscw udeb @subsystems[$str-1]")if(!defined $param_noclean or !defined $param_nobuild);
  				run_system_command("abld test build winscw udeb @subsystems[$str-1]")if(!defined $param_nobuild);  		
  				get_dll_names(@makefiles[$str-1]);
  				if($choice==1 or $choice==3){
  					get_source_files(@makefiles[$str-1]);	 
  					print "\n\n *** Setting the CodeTest Options *** \n\n";					
  				 	set_codetest_commands(); 		
  				 	print "\n\n *** Building again with Instrumentation *** \n\n";
  				 	system("abld test reallyclean winscw udeb @subsystems[$str-1]");
  					run_system_command("abld test build winscw udeb @subsystems[$str-1]");
  					print "\n\n *** End of Building with Instrumentation *** \n\n";
  					}
  				unset_codetest_commands(); 		
  				print "\n\n *** UnSetting the CodeTest Options *** \n\n"	;	  				
  				splice(@sourcefiles, 0, scalar @sourcefiles); # delete sourcefiles
  				$count++;  		  				
  				}
  			}  	  		  		  	
	}


sub set_codetest_commands	{	
	if( !defined $param_nosdkinst) {	
	print "\n*** Instrumenting SDK for CodeTest. ***\n";
	print "\tIf you done this already for your SDK use -nosdkinst option.\n\n";
	run_system_command("copy C:\\APPS\\ct\\cttarget\\rtos\\symbian\\Support\\native\\* $build_destination_dir\\");
	run_system_command("copy C:\\APPS\\ct\\cttarget\\rtos\\symbian\\binaries\\SOS_9\\native\\winscw\\* $build_destination_dir\\");	
	run_system_command("instctsymb.cmd "." -v ".(split(/:/,$currentdir))[0].":\ SOS_9");
	$param_nosdkinst = 1;
	}
	
	$ENV{'CODETEST_LIB'}= "NTmvc_TargetLibMD.lib ctsymbiannativemem.lib";	
	chomp(@sourcefiles);
	my $filename = join ",",@sourcefiles;
	$filename =" -CTonly-tag-files=".$filename;	
	my $val="-CTsize_t=unsigned -CTtag-allocator -CTv -CTno-tag-includes -CTtag-level=DC".$filename." -CTidb=".$codetest_workspace;			
	$ENV{'CTDRIVER_ARGS'}= $val;
	print "CTDRIVER_ARGS=".$ENV{'CTDRIVER_ARGS'}."\n";		
	}

sub unset_codetest_commands	{			
	$ENV{'CODETEST_LIB'}= "";	
	$ENV{'CTDRIVER_ARGS'}= "";		
	}

sub display_critical_error {  
  print "\n->Error Occured : ".$?."\n" if(!@_[0]);  
  print "\n->Error Occured : ".@_[0]."\n" if(@_[0]);  	
  exit 1;
  }

sub run_system_command {
	system(@_[0]);
	display_critical_error() if($?!=0);   
	}

sub usage	{
print<<ENDOFUSAGE;
 Usage :
      eunitrunner -command=ct|eu|mm -dest=resultdir	
        -ct -> To run Codetest.
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
       eunitrunner -command=ct
       eunitrunner -command=eu C:/temp/results
       eunitrunner -command=eu C:/temp/results -all
       eunitrunner -command=ct C:/temp/results\ -sourcefiles -nobuild -noclean -nostub
       
  Note:This script should be run from where bld.inf file available
  Note:Also you must specify TEMP environmental variable.(By default this exists in All windows system)
ENDOFUSAGE
	exit 0;
	}

sub	get_subsystems(){	
	open WHATCMD, "abld test makefile winscw -w |";
	my @temparray;
	while(<WHATCMD>){
		chomp;
		my $makeFile = $_;	
		if( $makeFile =~ /winscw$/i )	{
			@temparray=split(/\./,$makeFile);
			@temparray=split(/\\/,@temparray[0]);
			push @subsystems,pop(@temparray);						
			push @makefiles,$makeFile;
			}
		}
	close WHATCMD;	
	}

sub get_source_files($)	{
	open MAKE, @_[0] or die "Cant open $!";	
	my @temparray;
	while (<MAKE>){
			my($line)=$_;						
			if($line=~/(EPOCBLDUREL).*.\.o.*.:.*.\.cpp/){
					if($line!~/INTERNAL|internal/){		
						@temparray=split(/:/,$line);
						@temparray=split(/\\/,@temparray[1]);					
						push @sourcefiles,pop @temparray;						
						}
				}			
		}
	close MAKE;	
	}

 sub get_dll_names($){ 	
	open MAKE, @_[0] or die "Cant open $_ $!";		
	my @temparray;
	my $dll_name;
	while (<MAKE>){
		my($line)=$_;									
		chomp($line);
		if($line=~/Target.*.\.dll/)	{				
				@temparray=split(/\s+/,$line);				
				$dll_name=pop @temparray;								
				chomp($dll_name);										
				push @dll_filenames,$dll_name;
				last;
				} 
		}
	close MAKE;	
	}

sub print_html_report	{
	my $simple = XML::Simple->new();
	my($total)=0;
	my($pass_count)=0;
	my($fail_count)=0;
	my($passPercent)=0.00;
	my $struct = $simple->XMLin($eunit_xml_file, forcearray => 1,keyattr => []);	
	open(HTML_FILE, ">$results_destinationdir/$unittest_result_file") || die "Can't open file: $!\n";	
	# Print initial HTML tags
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
	
	foreach my $dll(@{$struct->{dll}}){
		foreach my $mtestsuite(@{$dll->{testsuite}}){				
			#if($mtestsuite->{name} eq "Wizard generated"){
			#		next;
			#		}
			foreach my $testsuite(@{$mtestsuite->{testsuite}}){				
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
			foreach my $testcase(@{$testsuite->{testcase}}){											
					print HTML_FILE "<tr>\n";							
					print HTML_FILE "<td BGCOLOR=\"#CCCCCC\"><P ALIGN=Left>$testcount</td>\n";
					print HTML_FILE "<td BGCOLOR=\"#CCCCCC\"><P ALIGN=Left>$testcase->{name}</td>\n";
					print HTML_FILE "<td BGCOLOR=\"#CCCCCC\"><P ALIGN=left>$testcase->{class}</td>\n";
					print HTML_FILE "<td BGCOLOR=\"#CCCCCC\"><P ALIGN=Left>$testcase->{method}</td>\n";
					print HTML_FILE "<td BGCOLOR=\"#CCCCCC\"><P ALIGN=Left>$testcase->{type}</td>\n";
					if($testcase->{result}->[0]->{status} eq "OK"){
						print HTML_FILE "<TD BGCOLOR=\"#00FF00\"><P ALIGN=Left>PASS</TD>\n";		
						$pass_count++;				
						}
					else{
						print HTML_FILE "<TD BGCOLOR=\"#FF0000\"><P ALIGN=Left>FAIL</TD>\n";
						$fail_count++;
						}
					print HTML_FILE "</tr>\n";	
					$testcount++;
					}
				print HTML_FILE "</table>\n";
				print HTML_FILE "<BR>\n";
				}
			}
		}
	
	# Print ending HTML tags
	print HTML_FILE "</body><br></html>";	
	$total=$fail_count+$pass_count;
	seek HTML_FILE,301,0;
	print HTML_FILE "$total";
	seek HTML_FILE,334,0;
	print HTML_FILE "$pass_count";
	seek HTML_FILE,367,0;
	print HTML_FILE "$fail_count";
	if($pass_count>0 && $total>0){
		$passPercent=sprintf "%.2f", (($pass_count/$total)*100);	
		}
	else{
		$passPercent=sprintf "%.2f","0.00";	
		}
	seek HTML_FILE,103,0;
	print HTML_FILE "$passPercent%";		
	close (HTML_FILE);                   # Close html
	}
	
__END__

:invoke_perl
@perl -x -S EunitRunner.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
