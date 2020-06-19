#pragma once

/*
Description : ModelerView Node based on Anit Grain Graphic library
Function : Rendering Geometry Nodes
Camera Zoom & Move
Last Update : 2016.04.20
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include <vector>
#include "core/node.h"
#include "core/singleton3.h"
#include "engine/geomheadnode.h"
#include "platform_specific.h"
#include "agg_renderer_base.h"
#include "agg_renderer_scanline.h"
#include "agg_scanline_p.h"

//#define AGG_GRAY16
//#define AGG_BGR24
//#define AGG_BGR48 
//#define AGG_RGB_AAA
//#define AGG_BGRA32
#define AGG_RGBA32 
//#define AGG_ARGB32 
//#define AGG_ABGR32
//#define AGG_RGB565
//#define AGG_RGB555
#include "pixel_formats.h"

using namespace agg;

namespace bzmag
{
namespace engine
{
    class ModelerView : public Node, public Singleton3<ModelerView>
    {
    public:
        typedef renderer_base<pixfmt> renderer_base;
        typedef renderer_scanline_aa_solid<renderer_base> renderer_solid;
        typedef agg::scanline_p8 scanline_type;
        typedef std::list<GeomHeadNode*> GeomHeadNodes;
        typedef GeomHeadNodes::iterator GeomHeadIterator;
        enum ZoomMode
        {
            ZoomOff,
            ZoomIn,
            ZoomOut,
            ZoomRect,
            Drag
        };

    public:
        ModelerView();
        virtual ~ModelerView();
        DECLARE_CLASS(ModelerView, Node);

        bool set_window_handle(int hwnd);
        int get_window_handle() const;
        bool set_geometry_path(const String& path);
        void draw();


        //--------------------------------------------------------------------
        // These 3 methods handle working with images. The image
        // formats are the simplest ones, such as .BMP in Windows or 
        // .ppm in Linux. In the applications the names of the files
        // should not have any file extensions. Method load_img() can
        // be called before init(), so, the application could be able 
        // to determine the initial size of the window depending on 
        // the size of the loaded image. 
        // The argument "idx" is the number of the image 0...max_images-1
        bool load_img(unsigned idx, const char* file);
        bool save_img(unsigned idx, const char* file);
        bool create_img(unsigned idx, unsigned width = 0, unsigned height = 0);

        //--------------------------------------------------------------------
        // The very same parameters that were used in the constructor
        pix_format_e format() const { return m_format; }
        bool flip_y() const { return m_flip_y; }
        unsigned bpp() const { return m_bpp; }

        //--------------------------------------------------------------------
        // The following provides a very simple mechanism of doing something
        // in background. It's not multi-threading. When wait_mode is true
        // the class waits for the events and it does not ever call on_idle().
        // When it's false it calls on_idle() when the event queue is empty.
        // The mode can be changed anytime. This mechanism is satisfactory
        // to create very simple animations.
        bool wait_mode() const { return m_wait_mode; }
        void wait_mode(bool wait_mode) { m_wait_mode = wait_mode; }

        //--------------------------------------------------------------------
        // These two functions control updating of the window. 
        // force_redraw() is an analog of the Win32 InvalidateRect() function.
        // Being called it sets a flag (or sends a message) which results
        // in calling on_draw() and updating the content of the window 
        // when the next event cycle comes.
        // update_window() results in just putting immediately the content 
        // of the currently rendered buffer to the window without calling
        // on_draw().
        void force_redraw();
        void update_window();

        //--------------------------------------------------------------------
        // So, finally, how to draw anything with AGG? Very simple.
        // rbuf_window() returns a reference to the main rendering 
        // buffer which can be attached to any rendering class.
        // rbuf_img() returns a reference to the previously created
        // or loaded image buffer (see load_img()). The image buffers 
        // are not displayed directly, they should be copied to or 
        // combined somehow with the rbuf_window(). rbuf_window() is
        // the only buffer that can be actually displayed.
        rendering_buffer& rbuf_window() { return m_rbuf_window; }
        rendering_buffer& rbuf_img(unsigned idx) { return m_rbuf_img[idx]; }


        //--------------------------------------------------------------------
        // Returns file extension used in the implementation for the particular
        // system.
        const char* img_ext() const;

        //--------------------------------------------------------------------
        void copy_img_to_window(unsigned idx)
        {
            if (idx < max_images && rbuf_img(idx).buf())
            {
                rbuf_window().copy_from(rbuf_img(idx));
            }
        }

        //--------------------------------------------------------------------
        void copy_window_to_img(unsigned idx)
        {
            if (idx < max_images)
            {
                create_img(idx, rbuf_window().width(), rbuf_window().height());
                rbuf_img(idx).copy_from(rbuf_window());
            }
        }

        //--------------------------------------------------------------------
        void copy_img_to_img(unsigned idx_to, unsigned idx_from)
        {
            if (idx_from < max_images &&
                idx_to < max_images &&
                rbuf_img(idx_from).buf())
            {
                create_img(idx_to,
                    rbuf_img(idx_from).width(),
                    rbuf_img(idx_from).height());
                rbuf_img(idx_to).copy_from(rbuf_img(idx_from));
            }
        }

        //--------------------------------------------------------------------
        // Event handlers. They are not pure functions, so you don't have
        // to override them all.
        // In my demo applications these functions are defined inside
        // the the_application class (implicit inlining) which is in general 
        // very bad practice, I mean virtual inline methods. At least it does
        // not make sense. 
        // But in this case it's quite appropriate because we have the only
        // instance of the the_application class and it is in the same file 
        // where this class is defined.
        virtual void on_size(int sx, int sy);
        virtual void on_mouse_move(int x, int y, unsigned flags);
        virtual void on_mouse_lbutton_down(int x, int y, unsigned flags);
        virtual void on_mouse_lbutton_up(int x, int y, unsigned flags);
        virtual void on_mouse_rbutton_down(int x, int y, unsigned flags);
        virtual void on_mouse_rbutton_up(int x, int y, unsigned flags);
        virtual void on_key_down(int x, int y, unsigned key, unsigned flags);
        virtual void on_syskey_down(int x, int y, unsigned key, unsigned flags);
        virtual void on_key_up(int x, int y, unsigned key, unsigned flags);
        virtual void on_syskey_up(int x, int y, unsigned key, unsigned flags);
        virtual void on_char(int x, int y, unsigned key, unsigned flags);
        virtual void on_syschar(int x, int y, unsigned key, unsigned flags);
        virtual void on_draw(int hDC);


        void     set_mouse_position(int x, int y, bool bInit = false);
        double   width()  const { return m_rbuf_window.width(); }
        double   height() const { return m_rbuf_window.height(); }

        //--------------------------------------------------------------------
        // Get raw display handler depending on the system. 
        // For win32 its an HDC, for other systems it can be a pointer to some
        // structure. See the implementation files for details.
        // It's provided "as is", so, first you should check if it's not null.
        // If it's null the raw_display_handler is not supported. Also, there's 
        // no guarantee that this function is implemented, so, in some 
        // implementations you may have simply an unresolved symbol when linking.
        void* raw_display_handler();

        //--------------------------------------------------------------------
        // display message box or print the message to the console 
        // (depending on implementation)
        void message(const char* msg);

        //--------------------------------------------------------------------
        // Stopwatch functions. Function elapsed_time() returns time elapsed 
        // since the latest start_timer() invocation in millisecods. 
        // The resolution depends on the implementation. 
        // In Win32 it uses QueryPerformanceFrequency() / QueryPerformanceCounter().
        void   start_timer();
        double elapsed_time() const;

        //--------------------------------------------------------------------
        // Get the full file name. In most cases it simply returns
        // file_name. As it's appropriate in many systems if you open
        // a file by its name without specifying the path, it tries to 
        // open it in the current directory. The demos usually expect 
        // all the supplementary files to be placed in the current 
        // directory, that is usually coincides with the directory where
        // the the executable is. However, in some systems (BeOS) it's not so. 
        // For those kinds of systems full_file_name() can help access files 
        // preserving commonly used policy.
        // So, it's a good idea to use in the demos the following:
        // FILE* fd = fopen(full_file_name("some.file"), "r"); 
        // instead of
        // FILE* fd = fopen("some.file", "r"); 
        const char* full_file_name(const char* file_name);


    public:
        // Set the zoom mode. Returns previous zoom mode.
        ZoomMode setZoomMode(ZoomMode newMode)
        {
            ZoomMode temp(m_zoomMode); m_zoomMode = newMode; return temp;
        }

        // Get the current zoom mode.
        ZoomMode getZoomMode() const { return m_zoomMode; }

        void set_model_rect(double left, double top, double right, double bottom);
        void set_window_size(int width, int height);

        // zoom in/out based on a point(cx,cy) which is clicked point on the screen
        double zoom_to(double zoom, int cx, int cy);

        // zoom in the rectangle area on the screen 
        double zoom_to_rectangle(int left, int top, int right, int bottom);
        void fit_to_window();

        // 
        bool is_tracking() const
        {
            return m_bTracking;
        }

        void clear_selection();

        size_t getNumberOfSelectedNode()
        {
            return m_selected.size();
        }

        GeomHeadIterator firstSelectedNode()
        {
            return m_selected.begin();
        }

        GeomHeadIterator lastSelectedNode()
        {
            return m_selected.end();
        }



    public:
        static void bindMethod();
        static void bindProperty();

    protected:
        // draw rubber band area (it work on ZoomOff mode)
        void draw_rubber_band(HDC hDC);
        void calculate();
        void on_select_node();

        //static void convert_path()


    private:
        platform_specific* m_specific;

        pix_format_e       m_format;
        unsigned           m_bpp;
        rendering_buffer   m_rbuf_window;
        rendering_buffer   m_rbuf_img[max_images];
        unsigned           m_input_flags;
        bool               m_wait_mode;
        bool               m_flip_y;
        char               m_caption[256];
        trans_affine       m_resize_mtx;

        ZoomMode           m_zoomMode;			// 줌 모드	
        bool               m_bTracking;			// 현재 트래킹 중인지?
        static const double m_maxZoom;
        double             m_zoom;
        double             m_fit;
        double             m_left;				// Model size (left, right, top, bottom)
        double             m_top;
        double             m_right;
        double             m_bottom;
        double             m_basex, m_basey;	// center of real model

                                                // 마우스 위치정보
        int                m_cur_x;
        int                m_cur_y;
        int                m_init_x;
        int                m_init_y;

        // 객체 선택 정보
        String             m_geom_path;
        GeomHeadNodes      m_selected;

        // mouse hit 시 객체와의 근접거리 허용 오차
        static double      m_tolerance;
    };
}
}
