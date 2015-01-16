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
	string preCmd;

	virtual void SetUp() {
		useOt = std::make_shared<nOT::nUse::cUseOT>("outpayment-cheque");
		nym1 = "FT's Test Nym";
		preCmd = "ot outpayment ";

		cout << "outpayment test" << endl;
	}

	virtual void TearDown() {
		cout << "tear down" << endl;
	}
};

TEST_F(cUseOtOutpaymentTest, cmdOk) {
	shared_ptr<nOT::nNewcli::cCmdParser> parser(new nOT::nNewcli::cCmdParser);
	parser->Init();
	auto tmp = parser->StartProcessing("ot outpayment ls", useOt);
	tmp.UseExecute();

}

TEST_F(cUseOtOutpaymentTest, cmdFail) {
	shared_ptr<nOT::nNewcli::cCmdParser> parser(new nOT::nNewcli::cCmdParser);
	parser->Init();

	string cmd = preCmd + "show " + nym1 + " -1";
	_fact(cmd);
	EXPECT_ANY_THROW(parser->StartProcessing(cmd, useOt).UseExecute());

	cmd = preCmd + "show " + nym1 + " 1000";
	_fact(cmd);
	EXPECT_ANY_THROW(parser->StartProcessing(cmd, useOt).UseExecute());

}
