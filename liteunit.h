/*
LiteUnit
Public domain unit testing library
Created by Javier San Juan Cervera
No warranty implied. Use as you wish and at your own risk
*/

#ifndef LITE_UNIT_H
#define LITE_UNIT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ltestcase_s;
typedef void(* ltest_f)(struct ltestcase_s*);

typedef void(* ltestprint_f)(const char*);

typedef struct
{
  char str[128];
} lteststr_t;

typedef struct
{
  ltest_f test;
  lteststr_t msg;
  lteststr_t output;
} ltest_t;

typedef struct ltestcase_s
{
  lteststr_t name;
  ltest_f initcase;
  ltest_f deinitcase;
  ltest_f inittest;
  ltest_f deinittest;
  ltest_t* tests;
  size_t num_tests;
  size_t capacity;
  size_t current_test;
  size_t ok_asserts;
  size_t fail_asserts;
} ltestcase_t;

void ltestcase_init(ltestcase_t* case_, const char* name, ltest_f initcase, ltest_f deinitcase, ltest_f inittest, ltest_f deinittest);
void ltestcase_addtest(ltestcase_t* case_, ltest_f func, const char* msg);
void ltestcase_run(ltestcase_t* case_);
int ltest_assert(ltestcase_t* case_, int expression);
void ltest_printfunc(ltestprint_f func);




/* IMPLEMENTATION */




#ifdef LITE_UNIT_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void _ltest_printstd(const char* msg)
{
  printf("%s\n", msg);
}

static ltestprint_f _ltest_printfunc = _ltest_printstd;

void ltestcase_init(ltestcase_t* case_, const char* name, ltest_f initcase, ltest_f deinitcase, ltest_f inittest, ltest_f deinittest)
{
  snprintf(case_->name.str, sizeof(lteststr_t), "%s", name);
  case_->initcase = initcase;
  case_->deinitcase = deinitcase;
  case_->inittest = inittest;
  case_->deinittest = deinittest;
  case_->tests = NULL;
  case_->num_tests = 0;
  case_->capacity = 0;
  case_->current_test = 0;
  case_->ok_asserts = 0;
  case_->fail_asserts = 0;
}

void ltestcase_addtest(ltestcase_t* case_, ltest_f func, const char* msg)
{
  if (case_->num_tests == case_->capacity)
  {
    case_->capacity += 10;
    case_->tests = (ltest_t*)realloc(case_->tests, case_->capacity * sizeof(ltest_t));
  }
  case_->tests[case_->num_tests].test = func;
  snprintf(case_->tests[case_->num_tests].msg.str, sizeof(lteststr_t), "%s", msg);
  strcpy(case_->tests[case_->num_tests].output.str, "");
  case_->num_tests++;
}

void ltestcase_run(ltestcase_t* case_)
{
  lteststr_t str;
  
  /* init tests */
  snprintf(str.str, sizeof(lteststr_t), "# Testing %s", case_->name.str);
  _ltest_printfunc(str.str);
  if (case_->initcase) case_->initcase(case_);
  case_->ok_asserts = 0;
  case_->fail_asserts = 0;

  /* run tests */
  for (case_->current_test = 0; case_->current_test < case_->num_tests; ++case_->current_test)
  {
    /* init environment */
    if (case_->inittest) case_->inittest(case_);
    strcpy(case_->tests[case_->current_test].output.str, "");

    /* run test */
    case_->tests[case_->current_test].test(case_);
    if (case_->deinittest) case_->deinittest(case_);

    /* print output */
    snprintf(str.str, sizeof(lteststr_t), "* %s -> %s",
      case_->tests[case_->current_test].msg.str,
      case_->tests[case_->current_test].output.str);
    _ltest_printfunc(str.str);
  }
  if (case_->deinitcase) case_->deinitcase(case_);

  /* print results */
  snprintf(str.str, sizeof(lteststr_t), "> Finished %i tests", case_->num_tests);
  _ltest_printfunc(str.str);
  snprintf(str.str, sizeof(lteststr_t), "> * %i passed %s", case_->ok_asserts, case_->ok_asserts == 1 ? "assertion" : "assertions");
  _ltest_printfunc(str.str);
  snprintf(str.str, sizeof(lteststr_t), "> * %i failed %s", case_->fail_asserts, case_->fail_asserts == 1 ? "assertion" : "assertions");
  _ltest_printfunc(str.str);
  _ltest_printfunc("");
}

int ltest_assert(ltestcase_t* case_, int expression)
{
  strcat(case_->tests[case_->current_test].output.str, expression ? "+" : "-");
  if (expression) case_->ok_asserts++;
  else case_->fail_asserts++;
  return expression;
}

void ltest_printfunc(ltestprint_f func)
{
  _ltest_printfunc = func;
}

#endif /* LITE_UNIT_IMPLEMENTATION */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LITE_UNIT_H */
