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
	auto result = useOt->PaymentSend(fromNym, toNym, 0, false);
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
/*
TEST_F(cUseOtChequeTest, BasketNew) {

	useOt->BasketNew();
}

TEST_F(cUseOtChequeTest, BasketDisplay) {
	using namespace opentxs;
	auto tmp = useOt->Init();

	auto assetCount = OTAPI_Wrap::GetAssetTypeCount();
	_dbg2("asset count: " << assetCount);

	int32_t index = 0;
	bool ok = false;

	for (int i = 0; i < assetCount; ++i) {
		const auto assetID = OTAPI_Wrap::GetAssetType_ID(i);
		const auto asset = useOt->AssetGetName(assetID);

		_dbg2("asset: " << asset);

		auto isb = OTAPI_Wrap::IsBasketCurrency(assetID);
		_dbg2(isb);
		ok = ok || isb;
	}
	EXPECT_TRUE(ok);
}*/
TEST_F(cUseOtChequeTest, Cleanup) {
	const int maxTimes = 10;
	bool display = true;
	int32_t txnCount = -1;

	for (int i = 0; i < maxTimes; ++i) {
		string inbox = opentxs::OTAPI_Wrap::LoadInbox(useOt->ServerGetId(server), useOt->NymGetId(fromNym),
				useOt->AccountGetId(fromAcc));

		if(inbox.empty()) {
			txnCount = 0;
			break;
		}
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

