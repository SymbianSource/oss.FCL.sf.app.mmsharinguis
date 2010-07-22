/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/
#ifndef LCAPPLICATION_H
#define LCAPPLICATION_H

#include <hbapplication.h>
#include <QString.h>

class CMusTestDirector;

class LcHbApplication : public HbApplication
{
    Q_OBJECT
    friend class UT_LcApplication;
    
public:
    
    LcHbApplication(int &argc, char *argv[],QString useCase);
    ~LcHbApplication();
    
signals:
    
    void handleQuit();

public slots:

    //from QCoreApplication
    static void quit();        

private:
	  void setUseCase( QString useCase );

private: //data
    
    static LcHbApplication* self;
    CMusTestDirector* iTestDirector;
};


#endif //LCAPPLICATION_H
