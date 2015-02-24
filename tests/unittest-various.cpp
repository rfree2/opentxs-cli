#include "gtest/gtest.h"

#include "../src/base/lib_common2.hpp"
#include "../src/base/lib_common3.hpp"

#include "../src/base/useot.hpp"
#include <typeinfo>

#include "../src/base/addressbook.hpp"

using namespace nOT::nUtils;

class cUseOtTest: public testing::Test {
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

TEST_F(cUseOtTest, AddressbookAdd) {
	useOt->Init();

	const string nym = "Alice";
	const auto nymID = useOt->NymGetId(nym);

	const string nym2 = "issuer";
	const auto nymID2 = useOt->NymGetId(nym2);

	auto addressbook = nOT::AddressBook::Load(nymID);

	EXPECT_TRUE(addressbook.add(toNym, useOt->NymGetId(toNym)));
	EXPECT_TRUE(addressbook.nymExist(useOt->NymGetId(toNym)));
	EXPECT_FALSE(addressbook.add(toNym, useOt->NymGetId(toNym)));
	EXPECT_FALSE(addressbook.add(nym2, nymID2));

	addressbook.display();
}

TEST_F(cUseOtTest, AddressbookRemove) {
	const string nym = "Alice";
	const auto nymID = useOt->NymGetId(nym);
	auto addressbook = nOT::AddressBook::Load(nymID);

	const auto removingNymID = useOt->NymGetId(toNym);
	EXPECT_TRUE(addressbook.remove(removingNymID));
	EXPECT_FALSE(addressbook.nymExist(removingNymID));
	EXPECT_FALSE(addressbook.remove(removingNymID));

}
