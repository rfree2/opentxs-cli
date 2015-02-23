#include "gtest/gtest.h"

#include "../src/base/lib_common2.hpp"
#include "../src/base/lib_common3.hpp"

#include "../src/base/useot.hpp"
#include <typeinfo>

#include "../src/base/addressbook.hpp"

using namespace nOT::nUtils;

class cUseOtRecordTest: public testing::Test {
protected:
	std::shared_ptr<nOT::nUse::cUseOT> useOt;
	string preCmd;
	string toNym;
	string toAcc;
	string server;

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

TEST_F(cUseOtRecordTest, ClearExpired) {
//	EXPECT_TRUE(useOt->RecordClear(toAcc, server, false, false));

//	EXPECT_FALSE(useOt->RecordClear("silver issuer", false, false));

//	EXPECT_TRUE(useOt->RecordDisplay(toAcc, server, false));
}

TEST_F(cUseOtRecordTest, ClearAll) {
//	EXPECT_TRUE(useOt->RecordClear(toAcc, server, true, false));
//	EXPECT_FALSE(useOt->RecordClear(toAcc, server, true, false));

}

TEST_F(cUseOtRecordTest, NymAcc) {
	useOt->Init();
	EXPECT_EQ(toNym, useOt->AccountGetNym(toAcc));

}

TEST_F(cUseOtRecordTest, Basket) {
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

TEST_F(cUseOtRecordTest, Addressbook) {
	useOt->Init();

	const string nym = "Alice";
	const auto nymID = useOt->NymGetId(nym);

	const string nym2 = "issuer";
	const auto nymID2 = useOt->NymGetId(nym2);

	auto addressbook = nOT::AddressBook::Load(nymID);

	addressbook.add(toNym, useOt->NymGetId(toNym));
	addressbook.add(nym2, nymID2);

	addressbook.display();
}
