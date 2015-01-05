#include "gtest/gtest.h"

#include "../src/base/lib_common2.hpp"
#include "../src/base/lib_common3.hpp"

#include "../src/base/useot.hpp"

using namespace nOT::nUtils;

class cUseOtChequeTest: public testing::Test {
protected:
	std::shared_ptr<nOT::nUse::cUseOT> useOt;

	virtual void SetUp() {
		useOt = std::make_shared<nOT::nUse::cUseOT>("test");
		cout << "cheque test" << endl;
	}

	virtual void TearDown() {
		cout << "" << endl;
	}
};
