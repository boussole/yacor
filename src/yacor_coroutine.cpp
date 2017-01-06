/*
 * \file    yacor_coroutine.cpp
 * \brief   yacor
 * \details This file is a part of `yacor` (Yet Another CORoutine) library
 * \author  boussole
 */

#include <assert.h>

#include <iostream>
#include <list>

#include "yacor_coroutine.h"

namespace yacor {

using namespace std;

/* context control: main context is always 0 */

static uint64_t __thread context_id;
static uint64_t __thread context_id_counter;

struct __details :: coroutine_impl {

	uint64_t _parent_id;

	uint64_t _prev_id;

	ucontext_t _prev_ctx;

	uint64_t _id;

	void *_obj_stack;

	ucontext_t _ctx;                           /* coroutine context */

	std :: unique_ptr<uint8_t[]> _stack;       /* coroutine stack */
};

coroutine :: coroutine() :
	_impl(make_unique<__details :: coroutine_impl>())
{
	int r;
	if ((r = getcontext(&_impl->_ctx)))
	{
		// log
		throw r;
	}
}

coroutine :: coroutine(uint64_t stack_size, function<void(void*)> fn, void *args) :
	_impl(make_unique<__details :: coroutine_impl>())
{
	int r;
	if ((r = getcontext(&_impl->_ctx)))
	{
		// log
		throw r;
	}

	/* TODO: allocate more pages than caller has asked and mprotect boundary pages */
	_impl->_stack = make_unique<uint8_t[]>(stack_size);
	_impl->_ctx.uc_stack.ss_sp = _impl->_stack.get();
	_impl->_ctx.uc_stack.ss_size = stack_size;
	_impl->_parent_id = context_id;
	_impl->_id = ++context_id_counter;
	_impl->_prev_id = _impl->_id;

	void (*adapt)(void) = reinterpret_cast<void(*)(void)>(*fn.target<void(*)(void*)>());

	makecontext(&_impl->_ctx, adapt, 1, args);

	cout << ">>> " << coroutine :: current_id() << ": `" << __PRETTY_FUNCTION__ << "`; context = " << _impl->_id << ".\n";
}

/*
   \note  Research note:
          An object should be destroyed in the same coroutine that it had been created, in other case segmentation fault.
          But boost.org says that it is an UB.
          But it is Ok with a `-fsanitize=address` build flag
 */
coroutine :: ~coroutine()
{
	cout << "<<< " << coroutine :: current_id() << ": `" << __PRETTY_FUNCTION__ << "`; context = " << _impl->_id << ".\n";

	assert(context_id == _impl->_parent_id);
}

/*
 * \brief Run coroutine
 */
int coroutine :: run()
{
	assert(_impl->_id != context_id);

	_impl->_prev_id = context_id;

	context_id = _impl->_id;

	int r = swapcontext(&_impl->_prev_ctx, &_impl->_ctx);

	context_id = _impl->_prev_id;

	return r;
}

int coroutine :: switch_to(coroutine &co)
{
	assert(_impl->_id != co._impl->_id);
	assert(_impl->_id == context_id);

	co._impl->_prev_id = context_id;

	context_id = co._impl->_id;

	int r = swapcontext(&co._impl->_prev_ctx, &co._impl->_ctx);

	context_id = co._impl->_prev_id;

	return r;
}

int coroutine :: restore()
{
	assert(_impl->_prev_id != _impl->_id);

	int r = setcontext(&_impl->_prev_ctx);

	/* we never visit this peace of code, context_id is restored by an owner context */

	return r;
}

std :: uint64_t coroutine :: id()
{
	return _impl->_id;
}

int coroutine :: current_id()
{
	return context_id;
}

} /* namespace yacor */
