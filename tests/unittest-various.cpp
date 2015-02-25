#include "gtest/gtest.h"

#include "../src/base/lib_common2.hpp"
#include "../src/base/lib_common3.hpp"

#include "../src/base/useot.hpp"
#include <typeinfo>


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


