//
//  main.cpp
//  MJV5
//
//  Created by Bob Yin on 2022/7/13.
//

#include <iostream>
#include "MJTest.hpp"

using namespace std;
using namespace MJClassTools;


union T1{
    char a[5];
    char b[2];
};

int main(int argc, const char * argv[]) {
    MJConstTest T1;
    MJClassTest T2;
    MJTestPer TPer;
    unionTest T3;
    
//    T1.CoutTestV2();
//    MJClassTest T2;
//    T2.TestTable();
//    T2.TestHand();
//    T2.TestHPType();
//    T2.TestHPCheckSHQHP();
 //   T2.TestHPCheckCMXZDZ();
 
 //   T2.TestCoder();
//    T2.TestComb();
//    T2.TestDKSCombList();
//    T2.TestOkTable();
//    T2.TestHandCoder();
    T2.TestTing();
    
//    TPer.Init();
//    TPer.TestCoderHP();
//    T3.test();
    return 0;
}
