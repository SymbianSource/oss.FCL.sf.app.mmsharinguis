@rem
@rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of "Eclipse Public License v1.0"
@rem which accompanies this distribution, and is available
@rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
@rem
@rem Initial Contributors:
@rem Nokia Corporation - initial contribution.
@rem
@rem Contributors:
@rem
@rem Description: 
@rem

@goto invoke_perl

#!perl
#line 5

################################################################################################################
# This Script will package livecomms ui compoenents to run in windows environment
################################################################################################################

#	Packages needed
use File::Find;			# File operations
File::Spec::Win32;  # File operartions
use Getopt::Long;		# parameter handling
use Cwd;						# Directory operations
Getopt::Long::Configure( "bundling_override","ignore_case_always" );

#	Global variables
my $orbit_bin_dir = "c:\\hb\\bin";
my $qt_bin_dir = "C:\\Qt\\4.6.0-beta1\\bin";
my $mingw_dll = "C:\\MinGW\\bin\\mingwm10.dll";
my $livecomms_ui_exe = "$qt_bin_dir\\musui.exe";
my $livecomms_ui_dll = "$qt_bin_dir\\livecommsui.dll";
my $package_name = "livecommsui_win";
my $launch_bat = "launch.bat";
my $config_file = "lcconfig.xml";

  
opendir(DIR,$orbit_bin_dir) or die "Can't find dir: $!\n";
closedir(DIR);
opendir(DIR,$qt_bin_dir) or die "Can't find dir: $!\n";
closedir(DIR);
die "$livecomms_ui_exe Doesn't Exist!" unless (-e $livecomms_ui_exe) ;
die "$livecomms_ui_dll Doesn't Exist!" unless (-e $livecomms_ui_dll);
die "$mingw_dll Doesn't Exist!" unless (-e $mingw_dll);
opendir(DIR,$package_name) or system("mkdir $package_name");
closedir(DIR);
system("copy $orbit_bin_dir $package_name\\*");
system("copy $qt_bin_dir $package_name\\*");
system("copy $mingw_dll $package_name\\*");
system("copy $launch_bat $package_name\\*");
system("copy $config_file $package_name\\*");
system("del $package_name\.zip") if( -e "$package_name\.zip" );
system("zip $package_name $package_name\\*");
system("rmdir /s/q $package_name");
	
__END__

:invoke_perl
@perl -x -S C:\SynergyWorkArea\Mercurial\mmsharinguis\mmsharing\mmshlcui\mmshlcuiengine\win32\package.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
