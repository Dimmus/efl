/*
 * Copyright (C) 2009 Rafael Antognolli
 */

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdio.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

struct test_rect
{
    unsigned long col, row;
    int           x, y, w, h;
    Efl_Bool      full;
};

static void
check_iterator(Eina_Iterator *it, struct test_rect *cur_test)
{
    unsigned int                i = 0;
    struct Eina_Tile_Grid_Info *tile;

    EINA_ITERATOR_FOREACH(it, tile)
    {
        efl_assert_fail_if(
            cur_test[i].col != tile->col || cur_test[i].row != tile->row ||
            cur_test[i].x != tile->rect.x || cur_test[i].y != tile->rect.y ||
            cur_test[i].w != tile->rect.w || cur_test[i].h != tile->rect.h ||
            cur_test[i].full != tile->full);
        i++;
    }

    efl_assert_fail_if(i == 0);
}

TEST(test_tile_grid_slicer_iterator)
{
    Eina_Iterator    *it;
    struct test_rect *cur_test;
    struct test_rect  test1[] = {
        {1, 1, 72, 82, 10, 15, 0}
    };
    struct test_rect test2[] = {
        {1, 1, 72, 82,  56, 15, 0},
        {2, 1,  0, 82, 128, 15, 0},
        {3, 1,  0, 82, 116, 15, 0}
    };
    struct test_rect test3[] = {
        {1, 1, 72, 82, 10,  46, 0},
        {1, 2, 72,  0, 10, 128, 0},
        {1, 3, 72,  0, 10, 126, 0}
    };
    struct test_rect test4[] = {
        {1, 1, 72, 82,  56,  46, 0},
        {2, 1,  0, 82, 128,  46, 0},
        {3, 1,  0, 82, 128,  46, 0},
        {4, 1,  0, 82,  88,  46, 0},
        {1, 2, 72,  0,  56, 128, 0},
        {2, 2,  0,  0, 128, 128, 1},
        {3, 2,  0,  0, 128, 128, 1},
        {4, 2,  0,  0,  88, 128, 0},
        {1, 3, 72,  0,  56, 126, 0},
        {2, 3,  0,  0, 128, 126, 0},
        {3, 3,  0,  0, 128, 126, 0},
        {4, 3,  0,  0,  88, 126, 0}
    };
    struct test_rect test5[] = {
        {1, 1, 0, 0, 128, 128, 1}
    };
    struct test_rect test6[] = {
        {1, 1, 0, 0, 1, 1, 0}
    };
    struct test_rect test7[] = {
        {1, 1, 0, 0, 128, 128, 1},
        {2, 1, 0, 0,   1, 128, 0},
        {1, 2, 0, 0, 128,   1, 0},
        {2, 2, 0, 0,   1,   1, 0}
    };

    cur_test = test1;
    it       = eina_tile_grid_slicer_iterator_new(200, 210, 10, 15, 128, 128);
    check_iterator(it, cur_test);
    eina_iterator_free(it);

    cur_test = test2;
    it       = eina_tile_grid_slicer_iterator_new(200, 210, 300, 15, 128, 128);
    check_iterator(it, cur_test);
    eina_iterator_free(it);

    cur_test = test3;
    it       = eina_tile_grid_slicer_iterator_new(200, 210, 10, 300, 128, 128);
    check_iterator(it, cur_test);
    eina_iterator_free(it);

    cur_test = test4;
    it       = eina_tile_grid_slicer_iterator_new(200, 210, 400, 300, 128, 128);
    check_iterator(it, cur_test);
    eina_iterator_free(it);

    cur_test = test5;
    it       = eina_tile_grid_slicer_iterator_new(128, 128, 128, 128, 128, 128);
    check_iterator(it, cur_test);
    eina_iterator_free(it);

    cur_test = test6;
    it       = eina_tile_grid_slicer_iterator_new(128, 128, 1, 1, 128, 128);
    check_iterator(it, cur_test);
    eina_iterator_free(it);

    cur_test = test7;
    it       = eina_tile_grid_slicer_iterator_new(128, 128, 129, 129, 128, 128);
    check_iterator(it, cur_test);
    eina_iterator_free(it);
}

TEST(test_tiler_all)
{
    Eina_Tiler     *tl;
    Eina_Iterator  *it;
    Eina_Rectangle *rp;
    Eina_Rectangle  r;
    int             i = 0;
    int             width, height;

    tl = eina_tiler_new(1, 1);

    eina_tiler_area_size_get(tl, &width, &height);
    efl_assert_fail_if(width != 1 && height != 1);

    width  = 640;
    height = 480;
    eina_tiler_area_size_set(tl, width, height);
    eina_tiler_area_size_get(tl, &width, &height);
    efl_assert_fail_if(width != 640 && height != 480);

    eina_tiler_tile_size_set(tl, 32, 32);

    EINA_RECTANGLE_SET(&r, 50, 50, 20, 20);
    efl_assert_fail_if(!eina_tiler_rect_add(tl, &r));

    EINA_RECTANGLE_SET(&r, -10, -10, 5, 5);
    efl_assert_fail_if(eina_tiler_rect_add(tl, &r));

    EINA_RECTANGLE_SET(&r, 40, 40, 20, 20);
    eina_tiler_rect_del(tl, &r);

    it = eina_tiler_iterator_new(tl);
    efl_assert_fail_if(!it);

    EINA_ITERATOR_FOREACH(it, rp)
    {
        efl_assert_fail_if(rp->w <= 0);
        efl_assert_fail_if(rp->h <= 0);
        efl_assert_fail_if(rp->x < 0 || rp->x + rp->w > 640);
        efl_assert_fail_if(rp->y < 0 || rp->y + rp->h > 480);
        ++i;
    }

    efl_assert_fail_if(eina_iterator_container_get(it) != tl);

    eina_iterator_free(it);

    efl_assert_fail_if(i == 0);

    eina_tiler_clear(tl);

    eina_tiler_free(tl);
}

TEST(test_tiler_stable)
{
    Eina_Tiler     *tl;
    Eina_Rectangle *rp;
    Eina_Iterator  *it;
    Eina_Rectangle  r;
    int             i = 0;

    tl = eina_tiler_new(640, 480);
    efl_assert_fail_if(!tl);

    eina_tiler_tile_size_set(tl, 1, 1);

    EINA_RECTANGLE_SET(&r, 50, 50, 20, 20);
    efl_assert_fail_if(!eina_tiler_rect_add(tl, &r));

    EINA_RECTANGLE_SET(&r, 40, 40, 20, 20);
    eina_tiler_rect_del(tl, &r);

    EINA_RECTANGLE_SET(&r, 50, 50, 20, 20);
    efl_assert_fail_if(!eina_tiler_rect_add(tl, &r));

    EINA_RECTANGLE_SET(&r, 40, 40, 20, 20);
    eina_tiler_rect_del(tl, &r);

    it = eina_tiler_iterator_new(tl);
    efl_assert_fail_if(!it);

    EINA_ITERATOR_FOREACH(it, rp)
    {
        EINA_RECTANGLE_SET(&r, 40, 40, 20, 20);
        efl_assert_fail_if(eina_rectangle_intersection(&r, rp));

        EINA_RECTANGLE_SET(&r, 50, 50, 20, 20);
        efl_assert_fail_if(!eina_rectangles_intersect(&r, rp));
        ++i;
    }

    efl_assert_fail_if(i != 2);

    eina_iterator_free(it);

    eina_tiler_free(tl);
}

TEST(test_tiler_calculation)
{
    Eina_Tiler    *t1, *t2, *t;
    Eina_Iterator *itr;
    Eina_Rectangle r1, r2, r3, *rp;
    int            i = 0;

    t1 = eina_tiler_new(500, 500);
    efl_assert_fail_if(!t1);

    t2 = eina_tiler_new(500, 500);
    efl_assert_fail_if(!t2);

    eina_tiler_tile_size_set(t1, 1, 1);
    eina_tiler_tile_size_set(t2, 1, 1);

    EINA_RECTANGLE_SET(&r1, 0, 0, 500, 500);
    eina_tiler_rect_add(t1, &r1);

    EINA_RECTANGLE_SET(&r2, 100, 100, 300, 300);
    eina_tiler_rect_add(t2, &r2);

    efl_assert_fail_if(!eina_tiler_union(t1, t2));

    itr = eina_tiler_iterator_new(t1);
    EINA_ITERATOR_FOREACH(itr, rp)
    {
        efl_assert_fail_if(rp->w != 500);
        efl_assert_fail_if(rp->h != 500);
        efl_assert_fail_if(rp->x != 0);
        efl_assert_fail_if(rp->y != 0);
        ++i;
    }

    eina_iterator_free(itr);
    eina_tiler_clear(t1);

    efl_assert_fail_if(i != 1);

    eina_tiler_rect_add(t1, &r1);

    efl_assert_fail_if(!eina_tiler_subtract(t1, t2));

    i   = 0;
    itr = eina_tiler_iterator_new(t1);
    EINA_ITERATOR_FOREACH(itr, rp)
    {
        efl_assert_fail_if(!eina_rectangles_intersect(&r1, rp));
        efl_assert_fail_if(eina_rectangles_intersect(&r2, rp));

        efl_assert_fail_if(rp->w <= 0);
        efl_assert_fail_if(rp->h <= 0);
        efl_assert_fail_if(rp->x < 0 || rp->x + rp->w > 500);
        efl_assert_fail_if(rp->y < 0 || rp->y + rp->h > 500);
        ++i;
    }

    eina_iterator_free(itr);
    eina_tiler_clear(t1);

    efl_assert_fail_if(i != 4);

    EINA_RECTANGLE_SET(&r3, 0, 0, 50, 50);
    eina_tiler_rect_add(t1, &r3);

    t = eina_tiler_intersection(t1, t2);
    efl_assert_fail_if(t);

    eina_tiler_clear(t1);

    eina_tiler_rect_add(t1, &r1);

    t = eina_tiler_intersection(t1, t2);
    efl_assert_fail_if(!t);

    i   = 0;
    itr = eina_tiler_iterator_new(t);
    EINA_ITERATOR_FOREACH(itr, rp)
    {
        efl_assert_fail_if(!eina_rectangles_intersect(&r1, rp));
        efl_assert_fail_if(!eina_rectangles_intersect(&r2, rp));

        efl_assert_fail_if(rp->w <= 0);
        efl_assert_fail_if(rp->h <= 0);
        efl_assert_fail_if(rp->x < 0 || rp->x + rp->w > 500);
        efl_assert_fail_if(rp->y < 0 || rp->y + rp->h > 500);
        ++i;
    }

    eina_iterator_free(itr);
    eina_tiler_clear(t);

    efl_assert_fail_if(i != 1);

    eina_tiler_rect_add(t, &r1);

    efl_assert_fail_if(!eina_tiler_equal(t, t1));
    efl_assert_fail_if(!eina_tiler_equal(t1, t));
    EINA_RECTANGLE_SET(&r1, 0, 0, 250, 250);
    eina_tiler_rect_del(t, &r1);
    efl_assert_fail_if(eina_tiler_equal(t1, t));

    eina_tiler_free(t);
    eina_tiler_free(t1);
    eina_tiler_free(t2);
}

TEST(test_tiler_size)
{
    Eina_Rectangle *r;
    Eina_Iterator  *it;
    Eina_Tiler     *t;
    Efl_Bool        rects = EFL_FALSE;

    t = eina_tiler_new(131070, 131070);
    efl_assert_fail_if(!t);

    eina_tiler_tile_size_set(t, 1, 1);
    efl_assert_fail_if(
        !eina_tiler_rect_add(t, &(Eina_Rectangle){ 0, 0, 131070, 131070 }));
    it = eina_tiler_iterator_new(t);
    efl_assert_fail_if(!it);
    EINA_ITERATOR_FOREACH(it, r)
    {
        rects = EFL_TRUE;
        efl_assert_fail_if(r->x);
        efl_assert_fail_if(r->y);
        efl_assert_fail_if(r->w != 131070);
        efl_assert_fail_if(r->h != 131070);
    }
    efl_assert_fail_if(!rects);
    eina_iterator_free(it);
    eina_tiler_free(t);
}
