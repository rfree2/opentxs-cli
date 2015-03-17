#include "gtest/gtest.h"

#include "../src/base/lib_common2.hpp"
#include "../src/base/lib_common3.hpp"

#include "../src/base/useot.hpp"

using namespace nOT::nUtils;

class cUseOtChequeTest: public testing::Test {
protected:
	std::shared_ptr<nOT::nUse::cUseOT> useOt;

	string fromAcc;
	string fromNym;
	string toNym;
	string toAcc;
	string server;
	int64_t amount;

	virtual void SetUp() {
		useOt = std::make_shared<nOT::nUse::cUseOT>("test-cheque");

		toNym = "fellowtraveler";
		toAcc = "FT's dollars";

		fromNym = "trader bob";
		fromAcc = "Bob's dollars";

		server = "Transactions.com";
		amount = 3;

		cout << "cheque test" << endl;
	}

	virtual void TearDown() {
		cout << "tear down" << endl;
		sleep(10);
	}
};


TEST_F(cUseOtChequeTest, Create) {
	auto result = useOt->ChequeCreate(fromAcc, fromNym, toNym, amount, server, "test_cheque", false);
	EXPECT_TRUE(result);
	useOt->Refresh(true);
}

TEST_F(cUseOtChequeTest, SendCheque) {
	//EXPECT_FALSE(useOt->PaymentSend("Alice", toNym, 0, false));
	sleep(5);
	auto result = useOt->OutpaymentSend(fromNym, toNym, 0, false);
	useOt->NymRefresh(toNym, true, false);
	useOt->AccountRefresh(fromAcc, true, false);
	ASSERT_TRUE(result);
	useOt->Refresh(true);
	sleep(10);

}

TEST_F(cUseOtChequeTest, accept) {
	const auto toNymBalance = opentxs::OTAPI_Wrap::GetAccountWallet_Balance(useOt->AccountGetId(toAcc));
	const auto fromNymBalance = opentxs::OTAPI_Wrap::GetAccountWallet_Balance(useOt->AccountGetId(fromAcc));

	ASSERT_TRUE(useOt->PaymentAccept(toAcc, -1, false));

	sleep(15);
	useOt->NymRefresh(toNym, true, false);
	useOt->AccountRefresh(toAcc, true, false);

	sleep(150);

	EXPECT_EQ(toNymBalance + amount, opentxs::OTAPI_Wrap::GetAccountWallet_Balance(useOt->AccountGetId(toAcc)));
	EXPECT_EQ(fromNymBalance - amount, opentxs::OTAPI_Wrap::GetAccountWallet_Balance(useOt->AccountGetId(fromAcc)));
	useOt->Refresh(true);
}
/*
TEST_F(cUseOtChequeTest, CreateAndDiscard) {
	auto create = useOt->ChequeCreate(fromAcc, fromNym, toNym, amount, server, "to discard", false);
	ASSERT_TRUE(create);

	auto discard = useOt->ChequeDiscard(fromAcc, fromNym, 0, false);
	EXPECT_TRUE(discard);

	useOt->NymRefresh(toNym, true, false);

	auto send = useOt->PaymentSend( fromNym, toNym, 0, false);
	ASSERT_TRUE(send);

	_dbg1(send);
	useOt->NymRefresh(toNym, true, false);

	ASSERT_TRUE(useOt->PaymentAccept(toAcc, -1, false));
	useOt->Refresh(true);
}*/

TEST_F(cUseOtChequeTest, Cleanup) {
	ASSERT_TRUE(useOt->AccountInAccept(fromAcc, 0, true, false));
}

