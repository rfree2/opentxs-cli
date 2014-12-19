#include "gtest/gtest.h"

#include "../src/base/lib_common2.hpp"
#include "../src/base/lib_common3.hpp"

#include "../src/base/useot.hpp"

using namespace nOT::nUtils;

class cUseOtTest: public testing::Test {
protected:
	std::shared_ptr<nOT::nUse::cUseOT> useOt;
	std::string nym1;
	std::string toNym;
	std::string fromAcc;
	std::string fromNym;
	int32_t amount;

	virtual void SetUp() {
		nym1 = "alice";
		toNym = "FT's Test Nym";
		fromNym = "Trader Bob";
		fromAcc = "Bob's Tokens";
		amount = 100;
		useOt = std::make_shared<nOT::nUse::cUseOT>("test");
	}

	virtual void TearDown() {

	}
};

TEST_F(cUseOtTest, OutPayments) {
	EXPECT_FALSE(useOt->OutpaymentsShow(nym1, 200, 0));
	EXPECT_FALSE(useOt->OutpaymentsShow(nym1, -1, 0));
}

TEST_F(cUseOtTest, VoucherCreate) {
	EXPECT_FALSE(useOt->VoucherWithdraw(nym1, -20, "bitcoins", "some memo", 0));

	auto accID = useOt->AccountGetId(fromAcc);
	auto ballance = opentxs::OTAPI_Wrap::GetAccountWallet_Balance(accID);

	EXPECT_FALSE(useOt->VoucherWithdraw(toNym, ballance + 1, fromAcc, "", false));

	EXPECT_TRUE(useOt->VoucherWithdraw(toNym, amount, fromAcc, "", false));
	EXPECT_TRUE(useOt->OutpaymentsShow(fromNym, 0, false));
	EXPECT_EQ(ballance - amount, opentxs::OTAPI_Wrap::GetAccountWallet_Balance(accID));
}

TEST_F(cUseOtTest, VoucherDeposit) {
	auto accID = useOt->AccountGetId(fromAcc);
	auto ballance = opentxs::OTAPI_Wrap::GetAccountWallet_Balance(accID) - amount;

	EXPECT_TRUE(useOt->VoucherDeposit(fromAcc, toNym, 0, false));
	EXPECT_TRUE(useOt->AccountInAccept(fromAcc, 0, false, false));
	EXPECT_EQ(ballance, opentxs::OTAPI_Wrap::GetAccountWallet_Balance(accID));
	EXPECT_FALSE(useOt->OutpaymentsShow(fromNym, 0, false));
}

/*
 TEST_F(cUseOtTest, VoucherSend) {
 std::string fromNym = "FT's Test Nym";
 auto result = useOt->VoucherSend(fromNym, 0, false);
 EXPECT_TRUE(result);
 }*/

