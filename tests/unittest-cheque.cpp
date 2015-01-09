#include "gtest/gtest.h"

#include "../src/base/lib_common2.hpp"
#include "../src/base/lib_common3.hpp"

#include "../src/base/useot.hpp"

using namespace nOT::nUtils;

class cUseOtChequeTest : public testing::Test {
protected:
	std::shared_ptr<nOT::nUse::cUseOT> useOt;

//	nOT::nUse::cUseOT *useOt;
	string fromAcc;
	string fromNym;
	string toNym;
	string toAcc;
	string server;
	int64_t amount;

	virtual void SetUp() {
		useOt = std::make_shared<nOT::nUse::cUseOT>("test-cheque");

		//useOt = new nOT::nUse::cUseOT("test");
		fromAcc = "FT's Tokens";
		fromNym = "FT's Test Nym";
		toNym = "Trader Bob";
		toAcc = "Bob's Tokens";
		//server = useOt->ServerGetName(useOt->ServerGetDefault());
		amount = 7;

		cout << "cheque test" << endl;
	}

	virtual void TearDown() {
		cout << "tear down" << endl;
	}
};

TEST_F(cUseOtChequeTest, CreateCheque) {

	auto result = useOt->ChequeCreate(fromAcc, toNym, amount, "Transactions.com", "test cheque", false);
	EXPECT_TRUE(result);

}

TEST_F(cUseOtChequeTest, SendCheque) {
	//EXPECT_FALSE(useOt->PaymentSend("Alice", toNym, 0, false));
	auto result = useOt->PaymentSend(fromNym, toNym, 0, false);
	useOt->NymRefresh(toNym, true, false);
	useOt->AccountRefresh(fromAcc, true, false);
	ASSERT_TRUE(result);
}

TEST_F(cUseOtChequeTest, DepositCheque) {
	const auto toNymBalance = opentxs::OTAPI_Wrap::GetAccountWallet_Balance(useOt->AccountGetId(toAcc));
	const auto fromNymBalance = opentxs::OTAPI_Wrap::GetAccountWallet_Balance(useOt->AccountGetId(fromAcc));


	ASSERT_TRUE(useOt->PaymentAccept(toAcc, -1, false));

	useOt->NymRefresh(toNym, true, false);
	useOt->AccountRefresh(toAcc, true, false);

	EXPECT_EQ(toNymBalance + amount, opentxs::OTAPI_Wrap::GetAccountWallet_Balance(useOt->AccountGetId(toAcc)));
	EXPECT_EQ(fromNymBalance - amount, opentxs::OTAPI_Wrap::GetAccountWallet_Balance(useOt->AccountGetId(fromAcc)));

}

