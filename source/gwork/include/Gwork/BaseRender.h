/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_BASERENDER_H
#define GWK_BASERENDER_H

#include "Gwork/Structures.h"

namespace Gwk
{
    struct Font;
    struct Texture;
    class WindowProvider;

    namespace Renderer
    {
        class Base;

        class ICacheToTexture
        {
        public:

            virtual ~ICacheToTexture() {}

            virtual void Initialize() = 0;
            virtual void ShutDown() = 0;
            virtual void SetupCacheTexture(Gwk::Controls::Base* control) = 0;
            virtual void FinishCacheTexture(Gwk::Controls::Base* control) = 0;
            virtual void DrawCachedControlTexture(Gwk::Controls::Base* control) = 0;
            virtual void CreateControlCacheTexture(Gwk::Controls::Base* control) = 0;
            virtual void UpdateControlCacheTexture(Gwk::Controls::Base* control) = 0;
            virtual void SetRenderer(Gwk::Renderer::Base* renderer) = 0;

        };

        //
        /// Base class for all renderer implementations.
        ///
        /// @note We never instance this directly, only the derived implementations.
        //
        class GWK_EXPORT Base
        {
        protected:
            
            Base(); // We only instance subclasses of this.

        public:

            virtual ~Base();

            virtual void Init()         {}
            virtual void Begin()        {}
            virtual void End()          {}

            virtual void SetDrawColor(Color color)          {}

            virtual void DrawFilledRect(Gwk::Rect rect)    {}

            virtual void StartClip()    {}
            virtual void EndClip()      {}

            virtual void LoadTexture(Gwk::Texture* pTexture)   {}
            virtual void FreeTexture(Gwk::Texture* pTexture)   {}

            virtual void DrawTexturedRect(Gwk::Texture* pTexture, Gwk::Rect pTargetRect,
                                          float u1 = 0.0f, float v1 = 0.0f,
                                          float u2 = 1.0f, float v2 = 1.0f)
            {}

            virtual void DrawMissingImage(Gwk::Rect pTargetRect);

            virtual Gwk::Color PixelColour(Gwk::Texture* pTexture,
                                            unsigned int x, unsigned int y,
                                            const Gwk::Color& col_default =
                                                                    Gwk::Color(255,255,255,255))
            {
                return col_default;
            }

            virtual ICacheToTexture* GetCTT()
            {
                return NULL;
            }

            virtual void LoadFont(Gwk::Font* pFont)    {}
            virtual void FreeFont(Gwk::Font* pFont)    {}

            virtual void RenderText(Gwk::Font* pFont,
                                    Gwk::Point pos,
                                    const Gwk::String& text);
            
            virtual Gwk::Point MeasureText(Gwk::Font* pFont,
                                            const Gwk::String& text);

            /// \sect{Render Specialisation}
            ///     No need to implement these functions in your derived class, but
            ///     if you can do them faster than the default implementation it's a
            ///     good idea to.
            //
            virtual void    DrawLinedRect(Gwk::Rect rect);
            virtual void    DrawPixel(int x, int y);
            virtual void    DrawShavedCornerRect(Gwk::Rect rect, bool bSlight = false);
            /// \}

        public:

            /// \sect{Translate}
            ///     Translate a panel's local drawing coordinate
            ///     into view space, taking Offsets into account.
            //
            void Translate(int& x, int& y);
            void Translate(Gwk::Rect& rect);
            /// \}

            //
            /// Set the rendering offset. You shouldn't have to touch these, ever.
            //
            void SetRenderOffset(const Gwk::Point& offset)
            {
                m_RenderOffset = offset;
            }

            void AddRenderOffset(const Gwk::Rect& offset)
            {
                m_RenderOffset.x += offset.x;
                m_RenderOffset.y += offset.y;
            }

            const Gwk::Point& GetRenderOffset() const
            {
                return m_RenderOffset;
            }

        private:

            Gwk::Point m_RenderOffset;

        public:

            void              SetClipRegion(Gwk::Rect rect);
            void              AddClipRegion(Gwk::Rect rect);
            bool              ClipRegionVisible();
            const Gwk::Rect& ClipRegion() const;

        private:

            Gwk::Rect m_rectClipRegion;
            ICacheToTexture* m_RTT;

        public:

            void SetScale(float fScale)     { m_fScale = fScale; }
            float Scale() const             { return m_fScale; }

        public:

            //
            // Self Initialization, shutdown
            //

            virtual bool InitializeContext(Gwk::WindowProvider* pWindow)
            {
                return false;
            }

            virtual bool ShutdownContext(Gwk::WindowProvider* pWindow)
            {
                return false;
            }

            virtual bool ResizedContext(Gwk::WindowProvider* pWindow, int w, int h)
            {
                return false;
            }

            virtual bool BeginContext(Gwk::WindowProvider* pWindow)
            {
                return false;
            }

            virtual bool EndContext(Gwk::WindowProvider* pWindow)
            {
                return false;
            }

            virtual bool PresentContext(Gwk::WindowProvider* pWindow)
            {
                return false;
            }
            
        protected:
            
            float m_fScale;            
        };


    }
}
#endif // ifndef GWK_BASERENDER_H