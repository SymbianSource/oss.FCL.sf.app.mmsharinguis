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

#call qmake
#call bldmake bldfiles
#call abld build winscw udeb
call \epoc32\release\winscw\udeb\ut_lcuieng.exe -xml -o c:\ut_lcuieng.xml
call copy R:\epoc32\winscw\c\ut_lcuieng.xml .
call type ut_lcuieng.xml