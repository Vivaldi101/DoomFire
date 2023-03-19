#include <time.h>
#include <Windows.h>

typedef unsigned int u32;
typedef int s32;
typedef unsigned char byte;
typedef float r32;
typedef double r64;

static int global_quit = 0;
static int global_time_residual = 0;
static int global_frame = 0;
static int global_last_frame_time = 0;
static DWORD global_sys_time_base = 0; 

#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof(*arr))
#define NUM_PALETTE 40
#define MSEC_PER_FRAME 8

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
static time_t global_rand_time;
static u32 global_fire_buffer[SCREEN_HEIGHT*SCREEN_WIDTH] = {0};
static u32 global_palette[NUM_PALETTE];
static const byte global_rgbs[NUM_PALETTE*3] = 
{
    0x00,0x00,0x00,
    0x07,0x07,0x07,
    0x1F,0x07,0x07,
    0x2F,0x0F,0x07,
    0x47,0x0F,0x07,
    0x57,0x17,0x07,
    0x67,0x1F,0x07,
    0x77,0x1F,0x07,
    0x8F,0x27,0x07,
    0x9F,0x2F,0x07,
    0xAF,0x3F,0x07,
    0xBF,0x47,0x07,
    0xC7,0x47,0x07,
    0xDF,0x4F,0x07,
    0xDF,0x57,0x07,
    0xDF,0x57,0x07,
    0xD7,0x5F,0x07,
    0xD7,0x5F,0x07,
    0xD7,0x67,0x0F,
    0xCF,0x6F,0x0F,
    0xCF,0x77,0x0F,
    0xCF,0x7F,0x0F,
    0xCF,0x87,0x17,
    0xC7,0x87,0x17,
    0xC7,0x8F,0x17,
    0xC7,0x97,0x1F,
    0xBF,0x9F,0x1F,
    0xBF,0x9F,0x1F,
    0xBF,0xA7,0x27,
    0xBF,0xA7,0x27,
    0xBF,0xAF,0x2F,
    0xB7,0xAF,0x2F,
    0xB7,0xB7,0x2F,
    0xB7,0xB7,0x37,
    0xCF,0xCF,0x6F,
    0xDF,0xDF,0x9F,
    0xCF,0xCF,0xC7,
    0xDF,0xEF,0xC9,
    0xEF,0xEF,0xF9,
    0xFF,0xFF,0xFF
};

typedef struct 
{
    HDC			hdc;
    HDC			hdc_dib_section;	// DC compatible with DIB section
    HWND		   window;
    HBITMAP		dib_section;		// DIB section

    byte *buffer;
    u32 width, height, pitch;
} screen_handle_t;
static screen_handle_t global_screen_handle;

static HGDIOBJ	global_previously_selected_GDI_obj;
typedef struct 
{
    BITMAPINFOHEADER	header;
    //RGBQUAD				colors[256];	
} dib_data_t;

void SpreadFire(u32 fire_index)
{
#define w SCREEN_WIDTH
    u32 pal = global_fire_buffer[fire_index];
    if (pal == 0)
    {
        global_fire_buffer[fire_index - w] = 0;
    }
    else
    {
        u32 ri = (u32)floor(((r64)rand() / (r64)RAND_MAX) * 3);
        u32 new_fire_index = fire_index - ri + 1;
        global_fire_buffer[new_fire_index - w] = pal - (ri & 1);
        ri = (u32)floor(((r64)rand() / (r64)RAND_MAX) * 3);
    }
#undef w
}

void StartFire()
{
#define w SCREEN_WIDTH
#define h SCREEN_HEIGHT
    for(u32 i = 0; i < w; i++)
    {
        //u32 ri = (u32)floor(((r64)rand() / (r64)RAND_MAX) * 999) & (NUM_PALETTE - 1);
        global_fire_buffer[(h-1)*w + i] = NUM_PALETTE-1;
    }
    for(u32 i = 0; i < w; i++)
    {
        for(u32 j = 1; j < h; j++)
        {
            SpreadFire(j*w + i);
        }
    }

#undef w
#undef h
}

void FillGlobalFirePal()
{
    for(u32 i = 0; i < NUM_PALETTE; i++)
    {
        u32 red = (global_rgbs[i * 3 + 0] << 16);
        u32 green = (global_rgbs[i * 3 + 1] << 8);
        u32 blue = (global_rgbs[i * 3 + 2] << 0);
        u32 total = red | green | blue;

        global_palette[i] = total;
    }

    for(u32 i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++)
    {
        global_fire_buffer[i] = 0;
    }
}

int Win_InitDIB() 
{
    dib_data_t dib;
    BITMAPINFO *win_dib_info = (BITMAPINFO *)&dib;

    memset(&dib, 0, sizeof(dib));

    if (!(global_screen_handle.hdc = GetDC(global_screen_handle.window))) 
    {
        //Sys_Print("\nCouldn't get dc for window\n");
        return 0;
    }

    //rt->bpp = BYTES_PER_PIXEL;

    win_dib_info->bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
    win_dib_info->bmiHeader.biWidth         = global_screen_handle.width;		
    win_dib_info->bmiHeader.biHeight        = -(s32)global_screen_handle.height; 
    win_dib_info->bmiHeader.biPlanes        = 1;
    win_dib_info->bmiHeader.biBitCount      = (WORD)(32);
    win_dib_info->bmiHeader.biCompression   = BI_RGB;
    win_dib_info->bmiHeader.biSizeImage     = 0;
    win_dib_info->bmiHeader.biXPelsPerMeter = 0;
    win_dib_info->bmiHeader.biYPelsPerMeter = 0;
    win_dib_info->bmiHeader.biClrUsed       = 0;
    win_dib_info->bmiHeader.biClrImportant  = 0;

    global_screen_handle.dib_section = CreateDIBSection(global_screen_handle.hdc,
            win_dib_info,
            DIB_RGB_COLORS,
            (void **)&global_screen_handle.buffer,
            0,
            0);

    if (!global_screen_handle.dib_section) 
    {
        //Sys_Print("\nCouldn't create the dib section\n");
        return 0;
    }

    global_screen_handle.pitch = global_screen_handle.width * 4;

    memset(global_screen_handle.buffer, 0, global_screen_handle.pitch * global_screen_handle.height);

    if (!(global_screen_handle.hdc_dib_section = CreateCompatibleDC(global_screen_handle.hdc))) 
    {
        //Sys_Print("\nDIB_Init() - CreateCompatibleDC failed\n");
        return 0;
    }
    if (!(global_previously_selected_GDI_obj = SelectObject(global_screen_handle.hdc_dib_section, global_screen_handle.dib_section))) 
    {
        //Sys_Print("\nDIB_Init() - SelectObject failed\n");
        return 0;
    }

    return 1;
}

int Com_ModifyFrameMsec(int frame_msec) 
{
    int clamped_msec = (int)(MSEC_PER_FRAME + MSEC_PER_FRAME);
    if (frame_msec > clamped_msec) {
        frame_msec = clamped_msec;
    }

    return frame_msec;
}

LRESULT WINAPI Win_MainWndProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam) 
{
    LRESULT result = 0;
    switch (msg) {
        case WM_SYSKEYDOWN:
            // fall through 
        case WM_KEYDOWN:
            //Sys_QueEvent(global_win_vars.sys_msg_time, SET_KEY, (int)wparam, true, 0, 0);
            break;

        case WM_SYSKEYUP:
            // fall through 
        case WM_KEYUP:
            //Sys_QueEvent(global_win_vars.sys_msg_time, SET_KEY, (int)wparam, false, 0, 0);
            break;

        case WM_CHAR:
            if (wparam == 'q') 
            {
                global_quit = 1;
            } 
            else 
            {
                //Sys_QueEvent(global_win_vars.sys_msg_time, SET_CHAR, (int)wparam, 0, 0, 0);
            } break;
        case WM_CLOSE:
            {
                global_quit = 1;
            }
            break;
        default:
            result = DefWindowProc(window, msg, wparam, lparam); 
            break;
    }

    return result;
}


#define WINDOW_CLASS_NAME "Fire"
int Win_InitWindow(void *wndproc, void *hinstance) 
{
    WNDCLASS wc = {0};

    int ex_style_bits = WS_EX_TOPMOST;

    HDC hdc = GetDC(GetDesktopWindow());
    int width = SCREEN_WIDTH;
    int height = SCREEN_HEIGHT;
    int style_bits = WS_OVERLAPPEDWINDOW;

    ReleaseDC(GetDesktopWindow(), hdc);

    wc.lpfnWndProc   = (WNDPROC)wndproc;
    wc.hInstance     = (HINSTANCE)hinstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_GRAYTEXT;
    wc.lpszClassName = WINDOW_CLASS_NAME;

    if (!RegisterClass (&wc)) 
    {
        return 0;
    }

    global_screen_handle.window = CreateWindowEx(ex_style_bits,
            WINDOW_CLASS_NAME,
            "Fire",
            style_bits,
            0, 0, width, height,
            0,
            0,
            (HINSTANCE)hinstance,
            0);
    global_screen_handle.width = width;
    global_screen_handle.height = height;

    if (!global_screen_handle.window) 
    {
        return 0;
    }

    ShowWindow(global_screen_handle.window, SW_SHOWNORMAL);
    UpdateWindow(global_screen_handle.window);
    SetForegroundWindow(global_screen_handle.window);
    SetFocus(global_screen_handle.window);

    return 1;
}

int Win_PumpEvents() 
{
    u32 time = 0;
    MSG msg;

    while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) 
    {
        if (!GetMessage(&msg, NULL, 0, 0)) 
        {
            global_quit = 1;
        }
        time = msg.time;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return time;
}

void Win_GenerateEvents() 
{
    u32 event_time = Win_PumpEvents();
}

void R_SwapBuffers(u32 min_x, u32 min_y, u32 max_x, u32 max_y) 
{
    BitBlt(global_screen_handle.hdc, min_x, min_y, max_x, max_y, global_screen_handle.hdc_dib_section, 0, 0, SRCCOPY);
}

void R_ClearFrameBuffer(byte *frame_buffer, u32 size) 
{
    memset(frame_buffer, 0, size);
}

void R_Render()
{
#define w SCREEN_WIDTH
#define h SCREEN_HEIGHT
    R_ClearFrameBuffer(global_screen_handle.buffer, global_screen_handle.pitch * global_screen_handle.height);

    StartFire();
    u32 *base = (u32 *)global_screen_handle.buffer;
    for(u32 i = 0; i < h; i++)
    {
        for(u32 j = 0; j < w; j++)
        {
            u32 pal = global_fire_buffer[i*w + j];
            u32 pal_pixel = global_palette[pal];

            base[i*w + j] = pal_pixel;
        }
    }
    R_SwapBuffers(0, 0, global_screen_handle.width, global_screen_handle.height);
#undef w
#undef h
}

int Sys_GetMilliseconds() 
{
    if (!global_sys_time_base)
    {
        global_sys_time_base = timeGetTime();
    }
    return timeGetTime() - global_sys_time_base;
}

void Sys_Sleep(DWORD ms) 
{
    Sleep(ms);
}

void Sys_Init() 
{
    timeBeginPeriod(1);
    srand((u32)time(NULL));
}

void Com_RunFrame()
{
    Win_PumpEvents();

    int prev_time = Sys_GetMilliseconds();
    int num_frames_to_run = 0;
    for(;;) 
    {
        const int current_frame_time = Sys_GetMilliseconds();
        if (!global_last_frame_time)
        {
            global_last_frame_time = current_frame_time;	
        }
        int delta_milli_seconds = current_frame_time - global_last_frame_time;
        global_last_frame_time = current_frame_time;

        delta_milli_seconds = Com_ModifyFrameMsec(delta_milli_seconds);

        global_time_residual += delta_milli_seconds;

        for(;;) 
        {
            // how much to wait before running the next frame
            if (global_time_residual < MSEC_PER_FRAME) 
            {		
                break;
            }
            global_time_residual -= MSEC_PER_FRAME;
            global_frame++;
            num_frames_to_run++;
        }

        if (num_frames_to_run > 0) 
        {
            // run the frames
            break;
        }
        Sys_Sleep(0);
    }

    int curr_time = Sys_GetMilliseconds();
    R_Render();
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prev_instance, LPSTR cmd_line, int cmd_show) 
{
    if (!Win_InitWindow((void *)Win_MainWndProc, hinstance) || !Win_InitDIB())
    {
        MessageBox(NULL, "Could not create a window", NULL, MB_OK|MB_ICONERROR);
        return 1;
    }

    Sys_Init();
    FillGlobalFirePal();
    for(;;)
    {
        if (global_quit)
        {
            break;
        }
        Com_RunFrame();
    }

    return 0;
}

