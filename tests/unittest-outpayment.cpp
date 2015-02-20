#include "gtest/gtest.h"

#include "../src/base/lib_common2.hpp"
#include "../src/base/lib_common3.hpp"

#include "../src/base/useot.hpp"
#include "../src/base/cmd.hpp"

using namespace nOT::nUtils;

class cUseOtOutpaymentTest: public testing::Test {
protected:
	std::shared_ptr<nOT::nUse::cUseOT> useOt;
	string nym1;
	string nym2;

	string preCmd;

	virtual void SetUp() {
		useOt = std::make_shared<nOT::nUse::cUseOT>("outpayment-cheque");
		nym1 = "fellowtraveler";
		nym2 = "trader bob";
		preCmd = "ot outpayment ";

		cout << "outpayment test" << endl;
	}

	virtual void TearDown() {
		cout << "tear down" << endl;
	}
};

//TEST_F(cUseOtOutpaymentTest, cmdDisplayOk) {
//	shared_ptr<nOT::nNewcli::cCmdParser> parser(new nOT::nNewcli::cCmdParser);
//	parser->Init();
//	parser->StartProcessing("ot outpayment ls", useOt).UseExecute();
//}

TEST_F(cUseOtOutpaymentTest, cmdIncorrectIndex) {
	const string incorrectIndex1 = "-1";
	const string incorrectIndex2 = "1000";
	const string showCmdForNym = preCmd + "show " + nym1 + " ";

	shared_ptr<nOT::nNewcli::cCmdParser> parser(new nOT::nNewcli::cCmdParser);
	parser->Init();

	auto execute = [&, parser](const string &cmd)-> void {
		_fact(cmd);
		parser->StartProcessing(cmd, useOt).UseExecute();
	};

	EXPECT_ANY_THROW(execute(showCmdForNym + incorrectIndex1));
	EXPECT_ANY_THROW(execute(showCmdForNym + incorrectIndex2));
}
