//
//  MJTest.cpp
//  MJ
//
//  Created by Bob Yin on 2022/6/29.
//

#include <iostream>

using namespace std;

#include "MJTools.hpp"
#include "MJTest.hpp"
using namespace MJClassTools;
using namespace MJClassV5;
using namespace MJHPTools;

void MJConstTest::CoutTestV2(){
    
    string s;
    
    cout << "测试——麻将常数类V2——Begin" << endl;

    cout << "  牌类型：" ;
    for (int i=0; i<5; i++)
        cout << MJC2.getHuaSeHZ(i)<< "、";
    cout << endl;
    cout << "  筒：" ;
    for (int i=0; i<9; i++)
        cout << MJC2.getCardHZ(MJ_E_HS_Tong*10+i)<< "、";
    cout << endl;
    cout << "  条：" ;
    for (int i=0; i<9; i++)
        cout << MJC2.getCardHZ(MJ_E_HS_Tiao*10+i)<< "、";
    cout << endl;
    cout << "  万：" ;
    for (int i=0; i<9; i++)
        cout << MJC2.getCardHZ(MJ_E_HS_Wan*10+i)<< "、";
    cout << endl;
    cout << "  风：" ;
    for (int i=0; i<7; i++)
        cout << MJC2.getCardHZ(MJ_E_HS_Feng*10+i)<< "、";
    cout << endl;
    cout << "  花：" ;
    for (int i=0; i<8; i++)
        cout << MJC2.getCardHZ(MJ_E_HS_Hua*10+i)<< "、";
    cout << endl;
    
    cout << "  牌型：";
    for (int i=0; i<MJ_E_PX_End; i++)
        cout << MJC2.getPaiXingHZ(i) << "、";
    cout << endl;
   
    cout << "测试——麻将常数类——Over" << endl;
}

void MJClassTest::TestTable(){
    MJ_C_Table T;
    
    cout << "===   一桌麻将  测试   ===" << endl;
    cout << "-  上海清混碰  测试   -" << endl;
    MJ_C_Loc_SHQHP SH1;
    T.reSet(SH1.Loc);
    T.show();
    
    cout << "-  上海敲痳  测试   -" << endl;
    MJ_C_Loc_SHQM SH2;
    T.reSet(SH2.Loc);
    T.show();

    cout << "-  川麻血战到底  测试   -" << endl;
    MJ_C_Loc_CMXZDZ CM;
    T.reSet(CM.Loc);
    T.show();
    
    cout << "===   一桌麻将测试 Over  ===" << endl;
}

void MJClassTest::TestHand(){
    MJ_C_Table T;
    MJ_C_Loc_SHQHP SH;
    MJConstV2 Tool;
    
    MJ_HandCard MH,MH1;
    
    T.reSet(SH.Loc);
    
    int c[13];
    cout << "===   一手牌、理牌  测试   ===" << endl;
    cout << "-   一手牌  测试   -" << endl;
    c[0]=0;c[1]=3;c[2]=3;c[3]=2;
    c[4]=44;c[5]=1;c[6]=2;c[7]=3;
    c[8]=24;c[9]=1;c[10]=2;c[11]=3;
    c[12]=36;
    MH.MoPai(c,13);
    
    int R=-1;
    
    MH.Show();
    cout << "能否碰二筒：" << MH.checkMPeng(1) << endl;
    cout << "能否碰一筒：" << MH.checkMPeng(0) << endl;
    cout << "能否杠三筒：" << MH.checkMGang(2) << endl;
    cout << "能否杠一筒：" << MH.checkMGang(0) << endl;
    cout << "能否暗杠：  " << MH.checkAGang(R) << " | " << R << endl;
    cout << "能否吃五筒：" << MH.checkChi(4) << endl;
    cout << "能否吃六筒：" << MH.checkChi(5) << endl;
    cout << "能否吃四万：" << MH.checkChi(23) << endl;
    cout << "能否吃五万：" << MH.checkChi(24) << endl;
    cout << "--  别人打二筒，碰出：---" << endl;
    cout << "打出：" << Tool.getCardHZ(MH.pengOut(1)) << endl;
    MH.Show();
    cout << "--  别人打三筒，明杠，补进一筒：---" << endl;
    cout << "打出：" << Tool.getCardHZ(MH.GangOut(MJ_E_PX_MGang,2, 0)) << endl;
    MH.Show();
    cout << "--  摸六万，暗杠出四筒，补进五万：---" << endl;
    MH.MoPai(25);
    cout << "打出：" << Tool.getCardHZ(MH.GangOut(MJ_E_PX_AGang,R, 24)) << endl;
    MH.Show();
    cout << "--  补杠出二筒，补进三万：---" << endl;
    cout << "打出：" << Tool.getCardHZ(MH.GangOut(MJ_E_PX_MGang,1, 22)) << endl;
    MH.Show();
    cout << "--  吃四万：---" << endl;
    cout << "打出：" << Tool.getCardHZ(MH.chiOut(23)) << endl;
    MH.Show();
    cout << "===   一手牌测试  Over  ===" << endl << endl;
    
    cout << "-   胡牌  测试   -" << endl;
    MH.Show();
    {   MJ_S_HPFan HPF;
        MJ_HPCalc HP(SH);
        vector<MJ_C_HPJieGuo> HPD;
        HP.isHu(MH,HPD);
        cout << "能否胡牌？ " << HP.GetErrMsg() << endl;
    }
    cout << "--  摸1筒---" << endl;
    MH.MoPai(0);
    MH.Show();
    {   MJ_S_HPFan HPF;
        MJ_HPCalc HP(SH);
        vector<MJ_C_HPJieGuo> HPD;
        HP.isHu(MH,HPD);
        cout << "能否胡牌？ " << HP.GetErrMsg() << endl;
        HPD[0].Show();
    }
    
    cout << "顺差计算" << endl;
    int d[14]={200, 511, 614, 206, 127};
    MH1.MoPai(d, 5);
    MH1.Show();
    
    cout << "特殊牌型图" << endl;
/*    MJ_C_CardsPos CL;
    CL.Add1PX(0);
    CL.Add1PX(1);
    CL.Add1PX(2);
    CL.Add1PX(3);
    CL.Add1PX(3);
    CL.Add1PX(3);
    CL.Add1PX(11);
    CL.Add1PX(12);
    CL.Add1PX(13);
    CL.Add1PX(24);
    CL.Add1PX(25);
    CL.Add1PX(26);
    CL.Add1PX(6);
    CL.Add1PX(6);
    cout << CL.isHu() << endl;
    CL.Show();
  */
}

void MJClassTest::TestHPType(){
    MJ_C_HPAllKinds HPT;
    
    cout << "===   胡牌牌型模式定义测试   ===" << endl;
    HPT.ShowBase();
    cout << "===   胡牌牌型模式定义测试（Over）   ===" << endl;
    
}

void MJClassTest::checkHP(MJ_C_LocationBase &L, vector<pair<string,vector <int>>> &C){
    MJ_HPCalc HP(L);
    MJConstV2 Tool;
    int n=0;
    
    for (auto t:C){
        cout << "====测试：" << n << '|' << t.first << "| (Begin)" << endl;
        MJ_HandCard MH;
        vector<MJ_C_HPJieGuo> HPD;
        MH.MoPai(t.second);
        HP.isHu(MH,HPD);
        cout << "能否胡牌？ " << HP.GetErrMsg() << endl;
        for(auto n:HPD) n.Show();
        cout << "====测试：" << n++ << '|' << t.first << '|' << " (End) --" << endl << endl;
    }
}

void MJClassTest::TestHPCheckSHQHP(){
    MJ_C_Loc_SHQHP SH;
    vector<pair<string,vector <int>>> C={
/*        {"乱风向",{30,30,31, 31,31,31, 32,32,32, 33,33,34, 35,35}}, //乱风向
        {"字一色",{30,30,30, 31,31,31, 32,32,32, 33,33,33, 34,34}}, //字一色
        {"乱风向",{30,30,31, 31,31,31, 32,32,32, 33,33,34, 35,35}}, //乱风向
        {"清  碰",{ 0, 0, 0,  1, 1, 1,  3, 3, 3,  4, 4, 4,  5,15}}, //清碰
        {"混  碰",{ 0, 0, 0,  1, 1, 1,  3, 3, 3,  4, 4, 4, 33, 33}}, //混碰
 */
        {"清一色",{ 0, 0, 0,  1, 1, 1,  2, 2, 2,  3, 3, 4,  4,  5}},//混碰
    };
    
    cout << endl << "===   " << SH.Loc.LocName << "-胡牌牌型检查测试   ===" << endl;
    checkHP(SH,C);
    cout << "===   " << SH.Loc.LocName << "-胡牌牌型检查测试（Over）   ===" << endl;

    
}

void MJClassTest::TestHPCheckCMXZDZ(){
    MJ_C_Loc_CMXZDZ XZ;
    vector<pair<string,vector <int>>> C={
        {"一色双龙会",  { 0, 1, 2,  0, 1, 2,  6, 7, 8,  6, 7, 8,  4, 4}}, //一色双龙会
        {"清龙七对  ",  { 0, 0, 0,  0, 1, 1,  1, 1, 3,  3, 4, 4,  5, 5}}, //青龙七对
        {"将对      ",  { 1, 1, 4,  4, 7, 7, 11,11,14, 14,17,17, 17,17}}, //
        {"全带幺    ",  { 0, 1, 2,  0, 1, 2,  6, 7, 8, 18,18,18, 10,10}}, //全带幺
        {"三色三步高",  { 0, 1, 2, 11,12,13, 22,23,24,  6, 7, 8,  4, 4}}, //三色三步高
        {"全不靠    ",  { 0, 3, 6, 11,14,22, 25,28,30, 31,32,33, 34,35}},  //全不靠
        {"七星不靠  ",  { 0, 3, 6, 11,14,22, 25,36,30, 31,32,33, 34,35}},  //七星不靠
        {"组合龙    ",  { 0, 3, 6, 11,14,17, 25,28,22, 31,32,33, 34,35}},  //组合龙
        {"一色双龙会",  { 0, 1, 2,  0, 1, 2,  6, 7, 8,  6, 7, 8,  4, 4}}, //一色双龙会
        {"三色双龙会",  {10,11,12, 16,17,18, 20,21,22, 26,27,28,  4, 4}}, //三色双龙会
        {"三色双龙会",  { 0, 1, 2,  6, 7, 8, 20,21,22, 26,27,28, 14,14}}, //三色双龙会
        {"三色双龙会",  {10,11,12, 16,17,18,  0, 1, 2,  6, 7, 8, 24,24}}, //三色双龙会
        {"三色双龙会",  { 0, 1, 2,  6, 7, 8,  0, 1, 2,  6, 7, 8, 24,24}},//三色双龙会
        {"小  三  元",  {34,34,34, 35,35,35,  6, 7, 8,  6, 7, 8, 36,36}}, //小三元
    };

    cout << endl << "===   " << XZ.Loc.LocName << "-胡牌牌型检查测试   ===" << endl;
    checkHP(XZ, C);
    cout << "===   " << XZ.Loc.LocName << "-胡牌牌型检查测试（Over）   ===" << endl;
}

void MJClassTest::TestCoder(){
    MJ_C_CoderTools CT;
    int N[][9]={{0,0,0,0,0,0,4,2,2},
                {1,2,3,4,1,2,3,4,1}};
    int Code;
    
    Code=CT.enCode32(N[0]);
    CT.ShowCode32(Code);
 //   Code=CT.enCode32(N[1]);
 //   CT.ShowCode32(Code);
    for (int i=1;i<=9;i++)
        cout << i << ':' << CT.getNoNum32(Code, i) << '|';
    cout << endl;
}

void MJClassTest::TestOkTable(){
    cout << "----------Test OkTable--------------" << endl;
    
    MJ_C_OKTable T1("筒万条");
    T1.Show();
}

void MJClassTest::TestComb(){
    MJ_C_CoderTools CT;
    vector<uint32_t> Comb1,Comb2;
    
   // CT.getCombBit3(5, 3, Comb1);
    CT.getCombBit(5, 3, Comb2);
 //   CT.showCombBit(Comb1,true);
  //  CT.showCombBit(Comb1);
    CT.showCombBit(Comb2);
   /* if (Comb1.size()!=Comb2.size()) cout << "Error" << endl;
    for (int i=0;i<Comb1.size();i++)
        if (Comb1[i]!=Comb2[i]){
            cout << "i:" << i << endl;
            CT.showCombBit(Comb1[i]);
            CT.showCombBit(Comb2[i]);
        } */
    
    
//    bitset<sizeof(int)*4> T;
//    T.reset();
//    T[0]=T[0+7]=T[6+14]=T[6+21]=1;
    
 //   CT.showCombBit((int)T.to_ulong(),7);
}

void MJClassTest::TestHandCoder(){
    vector<pair<string,vector <int>>> C={
        {"全不靠    ",  { 0, 3, 6, 11,14,22, 25,28,30, 31,32,33, 34,35}},  //全不靠
        {"清龙七对  ",  { 0, 0, 0,  0, 1, 1,  1, 1, 3,  3, 4, 4,  5, 5}}, //青龙七对
        {"将对      ",  { 1, 1, 4,  4, 7, 7, 11,11,14, 14,17,17, 17,17}}, //
        {"全带幺    ",  { 0, 1, 2,  0, 1, 2,  6, 7, 8, 18,18,18, 10,10}}, //全带幺
        {"三色三步高",  { 0, 1, 2, 11,12,13, 22,23,24,  6, 7, 8,  4, 4}}, //三色三步高
        {"全不靠    ",  { 0, 3, 6, 11,14,22, 25,28,30, 31,32,33, 34,35}},  //全不靠
        {"七星不靠  ",  { 0, 3, 6, 11,14,22, 25,36,30, 31,32,33, 34,35}},  //七星不靠
        {"组合龙    ",  { 0, 3, 6, 11,14,17, 25,28,22, 31,32,33, 34,35}},  //组合龙
        {"一色双龙会",  { 0, 1, 2,  0, 1, 2,  6, 7, 8,  6, 7, 8,  4, 4}}, //一色双龙会
        {"三色双龙会",  {10,11,12, 16,17,18, 16,17,18, 35,35,35,  6, 6}}, //三色双龙会
        {"三色双龙会",  { 0, 1, 2,  6, 7, 8, 20,21,22, 26,27,28, 14,14}}, //三色双龙会
        {"三色双龙会",  {10,11,12, 16,17,18,  0, 1, 2,  6, 7, 8, 24,24}}, //三色双龙会
        {"三色双龙会",  { 0, 1, 2,  6, 7, 8,  0, 1, 2,  6, 7, 8, 24,24}},//三色双龙会
        {"小  三  元",  {34,34,34, 35,35,35,  6, 7, 8,  6, 7, 8, 36,36}}, //小三元
        {"乱  风  向",  {30,30,31, 31,31,31, 32,32,32, 33,33,34, 35,35}}, //乱风向
        {"字  一  色",  {30,30,30, 31,31,31, 32,32,32, 33,33,33, 34,34}}, //字一色
        {"清      碰",  { 0, 0, 0,  1, 1, 1,  3, 3, 3,  4, 4, 4,  5, 5}}, //清碰
        {"混      碰",  { 0, 0, 0,  1, 1, 1,  3, 3, 3,  4, 4, 4, 33,33}}, //混碰
        {"清龙七对-1",  { 0, 0, 0,  0, 1, 1,  1, 1, 3,  3, 4, 4, 15,15}}, //青龙七对 */
    };

    
    MJ_C_OKTable OKT("all"); //筒万条，不是风，有7对
    MJ_C_HandCoder HC(OKT);
    MJConstV2 MJC;
    MJ_C_CoderHPPX HPD;
    MJ_C_CoderTools CT;
    
    /*    MJ_C_CoderTools CT;
     [0]    uint32_t    268960771
     [1]    uint32_t    33619970
     [2]    uint32_t    4202498
     [3]    uint32_t    306783239
     [3]    uint32_t    920256526
     Value    int    16781581
 
 [1]    int    2291644
 [0]    int    4790754
 00 0000 0001 0010 0100 0110 0111 1000 10
      0     1    2  4     6    7   8 */

 //   CT.ShowCode32(920256526);
 //   CT.ShowCode32(33619970);
 //   CT.ShowCode32(4202498);
 //   CT.ShowCode32(306783239);
    
    for (auto n:C){
        cout << n.first << ":" ;
        HC.addCards(n.second);
        HPD.Clear();
        if (HC.isHu(HPD)) {
            cout << "胡了！|" ;
            HPD.Show();
        }else cout << endl;
    }
    
 //   CT.TestTimeDemo();
}

void MJClassTest::TestDKSCombList(){
    MJ_C_DKSCombList DKSTong,DKSFeng(true);
    
    DKSTong.setDKSNum(1,0,4);
    DKSTong.Show();
 //   bool found= (end(DKSTong.ShunCombList)!=find(begin(DKSTong.ShunCombList),end(DKSTong.ShunCombList),135266433));
 //   cout << (found?"True":"False") << endl;
    
    DKSFeng.setDKSNum(1,3,0);
    DKSFeng.Show();
}

void MJTestPer::Init(){
    vector<TT> t1,t2,t3,t4;
    
/*    T1.clear();
    T2.clear();
    T3.clear();
    T4.clear(); */
    
    Comb2List(T1, 18, 14);
    Comb2List(T2, 21, 14);
    Comb2List(T3, 24, 14);
    Comb2List(T4, 27, 14);
    
/*    Comb2List2(CodeList1, 18, 14);
    Comb2List2(CodeList2, 21, 14);
    Comb2List2(CodeList3, 24, 14);
    Comb2List2(CodeList4, 27, 14); */
}

void MJTestPer::Comb2List(unordered_map<int32_t, int> &T, int n, int m){
    MJ_C_CoderTools CT;
    vector<uint32_t> cl, CodeList;
    int HSCode;
 //   TT Cards;
    int k=0;
    
    cout << "Init Data, C(" << n << ',' << m << ")." << endl;
    cout << "Get ComList...";
    
    auto b1=chrono::high_resolution_clock::now();
    CT.getCombBit(n, m, cl);
    auto e1 = chrono::high_resolution_clock::now();
    cout << "OK! Total:" << cl.size() << endl;
    auto el1 = chrono::duration_cast<std::chrono::nanoseconds>(e1 - b1);
    printf("耗时: %.3f 秒。\n", el1.count() * 1e-9);
    
 //   CT.showAnyBit(31);
 //   CT.showAnyBit(511); //C1    uint32_t    153391688 Code    uint32_t    19173961 C1    uint32_t    153391688
//    CT.showAnyBit(153391688);
    cout << "Add HandCards..." ;
    b1=chrono::high_resolution_clock::now();
 //   CT.CombList2Codes(cl, CodeList, MJ_E_PX_NULL, MJ_E_HS_Tong, m/9);
    for (auto t:cl) T[CT.encode32(t)]=1;
 //       T[CT.Comb2Code(t, n)]=1;
    e1 = chrono::high_resolution_clock::now();
    cout << "OK! Total:" << T.size() << endl ;
    el1 = chrono::duration_cast<std::chrono::nanoseconds>(e1 - b1);
    double el2=el1.count() * 1e-9;
    double per=T.size()/el2/10000;
    printf("耗时: %.3f 秒, %.2f 万笔/s \n\n", el2, per);
}

void MJTestPer::CoderHP(unordered_map<int32_t, int> &T){
    MJ_C_OKTable OKT("OkTable"); //筒万条，不是风，有7对
    MJ_C_HandCoder HC(OKT);
    MJConstV2 MJC;
    MJ_C_CoderHPPX HPD;
    
//    MJ_C_CoderTools CT;
    
    int totH=0;
    
    cout << "Test Coder HP, Total " << T.size() << "..." << endl;
    auto b1=chrono::high_resolution_clock::now();
    for(auto n:T){
 //       HC.addCards(n.second.C, 14, MJ_E_HS_Tong);
        HC.addCards(n.first, MJ_E_HS_Tong);
        HPD.Clear();
        if (HC.isHu(HPD)) {
            totH++;
  //          T[n.first].HP1=true;
        }
    }
    auto e1 = chrono::high_resolution_clock::now();
    cout << "OK! HP Num:" << totH << "   " << totH*100/T.size() << '%' << endl ;
    auto el1 = chrono::duration_cast<std::chrono::nanoseconds>(e1 - b1);
    double el2=el1.count() * 1e-9;
    double per=T.size()/el2/10000;
    printf("耗时: %.3f 秒, %.2f 万笔/s \n\n", el2,per);
}

void MJTestPer::TestCoderHP(){
    MJ_C_OKTable OKT("OKTable"); //筒万条，不是风，有7对
    MJ_C_HandCoder HC(OKT);
    MJ_C_Loc_CMXZDZ XZ;
    MJ_C_CoderHPPX HPD;
    MJ_C_CoderTools CT;
    
/*    CoderHP2(CodeList1);
    CoderHP2(CodeList1);
    CoderHP2(CodeList1);
    CoderHP2(CodeList1);*/
    
    CoderHP(T1);
    CoderHP(T2);
    CoderHP(T3);
    CoderHP(T4);
    
/*    checkHP(XZ,T1);
    checkHP(XZ,T2);
    checkHP(XZ,T3);
    checkHP(XZ,T4);
    
    cout << "寻找差异" << endl;
    vector<int> CY1;
    for (auto n:T4){
        if (n.second.HP1==n.second.HP2) continue;
        HC.addCards(n.second.C, 14);
        CT.ShowCode32(HC.getHSCode(MJ_E_HS_Tong));
        HPD.Clear();
        HC.isHu(HPD);
        HPD.Show();
    
        if (n.second.HP1) CY1.emplace_back(n.first);
    }
    cout << "查表 Y,回溯 N：" << CY1.size() << endl;
 */
}

void MJTestPer::checkHP(MJ_C_LocationBase &L, unordered_map<int32_t, TT> &T){
    MJ_HPCalc HP(L);
    int totH=0;
    
    cout << "Test HP：" << T.size() << "..." << endl;
    auto b1=chrono::high_resolution_clock::now();
    for (auto t:T){
        MJ_HandCard MH;
        vector<MJ_C_HPJieGuo> HPD;
        MH.MoPai((int *)t.second.C, 14);
        if (HP.isHu(MH,HPD)) {
            totH++;
            T[t.first].HP2=true;
        }
    }
    auto e1 = chrono::high_resolution_clock::now();
    cout << "OK! HP Num:" << totH << "   " << totH*100/T.size() << '%' << endl ;
    auto el1 = chrono::duration_cast<std::chrono::nanoseconds>(e1 - b1);
    double el2=el1.count() * 1e-9;
    double per=T.size()/el2/10000;
    printf("耗时: %.3f 秒, %.2f 万笔/s \n\n", el2,per);
}

void unionTest::test(){
    MJ_C_CoderTools CT;
    MJConstV2 MJC;
    int N1[9]={3,1,1,1,1,1,1,2,3},C1;
    int N2[9]={0,0,0,0,0,0,0,0,4},C2;

    
    C1= CT.enCode32(N1);
    C2= CT.enCode32(N2);
 
    CT.ShowCode32(C1);
    CT.ShowCode32(C2);
  //  CT.ShowCode32(38414670);
    
    uint32_t C3;
    
    if (CT.CodeAdd32(C3, C1, C2)) CT.ShowCode32(C3);
    else cout << "Error!" << endl;
    
//    uint32_t Comb=7;
//    CT.showAnyBit(Comb);
//    CT.showAnyBit(1913652);
//    CT.ShowCode32(306184195);
    
  //  CT.ShowCode32(CT.CombBit2Code2(Comb, MJ_E_PX_NULL, MJ_E_HS_Tong));
  //  CT.ShowCode32(CT.CombBit2Code2(Comb, MJ_E_PX_Dui, MJ_E_HS_Tong));
  //  CT.ShowCode32(CT.CombBit2Code2(Comb, MJ_E_PX_AKe, MJ_E_HS_Tong));
  //  CT.ShowCode32(CT.CombBit2Code2(Comb, MJ_E_PX_AGang, MJ_E_HS_Tong));
   // CT.ShowCode32(CT.CombBit2Code2(Comb, MJ_E_PX_AShun, MJ_E_HS_Tong));
    
    //CT.showAnyBit((1<<6)-1);

 /*   vector<uint32_t> CombList,CodeList;
    CT.getCombBit(7, 2, CombList);
    CT.CombList2Codes(CombList, CodeList, MJ_E_PX_AKe, MJ_E_HS_Feng, 2);
    
    for (auto n:CodeList)
        CT.ShowCode32(n);
    cout << CodeList.size() << endl;
    
    CT.ShowCode32(68); */
    
   // cout << CT.getCodeCardNum(C1) << endl;

}

void MJClassTest::TestTing(){
    vector<pair<string,vector <int>>> C={
        {"清一色    ",  { 2, 2, 3,  3, 3, 4,  4, 4, 4,  5, 5, 5,  6 }},  //七星不靠
        {"九莲宝灯  ",  { 0, 0, 0,  1, 2, 3,  4, 5, 6,  7, 8, 8,  8 }},  //全不靠
        {"清龙七对  ",  { 0, 0, 0,  0, 1, 1,  1, 1, 3,  3, 4, 4,  5 }}, //青龙七对
        {"将对      ",  { 1, 1, 4,  4, 7, 7, 11,11,14, 14,17,17, 17 }}, //
        {"全带幺    ",  { 0, 1, 2,  0, 1, 2,  6, 7, 8, 18,18,18, 10 }}, //全带幺
        {"三色三步高",  { 0, 1, 2, 11,12,13, 22,23,24,  6, 7, 8,  4 }}, //三色三步高
        {"全不靠    ",  { 0, 3, 6, 11,14,22, 25,28,30, 31,32,33, 34 }},  //全不靠
        {"七星不靠  ",  { 0, 3, 6, 11,14,22, 25,36,30, 31,32,33, 34 }},  //七星不靠
        {"组合龙    ",  { 0, 3, 6, 11,14,17, 25,28,22, 31,32,33, 34 }},  //组合龙
        {"一色双龙会",  { 0, 1, 2,  0, 1, 2,  6, 7, 8,  6, 7, 8,  4 }}, //一色双龙会
        {"三色双龙会",  {10,11,12, 16,17,18, 16,17,18, 35,35,35,  6 }}, //三色双龙会
        {"三色双龙会",  { 0, 1, 2,  6, 7, 8, 20,21,22, 26,27,28, 14 }}, //三色双龙会
        {"三色双龙会",  {10,11,12, 16,17,18,  0, 1, 2,  6, 7, 8, 24 }}, //三色双龙会
        {"三色双龙会",  { 0, 1, 2,  6, 7, 8,  0, 1, 2,  6, 7, 8, 24 }},//三色双龙会
        {"小  三  元",  {34,34,34, 35,35,35,  6, 7, 8,  6, 7, 8, 36 }}, //小三元
        {"乱  风  向",  {30,30,31, 31,31,31, 32,32,32, 33,33,34, 35 }}, //乱风向
        {"字  一  色",  {30,30,30, 31,31,31, 32,32,32, 33,33,33, 34 }}, //字一色
        {"清      碰",  { 0, 0, 0,  1, 1, 1,  3, 3, 3,  4, 4, 4,  5 }}, //清碰
        {"混      碰",  { 0, 0, 0,  1, 1, 1,  3, 3, 3,  4, 4, 4, 33 }}, //混碰
        {"清龙七对-1",  { 0, 0, 0,  0, 1, 1,  1, 1, 3,  3, 4, 4, 15 }}, //青龙七对 */
    };

    
    MJ_C_OKTable OKT("all"); //筒万条，不是风，有7对
    MJ_C_HandCoder HC(OKT);
    MJConstV2 MJC;
    MJ_C_CoderHPPX HPD;
    MJ_C_CoderTools CT;
    set<int> Cards;
    
    /*    MJ_C_CoderTools CT;
     [0]    uint32_t    268960771
     [1]    uint32_t    33619970
     [2]    uint32_t    4202498
     [3]    uint32_t    306783239
     [3]    uint32_t    920256526
     Value    int    16781581
 
 [1]    int    2291644
 [0]    int    4790754
 00 0000 0001 0010 0100 0110 0111 1000 10
      0     1    2  4     6    7   8 */

 //   CT.ShowCode32(920256526);
 //   CT.ShowCode32(33619970);
 //   CT.ShowCode32(4202498);
 //   CT.ShowCode32(306783239);
   
    cout << "---测试是否听牌---" << endl;
    for (auto n:C){
        cout << n.first << ":" ;
        HC.addCards(n.second);
        Cards.clear();
        if (HC.isTing(Cards)) {
            cout << "听了：|" ;
            for (auto n:Cards)
                cout << MJC.getNCID_HZ(n) << '.';
        }
        cout << endl;
    }
    
 //   CT.TestTimeDemo();
}
