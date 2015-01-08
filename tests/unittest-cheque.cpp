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
		useOt = std::make_shared<nOT::nUse::cUseOT>("test");

		fromAcc = "FT's Tokens";
		fromNym = "FT's Test Nym";
		toNym = "Trader Bob";
		toAcc = "Bob's Tokens";
		server = useOt->ServerGetName(useOt->ServerGetDefault());
		amount = 10;

		cout << "cheque test" << endl;
	}

	virtual void TearDown() {
		cout << "" << endl;
	}
};

TEST_F(cUseOtChequeTest, CreateCheque) {

	auto result = useOt->ChequeCreate(fromAcc, toNym, amount, server, "test cheque", false);
	EXPECT_TRUE(result);

}

TEST_F(cUseOtChequeTest, SendCheque) {
	//EXPECT_FALSE(useOt->PaymentSend("Alice", toNym, 0, false));
	ASSERT_TRUE(useOt->PaymentSend(fromNym, toNym, 0, false));
}

TEST_F(cUseOtChequeTest, DepositCheque) {
	useOt->NymRefresh(toNym, true, false);
	useOt->AccountRefresh(fromAcc, true, false);

	const auto toNymBalance = opentxs::OTAPI_Wrap::GetAccountWallet_Balance(useOt->AccountGetId(toAcc));

	EXPECT_TRUE(useOt->PaymentAccept(toAcc, 0, false));

	useOt->NymRefresh(toNym, true, false);
	useOt->AccountRefresh(toAcc, true, false);

	EXPECT_EQ(toNymBalance + amount, opentxs::OTAPI_Wrap::GetAccountWallet_Balance(useOt->AccountGetId(toAcc)));

}

