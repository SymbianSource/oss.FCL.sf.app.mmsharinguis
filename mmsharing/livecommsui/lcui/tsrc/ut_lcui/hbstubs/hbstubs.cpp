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

// Orbit classes
#include <hbinstance.h>
#include <hbmainwindow.h>
#include <hbwidget.h>
#include <hbdialog.h>
#include <hbmenu.h>
#include <hbaction.h>
#include <hblabel.h>
#include <hbicon.h>
#include <hbmessagebox.h>
#include <hbprogressdialog.h>
#include <hbzoomsliderpopup.h>
#include <hbsliderpopup.h>
#include <hbpushbutton.h>
#include <hbabstractbutton.h>
#include <hbaction.h>
#include <hbframedrawer.h>

#include <hbtoolbar.h>
#include <hbview.h>
#include <hbdocumentloader.h>
#include <hbtransparentwindow.h>
#include <hbgesture.h>
#include <hbgesturefilter.h>
#include <hbeffect.h>
#include <hblineedit.h>
#include <QCoreApplication>
#include <QGesture>
#include <hbtapgesture.h>

// Helper Class
#include "hbstub_helper.h"

// LC UI
#include "lcuidefs.h"



// Static data that simulates HbInstance and HbMainWindow
static HbInstance* hbInstanceSingleton = 0;

// Static data for hbeffects
bool HbEffect::mRunning;
    
// No Gesture default state.    
Qt::GestureState testState = Qt::NoGesture;


HbTapGesture::TapStyleHint testStyleHint = HbTapGesture::Tap; 

    
// -----------------------------------------------------------------------------
// QCoreApplication::quit
// -----------------------------------------------------------------------------
// 
void QCoreApplication::quit()
{
}

// -----------------------------------------------------------------------------
// QCoreApplication::exit
// -----------------------------------------------------------------------------
//
void QCoreApplication::exit(int /*retcode*/)
{
}


// HbInstance

// -----------------------------------------------------------------------------
// HbInstance::instance
// -----------------------------------------------------------------------------
//
HbInstance* HbInstance::instance()
{
	if (!hbInstanceSingleton)
		hbInstanceSingleton = new HbInstance;
	return hbInstanceSingleton;
}

// -----------------------------------------------------------------------------
// HbInstance::allMainWindows
// -----------------------------------------------------------------------------
//
QList<HbMainWindow *> HbInstance::allMainWindows() const
{
    return mAllMainWindows;
}


// -----------------------------------------------------------------------------
// HbInstance::HbInstance
// -----------------------------------------------------------------------------
//
HbInstance::HbInstance()
{
    HbMainWindow *mainWindow = new HbMainWindow;
    mAllMainWindows.append(mainWindow);
    mScene = new QGraphicsScene;
}

// -----------------------------------------------------------------------------
// HbInstance::~HbInstance
// -----------------------------------------------------------------------------
//
HbInstance::~HbInstance()
{
    mAllMainWindows.clear();
    delete mScene;
}

// -----------------------------------------------------------------------------
// HbInstance::HbInstance
// -----------------------------------------------------------------------------
//
Qt::Orientation HbInstance::orientation() const
{
    return mOrientation;
}

// -----------------------------------------------------------------------------
// HbInstance::HbInstance
// -----------------------------------------------------------------------------
//
void HbInstance::setOrientation(Qt::Orientation orientation)
{
    mOrientation = orientation;
}

// -----------------------------------------------------------------------------
// HbInstance::HbInstance
// -----------------------------------------------------------------------------
//
QGraphicsScene* HbInstance::scene() const
{
    return mScene;
}

// HbMainWindow

// -----------------------------------------------------------------------------
// HbMainWindow::HbMainWindow
// -----------------------------------------------------------------------------
//
HbMainWindow::HbMainWindow(QWidget *parent,Hb::WindowFlags windowFlags)
: QGraphicsView(parent), mCurrentView(0), mViewCount(0)
{
    Q_UNUSED(windowFlags)
}

// -----------------------------------------------------------------------------
// HbMainWindow::~HbMainWindow
// -----------------------------------------------------------------------------
//
HbMainWindow::~HbMainWindow()
{
}

// -----------------------------------------------------------------------------
// HbMainWindow::addView
// -----------------------------------------------------------------------------
//
HbView* HbMainWindow::addView(QGraphicsWidget *widget)
{
    Q_UNUSED(widget)
    mTestViews.append(static_cast<HbView*>( widget ) );
    return 0;
}

// -----------------------------------------------------------------------------
// HbMainWindow::removeView
// -----------------------------------------------------------------------------
//
QGraphicsWidget* HbMainWindow::removeView(int index)
{
    Q_UNUSED(index)
    if ( index >= 0 && index < mTestViews.count() ){
        return mTestViews.takeAt(index);
    }
    return mCurrentView;
}

// -----------------------------------------------------------------------------
// HbMainWindow::removeView
// -----------------------------------------------------------------------------
//
void HbMainWindow::removeView(QGraphicsWidget *widget)
{
    for(int i = 0; i < mTestViews.count(); i++ ){
        if ( mTestViews.at(i) == widget ){
            mTestViews.takeAt(i);
        }
    }
}

// -----------------------------------------------------------------------------
// HbMainWindow::currentView
// -----------------------------------------------------------------------------
//
HbView* HbMainWindow::currentView() const
{
    return mCurrentView;
}

// -----------------------------------------------------------------------------
// HbMainWindow::setCurrentView
// -----------------------------------------------------------------------------
//
void HbMainWindow::setCurrentView(HbView *view, bool animate)
{
	mCurrentView = view;
    Q_UNUSED(animate)
}

// -----------------------------------------------------------------------------
// HbMainWindow::viewCount
// -----------------------------------------------------------------------------
//
int HbMainWindow::viewCount() const
{
    return mTestViews.count();
}

// -----------------------------------------------------------------------------
// HbMainWindow::viewCount
// -----------------------------------------------------------------------------
//
void HbMainWindow::setOrientation(Qt::Orientation orientation, bool animate)
{
    Q_UNUSED(animate);
    mOrientation = orientation;
}

// -----------------------------------------------------------------------------
// HbMainWindow::viewCount
// -----------------------------------------------------------------------------
//
Qt::Orientation HbMainWindow::orientation() const
{
    return mOrientation;
}

// -----------------------------------------------------------------------------
// HbMainWindow::views
// -----------------------------------------------------------------------------
//
QList<HbView *> HbMainWindow::views() const
{
    return mTestViews;
}

// -----------------------------------------------------------------------------
// HbMainWindow::sceneRect
// -----------------------------------------------------------------------------
//
QRectF HbMainWindow::sceneRect() const
{
    return QRectF( 0, 0, 360, 640 );
}

// HbWidget

// -----------------------------------------------------------------------------
// HbWidget::HbWidget
// -----------------------------------------------------------------------------
//
HbWidget::HbWidget(QGraphicsItem *parent, Qt::WindowFlags wFlags)
: QGraphicsWidget(parent,wFlags)
{
    
}

// -----------------------------------------------------------------------------
// HbWidget::~HbWidget
// -----------------------------------------------------------------------------
//
HbWidget::~HbWidget()
{
    
}

// -----------------------------------------------------------------------------
// HbWidget::clearActions
// -----------------------------------------------------------------------------
//
void HbWidget::clearActions()
{
}

// -----------------------------------------------------------------------------
// HbWidget::itemChange
// -----------------------------------------------------------------------------
//
QVariant HbWidget::itemChange(GraphicsItemChange change, const QVariant& value)
{
    Q_UNUSED(change)
    return QVariant(value);
}

// -----------------------------------------------------------------------------
// HbWidget::mainWindow
// -----------------------------------------------------------------------------
//
HbMainWindow* HbWidget::mainWindow() const
{   
   
}

// HbDialog

// -----------------------------------------------------------------------------
// HbDialog::HbDialog
// -----------------------------------------------------------------------------
//
HbDialog::HbDialog(QGraphicsItem *parent) : HbWidget(parent)
{
	resize(10,10); // Just some values for height and width
    mDismissPolicy = NoDismiss;
    mUnfadedItems = Hb::NoItem;
}

// -----------------------------------------------------------------------------
// HbDialog::~HbDialog
// -----------------------------------------------------------------------------
//
HbDialog::~HbDialog()
{
}

// -----------------------------------------------------------------------------
// HbDialog::timeout
// -----------------------------------------------------------------------------
//
int HbDialog::timeout() const
{
    return mTimeout;
}

// -----------------------------------------------------------------------------
// HbDialog::setTimeout
// -----------------------------------------------------------------------------
//
void HbDialog::setTimeout(int timeout)
{
    mTimeout = timeout;
}

// -----------------------------------------------------------------------------
// HbDialog::setTimeout
// -----------------------------------------------------------------------------
//
void HbDialog::setTimeout(HbDialog::DefaultTimeout timeout)
{
    // Timeout Values from the current implementation of orbit.
    switch (timeout)
        {
        case HbDialog::NoTimeout:
        mTimeout = 0;
        break;
        case HbDialog::ConfirmationNoteTimeout:
        mTimeout = 1500;
        break;
        case HbDialog::StandardTimeout:
        mTimeout = 3000;
        break;
        case HbDialog::ContextMenuTimeout:
        mTimeout = 6000;
        break;
        }
}

// -----------------------------------------------------------------------------
// HbDialog::setHeadingWidget
// -----------------------------------------------------------------------------
//
void HbDialog::setHeadingWidget(QGraphicsWidget *headingWidget)
{
    Q_UNUSED(headingWidget)    
}

// -----------------------------------------------------------------------------
// HbDialog::setContentWidget
// -----------------------------------------------------------------------------
//
void HbDialog::setContentWidget(QGraphicsWidget *contentWidget)
{
    Q_UNUSED(contentWidget)    
}

// -----------------------------------------------------------------------------
// HbDialog::unfadedItems
// -----------------------------------------------------------------------------
//
Hb::SceneItems HbDialog::unfadedItems() const
{
    return mUnfadedItems;
}

// -----------------------------------------------------------------------------
// HbDialog::setUnfadedItems
// -----------------------------------------------------------------------------
//
void HbDialog::setUnfadedItems(Hb::SceneItems unfadedItems)
{
	mUnfadedItems = unfadedItems;
}

// -----------------------------------------------------------------------------
// HbDialog::dismissPolicy
// -----------------------------------------------------------------------------
//
HbDialog::DismissPolicy HbDialog::dismissPolicy() const
{
    return mDismissPolicy;
}

// -----------------------------------------------------------------------------
// HbDialog::setDismissPolicy
// -----------------------------------------------------------------------------
//
void HbDialog::setDismissPolicy(HbDialog::DismissPolicy dismissPolicy)
{
    mDismissPolicy = dismissPolicy;
}

// -----------------------------------------------------------------------------
// HbDialog::itemChange
// -----------------------------------------------------------------------------
//
QVariant HbDialog::itemChange(GraphicsItemChange change, const QVariant& value)
{
    Q_UNUSED(change)
    return QVariant(value);
}


// HbMenu

// -----------------------------------------------------------------------------
// HbMenu::HbMenu
// -----------------------------------------------------------------------------
//
HbMenu::HbMenu(QGraphicsItem *parent) : HbDialog(parent)
{
    mIsEmpty = true;
}

// -----------------------------------------------------------------------------
// HbMenu::~HbMenu
// -----------------------------------------------------------------------------
//
HbMenu::~HbMenu()
{
}

// -----------------------------------------------------------------------------
// HbMenu::open
// -----------------------------------------------------------------------------
//
void HbMenu::open(QObject* receiver, const char* member)
{
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

// -----------------------------------------------------------------------------
// HbMenu::addAction
// -----------------------------------------------------------------------------
//
HbAction* HbMenu::addAction(const QString &text)
{
    Q_UNUSED(text)
    mIsEmpty = false;
    return 0;
}

// -----------------------------------------------------------------------------
// HbMenu::isEmpty
// -----------------------------------------------------------------------------
//
bool HbMenu::isEmpty() const
{
	return mIsEmpty;
}


// HbAction

// -----------------------------------------------------------------------------
// HbAction::HbAction
// -----------------------------------------------------------------------------
//
HbAction::HbAction(QObject *parent) 
: QAction(parent), mIcon(QString())
{
}

// -----------------------------------------------------------------------------
// HbAction::HbAction
// -----------------------------------------------------------------------------
//
HbAction::HbAction(const QString &text, QObject *parent) 
: QAction(text,parent), mIcon(QString())
{    
}

// -----------------------------------------------------------------------------
// HbAction::~HbAction
// -----------------------------------------------------------------------------
//
HbAction::~HbAction()
{
}

// -----------------------------------------------------------------------------
// HbAction::setIcon
// -----------------------------------------------------------------------------
//
void HbAction::setIcon(const HbIcon &icon) 
{
    mIcon = icon;
}

// -----------------------------------------------------------------------------
// HbAction::icon
// -----------------------------------------------------------------------------
//
HbIcon HbAction::icon() const
{
    return mIcon;
}

// -----------------------------------------------------------------------------
// HbAction::HbAction
// -----------------------------------------------------------------------------
//
HbAction::HbAction(Hb::NavigationAction action, QObject* parent)
    : QAction ( parent ),mIcon(QString())
{
    Q_UNUSED(action);
}

// HbLabel

// -----------------------------------------------------------------------------
// HbLabel::HbLabel
// -----------------------------------------------------------------------------
//
HbLabel::HbLabel(QGraphicsItem *parent) : HbWidget(parent)
{
}

// -----------------------------------------------------------------------------
// HbLabel::HbLabel
// -----------------------------------------------------------------------------
//
HbLabel::HbLabel(const QString &displayText, QGraphicsItem *parent) 
: HbWidget(parent)
{
    Q_UNUSED(displayText)
}

// -----------------------------------------------------------------------------
// HbLabel::~HbLabel
// -----------------------------------------------------------------------------
//
HbLabel::~HbLabel()
{
}

// -----------------------------------------------------------------------------
// HbLabel::setGeometry
// -----------------------------------------------------------------------------
//
void HbLabel::setGeometry(const QRectF &rect)
{
    Q_UNUSED(rect)
}

// -----------------------------------------------------------------------------
// HbLabel::clear
// -----------------------------------------------------------------------------
//
void HbLabel::clear()
{
}

// -----------------------------------------------------------------------------
// HbLabel::setPlainText
// -----------------------------------------------------------------------------
//
void HbLabel::setPlainText(const QString &text)
{
    Q_UNUSED(text)
}

// -----------------------------------------------------------------------------
// HbLabel::setText
// -----------------------------------------------------------------------------
//
void HbLabel::setIcon(const HbIcon &icon)
{
    Q_UNUSED(icon)
}


// HbIcon

// -----------------------------------------------------------------------------
// HbIcon::HbIcon
// -----------------------------------------------------------------------------
//
HbIcon::HbIcon(const QString &iconName)
{
    mName = iconName;
}

// -----------------------------------------------------------------------------
// HbIcon::~HbIcon
// -----------------------------------------------------------------------------
//
HbIcon::~HbIcon()
{
}

// -----------------------------------------------------------------------------
// HbIcon::operator==
// -----------------------------------------------------------------------------
//
bool HbIcon::operator==(const HbIcon &other) const
{
	return (other.mName == mName);
}


// HbProgressDialog

// -----------------------------------------------------------------------------
// HbProgressDialog::HbProgressDialog
// -----------------------------------------------------------------------------
//
HbProgressDialog::HbProgressDialog(QGraphicsItem *parent) : HbDialog(parent)
{
}

// -----------------------------------------------------------------------------
// HbProgressDialog::itemChange
// -----------------------------------------------------------------------------
//
QVariant HbProgressDialog::itemChange(GraphicsItemChange change, const QVariant& value)
{
    Q_UNUSED(change)
    return QVariant(value);
}

// -----------------------------------------------------------------------------
// HbProgressDialog::setText
// -----------------------------------------------------------------------------
//
void HbProgressDialog::setText(const QString &text)
{
    Q_UNUSED(text)
}

// HbSliderPopup
// -----------------------------------------------------------------------------
// HbSliderPopup::HbSliderPopup
// -----------------------------------------------------------------------------
//

HbSliderPopup::HbSliderPopup(QGraphicsItem *parent) 
    : HbDialog(parent)
    
{

}

// -----------------------------------------------------------------------------
// HbSliderPopup::~HbSliderPopup
// -----------------------------------------------------------------------------
//
HbSliderPopup::~HbSliderPopup()
{
}

// -----------------------------------------------------------------------------
// HbSliderPopup::setRange
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setRange(int min,int max)
{
    mMinValue = min;
    mMaxValue = max;
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
int HbSliderPopup::minimum() const
{
    return mMinValue;
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
int HbSliderPopup::maximum() const
{
    return mMaxValue;
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
int HbSliderPopup::value() const
{
    return mValue;
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
int HbSliderPopup::singleStep() const
{
    return mSingleStep;
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setSingleStep(int step)
    {
    mSingleStep = step;
    }

// -----------------------------------------------------------------------------
// HbSliderPopup::setValue
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setValue(int value)
{
    mValue = value;
}



// HbZoomSliderPopup

// -----------------------------------------------------------------------------
// HbZoomSliderPopup::HbZoomSliderPopup
// -----------------------------------------------------------------------------
//
HbZoomSliderPopup::HbZoomSliderPopup( ) : HbSliderPopup()
{
}

// -----------------------------------------------------------------------------
// HbZoomSliderPopup::~HbZoomSliderPopup
// -----------------------------------------------------------------------------
//
HbZoomSliderPopup::~HbZoomSliderPopup()
{
}


// -----------------------------------------------------------------------------
// HbToolBar::HbToolBar
// -----------------------------------------------------------------------------
//
HbToolBar::HbToolBar(QGraphicsItem *parent) : HbWidget(parent)
{
}

// -----------------------------------------------------------------------------
// HbToolBar::~HbToolBar
// -----------------------------------------------------------------------------
//
HbToolBar::~HbToolBar()
{
}


// HbView

// -----------------------------------------------------------------------------
// HbView::HbView
// -----------------------------------------------------------------------------
//
HbView::HbView(QGraphicsItem *parent) : HbWidget(parent)
{
    mMenu = new HbMenu(this);
    mToolBar = new HbToolBar(this);
    mTitlebarVisible = true;
    mDockWidgetVisible = true;
}

// -----------------------------------------------------------------------------
// HbView::~HbView
// -----------------------------------------------------------------------------
//
HbView::~HbView()
{
}

// -----------------------------------------------------------------------------
// HbView::menu
// -----------------------------------------------------------------------------
//
HbMenu* HbView::menu() const
{
    return mMenu;
}

// -----------------------------------------------------------------------------
// HbView::toolBar
// -----------------------------------------------------------------------------
//
HbToolBar* HbView::toolBar() const
{
    return mToolBar;
}

// -----------------------------------------------------------------------------
// HbView::setTitle
// -----------------------------------------------------------------------------
//
void HbView::setTitle(const QString &title)
{
    Q_UNUSED(title)
}

// -----------------------------------------------------------------------------
// HbView::showItems
// -----------------------------------------------------------------------------
//
void HbView::showItems(Hb::SceneItems items)
{
    if ( items == Hb::DockWidgetItem ) {
        mDockWidgetVisible = true;
    } 
}

// -----------------------------------------------------------------------------
// HbView::hideItems
// -----------------------------------------------------------------------------
//
void HbView::hideItems(Hb::SceneItems items)
{
    if ( items == Hb::DockWidgetItem ) {
        mDockWidgetVisible = false;
    }
}


// -----------------------------------------------------------------------------
// HbView::isItemVisible
// -----------------------------------------------------------------------------
//
bool HbView::isItemVisible(Hb::SceneItem items)  const
{
    if ( items == Hb::DockWidgetItem ) {
        return mDockWidgetVisible;
    }
    return false;
}

// -----------------------------------------------------------------------------
// HbView::setTitleBarVisible
// -----------------------------------------------------------------------------
//
void HbView::setTitleBarVisible(bool visible)
{
    mTitlebarVisible = visible;
}

// -----------------------------------------------------------------------------
// HbView::isTitleBarVisible
// -----------------------------------------------------------------------------
//
bool HbView::isTitleBarVisible()
{
    return mTitlebarVisible;
}

// -----------------------------------------------------------------------------
// HbView::setContentFullScreen
// -----------------------------------------------------------------------------
//
void HbView::setContentFullScreen(bool /*enable*/)
{
}

// -----------------------------------------------------------------------------
// HbView::navigationAction
// -----------------------------------------------------------------------------
//
HbAction *HbView::navigationAction() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// HbView::setNavigationAction
// -----------------------------------------------------------------------------
//
void HbView::setNavigationAction(HbAction *action)
{
    Q_UNUSED(action);
}

// HbDocumentLoader

// -----------------------------------------------------------------------------
// HbDocumentLoader::HbDocumentLoader
// -----------------------------------------------------------------------------
//
HbDocumentLoader::HbDocumentLoader()
{
    mLabel = new HbLabel;
    mAction = new HbAction;
    mReturnSendVideo = true;
    mPushButton = new HbPushButton();
    
}

// -----------------------------------------------------------------------------
// HbDocumentLoader::~HbDocumentLoader
// -----------------------------------------------------------------------------
//
HbDocumentLoader::~HbDocumentLoader()
{
    delete mAction;
    delete mLabel;
    delete mPushButton;
}

// -----------------------------------------------------------------------------
// HbDocumentLoader::load
// -----------------------------------------------------------------------------
//
QObjectList HbDocumentLoader::load( const QString &fileName, 
        const QString &section , bool *ok )
{
    Q_UNUSED(fileName)
    if ( section == lcLayoutPortraitDefaultId ||
         section == lcLayoutPortraitSwappedId ||
         section == lcLayoutLandscapeDefaultId ||
         section == lcLayoutLandscapeSwappedId ||
         section == lcLayoutFullscreenId ||
         section == lcLayoutLandscapeDialpadId ) {
        mLayoutSection = section;
        *ok = true;
    } else {
        *ok = false;
    }
    return QObjectList();  
}

// -----------------------------------------------------------------------------
// HbDocumentLoader::load
// -----------------------------------------------------------------------------
//
QObjectList HbDocumentLoader::load(const QString &fileName, bool *ok)
{
	*ok = true;
    Q_UNUSED(fileName)
    return QObjectList();
}

// -----------------------------------------------------------------------------
// HbDocumentLoader::findWidget
// -----------------------------------------------------------------------------
//
QGraphicsWidget* HbDocumentLoader::findWidget(const QString &name)
{
        if ( !mReturnSendVideo && name == lcWidgetSendVideoId ) {
            return 0;
        }
	foreach(QGraphicsWidget* widget, mWidgets) {
		if (widget->objectName() == name)
			return widget;
	}
	if (name == lcLabelRecipientId ||
	    name == lcLabelDurationId ||
	    name == lcIconContactId ||
	    name == lcIconBrandId ||
	    name == lcWidgetSendVideoId2 ||
	    name == lcWidgetRecvVideoId2 ) {
		return mLabel;
	}
	QString dummy = "";
	QGraphicsWidget* createdWidget = 
		static_cast<QGraphicsWidget*>(createObject(dummy,name));
    if (createdWidget) {
        mWidgets.append(createdWidget);
    }
    return createdWidget;
}

// -----------------------------------------------------------------------------
// HbDocumentLoader::findObject
// -----------------------------------------------------------------------------
//
QObject* HbDocumentLoader::findObject(const QString &name) const
{
	if (name == lcActMuteId ||
	    name == lcActSpeakerId ) {
		return mAction;
	}
	if( name == lcButtonEndCall ){
        return mPushButton;
	}
        foreach(HbAction* action, mActions) {
                if (action->objectName() == name)
                        return action;
        }

    return 0;
}

// -----------------------------------------------------------------------------
// HbDocumentLoader::setObjectTree
// -----------------------------------------------------------------------------
//
bool HbDocumentLoader::setObjectTree( QObjectList roots )
{
    Q_UNUSED(roots)
    return true;
}

// -----------------------------------------------------------------------------
// HbDocumentLoader::reset
// -----------------------------------------------------------------------------
//
void HbDocumentLoader::reset()
{
}

// -----------------------------------------------------------------------------
// HbDocumentLoader::createObject
// -----------------------------------------------------------------------------
//
QObject* HbDocumentLoader::createObject(
    const QString& type, 
    const QString& name)
{
    Q_UNUSED(type)
    Q_UNUSED(name)
    return 0;
}


// HbTransparentWindow

// -----------------------------------------------------------------------------
// HbTransparentWindow::HbTransparentWindow
// -----------------------------------------------------------------------------
//
HbTransparentWindow::HbTransparentWindow(QGraphicsItem *parent)
{
    Q_UNUSED(parent)
}

// -----------------------------------------------------------------------------
// HbTransparentWindow::~HbTransparentWindow
// -----------------------------------------------------------------------------
//
HbTransparentWindow::~HbTransparentWindow()
{
}

// -----------------------------------------------------------------------------
// HbGesture::HbGesture
// -----------------------------------------------------------------------------
//
HbGesture::HbGesture( Direction direction, int minDistance, QObject *parent )
    : QObject(parent)
{
    Q_UNUSED(direction)
    Q_UNUSED(minDistance)
}

// -----------------------------------------------------------------------------
// HbGesture::~HbGesture
// -----------------------------------------------------------------------------
//
HbGesture::~HbGesture()
{
}

// -----------------------------------------------------------------------------
// HbGesturSeceneFilter::HbGestureSceneFilter
// -----------------------------------------------------------------------------
//
HbGestureSceneFilter::HbGestureSceneFilter( Qt::MouseButton button, QGraphicsItem *parent ) 
    : HbWidget(parent)
{
    Q_UNUSED(button)
}

// -----------------------------------------------------------------------------
// HbGestureSceneFilter::~HbGestureSceneFilter
// -----------------------------------------------------------------------------
//
HbGestureSceneFilter::~HbGestureSceneFilter()
{
    qDeleteAll(gestures);
    gestures.clear();
}

// -----------------------------------------------------------------------------
// HbGestureSceneFilter::addGesture
// -----------------------------------------------------------------------------
//
void HbGestureSceneFilter::addGesture( HbGesture *gesture )
{
    gestures.append(gesture);
}


// -----------------------------------------------------------------------------
// HbEffect::HbEffect
// -----------------------------------------------------------------------------
//
HbEffect::HbEffect(  )
{
    mRunning = false;
}

// -----------------------------------------------------------------------------
// HbEffect::start
// -----------------------------------------------------------------------------
//
bool HbEffect::start( QGraphicsItem *item, 
                            const QString &itemType, 
                            const QString &effectEvent,
                            QObject *receiver,
                            const char *member,
                            const QVariant &userData )
{
    Q_UNUSED(item)
    Q_UNUSED(itemType)
    Q_UNUSED(effectEvent)
    Q_UNUSED(receiver)
    Q_UNUSED(member)
    Q_UNUSED(userData)
    return true;
}

// -----------------------------------------------------------------------------
// HbEffect::add
// -----------------------------------------------------------------------------
//
bool HbEffect::add( const QString &itemType, const QString &filePath, const QString &effectEvent )
{
    Q_UNUSED(itemType)
    Q_UNUSED(filePath)
    Q_UNUSED(effectEvent)
    return true;
}

// -----------------------------------------------------------------------------
// HbEffect::effectRunning
// -----------------------------------------------------------------------------
//
bool HbEffect::effectRunning(QGraphicsItem *item, const QString &effectEvent )
{
    Q_UNUSED(item)
    Q_UNUSED(effectEvent)
    return mRunning;
}


// -----------------------------------------------------------------------------
// HbPushButton::HbPushButton
// -----------------------------------------------------------------------------
//
HbPushButton::HbPushButton(QGraphicsItem *parent) : HbAbstractButton(parent)
{
}


// -----------------------------------------------------------------------------
// HbPushButton::HbPushButton
// -----------------------------------------------------------------------------
//
HbPushButton::HbPushButton(const QString &text, QGraphicsItem *parent)
    : HbAbstractButton(parent)
{
    Q_UNUSED(text)
}



// -----------------------------------------------------------------------------
// HbPushButton::~HbPushButton
// -----------------------------------------------------------------------------
//
HbPushButton::~HbPushButton()
{
}

// -----------------------------------------------------------------------------
// HbPushButton::setFrameBackground
// -----------------------------------------------------------------------------
//
void HbPushButton::setFrameBackground( HbFrameDrawer *backgroundFrameDrawer )
{
    delete backgroundFrameDrawer;
}

// -----------------------------------------------------------------------------
// HbAbstractButton::HbAbstractButton
// -----------------------------------------------------------------------------
//
HbAbstractButton::HbAbstractButton(QGraphicsItem *parent) : HbWidget(parent)
{
}

// -----------------------------------------------------------------------------
// HbAbstractButton::~HbAbstractButton
// -----------------------------------------------------------------------------
//
HbAbstractButton::~HbAbstractButton()
{
}

// -----------------------------------------------------------------------------
// HbFrameDrawer::HbFrameDrawer
// -----------------------------------------------------------------------------
//
HbFrameDrawer::HbFrameDrawer(const QString &frameGraphicsName, FrameType type, bool cacheFlag)
{
    Q_UNUSED(frameGraphicsName);
    Q_UNUSED(type);
    Q_UNUSED(cacheFlag);
}

// -----------------------------------------------------------------------------
// HbFrameDrawer::~HbFrameDrawer
// -----------------------------------------------------------------------------
//
HbFrameDrawer::~HbFrameDrawer()
{
    
}

// -----------------------------------------------------------------------------
// HbLineEdit::HbLineEdit
// -----------------------------------------------------------------------------
//
HbLineEdit::HbLineEdit(QGraphicsItem *parent)
{
    Q_UNUSED(parent);
}

// -----------------------------------------------------------------------------
// HbLineEdit::HbLineEdit
// -----------------------------------------------------------------------------
//
HbLineEdit::HbLineEdit(const QString &text, QGraphicsItem *parent)
{
    Q_UNUSED(parent);
    mText = text;
}

// -----------------------------------------------------------------------------
// HbLineEdit::~HbLineEdit
// -----------------------------------------------------------------------------
//
HbLineEdit::~HbLineEdit()
{
    
}

// -----------------------------------------------------------------------------
// HbLineEdit::setText
// -----------------------------------------------------------------------------
//
void HbLineEdit::setText(const QString &text)
{
    mText = text;
}

// -----------------------------------------------------------------------------
// HbLineEdit::text
// -----------------------------------------------------------------------------
//
QString HbLineEdit::text() const
{
    return mText;
}

// -----------------------------------------------------------------------------
// QGesture
// -----------------------------------------------------------------------------
//

Qt::GestureState QGesture::state() const
{
    return testState;
}


// -----------------------------------------------------------------------------
// HbTapGesture::HbTapGesture
// -----------------------------------------------------------------------------
//

HbTapGesture::HbTapGesture(QObject *paren) 
{
    Q_UNUSED(paren);
}


// -----------------------------------------------------------------------------
// HbTapGesture::~HbTapGesture
// -----------------------------------------------------------------------------
//

HbTapGesture::~HbTapGesture()
{
}


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
HbTapGesture::TapStyleHint HbTapGesture::tapStyleHint() const
{
    return testStyleHint;
}



// -----------------------------------------------------------------------------
// Stub Helper
// -----------------------------------------------------------------------------
//
void HbStubHelper::setGestureState(int state)
{
    testState = static_cast<Qt::GestureState> (state);
}


void HbStubHelper::setGestureStyleHint(int style) 
{
    testStyleHint = static_cast<HbTapGesture::TapStyleHint> (style);
}

void HbStubHelper::reset()
{
    testState = Qt::NoGesture;
}

// end of file
