/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Utility.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/NumericUpDown.h>

using namespace Gwk;
using namespace Gwk::Controls;


GWK_CONTROL_CONSTRUCTOR(NumericUpDown)
{
    SetSize(100, 20);
    Controls::Base* pSplitter = new Controls::Base(this);
    pSplitter->Dock(Docking::Right);
    pSplitter->SetWidth(13);
    NumericUpDownButton_Up* pButtonUp = new NumericUpDownButton_Up(pSplitter);
    pButtonUp->onPress.Add(this, &NumericUpDown::OnButtonUp);
    pButtonUp->SetTabable(false);
    pButtonUp->Dock(Docking::Top);
    pButtonUp->SetHeight(10);
    NumericUpDownButton_Down* pButtonDown = new NumericUpDownButton_Down(pSplitter);
    pButtonDown->onPress.Add(this, &NumericUpDown::OnButtonDown);
    pButtonDown->SetTabable(false);
    pButtonDown->Dock(Docking::Fill);
    pButtonUp->SetPadding(Padding(0, 1, 1, 0));
    m_iMax = 100;
    m_iMin = 0;
    m_iNumber = 0;
    SetText("0");
}

void NumericUpDown::OnButtonUp(Base* /*control*/)
{
    SyncNumberFromText();
    SetIntValue(m_iNumber+1);
}

void NumericUpDown::OnButtonDown(Base* /*control*/)
{
    SyncNumberFromText();
    SetIntValue(m_iNumber-1);
}

void NumericUpDown::SyncTextFromNumber()
{
    SetText(Utility::ToString(m_iNumber));
}

void NumericUpDown::SyncNumberFromText()
{
    SetIntValue((int)GetFloatFromText());
}

void NumericUpDown::SetMin(int i)
{
    m_iMin = i;
}

void NumericUpDown::SetMax(int i)
{
    m_iMax = i;
}

void NumericUpDown::SetIntValue(int i)
{
    i = Clamp(i, m_iMin, m_iMax);

    if (m_iNumber == i)
        return;

    m_iNumber = i;
    // Don't update the text if we're typing in it..
    // Undone - any reason why not?
    // if ( !HasFocus() )
    {
        SyncTextFromNumber();
    }
    OnChange();
}

void NumericUpDown::OnChange()
{
    onChanged.Call(this);
}

void NumericUpDown::OnTextChanged()
{
    ParentClass::OnTextChanged();
    SyncNumberFromText();
}

void NumericUpDown::OnEnter()
{
    SyncNumberFromText();
    SyncTextFromNumber();
    ParentClass::OnEnter();
}
