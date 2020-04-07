/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_PROPERTY_SLIDER_H
#define GWK_CONTROLS_PROPERTY_SLIDER_H

#include <Gwork/Controls/Property/BaseProperty.h>
#include <Gwork/Controls/HorizontalSlider.h>

namespace Gwk
{
    namespace Controls
    {
        namespace Property
        {
            class GWK_EXPORT Slider : public Property::Base
            {
            public:

                GWK_CONTROL_INLINE(Slider, Property::Base)
                {
                    m_slider = new Gwk::Controls::HorizontalSlider(this);
                    m_slider->Dock(Position::Fill);
                    m_slider->onValueChanged.Add(this, &ParentClass::OnPropertyValueChanged);
                    m_slider->SetTabable(true);
                    m_slider->SetKeyboardInputEnabled(true);
                    m_slider->SetShouldDrawBackground(false);
                    SetHeight(18);
                }

                String GetPropertyValue() override
                {
                    return std::to_string(m_slider->GetFloatValue());
                }

                void SetPropertyValue(const String& val, bool bFireChangeEvents) override
                {
                    m_slider->SetFloatValue(std::stof(val), bFireChangeEvents);
                }

                bool IsEditing() override
                {
                    return m_slider->IsFocussed();
                }

                bool IsHovered() const override
                {
                    return ParentClass::IsHovered() || m_slider->IsHovered();
                }

                Gwk::Controls::HorizontalSlider* GetSlider()
                {
                    return m_slider;
                }

            protected:

                Gwk::Controls::HorizontalSlider* m_slider;
            };


        }
    }
}
#endif // ifndef GWK_CONTROLS_PROPERTY_SLIDER_H
