/*
  CTEST -- A simple unit test framework for C
  Copyright (C) 2011, by Christopher A. Mosher

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ctest.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



/*
  CTEST suite context. It just contains
  counts of tests passed and failed.
*/
struct ctest_ctx
  {
    long magic;
    long c_pass;
    long c_fail;
  };


/*
Fill unused memory with 0x95.
This is the only value that meets these conditions:
1. has 8 bits
2. has 4 bits set and 4 bits clear (10010101)
3. when unsigned, is prime (149)
4. when signed, is negative (high-order bit set)
5. when signed, has a nagative prime (-107)
6. has an odd high-order nibble (9)
*/
#define BAD_MEM 0x95

/* Magic bytes: "CTst" */
#define MAGIC 0x74735443

static void ctest_ctx_check(const ctest_ctx *const ctx)
{
  assert(ctx);
  assert(ctx->magic==MAGIC);
  assert(ctx->c_pass >= 0);
  assert(ctx->c_fail >= 0);
}

/*
  Allocation and deallocation of the suite context
*/
ctest_ctx *ctest_ctx_alloc(void)
{
  ctest_ctx *const ctx = (ctest_ctx*)malloc(sizeof(*ctx));
  memset(ctx,BAD_MEM,sizeof(ctest_ctx));

  ctx->magic = MAGIC;

  ctx->c_pass = 0;
  ctx->c_fail = 0;

  ctest_ctx_check(ctx);

  return ctx;
}

void ctest_ctx_free(ctest_ctx *const ctx)
{
  ctest_ctx_check(ctx);
  if (!ctest_count_test(ctx))
    {
      fprintf(stderr,"Warning: no CTEST unit tests were run.\n");
    }
  memset(ctx,BAD_MEM,sizeof(ctest_ctx));
  free(ctx);
}


/*
  The main test function (best called by the CTEST macro).
  If is_true is false, print an error message containing
  file_name, line_number. and name.
  Update counts in the given suite context ctx.
*/
void ctest(ctest_ctx *const ctx, const char *const name, const int is_true, const char *const file_name, const unsigned long line_number)
{
  ctest_ctx_check(ctx);
  if (is_true)
    {
      ++ctx->c_pass;
    }
  else
    {
      ++ctx->c_fail;
      fprintf(stderr,"%s:%lu: test failed: %s\n",file_name,line_number,name);
    }
}



/*
  Simple accessors for the counts
*/
long ctest_count_pass(const ctest_ctx *const ctx)
{
  ctest_ctx_check(ctx);
  return ctx->c_pass;
}

long ctest_count_fail(const ctest_ctx *const ctx)
{
  ctest_ctx_check(ctx);
  return ctx->c_fail;
}

long ctest_count_test(const ctest_ctx *const ctx)
{
  ctest_ctx_check(ctx);
  return ctx->c_pass+ctx->c_fail;
}
