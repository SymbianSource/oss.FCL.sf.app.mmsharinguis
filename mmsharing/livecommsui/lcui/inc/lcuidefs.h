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
#ifndef LCUIDEFS_H_
#define LCUIDEFS_H_

#include <QObject>

class LcUtil {

public:
    
    inline static void asyncSlotCall( QObject* instance, const char* slot )
        {
        QMetaObject::invokeMethod(instance, slot, Qt::QueuedConnection );
        }
    
    inline static void asyncSlotCall( QObject* instance, const char* slot, QGenericArgument arg )
        {
        QMetaObject::invokeMethod(instance, slot, Qt::QueuedConnection, arg );
        }
};

// Panic the current thread
#if ( defined __WINSCW__ ) || ( defined __WINS__ ) || ( defined __ARMV5__ )
#include <e32std.h>
#define LC_PANIC() User::Invariant()
#else
#define LC_PANIC()
#endif

#define LC_MCC_WINDOW_FACTOR 50

#define LC_NOTE_ON_TOP 20

#define LC_NOTE_TIMEOUT 3000

#define DESC_TO_QSTRING( desc )\
    QString::fromRawData( reinterpret_cast<const QChar*>(desc.Ptr()), desc.Length());

#define LC_INVITING "Inviting..."
#define LC_WAITING_VIDEO "Waiting for shared video..."
#define LC_OK "OK"
#define LC_CANCEL "Cancel"
#define LC_ACCEPT_SESSION "Accept incoming session from"
#define LC_RECIPIENT_QUERY "Enter recipient address:"

#ifndef QT_TESTLIB_LIB    
const char lcIdleViewFile[] = ":/xml/lcidleview.docml";
const char lcReceiveViewFile[] = ":/xml/lcreceiveview.docml";
const char lcReceiveOnlyViewFile[] = ":/xml/lcreceiveonlyview.docml";
const char lcSendViewFile[] = ":/xml/lcsendview.docml";
const char lcTwowayViewFile[] = ":/xml/lctwowayview.docml";
const char lcAllInOneViewFile[] = ":/xml/lcvtview.docml";
#else
const char lcIdleViewFile[] = "c:/lcidleview.docml";
const char lcReceiveViewFile[] = "C:/lcreceiveview.docml";
const char lcReceiveOnlyViewFile[] = "C:/lcreceiveonlyview.docml";
const char lcSendViewFile[] = "C:/lcsendview.docml";
const char lcTwowayViewFile[] = "C:/lctwowayview.docml";
const char lcAllInOneViewFile[] = "C:/lcvtview.docml";

#endif

const char lcLcViewTypeName[] = "LcView";

//view ids
const char lcViewIdleId[] = "lc_view_idle";
const char lcViewReceiveId[] = "lc_view_recv";
const char lcViewReceiveOnlyId[] = "lc_view_recvonly";
const char lcViewSendId[] = "lc_view_send";
const char lcViewTwowayId[] = "lc_view_twoway";
const char lcViewAllInOneId[] = "lc_view_allinone";

//widget ids
const char lcIconContactId[] = "lc_icon_contact";
const char lcIconBrandId[] = "lc_icon_brand";
const char lcLabelRecipientId[] = "lc_label_recipient";
const char lcLabelDurationId[] = "lc_label_duration";
const char lcWidgetRecvVideoId[] = "lc_widget_recvvideo";
const char lcWidgetSendVideoId[] = "lc_widget_sendvideo";
const char lcWidgetRecvVideoId2[] = "lc_widget_recvvideo2";
const char lcWidgetSendVideoId2[] = "lc_widget_sendvideo2";
const char lcButtonEndCall[] = "lc_button_endCall";
const char lcWidgetDialpad[] = "lc_label_dialpad";

//action ids
const char lcActFullscreenId[] = "lc_act_fullscreen";
const char lcActVideopreferenceId[] = "lc_act_videopreference";
const char lcActDialerId[] = "lc_act_dialer";
const char lcActEndactivecallId[] = "lc_act_endactivecall";
const char lcActStopSharingId[] = "lc_act_stopsharing";
const char lcActEnableCameraId[] = "lc_act_enablecamera";
const char lcActMuteId[] = "lc_act_mute";
const char lcActGalleryId[] = "lc_act_gallery";
const char lcActChangeCameraId[] = "lc_act_changecamera";
const char lcActMenuChangeCameraId[] = "lc_act_menu_changecamera";
const char lcActAdjustVideoId[] = "lc_act_adjustvideo";
const char lcActShareImageId[] = "lc_act_shareimage";
const char lcActSwapViewsId[] = "lc_act_swapviews";
const char lcActZoomId[] = "lc_act_zoom";
const char lcActSwitchToVoiceCallId[] = "lc_act_switchtovoicecall";
const char lcActDisableCameraId[] = "lc_act_disablecamera";
const char lcActMenuDisableCameraId[] = "lc_act_menu_disablecamera";
const char lcActSpeakerId[] = "lc_act_speaker";
const char lcActOpenKeypadId[] = "lc_act_openkeypad";


//layouts ids
const char lcLayoutPortraitDefaultId[] = "portrait";   //Main window - received content
const char lcLayoutLandscapeDefaultId[] = "landscape"; //Main window - received content
const char lcLayoutFullscreenId[] = "fullscreen";
const char lcLayoutPortraitSwappedId[] = "lc_layout_portrait_swapped";  //Main window - sent content
const char lcLayoutLandscapeSwappedId[] = "lc_layout_landscape_swapped";//Main window - sent content
const char lcLayoutLandscapeDialpadId[] = "lc_layout_landscape_dialpad"; // Dialpad layout


//translations Id
const char lcTranslationsDirectory[] = ":/translations";
const char lcTranslationsFileNameDefault[] = "livecommsui_en_GB";
const char lcTranslationsFileName[] = "livecommsui_";

//icon names
const char lcIconNameUnmuteMic[] = "qtg_mono_mic_unmute";
const char lcIconNameMuteMic[] = "qtg_mono_mic_mute";
const char lcIconNameHandset[] = "qtg_mono_mobile";
const char lcIconNameLoudspeaker[] = "qtg_mono_speaker";
const char lcIconNameMainCamera[] = "qtg_mono_change_cam_mode";
const char lcIconNameSecondaryCamera[] = "qtg_mono_change_cam_mode";
const char lcIconNameEnableCamera[] = "qtg_mono_camcorder";
const char lcIconNameDisableCamera[] = "qtg_mono_camcorder_off";

const char lcIconNameBlackWin[] = ":/black_win.bmp";


//effects
const char lcEffectWinAppear[] = ":/win_appear.fxml";
const char lcEffectWinDisappear[] = ":/win_disappear.fxml";
const char lcEffectWinFlip[] = ":/win_flip.fxml";
const char lcEffectWinSwapDown[] = ":/win_swap_down.fxml";
const char lcEffectWinSwapUp[] = ":/win_swap_up.fxml";

const char lcEffectWinId[] = "vidwindow";
const char lcEffectWinAppearId[] = "appear";
const char lcEffectWinDisappearId[] = "disappear";
const char lcEffectWinFlipId[] = "flip";
const char lcEffectWinSwapDownId[] = "swapDown";
const char lcEffectWinSwapUpId[] = "swapUp";

const char lcEffectWinDefaultCallback[] = "windowDisappearComplete";


#endif /* LCUIDEFS_H_ */
