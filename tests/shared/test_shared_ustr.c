/*
 * Copyright (C) 2010 Brett Nash
 */

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

#ifdef EINA_SAFETY_CHECKS
struct log_ctx
{
    const char *msg;
    const char *fnc;
    Efl_Bool    did;
};

/* tests should not output on success, just uncomment this for debugging */
//#define SHOW_LOG 1

static void
_eina_test_safety_print_cb(const Eina_Log_Domain *d,
                           Eina_Log_Level         level,
                           const char            *file,
                           const char            *fnc,
                           int                    line,
                           const char            *fmt,
                           void                  *data,
                           va_list args           EFL_UNUSED)
{
    struct log_ctx *ctx = data;
    va_list         cp_args;
    const char     *str;

    va_copy(cp_args, args);
    str = va_arg(cp_args, const char *);
    va_end(cp_args);

    efl_assert_int_eq(level, EINA_LOG_LEVEL_ERR);
    efl_assert_str_eq(fmt, "%s");
    efl_assert_str_eq(ctx->msg, str);
    efl_assert_str_eq(ctx->fnc, fnc);
    ctx->did = EFL_TRUE;

#  ifdef SHOW_LOG
    eina_log_print_cb_stderr(d, level, file, fnc, line, fmt, NULL, args);
#  else
    (void)d;
    (void)file;
    (void)line;
#  endif
}
#endif

static const Eina_Unicode STR1[] = {
    'P', 'a', 'n', 't', 's', ' ', 'O', 'n', 0
};
static const Eina_Unicode STR2[] = { 'P', 'a', 'n', 't', 's',
                                     ' ', 'O', 'f', 'f', 0 };
static const Eina_Unicode STR3[] = {
    'P', 'a', 'n', 't', 's', ' ', 'O', 'n', 0
};
static const Eina_Unicode STR4[]     = { 'A', 0 };
static const Eina_Unicode EMPTYSTR[] = { 0 };

TEST(unicode_strcmp_test)
{
   /* 1 & 2 */
    efl_assert_fail_if(eina_unicode_strcmp(STR1, STR2) == 0);
    efl_assert_fail_if(eina_unicode_strcmp(STR1, STR2) < 1);

   /* 1 & 3 */
    efl_assert_fail_if(eina_unicode_strcmp(STR1, STR3) != 0);

   /* 1 & 4 */
    efl_assert_fail_if(eina_unicode_strcmp(STR1, STR4) == 0);
    efl_assert_fail_if(eina_unicode_strcmp(STR1, STR4) > 1);

   /* 1 & empty */
    efl_assert_fail_if(eina_unicode_strcmp(STR1, EMPTYSTR) < 1);

   /* Self tests */
    efl_assert_fail_if(eina_unicode_strcmp(STR1, STR1) != 0);
    efl_assert_fail_if(eina_unicode_strcmp(STR2, STR2) != 0);
    efl_assert_fail_if(eina_unicode_strcmp(STR3, STR3) != 0);
    efl_assert_fail_if(eina_unicode_strcmp(STR4, STR4) != 0);
    efl_assert_fail_if(eina_unicode_strcmp(EMPTYSTR, EMPTYSTR) != 0);
}

TEST(unicode_strcpy_test)
{
    Eina_Unicode  buf[10] = { 0 };
    Eina_Unicode *rv;

    rv = eina_unicode_strcpy(buf, STR1);
    efl_assert_fail_if(rv != buf);
    efl_assert_fail_if(eina_unicode_strcmp(buf, STR1) != 0);

    rv = eina_unicode_strcpy(buf, STR2);
    efl_assert_fail_if(rv != buf);
    efl_assert_fail_if(eina_unicode_strcmp(buf, STR2) != 0);

   /* Now a shorter string */
    rv = eina_unicode_strcpy(buf, STR2);
    efl_assert_fail_if(rv != buf);
    efl_assert_fail_if(eina_unicode_strcmp(buf, STR2) != 0);

   /* Really short string */
    rv = eina_unicode_strcpy(buf, STR4);
    efl_assert_fail_if(rv != buf);
    efl_assert_fail_if(eina_unicode_strcmp(buf, STR4) != 0);
    efl_assert_fail_if(buf[2] != 'n'); /* check old buf is there */

    buf[1] = '7';
    rv     = eina_unicode_strcpy(buf, EMPTYSTR);
    efl_assert_fail_if(rv != buf);
    efl_assert_fail_if(buf[0] != 0);
    efl_assert_fail_if(buf[1] != '7');
}

TEST(unicode_strncpy_test)
{
    Eina_Unicode  buf[10] = { 0 };
    Eina_Unicode *rv;

    rv = eina_unicode_strncpy(buf, STR1, 9);
    efl_assert_fail_if(rv != buf);
    efl_assert_fail_if(eina_unicode_strcmp(buf, STR1) != 0);

    buf[1] = '7';
    rv     = eina_unicode_strncpy(buf, STR1, 1);
    efl_assert_fail_if(rv != buf);
    efl_assert_fail_if(buf[1] != '7');
    efl_assert_fail_if(buf[0] != STR1[0]);

    buf[9] = '7';
    rv     = eina_unicode_strncpy(buf, STR4, 10);
    efl_assert_fail_if(rv != buf);
    efl_assert_fail_if(eina_unicode_strcmp(buf, STR4) != 0);
    efl_assert_fail_if(buf[9] != 0);

    buf[0] = '7';
    eina_unicode_strncpy(buf, STR1, 0);
    efl_assert_fail_if(buf[0] != '7');

#ifdef EINA_SAFETY_CHECKS
    {
        struct log_ctx ctx;

#  define TEST_MAGIC_SAFETY(fn, _msg) \
      ctx.msg = _msg;                 \
      ctx.fnc = fn;                   \
      ctx.did = EFL_FALSE

        eina_log_print_cb_set(_eina_test_safety_print_cb, &ctx);

      /* may segfault */
        buf[0] = '7';
#  ifdef SHOW_LOG
        fprintf(stderr, "you should have a safety check failure below:\n");
#  endif
        TEST_MAGIC_SAFETY("eina_unicode_strncpy",
                          "safety check failed: source == NULL");
        rv = eina_unicode_strncpy(buf, NULL, 0);
        efl_assert_fail_if(buf[0] != '7');
        efl_assert_true(ctx.did);

      /* Hopefully won't segfault */
#  ifdef SHOW_LOG
        fprintf(stderr, "you should have a safety check failure below:\n");
#  endif
        TEST_MAGIC_SAFETY("eina_unicode_strncpy",
                          "safety check failed: dest == NULL");
        rv = eina_unicode_strncpy(NULL, STR1, 0);
        efl_assert_fail_if(rv != NULL);
        efl_assert_true(ctx.did);

        eina_log_print_cb_set(eina_log_print_cb_stderr, NULL);
#  undef TEST_MAGIC_SAFETY
    }
#endif
}

TEST(ustr_strlen_test)
{
    efl_assert_fail_if(eina_unicode_strlen(STR1) != 8);
    efl_assert_fail_if(eina_unicode_strlen(STR2) != 9);
    efl_assert_fail_if(eina_unicode_strlen(STR3) != 8);
    efl_assert_fail_if(eina_unicode_strlen(STR4) != 1);
    efl_assert_fail_if(eina_unicode_strlen(EMPTYSTR) != 0);

#ifdef EINA_SAFETY_CHECKS
    {
        struct log_ctx ctx;

#  define TEST_MAGIC_SAFETY(fn, _msg) \
      ctx.msg = _msg;                 \
      ctx.fnc = fn;                   \
      ctx.did = EFL_FALSE

        eina_log_print_cb_set(_eina_test_safety_print_cb, &ctx);

#  ifdef SHOW_LOG
        fprintf(stderr, "you should have a safety check failure below:\n");
#  endif
        TEST_MAGIC_SAFETY("eina_unicode_strlen",
                          "safety check failed: ustr == NULL");
        efl_assert_fail_if(eina_unicode_strlen(NULL));
        efl_assert_true(ctx.did);

        eina_log_print_cb_set(eina_log_print_cb_stderr, NULL);
#  undef TEST_MAGIC_SAFETY
    }
#endif
}

TEST(unicode_strnlen_test)
{
   /* Strlen style tests*/
    efl_assert_fail_if(eina_unicode_strnlen(STR1, 10) != 8);
    efl_assert_fail_if(eina_unicode_strnlen(STR2, 10) != 9);
    efl_assert_fail_if(eina_unicode_strnlen(STR3, 10) != 8);
    efl_assert_fail_if(eina_unicode_strnlen(STR4, 10) != 1);
    efl_assert_fail_if(eina_unicode_strnlen(EMPTYSTR, 10) != 0);

   /* Too short tests */
    efl_assert_fail_if(eina_unicode_strnlen(STR1, 3) != 3);
    efl_assert_fail_if(eina_unicode_strnlen(STR2, 3) != 3);
    efl_assert_fail_if(eina_unicode_strnlen(STR3, 3) != 3);
    efl_assert_fail_if(eina_unicode_strnlen(EMPTYSTR, 1) != 0);

#ifdef EINA_SAFETY_CHECKS
    {
        struct log_ctx ctx;

#  define TEST_MAGIC_SAFETY(fn, _msg) \
      ctx.msg = _msg;                 \
      ctx.fnc = fn;                   \
      ctx.did = EFL_FALSE

        eina_log_print_cb_set(_eina_test_safety_print_cb, &ctx);

#  ifdef SHOW_LOG
        fprintf(stderr, "you should have a safety check failure below:\n");
#  endif
        TEST_MAGIC_SAFETY("eina_unicode_strnlen",
                          "safety check failed: ustr == NULL");
        efl_assert_fail_if(eina_unicode_strnlen(NULL, 0) != 0);
        efl_assert_true(ctx.did);

        eina_log_print_cb_set(eina_log_print_cb_stderr, NULL);
#  undef TEST_MAGIC_SAFETY
    }
#endif
}

TEST(unicode_strdup_test)
{
    Eina_Unicode *buf;

    buf = eina_unicode_strdup(STR1);
    efl_assert_fail_if(!buf);
    efl_assert_fail_if(eina_unicode_strlen(buf) != eina_unicode_strlen(STR1));
    efl_assert_fail_if(eina_unicode_strcmp(buf, STR1));
    free(buf);

    buf = eina_unicode_strdup(EMPTYSTR);
    efl_assert_fail_if(!buf);
    efl_assert_fail_if(buf[0] != 0);
    free(buf);
}

TEST(unicode_strstr_test)
{
    Eina_Unicode *buf;
    Eina_Unicode  on[] = { 'O', 'n', 0 };

    buf = eina_unicode_strstr(STR1, on);
    efl_assert_fail_if(!buf);
    efl_assert_fail_if(buf != STR1 + 6);
    efl_assert_fail_if(eina_unicode_strcmp(buf, on) != 0);

    buf = eina_unicode_strstr(STR2, on);
    efl_assert_fail_if(buf);

    buf = eina_unicode_strstr(EMPTYSTR, on);
    efl_assert_fail_if(buf);

    buf = eina_unicode_strstr(STR1, EMPTYSTR);
    efl_assert_fail_if(!buf);
    efl_assert_fail_if(buf != STR1);
}

TEST(unicode_escape_test)
{
    Eina_Unicode str[][10] = {
        { 'P', 'a', ' ', 'O', 'n', 0 },
        { 'P', 'a', ' ', '\\', '\'', 0 },
        { 'n', 0 },
        { 0 }
    };
    Eina_Unicode result_str[][10] = {
        { 'P', 'a', '\\', ' ', 'O', 'n', 0 },
        { 'P', 'a', '\\', ' ', '\\', '\\', '\\', '\'', 0 },
        { 'n', 0 },
        { 0 }
    };
    Eina_Unicode *buf;
    size_t        i;

    for (i = 0; i < sizeof(str) / sizeof(str[0]); ++i)
    {
        buf = eina_unicode_escape(str[i]);
        efl_assert_fail_if(!buf);
        efl_assert_fail_if(eina_unicode_strlen(buf) !=
                           eina_unicode_strlen(result_str[i]));
        efl_assert_fail_if(eina_unicode_strcmp(buf, result_str[i]) != 0);
        free(buf);
    }
}

TEST(unicode_utf8)
{
    int           ind;
    unsigned char ch;

   /* Valid utf-8 cases */
   /* First possible sequence of a certain length */
    ind = 0;
    efl_assert_fail_if((eina_unicode_utf8_next_get("\x00", &ind) != 0x00) ||
                       (ind != 0));
    ind = 0;
    efl_assert_fail_if((eina_unicode_utf8_next_get("\x01", &ind) != 0x01) ||
                       (ind != 1));
    ind = 0;
    efl_assert_fail_if((eina_unicode_utf8_next_get("\xC2\x80", &ind) != 0x80) ||
                       (ind != 2));
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xE0\xA0\x80", &ind) != 0x800) ||
        (ind != 3));
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xF0\x90\x80\x80", &ind) != 0x10000) ||
        (ind != 4));
    ind = 0;
    efl_assert_fail_if((eina_unicode_utf8_next_get("\xF8\x88\x80\x80\x80",
                                                   &ind) != 0x200000) ||
                       (ind != 5));
    ind = 0;
    efl_assert_fail_if((eina_unicode_utf8_next_get("\xFC\x84\x80\x80\x80\x80",
                                                   &ind) != 0x4000000) ||
                       (ind != 6));

   /* Last possible sequence of a certain length */
    ind = 0;
    efl_assert_fail_if((eina_unicode_utf8_next_get("\x7F", &ind) != 0x7F) ||
                       (ind != 1));
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xDF\xBF", &ind) != 0x7FF) || (ind != 2));
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xEF\xBF\xBF", &ind) != 0xFFFF) ||
        (ind != 3));
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xF7\xBF\xBF\xBF", &ind) != 0x1FFFFF) ||
        (ind != 4));
    ind = 0;
    efl_assert_fail_if((eina_unicode_utf8_next_get("\xFB\xBF\xBF\xBF\xBF",
                                                   &ind) != 0x3FFFFFF) ||
                       (ind != 5));
    ind = 0;
    efl_assert_fail_if((eina_unicode_utf8_next_get("\xFD\xBF\xBF\xBF\xBF\xBF",
                                                   &ind) != 0x7FFFFFFF) ||
                       (ind != 6));

   /* Other boundary conditions */
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xED\x9F\xBF", &ind) != 0xD7FF) ||
        (ind != 3));
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xEE\x80\x80", &ind) != 0xE000) ||
        (ind != 3));
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xEF\xBF\xBD", &ind) != 0xFFFD) ||
        (ind != 3));
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xF4\x8F\xBF\xBF", &ind) != 0x10FFFF) ||
        (ind != 4));
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xF4\x90\x80\x80", &ind) != 0x110000) ||
        (ind != 4));

   /* Error cases */
   /* Standalone continuation bytes */
    ind = 0;
    efl_assert_fail_if((eina_unicode_utf8_next_get("\x80", &ind) != 0xDC80) ||
                       (ind != 1));
    ind = 0;
    efl_assert_fail_if((eina_unicode_utf8_next_get("\xBF", &ind) != 0xDCBF) ||
                       (ind != 1));
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\x80\xBF", &ind) != 0xDC80) || (ind != 1));
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xBF\x80", &ind) != 0xDCBF) || (ind != 1));
   /* All possible continuation bytes */
    for (ch = 0x80; ch <= 0xBF; ch++)
    {
        char buf[] = { ch, 0 };
        ind        = 0;
        efl_assert_fail_if(
            (eina_unicode_utf8_next_get(buf, &ind) != (0xDC00 | ch)) ||
            (ind != 1));
    }

   /* Isolated starting sequences */
#define _FIRST_SEQUENCES(start, end)                                        \
    do                                                                      \
    {                                                                       \
        int   i;                                                            \
        char *buf = alloca(((end - start + 1) * 2) + 1);                    \
        for (i = 0, ch = start; ch <= end; i++, ch++)                       \
        {                                                                   \
            buf[i * 2]       = ch;                                          \
            buf[(i * 2) + 1] = ' ';                                         \
        }                                                                   \
        ind = 0;                                                            \
        for (i = 0, ch = start; ch <= end; ch++)                            \
        {                                                                   \
            efl_assert_fail_if(                                             \
                (eina_unicode_utf8_next_get(buf, &ind) != (0xDC00 | ch)) || \
                (ind != ++i));                                              \
            efl_assert_fail_if(                                             \
                (eina_unicode_utf8_next_get(buf, &ind) != 0x20) ||          \
                (ind != ++i));                                              \
        }                                                                   \
    }                                                                       \
    while (0)
   /* all first bytes of 2-byte sequences separated by spaces. */
    _FIRST_SEQUENCES(0xC0, 0xDF);
   /* all first bytes of 3-byte sequences separated by spaces. */
    _FIRST_SEQUENCES(0xE0, 0xEF);
   /* all first bytes of 4-byte sequences separated by spaces. */
    _FIRST_SEQUENCES(0xF0, 0xF7);
   /* all first bytes of 5-byte sequences separated by spaces. */
    _FIRST_SEQUENCES(0xF8, 0xFB);
   /* all first bytes of 6-byte sequences separated by spaces. */
    _FIRST_SEQUENCES(0xFC, 0xFD);

   /* Incomplete sequences first means the first utf8 char, len means
    * the correct length */
#define _INCOMPLETE_SEQUENCES(first, conti, len)                              \
    do                                                                        \
    {                                                                         \
        int   i, j;                                                           \
        char *buf = alloca(len + 1);                                          \
        i         = 0;                                                        \
        buf[i++]  = first;                                                    \
        for (; i < len; i++)                                                  \
        {                                                                     \
            Eina_Unicode val;                                                 \
            for (j = 1; j < i; j++)                                           \
            {                                                                 \
                buf[j] = conti;                                               \
            }                                                                 \
            buf[j] = 0;                                                       \
            ind    = 0;                                                       \
            efl_assert_fail_if(                                               \
                (eina_unicode_utf8_next_get(buf, &ind) != (0xDC00 | first))); \
            while ((val = eina_unicode_utf8_next_get(buf, &ind)))             \
            {                                                                 \
                efl_assert_fail_if(val != (0xDC00 | conti));                  \
            }                                                                 \
            efl_assert_fail_if(ind != i);                                     \
        }                                                                     \
    }                                                                         \
    while (0)

   /* Sequences with missing continuation */
    _INCOMPLETE_SEQUENCES(0xC0, 0x81, 2);
    _INCOMPLETE_SEQUENCES(0xDF, 0xBF, 2);
    _INCOMPLETE_SEQUENCES(0xE0, 0x81, 3);
    _INCOMPLETE_SEQUENCES(0xEF, 0xBF, 3);
    _INCOMPLETE_SEQUENCES(0xF0, 0x81, 4);
    _INCOMPLETE_SEQUENCES(0xF7, 0xBF, 4);
    _INCOMPLETE_SEQUENCES(0xF8, 0x81, 5);
    _INCOMPLETE_SEQUENCES(0xFB, 0xBF, 5);
    _INCOMPLETE_SEQUENCES(0xFC, 0x81, 6);
    _INCOMPLETE_SEQUENCES(0xFD, 0xBF, 6);

   /* Impossible bytes */
    ind = 0;
    efl_assert_fail_if((eina_unicode_utf8_next_get("\xFE", &ind) != 0xDCFE) ||
                       (ind != 1));
    ind = 0;
    efl_assert_fail_if((eina_unicode_utf8_next_get("\xFF", &ind) != 0xDCFF) ||
                       (ind != 1));

   /* Overlong sequences */
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xC0\xAF", &ind) != 0xDCC0) || (ind != 1));
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xE0\x80\xAF", &ind) != 0xDCE0) ||
        (ind != 1));
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xF0\x80\x80\xAF", &ind) != 0xDCF0) ||
        (ind != 1));
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xF8\x80\x80\x80\xAF", &ind) != 0xDCF8) ||
        (ind != 1));
    ind = 0;
    efl_assert_fail_if((eina_unicode_utf8_next_get("\xFC\x80\x80\x80\x80\xAF",
                                                   &ind) != 0xDCFC) ||
                       (ind != 1));

   /* Maximum overlong sequences */
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xC1\xBF", &ind) != 0xDCC1) || (ind != 1));
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xE0\x9F\xBF", &ind) != 0xDCE0) ||
        (ind != 1));
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xF0\x8F\xBF\xBF", &ind) != 0xDCF0) ||
        (ind != 1));
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_next_get("\xF8\x87\xBF\xBF\xBF", &ind) != 0xDCF8) ||
        (ind != 1));
    ind = 0;
    efl_assert_fail_if((eina_unicode_utf8_next_get("\xFC\x83\xBF\xBF\xBF\xBF",
                                                   &ind) != 0xDCFC) ||
                       (ind != 1));
   /* Add some more error cases here */

   /* Just to cover prev/len. General utf-8 parsing was covered above */
    efl_assert_fail_if(
        eina_unicode_utf8_get_len("\xF4\x90\x80\x80\xF4\x8F\xBF\xBF") != 2);
    ind = 0;
    efl_assert_fail_if(
        (eina_unicode_utf8_get_prev("\xED\x9F\xBF", &ind) != 0xD7FF) ||
        (ind != 0));
    ind = 3;
    efl_assert_fail_if(
        (eina_unicode_utf8_get_prev("\xED\x9F\xBF", &ind) != 0x00) ||
        (ind != 0));
}

TEST(unicode_utf8_conversion)
{
    Eina_Unicode  uni_in[] = { 0x5D0,      0xFDF6, 0xDC80, 0x1F459, 0x3FFFFFF,
                               0x7FFFFFFF, 'a',    'b',    'c',     0 };
    Eina_Unicode *uni_out;
    char          c_in[] = "\xD7\x90"
                           "\xEF\xB7\xB6"
                           "\x80"
                           "\xF0\x9F\x91\x99"
                           "\xFB\xBF\xBF\xBF\xBF"
                           "\xFD\xBF\xBF\xBF\xBF\xBF"
                           "abc";
    char         *c_out;

   /* Substring of c_in (offset = 2, length = 3) */
    char  c_sub[] = "\x80"
                    "\xF0\x9F\x91\x99"
                    "\xFB\xBF\xBF\xBF\xBF";
    char *c_sub_out;
    int   len;

    uni_out = eina_unicode_utf8_to_unicode(c_in, &len);
    efl_assert_fail_if((len != 9) || eina_unicode_strcmp(uni_in, uni_out));
    free(uni_out);

    c_out = eina_unicode_unicode_to_utf8(uni_in, &len);
    efl_assert_fail_if((len != 24) || strcmp(c_in, c_out));
    free(c_out);

   /* Range conversion */
    c_sub_out = eina_unicode_unicode_to_utf8_range(uni_in + 2, 3, &len);
    efl_assert_int_eq(len, 10);
    efl_assert_str_eq(c_sub, c_sub_out);

    c_sub_out = eina_unicode_unicode_to_utf8_range(uni_in, 100, &len);
    efl_assert_int_eq(len, 24);
    efl_assert_str_eq(c_in, c_sub_out);

    c_sub_out = eina_unicode_unicode_to_utf8_range(uni_in, 0, &len);
    efl_assert_int_eq(len, 0);
    efl_assert_str_eq("", c_sub_out);
}
