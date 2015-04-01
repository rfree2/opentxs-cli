#include "gtest/gtest.h"

#include "../src/base/lib_common2.hpp"
#include "../src/base/lib_common3.hpp"

#include "../src/base/useot.hpp"
#include <typeinfo>

//#include <condition_variable>
//#include <atomic>
//#include <chrono>

using namespace nOT::nUtils;

class cUseOtTest: public testing::Test {
protected:
	std::shared_ptr<nOT::nUse::cUseOT> useOt;
	string preCmd;
	string toNym;
	string toAcc;
	string server;

	enum  color { red, blue };

	virtual void SetUp() {
		useOt = std::make_shared<nOT::nUse::cUseOT>("test-record");
		preCmd = "ot record ";
		toNym = "trader bob";
		toAcc = "Bob's bitcoin";
		server = "Transactions.com";

		cout << "record test" << endl;
		useOt->Init();
	}

	virtual void TearDown() {
		cout << "tear down" << endl;
	}
};

/*
TEST_F(cUseOtTest, BetterPing) {

	std::condition_variable cv;
	std::mutex cv_m;
	std::atomic<int> isPing{0};

	auto timeout = [&, this]() -> void {
		std::unique_lock<std::mutex> lk(cv_m);
		const int interval_sec = 2;
		auto start = std::chrono::system_clock::now();
		int all_time = interval_sec;
		while(true) {
			if(cv.wait_until(lk, std::chrono::system_clock::now() + std::chrono::milliseconds(interval_sec * 1000)), (isPing == 1) ) {
				cout << zkr::cc::fore::lightgreen << "Connection successful" << zkr::cc::console << endl;
				break;
			} else {
				cout << zkr::cc::fore::yellow << "ping server " << all_time << " sec." << zkr::cc::console << endl;
			}
			all_time += interval_sec;
		}
		_dbg3("end lambda");
	};
	std::thread thread_check(timeout);
	thread_check.detach();
	auto ping = opentxs::OTAPI_Wrap::pingNotary(useOt->ServerGetDefault(), useOt->NymGetDefault());
	isPing = 1;
	_info("ok");
	cv.notify_all();

}*/

TEST_F(cUseOtTest, NymAcc) {
	useOt->Init();
	EXPECT_EQ(toNym, useOt->AccountGetNym(toAcc));

}

TEST_F(cUseOtTest, Basket) {
	useOt->Init();
	auto assets = useOt->AssetGetAllNames();
	_dbg3(assets.size());
	for (auto asset : assets) {
		auto assetID = useOt->AssetGetId(asset);
		if (!opentxs::OTAPI_Wrap::IsBasketCurrency(assetID)) {
			_info(asset << " is not a basket currency");
		} else {
			auto count = opentxs::OTAPI_Wrap::Basket_GetMemberCount(assetID);
			_mark("count = " << count);
		}
	}
}

TEST_F(cUseOtTest, AccGetNym) {
	ASSERT_TRUE(useOt->Init());
	auto allAccs = useOt->AccountGetAllNames();
	ASSERT_TRUE(!allAccs.empty());

	for(auto acc : allAccs) {
		auto nym = useOt->AccountGetNym(acc);
		_dbg1(nym << "\t" << acc);
		EXPECT_EQ(useOt->NymGetId(nym), useOt->AccountGetNymID(acc));
	}
}

TEST_F(cUseOtTest, Nymbox) {
	const auto srvID = useOt->ServerGetId(server);
	for(auto nymID : useOt->NymGetAllIDs()) {
		auto proccessNymbox = opentxs::OTAPI_Wrap::processNymbox(srvID, nymID);
		EXPECT_GT(proccessNymbox, -1);
		auto nymboxNumber = opentxs::OTAPI_Wrap::getNymbox(srvID, nymID);
		_info(useOt->NymGetName(nymID) << ": " << nymboxNumber << " nymbox");

		auto nymbox = opentxs::OTAPI_Wrap::LoadNymbox(srvID, nymID);
		_dbg2(nymbox);


		auto txnNum = opentxs::OTAPI_Wrap::GetNym_TransactionNumCount(srvID, nymID);
		_info(txnNum);
	}
}

TEST_F(cUseOtTest, BasketDisplay) {
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
	// EXPECT_TRUE(ok);
}

TEST_F(cUseOtTest, Home) {
	auto home = cFilesystemUtils::GetHomeDir();
	_note(home);
	string withTilde = "~/test";
	string withoutTilde = home + "/test";

	EXPECT_EQ(withoutTilde, cFilesystemUtils::TildeToHome(withTilde));
}

TEST_F(cUseOtTest, vector) {
	using namespace nOper;
	vector<string> testNyms { "Betty", "Ted", "Wilma", "Fred" };
	vector<string> testNyms_copy(testNyms);

	testNyms = testNyms - testNyms_copy.at(2);
	EXPECT_EQ(testNyms_copy.size()-1, testNyms.size());
	EXPECT_EQ(testNyms.at(2), testNyms_copy.at(3));

	vector<string> one { "test" };
	one = one - string("test");
	EXPECT_TRUE(one.empty());
}
