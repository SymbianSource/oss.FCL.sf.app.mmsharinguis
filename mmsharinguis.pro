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
    :BLD_INF_RULES.prj_exports += "$${LITERAL_HASH}include <platform_paths.hrh>"
    :BLD_INF_RULES.prj_exports += "rom/multimediasharing.iby CORE_APP_LAYER_IBY_EXPORT_PATH(multimediasharing.iby)"
    :BLD_INF_RULES.prj_exports += "rom/multimediasharingresources.iby LANGUAGE_APP_LAYER_IBY_EXPORT_PATH(multimediasharingresources.iby)"
    :BLD_INF_RULES.prj_exports += "rom/multimediasharing_stub.SIS /epoc32/data/z/system/install/multimediasharing_stub.SIS"
    
    :BLD_INF_RULES.prj_mmpfiles += $$LITERAL_HASH"include \"mmsharinguis_plat/group/bld.inf\""
    :BLD_INF_RULES.prj_mmpfiles += $$LITERAL_HASH"include \"mmshplugins/group/bld.inf\""   
    :BLD_INF_RULES.prj_mmpfiles += $$LITERAL_HASH"include \"mmsharing/mmshapp/help/group/bld.inf\""
}

SUBDIRS += mmsharing/group
