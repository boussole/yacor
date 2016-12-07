/*
 * \file    get_set_current_context.cpp
 * \brief   yacor
 * \details This file is a part of `yacor` (Yet Another CORoutine) library
 * \author  boussole
 */

#include <assert.h>
#include <iostream>
#include <ucontext.h>
#include <unistd.h>

/*
  Simple example of usage getcontext/setcontext.

  Scenario:
  1) save current context
  2) restore saved context

  Note: it's the same as `goto back`
 */

using namespace std;

int main()
{
	int iteration = 0; /* we use the same context (stack) for swapping, so this value is the same after context switch */

	ucontext_t ctx;
	if (getcontext(&ctx)) {
		cout << __PRETTY_FUNCTION__ << ": getcontext() failed\n";
		return 1;
	}

	std :: cout << iteration++ << ": sleeping 1 sec\n";

	sleep(1);

	if (setcontext(&ctx)) {
		cout << __PRETTY_FUNCTION__ << ": setcontext() failed\n";
		return 1;
	}

	std :: cout << "You will never see this message\n";

	assert(0);

	return 0;
}
