#include "gtest/gtest.h"

#include "../src/base/lib_common2.hpp"
#include "../src/base/lib_common3.hpp"

#include "../src/base/useot.hpp"
                                                                                                                                                                                                                                                                                
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
                toNym = "Trader Bob";
        		toAcc = "Bob's Tokens";
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

	EXPECT_FALSE(useOt->RecordClear("silver issuer", false, false));


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

