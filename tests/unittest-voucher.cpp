#include "gtest/gtest.h"

#include "../src/base/lib_common2.hpp"
#include "../src/base/lib_common3.hpp"

#include "../src/base/useot.hpp"

#include "../src/base/cmd.hpp"
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
		nym1 = "trader bob";

		toNym = "fellowtraveler";
		toAcc = "FT's dollars";

		fromNym = "trader bob";
		fromAcc = "Bob's dollars";

		amount = 3;
		amount2 = 5;
		useOt = std::make_shared<nOT::nUse::cUseOT>("test");

		_note("amount1 " << amount);
		_note("amount2 " << amount2);

		useOt->Refresh(true);
	}

	virtual void TearDown() {
		cout << "END" << endl;
	}
};

TEST_F(cUseOtVoucherTest, VoucherCreate) {
	ASSERT_TRUE(useOt->Init());
	const string acc = "silver_issuer";

	_info(useOt->AccountGetNym(acc));
	ASSERT_FALSE(useOt->AccountGetNym(acc).empty());

	EXPECT_FALSE(useOt->VoucherWithdraw(acc, useOt->AccountGetNym(acc), toNym, -20, "some memo", 0));

	auto fromAccID = useOt->AccountGetId(fromAcc);

	EXPECT_FALSE(
			useOt->VoucherWithdraw(fromAcc, useOt->AccountGetNym(fromAcc), toNym, opentxs::OTAPI_Wrap::GetAccountWallet_Balance(fromAccID) + 1, "memo",
					false));

	sleep(3);
	const auto balance = opentxs::OTAPI_Wrap::GetAccountWallet_Balance(fromAccID);

	EXPECT_TRUE(useOt->VoucherWithdraw(fromAcc, useOt->AccountGetNym(fromAcc), toNym, amount, "memo", false));
	EXPECT_TRUE(useOt->OutpaymentShow(fromNym, 0, false));

	sleep(10);
	useOt->Refresh(true);

	sleep(120);
	_info("checking balance:");
	EXPECT_EQ(balance - amount, opentxs::OTAPI_Wrap::GetAccountWallet_Balance(fromAccID));
}

TEST_F(cUseOtVoucherTest, VoucherCancel) {
	auto accID = useOt->AccountGetId(fromAcc);
	auto currentBallance = opentxs::OTAPI_Wrap::GetAccountWallet_Balance(accID);

	ASSERT_TRUE(useOt->VoucherCancel(fromAcc, fromNym, 0, false));
	EXPECT_TRUE(useOt->AccountInAccept(fromAcc, 0, false, false));

	useOt->NymRefresh(fromNym, true, false);
	useOt->AccountRefresh(fromAcc, true, false);
	sleep(15);
	EXPECT_EQ(currentBallance + amount, opentxs::OTAPI_Wrap::GetAccountWallet_Balance(accID));
}

TEST_F(cUseOtVoucherTest, Create) {
	const auto startBalance = opentxs::OTAPI_Wrap::GetAccountWallet_Balance(useOt->AccountGetId(fromAcc));

	// create
	ASSERT_TRUE(useOt->VoucherWithdraw(fromAcc, useOt->AccountGetNym(fromAcc), toNym, amount2, "", false));
	sleep(15);
	_info("after sleeping");
	EXPECT_EQ(startBalance - amount2, opentxs::OTAPI_Wrap::GetAccountWallet_Balance(useOt->AccountGetId(fromAcc)));
	EXPECT_TRUE(useOt->OutpaymentShow(fromNym, 0, false));
}

TEST_F(cUseOtVoucherTest, Send) {
	// send
	ASSERT_TRUE(useOt->PaymentSend(toNym, fromNym, 0, false));

	// refresh
	useOt->NymRefresh(toNym, false, false);
	useOt->AccountRefresh(toAcc, false, false);
}

TEST_F(cUseOtVoucherTest, Accept) {
	const auto toNymBalance = opentxs::OTAPI_Wrap::GetAccountWallet_Balance(useOt->AccountGetId(toAcc));

	// accept
	sleep(5);
	EXPECT_TRUE(useOt->PaymentAccept(toAcc, -1, false));

	sleep(30);
	EXPECT_EQ(toNymBalance + amount2, opentxs::OTAPI_Wrap::GetAccountWallet_Balance(useOt->AccountGetId(toAcc)));
}

TEST_F(cUseOtVoucherTest, OutpaymentRemove) {
	EXPECT_TRUE(useOt->OutpaymentRemove(fromNym, 0, true, false));
	EXPECT_FALSE(useOt->OutpaymentShow(fromNym, 0, false));

	useOt->OutpaymentDisplay(nym1, false);
}

TEST_F(cUseOtVoucherTest, Cleanup) {
	const int maxTimes = 10;
	bool display = true;
	int32_t txnCount = -1;
	const string server = "Transaction.com";
	for (int i = 0; i < maxTimes; ++i) {
		string inbox = opentxs::OTAPI_Wrap::LoadInbox(useOt->ServerGetId(server), useOt->NymGetId(fromNym),
				useOt->AccountGetId(fromAcc));
		ASSERT_TRUE(!inbox.empty());
		txnCount = opentxs::OTAPI_Wrap::Ledger_GetCount(useOt->ServerGetId(server), useOt->NymGetId(fromNym),
				useOt->AccountGetId(fromAcc), inbox);

		_info("txn: " << txnCount);
		if(txnCount == 0) break;

		useOt->AccountInAccept(fromAcc, 0, true, false);
		display = useOt->AccountInDisplay(fromAcc, false);
	}
	_dbg2("end loop");
	EXPECT_EQ(0, txnCount);
}

