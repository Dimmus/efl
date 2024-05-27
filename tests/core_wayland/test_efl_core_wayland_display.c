#include "efl_core_wayland_suite.h"
#include "efl_core_wayland_tests_helpers.h"

static Ecore_Wl_Display *
_display_setup(void)
{
    return ecore_wl_display_create(NULL);
}

EFL_START_TEST(core_wayland_display_create)
{
    Ecore_Wl_Display *disp;

    disp = _display_setup();
    ck_assert(disp != NULL);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_display_destroy)
{
    Ecore_Wl_Display *disp;

    disp = _display_setup();
    ck_assert(disp != NULL);

    ecore_wl_display_destroy(disp);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_display_get)
{
    Ecore_Wl_Display  *disp;
    struct wl_display *wdisp;

    disp = _display_setup();
    ck_assert(disp != NULL);

    wdisp = ecore_wl_display_get(disp);
    ck_assert(wdisp != NULL);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_display_name_get)
{
    Ecore_Wl_Display *disp;

    disp = _display_setup();
    ck_assert(disp != NULL);

    ck_assert(ecore_wl_display_name_get(disp) != NULL);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_display_connect)
{
    Ecore_Wl_Display *disp;

    disp = _display_connect();
    ck_assert(disp != NULL);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_display_disconnect)
{
    Ecore_Wl_Display *disp;

    disp = _display_connect();
    ck_assert(disp != NULL);

    ecore_wl_display_disconnect(disp);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_display_registry_get)
{
    Ecore_Wl_Display *disp;

    disp = _display_connect();
    ck_assert(disp != NULL);

    ck_assert(ecore_wl_display_registry_get(disp) != NULL);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_display_shm_get)
{
    Ecore_Wl_Display *disp;
    struct wl_shm    *shm;

    disp = _display_connect();
    ck_assert(disp != NULL);

    shm = ecore_wl_display_shm_get(disp);
    ck_assert(shm != NULL);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_display_dmabuf_get)
{
    Ecore_Wl_Display *disp;
    void             *dma;

    disp = _display_connect();
    ck_assert(disp != NULL);

    dma = ecore_wl_display_dmabuf_get(disp);
    ck_assert(dma != NULL);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_display_globals_get)
{
    Ecore_Wl_Display *disp;
    Ecore_Wl_Global  *global;
    Eina_Iterator    *itr;
    void             *data;

    disp = _display_connect();
    ck_assert(disp != NULL);

    itr = ecore_wl_display_globals_get(disp);
    ck_assert(itr != NULL);

    EINA_ITERATOR_FOREACH(itr, data)
    {
        global = (Ecore_Wl_Global *)data;
        printf("Interface: <%s>\n", global->interface);
    }

    eina_iterator_free(itr);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_display_screen_size_get)
{
    Ecore_Wl_Display *disp;
    int               w, h;

    disp = _display_connect();
    ck_assert(disp != NULL);

    ecore_wl_display_screen_size_get(disp, &w, &h);
    ck_assert_int_ne(w, 0);
    ck_assert_int_ne(h, 0);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_display_inputs_get)
{
    Ecore_Wl_Display *disp;
    Eina_Iterator    *itr;

    disp = _display_connect();
    ck_assert(disp != NULL);

    itr = ecore_wl_display_inputs_get(disp);
    ck_assert(itr != NULL);

    eina_iterator_free(itr);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_display_compositor_version_get)
{
    Ecore_Wl_Display *disp;
    int               ver;

    disp = _display_connect();
    ck_assert(disp != NULL);

    ver = ecore_wl_display_compositor_version_get(disp);
    ck_assert_int_ne(ver, 0);
}

EFL_END_TEST

Ecore_Wl_Input *test_wl_input;

static Efl_Bool
_test_input_find_configure_complete(void       *data,
                                    int type    EFL_UNUSED,
                                    void *event EFL_UNUSED)
{
    Test_Data *td = data;

   /* NB: Enlightenment uses "seat0" here, but Weston uses "default" */
    if (getenv("E_START"))
        test_wl_input =
            ecore_wl_display_input_find_by_name(td->display, "seat0");
    else
        test_wl_input =
            ecore_wl_display_input_find_by_name(td->display, "default");

    ck_assert(test_wl_input != NULL);
    test_wl_input = NULL;

    if (getenv("E_START"))
    {
        test_wl_input = ecore_wl_display_input_find(td->display, 13);
        ck_assert(test_wl_input != NULL);
    }

    core_main_loop_quit();

    return CORE_CALLBACK_PASS_ON;
}

EFL_START_TEST(core_wayland_display_input_find)
{
    Test_Data *td;

    ecore_wl_init();

    td         = calloc(1, sizeof(Test_Data));
    td->width  = WIDTH;
    td->height = HEIGHT;

    td->display = _display_connect();
    ck_assert(td->display != NULL);

    td->win = _window_create(td->display);
    ck_assert(td->win != NULL);

    ecore_wl_window_show(td->win);

    td->handler =
        core_event_handler_add(ECORE_WL2_EVENT_WINDOW_CONFIGURE_COMPLETE,
                               _test_input_find_configure_complete,
                               td);

    core_main_loop_begin();

    ecore_wl_shutdown();
    free(td);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_display_flush)
{
    Ecore_Wl_Display *disp;

    disp = _display_connect();
    ck_assert(disp != NULL);

    //FIXME: Ambiguous way to check with code to make sure flushing was successful.
    //       We might think it's being verified by another TC that actually draws to the screen buffer ...
    ecore_wl_display_flush(disp);
}

EFL_END_TEST

static Efl_Bool
_test_sync_done(void *data, int type EFL_UNUSED, void *event EFL_UNUSED)
{
    Test_Data *td = data;
    Efl_Bool   ret;

    ret = ecore_wl_display_sync_is_done(td->display);

    fail_if(ret == EFL_FALSE);

    core_main_loop_quit();

    return CORE_CALLBACK_PASS_ON;
}

EFL_START_TEST(core_wayland_display_sync_is_done)
{
    Test_Data *td;

    ecore_wl_init();

    td         = calloc(1, sizeof(Test_Data));
    td->width  = WIDTH;
    td->height = HEIGHT;

    td->display = _display_connect();
    ck_assert(td->display != NULL);

    td->win = _window_create(td->display);
    ck_assert(td->win != NULL);

    ecore_wl_window_show(td->win);

    core_event_handler_add(ECORE_WL2_EVENT_SYNC_DONE, _test_sync_done, td);

    core_main_loop_begin();

    ecore_wl_shutdown();
    free(td);
}

EFL_END_TEST

void
test_display(TCase *tc)
{
    if ((!getenv("WAYLAND_DISPLAY")) && (getenv("XDG_RUNTIME_DIR")))
    {
        /* tests here are for server-side functions */
        printf(
            "No Wayland Compositor detected. Testing server-side functions\n");
        tcase_add_test(tc, core_wayland_display_create);
        tcase_add_test(tc, core_wayland_display_destroy);
        tcase_add_test(tc, core_wayland_display_get);
        tcase_add_test(tc, core_wayland_display_name_get);
    }
    else if (getenv("WAYLAND_DISPLAY"))
    {
        /* tests here are for client-side functions */
        printf("Wayland Compositor detected. Testing client-side functions\n");
        tcase_add_test(tc, core_wayland_display_connect);
        tcase_add_test(tc, core_wayland_display_disconnect);
        tcase_add_test(tc, core_wayland_display_registry_get);
        tcase_add_test(tc, core_wayland_display_shm_get);
        tcase_add_test(tc, core_wayland_display_globals_get);
        tcase_add_test(tc, core_wayland_display_inputs_get);
        tcase_add_test(tc, core_wayland_display_compositor_version_get);
        tcase_add_test(tc, core_wayland_display_input_find);
        tcase_add_test(tc, core_wayland_display_flush);
        tcase_add_test(tc, core_wayland_display_sync_is_done);
        if (!getenv("E_START"))
        {
            tcase_add_test(tc, core_wayland_display_dmabuf_get);
            tcase_add_test(tc, core_wayland_display_screen_size_get);
        }
    }
}
