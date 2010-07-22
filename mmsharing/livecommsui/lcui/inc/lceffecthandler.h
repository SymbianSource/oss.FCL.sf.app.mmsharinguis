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
#ifndef LCEFFECTHANDLER_H
#define LCEFFECTHANDLER_H

#include <QObject>
#include <hbeffect.h>

class HbWidget;
class LcUiEngine;

/**
 * 
 */
class LcEffectHandler : public QObject
{
    Q_OBJECT
    friend class UT_LcEffectHandler;
    friend class UT_LcView;
    
public:
    
    enum DissapperEffect{
        NormalDissappear,
        DissappearToFlip
    };

    explicit LcEffectHandler(LcUiEngine& engine,
                             HbWidget* sharedVideo, 
                             HbWidget* sharedVideoOverlay,
                             HbWidget* receivedVideo,
                             HbWidget* receivedVideoOverlay);
    ~LcEffectHandler();

    void setVisibility(HbWidget* widget, bool visible);
    
    void setDissappearEffect( LcEffectHandler::DissapperEffect aEffectType );

    bool isSwapInProgress();
    
    void startEffects();

signals:
    void swapCompleted();
    void loadSwapLayout();
       
public slots:

    void showSendWindow();
    void showReceiveWindow();
    void hideSendWindow();
    void hideReceiveWindow();
    void windowAppearComplete( HbEffect::EffectStatus status );
    void windowDisappearComplete( HbEffect::EffectStatus status );
    void windowDisappearToFlipComplete( HbEffect::EffectStatus status );
    void windowFlipComplete( HbEffect::EffectStatus status );
    void windowSwapSharedComplete( HbEffect::EffectStatus status );
    void windowSwapReceivedComplete( HbEffect::EffectStatus status );
    void swap();
    
private:
    void startEffect( HbWidget* widget, const char *effectId, const char *callBack = "" );
    bool isSharedVideoAtTop() const;
    bool videosSideBySide() const;
    void handleSwapCompletion();
    void initializeSwap();
    
private: //data 
    
    LcUiEngine& mEngine;
    HbWidget* mSharedVideo;
    HbWidget* mSharedVideoOverlay;
    HbWidget* mReceivedVideo;
    HbWidget* mReceivedVideoOverlay;
    
    QString mEffectCallBack;
    bool mSwapSharedCompleted;
    bool mSwapReceivedCompleted;
    bool mSwapInProgress;
};
                  
#endif // LCEFFECTHANDLER_H
