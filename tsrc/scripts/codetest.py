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
# codetest.py
# Start EUnitExeRunner and collect relevent data's.

#Import relevent modules
import time,os,sys,re
from ctest.DataObjM import CovObjDef
from ctest import CTCommonDef
from ctest.DataObjM import TaskObjDef 

try:
	
	#Close Existing Workspace if any
	ctob.closeWorkspace()
	
	#port is hardcored to 3020 which is default for codetest
	port = int('3020')
	
	#Start CT Server
	ctserverDir=os.environ.get("AMC_HOME")+'/bin/ctserver.exe'
	os.spawnv(os.P_NOWAIT,ctserverDir,['ctserver'])
	
	# Create a SWIC/Native Data Source with a 30 second comm timeout.
	ds = ctob.createCtSWIC('localhost', port, 30)
	print ds.getName(), 'data source created...'
	
	# Set data source configuration information.
	ds.setIDB('$TEMP/codetest.idb')
	
	# Connect to the data source.
	ds.connect()
	
	# Put the data source into continuous mode.
	ds.setContinuousMode()
	
	# Start data collection.
	ds.start()
	
	#start EUnitExeRunner
	print 'Starting application...'
	myfile = open(os.environ.get("TEMP")+'\unitrunner.dat', 'r')      
	commandList = myfile.readlines()
	myfile.close()
	parser = re.compile(r'\s')
	options=parser.split(commandList[1])
	eUnitExeRunner = commandList[0].rstrip()
	os.spawnv(os.P_WAIT,eUnitExeRunner,options)
	
	print 'wait 5 seconds for data collection...'
	# Wait for 10 seconds to allow data collection.
	time.sleep(5)
	
	# Collect continuous mode data.
	ds.collectData()
	
	# Stop data collection.
	ds.stop()
	
	# At this point both continuous and trace data should appear in GUI.
	# Save to a datafile
	fileName ='$TEMP/codetest_result.dat'
	retv = ds.saveData(fileName)
	if retv:
	  print '%s: data saved to %s.'%(ds.getName(), fileName)
	else:
	  print 'Error saving datafile!'
	
	ds.shutdown()

except:
	ds.shutdown()
	sys.exit()
	
try :
	# generate result
	print 'Now generating results file......'
	df = ctob.createCtDataFile(fileName)
	print df.dataFileName(), ' data file loaded to workspace...'
	
	#Get data set objects
	coverageObj0 = df.getCoverageObjs()[0]
	print "Got dataset objects"
	
	coverageObj0.sortData('coverage',1)
	coverageObj0.setViewFuncNameLong()
	
	coverageObj0.setFileCoverageMode()
	cov_view_one = '$TEMP/codetest_file_coverage.html'
	coverageObj0.exportView(cov_view_one, CTCommonDef.HTML_FORMAT,' ',' ' )
	print 'Coverage details are saved in '+cov_view_one;
	covReportFile = '$TEMP/codetest_summary.txt'
	coverageObj0.writeCoverageReport(covReportFile, CovObjDef.OVERALL_SUMMARY)
	print 'Overall coverage summary saved in '+covReportFile;
	
	covFunctionReportFile = '$TEMP/codetest_function_coverage.txt'
	coverageObj0.writeCoverageReport(covFunctionReportFile, CovObjDef.FUNCTION_SUMMARY)
	print 'Function coverage summary saved in '+covFunctionReportFile;
	
	covUnCoveredReportFile = '$TEMP/codetest_function_uncovered_statements.txt'
	coverageObj0.writeCoverageReport(covUnCoveredReportFile, CovObjDef.UNCOVERED_ONLY)
	print 'Uncovered coverage summary saved in '+covUnCoveredReportFile;

except:
	print 'Unable to write reports'
	sys.exit()