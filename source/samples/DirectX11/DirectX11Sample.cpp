/*
 *  Gwork
 *  Copyright (c) 2013-2017 Bill Quith
 *  Sample provided by Rashawn Clarke - topblast@github
 *  See license in Gwork.h
 */

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <memory.h>
#include <tchar.h>

#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/Test.h>
#include <Gwork/Input/Windows.h>
#include <Gwork/Renderers/DirectX11.h>
#include <Gwork/Platform.h>

HWND                    g_pHWND = NULL;
ID3D11Device*           g_pd3dDevice = NULL;
ID3D11DeviceContext*    g_pImmediateContext = NULL;
IDXGISwapChain*         g_pSwapChain = NULL;
ID3D11RenderTargetView* g_pRenderTargetView = NULL;

//
// Create a Window to render to.
//
static HWND CreateGameWindow( void )
{
    WNDCLASS    wc;
    ZeroMemory( &wc, sizeof( wc ) );
    wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc      = DefWindowProc;
    wc.hInstance        = GetModuleHandle( NULL );
    wc.lpszClassName    = "GWorkWindow";
    wc.hCursor          = LoadCursor( NULL, IDC_ARROW );
    RegisterClass( &wc );
    HWND hWindow = CreateWindowEx( ( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE ),
                                   wc.lpszClassName,
                                   "GWork DirectX11 Sample",
                                   ( WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN )
                                       & ~( WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME ),
                                   -1, -1, 1004, 650, NULL, NULL,
                                   GetModuleHandle( NULL ),
                                   NULL );
    ShowWindow( hWindow, SW_SHOW );
    SetForegroundWindow( hWindow );
    SetFocus( hWindow );
    return hWindow;
}

//
// Typical DirectX stuff to create a D3D device
//
static void CreateD3DDevice()
{
    RECT ClientRect;
    GetClientRect( g_pHWND, &ClientRect );
    UINT width = ClientRect.right - ClientRect.left;
    UINT height = ClientRect.bottom - ClientRect.top;

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_pHWND;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    sd.BufferDesc.RefreshRate = { 1, 60 };
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    
    HRESULT hr = S_OK;
    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        hr = D3D11CreateDeviceAndSwapChain( NULL,
                                            driverTypes[driverTypeIndex],
                                            NULL,
                                            D3D11_CREATE_DEVICE_SINGLETHREADED,
                                            featureLevels,
                                            numFeatureLevels,
                                            D3D11_SDK_VERSION,
                                            &sd,
                                            &g_pSwapChain,
                                            &g_pd3dDevice,
                                            NULL,
                                            &g_pImmediateContext );
        if (SUCCEEDED(hr))
            break;
    }

    if ( FAILED( hr ) )
    {
        OutputDebugString("Couldn't create D3D Device for some reason!");
        return;
    }

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = NULL;
    hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr))
    {
        OutputDebugString("Couldn't get back buffer from Swap Chain for some reason!");
        return;
    }

    hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
    {
        OutputDebugString("Couldn't create render target for some reason!");
        return;
    }

    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports(1, &vp);
}

//
// Program starts here
//
int main( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
    SetCurrentDirectory(Gwk::Platform::GetExecutableDir().c_str()); 

    // Create a window and attach DirectX to it
    g_pHWND = CreateGameWindow();
    CreateD3DDevice();
    RECT FrameBounds;
    GetClientRect( g_pHWND, &FrameBounds );

    // Create resource path calculator and loader.
    Gwk::Platform::RelativeToExecutablePaths paths(GWORK_RESOURCE_DIR);
    Gwk::Renderer::DirectX11ResourceLoader loader(paths, g_pd3dDevice);

    // Create a GWork DirectX renderer
    Gwk::Renderer::DirectX11* pRenderer = new Gwk::Renderer::DirectX11(loader, g_pd3dDevice);

    // Create a GWork skin
    Gwk::Skin::TexturedBase* pSkin = new Gwk::Skin::TexturedBase( pRenderer );
    pSkin->Init( "DefaultSkin.png" );

    // Create a Canvas (it's root, on which all other Gwork panels are created)
    Gwk::Controls::Canvas* pCanvas = new Gwk::Controls::Canvas( pSkin );
    pCanvas->SetSize( FrameBounds.right, FrameBounds.bottom );
    pCanvas->SetDrawBackground( true );
    pCanvas->SetBackgroundColor( Gwk::Color( 150, 170, 170, 255 ) );

    // Create our unittest control (which is a Window with controls in it)
    auto pUnit = new TestFrame(pCanvas);
    pUnit->SetPos(10, 10);

    // Create a Windows Control helper
    // (Processes Windows MSG's and fires input at Gwork)
    Gwk::Input::Windows input(pCanvas);

    // Begin the main game loop
    MSG msg;
    while ( true )
    {
        // Skip out if the window is closed
        if ( !IsWindowVisible( g_pHWND ) )
            break;

        // If we have a message from windows..
        if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            // .. give it to the input handler to process
            if (input.ProcessMessage(msg))
                continue;

            // if it's QUIT then quit..
            if ( msg.message == WM_QUIT )
                break;

            // Handle the regular window stuff..
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            float ClearColor[4] = { 0.128f, 0.128f, 0.3f, 1.0f };
            // Normal DirectX rendering loop
            g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

            // Render Gwork.
            pCanvas->RenderCanvas();

            g_pSwapChain->Present(0, 0);
        }
    }

    delete pCanvas;
    delete pSkin;
    //delete pRenderer;     /// @todo - Fix Windows font clean up

    if (g_pRenderTargetView)
    {
        g_pRenderTargetView->Release();
        g_pRenderTargetView = NULL;
    }

    if (g_pSwapChain)
    {
        g_pSwapChain->Release();
        g_pSwapChain = NULL;
    }

    if (g_pImmediateContext)
    {
        g_pImmediateContext->Release();
        g_pImmediateContext = NULL;
    }

    if (g_pd3dDevice)
    {
        g_pd3dDevice->Release();
        g_pd3dDevice = NULL;
    }
}
