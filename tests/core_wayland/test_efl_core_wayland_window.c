#include "efl_core_wayland_suite.h"
#include "efl_core_wayland_tests_helpers.h"

#ifdef GL_GLES
#  include "efl_core_wayland_tests_helper_egl.h"
#endif

static struct wl_surface *
_surface_get(Ecore_Wl_Window *win)
{
    return ecore_wl_window_surface_get(win);
}

EFL_START_TEST(core_wayland_window_new)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_surface_test)
{
    Ecore_Wl_Display  *disp;
    Ecore_Wl_Window   *win;
    struct wl_surface *surf;
    int                id = -1;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    surf = ecore_wl_window_surface_get(win);
    ck_assert(surf != NULL);

    id = ecore_wl_window_surface_id_get(win);
    ck_assert_int_gt(id, 0);

    ck_assert_int_eq(wl_proxy_get_id((struct wl_proxy *)surf), id);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_rotation)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    int               rot = -1;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    rot = ecore_wl_window_rotation_get(win);
    ck_assert_int_ge(rot, 0);

    ecore_wl_window_rotation_set(win, 90);

    rot = ecore_wl_window_rotation_get(win);
    fail_if(rot != 90);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_display_get)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ck_assert(ecore_wl_window_display_get(win) != NULL);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_alpha)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    Efl_Bool          alpha = EFL_FALSE;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ecore_wl_window_alpha_set(win, EFL_TRUE);

    alpha = ecore_wl_window_alpha_get(win);
    fail_if(alpha != EFL_TRUE);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_floating_mode)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    Efl_Bool          f = EFL_FALSE;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ecore_wl_window_floating_mode_set(win, EFL_TRUE);

    f = ecore_wl_window_floating_mode_get(win);
    fail_if(f != EFL_TRUE);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_focus_skip)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    Efl_Bool          skip = EFL_FALSE;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ecore_wl_window_focus_skip_set(win, EFL_TRUE);

    skip = ecore_wl_window_focus_skip_get(win);
    fail_if(skip != EFL_TRUE);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_fullscreen)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    Efl_Bool          full = EFL_FALSE;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ecore_wl_window_fullscreen_set(win, EFL_TRUE);

    full = ecore_wl_window_fullscreen_get(win);
    fail_if(full != EFL_TRUE);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_maximize)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    Efl_Bool          m = EFL_FALSE;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ecore_wl_window_maximized_set(win, EFL_TRUE);

    m = ecore_wl_window_maximized_get(win);
    fail_if(m != EFL_TRUE);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_preferred_rot)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    int               rot = 0;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ecore_wl_window_preferred_rotation_set(win, 90);

    rot = ecore_wl_window_preferred_rotation_get(win);
    fail_if(rot != 90);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_rotation_app)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    Efl_Bool          r = EFL_FALSE;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ecore_wl_window_rotation_app_set(win, EFL_TRUE);

    r = ecore_wl_window_rotation_app_get(win);
    fail_if(r != EFL_TRUE);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_wm_window_rotation_app)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    Efl_Bool          r = EFL_FALSE;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ecore_wl_window_wm_rotation_supported_set(win, EFL_TRUE);

    r = ecore_wl_window_wm_rotation_supported_get(win);
    fail_if(r != EFL_TRUE);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_geometry)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    int               x, y, w, h;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ecore_wl_window_geometry_set(win, 10, 10, 100, 100);

    ecore_wl_window_geometry_get(win, &x, &y, &w, &h);

    fail_if(x != 10);
    fail_if(y != 10);
    fail_if(w != 100);
    fail_if(h != 100);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_type)
{
    Ecore_Wl_Display    *disp;
    Ecore_Wl_Window     *win;
    Ecore_Wl_Window_Type type = ECORE_WL2_WINDOW_TYPE_NONE;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ecore_wl_window_type_set(win, ECORE_WL2_WINDOW_TYPE_TOPLEVEL);

    type = ecore_wl_window_type_get(win);
    fail_if(type != ECORE_WL2_WINDOW_TYPE_TOPLEVEL);
}

EFL_END_TEST

#ifdef GL_GLES
static void
_test_activated_frame_cb(Ecore_Wl_Window *win EFL_UNUSED,
                         uint32_t timestamp   EFL_UNUSED,
                         void                *data)
{
    Test_Data *td = data;

    td->frame_callback_count++;
    if (td->frame_callback_count % 4 == 0) glClearColor(0.0, 1.0, 0.0, 0.0);
    else if (td->frame_callback_count % 4 == 1)
        glClearColor(0.0, 0.0, 1.0, 0.0);
    else if (td->frame_callback_count % 4 == 2)
        glClearColor(0.0, 0.0, 0.0, 1.0);
    else glClearColor(1.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    eglSwapBuffers(td->egl_display, td->egl_surface);

    ecore_wl_window_commit(td->win, EFL_TRUE);
}

static Efl_Bool
_test_activated_configure_complete(void       *data,
                                   int type    EFL_UNUSED,
                                   void *event EFL_UNUSED)
{
    Test_Data *td = data;

    td->frame_callback_handler =
        ecore_wl_window_frame_callback_add(td->win,
                                           _test_activated_frame_cb,
                                           td);
    ecore_wl_window_commit(td->win, EFL_TRUE);

    return CORE_CALLBACK_PASS_ON;
}

static Efl_Bool
_test_activated_window_activate(void *data  EFL_UNUSED,
                                int type    EFL_UNUSED,
                                void *event EFL_UNUSED)
{
   //TC Pass
    core_main_loop_quit();

    return CORE_CALLBACK_PASS_ON;
}

EFL_START_TEST(core_wayland_window_activated)
{
    Test_Data *td;
    Efl_Bool   ret = EFL_FALSE;

    ecore_wl_init();

    td                       = calloc(1, sizeof(Test_Data));
    td->width                = WIDTH;
    td->height               = HEIGHT;
    td->frame_callback_count = 0;

    td->display = _display_connect();
    ck_assert(td->display != NULL);

    td->win = _window_create(td->display);
    ck_assert(td->win != NULL);

    ecore_wl_window_type_set(td->win, ECORE_WL2_WINDOW_TYPE_TOPLEVEL);

    td->surface = _surface_get(td->win);
    ck_assert(td->surface != NULL);

    ecore_wl_window_show(td->win);

    ret = _init_egl(td);
    fail_if(ret != EFL_TRUE);

    td->handler =
        core_event_handler_add(ECORE_WL2_EVENT_WINDOW_CONFIGURE_COMPLETE,
                               _test_activated_configure_complete,
                               td);
    core_event_handler_add(ECORE_WL2_EVENT_WINDOW_ACTIVATE,
                           _test_activated_window_activate,
                           NULL);

    core_main_loop_begin();

    _term_egl(td);
    ecore_wl_shutdown();
    free(td);
}

EFL_END_TEST
#else
EFL_START_TEST(core_wayland_window_activated)
{
    fail_if("No GL enabled GL should be enabled for API test");
}

EFL_END_TEST
#endif

EFL_START_TEST(core_wayland_window_aspect)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    int               w, h;
    unsigned int      aspect;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ecore_wl_window_aspect_set(win, 1, 1, 3);
    ecore_wl_window_aspect_get(win, &w, &h, &aspect);

    fail_if(w != 1);
    fail_if(h != 1);
    fail_if(aspect != 3);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_title)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    const char       *title;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ecore_wl_window_title_set(win, "TEST");
    title = ecore_wl_window_title_get(win);

    fail_if(strcmp(title, "TEST"));
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_class)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    const char *class;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ecore_wl_window_class_set(win, "TEST");
    class = ecore_wl_window_class_get(win);

    fail_if(strcmp(class, "TEST"));
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_available_rotation)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    Efl_Bool          ret;
    int               rots[2] = { 90, 180 };
    int              *ret_rots;
    unsigned int      ret_count;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ecore_wl_window_available_rotations_set(win, rots, 2);

    ret = ecore_wl_window_available_rotations_get(win, &ret_rots, &ret_count);

    fail_if(ret != EFL_TRUE);
    fail_if(ret_rots[0] != 90);
    fail_if(ret_rots[1] != 180);
    fail_if(ret_count != 2);

    free(ret_rots);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_role)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    const char       *role;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ecore_wl_window_role_set(win, "TEST");
    role = ecore_wl_window_role_get(win);

    fail_if(strcmp(role, "TEST"));
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_input_region)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    int               x, y, w, h;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ecore_wl_window_input_region_set(win, 10, 10, 100, 100);

    ecore_wl_window_input_region_get(win, &x, &y, &w, &h);
    fail_if(x != 10);
    fail_if(y != 10);
    fail_if(w != 100);
    fail_if(h != 100);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_opaque_region)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    int               x, y, w, h;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ecore_wl_window_opaque_region_set(win, 10, 10, 100, 100);

    ecore_wl_window_opaque_region_get(win, &x, &y, &w, &h);
    fail_if(x != 10);
    fail_if(y != 10);
    fail_if(w != 100);
    fail_if(h != 100);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_popup_input)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    Ecore_Wl_Input   *input;
    Eina_Iterator    *itr;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ecore_wl_window_type_set(win, ECORE_WL2_WINDOW_TYPE_MENU);

    itr = ecore_wl_display_inputs_get(disp);
    ck_assert(itr != NULL);

    EINA_ITERATOR_FOREACH(itr, input)
    {
        if (ecore_wl_input_seat_capabilities_get(input) !=
            ECORE_WL2_SEAT_CAPABILITIES_POINTER)
            continue;

        ecore_wl_window_popup_input_set(win, input);
        fail_if(ecore_wl_window_popup_input_get(win) != input);
        break;
    }

    eina_iterator_free(itr);
}

EFL_END_TEST

static void
_test_commit_frame_cb(Ecore_Wl_Window *win EFL_UNUSED,
                      uint32_t timestamp   EFL_UNUSED,
                      void                *data)
{
    Test_Data *td = data;
    td->frame_callback_count++;
    core_main_loop_quit();
}

static Efl_Bool
_test_commit_configure_complete(void       *data,
                                int type    EFL_UNUSED,
                                void *event EFL_UNUSED)
{
    Test_Data *td = data;

    td->frame_callback_handler =
        ecore_wl_window_frame_callback_add(td->win, _test_commit_frame_cb, td);
    ecore_wl_window_commit(td->win, EFL_TRUE);

    return CORE_CALLBACK_PASS_ON;
}

EFL_START_TEST(core_wayland_window_commit)
{
    Test_Data *td;

    ecore_wl_init();

    td                       = calloc(1, sizeof(Test_Data));
    td->width                = WIDTH;
    td->height               = HEIGHT;
    td->frame_callback_count = 0;

    td->display = _display_connect();
    ck_assert(td->display != NULL);

    td->win = _window_create(td->display);
    ck_assert(td->win != NULL);

    ecore_wl_window_type_set(td->win, ECORE_WL2_WINDOW_TYPE_TOPLEVEL);

    td->surface = _surface_get(td->win);
    ck_assert(td->surface != NULL);

    ecore_wl_window_show(td->win);

    td->handler =
        core_event_handler_add(ECORE_WL2_EVENT_WINDOW_CONFIGURE_COMPLETE,
                               _test_commit_configure_complete,
                               td);

    core_main_loop_begin();

    //Check if the frame callback was called properly by ecore_wl_window_commit().
    fail_if(td->frame_callback_count == 0);

    ecore_wl_shutdown();
    free(td);
}

EFL_END_TEST

static void
_test_frame_callback_frame_cb(Ecore_Wl_Window *win EFL_UNUSED,
                              uint32_t timestamp   EFL_UNUSED,
                              void                *data)
{
    Test_Data *td = data;
    td->frame_callback_count++;
    if (td->frame_callback_count == 1)
    {
        ecore_wl_window_frame_callback_del(td->frame_callback_handler);
        td->frame_callback_handler = NULL;
        core_main_loop_quit();
    }
}

static Efl_Bool
_test_frame_callback_configure_complete(void       *data,
                                        int type    EFL_UNUSED,
                                        void *event EFL_UNUSED)
{
    Test_Data *td = data;

    td->frame_callback_handler =
        ecore_wl_window_frame_callback_add(td->win,
                                           _test_frame_callback_frame_cb,
                                           td);
    ecore_wl_window_commit(td->win, EFL_TRUE);

    return CORE_CALLBACK_PASS_ON;
}

EFL_START_TEST(core_wayland_window_frame_callback)
{
    Test_Data *td;

    ecore_wl_init();

    td                       = calloc(1, sizeof(Test_Data));
    td->width                = WIDTH;
    td->height               = HEIGHT;
    td->frame_callback_count = 0;

    td->display = _display_connect();
    ck_assert(td->display != NULL);

    td->win = _window_create(td->display);
    ck_assert(td->win != NULL);

    ecore_wl_window_type_set(td->win, ECORE_WL2_WINDOW_TYPE_TOPLEVEL);

    td->surface = _surface_get(td->win);
    ck_assert(td->surface != NULL);

    ecore_wl_window_show(td->win);

    td->handler =
        core_event_handler_add(ECORE_WL2_EVENT_WINDOW_CONFIGURE_COMPLETE,
                               _test_frame_callback_configure_complete,
                               td);

    core_main_loop_begin();

    //Check if the frame callback called after then it sets NULL or not.
    fail_if(td->frame_callback_count != 1);
    fail_if(td->frame_callback_handler != NULL);

    ecore_wl_shutdown();
    free(td);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_free)
{
    Ecore_Wl_Window *t_win;
    Test_Data       *td;

    ecore_wl_init();

    td = calloc(1, sizeof(Test_Data));

    td->display = _display_connect();
    ck_assert(td->display != NULL);

    td->win = _window_create(td->display);
    ck_assert(td->win != NULL);

    ecore_wl_window_type_set(td->win, ECORE_WL2_WINDOW_TYPE_TOPLEVEL);

    td->surface = _surface_get(td->win);
    ck_assert(td->surface != NULL);

    ecore_wl_window_show(td->win);

    t_win = ecore_wl_display_window_find_by_surface(td->display, td->surface);

    fail_if(td->win != t_win);

    ecore_wl_window_free(td->win);
    t_win = ecore_wl_display_window_find_by_surface(td->display, td->surface);

    //Check the returned window with freed window.
    fail_if(td->win == t_win);

    ecore_wl_shutdown();
    free(td);
}

EFL_END_TEST

static Efl_Bool
_test_hide_window_hide(void *data  EFL_UNUSED,
                       int type    EFL_UNUSED,
                       void *event EFL_UNUSED)
{
    //TC pass
    core_main_loop_quit();

    return CORE_CALLBACK_PASS_ON;
}

EFL_START_TEST(core_wayland_window_hide)
{
    Test_Data *td;

    ecore_wl_init();

    td                       = calloc(1, sizeof(Test_Data));
    td->width                = WIDTH;
    td->height               = HEIGHT;
    td->frame_callback_count = 0;

    td->display = _display_connect();
    ck_assert(td->display != NULL);

    td->win = _window_create(td->display);
    ck_assert(td->win != NULL);

    ecore_wl_window_type_set(td->win, ECORE_WL2_WINDOW_TYPE_TOPLEVEL);

    td->surface = _surface_get(td->win);
    ck_assert(td->surface != NULL);

    ecore_wl_window_show(td->win);

    ecore_wl_window_hide(td->win);
    td->handler = core_event_handler_add(ECORE_WL2_EVENT_WINDOW_HIDE,
                                         _test_hide_window_hide,
                                         NULL);

    core_main_loop_begin();

    ecore_wl_shutdown();
    free(td);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_shell_surface_exists)
{
    Test_Data *td;

    ecore_wl_init();

    td                       = calloc(1, sizeof(Test_Data));
    td->width                = WIDTH;
    td->height               = HEIGHT;
    td->frame_callback_count = 0;

    td->display = _display_connect();
    ck_assert(td->display != NULL);

    td->win = _window_create(td->display);
    ck_assert(td->win != NULL);

    ecore_wl_window_type_set(td->win, ECORE_WL2_WINDOW_TYPE_TOPLEVEL);

    td->surface = _surface_get(td->win);
    ck_assert(td->surface != NULL);

    ecore_wl_window_show(td->win);

    //window_show function will create shell surface. then checks it.
    fail_if(ecore_wl_window_shell_surface_exists(td->win) == EFL_FALSE);

    ecore_wl_shutdown();
    free(td);
}

EFL_END_TEST

static Efl_Bool
_test_show_window_show(void *data  EFL_UNUSED,
                       int type    EFL_UNUSED,
                       void *event EFL_UNUSED)
{
    //TC pass
    core_main_loop_quit();

    return CORE_CALLBACK_PASS_ON;
}

EFL_START_TEST(core_wayland_window_show)
{
    Test_Data *td;

    ecore_wl_init();

    td                       = calloc(1, sizeof(Test_Data));
    td->width                = WIDTH;
    td->height               = HEIGHT;
    td->frame_callback_count = 0;

    td->display = _display_connect();
    ck_assert(td->display != NULL);

    td->win = _window_create(td->display);
    ck_assert(td->win != NULL);

    ecore_wl_window_type_set(td->win, ECORE_WL2_WINDOW_TYPE_TOPLEVEL);

    td->surface = _surface_get(td->win);
    ck_assert(td->surface != NULL);

    ecore_wl_window_show(td->win);

    td->handler = core_event_handler_add(ECORE_WL2_EVENT_WINDOW_SHOW,
                                         _test_show_window_show,
                                         NULL);

    core_main_loop_begin();

    ecore_wl_shutdown();
    free(td);
}

EFL_END_TEST

static Efl_Bool _window_configure_event_called = EFL_FALSE;

static Efl_Bool
_test_update_window_configure(void *data  EFL_UNUSED,
                              int type    EFL_UNUSED,
                              void *event EFL_UNUSED)
{
    _window_configure_event_called = EFL_TRUE;

    return CORE_CALLBACK_PASS_ON;
}

static Efl_Bool
_test_update_window_configure_complete(void *data  EFL_UNUSED,
                                       int type    EFL_UNUSED,
                                       void *event EFL_UNUSED)
{
    //Checks if the configure_complete calling before configure calling
    //when ecore_wl_window_update_begin() called.
    fail_if(_window_configure_event_called == EFL_TRUE);

    core_main_loop_quit();

    return CORE_CALLBACK_PASS_ON;
}

EFL_START_TEST(core_wayland_window_update_begin)
{
    Test_Data *td;

    ecore_wl_init();

    td                       = calloc(1, sizeof(Test_Data));
    td->width                = WIDTH;
    td->height               = HEIGHT;
    td->frame_callback_count = 0;

    td->display = _display_connect();
    ck_assert(td->display != NULL);

    td->win = _window_create(td->display);
    ck_assert(td->win != NULL);

    ecore_wl_window_type_set(td->win, ECORE_WL2_WINDOW_TYPE_TOPLEVEL);

    td->surface = _surface_get(td->win);
    ck_assert(td->surface != NULL);

    ecore_wl_window_show(td->win);

    ecore_wl_window_update_begin(td->win);
    core_event_handler_add(ECORE_WL2_EVENT_WINDOW_CONFIGURE,
                           _test_update_window_configure,
                           NULL);
    core_event_handler_add(ECORE_WL2_EVENT_WINDOW_CONFIGURE_COMPLETE,
                           _test_update_window_configure_complete,
                           NULL);

    core_main_loop_begin();

    ecore_wl_shutdown();
    free(td);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_move)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    //FIXME: Need some discussion about how to validate this API in TC.
    ecore_wl_window_move(NULL, NULL);
    ecore_wl_window_move(win, NULL);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_resize)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    //FIXME: Need some discussion about how to validate this API in TC.
    ecore_wl_window_resize(NULL, NULL, 0);
    ecore_wl_window_resize(win, NULL, 0);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_resizing_get)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    Efl_Bool          ret;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    ret = ecore_wl_window_resizing_get(win);
    fail_if(ret == EFL_TRUE);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_window_output_find)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Window  *win;
    Ecore_Wl_Output  *output;

    disp = _display_connect();
    ck_assert(disp != NULL);

    win = _window_create(disp);
    ck_assert(win != NULL);

    //FIXME: Need some discussion about how to validate this API in TC.
    output = ecore_wl_window_output_find(win);
    output = ecore_wl_window_output_find(NULL);
    fail_if(output != NULL);
}

EFL_END_TEST

void
test_window(TCase *tc)
{
    if (getenv("WAYLAND_DISPLAY"))
    {
        /* window tests can only run if there is an existing compositor */
        tcase_add_test(tc, core_wayland_window_new);
        tcase_add_test(tc, core_wayland_window_surface_test);
        tcase_add_test(tc, core_wayland_window_rotation);
        if (getenv("E_START"))
        {
            tcase_add_test(tc, core_wayland_window_commit);
            tcase_add_test(tc, core_wayland_window_frame_callback);
            tcase_add_test(tc, core_wayland_window_free);
            tcase_add_test(tc, core_wayland_window_hide);
            tcase_add_test(tc, core_wayland_window_shell_surface_exists);
            tcase_add_test(tc, core_wayland_window_show);
            tcase_add_test(tc, core_wayland_window_update_begin);
            tcase_add_test(tc, core_wayland_window_activated);
        }
        tcase_add_test(tc, core_wayland_window_display_get);
        tcase_add_test(tc, core_wayland_window_alpha);
        tcase_add_test(tc, core_wayland_window_floating_mode);
        tcase_add_test(tc, core_wayland_window_focus_skip);
        tcase_add_test(tc, core_wayland_window_fullscreen);
        tcase_add_test(tc, core_wayland_window_maximize);
        tcase_add_test(tc, core_wayland_window_preferred_rot);
        tcase_add_test(tc, core_wayland_window_rotation_app);
        tcase_add_test(tc, core_wayland_wm_window_rotation_app);
        tcase_add_test(tc, core_wayland_window_geometry);
        tcase_add_test(tc, core_wayland_window_type);
        tcase_add_test(tc, core_wayland_window_available_rotation);
        tcase_add_test(tc, core_wayland_window_aspect);
        tcase_add_test(tc, core_wayland_window_class);
        tcase_add_test(tc, core_wayland_window_title);
        tcase_add_test(tc, core_wayland_window_role);
        tcase_add_test(tc, core_wayland_window_input_region);
        tcase_add_test(tc, core_wayland_window_opaque_region);
        tcase_add_test(tc, core_wayland_window_popup_input);
        tcase_add_test(tc, core_wayland_window_move);
        tcase_add_test(tc, core_wayland_window_resize);
        tcase_add_test(tc, core_wayland_window_resizing_get);
        tcase_add_test(tc, core_wayland_window_output_find);
    }
}
