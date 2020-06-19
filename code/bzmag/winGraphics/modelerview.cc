#include "modelerview.h"

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "core/kernel.h"
#include "core/exception.h"
#include "core/simplepropertybinder.h"
#include "core/exception.h"

#include "engine/geomheadnode.h"
#include "engine/geom_to_path.h"

#include "graphics_exception.h"
#include "platform_specific.h"

#include "agg/agg_path_storage.h"
#include "agg/agg_rasterizer_scanline_aa.h"
#include "agg_conv_stroke.h"

using namespace bzmag;
using namespace bzmag::engine;

IMPLEMENT_CLASS(ModelerView, Node);

//------------------------------------------------------------------------
const double ModelerView::m_maxZoom = 20.0;
double ModelerView::m_tolerance = 1.0;

//------------------------------------------------------------------------
ModelerView::ModelerView() :
    m_specific(new platform_specific(pix_format, true)),
    //	m_hwnd(0),
    m_format(pix_format),		// see enum pix_format_e {};
    m_flip_y(true),				// true if you want to have the Y-axis flipped vertically.
    m_bpp(m_specific->m_bpp),
    m_bTracking(false),
    m_left(-100),
    m_right(100),
    m_top(-100),
    m_bottom(100),
    m_basex(0),
    m_basey(0),
    m_wait_mode(true),
    m_zoomMode(ZoomOff),
    m_zoom(1.0f),
    m_fit(1.0f),
    m_cur_x(0),
    m_cur_y(0),
    m_init_x(0),
    m_init_y(0),
    m_geom_path("/geom")
{

}

//------------------------------------------------------------------------
ModelerView::~ModelerView()
{
    delete m_specific;
}

/*
//------------------------------------------------------------------------
bool ModelerView::init(int hwnd, double left, double top, double right, double bottom)
{
if(m_specific->m_sys_format == pix_format_undefined)
{
return false;
}
m_specific->m_hwnd = (HWND)hwnd;

if(m_specific->m_hwnd == 0)
{
return false;
}

m_left   = left;
m_right  = right;
m_top    = top;
m_bottom = bottom;

return true;
}
*/
/*
// ---------------------------------------------------------------------------- -
void ModelerView::setDisplayMode(const DisplayMode& display_mode)
{
m_display_mode = display_mode;
initialize_window(display_mode);
m_specific->m_hwnd = m_hwnd;
}


//-----------------------------------------------------------------------------
const DisplayMode& ModelerView::getDisplayMode() const
{
return m_display_mode;
}
*/


//------------------------------------------------------------------------
void ModelerView::start_timer()
{
    ::QueryPerformanceCounter(&(m_specific->m_sw_start));
}

//------------------------------------------------------------------------
double ModelerView::elapsed_time() const
{
    LARGE_INTEGER stop;
    ::QueryPerformanceCounter(&stop);
    return double(stop.QuadPart -
        m_specific->m_sw_start.QuadPart) * 1000.0 /
        double(m_specific->m_sw_freq.QuadPart);
}

//------------------------------------------------------------------------
static unsigned get_key_flags(int wflags)
{
    unsigned flags = 0;
    if (wflags & MK_LBUTTON) flags |= mouse_left;
    if (wflags & MK_RBUTTON) flags |= mouse_right;
    if (wflags & MK_SHIFT)   flags |= kbd_shift;
    if (wflags & MK_CONTROL) flags |= kbd_ctrl;
    return flags;
}

//------------------------------------------------------------------------
void* ModelerView::raw_display_handler()
{
    return ::GetDC(m_specific->m_hwnd);
}

//------------------------------------------------------------------------
void ModelerView::message(const char* msg)
{
    //::MessageBox(m_specific->m_hwnd, msg, L"Message", MB_OK);
}

//------------------------------------------------------------------------
const char* ModelerView::img_ext() const
{
    return ".bmp";
}

//------------------------------------------------------------------------
const char* ModelerView::full_file_name(const char* file_name)
{
    return file_name;
}

//------------------------------------------------------------------------
bool ModelerView::load_img(unsigned idx, const char* file)
{
    if (idx < max_images)
    {
        char fn[1024];
        strcpy(fn, file);
        int len = strlen(fn);
        if (len < 4 || _stricmp(fn + len - 4, ".BMP") != 0)
        {
            strcat(fn, ".bmp");
        }
        return m_specific->load_pmap(fn, idx, &m_rbuf_img[idx]);
    }
    return true;
}

//------------------------------------------------------------------------
bool ModelerView::save_img(unsigned idx, const char* file)
{
    if (idx < max_images)
    {
        char fn[1024];
        strcpy(fn, file);
        int len = strlen(fn);
        if (len < 4 || _stricmp(fn + len - 4, ".BMP") != 0)
        {
            strcat(fn, ".bmp");
        }
        return m_specific->save_pmap(fn, idx, &m_rbuf_img[idx]);
    }
    return true;
}

//------------------------------------------------------------------------
bool ModelerView::create_img(unsigned idx, unsigned width, unsigned height)
{
    if (idx < max_images)
    {
        if (width == 0) width = m_specific->m_pmap_window.width();
        if (height == 0) height = m_specific->m_pmap_window.height();
        m_specific->m_pmap_img[idx].create(width, height, org_e(m_specific->m_bpp));
        m_rbuf_img[idx].attach(m_specific->m_pmap_img[idx].buf(),
            m_specific->m_pmap_img[idx].width(),
            m_specific->m_pmap_img[idx].height(),
            m_flip_y ?
            m_specific->m_pmap_img[idx].stride() :
            -m_specific->m_pmap_img[idx].stride());
        return true;
    }
    return false;
}

//------------------------------------------------------------------------
void ModelerView::force_redraw()
{
    ::InvalidateRect(m_specific->m_hwnd, 0, FALSE);
}

//------------------------------------------------------------------------
void ModelerView::update_window()
{
    HDC dc = ::GetDC(m_specific->m_hwnd);
    m_specific->display_pmap(dc, &m_rbuf_window);
    ::ReleaseDC(m_specific->m_hwnd, dc);
}

//------------------------------------------------------------------------
void ModelerView::on_size(int sx, int sy)
{
    set_window_size(sx, sy);
}

//------------------------------------------------------------------------
void ModelerView::on_mouse_move(int x, int y, unsigned flags)
{
    set_mouse_position(x, y);
    m_input_flags = get_key_flags(flags);

    if (m_zoomMode == Drag && m_bTracking)
    {
        int del_x = m_init_x - m_cur_x;
        int del_y = m_init_y - m_cur_y;

        double del_xx = (double)del_x / (m_zoom * m_fit);
        double del_yy = (double)del_y / (m_zoom * m_fit);

        m_basex += del_xx;
        m_basey += del_yy;

        set_mouse_position(x, y, true);
    }
}

//------------------------------------------------------------------------
void ModelerView::on_mouse_lbutton_down(int x, int y, unsigned flags)
{
    ::SetCapture(m_specific->m_hwnd);
    set_mouse_position(x, y, true);

    m_input_flags = mouse_left | get_key_flags(flags);
    m_bTracking = true;


    // 줌모드 Off인 경우, 마우스 클릭시 객체 선택
    if (m_zoomMode == ModelerView::ZoomOff)
    {
        on_select_node();
    }
}

//------------------------------------------------------------------------
void ModelerView::on_mouse_rbutton_down(int x, int y, unsigned flags)
{
    ::SetCapture(m_specific->m_hwnd);
    set_mouse_position(x, y);
    m_input_flags = mouse_right | get_key_flags(flags);
}

//------------------------------------------------------------------------
void ModelerView::on_mouse_lbutton_up(int x, int y, unsigned flags)
{
    ::ReleaseCapture();
    set_mouse_position(x, y);
    m_input_flags = mouse_left | get_key_flags(flags);

    if (m_zoomMode == ZoomIn || m_zoomMode == ZoomOut || m_zoomMode == ZoomRect) {
        int left, right, top, bottom;
        left = (m_init_x < m_cur_x) ? m_init_x : m_cur_x;
        right = (m_init_x >= m_cur_x) ? m_init_x : m_cur_x;

        top = (m_init_y >= m_cur_y) ? m_init_y : m_cur_y;
        bottom = (m_init_y < m_cur_y) ? m_init_y : m_cur_y;

        zoom_to_rectangle(left, top, right, bottom);
    }

    set_mouse_position(0, 0, true);
    m_bTracking = false;
}

//------------------------------------------------------------------------
void ModelerView::on_mouse_rbutton_up(int x, int y, unsigned flags)
{
    ::ReleaseCapture();
    set_mouse_position(x, y);
    m_input_flags = mouse_right | get_key_flags(flags);
}

//------------------------------------------------------------------------
void ModelerView::on_key_down(int x, int y, unsigned key, unsigned flags)
{
    m_specific->m_last_translated_key = 0;
    switch (key)
    {
    case VK_CONTROL:
        m_input_flags |= kbd_ctrl;
        break;

    case VK_SHIFT:
        m_input_flags |= kbd_shift;
        break;

    default:
        m_specific->translate(flags);
        break;
    }

    if (m_specific->m_last_translated_key)
    {
        bool left = false;
        bool up = false;
        bool right = false;
        bool down = false;

        switch (m_specific->m_last_translated_key)
        {
        case key_left:
            left = true;
            break;

        case key_up:
            up = true;
            break;

        case key_right:
            right = true;
            break;

        case key_down:
            down = true;
            break;

        case key_f2:
            copy_window_to_img(max_images - 1);
            save_img(max_images - 1, "screenshot");
            break;
        }
    }
}

//------------------------------------------------------------------------
void ModelerView::on_syskey_down(int x, int y, unsigned key, unsigned flags)
{
    on_key_down(x, y, key, flags);
}

//------------------------------------------------------------------------
void ModelerView::on_key_up(int x, int y, unsigned key, unsigned flags)
{
    m_specific->m_last_translated_key = 0;
    switch (key)
    {
    case VK_CONTROL:
        m_input_flags &= ~kbd_ctrl;
        break;

    case VK_SHIFT:
        m_input_flags &= ~kbd_shift;
        break;
    }
}

//------------------------------------------------------------------------
void ModelerView::on_syskey_up(int x, int y, unsigned key, unsigned flags)
{
    on_key_up(x, y, key, flags);
}

//------------------------------------------------------------------------
void ModelerView::on_char(int x, int y, unsigned key, unsigned flags)
{
    if (m_specific->m_last_translated_key == 0)
    {

    }
}

//------------------------------------------------------------------------
void ModelerView::on_syschar(int x, int y, unsigned key, unsigned flags)
{
    on_char(x, y, key, flags);
}

//------------------------------------------------------------------------
void ModelerView::on_draw(int hDC)
{
    Kernel* kernel = Kernel::instance();
    Node* geom_root = kernel->lookup(m_geom_path);

    int width = rbuf_window().width();
    int height = rbuf_window().height();

    pixfmt pixf(rbuf_window());
    renderer_base rb(pixf);
    rb.clear(agg::rgba(1, 1, 1));

    renderer_solid r(rb);
    agg::rasterizer_scanline_aa<> ras;
    scanline_type sl;

    // 배경 초기화
    agg::render_scanlines(ras, sl, r);

    if (geom_root)
    {
        // 변환행렬 선언
        agg::trans_affine mtx;
        mtx *= agg::trans_affine_translation(-m_basex, -m_basey);       // 중심을 원점으로 이동
        mtx *= agg::trans_affine_scaling(m_fit*m_zoom);                 // 주밍
        mtx *= agg::trans_affine_translation(width / 2.0, height / 2.0);    // 타겟 좌표를 윈도우의 중심으로 이동

        Node::NodeIterator it;
        for (it = geom_root->firstChildNode(); it != geom_root->lastChildNode(); ++it)
        {
            Node* nn = *it;
            GeomHeadNode* head_node = dynamic_cast<GeomHeadNode*>(nn);
            if (!head_node) continue;
            if (head_node->isHide() || !head_node->isStandAlone()) continue;

            // 솔리드 그리기
            typedef agg::conv_transform<agg::path_storage, agg::trans_affine> path_transform;
            //GeomToPath nv(head_node);

            agg::path_storage path;
            GeomToPath::VertexList vertices;
            GeomToPath path_converter(head_node);
            path_converter.makePath(vertices);

            // vertices 를 path로 변환해야 함
            // 여기에 관련 코드 구현해야함 2019.05.10
            GeomToPath::VertexList::iterator vit;
            for (vit = vertices.begin(); vit!=vertices.end(); ++vit)
            {
                GeomToPath::VertexInfo info = *vit;
                if (info.cmd == 1) {
                    path.move_to(info.x, info.y);
                }
                else if (info.cmd == 2) {
                    path.line_to(info.x, info.y);
                }
            }

            path_transform trans(path, mtx);
            ras.add_path(trans);

            Color color = head_node->getColor();
            r.color(agg::rgba8(color.r_, color.g_, color.b_, color.a_));
            agg::render_scanlines(ras, sl, r);

            // 아웃라인 그리기
            agg::conv_stroke<path_transform> stroke(trans);
            stroke.width(1);
            ras.reset();
            ras.add_path(stroke);
            agg::render_scanlines(ras, sl, r);
        }
    }

    // 트래킹 경계라인 그리기
    if (m_bTracking) {
        draw_rubber_band((HDC)hDC);
    }

    m_specific->display_pmap((HDC)hDC, &rbuf_window());
}

//------------------------------------------------------------------------
void ModelerView::draw_rubber_band(HDC hDC)
{
    int left, right, top, bottom;
    left = m_init_x;
    right = m_cur_x;
    top = m_init_y;
    bottom = m_cur_y;

    agg::rasterizer_scanline_aa<> ras;
    scanline_type sl;
    pixfmt pixf(rbuf_window());
    renderer_base rb(pixf);
    renderer_solid r(rb);

    agg::path_storage path;

    path.move_to(left, top);
    path.line_to(right, top);
    path.line_to(right, bottom);
    path.line_to(left, bottom);
    path.close_polygon();


    agg::conv_stroke<agg::path_storage> stroke(path);
    stroke.width(1);

    ras.add_path(path);
    r.color(agg::rgba(0, 1, 1, 0.2));
    agg::render_scanlines(ras, sl, r);

    ras.reset();
    ras.add_path(stroke);
    r.color(agg::rgba(0, 0, 0));
    //agg::render_scanlines(ras, sl, r);

}

//------------------------------------------------------------------------
bool ModelerView::set_window_handle(int hwnd)
{
    if (m_specific->m_sys_format == pix_format_undefined)
    {
        return false;
    }
    m_specific->m_hwnd = (HWND)hwnd;

    if (m_specific->m_hwnd == 0)
    {
        return false;
    }

    return true;
}

//------------------------------------------------------------------------
int ModelerView::get_window_handle() const
{
    return (int)m_specific->m_hwnd;
}

//------------------------------------------------------------------------
bool ModelerView::set_geometry_path(const String& path)
{
    m_geom_path = path;
    return true;
}

//------------------------------------------------------------------------
void ModelerView::set_mouse_position(int x, int y, bool bInit)
{
    if (bInit) {
        m_init_x = x;
        m_init_y = flip_y() ? rbuf_window().height() - y : y;
    }
    m_cur_x = x;
    m_cur_y = flip_y() ? rbuf_window().height() - y : y;
}

//------------------------------------------------------------------------
void ModelerView::set_model_rect(double left, double top, double right, double bottom)
{
    m_left = left;
    m_top = top;
    m_right = right;
    m_bottom = bottom;
    calculate();
}

void ModelerView::set_window_size(int width, int height)
{
    m_specific->create_pmap(width, height, &rbuf_window());
    calculate();
    force_redraw();
}

//------------------------------------------------------------------------
void ModelerView::fit_to_window()
{
    m_zoom = 1.0f;
    m_basex = (m_left + m_right) / 2.0;
    m_basey = (m_top + m_bottom) / 2.0;
}

//------------------------------------------------------------------------
double ModelerView::zoom_to(double zoom, int cx, int cy)
{
    if (zoom > m_maxZoom) zoom = m_maxZoom;

    int width = rbuf_window().width();
    int height = rbuf_window().height();

    double prev_zoom = m_zoom;
    m_zoom = zoom;
    m_basex += (cx - width / 2.0) / (prev_zoom*m_fit);
    m_basey -= (cy - height / 2.0) / (prev_zoom*m_fit);
    return prev_zoom;
}

//------------------------------------------------------------------------
double ModelerView::zoom_to_rectangle(int left, int top, int right, int bottom)
{
    int width = rbuf_window().width();
    int height = rbuf_window().height();

    top = flip_y() ? rbuf_window().height() - top : top;
    bottom = flip_y() ? rbuf_window().height() - bottom : bottom;

    pixfmt pixf(rbuf_window());
    renderer_base rb(pixf);
    renderer_solid r(rb);

    // 선택영역의 사이즈(윈도우 좌표)
    double rwidth = abs(right - left);
    double rheight = abs(bottom - top);

    // 선택영역의 중심(윈도우 좌표)
    double rcx = (left + right) / 2.0;
    double rcy = (top + bottom) / 2.0;

    // 줌 비 계산
    double w_ratio = width / rwidth;
    double h_ratio = height / rheight;
    double zoom_ratio = __min(w_ratio, h_ratio);
    double zoom = m_zoom * zoom_ratio;

    // 줌
    return zoom_to(zoom, (int)rcx, (int)rcy);
}

//------------------------------------------------------------------------
void ModelerView::calculate()
{
    int width = rbuf_window().width();
    int height = rbuf_window().height();

    double width_mid = m_right - m_left;
    double heigh_mid = m_bottom - m_top;
    double w_fit = width / width_mid;
    double h_fit = height / heigh_mid;
    m_fit = __min(abs(w_fit), abs(h_fit));
}

//------------------------------------------------------------------------
void ModelerView::clear_selection()
{
    m_selected.clear();
}

//------------------------------------------------------------------------
void ModelerView::on_select_node()
{

}

//------------------------------------------------------------------------
void ModelerView::draw()
{
    HDC dc = ::GetDC(m_specific->m_hwnd);
    on_draw((int)dc);
}

//------------------------------------------------------------------------
void ModelerView::bindProperty()
{

}