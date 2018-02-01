/*
 * \file    main.cpp
 * \brief   yacor
 * \details This file is a part of `yacor` (Yet Another CORoutine) library
 * \author  boussole
 */

#include <ucontext.h>
#include <assert.h>

#include <iostream>

#include "yacor_coroutine.h"

using namespace yacor;
using namespace std;

shared_ptr<coroutine> c0, c1, c2;
//coroutine *c3, *c4;

unique_ptr<int> iii;

#define STACK_SZ (4 * 1024 * 1024)

void co_1(void *args);
void co_2(void *args);

void co_1(void *args)
{
	int *val = static_cast<int*>(args);

	cout << ">>> " << coroutine :: current_id() << ": `" << __PRETTY_FUNCTION__ << "`; val = " << *val << endl;

	int v2 = 2;

	c2 = make_unique<coroutine>(STACK_SZ, co_2, &v2);

	c1->switch_to(*c2);

	cout << "<<< " << coroutine :: current_id() << ": `" << __PRETTY_FUNCTION__ << "`; val = " << *val << endl;

	c2.reset();

	c1->restore();

	cout << "you will never see me " << __PRETTY_FUNCTION__ << ": " << *val << endl;
}

void co_2(void *args)
{
	int *val = static_cast<int*>(args);

	cout << ">>> " << coroutine :: current_id() << ": `" << __PRETTY_FUNCTION__ << "`; val = " << *val << endl;

	cout << "<<< " << coroutine :: current_id() << ": `" << __PRETTY_FUNCTION__ << "`; val = " << *val << endl;

	c2->restore();

	cout << "you will never see me " << __PRETTY_FUNCTION__ << ": " << *val << endl;
}

int main()
{
	cout << ">>> " << coroutine :: current_id() << ": `" << __PRETTY_FUNCTION__ << "`\n";

	int v1 = 1;

	c1 = make_shared<coroutine>(STACK_SZ, co_1, &v1);

	c1->run();

	cout << "<<< " << coroutine :: current_id() << ": `" << __PRETTY_FUNCTION__ << "`\n";

	return 0;
}
