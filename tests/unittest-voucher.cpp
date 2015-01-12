#include "gtest/gtest.h"

#include "../src/base/lib_common2.hpp"
#include "../src/base/lib_common3.hpp"

#include "../src/base/useot.hpp"

using namespace nOT::nUtils;

class cUseOtVoucherTest: public testing::Test {
protected:
	std::shared_ptr<nOT::nUse::cUseOT> useOt;
	std::string nym1;
	std::string toNym;
	std::string toAcc;
	std::string fromAcc;
	std::string fromNym;
	int32_t amount;
	int32_t amount2;

	virtual void SetUp() {
		nym1 = "Trader Bob";

		toNym = "FT's Test Nym";
		toAcc = "FT's Tokens";

		fromNym = "Trader Bob";
		fromAcc = "Bob's Tokens";

		amount = 50;
		amount2 = 1;
		useOt = std::make_shared<nOT::nUse::cUseOT>("test");
	}

	virtual void TearDown() {
		cout << "END" << endl;
	}
};

TEST_F(cUseOtVoucherTest, OutPayments) {
	EXPECT_FALSE(useOt->OutpaymentShow(nym1, 200, 0));
	EXPECT_FALSE(useOt->OutpaymentShow(nym1, -1, 0));
}

TEST_F(cUseOtVoucherTest, VoucherCreate) {
	EXPECT_FALSE(useOt->VoucherWithdraw("bitcoins", toNym, -20, "some memo", 0));

	auto accID = useOt->AccountGetId(fromAcc);
	auto ballance = opentxs::OTAPI_Wrap::GetAccountWallet_Balance(accID);

	EXPECT_FALSE(useOt->VoucherWithdraw(fromAcc, toNym, ballance + 1, "", false));

	ASSERT_TRUE(useOt->VoucherWithdraw(fromAcc, toNym, amount, "", false));
	ASSERT_TRUE(useOt->OutpaymentShow(fromNym, 0, false));
	EXPECT_EQ(ballance - amount, opentxs::OTAPI_Wrap::GetAccountWallet_Balance(accID));
}

TEST_F(cUseOtVoucherTest, VoucherCancel) {
	auto accID = useOt->AccountGetId(fromAcc);
	auto currentBallance = opentxs::OTAPI_Wrap::GetAccountWallet_Balance(accID);

	ASSERT_TRUE(useOt->VoucherCancel(fromAcc, 0, false));
	EXPECT_TRUE(useOt->AccountInAccept(fromAcc, 0, false, false));
	EXPECT_EQ(currentBallance + amount, opentxs::OTAPI_Wrap::GetAccountWallet_Balance(accID));
}

TEST_F(cUseOtVoucherTest, VoucherFullOperation) {
	const auto startBalance = opentxs::OTAPI_Wrap::GetAccountWallet_Balance(useOt->AccountGetId(fromAcc));

	// create
	ASSERT_TRUE(useOt->VoucherWithdraw(fromAcc, toNym, amount2, "", false));

	EXPECT_EQ(opentxs::OTAPI_Wrap::GetAccountWallet_Balance(useOt->AccountGetId(fromAcc)), startBalance - 1);
	EXPECT_TRUE(useOt->OutpaymentShow(fromNym, 0, false));

	// send
	ASSERT_TRUE(useOt->PaymentSend(fromNym, toNym, 0, false));
	//ASSERT_TRUE(useOt->VoucherSend(fromNym, toNym, 0, false));

	// refresh
	useOt->NymRefresh(toNym, true, false);
	useOt->AccountRefresh(toAcc, true, false);

	const auto toNymBalance = opentxs::OTAPI_Wrap::GetAccountWallet_Balance(useOt->AccountGetId(toAcc));

	// accept
	EXPECT_TRUE(useOt->PaymentAccept(toAcc, -1, false));

	useOt->NymRefresh(toNym, true, false);
	useOt->AccountRefresh(toAcc, true, false);

	EXPECT_EQ(toNymBalance + amount2, opentxs::OTAPI_Wrap::GetAccountWallet_Balance(useOt->AccountGetId(toAcc)));
}

