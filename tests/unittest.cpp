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
		nym1 = "Trader Bob";
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
	EXPECT_FALSE(useOt->VoucherWithdraw("bitcoins", -20, "some memo", 0));

	auto accID = useOt->AccountGetId(fromAcc);
	auto ballance = opentxs::OTAPI_Wrap::GetAccountWallet_Balance(accID);

	EXPECT_FALSE(useOt->VoucherWithdraw(fromAcc, ballance + 1,  "", false));

	ASSERT_TRUE(useOt->VoucherWithdraw(fromAcc, amount, "", false));
	ASSERT_TRUE(useOt->OutpaymentsShow(fromNym, 0, false));
	EXPECT_EQ(ballance - amount, opentxs::OTAPI_Wrap::GetAccountWallet_Balance(accID));
}

TEST_F(cUseOtTest, VoucherDeposit) {
	auto accID = useOt->AccountGetId(fromAcc);
	auto currentBallance = opentxs::OTAPI_Wrap::GetAccountWallet_Balance(accID);

	ASSERT_TRUE(useOt->VoucherDeposit(fromAcc, fromNym, 0, false));
	EXPECT_TRUE(useOt->AccountInAccept(fromAcc, 0, false, false));
	EXPECT_EQ(currentBallance + amount, opentxs::OTAPI_Wrap::GetAccountWallet_Balance(accID));
	//EXPECT_FALSE(useOt->OutpaymentsShow(fromNym, 0, false));
}

/*
 TEST_F(cUseOtTest, VoucherSend) {
 std::string fromNym = "FT's Test Nym";
 auto result = useOt->VoucherSend(fromNym, 0, false);
 EXPECT_TRUE(result);
 }*/

