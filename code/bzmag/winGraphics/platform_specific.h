#pragma once

#include <windows.h>
//#include <string.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_trans_viewport.h"
#include "platform/win32/agg_win32_bmp.h"


using namespace agg;

namespace bzmag
{
namespace engine
{
    //----------------------------------------------------------window_flag_e
    // These are flags used in method init(). Not all of them are
    // applicable on different platforms, for example the win32_api
    // cannot use a hardware buffer (window_hw_buffer).
    // The implementation should simply ignore unsupported flags.
    enum window_flag_e
    {
        window_resize = 1,
        window_hw_buffer = 2,
        window_keep_aspect_ratio = 4,
        window_process_all_keys = 8
    };

    //-----------------------------------------------------------pix_format_e
    // Possible formats of the rendering buffer. Initially I thought that it's
    // reasonable to create the buffer and the rendering functions in 
    // accordance with the native pixel format of the system because it 
    // would have no overhead for pixel format conversion. 
    // But eventually I came to a conclusion that having a possibility to 
    // convert pixel formats on demand is a good idea. First, it was X11 where 
    // there lots of different formats and visuals and it would be great to 
    // render everything in, say, RGB-24 and display it automatically without
    // any additional efforts. The second reason is to have a possibility to 
    // debug renderer for different pixel formats and colorspaces having only 
    // one computer and one system.
    //
    // This stuff is not included into the basic AGG functionality because the 
    // number of supported pixel formats (and/or colorspaces) can be great and 
    // if one needs to add new format it would be good only to add new 
    // rendering files without having to modify any existing ones (a general 
    // principle of encapsulation and isolation).
    //
    // Using a particular pixel format doesn't obligatory mean the necessity
    // of software conversion. For example, win32 API can natively display 
    // gray8, 15-bit RGB, 24-bit BGR, and 32-bit BGRA formats. 
    // This list can be (and will be!) extended in future.
    enum pix_format_e
    {
        pix_format_undefined = 0,  // By default. No conversions are applied 
        pix_format_bw,             // 1 bit per color B/W
        pix_format_gray8,          // Simple 256 level gray scale
        pix_format_gray16,         // Simple 65535 level gray scale
        pix_format_rgb555,         // 15 bit rgb. Depends on the byte ordering!
        pix_format_rgb565,         // 16 bit rgb. Depends on the byte ordering!
        pix_format_rgbAAA,         // 30 bit rgb. Depends on the byte ordering!
        pix_format_rgbBBA,         // 32 bit rgb. Depends on the byte ordering!
        pix_format_bgrAAA,         // 30 bit bgr. Depends on the byte ordering!
        pix_format_bgrABB,         // 32 bit bgr. Depends on the byte ordering!
        pix_format_rgb24,          // R-G-B, one byte per color component
        pix_format_bgr24,          // B-G-R, native win32 BMP format.
        pix_format_rgba32,         // R-G-B-A, one byte per color component
        pix_format_argb32,         // A-R-G-B, native MAC format
        pix_format_abgr32,         // A-B-G-R, one byte per color component
        pix_format_bgra32,         // B-G-R-A, native win32 BMP format
        pix_format_rgb48,          // R-G-B, 16 bits per color component
        pix_format_bgr48,          // B-G-R, native win32 BMP format.
        pix_format_rgba64,         // R-G-B-A, 16 bits byte per color component
        pix_format_argb64,         // A-R-G-B, native MAC format
        pix_format_abgr64,         // A-B-G-R, one byte per color component
        pix_format_bgra64,         // B-G-R-A, native win32 BMP format

        end_of_pix_formats
    };

    //-------------------------------------------------------------input_flag_e
    // Mouse and keyboard flags. They can be different on different platforms
    // and the ways they are obtained are also different. But in any case
    // the system dependent flags should be mapped into these ones. The meaning
    // of that is as follows. For example, if kbd_ctrl is set it means that the 
    // ctrl key is pressed and being held at the moment. They are also used in 
    // the overridden methods such as on_mouse_move(), on_mouse_button_down(),
    // on_mouse_button_dbl_click(), on_mouse_button_up(), on_key(). 
    // In the method on_mouse_button_up() the mouse flags have different
    // meaning. They mean that the respective button is being released, but
    // the meaning of the keyboard flags remains the same.
    // There's absolute minimal set of flags is used because they'll be most
    // probably supported on different platforms. Even the mouse_right flag
    // is restricted because Mac's mice have only one button, but AFAIK
    // it can be simulated with holding a special key on the keyboard.
    enum input_flag_e
    {
        mouse_left = 1,
        mouse_right = 2,
        kbd_shift = 4,
        kbd_ctrl = 8
    };

    //--------------------------------------------------------------key_code_e
    // Keyboard codes. There's also a restricted set of codes that are most 
    // probably supported on different platforms. Any platform dependent codes
    // should be converted into these ones. There're only those codes are
    // defined that cannot be represented as printable ASCII-characters. 
    // All printable ASCII-set can be used in a regular C/C++ manner: 
    // ' ', 'A', '0' '+' and so on.
    // Since the class is used for creating very simple demo-applications
    // we don't need very rich possibilities here, just basic ones. 
    // Actually the numeric key codes are taken from the SDL library, so,
    // the implementation of the SDL support does not require any mapping.
    enum key_code_e
    {
        // ASCII set. Should be supported everywhere
        key_backspace = 8,
        key_tab = 9,
        key_clear = 12,
        key_return = 13,
        key_pause = 19,
        key_escape = 27,

        // Keypad 
        key_delete = 127,
        key_kp0 = 256,
        key_kp1 = 257,
        key_kp2 = 258,
        key_kp3 = 259,
        key_kp4 = 260,
        key_kp5 = 261,
        key_kp6 = 262,
        key_kp7 = 263,
        key_kp8 = 264,
        key_kp9 = 265,
        key_kp_period = 266,
        key_kp_divide = 267,
        key_kp_multiply = 268,
        key_kp_minus = 269,
        key_kp_plus = 270,
        key_kp_enter = 271,
        key_kp_equals = 272,

        // Arrow-keys and stuff
        key_up = 273,
        key_down = 274,
        key_right = 275,
        key_left = 276,
        key_insert = 277,
        key_home = 278,
        key_end = 279,
        key_page_up = 280,
        key_page_down = 281,

        // Functional keys. You'd better avoid using
        // f11...f15 in your applications if you want 
        // the applications to be portable
        key_f1 = 282,
        key_f2 = 283,
        key_f3 = 284,
        key_f4 = 285,
        key_f5 = 286,
        key_f6 = 287,
        key_f7 = 288,
        key_f8 = 289,
        key_f9 = 290,
        key_f10 = 291,
        key_f11 = 292,
        key_f12 = 293,
        key_f13 = 294,
        key_f14 = 295,
        key_f15 = 296,

        // The possibility of using these keys is 
        // very restricted. Actually it's guaranteed 
        // only in win32_api and win32_sdl implementations
        key_numlock = 300,
        key_capslock = 301,
        key_scrollock = 302,

        // Phew!
        end_of_key_codes
    };

    enum max_images_e { max_images = 16 };

    //------------------------------------------------------------------------
    class platform_specific
    {
    public:
        platform_specific(pix_format_e format, bool flip_y);

        void create_pmap(unsigned width, unsigned height,
            rendering_buffer* wnd);

        void display_pmap(HDC dc, const rendering_buffer* src);
        bool load_pmap(const char* fn, unsigned idx,
            rendering_buffer* dst);

        bool save_pmap(const char* fn, unsigned idx,
            const rendering_buffer* src);

        unsigned translate(unsigned keycode);

        static void convert_pmap(rendering_buffer* dst,
            const rendering_buffer* src,
            pix_format_e format);

        pix_format_e  m_format;
        pix_format_e  m_sys_format;
        bool          m_flip_y;
        unsigned      m_bpp;
        unsigned      m_sys_bpp;
        HWND          m_hwnd;
        pixel_map     m_pmap_window;
        pixel_map     m_pmap_img[max_images];
        unsigned      m_keymap[256];
        unsigned      m_last_translated_key;
        LARGE_INTEGER m_sw_freq;
        LARGE_INTEGER m_sw_start;
    };
}
}