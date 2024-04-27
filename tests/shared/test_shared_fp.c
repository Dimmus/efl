/*
 * Copyright (C) 2010 Cedric Bail
 */

#include <stdio.h>
#include <math.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

TEST(fp_cos)
{
    Eina_F32p32 fc;
    Eina_F32p32 fl;
    Eina_F32p32 step;
    Eina_F32p32 fresult;
    double      dc;
    double      dresult;
    double      delta;

    fl   = eina_f32p32_scale(EINA_F32P32_PI, 4);
    step = eina_f32p32_div(fl, eina_f32p32_int_from(2048));

    for (fc = 0; fc < fl; fc += step)
    {
        fresult = eina_f32p32_cos(fc);
        dc      = eina_f32p32_double_to(fc);
        dresult = cos(dc);

        delta = fabs(dresult - eina_f32p32_double_to(fresult));
        efl_assert_fail_if(delta > 0.005);
    }
}

TEST(fp_sin)
{
    Eina_F32p32 fc;
    Eina_F32p32 fl;
    Eina_F32p32 step;
    Eina_F32p32 fresult;
    double      dc;
    double      dresult;
    double      delta;

    fl   = eina_f32p32_scale(EINA_F32P32_PI, 4);
    step = eina_f32p32_div(fl, eina_f32p32_int_from(2048));

    for (fc = 0; fc < fl; fc += step)
    {
        fresult = eina_f32p32_sin(fc);
        dc      = eina_f32p32_double_to(fc);
        dresult = sin(dc);

        delta = fabs(dresult - eina_f32p32_double_to(fresult));
        efl_assert_fail_if(delta > 0.005);
    }
}

TEST(fp_mul)
{
    Eina_F32p32 fc1, fc2;
    Eina_F32p32 fresult;
    double      dc1, dc2;
    double      dl1, dl2;
    double      step1, step2;
    double      dresult;
    double      delta, delta_per;
    double      maxdelta     = 0;
    double      maxdelta_per = 0;

    dl1   = 10;
    step1 = 0.007;
    dl2   = 1000;
    step2 = 0.061;

    for (dc1 = 0; dc1 < dl1; dc1 += step1)
    {
        if ((int)dc1 % 5 == 2) dc1 += 2;
        for (dc2 = 0; dc2 < dl2; dc2 += step2)
        {
            if ((int)dc2 % 10 == 5) dc2 += 20;

            dresult = dc1 * dc2;

            fc1     = eina_f32p32_double_from(dc1);
            fc2     = eina_f32p32_double_from(dc2);
            fresult = eina_f32p32_mul(fc1, fc2);

            delta     = fabs(dresult - eina_f32p32_double_to(fresult));
            delta_per = delta / dresult;
            if (delta > 0.008 || delta_per > 0.01)
            {
                printf("%f*%f = %f (delta: %f, %f%%)\n",
                       dc1,
                       dc2,
                       dresult,
                       delta,
                       delta_per * 100);
                efl_assert_fail_if(delta > 0.005 || delta_per > 0.01);
            }
            if (delta > maxdelta) maxdelta = delta;
            if (delta_per > maxdelta_per) maxdelta_per = delta_per;
        }
    }
    testlog("Max delta(multiplication): %f (%f%%)\n",
            maxdelta,
            maxdelta_per * 100);
}

TEST(fp_div)
{
    Eina_F32p32 fc1, fc2;
    Eina_F32p32 fresult;
    double      dc1, dc2;
    double      dl1, dl2;
    double      step1, step2;
    double      dresult;
    double      delta, delta_per;
    double      maxdelta     = 0;
    double      maxdelta_per = 0;

    dl1   = 10;
    step1 = 0.007;
    dl2   = 1000;
    step2 = 0.061;

    for (dc1 = 0; dc1 < dl1; dc1 += step1)
    {
        if ((int)dc1 % 5 == 2) dc1 += 2;
        for (dc2 = step2; dc2 < dl2; dc2 += step2)
        {
            if ((int)dc2 % 10 == 5) dc2 += 20;
            dresult = dc1 / dc2;

            fc1     = eina_f32p32_double_from(dc1);
            fc2     = eina_f32p32_double_from(dc2);
            fresult = eina_f32p32_div(fc1, fc2);

            delta     = fabs(dresult - eina_f32p32_double_to(fresult));
            delta_per = delta / dresult;
            if (delta > 0.005 || delta_per > 0.03)
            {
                printf("%f/%f = %f (delta %f, %f%%)\n",
                       dc1,
                       dc2,
                       dresult,
                       delta,
                       delta_per * 100);
                efl_assert_fail_if(delta > 0.005 || delta_per > 0.03);
            }
            if (delta > maxdelta) maxdelta = delta;
            if (delta_per > maxdelta_per) maxdelta_per = delta_per;
        }
    }
    testlog("Max delta(division): %f (%f%%)\n", maxdelta, maxdelta_per * 100);
}
