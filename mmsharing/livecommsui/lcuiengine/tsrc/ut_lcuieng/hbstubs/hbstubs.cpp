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
#include <hbaction.h>
#include <hblabel.h>
#include <hbprogressdialog.h>
#include <hblineedit.h>
#include <hbview.h>

// Static data that simulates HbInstance and HbMainWindow
static HbInstance* hbInstanceSingleton = 0;

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
}

// -----------------------------------------------------------------------------
// HbInstance::~HbInstance
// -----------------------------------------------------------------------------
//
HbInstance::~HbInstance()
{
    mAllMainWindows.clear();
}


// HbMainWindow

// -----------------------------------------------------------------------------
// HbMainWindow::HbMainWindow
// -----------------------------------------------------------------------------
//
HbMainWindow::HbMainWindow(QWidget *parent,Hb::WindowFlags windowFlags)
: QGraphicsView(parent)
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

// HbDialog

// -----------------------------------------------------------------------------
// HbDialog::HbDialog
// -----------------------------------------------------------------------------
//
HbDialog::HbDialog(QGraphicsItem *parent) : HbWidget(parent)
{
}

// -----------------------------------------------------------------------------
// HbDialog::~HbDialog
// -----------------------------------------------------------------------------
//
HbDialog::~HbDialog()
{
}

// -----------------------------------------------------------------------------
// HbDialog::open()
// -----------------------------------------------------------------------------
//
void HbDialog::open(QObject* receiver, const char* member)
{
    Q_UNUSED(receiver);
    Q_UNUSED(member);
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
// HbDialog::itemChange
// -----------------------------------------------------------------------------
//
QVariant HbDialog::itemChange(GraphicsItemChange change, const QVariant& value)
{
    Q_UNUSED(change)
    return QVariant(value);
}

// -----------------------------------------------------------------------------
// HbDialog::setTimeout
// -----------------------------------------------------------------------------
//
void HbDialog::setTimeout(int timeout )
{
    Q_UNUSED(timeout)
}

// -----------------------------------------------------------------------------
// HbDialog::setDismissPolicy
// -----------------------------------------------------------------------------
//
void HbDialog::setDismissPolicy( HbDialog::DismissPolicy dismissPolicy )
{
    Q_UNUSED(dismissPolicy)
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


// HbLineEdit

// -----------------------------------------------------------------------------
// HbLineEdit::HbLineEdit
// -----------------------------------------------------------------------------
//
HbLineEdit::HbLineEdit(const QString &text, QGraphicsItem *parent) 
: HbWidget(parent)
{
    Q_UNUSED(text)
}

// -----------------------------------------------------------------------------
// HbLineEdit::~HbLineEdit
// -----------------------------------------------------------------------------
//
HbLineEdit::~HbLineEdit()
{
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


// HbView

// -----------------------------------------------------------------------------
// HbView::HbView
// -----------------------------------------------------------------------------
//
HbView::HbView(QGraphicsItem *parent) : HbWidget(parent)
{
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
    return 0;
}

// -----------------------------------------------------------------------------
// HbView::toolBar
// -----------------------------------------------------------------------------
//
HbToolBar* HbView::toolBar() const
{
    return 0;
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


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
QString hbTrId(const char *id, int n)
{
    Q_UNUSED(n);
    return QString(id);
}
