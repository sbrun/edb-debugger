/*
Copyright (C) 2015 - 2015 Evan Teran
                          evan.teran@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PLATFORM_THREAD_20151013_H_
#define PLATFORM_THREAD_20151013_H_

#include "IThread.h"
#include "IBreakpoint.h"
#include <memory>
#include <QCoreApplication>

class IProcess;

namespace DebuggerCorePlugin {

class DebuggerCore;
class PlatformState;

class PlatformThread : public IThread {
	Q_DECLARE_TR_FUNCTIONS(PlatformThread)
	friend class DebuggerCore;

public:
	enum SignalStatus {
		Stopped,
		Signaled
	};

public:
	PlatformThread(DebuggerCore *core, IProcess *process, edb::tid_t tid);
	virtual ~PlatformThread() override;

private:
	PlatformThread(const PlatformThread &) = delete;
	PlatformThread& operator=(const PlatformThread &) = delete;

public:
	virtual edb::tid_t tid() const override;
	virtual QString name() const override;
	virtual int priority() const override;
	virtual edb::address_t instruction_pointer() const override;
	virtual QString runState() const override;

public:
	virtual void get_state(State *state) override;
	virtual void set_state(const State &state) override;

public:
	virtual Status step() override;
	virtual Status step(edb::EVENT_STATUS status) override;
	virtual Status resume() override;
	virtual Status resume(edb::EVENT_STATUS status) override;
	virtual Status stop() override;

public:
	virtual bool isPaused() const override;

private:
	void fillSegmentBases(PlatformState* state);
	bool fillStateFromPrStatus(PlatformState* state);
	bool fillStateFromSimpleRegs(PlatformState* state);

private:
	unsigned long get_debug_register(std::size_t n);
	long set_debug_register(std::size_t n, long value);

private:
	DebuggerCore *const core_;
	IProcess *const     process_;
	edb::tid_t          tid_;
	int                 status_;
	SignalStatus        signal_status_;

#if defined EDB_ARM32 || defined EDB_ARM64
private:
	Status doStep(edb::tid_t tid, long status);
	std::shared_ptr<IBreakpoint> singleStepBreakpoint;
#endif
};

}

#endif
