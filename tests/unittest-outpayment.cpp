#include "gtest/gtest.h"

#include "../src/base/lib_common2.hpp"
#include "../src/base/lib_common3.hpp"

#include "../src/base/useot.hpp"
                                                                                                                                                                                                                                                                                
using namespace nOT::nUtils;                                                                                                                                                                                                                                                    
                                                                                                                                                                                                                                                                                
class cUseOtOutPaymentTest: public testing::Test {                                                                                                                                                                                                                                  
protected:                                                                                                                                                                                                                                                                      
        std::shared_ptr<nOT::nUse::cUseOT> useOt;                                                                                                                                                                                                                               
        virtual void SetUp() {                                                                                                                                                                                     
                useOt = std::make_shared<nOT::nUse::cUseOT>("outpayment-cheque");                                                                                                                                        
                                                                                                                                                                                                                                              
                cout << "outpayment test" << endl;                                                                                                                                                                                                
        }                                                                                                                                                                                                                                     
                                                                                                                                                                                                                                              
        virtual void TearDown() {                                                                                                                                                                                                             
                cout << "tear down" << endl;                                                                                                                                                                                                  
        }
};

