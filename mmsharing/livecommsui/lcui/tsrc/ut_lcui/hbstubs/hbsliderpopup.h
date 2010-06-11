/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  HbSliderPopup class definition.
*
*/


#ifndef HB_SLIDERPOPUP_H
#define HB_SLIDERPOPUP_H

//#include <hbslider.h>
#include <hbdialog.h>

class HbSliderPopup : public HbDialog
{

public:
    explicit HbSliderPopup(QGraphicsItem *parent = 0);
    virtual ~HbSliderPopup();

    int maximum() const;
    void setMaximum(int max);

    int minimum() const;
    void setMinimum(int min);

    void setRange(int min, int max);

    int singleStep() const;
    void setSingleStep(int step);
    int value() const;

public slots:
    void setValue(int value);

private: // Data added for the unit testing stub
    int mMinValue;
    int mMaxValue;
    int mValue;
    int mSingleStep;
};

#endif // HB_SLIDERPOPUP_H
