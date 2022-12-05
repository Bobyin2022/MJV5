//
//  MJCoder.hpp
//  MJ
//  数字牌编码与解码
//  Created by Bob Yin on 2022/7/1.
//

#ifndef MJCoder_hpp
#define MJCoder_hpp

#include <cstdio>
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>

#include "MJTools.hpp"
using namespace std;
using namespace MJClassTools;

namespace MJClassCoder{   //MJClassTools V1.0 命名空间
struct MJ_S_CodeHPPX{
    int V=-1; //7Dor3n+2
    int Ds=0,Ks=0,Ss=0,Zs=0;
    vector<int> NCID_PXs;
};

class MJ_C_CoderHPPX{
public:
    vector<MJ_S_CodeHPPX> HPData;
    
    void Clear(){
        vector<MJ_S_CodeHPPX> p;
        HPData.swap(p);
    }
    void Show(){
        MJConstV2 MJC;
        
        for (auto n:HPData){
            if (n.V==2) cout << " 7对：";
            if (n.V==3) cout << "特殊：";
            if (n.V<2) cout << "普通：";
            cout << n.Ds << '-' << n.Ks << '-' << n.Ss << ' ';
            sort(n.NCID_PXs.begin(),n.NCID_PXs.end());
            for(auto m:n.NCID_PXs)
                cout << MJC.getNCID_HZ(m) << '.';
            cout << endl;
        }
    }
};

class MJ_C_CoderTools{
private:
    int No,Type;
    MJConstV2 MJC;
public:
    //编码相关工具
    //单一花色编码
    uint32_t enCode32(int *N); //给出单花色No-Num数组(下标数字[1-9]，内容张数)，返回编码后的int数字
    uint32_t enCode32(int *IDs, int Len); //根据给定的一组牌（一种花色），返回编码后的Code
    uint32_t encode32(uint32_t Comb,int m=9); //根据组合数，返回编码
    uint32_t enCode32(vector<int> &Ids);
    bool CheckCode32(uint32_t Code);
    //混合花色编码
    void enCodeList32(uint32_t *CodeN, int *IDs, int Len); //根据给定的一组牌（所有花色），返回编码后的数组，以花色为下标
    void enCodeList32(uint32_t *CodeN, vector<int> &Ids){ enCodeList32(CodeN, &Ids[0],(int) Ids.size());} //Vector
    //Code加法
    bool CodeAdd32(uint32_t &R, uint32_t C1, uint32_t C2); //Code相加 R=C1+C2,返回是否溢出
    uint32_t CodeAdd1No(uint32_t Code, int No){
        uint32_t  R;
        if (getNoNum32(Code, No)<4) CodeAdd32(R, Code, ((1<<(8-No)*3)<<4)|1);
        else return 0;
   //     ShowCode32(R);
        return R;
    }
    //解码
    int deCode32(int *N,uint32_t Code); //根据编码规则，对Coder解码为N-N数组(下标数字，内容张数),返回总张数
    bool deCode32(int *CardsNo, uint32_t Code, MJ_E_PaiXing PX); //依据PX规则，解码Code,返回本牌型的牌ID；
    
    inline int getCodeCardNum(uint32_t Code){ return Code&15;}
    inline int  getNoNum32(uint32_t Code,int No){ return ((7<<((8-No)*3))&(Code>>4))>>((8-No)*3);} //取出Code中指定No的张数
    void ShowCode32(uint32_t Code); //显示Code内容
    
                   
    //组合数相关工具
    void getCombBit(int n, int m, vector<uint32_t> &ComList); //C(n,m),获取二进制模式的列表
    void getCombBit64(int n, int m, vector<uint64_t> &ComList); //C(n,m),获取二进制模式的列表
    void showCombBit(vector<uint32_t> &ComList, bool showD=false, int baseLen=9);
    void showCombBit(uint32_t Comb, int baseLen=9);
    //组合数列表转换为编码列表；
    void CombList2Codes(vector<uint32_t> &ComList, vector<uint32_t> &CodeList, MJ_E_PaiXing PX, MJ_E_HuaSe HS){ //把一组组合数转换为Code, n为候选数据的个数，缺省为0，不同的组合数，编码后的Code可能相同，所以用set
        uint32_t Code;
        for (auto n:ComList){
            if ((Code=CombBit2Code2(n, PX, HS))!=0){
                CodeList.emplace_back(Code); //有效数据
            }
        }
    }
    uint32_t CombBit2Code2(uint32_t Comb, MJ_E_PaiXing PX, MJ_E_HuaSe HS); //组合数还原为No-Num（NN）数组，返回Code
    
    //OKtable Value相关函数
    inline int getValveV(int V){ return V&3; }; //获取Value中的 V值
    void TestTimeDemo();
    void showValue(int Value);
    void getValuePX(int Value, MJ_S_CodeHPPX &PX, int HS){ //获取Oktable中Value中的牌型
        PX.V=getValveV(Value);
        Value>>=2;
        
        if (PX.V==2) { //7对牌型
            for(int i=0; i<7; i++){
                if ((No=Value&15)<15) {
                    PX.NCID_PXs.emplace_back(MJC.getNewCardID(No,(MJ_E_HuaSe) HS, MJ_E_PX_Dui)); //还原7对数字
                    PX.Ds++;
                }
                Value>>=4;
            }
        }
        if (PX.V==3) { //特殊牌型
            for(int i=0; i<7; i++){
                if ((No=Value&15)<15) {
                    PX.NCID_PXs.emplace_back(MJC.getNewCardID(No,(MJ_E_HuaSe) HS, MJ_E_PX_NULL)); //还原牌数字
                    PX.Zs++;
                }
                Value>>=4;
            }
        }
        if (PX.V<2){ //3n+2牌型
            for (int i=0; i<4; i++){
                No=Value&15; Value>>=4;
                Type=Value&1; Value>>=1;
                if (No<15) {
                    PX.NCID_PXs.emplace_back(MJC.getNewCardID(No,(MJ_E_HuaSe) HS, (Type?MJ_E_PX_AShun:MJ_E_PX_AKe))); //还原顺刻数字
                    PX.Ss+=(Type)?1:0;
                    PX.Ks+=(Type)?0:1;
                }
            }
            No=Value&15; //还原麻将头
            if (No<15){
                PX.NCID_PXs.emplace_back(MJC.getNewCardID(No,(MJ_E_HuaSe) HS, MJ_E_PX_Dui)); //还原7对数字
                PX.Ds++;
            }
        }
    }
    
    //test Tools
    void showAnyBit(uint32_t v){
        bitset<32> Bits(v);
        cout << dec << v << '.' << oct << v  << dec << '|' << Bits << endl;
    }
};

class MJ_C_DKSCombList{ //对-刻-顺组合牌
    char DuiNum=0,KeNum=0,ShunNum=0; //对、刻、顺的数量
    MJConstV2 MJC;
    MJ_C_CoderTools CT;
public:
    const bool isFeng;
    vector<uint32_t> DuiCodes,KeCodes,ShunCodes; ////对、顺、刻的Coder列表，从组合数转换为Coder；
    
    MJ_C_DKSCombList(bool isF=false):isFeng(isF){} //初始化必须明确本组合的花色类型，筒、万、条、风
    MJ_C_DKSCombList(int D,int K,int S, bool isF=false):isFeng(isF){setDKSNum(D,K,S);} //初始化必须明确本组合的花色类型，筒、万、条、风
    inline int getCardNum(){return DuiNum*2+(KeNum+ShunNum)*3;} //获取本组合的牌张数
    inline int getDuiN() { return DuiNum;  }
    inline int getKeN()  { return KeNum;   }
    inline int getShunN(){ return ShunNum; }
    inline int getAllN() { return DuiNum+KeNum+ShunNum;}
    bool setDKSNum(int D,int K,int S); //设置对、刻、顺的数量，并确定组合数列表
    void Show();
};

class MJ_C_OKTable{ //生成可以计算胡牌的数字组合
/*
 可以胡牌的数字组合是一个KV对，Key是同花色数字牌enCoder之后的int32编码，V是类型：
 0：3n模式，无对
 1：3n+2模式，有对
 2：2对以上
 胡牌判定：
 1、每种花色的enCoder，得到Code[4]，对应筒条万风（MJ_E_HuaSe）
 2、查表获得每种花色的V，
 3、所有花色V想加，只有等于1，才是胡牌
 */
private:
    string Name;
  //  int Split=12; //高低区Data的分界，L={0,Split], H={Split,14]
    unordered_map<uint32_t, uint32_t> DataN, DataF; //OkTable, nor和风向
    vector<MJ_C_DKSCombList> DKSs;
    vector<pair<int,int>> ComKS; //刻顺的组合，first刻的数量，secon顺的数量
    
    MJ_C_CoderTools CT;
    int append(int Code, MJ_C_DKSCombList &DKS, uint32_t DC, uint32_t KC, uint32_t SC); //Data表中追加数据, 返回值：-1为错误，其余位正确的value
    void appendSpecRec(); //增加特殊牌型数据
    
    void Init(int zs);
    inline void addDKSList(int D,int K,int S){
        MJ_C_DKSCombList D1(D, K, S); //无风的DKS列表
    //    MJ_C_DKSCombList D2(D, K, S,true); //有风的DKS列表
        DKSs.emplace_back(D1);
    //    if (D2.getAllN()>0) DKSs.emplace_back(D2);
    }
    void CreatDKS(); // 根据DKSs的组合数列表，追加Oktable的牌型
public:
    MJ_C_OKTable(string s,int ZS=MJ_MaxPXNum*3+2):Name(s){Init(ZS);}; //明确
    uint32_t FindN(uint32_t Code){ //查NormalCode， 返回-1为没找到，>=0为找到，返回V
        if (Code==0) return -1; //不找0；
        const auto& got = DataN.find(Code);
        return (got == DataN.end())?-1:got->second;
    }
    uint32_t FindF(uint32_t Code){ //查FengCode， 返回-1为没找到，>=0为找到，返回V
        if (Code==0) return -1; //不找0；
        const auto& got = DataF.find(Code);
        return (got == DataF.end())?-1:got->second;
        
    }
    inline uint32_t FindAll(uint32_t Code,MJ_E_HuaSe HS){ //查AllCode， 返回-1为没找到，>=0为找到，返回V
        return (HS==MJ_E_HS_Feng)?FindF(Code):FindN(Code);
    }
    void Show(bool detail=false);
    inline const string getName(){ return Name;}; //获取本实例名字
};

class MJ_C_CardsG{
public:
    set<int> Ids;
 //   vector<int> Ids;
    inline void insert(int c){
        Ids.insert(c);
 //       Ids.emplace_back(c);
    }
    inline void clear(){
        Ids.clear();
    }
};

class MJ_C_HandCoder{ //一手牌，用Coder编码
private:
    uint32_t HSCode[MJ_E_HS_Hua]; //编码后的手中牌，
    //uint32_t HSValue[MJ_E_HS_Hua]; //每种花色对应的value;
    int CardNum=0;
    MJ_C_CoderTools CT; //编码工具
    MJ_C_OKTable *OKTable=NULL; //筒万条OKTable和风向的OKTable
    inline void HSCodeClear(){HSCode[0]=HSCode[1]=HSCode[2]=HSCode[3]=0;} //重置HSCode；
    bool addTindID(MJ_C_CardsG &Ids,MJ_E_HuaSe HS,int type);
public:
    MJ_C_HandCoder(MJ_C_OKTable &T):OKTable(&T){}
    inline int getHSCode(MJ_E_HuaSe HS){return HSCode[HS];}
    
    //加入牌
    void addCards(vector<int> &Cards){ HSCodeClear(); CardNum=(int)Cards.size(); CT.enCodeList32(HSCode, Cards);} //所有花色
    void addCards(int *Cards, int len){ HSCodeClear(); CardNum=len; CT.enCodeList32(HSCode, Cards, len);} //所有花色
    void addCards(int *Cards, int len, MJ_E_HuaSe HS){
        HSCodeClear(); CardNum=len;
        HSCode[HS]=CT.enCode32(Cards, len);
    }
    void addCards(uint32_t Code, MJ_E_HuaSe HS){
        HSCodeClear();
        HSCode[HS]=Code;
        CardNum=CT.getCodeCardNum(Code);
    }  //加入指定花色牌的编码
    
    bool isHu(MJ_C_CoderHPPX &HPD); //计算手中牌是否胡牌
    bool isTing(MJ_C_CardsG &Ids); //计算手中牌是否上听
    void Show(); 
};

} //MJClassCoder

#endif /* MJTools_hpp */
