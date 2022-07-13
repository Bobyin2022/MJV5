//
//  MJHand.cpp
//  MJ
//
//  Created by Bob Yin on 2022/7/1.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "MJHand.hpp"
#include "MJCLassV5.hpp"

using namespace MJClassV5;
using namespace MJHPTools;

//本胡牌模式的匹配主要算法，返回true为通过
bool MJ_C_HPKindBase::Check(MJ_C_CardToTal &CT,vector<int> OkKinds){
/*参数说明：
MJ_C_CardToTal &CT：本次胡牌的牌型统计数据
vector<int> OkKinds：已经通过的胡牌牌型
 */
    bool f=false;

    //不是特殊牌型，不允许牌型数量>3n+2
    if ((!SType)&&(CT.PXs.size()>(MJ_MaxPXNum+1))) return false;
    //是特殊牌型，不允许牌型数量<=3n+2
    if ((SType)&&(CT.PXs.size()<=(MJ_MaxPXNum+1))) return false;
    
    //查找前序要求，必须全部满足才通过
    if (ExType.size()>0) {
        for (auto Ex:ExType){ // 在已经符合的种类中，查找前序牌型
            if (find(OkKinds.begin(),OkKinds.end(),Ex)!=OkKinds.end()) f=true; //找到
            else f=Base[Ex].Check(CT, OkKinds); //没找到重新再算前序牌
            if (!f) return false; //没找到，不通过，退出
        }
    }//前序要求满足
    
    //根据要求查找匹配，必须全部满足才通过
    if (Patt.size()>0){
        f=false;
        for (auto P:Patt)
            if (FuncT->Func(P, CT)) f=true;
            else {f=false; break;} //一个不成功
        if (!f) return false; //有不成功
    } //匹配要求满足
    return f;
}

//对于指定的胡牌配型，增加一个牌型匹配要求
void MJ_C_HPKindBase::addMatch(MJ_E_HPPatternC PN,string S){
/*参数说明：
MJ_E_HPPatternC PN：匹配要求的功能编号
string S: 需要拆解的字符串
本方法将字符串拆解后，加入到本类的Patt数组中
*/
    MJ_S_HPPatt P;
    P.PN=PN;
    const char* C=S.c_str();
    int begin=-1, //拆分字符串的起始下标
        j=-1; //有效数字长度，不算‘，’
    
    switch (PN) {
        case MJ_E_HPP_OnlyCard: //只能有牌集合，内容每张牌的唯一码，“，”分割
            //内容为"00,08,10,18,20,28,30,31,32,33,34,35,36,"
            begin=0;j=2;
            break;
        case MJ_E_HPP_OnlyType: //只能有的牌型集合，顺序为MJCardType，一个字节对应一个内容，
            //"0700000" 7对 ,'*'可以有，‘0-9’数量
            begin=0;j=MJ_E_PX_End;
            break;
        case MJ_E_HPP_OnlyHuaSe://只能有的花色集合，顺序为MJType，“，”分割
            //"1001，0101，0011" //混一色
            begin=0;j=MJ_E_HS_End-1;
            break;
        case MJ_E_HPP_ShuZi: //数字要求，内容'*'可以有，‘0-9’数量
            begin=0;j=9;
            break;
        case MJ_E_HPP_4Cards: //有四张牌，内容为数量 “2”，有两个4张
            P.A[0]=C[0]-'0';
            break;
        case MJ_E_HPP_SType: //特殊牌型匹配
            P.A[0]=C[0];
            break;
        case MJ_E_HPP_OnlyPX:  //这能有的牌型组合集合，"1|300,400,500,308,408,508,"); //数量1，内容19刻
        case MJ_E_HPP_KLine:
        case MJ_E_HPP_SLine:
            P.A[0]=C[0]-'0'; //"a|xyz", 2|226:两个226的线型；
            begin=2;j=3;
            break;
        default:
            break;
    }
    if (begin>=0){
        for(int i=begin; i<S.size(); i+=j){
            string tmp=S.substr(i,j);
            P.Set.insert(tmp);
            i++; //跳过逗号
        }
    };
    Patt.emplace_back(P);
}

void MJ_C_HPKindBase::show(){
    MJConstV2 Tools;
    
    cout << PattName << Tools.getHPHZ(PattName) << (SType?'*':' ') << ':' << endl;
    if (Patt.size()==0) return;
    for (auto t:Patt){
        cout << "  " << Tools.getHPPatternHZ(t.PN) << '|';
        switch (t.PN) {
            case MJ_E_HPP_OnlyCard: //只能有牌集合，内容每张牌的唯一码，“，”分割
                //内容为"00,08,10,18,20,28,30,31,32,33,34,35,36,"
                cout << "共" << t.Set.size() << "种：";
                for(auto i:t.Set)
                    cout << Tools.getCardHZ(stoi(i)) << '.';
                break;
            case MJ_E_HPP_OnlyPX:  //只能有的牌型组合集合，
            case MJ_E_HPP_KLine:
            case MJ_E_HPP_SLine:
                cout << (int)t.A[0] << "个符合" ;
            case MJ_E_HPP_OnlyType:
            case MJ_E_HPP_OnlyHuaSe:
            case MJ_E_HPP_ShuZi: //只能有的牌型集合，顺序为MJCardType，一个字节对应一个内容，
                //"0700000" 7对 ,'*'可以有，‘0-9’数量
                cout << "共" << t.Set.size() << "种：";
                for(auto n:t.Set){
                    cout << n << '.';
                };
                break;
            case MJ_E_HPP_4Cards: //有四张牌，内容为数量 “2”，有两个4张
                cout << (int)t.A[0];
                break;
            case MJ_E_HPP_SType: //特殊牌型要求
                switch (t.A[0]){
                    case '7':
                        cout << "七星不靠";
                        break;
                    case 'Z':
                        cout << "组合龙";
                        break;
                    case 'Q':
                        cout << "全不靠";
                        break;
                    default:
                        cout << "未定义";
                        break;
                }
                break;
            default:
                cout << "未定义";
                break;
        }
        cout << endl;
    }
}

//设置本系统支持的胡牌模式
MJ_C_HPAllKinds::MJ_C_HPAllKinds(){
    MJ_C_Tools Tools;
    for(int i=0; i<MJ_E_HPK_End; i++) Base[i].set(i,Base,&FuncTool);
    
//特殊型和普通牌型必须自成体系，前序牌只能是同一类牌。
    
    //四归一 MJ_E_HPK_421=0,
    Base[MJ_E_HPK_421].addMatch(MJ_E_HPP_4Cards, "1"); //特定四张数量1
    
//七对计算
    //MJ_E_HPK_7ZD=25, //七对
    Base[MJ_E_HPK_7ZD].SType=true; //特殊牌
    Base[MJ_E_HPK_7ZD].addMatch(MJ_E_HPP_OnlyType,"07000000,"); //6+1个对
    
    //MJ_E_HPK_D7L=30, //连七对
    Base[MJ_E_HPK_D7L].SType=true; //特殊牌
    Base[MJ_E_HPK_D7L].ExType.emplace_back(MJ_E_HPK_7ZD); //前序牌型；清7对
    Base[MJ_E_HPK_D7L].addMatch(MJ_E_HPP_ShuZi,"222222200,022222220,002222222,"); //有连续7张同色数字，每个数字2个
    
    //MJ_E_HPK_QJD=57, //将对
    Base[MJ_E_HPK_QJD].SType=true; //特殊牌
    Base[MJ_E_HPK_QJD].ExType.emplace_back(MJ_E_HPK_7ZD); //前序牌型；7对
    Base[MJ_E_HPK_QJD].addMatch(MJ_E_HPP_ShuZi,"0*00*00*0,"); //只允许258牌
    
    //MJ_E_HPK_1LD=6, //龙七对
    Base[MJ_E_HPK_1LD].SType=true; //特殊牌
    Base[MJ_E_HPK_1LD].ExType.emplace_back(MJ_E_HPK_7ZD); //前序牌型；7对
    Base[MJ_E_HPK_1LD].addMatch(MJ_E_HPP_4Cards, "1");//特定四张数量1
    
    //MJ_E_HPK_2LD=10, //双龙7对
    Base[MJ_E_HPK_2LD].SType=true; //特殊牌
    Base[MJ_E_HPK_2LD].ExType.emplace_back(MJ_E_HPK_7ZD); //前序牌型；7对
    Base[MJ_E_HPK_2LD].addMatch(MJ_E_HPP_4Cards, "2"); //特定四张数量2
    
    //MJ_E_HPK_3LD=19, //三龙七对
    Base[MJ_E_HPK_3LD].SType=true; //特殊牌
    Base[MJ_E_HPK_3LD].ExType.emplace_back(MJ_E_HPK_7ZD); //前序牌型；7对
    Base[MJ_E_HPK_3LD].addMatch(MJ_E_HPP_4Cards, "3");//特定四张数量3
    
    //MJ_E_HPK_Q7D=50, //清7对
    Base[MJ_E_HPK_Q7D].SType=true; //特殊牌
    Base[MJ_E_HPK_Q7D].ExType.emplace_back(MJ_E_HPK_7ZD); //前序牌型；7对
    Base[MJ_E_HPK_Q7D].addMatch(MJ_E_HPP_OnlyHuaSe,"1000,0100,0010,"); //只有筒万条,清一色

    
    //MJ_E_HPK_QLD=59, //青龙七对
    Base[MJ_E_HPK_QLD].SType=true; //特殊牌
    Base[MJ_E_HPK_QLD].addMatch(MJ_E_HPP_OnlyHuaSe,"1000,0100,0010,"); //只有筒万条,清一色
    Base[MJ_E_HPK_QLD].ExType.emplace_back(MJ_E_HPK_1LD); //前序牌型；龙7对
    
    //MJ_E_HPK_Q2L=43, //一色双龙会
    Base[MJ_E_HPK_Q2L].ExType.emplace_back(MJ_E_HPK_QYS); //前序牌型；清一色
    Base[MJ_E_HPK_Q2L].addMatch(MJ_E_HPP_OnlyType,"01130000,01220000,01310000,01400000,01040000,"); //四个顺
    Base[MJ_E_HPK_Q2L].addMatch(MJ_E_HPP_SLine, "1|246,"); //四个点，横线(2-2)，间隔6,
    
    
//杠计算
    //MJ_E_HPK_12C=1, 十二金钗
    Base[MJ_E_HPK_12C].addMatch(MJ_E_HPP_OnlyType,
                    "01****03,01****12,01****21,01****30,");//三个杠
    
    //MJ_E_HPK_18Z=3, //十八学士
    Base[MJ_E_HPK_18Z].addMatch(MJ_E_HPP_OnlyType,
                    "01000004,01000013,01000022,01000031,01000040,"); //四个杠
    
    //MJ_E_HPK_2AG=7, //双暗杠
    Base[MJ_E_HPK_2AG].addMatch(MJ_E_HPP_OnlyType,"01*****2,"); //两个2暗杠
    
    //MJ_E_HPK_2MG=11, //双明杠
    Base[MJ_E_HPK_2MG].addMatch(MJ_E_HPP_OnlyType,"01****2*,"); //两个2明杠
    
//刻计算
    //MJ_E_HPK_19K=4, //幺九刻
    {
        vector <int> IDs={0,8,10,18,20,28};
        Base[MJ_E_HPK_19K].addMatch(MJ_E_HPP_OnlyPX,"1|"+Tools.Ke2Str(IDs));
    }
    //有1个牌型要求，数量1，内容19刻
    
    //MJ_E_HPK_1JK=5, //箭刻
    //MJ_E_HPK_2JK=9, //双箭刻
    //MJ_E_HPK_D3Y=28, //大三元
    {
        vector <int> IDs={34,35,36};
        Base[MJ_E_HPK_1JK].addMatch(MJ_E_HPP_OnlyPX,"1|"+Tools.Ke2Str(IDs));
        //1个中发白刻或者杠
        Base[MJ_E_HPK_2JK].addMatch(MJ_E_HPP_OnlyPX,"2|"+Tools.Ke2Str(IDs));
        //2个中发白刻或者杠
        Base[MJ_E_HPK_D3Y].addMatch(MJ_E_HPP_OnlyPX,"3|"+Tools.Ke2Str(IDs));
        //3个中发白刻或者杠
    }
    
    //MJ_E_HPK_X3Y=66, //小三元
    Base[MJ_E_HPK_X3Y].ExType.emplace_back(MJ_E_HPK_2JK); //前序双箭刻
    Base[MJ_E_HPK_X3Y].addMatch(MJ_E_HPP_OnlyPX,"1|134,135,136,");
    //有箭对

    //MJ_E_HPK_2AK=8, //双暗刻
    Base[MJ_E_HPK_2AK].addMatch(MJ_E_HPP_OnlyType,
        "01***2**,01***1*1,");  //两个暗刻
    
    // MJ_E_HPK_3AK=17, //三暗刻
    Base[MJ_E_HPK_3AK].addMatch(MJ_E_HPP_OnlyType,
        "01***3**,01***2*1,01***1*2,");  //三个暗刻
    
    //MJ_E_HPK_4AK=21, //四暗刻
    Base[MJ_E_HPK_4AK].addMatch(MJ_E_HPP_OnlyType,
        "01000400,01000301,01000202,01000103,"); //四个暗刻

    
    //MJ_E_HPK_3FK=18, //三风刻
    //MJ_E_HPK_D4X=29, //大四喜
    {
        vector <int> IDs={30,31,32,33};
        Base[MJ_E_HPK_3FK].addMatch(MJ_E_HPP_OnlyPX,"3|"+Tools.Ke2Str(IDs));
        //三个东南西北风的刻或者杠
        Base[MJ_E_HPK_D4X].addMatch(MJ_E_HPP_OnlyPX,"4|"+Tools.Ke2Str(IDs));
        //四个东南西北风的刻或者杠
    }
    //三个东南西北风的刻或者杠
    
    //MJ_E_HPK_X4X=67, //小四喜
    Base[MJ_E_HPK_X4X].ExType.emplace_back(MJ_E_HPK_3FK); //前序三风刻
    Base[MJ_E_HPK_X4X].addMatch(MJ_E_HPP_OnlyPX,"1|1|130,131,132,133,"); //有一个风向对

    //MJ_E_HPK_2TK=12, //双同刻
    Base[MJ_E_HPK_3TK].addMatch(MJ_E_HPP_SLine,"1|210,"); //刻横线，间隔0
    
    //MJ_E_HPK_3TK=20, //三同刻
    Base[MJ_E_HPK_3TK].addMatch(MJ_E_HPP_KLine,"1|310,"); //刻竖线，间隔0

//顺计算
    //MJ_E_HPK_D19=27, //断幺九
    Base[MJ_E_HPK_D19].addMatch(MJ_E_HPP_ShuZi,"0*******0,");//没有19牌
    
    //MJ_E_HPK_DY5=31, //大于五
    Base[MJ_E_HPK_DY5].addMatch(MJ_E_HPP_ShuZi,"00000****,"); //都是6789
    
    //MJ_E_HPK_QDP=54, //全大
    Base[MJ_E_HPK_QDP].addMatch(MJ_E_HPP_ShuZi,"000000***,"); //都是789
    
    //MJ_E_HPK_XY5=69, //小于五
    Base[MJ_E_HPK_XY5].addMatch(MJ_E_HPP_ShuZi,"****00000,"); //都是1234
    
    //MJ_E_HPK_QXP=62, //全小
    Base[MJ_E_HPK_QXP].addMatch(MJ_E_HPP_ShuZi,"***000000,"); //都是123
    
    //MJ_E_HPK_QZP=64, //全中
    Base[MJ_E_HPK_QZP].addMatch(MJ_E_HPP_ShuZi,"000***000,"); //都是456
    
    //MJ_E_HPK_QJP=58, //全将牌
    Base[MJ_E_HPK_QJP].addMatch(MJ_E_HPP_ShuZi,"0*00*00*0,"); //只允许258牌
    
    //MJ_E_HPK_6LS=23, //六连顺
    Base[MJ_E_HPK_6LS].addMatch(MJ_E_HPP_SLine,"1|223,"); //顺横线，间隔3
    //联立集合，两个顺连接）
    
    //MJ_E_HPK_1TL=74, //一条龙
    Base[MJ_E_HPK_1TL].addMatch(MJ_E_HPP_SLine,"1|323,"); //顺横线，间隔3
    
    //MJ_E_HPK_HHL=33, //花龙
    Base[MJ_E_HPK_HHL].addMatch(MJ_E_HPP_SLine,"1|333,"); //顺斜线，间隔3
    
    //MJ_E_HPK_TBD=65, //推不倒
    Base[MJ_E_HPK_TBD].addMatch(MJ_E_HPP_OnlyCard,"00,01,02,03,04,07,08,11,13,14,15,17,18,");//都是1234589筒和245689条

    //MJ_E_HPK_QYS=63, //清一色
    Base[MJ_E_HPK_QYS].addMatch(MJ_E_HPP_OnlyHuaSe,"1000,0100,0010,"); //只有筒万条
    
    //MJ_E_HPK_Q1L=42, //清龙
    Base[MJ_E_HPK_Q1L].ExType.emplace_back(MJ_E_HPK_QYS); //前序牌型；清一色
    Base[MJ_E_HPK_Q1L].ExType.emplace_back(MJ_E_HPK_1TL); //前序牌型；一条龙
    
    //MJ_E_HPK_QLS=60, //绿一色
    Base[MJ_E_HPK_QLS].addMatch(MJ_E_HPP_OnlyHuaSe,"0100,");//只有条
    Base[MJ_E_HPK_QLS].addMatch(MJ_E_HPP_OnlyCard,"11,12,13,15,17,35,");//都是23468条和发财
    
    //MJ_E_HPK_Q9D=51, //九莲宝灯
    Base[MJ_E_HPK_Q9D].ExType.emplace_back(MJ_E_HPK_QYS); //前序牌型；清一色
    Base[MJ_E_HPK_Q9D].addMatch(MJ_E_HPP_ShuZi,
        "411111113,321111113,312111113,311211113,311121113,311112113,311111213,311111123,311111114,");
    
    //MJ_E_HPK_PPH=40, //碰碰和
    Base[MJ_E_HPK_PPH].addMatch(MJ_E_HPP_OnlyType,"0100****,");//没有顺
    
    //MJ_E_HPK_5MQ=22, //五门齐
    Base[MJ_E_HPK_5MQ].addMatch(MJ_E_HPP_OnlyHuaSe,"1111,"); //筒万条风都有
    {
        vector <int> IDs={34,35,36};
        Base[MJ_E_HPK_5MQ].addMatch(MJ_E_HPP_OnlyPX,"1|134,135,136,"+Tools.Ke2Str(IDs));
    }
    //有中发白
    {
        vector <int> IDs={30,31,32,33};
        Base[MJ_E_HPK_5MQ].addMatch(MJ_E_HPP_OnlyPX,"1|130,131,132,133,"+Tools.Ke2Str(IDs));
    }
    //有东南西北
    
    //MJ_E_HPK_HYS=34, //混一色
    Base[MJ_E_HPK_HYS].addMatch(MJ_E_HPP_OnlyHuaSe,"1001,0101,0011,");
    //只有筒万条和风
    
    //MJ_E_HPK_LSF=36, //老少副
    Base[MJ_E_HPK_LSF].addMatch(MJ_E_HPP_SLine,"1|226,"); //顺横线，间隔6
    
    //MJ_E_HPK_32L=13, //三色双龙会
    Base[MJ_E_HPK_32L].addMatch(MJ_E_HPP_OnlyHuaSe,"1110,");
    Base[MJ_E_HPK_32L].addMatch(MJ_E_HPP_OnlyPX,"1|104,114,124,");//有5对
    Base[MJ_E_HPK_32L].addMatch(MJ_E_HPP_SLine,"2|226,"); //有两个间隔为6的顺，123、789
    
    //MJ_E_HPK_HDY=55, //全带幺
    Base[MJ_E_HPK_HDY].addMatch(MJ_E_HPP_OnlyPX,"1|100,108,110,118,120,108,");//有19对
    {
        vector <int> IDs={0,8,10,18,20,28};
        Base[MJ_E_HPK_HDY].addMatch(MJ_E_HPP_OnlyPX,"1|"+Tools.Shun2Str(IDs)+Tools.Ke2Str(IDs));
    }
    
    //MJ_E_HPK_QDY=77, //清带幺
    Base[MJ_E_HPK_QDY].ExType.emplace_back(MJ_E_HPK_HDY); //前序牌型；全带幺
    Base[MJ_E_HPK_QDY].ExType.emplace_back(MJ_E_HPK_QYS); //前序牌型；混一色
    
    // MJ_E_HPK_HPP=75, //混碰
    Base[MJ_E_HPK_HPP].ExType.emplace_back(MJ_E_HPK_HYS); //前序牌型；混一色
    Base[MJ_E_HPK_HPP].ExType.emplace_back(MJ_E_HPK_PPH); //前序牌型；碰碰胡
    
    // MJ_E_HPK_QPP=76, //清碰
    Base[MJ_E_HPK_QPP].ExType.emplace_back(MJ_E_HPK_QYS); //前序牌型；清一色
    Base[MJ_E_HPK_QPP].ExType.emplace_back(MJ_E_HPK_PPH); //前序牌型；碰碰胡
    
    //MJ_E_HPK_Z1S=71, //风碰
    Base[MJ_E_HPK_Z1S].ExType.emplace_back(MJ_E_HPK_PPH); //前序牌型；碰碰胡
    Base[MJ_E_HPK_Z1S].addMatch(MJ_E_HPP_OnlyHuaSe,"0001"); //只有风
    
    
    //MJ_E_HPK_QSK=61, //全双刻
    Base[MJ_E_HPK_QSK].ExType.emplace_back(MJ_E_HPK_PPH); //前序牌型；碰碰胡
    Base[MJ_E_HPK_QSK].addMatch(MJ_E_HPP_ShuZi,"0*0*0*0*0");//只有数字2468
    
    //MJ_E_HPK_H19=32, //混幺九
    Base[MJ_E_HPK_H19].addMatch(MJ_E_HPP_OnlyHuaSe,"***1,"); //有风
    Base[MJ_E_HPK_H19].addMatch(MJ_E_HPP_ShuZi,"*0000000*");//只有数字19
    
    //MJ_E_HPK_Q19=41, //清幺九
    Base[MJ_E_HPK_Q19].addMatch(MJ_E_HPP_OnlyHuaSe,"***0,"); //无风
    Base[MJ_E_HPK_Q19].addMatch(MJ_E_HPP_ShuZi,"*0000000*");//只有数字19
    
    //MJ_E_HPK_QD5=53, //全带五
    Base[MJ_E_HPK_QD5].addMatch(MJ_E_HPP_OnlyPX,"1|104,114,124,");//有5对
    {
        vector <int> IDs={4,14,24};
        Base[MJ_E_HPK_QD5].addMatch(MJ_E_HPP_OnlyPX,"1|"+Tools.Shun2Str(IDs)+Tools.Ke2Str(IDs));
    }
    //对顺刻杠中，都包含5牌
    
    //MJ_E_HPK_33G=14, //三色三步高
    Base[MJ_E_HPK_33G].addMatch(MJ_E_HPP_SLine,"1|331,332,"); //顺斜线，间隔1或者2
    
    //MJ_E_HPK_33J=15, //三色三节高
    Base[MJ_E_HPK_33J].addMatch(MJ_E_HPP_KLine,"1|331,"); //刻斜线，间隔1
    
    //MJ_E_HPK_33S=16, //三色三同顺
    Base[MJ_E_HPK_33S].addMatch(MJ_E_HPP_SLine,"1|310,"); //顺竖线，间隔0
    
    //MJ_E_HPK_Q3G=44, //一色三步高
    Base[MJ_E_HPK_Q3G].addMatch(MJ_E_HPP_SLine,"1|321,"); //顺横线，间隔1
    
    //MJ_E_HPK_Q4G=47, //一色四步高
    Base[MJ_E_HPK_Q4G].addMatch(MJ_E_HPP_SLine,"1|421,"); //顺横线，间隔1

    //MJ_E_HPK_Q3J=45, //一色三节高
    Base[MJ_E_HPK_Q3J].addMatch(MJ_E_HPP_KLine,"1|321,"); //刻横线，间隔1
    
    //MJ_E_HPK_Q4J=48, //一色四节高
    Base[MJ_E_HPK_Q4J].addMatch(MJ_E_HPP_KLine,"1|421,"); //刻横线，间隔1

    //MJ_E_HPK_YBG=70, //一般高
    Base[MJ_E_HPK_YBG].addMatch(MJ_E_HPP_SLine,"1|200,"); //顺点
    
    //MJ_E_HPK_XXF=68, //喜相逢
    Base[MJ_E_HPK_XXF].addMatch(MJ_E_HPP_SLine,"1|210,"); //顺竖线，间隔0
    
    //MJ_E_HPK_Q3S=46, //一色三同顺
    Base[MJ_E_HPK_Q3S].addMatch(MJ_E_HPP_SLine,"1|300,"); //顺点
    
    //MJ_E_HPK_Q4S=49, //一色四同顺
    Base[MJ_E_HPK_Q4S].addMatch(MJ_E_HPP_SLine,"1|400,"); //顺差为0
    
    //MJ_E_HPK_PH=39, //平和
    Base[MJ_E_HPK_PH].addMatch(MJ_E_HPP_OnlyType,"01400000,01040000,");//四个顺
    
    //MJ_E_HPK_13Y=2, //十三幺
    Base[MJ_E_HPK_13Y].SType=true; //特殊牌
    Base[MJ_E_HPK_13Y].addMatch(MJ_E_HPP_OnlyCard,
        "00,08,10,18,20,28,30,31,32,33,34,35,36,");
    
    //MJ_E_HPK_LFX=73, //乱风向
    Base[MJ_E_HPK_LFX].SType=true; //特殊牌
    Base[MJ_E_HPK_LFX].addMatch(MJ_E_HPP_OnlyHuaSe,"0001"); //只有风
    
    //MJ_E_HPK_7BK=24, //七星不靠
    Base[MJ_E_HPK_7BK].SType=true; //特殊牌
    Base[MJ_E_HPK_7BK].addMatch(MJ_E_HPP_SType,"7"); //7星不靠
    
    //MJ_E_HPK_ZHL=72, //组合龙
    Base[MJ_E_HPK_ZHL].SType=true; //特殊牌
    Base[MJ_E_HPK_ZHL].addMatch(MJ_E_HPP_SType,"Z"); //组合龙
    
    //MJ_E_HPK_QBK=52, //全不靠
    Base[MJ_E_HPK_QBK].SType=true; //特殊牌
    Base[MJ_E_HPK_QBK].addMatch(MJ_E_HPP_SType,"Q"); //全不靠
    
    //MJ_E_HPK_LJH=35, //无番和
    Base[MJ_E_HPK_LJH].addMatch(MJ_E_HPP_OnlyType,"01******"); //全不靠
    
/*
    MJ_E_HPK_BQR=26, //不求人
    
    MJ_E_HPK_LJH=35, //无番和
    MJ_E_HPK_MFK=37, //门风刻
    MJ_E_HPK_MQQ=38, //门前清

    MJ_E_HPK_QFK=56, //圈风刻
 */
}

void MJ_C_HPAllKinds::ShowBase(){
    for(int i=0;i<MJ_E_HPK_End;i++){
        Base[i].show();
        if (Base[i].ExType.size()>0){
            cout << "  前序牌型： ";
            for(auto n1:Base[i].ExType)
                cout << MJC.getHPHZ(n1) << " | ";
            cout << endl;
        }
    }
}

//基于char数组的模糊对应匹配
bool MJ_C_HPCeckTools::match(char *a, const char *b, size_t l){ //模糊匹配
/*匹配规则：
 1、按顺序每字节匹配
 2、以b为模板，'*'为可有可无，'0'-'9'必须一样；
 3、a对应字节中有一个不符合返回false；
*/
    bool R=false;
    int i=0;
    
    for (i=0; i<l; i++){
        if (b[i]=='*') {R=true; continue;}
        if (b[i]==a[i]) R=true;
        else {R=false; break;}
    }
    return R;
}

bool MJ_C_HPCeckTools::Func_ShuZi(MJ_S_HPPatt &P, MJ_C_CardToTal &CT){
    //数字要求，下标是数字，内容'*'可以有，‘0-9’数量
    bool R=false;
    
    for (auto t:P.Set){
        R=match(CT.ShuZi, t.c_str(), 9);
        if (R) break;
    }
    return R;
}

bool MJ_C_HPCeckTools::Func_OnlyType(MJ_S_HPPatt &P, MJ_C_CardToTal &CT){
    //只能有的牌型集合，顺序为MJCardType，一个字节对应一个内容，
    //"0700000" 7对 ,'*'可以有，‘0-9’数量
    bool R=false;
    
    for (auto t:P.Set){
        R=match(CT.PaiXing, t.c_str(), MJ_E_PX_End);
        if (R) break;
    }
    return R;
}

bool MJ_C_HPCeckTools::Func_OnlyHuaSe(MJ_S_HPPatt &P, MJ_C_CardToTal &CT){
//只能有的花色集合，顺序为MJType，“，”分割
//"1001，0101，0011" //混一色
    bool R=false;
    char HS[MJ_E_HS_End];
    
    for(int i=0;i<MJ_E_HS_End-1; i++)
        HS[i]=(CT.HuaSe[i]=='0')?'0':'1';
    
    for (auto t:P.Set){
        R=match(HS, t.c_str(), MJ_E_HS_End-1);
        if (R) break;
    }
    return R;
}

bool MJ_C_HPCeckTools::Func_OnlyCard(MJ_S_HPPatt &P, MJ_C_CardToTal &CT){
    //只能有牌集合，内容每张牌的唯一码，“，”分割
    //内容为"00,08,10,18,20,28,30,31,32,33,34,35,36,"
    bool R=true;
    
    for (auto t:CT.BaseCardSet){
        for (auto n:P.Set)
            if (t!=stoi(n)) { R=false; break;}
        if (!R) break;
    }
    return R;
}

bool MJ_C_HPCeckTools::Func_OnlyPX(MJ_S_HPPatt &P, MJ_C_CardToTal &CT){
    //只能有的的牌型集合，a|xyz
    //a:数量
    //xyz:允许的内容集合
    //"1|300,400,500,308,408,508,"); //有1个牌型要求，数量1，内容19刻
    //要求，PX是从小到大排序好的
    int j=P.A[0],t1; //取出个数
    
    for (auto t:CT.PXs){  //取出牌型
        for (auto s:P.Set){ //遍历所有的set
            t1=stoi(s);
            if (t==t1) {
                j--;break; //找到一次,退出内循环；
            }
        }
        if (j==0) break; //找到指定次数，退出外循环
    }
    return (j==0);
}

bool MJ_C_HPCeckTools::Func_SType(MJ_S_HPPatt &P, MJ_C_CardToTal &CT){ //特殊牌型检查
    //"7":七星不靠
    //"Z":组合龙
    //"Q":全不靠
    bool R=false;
    
    for(int i=0; i<9; i++)
        if (CT.ShuZi[i]>'1') return R; //数字的个数超过1，
    
    for(int i=0; i<MJ_E_HS_End-1; i++)
        if (CT.HuaSe[i]<'1') return R; //不是筒万条风组成，
    
    for(int i=1; i<MJ_E_PX_End; i++)
        if (CT.PaiXing[i]>'0') return R; //有牌型
    
    if (!CT.isAllSingle) return R; //不是14张单独的牌
    
    if ((CT.HuaSe[MJ_E_HS_Wan]>'3')||
        (CT.HuaSe[MJ_E_HS_Tong]>'3')||
        (CT.HuaSe[MJ_E_HS_Tiao]>'3'))
        return R; //筒万条中有一种张数超过4
    
    int j=0,A=-1;
    for (auto n:CT.BaseCardSet){
        if (j<1) { A=n; j++; continue; }//跳过第一个数
        if (n/10==MJ_E_HS_Feng) continue; //不是风
        if ((n-A)<3) return false;//相同花色且差值不为3,不是147、258、369关系
        A=n;
    }
    
    if (P.A[0]=='Q') return true; //全不靠
    if (P.A[0]=='7') { //七星不靠,检查是否有7张风
        for (int i=0; i<7; i++)
            if (CT.FengZS[i]!='1') return false;
        return true;
    }
    
    if (P.A[0]=='Z') { //组合龙
        for (int i=0; i<9; i++)
            if (CT.ShuZi[i]!='1') return false;
        return true;
    }
    return R;
}

bool MJ_C_HPCeckTools::Func_Line(MJ_S_HPPatt &P,set<int> &Line){ //线型关系检查
    int j=P.A[0],t1; //查几个
    
    for (auto t:Line){  //取出线型
        for (auto s:P.Set){ //遍历所有的set
            t1=stoi(s);
            if (t/100==t1) {
                j--;break; //找到一次,退出内循环；
            }
        }
        if (j==0) break; //找到指定次数，退出外循环
    }
    return (j==0);
}
