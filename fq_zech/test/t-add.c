/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2012 Sebastian Pancratz 
    Copyright (C) 2012 Andres Goens

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "fq_zech.h"
#include "ulong_extras.h"
#include "long_extras.h"

int
main(void)
{
    int j, i, result;
    flint_rand_t state;
    fq_zech_ctx_t ctx;

    printf("add... ");
    fflush(stdout);

    flint_randinit(state);

    for (j = 0; j < 10; j++)
    {
        fq_zech_ctx_randtest(ctx, state);

        /* Check aliasing: a = a + b */
        for (i = 0; i < 200; i++)
        {
            fq_zech_t a, b, c;

            fq_zech_init(a, ctx);
            fq_zech_init(b, ctx);
            fq_zech_init(c, ctx);

            fq_zech_randtest(a, state, ctx);
            fq_zech_randtest(b, state, ctx);

            fq_zech_add(c, a, b, ctx);
            fq_zech_add(a, a, b, ctx);

            result = (fq_zech_equal(a, c, ctx));
            if (!result)
            {
                printf("FAIL a = a + b:\n\n");
                fq_zech_ctx_print(ctx);
                printf("\n");
                printf("a = "), fq_zech_print_pretty(a, ctx), printf("\n");
                printf("b = "), fq_zech_print_pretty(b, ctx), printf("\n");
                printf("c = "), fq_zech_print_pretty(c, ctx), printf("\n");
                abort();
            }

            fq_zech_clear(a, ctx);
            fq_zech_clear(b, ctx);
            fq_zech_clear(c, ctx);
        }

        /* Check aliasing: b = a + b */
        for (i = 0; i < 200; i++)
        {
            fq_zech_t a, b, c;

            fq_zech_init(a, ctx);
            fq_zech_init(b, ctx);
            fq_zech_init(c, ctx);

            fq_zech_randtest(a, state, ctx);
            fq_zech_randtest(b, state, ctx);

            fq_zech_add(c, a, b, ctx);
            fq_zech_add(b, a, b, ctx);

            result = (fq_zech_equal(b, c, ctx));
            if (!result)
            {
                printf("FAIL b = a + b:\n\n");
                fq_zech_ctx_print(ctx);
                printf("\n");
                printf("a = "), fq_zech_print_pretty(a, ctx), printf("\n");
                printf("b = "), fq_zech_print_pretty(b, ctx), printf("\n");
                printf("c = "), fq_zech_print_pretty(c, ctx), printf("\n");
                abort();
            }

            fq_zech_clear(a, ctx);
            fq_zech_clear(b, ctx);
            fq_zech_clear(c, ctx);
        }

        /* Check aliasing: a = a + a */
        for (i = 0; i < 2000; i++)
        {
            fq_zech_t a, c;

            fq_zech_init(a, ctx);
            fq_zech_init(c, ctx);

            fq_zech_randtest(a, state, ctx);

            fq_zech_add(c, a, a, ctx);
            fq_zech_add(a, a, a, ctx);

            result = (fq_zech_equal(a, c, ctx));
            if (!result)
            {
                printf("FAIL:\n\n");
                fq_zech_ctx_print(ctx);
                printf("\n");
                printf("a = "), fq_zech_print_pretty(a, ctx), printf("\n");
                printf("c = "), fq_zech_print_pretty(c, ctx), printf("\n");
                abort();
            }

            fq_zech_clear(a, ctx);
            fq_zech_clear(c, ctx);
        }

        /* Check that a + b == b + a */
        for (i = 0; i < 200; i++)
        {
            fq_zech_t a, b, c1, c2;

            fq_zech_init(a, ctx);
            fq_zech_init(b, ctx);
            fq_zech_init(c1, ctx);
            fq_zech_init(c2, ctx);

            fq_zech_randtest(a, state, ctx);
            fq_zech_randtest(b, state, ctx);

            fq_zech_add(c1, a, b, ctx);
            fq_zech_add(c2, b, a, ctx);

            result = (fq_zech_equal(c1, c2, ctx));
            if (!result)
            {
                printf("FAIL a + b = b + a\n\n");
                fq_zech_ctx_print(ctx);
                printf("\n");
                printf("a  = "), fq_zech_print_pretty(a, ctx), printf("\n");
                printf("b  = "), fq_zech_print_pretty(b, ctx), printf("\n");
                printf("c1 = "), fq_zech_print_pretty(c1, ctx), printf("\n");
                printf("c2 = "), fq_zech_print_pretty(c2, ctx), printf("\n");
                abort();
            }

            fq_zech_clear(a, ctx);
            fq_zech_clear(b, ctx);
            fq_zech_clear(c1, ctx);
            fq_zech_clear(c2, ctx);
        }

        /* Check that (a + b) + c == a + (b + c) */
        for (i = 0; i < 2000; i++)
        {
            fq_zech_t a, b, c, lhs, rhs;

            fq_zech_init(a, ctx);
            fq_zech_init(b, ctx);
            fq_zech_init(c, ctx);
            fq_zech_init(lhs, ctx);
            fq_zech_init(rhs, ctx);

            fq_zech_randtest(a, state, ctx);
            fq_zech_randtest(b, state, ctx);
            fq_zech_randtest(c, state, ctx);

            fq_zech_add(lhs, a, b, ctx);
            fq_zech_add(lhs, lhs, c, ctx);
            fq_zech_add(rhs, b, c, ctx);
            fq_zech_add(rhs, a, rhs, ctx);

            result = (fq_zech_equal(lhs, rhs, ctx));
            if (!result)
            {
                printf("FAIL (a+b)+c == a + (b+c):\n\n");
                fq_zech_ctx_print(ctx);
                printf("\n");
                printf("a   = "), fq_zech_print_pretty(a, ctx), printf("\n");
                printf("b   = "), fq_zech_print_pretty(b, ctx), printf("\n");
                printf("c   = "), fq_zech_print_pretty(c, ctx), printf("\n");
                printf("lhs = "), fq_zech_print_pretty(lhs, ctx), printf("\n");
                printf("rhs = "), fq_zech_print_pretty(rhs, ctx), printf("\n");
                abort();
            }

            fq_zech_clear(a, ctx);
            fq_zech_clear(b, ctx);
            fq_zech_clear(c, ctx);
            fq_zech_clear(lhs, ctx);
            fq_zech_clear(rhs, ctx);
        }

        fq_zech_ctx_clear(ctx);
    }


    flint_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}