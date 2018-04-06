#pragma once

class Subsystem {
public:
	virtual void init() {}
	virtual void test() {}
	virtual void flightProcess() {}
	virtual void groundProcess() {}
	virtual void teardown() {}
};
