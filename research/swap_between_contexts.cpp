/*
 * \file    swap_between_contexts.cpp
 * \brief   yacor
 * \details This file is a part of `yacor` (Yet Another CORoutine) library
 * \author  boussole
 * \note    https://www.gnu.org/software/libc/manual/html_node/System-V-contexts.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

/* The contexts. */
static ucontext_t uc[3];

/* This is the function doing the work.  It is just a
   skeleton, real code has to be filled in. */
static void f (int n)
{
	printf("#%d: start\n", n);

	int m = 0;
	while (m < 5) {
		printf("#%d: m = %d\n", n, m++);
		printf ("#%d: switching from #%d to #%d\n", n, n, 3 - n);
		/* Switch to the other context, saving the current one. */
		swapcontext (&uc[n], &uc[3 - n]);
	}

	printf("#%d: exit\n", n);
}

int main (void)
{
	char st1[8192];
	char st2[8192];

	/* Install the timer and get the context we can manipulate. */
	if (getcontext (&uc[1]) == -1 || getcontext (&uc[2]) == -1)
		abort ();

	/* Create a context with a separate stack which causes the
	   function f to be call with the parameter 1.
	   Note that the uc_link points to the main context
	   which will cause the program to terminate once the function return. */

	uc[1].uc_link = &uc[0];
	uc[1].uc_stack.ss_sp = st1;
	uc[1].uc_stack.ss_size = sizeof st1;
	makecontext (&uc[1], (void (*) (void)) f, 1, 1);

	/* Similarly, but 2 is passed as the parameter to f. */
	uc[2].uc_link = &uc[0];
	uc[2].uc_stack.ss_sp = st2;
	uc[2].uc_stack.ss_size = sizeof st2;
	makecontext (&uc[2], (void (*) (void)) f, 1, 2);

	/* Start running. */
	printf("#0: switch from #0 to #1\n");
	swapcontext (&uc[0], &uc[1]);
	printf("#0: exit\n");

	return 0;
}
