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
#

TEMPLATE = subdirs
CONFIG += ordered

symbian: {
    :BLD_INF_RULES.prj_exports += "../mmshshared/loc/mus.loc APP_LAYER_LOC_EXPORT_PATH(mus.loc)"
    :BLD_INF_RULES.prj_exports += "../mmshshared/loc/musoperator.loc APP_LAYER_LOC_EXPORT_PATH(musoperator.loc)"
    :BLD_INF_RULES.prj_exports += "../Conf/multimediasharing.confml APP_LAYER_CONFML(multimediasharing.confml)"
    :BLD_INF_RULES.prj_exports += "../Conf/multimediasharing_1028238B.crml APP_LAYER_CRML(multimediasharing_1028238B.crml)"
    :BLD_INF_RULES.prj_exports += "../cenrep/backup_registration.xml  /epoc32/data/z/private/1028238D/backup_registration.xml"

    :BLD_INF_RULES.prj_mmpfiles += $$LITERAL_HASH"include \"../mmshavailability/group/bld.inf\""
#    :BLD_INF_RULES.prj_mmpfiles += $$LITERAL_HASH"include \"../mmshindicator/group/bld.inf\"" - enable after removing AVKON Notifier API dependencies
    :BLD_INF_RULES.prj_mmpfiles += $$LITERAL_HASH"include \"../mmshmanagercli/group/bld.inf\""
#    :BLD_INF_RULES.prj_mmpfiles += $$LITERAL_HASH"include \"../mmshmanagersrv/group/bld.inf\"" - enable after removing AVKON Notifier API dependencies
    :BLD_INF_RULES.prj_mmpfiles += $$LITERAL_HASH"include \"../mmshengine/group/bld.inf\""
}

SUBDIRS += ../livecommsui
SUBDIRS += ../mmshapp

