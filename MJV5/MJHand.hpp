//
//  MJHand.hpp
//  MJ
//
//  Created by Bob Yin on 2022/7/1.
//
#ifndef MJHand_hpp
#define MJHand_hpp

#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <algorithm>
using namespace std;

#include "MJTools.hpp"
using namespace MJClassTools;

namespace MJHPTools{

struct MJ_S_HPPatt{ //胡牌类型模式结构，通过addMatch方法，将指定格式的string，转化为四个参数和string<set>集合
    MJ_E_HPPatternC PN; //模式匹配的功能编号
    char A[4];// a,b,c,d; //最大四个参数
    set<string> Set; //要求集合
};

class MJ_C_HPCeckTools:public MJ_C_Base{
private:
    bool match(char *a, const char *b, size_t l);//模糊匹配，返回匹配结果
    
    bool Func_OnlyCard(MJ_S_HPPatt &P, MJ_C_CardToTal &CTT);
    //只能有牌集合，内容每张牌的唯一码，“，”分割
    //内容为"00,08,10,18,20,28,30,31,32,33,34,35,36,"
    bool Func_OnlyType(MJ_S_HPPatt &P, MJ_C_CardToTal &CTT);
    //只能有的牌型集合，顺序为MJCardType，一个字节对应一个内容，
    //"0700000" 7对 ,'*'可以有，‘0-9’数量
    bool Func_OnlyHuaSe(MJ_S_HPPatt &P, MJ_C_CardToTal &CTT);
    //只能有的花色集合，顺序为MJType，“，”分割
    //"1001，0101，0011" //混一色
    bool Func_OnlyPX(MJ_S_HPPatt &P, MJ_C_CardToTal &CTT);
    //这能有的牌型组合集合，a|xyz
    //a:数量
    //xyz:允许的牌型集合
    //"1|300,400,500,308,408,508,"); //数量1，内容19刻
    bool Func_4Cards(MJ_S_HPPatt &P, MJ_C_CardToTal &CTT){return (P.A[0]<=CTT.Card4);}
    //有四张牌，内容为数量 “2”，有两个4张
    bool Func_ShuZi(MJ_S_HPPatt &P, MJ_C_CardToTal &CTT);
    //数字要求，下标是数字，内容'*'可以有，‘0-9’数量，
    bool Func_SType(MJ_S_HPPatt &P, MJ_C_CardToTal &CTT); //特殊牌型检查
    //"7":七星不靠
    //"Z":组合龙
    //"Q":全不靠
 //   bool Func_Line(MJ_S_HPPatt &P, MJ_C_CardsLine &Line); //差检查
    bool Func_Line(MJ_S_HPPatt &P, set<int> &L); //线检查
    //"a|xyz"
    //a:数量
    //xyz:线型ID集合
public:
    bool Func(MJ_S_HPPatt &P, MJ_C_CardToTal &CTT){ //根据匹配模式的功能编号，调用不同的匹配算法
        bool R=false;
        
        switch (P.PN) {
            case MJ_E_HPP_4Cards:
                R=Func_4Cards(P,CTT);
                break;
            case MJ_E_HPP_OnlyCard:
                R=Func_OnlyCard(P,CTT);
                break;
            case MJ_E_HPP_ShuZi:
                R=Func_ShuZi(P,CTT);
                break;
            case MJ_E_HPP_OnlyType:
                R=Func_OnlyType(P,CTT);
                break;
            case MJ_E_HPP_OnlyPX:
                R=Func_OnlyPX(P,CTT);
                break;
            case MJ_E_HPP_OnlyHuaSe:
                R=Func_OnlyHuaSe(P,CTT);
                break;
            case MJ_E_HPP_SType:
                R=Func_SType(P,CTT);
                break;
            case MJ_E_HPP_SLine:
                R=Func_Line(P,CTT.SLine);
                break;
            case MJ_E_HPP_KLine:
                R=Func_Line(P,CTT.KLine);
                break;
            default:
                break;
        }
        return R;
    }
};

class MJ_C_HPKindBase:public MJ_C_Base{ //每种胡牌模式，包含匹配要求和检查工具，匹配要求由前序胡牌牌型要求和本牌型特殊要求组成
private:
    MJ_E_HuPaiKind PattName; //本模式的编号
    MJ_C_HPKindBase *Base=NULL; //所有模式的数组，下标是模式编号
    MJ_C_HPCeckTools *FuncT=NULL; //模式检查的工具
public:
    void set(int i,MJ_C_HPKindBase *A,MJ_C_HPCeckTools *FT){ //设置模式数组和工具的指针
        PattName=(MJ_E_HuPaiKind)i;
        Base=A;FuncT=FT;
    }
    bool SType=false; //是否特殊牌型
    vector<MJ_S_HPPatt> Patt; //空为无要求，有几个就是几个必须同时满足
    vector<MJ_E_HuPaiKind> ExType; //前序牌型；空为无要求，有几个就是几个必须同时满足
    void addMatch(MJ_E_HPPatternC,string); //增加一个本模式的匹配要求
    bool Check(MJ_C_CardToTal &TC,vector<int> OkKinds); //确认本模式是否符合的主函数，返回true是符合
    void show();
};


//所有胡牌类型初始化
class MJ_C_HPAllKinds:public MJ_C_Base{
private:
    MJConstV2 MJC;
    MJ_C_HPCeckTools FuncTool;
public:
    MJ_C_HPKindBase Base[MJ_E_HPK_End];
    
    MJ_C_HPAllKinds(); //设置所有胡牌模式数组和匹配要求
    void ShowBase();
};

}
#endif /* MJHand_hpp */
