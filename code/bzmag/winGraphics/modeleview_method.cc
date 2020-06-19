#include "modelerview.h"

//#include "geomservernode.h"
#include "core/methodbinder.h"


using namespace bzmag;
using namespace bzmag::engine;

//-----------------------------------------------------------------------------
static void ModelerView_v_draw_v(ModelerView* self, Parameter* param)
{
    self->draw();
}

//-----------------------------------------------------------------------------
static void ModelerView_v_setWindowHandle_i(ModelerView* self, Parameter* param)
{
    self->set_window_handle(param->in()->get<int>(0));
}

//-----------------------------------------------------------------------------
static void ModelerView_i_getWindowHandle_v(ModelerView* self, Parameter* param)
{
    param->out()->get<int>(0) = self->get_window_handle();
}

//-----------------------------------------------------------------------------
static void ModelerView_d_zoomTo_ddd(ModelerView* self, Parameter* param)
{
    param->out()->get<double>(0) =
        self->zoom_to(
            param->in()->get<double>(0),
            param->in()->get<int>(1),
            param->in()->get<int>(2));
}

//-----------------------------------------------------------------------------
static void ModelerView_d_zoomToRectangle_dddd(ModelerView* self, Parameter* param)
{
    param->out()->get<double>(0) =
        self->zoom_to_rectangle(
            param->in()->get<int>(0),
            param->in()->get<int>(1),
            param->in()->get<int>(2),
            param->in()->get<int>(3));
}

//-----------------------------------------------------------------------------
static void ModelerView_v_fitWindow_v(ModelerView* self, Parameter* param)
{
    self->fit_to_window();
}

//-----------------------------------------------------------------------------
static void ModelerView_v_setWindowSize_ii(ModelerView* self, Parameter* param)
{
    self->set_window_size(
        param->in()->get<int>(0),
        param->in()->get<int>(1));
}

//-----------------------------------------------------------------------------
static void ModelerView_v_setModelRect_dddd(ModelerView* self, Parameter* param)
{
    self->set_model_rect(
        param->in()->get<double>(0),
        param->in()->get<double>(1),
        param->in()->get<double>(2),
        param->in()->get<double>(3));
}

//-----------------------------------------------------------------------------
static void ModelerView_b_setGeometryPath_s(ModelerView* self, Parameter* param)
{
    param->out()->get<bool>(0) =
        self->set_geometry_path(
            param->in()->get<String>(0));
}

//-----------------------------------------------------------------------------
static void ModelerView_v_OnSize_ii(ModelerView* self, Parameter* param)
{

}

//-----------------------------------------------------------------------------
static void ModelerView_v_OnMouseMove_iii(ModelerView* self, Parameter* param)
{

}

//-----------------------------------------------------------------------------
static void ModelerView_v_OnMouseLButtonDown_iii(ModelerView* self, Parameter* param)
{

}

//-----------------------------------------------------------------------------
static void ModelerView_v_OnMouseLButtonUp_iii(ModelerView* self, Parameter* param)
{

}

//-----------------------------------------------------------------------------
static void ModelerView_v_OnMouseRButtonDown_iii(ModelerView* self, Parameter* param)
{

}

//-----------------------------------------------------------------------------
static void ModelerView_v_OnMouseRButtonUp_iii(ModelerView* self, Parameter* param)
{

}

//-----------------------------------------------------------------------------
static void ModelerView_v_OnKeyDown_iiii(ModelerView* self, Parameter* param)
{

}

//-----------------------------------------------------------------------------
static void ModelerView_v_OnSysKeyDown_iiii(ModelerView* self, Parameter* param)
{

}

//-----------------------------------------------------------------------------
static void ModelerView_v_OnKeyUp_iiii(ModelerView* self, Parameter* param)
{

}

//-----------------------------------------------------------------------------
static void ModelerView_v_OnSysKeyUp_iiii(ModelerView* self, Parameter* param)

{

}

//-----------------------------------------------------------------------------
static void ModelerView_v_OnChar_iiii(ModelerView* self, Parameter* param)
{

}

//-----------------------------------------------------------------------------
static void ModelerView_v_OnSysChar_iiii(ModelerView* self, Parameter* param)
{

}


static void ModelerView_v_OnDraw_i(ModelerView* self, Parameter* param)
{

}

//-----------------------------------------------------------------------------
void ModelerView::bindMethod()
{
    BIND_METHOD(v_draw_v, ModelerView_v_draw_v);
    BIND_METHOD(v_setWindowHandle_i, ModelerView_v_setWindowHandle_i);
    BIND_METHOD(i_getWindowHandle_v, ModelerView_i_getWindowHandle_v);
    BIND_METHOD(d_zoomTo_ddd, ModelerView_d_zoomTo_ddd);
    BIND_METHOD(d_zoomToRectangle_dddd, ModelerView_d_zoomToRectangle_dddd);
    BIND_METHOD(v_fitWindow_v, ModelerView_v_fitWindow_v);
    BIND_METHOD(v_setWindowSize_ii, ModelerView_v_setWindowSize_ii);
    BIND_METHOD(v_setModelRect_dddd, ModelerView_v_setModelRect_dddd);
    BIND_METHOD(b_setGeometryPath_s, ModelerView_b_setGeometryPath_s);
}
