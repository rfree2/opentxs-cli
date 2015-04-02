#include "gtest/gtest.h"

#include "../src/base/lib_common2.hpp"
#include "../src/base/lib_common3.hpp"

#include "../src/base/useot.hpp"
#include "../src/base/cmd.hpp"

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
	sleep(1);

//	useOt->Refresh(false);

	EXPECT_TRUE(useOt->AccountInAccept(acc1, 0, false, false));

	sleep(2);
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

TEST_F(cUseOtAccountTest, Cmd) {
	auto nyms = useOt->NymGetAllNames();
	auto accs = useOt->AccountGetAllNames();
	_dbg3("getting default");

	auto defaultAccID = useOt->AccountGetDefault();
	_dbg2(defaultAccID);
	ASSERT_TRUE(defaultAccID != "-" && !defaultAccID.empty());
	auto defaultAcc = useOt->AccountGetName(defaultAccID);
	_dbg2(defaultAcc);
	ASSERT_TRUE(defaultAcc != "-" && !defaultAcc.empty());

	defaultAcc = "\"" + defaultAcc + "\" ";

	_note("default account: " << defaultAcc);

	if(nyms.size() < 2 || accs.size() < 2) return;
	auto acc1 = "\"" + accs.front() + "\" ";
	auto acc2 = "\"" + accs.back() + "\" ";

	shared_ptr<nOT::nNewcli::cCmdParser> parser(new nOT::nNewcli::cCmdParser);
	parser->Init();

	auto cmd = [this, parser] (string command) {
		command = "ot " + command + " --dryrun ";
		_mark(command);
		parser->StartProcessing(command, useOt).UseExecute();
	};

	string acc_cmd = "account transfer-from ";
	EXPECT_ANY_THROW(cmd( acc_cmd + acc1 + acc1 + "2"));
	EXPECT_NO_THROW(cmd( acc_cmd + acc1 + acc2 + "2"));

	string acc_cmd2 = "account transfer-to ";
	string acc_good = (acc1 == defaultAcc)? acc2 : acc1;
	string acc_cmd2_good = acc_cmd2 + acc_good + " 2";
	EXPECT_NO_THROW(cmd(acc_cmd2_good));

	string acc_cmd2_bad = acc_cmd2 + defaultAcc + " 2";
	EXPECT_ANY_THROW(cmd(acc_cmd2_bad));

}
