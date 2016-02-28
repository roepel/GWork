/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_IMAGEPANEL_H
#define GWK_CONTROLS_IMAGEPANEL_H

#include <Gwork/Gwork.h>
#include <Gwork/Controls/Base.h>
#include <Gwork/BaseRender.h>
#include <Gwork/Texture.h>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT ImagePanel : public Controls::Base
        {
        public:

            GWK_CONTROL_INLINE(ImagePanel, Controls::Base)
            {
                SetUV(0, 0, 1, 1);
                SetMouseInputEnabled(false);
                m_DrawColor = Colors::White;
                SetStretch(true);
            }

            virtual ~ImagePanel()
            {
                m_Texture.Release(GetSkin()->GetRender());
            }

            virtual void SetUV(float u1, float v1, float u2, float v2)
            {
                m_uv[0] = u1;
                m_uv[1] = v1;
                m_uv[2] = u2;
                m_uv[3] = v2;
            }

            virtual void SetImage(const String& imageName)
            {
                m_Texture.Load(imageName, GetSkin()->GetRender());
            }

            virtual String& GetImage()
            {
                return m_Texture.name;
            }

            virtual int TextureWidth()
            {
                return m_Texture.width;
            }

            virtual int TextureHeight()
            {
                return m_Texture.height;
            }

            virtual const String& GetImageName()
            {
                return m_Texture.name;
            }

            virtual void Render(Skin::Base* skin) override
            {
                skin->GetRender()->SetDrawColor(m_DrawColor);

                if (m_bStretch)
                {
                    skin->GetRender()->DrawTexturedRect(&m_Texture,
                                                        GetRenderBounds(),
                                                        m_uv[0], m_uv[1], m_uv[2], m_uv[3]);
                }
                else
                {
                    skin->GetRender()->DrawTexturedRect(&m_Texture,
                                                        Gwk::Rect(0, 0,
                                                                   m_Texture.width,m_Texture.height),
                                                        m_uv[0], m_uv[1], m_uv[2], m_uv[3]);
                }
            }

            virtual void SizeToContents()
            {
                SetSize(m_Texture.width, m_Texture.height);
            }

            virtual void SetDrawColor(Gwk::Color color)
            {
                m_DrawColor = color;
            }

            virtual bool FailedToLoad()
            {
                return m_Texture.FailedToLoad();
            }

            virtual bool GetStretch()
            {
                return m_bStretch;
            }

            virtual void SetStretch(bool b)
            {
                m_bStretch = b;
            }

        protected:

            Texture m_Texture;
            float m_uv[4];
            Gwk::Color m_DrawColor;

            bool m_bStretch;

        };


    }
}
#endif // ifndef GWK_CONTROLS_IMAGEPANEL_H
