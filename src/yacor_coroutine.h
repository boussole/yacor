/*
 * \file    yacor_coroutine.h
 * \brief   yacor
 * \details This file is a part of `yacor` (Yet Another CORoutine) library
 * \author  boussole
 */
#ifndef YACOR_COROUTINE_H_
#define YACOR_COROUTINE_H_

#include <ucontext.h>
#include <unistd.h>

#include <functional>
#include <memory>
#include <string>

namespace yacor {

namespace __details {
	struct coroutine_impl;
}

class coroutine {

public:

	/**
	 * \brief The constructor of current context
	 */
	coroutine();

	/*
	 * \brief The constructor
	 */
	coroutine(std::uint64_t stack_size, std :: function<void(void*)> fn, void *args);

	/*
	 * \brief The destructor
	 * \note  An object should be destroyed in the same coroutine that it had been created, in other case segmentation fault.
	 */
	~coroutine();

	/*
	 * \brief Run coroutine
	 */
	int run();

	/*
	 * \brief Context switch to another coroutine
	 */
	int switch_to(coroutine &co);

	/**
	 * \brief Restore context of caller
	 */
	int restore();

	/**
	 * \brief Identifier
	 */
	std :: uint64_t id();

	/**
	 * \brief Current context id
	 */
	static int current_id();

private:

	std :: string _name;                       /* human readable name (for process view and debugger) */

	std :: string _desc;                       /* description (for debug) */

	std :: unique_ptr<__details :: coroutine_impl> _impl;
};

} /* namespace yacor */

#endif /* YACOR_COROUTINE_H_ */
