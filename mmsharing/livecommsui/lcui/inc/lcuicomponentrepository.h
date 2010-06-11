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
#ifndef LCUICOMPONENTREPOSITORY_H
#define LCUICOMPONENTREPOSITORY_H

#include <hbdocumentloader.h>
#include <QString>
#include <QHash>
#include <hbtransparentwindow.h>

class LcView;
class QAction;
class HbProgressDialog;
class HbDialog;
class LcUiEngine;
class HbZoomSliderPopup;
class HbMenu;
class HbMessageBox;

class LcUiComponentRepository : public HbDocumentLoader   
{   
    friend class UT_LcUiComponentRepository;
    
public:    

    explicit LcUiComponentRepository(LcUiEngine& engine);
    ~LcUiComponentRepository();

public:
    
    LcView* idleView();
    LcView* receiveView();
    LcView* receiveOnlyView();
    LcView* twowayView();
    LcView* sendView();
    LcView* allInOneView();
    
    HbDialog* acceptQuery();
    HbProgressDialog* invitingNote();
    HbProgressDialog* waitingNote();
    HbDialog* recipientQuery();
    HbDialog* shareOwnVideoQuery();
    HbZoomSliderPopup* zoomSlider();
    void sharedVideoContextMenuActions( HbMenu* menu, LcView& view );
    bool loadLayout( const QString& layoutName );
    // returns the currently loaded layout name
    QString layout()const;
    // returns the previously loaded layout name
    QString previousLayout()const;
    
private: //from HbDocumentLoader

    QObject *createObject(const QString& type, const QString &name);

private:

    LcView* loadView( QObjectList& components, 
                      const QString& viewId, 
                      const QString& xmlFile );
    void connectActions( LcView& view ) const;
    bool connect( QAction& hbAction, LcView& view ) const;

private:
    
    QString mLastLoadedView;
    
    LcUiEngine& mEngine;
    
    QObjectList mIdleViewComponents;
    QObjectList mReceiveViewComponents;
    QObjectList mSendViewComponents;
    QObjectList mTwowayViewComponents;
    QObjectList mAllInOneViewComponents;
    QObjectList mReceiveOnlyViewComponents;
    
    QHash<QString,const char*>* mSlots;
    
    LcView* mIdleView;
    LcView* mReceiveView;
    LcView* mSendView;
    LcView* mTwowayView;
    LcView* mAllInOneView;
    LcView* mReceiveOnlyView;
    
    HbDialog* mAcceptQuery;
    HbProgressDialog* mInvitingNote;
    HbProgressDialog* mWaitingNote;
    HbDialog* mRecipientQuery;
    HbZoomSliderPopup* mZoomSlider;
    HbMessageBox* mShareOwnVideoQuery;
    
    QString mLayout;
    QString mPreviousLayout;
};

#endif // LCUICOMPONENTREPOSITORY_H
