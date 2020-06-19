#include "platform_specific.h"
#include "util/agg_color_conv_rgb8.h"
#include "util/agg_color_conv_rgb16.h"

using namespace bzmag;
using namespace bzmag::engine;

//------------------------------------------------------------------------
void platform_specific::convert_pmap(rendering_buffer* dst,
    const rendering_buffer* src,
    pix_format_e format)
{
    switch (format)
    {
    case pix_format_gray8:
        break;

    case pix_format_gray16:
        color_conv(dst, src, color_conv_gray16_to_gray8());
        break;

    case pix_format_rgb565:
        color_conv(dst, src, color_conv_rgb565_to_rgb555());
        break;

    case pix_format_rgbAAA:
        color_conv(dst, src, color_conv_rgbAAA_to_bgr24());
        break;

    case pix_format_bgrAAA:
        color_conv(dst, src, color_conv_bgrAAA_to_bgr24());
        break;

    case pix_format_rgbBBA:
        color_conv(dst, src, color_conv_rgbBBA_to_bgr24());
        break;

    case pix_format_bgrABB:
        color_conv(dst, src, color_conv_bgrABB_to_bgr24());
        break;

    case pix_format_rgb24:
        color_conv(dst, src, color_conv_rgb24_to_bgr24());
        break;

    case pix_format_rgb48:
        color_conv(dst, src, color_conv_rgb48_to_bgr24());
        break;

    case pix_format_bgr48:
        color_conv(dst, src, color_conv_bgr48_to_bgr24());
        break;

    case pix_format_abgr32:
        color_conv(dst, src, color_conv_abgr32_to_bgra32());
        break;

    case pix_format_argb32:
        color_conv(dst, src, color_conv_argb32_to_bgra32());
        break;

    case pix_format_rgba32:
        color_conv(dst, src, color_conv_rgba32_to_bgra32());
        break;

    case pix_format_bgra64:
        color_conv(dst, src, color_conv_bgra64_to_bgra32());
        break;

    case pix_format_abgr64:
        color_conv(dst, src, color_conv_abgr64_to_bgra32());
        break;

    case pix_format_argb64:
        color_conv(dst, src, color_conv_argb64_to_bgra32());
        break;

    case pix_format_rgba64:
        color_conv(dst, src, color_conv_rgba64_to_bgra32());
        break;
    }
}

//------------------------------------------------------------------------
platform_specific::platform_specific(pix_format_e format, bool flip_y) :
    m_format(format),
    m_sys_format(pix_format_undefined),
    m_flip_y(flip_y),
    m_bpp(0),
    m_sys_bpp(0),
    m_hwnd(0),
    m_last_translated_key(0)
{
    memset(m_keymap, 0, sizeof(m_keymap));

    m_keymap[VK_PAUSE] = key_pause;
    m_keymap[VK_CLEAR] = key_clear;

    m_keymap[VK_NUMPAD0] = key_kp0;
    m_keymap[VK_NUMPAD1] = key_kp1;
    m_keymap[VK_NUMPAD2] = key_kp2;
    m_keymap[VK_NUMPAD3] = key_kp3;
    m_keymap[VK_NUMPAD4] = key_kp4;
    m_keymap[VK_NUMPAD5] = key_kp5;
    m_keymap[VK_NUMPAD6] = key_kp6;
    m_keymap[VK_NUMPAD7] = key_kp7;
    m_keymap[VK_NUMPAD8] = key_kp8;
    m_keymap[VK_NUMPAD9] = key_kp9;
    m_keymap[VK_DECIMAL] = key_kp_period;
    m_keymap[VK_DIVIDE] = key_kp_divide;
    m_keymap[VK_MULTIPLY] = key_kp_multiply;
    m_keymap[VK_SUBTRACT] = key_kp_minus;
    m_keymap[VK_ADD] = key_kp_plus;

    m_keymap[VK_UP] = key_up;
    m_keymap[VK_DOWN] = key_down;
    m_keymap[VK_RIGHT] = key_right;
    m_keymap[VK_LEFT] = key_left;
    m_keymap[VK_INSERT] = key_insert;
    m_keymap[VK_DELETE] = key_delete;
    m_keymap[VK_HOME] = key_home;
    m_keymap[VK_END] = key_end;
    m_keymap[VK_PRIOR] = key_page_up;
    m_keymap[VK_NEXT] = key_page_down;

    m_keymap[VK_F1] = key_f1;
    m_keymap[VK_F2] = key_f2;
    m_keymap[VK_F3] = key_f3;
    m_keymap[VK_F4] = key_f4;
    m_keymap[VK_F5] = key_f5;
    m_keymap[VK_F6] = key_f6;
    m_keymap[VK_F7] = key_f7;
    m_keymap[VK_F8] = key_f8;
    m_keymap[VK_F9] = key_f9;
    m_keymap[VK_F10] = key_f10;
    m_keymap[VK_F11] = key_f11;
    m_keymap[VK_F12] = key_f12;
    m_keymap[VK_F13] = key_f13;
    m_keymap[VK_F14] = key_f14;
    m_keymap[VK_F15] = key_f15;

    m_keymap[VK_NUMLOCK] = key_numlock;
    m_keymap[VK_CAPITAL] = key_capslock;
    m_keymap[VK_SCROLL] = key_scrollock;


    switch (m_format)
    {
    case pix_format_bw:
        m_sys_format = pix_format_bw;
        m_bpp = 1;
        m_sys_bpp = 1;
        break;

    case pix_format_gray8:
        m_sys_format = pix_format_gray8;
        m_bpp = 8;
        m_sys_bpp = 8;
        break;

    case pix_format_gray16:
        m_sys_format = pix_format_gray8;
        m_bpp = 16;
        m_sys_bpp = 8;
        break;

    case pix_format_rgb565:
    case pix_format_rgb555:
        m_sys_format = pix_format_rgb555;
        m_bpp = 16;
        m_sys_bpp = 16;
        break;

    case pix_format_rgbAAA:
    case pix_format_bgrAAA:
    case pix_format_rgbBBA:
    case pix_format_bgrABB:
        m_sys_format = pix_format_bgr24;
        m_bpp = 32;
        m_sys_bpp = 24;
        break;

    case pix_format_rgb24:
    case pix_format_bgr24:
        m_sys_format = pix_format_bgr24;
        m_bpp = 24;
        m_sys_bpp = 24;
        break;

    case pix_format_rgb48:
    case pix_format_bgr48:
        m_sys_format = pix_format_bgr24;
        m_bpp = 48;
        m_sys_bpp = 24;
        break;

    case pix_format_bgra32:
    case pix_format_abgr32:
    case pix_format_argb32:
    case pix_format_rgba32:
        m_sys_format = pix_format_bgra32;
        m_bpp = 32;
        m_sys_bpp = 32;
        break;

    case pix_format_bgra64:
    case pix_format_abgr64:
    case pix_format_argb64:
    case pix_format_rgba64:
        m_sys_format = pix_format_bgra32;
        m_bpp = 64;
        m_sys_bpp = 32;
        break;
    }
    ::QueryPerformanceFrequency(&m_sw_freq);
    ::QueryPerformanceCounter(&m_sw_start);
}


//------------------------------------------------------------------------
void platform_specific::create_pmap(unsigned width,
    unsigned height,
    rendering_buffer* wnd)
{
    m_pmap_window.create(width, height, org_e(m_bpp));
    wnd->attach(m_pmap_window.buf(),
        m_pmap_window.width(),
        m_pmap_window.height(),
        m_flip_y ?
        m_pmap_window.stride() :
        -m_pmap_window.stride());
}

//------------------------------------------------------------------------
void platform_specific::display_pmap(HDC dc, const rendering_buffer* src)
{
    if (m_sys_format == m_format)
    {
        m_pmap_window.draw(dc);
    }
    else
    {
        pixel_map pmap_tmp;
        pmap_tmp.create(m_pmap_window.width(),
            m_pmap_window.height(),
            org_e(m_sys_bpp));

        rendering_buffer rbuf_tmp;
        rbuf_tmp.attach(pmap_tmp.buf(),
            pmap_tmp.width(),
            pmap_tmp.height(),
            m_flip_y ?
            pmap_tmp.stride() :
            -pmap_tmp.stride());

        convert_pmap(&rbuf_tmp, src, m_format);
        pmap_tmp.draw(dc);
    }
}

//------------------------------------------------------------------------
bool platform_specific::save_pmap(const char* fn, unsigned idx,
    const rendering_buffer* src)
{
    if (m_sys_format == m_format)
    {
        return m_pmap_img[idx].save_as_bmp(fn);
    }

    pixel_map pmap_tmp;
    pmap_tmp.create(m_pmap_img[idx].width(),
        m_pmap_img[idx].height(),
        org_e(m_sys_bpp));

    rendering_buffer rbuf_tmp;
    rbuf_tmp.attach(pmap_tmp.buf(),
        pmap_tmp.width(),
        pmap_tmp.height(),
        m_flip_y ?
        pmap_tmp.stride() :
        -pmap_tmp.stride());

    convert_pmap(&rbuf_tmp, src, m_format);
    return pmap_tmp.save_as_bmp(fn);
}

//------------------------------------------------------------------------
bool platform_specific::load_pmap(const char* fn, unsigned idx,
    rendering_buffer* dst)
{
    pixel_map pmap_tmp;
    if (!pmap_tmp.load_from_bmp(fn)) return false;

    rendering_buffer rbuf_tmp;
    rbuf_tmp.attach(pmap_tmp.buf(),
        pmap_tmp.width(),
        pmap_tmp.height(),
        m_flip_y ?
        pmap_tmp.stride() :
        -pmap_tmp.stride());

    m_pmap_img[idx].create(pmap_tmp.width(),
        pmap_tmp.height(),
        org_e(m_bpp),
        0);

    dst->attach(m_pmap_img[idx].buf(),
        m_pmap_img[idx].width(),
        m_pmap_img[idx].height(),
        m_flip_y ?
        m_pmap_img[idx].stride() :
        -m_pmap_img[idx].stride());

    switch (m_format)
    {
    case pix_format_gray8:
        switch (pmap_tmp.bpp())
        {
            //case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_gray8()); break;
        case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_gray8()); break;
            //case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_gray8()); break;
        }
        break;

    case pix_format_gray16:
        switch (pmap_tmp.bpp())
        {
            //case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_gray16()); break;
        case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_gray16()); break;
            //case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_gray16()); break;
        }
        break;

    case pix_format_rgb555:
        switch (pmap_tmp.bpp())
        {
        case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_rgb555()); break;
        case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_rgb555()); break;
        case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgb555()); break;
        }
        break;

    case pix_format_rgb565:
        switch (pmap_tmp.bpp())
        {
        case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_rgb565()); break;
        case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_rgb565()); break;
        case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgb565()); break;
        }
        break;

    case pix_format_rgb24:
        switch (pmap_tmp.bpp())
        {
        case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_rgb24()); break;
        case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_rgb24()); break;
        case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgb24()); break;
        }
        break;

    case pix_format_bgr24:
        switch (pmap_tmp.bpp())
        {
        case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_bgr24()); break;
        case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_bgr24()); break;
        case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_bgr24()); break;
        }
        break;

    case pix_format_rgb48:
        switch (pmap_tmp.bpp())
        {
            //case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_rgb48()); break;
        case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_rgb48()); break;
            //case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgb48()); break;
        }
        break;

    case pix_format_bgr48:
        switch (pmap_tmp.bpp())
        {
            //case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_bgr48()); break;
        case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_bgr48()); break;
            //case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_bgr48()); break;
        }
        break;

    case pix_format_abgr32:
        switch (pmap_tmp.bpp())
        {
        case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_abgr32()); break;
        case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_abgr32()); break;
        case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_abgr32()); break;
        }
        break;

    case pix_format_argb32:
        switch (pmap_tmp.bpp())
        {
        case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_argb32()); break;
        case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_argb32()); break;
        case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_argb32()); break;
        }
        break;

    case pix_format_bgra32:
        switch (pmap_tmp.bpp())
        {
        case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_bgra32()); break;
        case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_bgra32()); break;
        case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_bgra32()); break;
        }
        break;

    case pix_format_rgba32:
        switch (pmap_tmp.bpp())
        {
        case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_rgba32()); break;
        case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_rgba32()); break;
        case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgba32()); break;
        }
        break;

    case pix_format_abgr64:
        switch (pmap_tmp.bpp())
        {
            //case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_abgr64()); break;
        case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_abgr64()); break;
            //case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_abgr64()); break;
        }
        break;

    case pix_format_argb64:
        switch (pmap_tmp.bpp())
        {
            //case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_argb64()); break;
        case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_argb64()); break;
            //case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_argb64()); break;
        }
        break;

    case pix_format_bgra64:
        switch (pmap_tmp.bpp())
        {
            //case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_bgra64()); break;
        case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_bgra64()); break;
            //case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_bgra64()); break;
        }
        break;

    case pix_format_rgba64:
        switch (pmap_tmp.bpp())
        {
            //case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_rgba64()); break;
        case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_rgba64()); break;
            //case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgba64()); break;
        }
        break;

    }

    return true;
}

//------------------------------------------------------------------------
unsigned int platform_specific::translate(unsigned keycode)
{
    return m_last_translated_key = (keycode > 255) ? 0 : m_keymap[keycode];
}
