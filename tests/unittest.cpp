#include "gtest/gtest.h"

#include "../src/base/lib_common1.hpp"
#include "../src/base/useot.hpp"

class cUseOtTest: public testing::Test {
protected:
	std::shared_ptr<nOT::nUse::cUseOT> useOt;
	std::string nym1;

	virtual void SetUp() {
		nym1 = "alice";
		useOt = std::make_shared<nOT::nUse::cUseOT>("test");
	}

	virtual void TearDown() {

	}

};

TEST_F(cUseOtTest, OutPayments) {
	EXPECT_FALSE(useOt->OutpaymentsShow(nym1, 200, 0));
	EXPECT_FALSE(useOt->OutpaymentsShow(nym1, -1, 0));
}

TEST_F(cUseOtTest, Voucher) {
	EXPECT_FALSE(useOt->VoucherWithdraw(nym1, -20, "bitcoins", "some memo", 0));
}

