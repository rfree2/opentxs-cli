#include "gtest/gtest.h"

#include "../src/base/lib_common2.hpp"
#include "../src/base/lib_common3.hpp"

#include "../src/base/useot.hpp"
#include <typeinfo>

#include "../src/base/addressbook.hpp"

using namespace nOT::nUtils;

class cUseOtAddressBookTest: public testing::Test {
protected:
	std::shared_ptr<nOT::nUse::cUseOT> useOt;
	string preCmd;
	string toNym;
	string toAcc;
	string server;

	enum color {
		red, blue
	};

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

TEST_F(cUseOtAddressBookTest, Operation) {
	useOt->Init();

	const string nym = "Alice";
	const auto nymID = useOt->NymGetId(nym);

	const string nym2 = "issuer";
	const auto nymID2 = useOt->NymGetId(nym2);

	auto addressbook = nOT::AddressBookStorage::Get(nymID);

	EXPECT_TRUE(addressbook->add(toNym, useOt->NymGetId(toNym)));
	EXPECT_TRUE(addressbook->nymExist(useOt->NymGetId(toNym)));
	EXPECT_FALSE(addressbook->add(toNym, useOt->NymGetId(toNym)));
	EXPECT_FALSE(addressbook->add(nym2, nymID2));

	addressbook->display();

	const auto removingNymID = useOt->NymGetId(toNym);
	EXPECT_TRUE(addressbook->remove(removingNymID));
	EXPECT_FALSE(addressbook->nymExist(removingNymID));
	EXPECT_FALSE(addressbook->remove(removingNymID));

	addressbook->display();
}

TEST_F(cUseOtAddressBookTest, Display) {
	const string nym = "Alice";
	const auto nymID = useOt->NymGetId(nym);
	auto addressbook = nOT::AddressBookStorage::Get(nymID);
	addressbook->display();

}

TEST_F(cUseOtAddressBookTest, ClearObject) {
	for (auto nym : useOt->NymGetAllNames()) {
		EXPECT_TRUE(useOt->AddressBookDisplay(nym, false));
	}

	sleep(3);

	for (auto nym : useOt->NymGetAllNames()) {
		EXPECT_TRUE(useOt->AddressBookDisplay(nym, false));
	}
	nOT::AddressBookStorage::ForceClear();
}

TEST_F(cUseOtAddressBookTest, AddAndFind) {
	const string toAddNymName = "alice2";
	const string toAddNymID = "otxVhEonf15M4oN4gt8WSz2VrPV8tDFVxymr";
	const string ownerNymName = "Alice";
	const string ownerNymID = useOt->NymGetId(ownerNymName);

	EXPECT_FALSE(ownerNymID.empty());
	auto addressBook = nOT::AddressBookStorage::Get(ownerNymID);

	auto checkExist = [&](bool shouldExist)-> void {
		_info("check exist");
		auto exist = addressBook->nymExist(toAddNymID);
		if(shouldExist) {
			EXPECT_FALSE(addressBook->nymGetName(toAddNymID).empty());
			EXPECT_TRUE(addressBook->nymExist(toAddNymID));
		} else {
			EXPECT_TRUE(addressBook->nymGetName(toAddNymID).empty());
			EXPECT_FALSE(addressBook->nymExist(toAddNymID));
		}
	};

	checkExist(false);
	auto add = addressBook->add(toAddNymName, toAddNymID);
	addressBook->display();
	EXPECT_TRUE(add);

	checkExist(true);
	auto remove = addressBook->remove(toAddNymID);
	EXPECT_TRUE(remove);
	addressBook->display();
	checkExist(false);
}

