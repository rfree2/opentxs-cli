#include "gtest/gtest.h"

#include "../src/base/lib_common2.hpp"
#include "../src/base/lib_common3.hpp"

#include "../src/base/useot.hpp"

using namespace nOT::nUtils;
using namespace std;

class cUseOtAccountTest: public testing::Test {
protected:
	std::shared_ptr<nOT::nUse::cUseOT> useOt;
	string acc2 = "Bob's silver";
	string acc1 = "FT's silver";

	virtual void SetUp() {
		useOt = std::make_shared<nOT::nUse::cUseOT>("test");
		useOt->Refresh(true);
	}

	virtual void TearDown() {
		cout << "END" << endl;
	}
};

TEST_F(cUseOtAccountTest, Transfer1) {
	useOt->AccountInAccept(acc1, 1, true, false);
	useOt->AccountInAccept(acc2, 1, true, false);
	auto fromAccBalance = useOt->AccountGetBalance(acc2);
	auto toAccBalance = useOt->AccountGetBalance(acc1);
	int64_t amount = 10000;
	auto trans = useOt->AccountTransfer(acc2, acc1, amount, "test", false);
	EXPECT_TRUE(trans);
	useOt->Refresh(false);

	EXPECT_EQ(fromAccBalance-amount, useOt->AccountGetBalance(acc2));
	sleep(3);

//	useOt->Refresh(false);

	EXPECT_TRUE(useOt->AccountInAccept(acc1, 0, false, false));

	sleep(4);
	useOt->Refresh(false);
	sleep(1);
	EXPECT_EQ(toAccBalance+amount, useOt->AccountGetBalance(acc1));

}

TEST_F(cUseOtAccountTest, Transfer2) {
	auto fromAccBalance = useOt->AccountGetBalance(acc1);
	auto toAccBalance = useOt->AccountGetBalance(acc2);
	int64_t amount = 10000;
	useOt->Refresh(true);

	for(int64_t i=amount/10; i<=amount; i+=amount/10) {
		auto currBalance = useOt->AccountGetBalance(acc1);
		auto trans = useOt->AccountTransfer(acc1, acc2, amount/10, "test", false);
		EXPECT_TRUE(trans);
		useOt->Refresh(true);
		sleep(1);
		EXPECT_EQ(currBalance-amount/10, useOt->AccountGetBalance(acc1));
	}
	EXPECT_EQ(fromAccBalance - amount, useOt->AccountGetBalance(acc1));

	EXPECT_TRUE(useOt->AccountInAccept(acc1, 0, true, false));
	EXPECT_TRUE(useOt->AccountInAccept(acc2, 0, true, false));

	useOt->Refresh(true);
	EXPECT_EQ(toAccBalance+amount, useOt->AccountGetBalance(acc2));
}

