﻿/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/Test.h>
#include <Gwork/Controls/CollapsibleList.h>

using namespace Gwk;

class CollapsibleList : public GUnit
{
public:

    GWK_CONTROL_INLINE(CollapsibleList, GUnit)
    {
        Gwk::Controls::CollapsibleList* pControl = new Gwk::Controls::CollapsibleList(this);
        pControl->SetSize(100, 200);
        pControl->SetPos(10, 10);
        {
            Gwk::Controls::CollapsibleCategory* cat = pControl->Add("Category One");
            cat->Add("Hello");
            cat->Add("Two");
            cat->Add("Three");
            cat->Add("Four");
        }
        {
            Gwk::Controls::CollapsibleCategory* cat = pControl->Add("Shopping");
            cat->Add("Special");
            cat->Add("Two Noses");
            cat->Add("Orange ears");
            cat->Add("Beer");
            cat->Add("Three Eyes");
            cat->Add("Special");
            cat->Add("Two Noses");
            cat->Add("Orange ears");
            cat->Add("Beer");
            cat->Add("Three Eyes");
            cat->Add("Special");
            cat->Add("Two Noses");
            cat->Add("Orange ears");
            cat->Add("Beer");
            cat->Add("Three Eyes");
        }
        {
            Gwk::Controls::CollapsibleCategory* cat = pControl->Add("Category One");
            cat->Add("Hello");
            cat->Add("Two");
            cat->Add("Three");
            cat->Add("Four");
        }
    }
};


DEFINE_UNIT_TEST(CollapsibleList);
