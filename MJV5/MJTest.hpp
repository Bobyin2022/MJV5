//
//  MJTest.hpp
//  MJ
//
//  Created by Bob Yin on 2022/6/29.
//

#ifndef MJTest_hpp
#define MJTest_hpp

#include <cstdio>
#include "MJCLassV5.hpp"
#include "MJHand.hpp"
#include "MJCoder.hpp"

using namespace MJClassV5;
using namespace MJClassCoder;

class MJConstTest {
private:
    MJConstV2 MJC2;
public:
    void CoutTestV2();
};

class MJClassTest {
private:
    void checkHP(MJ_C_LocationBase &L, vector<pair<string,vector <int>>> &C);
public:
    MJClassTest(){};
    void TestTable();
    void TestHand();
 //   void TestHand2();
    void TestHPType();
    void TestHPCheckSHQHP();
    void TestHPCheckCMXZDZ();
    void TestCoder();
    void TestComb();
    void TestOkTable();
    void TestHandCoder();
    void TestDKSCombList();
    void TestTing();
};

class MJTestPer { //性能测试
private:
    struct TT {
        int C[14]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        bool HP1=false,HP2=false;
    };
    unordered_map<int32_t, int> T1,T2,T3,T4; //四组用于性能的测试用例,存放14张一手牌的数据
    vector<uint32_t> CodeList1,CodeList2,CodeList3,CodeList4;
  //  vector<uint32_t> CodeList1,CodeList2,CodeList3,CodeList4;
 //   set<int> OK1,OK2;
    void Comb2List(unordered_map<int32_t, int> &T, int n, int m);
  //  void Comb2List2(vector<uint32_t>  &T, int n, int m);
    void CoderHP(unordered_map<int32_t, int> &T);
  //  void CoderHP2(vector<uint32_t> &T);
    void checkHP(MJ_C_LocationBase &L, unordered_map<int32_t, TT> &T);
public:
    void Init(); //初始化
    void TestCoderHP();
};

class unionTest{
public:
 //   MJ_U_CoderValue T;
    
    void test();
};

#endif /* MJTest_hpp */
