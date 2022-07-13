//
//  MJCLassV5.hpp
//  MJ
//
//  Created by Bob Yin on 2022/7/13.
//

#ifndef MJCLassV5_hpp
#define MJCLassV5_hpp

#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
using namespace std;

#include "MJHand.hpp"
using namespace MJHPTools;

#include "MJTools.hpp"
using namespace MJClassTools;


namespace MJClassV5{   //MJClass V5 命名空间

class MJ_C_Table:public MJ_C_Base{ //一桌麻将
private:
    int HuaNum=0;
public:
    vector<MJ_S_Card1> Cards; //所有牌和状态，以序号为关键字
    set<int> CardSet; // 牌张
    MJConstV2 MJC;
    MJ_S_Location Loc;
    
    MJ_C_Table(){}
    MJ_C_Table(MJ_S_Location &L){reSet(L);}

    void reSet(MJ_S_Location &L){  //设置一桌的牌
        if (Cards.size()>0) clear();
        Loc=L;
        for (int i=0; i<MJ_E_HS_End; i++)
            for (int t=0; t<Loc.NumTimes[i][1]; t++)
                for (int n=0; n<Loc.NumTimes[i][0]; n++){
                    if (Loc.Type[i][n]!='0') {
                        MJ_S_Card1 C(0);
                        C.ID=i*10+n;C.No=(int)Cards.size();
                        Cards.emplace_back(C);
                        CardSet.insert(C.ID);
                        if (Loc.Type[i][n]=='H') HuaNum++;
                    }
                }
    }
    
    inline unsigned long getNum(){return Cards.size();}
    inline int getHuaNum(){return HuaNum;}
    void clear(){
        vector<MJ_S_Card1> C1;

        Cards.swap(C1);
        CardSet.clear();
        HuaNum=0;
    }
    void show(){
        cout << "本桌麻将为：" << Loc.LocName << endl;
        for (auto i:Cards){
            cout << MJC.getCardHZ(i.ID);
            if (MJC.getFunc(Loc, i.ID)[MJ_E_DZB_canHuan]) cout << '|';
        }
        cout << endl;
        cout << "张数：" << getNum() << endl;
        cout << "花张数：" << getHuaNum() << endl;
        cout << "使用的牌种类：" << endl;
     for (auto CS:CardSet) {
            cout << MJC.getCardHZ(CS);
            if (MJC.getFunc(Loc, CS)[MJ_E_DZB_canHuan]) cout << '-';
        }
        cout << endl;
        cout << "牌种张数：" << CardSet.size() << endl;
        cout << "胡牌与翻数：" << endl;
        for (auto i:Loc.HPFan){
            cout << "翻数：" << i.FanS << " | ";
            for (auto j:i.CardType)
                cout << MJC.getHPHZ(j) << "," ;
            cout << endl;
        }

    }
};

class MJ_HandCard:public MJ_C_Base{  //一手牌
private:
    MJ_C_LocationBase *Loc;
    bool isNumOK(int l=1){ //检查手中牌是不是合理，
        //规则是成型的牌（顺、刻、杠）数量小于要求，摊在牌桌上的就是牌型数量，手里牌按照整除3计算牌型
        //除牌型数外，剩余牌不超过2张
        int HL=CardIDs.Size()+l, TPXL=OnTableIDs.Size();
        bool R=(((HL/3+TPXL)>MJ_MaxPXNum)||((HL+TPXL*3)>(MJ_MaxPXNum*3+2)));
        if (R) SetErrMsg("张数不对：手中牌"+to_string(HL)+"张，桌上牌型"+to_string(TPXL)+"个");
        return !R;
    }
public:
    MJ_C_CardsGroup CardIDs, OnTableIDs; //手中牌和摊在桌上的牌

    MJ_HandCard():CardIDs("手中牌"),OnTableIDs("桌上牌"){}
    int chooseCard(); //选一张牌,返回选出的牌的ID
    bool MoPai(int ID){ bool R=isNumOK(); if (R) CardIDs.Insert(ID); return R; } //摸一张牌
    bool MoPai(int *H, int l){ bool R=isNumOK(); if (R) CardIDs.Insert(H,l); return R; }//摸指定张数的牌
    bool MoPai(vector<int> &H){ bool R=isNumOK(); if (R) CardIDs.Insert(H); return R;} //摸一组牌
    
    inline bool checkMPeng(int ID){ return (CardIDs.GetNum(ID)>=2); } //对于别人打出的牌，能不能碰
    inline bool checkMGang(int ID){ return (CardIDs.GetNum(ID)>=3); } //对于别人打出的牌，能不能明杠
    inline bool checkAGang(int &R){ R=CardIDs.Get4Cards(); return (R>=0); }//检查有没有暗杠,R 是能暗杠的牌，返回有没有
    inline bool checkBGang(int ID) { return OnTableIDs.FindPX(MJ_E_PX_MKe*100+ID);} //能不能补杠
    int checkChi(int ID){ //能不能吃,返回值：0，不能吃，1：45吃3，2：35吃4，3：34吃5；
        if ((ID/10)>MJ_E_HS_Wan) { SetErrMsg("牌："+to_string(ID)+"不是筒万条"); return 0; } //不是筒万条
        if (CardIDs.Size()<2) { SetErrMsg("手中牌小于2张"); return 0; } //张数不够
        int R=0;
        bool f[4]={CardIDs.FindPX(ID-2),CardIDs.FindPX(ID-1),CardIDs.FindPX(ID+1),CardIDs.FindPX(ID+2)};
        
        if (f[0]&f[1]) R=3; //3：12吃3
        if (f[1]&f[2]) R=2; //2：35吃4
        if (f[2]&f[3]) R=1; //1：89吃7
        return R;
    }
    
    int pengOut(int ID); //碰出，返回要出的一张牌，-1为没有碰出
    int GangOut(MJ_E_PaiXing T,int ID,int ID2); //明杠ID出，补一张ID2；，-1为没有碰出
    int chiOut(int ID, int How=0); //吃出，返回要出的一张牌，-1为没有吃出,How =0,随机吃，1：45吃3，2：35吃4，3：34吃5
    
    void Show(){ CardIDs.Show(); OnTableIDs.Show(); cout << endl;}
};

class MJ_HPCalc:public MJ_C_Base{ //胡牌计算类
private:
    const MJ_S_Location Loc;
    MJ_C_HPAllKinds Patts;
    MJ_C_Tools HDTools;
    void Hu3(MJ_C_CardsGroup &HC, MJ_C_CardsGroup &TC,bool SorK=true); //递归检查剩下的手中牌，是否可以都为一顺或者1刻；Sork True，3张时，优先查顺
    bool CheckHPKind(MJ_C_CardsGroup &HC,vector<MJ_C_HPJieGuo> &HPData,bool CheckS=false);
public:
    MJ_HPCalc(MJ_C_LocationBase &L):Loc(L.Loc){}
    bool isHu(MJ_HandCard &Cards, vector<MJ_C_HPJieGuo> &HPData); //返回值，0，可以胡，其余为出错码
};

} //MJClass V5 命名空间

#endif /* MJCLassV5_hpp */
