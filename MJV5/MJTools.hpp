//
//  MJTools.hpp
//  MJ
//  工具与常量
//  Created by Bob Yin on 2022/7/1.
//

#ifndef MJTools_hpp
#define MJTools_hpp

#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

namespace MJClassTools{   //MJClassTools V1.0 命名空间

enum MJ_E_MaxConst{ //最大牌型数
    MJ_MaxPXNum=4 //一手牌最大牌型数，不包含对；常规为4，就是一般4*3+1=手里牌，4*3+2为胡牌张数
};

enum MJ_E_HuaSe{  //麻将牌花色标识 最大不超过7
    MJ_E_HS_Tong=0, //筒
    MJ_E_HS_Tiao=1, //条
    MJ_E_HS_Wan=2,  //万
    MJ_E_HS_Feng=3, //风向
    MJ_E_HS_Hua=4,  //花
    MJ_E_HS_End=5 //结束标志
};

enum MJ_E_PaiXing{ //麻将牌型标识，最大不超过7
    MJ_E_PX_NULL=0,  //不成牌
    MJ_E_PX_Dui=1,   //对子
    MJ_E_PX_MShun=2,  //吃出的顺
    MJ_E_PX_AShun=3,  //手里的顺
    MJ_E_PX_MKe=4,    //明刻，碰出
    MJ_E_PX_AKe=5,    //暗刻
    MJ_E_PX_MGang=6,   //明杠
    MJ_E_PX_AGang=7,    //暗杠
    MJ_E_PX_End=8
};

enum MJ_E_CardOnTable{ //牌状态标识1
    MJ_E_CT_Normal=0, //没摸
    MJ_E_CT_inEast=1,   //东家手里
    MJ_E_CT_inSourth=2,   //南家手里
    MJ_E_CT_inWest=3,   //西家家手里
    MJ_E_CT_inNorth=4,  //北家手里
    MJ_E_CT_inRiver=5, //打到河里
    MJ_E_CT_End=6
};

/*
enum MJ_E_CardStatus{ //牌状态标识2
    MJ_E_CS_inHand=0,  //没用，在手里
    MJ_E_CS_onTable=1, //固定在桌上，吃出（明顺）、碰出（明刻）、杠出（明杠、补杠）等
    MJ_E_CS_Booking=2, //计划绑定为对、暗顺、暗刻等
    MJ_E_CS_End=3
};*/

enum MJ_E_DongZuoBool{ //牌的布尔数组序号
    MJ_E_DZB_canEat=0, //能吃否
    MJ_E_DZB_canPeng=1,//能碰否
    MJ_E_DZB_canOut=2, //能出否
    MJ_E_DZB_canHuan=3,//能换牌否
    MJ_E_DZB_isBaida=4,//百搭
    MJ_E_DZB_End=5
};

// 牌型唯一码编码规则， MJType*10+N
enum MJ_E_HuPaiKind{ //胡牌牌型
    MJ_E_HPK_421=0, //四归一
    MJ_E_HPK_12C=1, //十二金钗
    MJ_E_HPK_13Y=2, //十三幺
    MJ_E_HPK_18Z=3, //十八学士
    MJ_E_HPK_19K=4, //幺九刻
    MJ_E_HPK_1JK=5, //箭刻
    MJ_E_HPK_1LD=6, //龙七对
    MJ_E_HPK_2AG=7, //双暗杠
    MJ_E_HPK_2AK=8, //双暗刻
    MJ_E_HPK_2JK=9, //双箭刻
    MJ_E_HPK_2LD=10, //双龙7对
    MJ_E_HPK_2MG=11, //双明杠
    MJ_E_HPK_2TK=12, //双同刻
    MJ_E_HPK_32L=13, //三色双龙会
    MJ_E_HPK_33G=14, //三色三步高
    MJ_E_HPK_33J=15, //三色三节高
    MJ_E_HPK_33S=16, //三色三同顺
    MJ_E_HPK_3AK=17, //三暗刻
    MJ_E_HPK_3FK=18, //三风刻
    MJ_E_HPK_3LD=19, //三龙七对
    MJ_E_HPK_3TK=20, //三同刻
    MJ_E_HPK_4AK=21, //四暗刻
    MJ_E_HPK_5MQ=22, //五门齐
    MJ_E_HPK_6LS=23, //六连顺
    MJ_E_HPK_7BK=24, //七星不靠
    MJ_E_HPK_7ZD=25, //七对
    MJ_E_HPK_BQR=26, //不求人
    MJ_E_HPK_D19=27, //断幺九
    MJ_E_HPK_D3Y=28, //大三元
    MJ_E_HPK_D4X=29, //大四喜
    MJ_E_HPK_D7L=30, //连七对
    MJ_E_HPK_DY5=31, //大于五
    MJ_E_HPK_H19=32, //混老头
    MJ_E_HPK_HHL=33, //花龙
    MJ_E_HPK_HYS=34, //混一色
    MJ_E_HPK_LJH=35, //无番和
    MJ_E_HPK_LSF=36, //老少副
    MJ_E_HPK_MFK=37, //门风刻
    MJ_E_HPK_MQQ=38, //门前清
    MJ_E_HPK_PH=39, //平和
    MJ_E_HPK_PPH=40, //碰碰和
    MJ_E_HPK_Q19=41, //清老头
    MJ_E_HPK_Q1L=42, //清龙
    MJ_E_HPK_Q2L=43, //一色双龙会
    MJ_E_HPK_Q3G=44, //一色三步高
    MJ_E_HPK_Q3J=45, //一色三节高
    MJ_E_HPK_Q3S=46, //一色三同顺
    MJ_E_HPK_Q4G=47, //一色四步高
    MJ_E_HPK_Q4J=48, //一色四节高
    MJ_E_HPK_Q4S=49, //一色四同顺
    MJ_E_HPK_Q7D=50, //清7对
    MJ_E_HPK_Q9D=51, //九莲宝灯
    MJ_E_HPK_QBK=52, //全不靠
    MJ_E_HPK_QD5=53, //全带五
    MJ_E_HPK_QDP=54, //全大
    MJ_E_HPK_HDY=55, //全带幺
    MJ_E_HPK_QFK=56, //圈风刻
    MJ_E_HPK_QJD=57, //将对
    MJ_E_HPK_QJP=58, //全将牌
    MJ_E_HPK_QLD=59, //青龙七对
    MJ_E_HPK_QLS=60, //绿一色
    MJ_E_HPK_QSK=61, //全双刻
    MJ_E_HPK_QXP=62, //全小
    MJ_E_HPK_QYS=63, //清一色
    MJ_E_HPK_QZP=64, //全中
    MJ_E_HPK_TBD=65, //推不倒
    MJ_E_HPK_X3Y=66, //小三元
    MJ_E_HPK_X4X=67, //小四喜
    MJ_E_HPK_XXF=68, //喜相逢
    MJ_E_HPK_XY5=69, //小于五
    MJ_E_HPK_YBG=70, //一般高
    MJ_E_HPK_Z1S=71, //字一色
    MJ_E_HPK_ZHL=72, //组合龙
    MJ_E_HPK_LFX=73, //乱风向
    MJ_E_HPK_1TL=74, //一条龙
    MJ_E_HPK_HPP=75, //混碰
    MJ_E_HPK_QPP=76, //清碰
    MJ_E_HPK_QDY=77, //清带幺
    MJ_E_HPK_End=78
};

enum MJ_E_HPPatternC{  //牌型算翻的模式匹配常量，
    MJ_E_HPP_OnlyCard=0, //只能有牌集合，内容每张牌的唯一码，“，”分割
    //内容为"00,08,10,18,20,28,30,31,32,33,34,35,36,"
    MJ_E_HPP_OnlyType=1, //只能有的牌型集合，顺序为MJCardType，一个字节对应一个内容，
    //"0700000" 7对 ,'*'可以有，‘0-9’数量
    MJ_E_HPP_OnlyHuaSe=2, //只能有的花色集合，顺序为MJType，“，”分割
    //"1001，0101，0011" //混一色
    MJ_E_HPP_OnlyPX=3,  //只能有的牌型集合，
    //"1|300,400,500,308,408,508,"); //数量1，内容19刻
    MJ_E_HPP_4Cards=4, //有四张牌，内容为数量 “2”，有两个4张
    MJ_E_HPP_ShuZi=5, //数字要求(去除一对后的，7对是全部数字)，下标是数字，内容'*'可以有，‘0-9’数量
//    MJ_E_HPP_TypeShuZi=6, //制定牌型数字要求，"a|b|c|d"
    //a:s顺，k刻、杠
    //b:联立检查的花色
    //c:联立检查的数量要求
    //d:数字间隔
    //“k|2|2|0”,两种花色2个刻数字间隔0,"s|1|2|1"1个花色2个顺起始数字间隔1
    MJ_E_HPP_SType=6,
    //"7":七星不靠
    //"Z":组合龙
    //"Q":全不靠
//    MJ_E_HPP_inType=8, //牌型包含指定牌"a|b1,b2,"
    //a:s顺，k刻、杠
    //b:牌ID
    MJ_E_HPP_SLine=7, //顺牌要求,"xyz,xyz"
    //x联立点数(2.3.4)，
    //y线型(0：点，1:竖线，数字相同，2:横线，花色相同，3:斜线
    //z数字间隔
    //331,三点斜线间隔1，三色三步高
    MJ_E_HPP_KLine=8, //刻牌要求,"xyz,xyz"
    //x联立点数(2.3.4)，
    //y线型(0：点，1:竖线，数字相同，2:横线，花色相同，3:斜线
    //z数字间隔
    MJ_E_HPP_End=9
};

struct MJ_S_HPFan{
    int FanS;//翻数
    vector<MJ_E_HuPaiKind> CardType; //牌型集合
    bool CanDJ=true; //能否叠加计算
};

//地域麻将定义类，包括牌张数、种类
struct MJ_S_Location {
    char Type[MJ_E_HS_End][9]={{'N','N','N','N','N','N','N','N','N'},//九张筒，标准牌，
                               {'N','N','N','N','N','N','N','N','N'},//九张条，标准牌，
                               {'N','N','N','N','N','N','N','N','N'},//九张万，标准牌，
                               {'F','F','F','F','F','F','F'}, //7张风，风牌
                               {'H','H','H','H','H','H','H','H'}}; //8张花，花牌
    //功能类型有:‘N’普通类，能吃碰、出；‘F’风类，只能碰、出；‘H’花类不能出，‘B’百搭，‘0’不用的牌
    int NumTimes[5][2]={ {9,4},{9,4},{9,4}, //36张筒+36张万+36张条
        {7,4},{8,1} }; //28张风+8个花
    bool CanEat=true; //能不能吃
    string LocName; //地域名称
    vector<MJ_S_HPFan> HPFan;
};

//麻将常数类
class MJConstV2{
private:
    const string FengXiangHZ[7]={"东","南","西","北","中","发","白"};
    const string HuaHZ[8]={"梅","兰","竹","菊","春","夏","秋","冬"};
    const string ShuZiHZ[9]={"一","二","三","四","五","六","七","八","九"};
    const string HuaSeHZ[MJ_E_HS_End]={"筒","条","万","风","花"};
    const string PaiXingHZ[MJ_E_PX_End+1]={"","对","顺","暗顺","刻","暗刻","杠","暗杠","无"};
    const map<char, bitset<MJ_E_DZB_End>> CanDo={{'N',7},{'F',6},{'H',8},{'B',23},{'0',7}};
    //功能类型有:(二进制)
    //‘N’普通，能吃、碰、出，不能换、百搭；00111，7
    //‘F’风类，能碰、出，不能吃、换、百搭；00110，6
    //‘H’花类，能换，不能吃、碰、出、百搭；01000，8
    //‘B’百搭，能吃、碰、出、百搭，不能换；10111，23
    //‘0’，等同N，7
    const string HuPaiKindHZ[MJ_E_HPK_End]={
        "四归一",  "十二金钗","十三幺","十八学士","幺九刻","箭刻","龙七对","双暗杠",
        "双暗刻",  "双箭刻"  ,"双龙7对","双明杠","双同刻","三色双龙会","三色三步高",
        "三色三节高","三色三同顺","三暗刻","三风刻","三龙七对","三同刻","四暗刻",
        "五门齐","六连顺","七星不靠","七对","不求人","断幺九","大三元","大四喜","连七对",
        "大于五","混老头","花龙","混一色","无番和","老少副","门风刻","门前清","平和","碰碰和",
        "清老头","清龙","一色双龙会","一色三步高","一色三节高","一色三同顺","一色四步高",
        "一色四节高","一色四同顺","清7对","九莲宝灯","全不靠","全带五","全大","全带幺",
        "圈风刻","将对","全将牌","青龙七对","绿一色","全双刻","全小","清一色","全中","推不倒","小三元",
        "小四喜","喜相逢","小于五","一般高","字一色","组合龙","乱风向","一条龙","混碰","清碰",
        "清幺九"
    };
    const string HPPatternHZ[MJ_E_HPP_End]={
        "允许的牌集合","允许的牌型集合","允许的花色集合","牌型组合要求","四张数量",
        "允许的数字","特殊牌型要求","顺牌要求","刻、杠牌要求"};
public:
    MJConstV2(){ cout.imbue(locale(locale(),"zh_CN.UTF-8",LC_CTYPE));}
    //（旧）牌、牌型唯一码=牌型*100+类型*10+数字，ID=T*10+N，10进制编码
    
    /*（新）牌、牌型唯一码：16进制编码
     P+H+N组成
     N:数字，4位，[0,8]
     H:花色，4位，MJ-E-HuaSe
     P:牌型，4位，MJ-E-PaiXing
     例子：9筒暗杠, P=MJ_E_AnGang(7), H=MJ_E_HS_Tong(0), N=9
      P   H   N
      7   0   8
     111 000 1000
     */
    const inline uint32_t getNewCardID(int N, MJ_E_HuaSe H, MJ_E_PaiXing P){
        return (P<<8)|(H<<4)|N;
    } //新编码,768=12<<6,特定出错码
    const inline int CID_N2O(uint32_t NCID){ //新编码转换为旧编码
  //      int N=CID&15,H=(CID&(7<<4))>>4,P=(CID&(7<<7))>>7;
        return ((NCID&(15<<8))>>8)*100+((NCID&(15<<4))>>4)*10+(NCID&15);
    }
    const inline int getNCID_No(uint32_t CID){ return (CID==768)?-1:(CID&15);}
    const inline int getNCID_HS(uint32_t CID){ return (CID==768)?-1:((CID&(7<<4))>>4);}
    const inline int getNCID_PX(uint32_t CID){ return (CID==768)?-1:((CID&(7<<7))>>7);}
    const string getNCID_HZ(uint32_t CID){ //获取新编码汉字
        string s;
        int N=CID&15,H=(CID&(15<<4))>>4,P=(CID&(15<<8))>>8;
        if (H<MJ_E_HS_Feng) s=ShuZiHZ[N]+HuaSeHZ[H]+PaiXingHZ[P];
        if (H==MJ_E_HS_Feng) s=FengXiangHZ[N]+HuaSeHZ[H]+PaiXingHZ[P];
        if (H==MJ_E_HS_Hua) s=HuaHZ[N]+HuaSeHZ[H]+PaiXingHZ[P];
        return s;
    };
    
    
    const inline string getCardHZ(int ID){
        string s;
        if (((ID%100)/10)<MJ_E_HS_Feng)
            s=ShuZiHZ[(ID%100)%10]+HuaSeHZ[(ID%100)/10]+PaiXingHZ[ID/100];
        if (((ID%100)/10)==MJ_E_HS_Feng)
            s=FengXiangHZ[(ID%100)%10]+HuaSeHZ[(ID%100)/10]+PaiXingHZ[ID/100];
        if (((ID%100)/10)==MJ_E_HS_Hua)
            s=HuaHZ[(ID%100)%10]+HuaSeHZ[(ID%100)/10]+PaiXingHZ[ID/100];
        return s;
    } //获取汉字
    const inline string getPaiXingHZ(int ID){return PaiXingHZ[ID];} //获取牌型汉字
    const inline string getHPHZ(int ID){return HuPaiKindHZ[ID];} //获取胡牌牌型汉字
    const inline string getHuaSeHZ(int ID){return HuaSeHZ[ID];} //获取花色汉字
    const inline string getHPPatternHZ(int ID){return HPPatternHZ[ID];} //获取模式匹配汉字
    const inline string getShuZiHZ(int ID){return ShuZiHZ[ID];}
    const inline int getT(int ID){ return ID/10;} //获取牌类型
    const inline int getN(int ID){ return ID%10;} //获取牌数字

    bitset<MJ_E_DZB_End> getFunc(MJ_S_Location &L, int ID){
        int c=L.Type[ID/10][ID%10];
        return CanDo.find(c)->second;    //缺省返回‘N’，normal
    }
};

//麻将基类,定义出错信息，返回出错信息
class MJ_C_Base{
private:
    string ErrMsg="";
protected:
    inline void SetErrMsg(string s){ErrMsg=s;}
public:
    inline string GetErrMsg(){return ErrMsg;}
};

class MJ_C_Tools:public MJ_C_Base{
public:
    void inline sortCards(vector<int> &HC){sort(HC.begin(),HC.end());}
    void Sort3int(int &A, int &B, int &C){ //三整数从小到大排序
        if (A>B) swap(A,B);
        if (B>C) swap(B,C);
        if (A>B) swap(A,B);
    }
 //   string TypeCompose(vector<int> IDs, int n); //对于给定的牌型列表，生成n个牌型的组合
    string Ke2Str(vector<int> IDs); //对于给定的ID，生成所有刻和杠的要求牌型字符串，“,”分割
    string Shun2Str(vector<int> IDs); //对于给定的ID，生成所有包含ID的顺牌型字符串，","分割
    int PX2Cards(int PX, int *Temp){ //把牌型还原成基础牌，输入PX，结果放在TempC中，返回有效牌数
        int Len=1;
        Temp[0]=PX%100;
        switch (PX/100){
            case MJ_E_PX_MShun:  //吃出的顺
            case MJ_E_PX_AShun:  //手里的顺
                Temp[1]=PX%100+1;
                Temp[2]=PX%100+2;
                Len=3;
                break;
            case MJ_E_PX_Dui:   //对子
                Temp[1]=PX%100;
                Len=2;
                break;
            case MJ_E_PX_MKe:    //明刻，碰出
            case MJ_E_PX_AKe:    //暗刻
                Temp[1]=PX%100;
                Temp[2]=PX%100;
                Len=3;
                break;
            case MJ_E_PX_MGang:   //明杠
            case MJ_E_PX_AGang:    //暗杠
                Temp[1]=PX%100;
                Temp[2]=PX%100;
                Temp[3]=PX%100;
                Len=4;
                break;
            default:
                break;
        }
        return Len;
    }
};

class MJ_C_LocationBase:public MJ_C_Base{ //地方麻将定义基础类
public:
    MJ_S_Location Loc;
    virtual void set()=0;
};

class MJ_C_Loc_SHQHP:public MJ_C_LocationBase{ //上海清混碰，144,8花
public:
    MJ_C_Loc_SHQHP(){set();}
    void set(){
        Loc.LocName="上海清混碰";
        MJ_S_HPFan HP16,HP8,HP4,HP2,HP1;
        
        //*16的牌型
        HP16.FanS=16;
        HP16.CardType.emplace_back(MJ_E_HPK_Z1S); //字一色
        
        //*8的牌型
        HP8.FanS=8;
        HP8.CardType.emplace_back(MJ_E_HPK_LFX); //乱风向
        HP8.CardType.emplace_back(MJ_E_HPK_QPP); //清碰
        
        //*4的牌型
        HP4.FanS=4;
        HP4.CardType.emplace_back(MJ_E_HPK_QYS); //清一色
        HP4.CardType.emplace_back(MJ_E_HPK_HPP); //混碰
        
        //*2的牌型
        HP2.FanS=2;
        HP2.CardType.emplace_back(MJ_E_HPK_PPH); //碰碰胡
        HP2.CardType.emplace_back(MJ_E_HPK_HYS); //混一色
        HP2.CardType.emplace_back(MJ_E_HPK_BQR); //不求人
        HP2.CardType.emplace_back(MJ_E_HPK_MQQ); //门前清
        
        //*1的牌型
        HP1.FanS=1;
        HP1.CardType.emplace_back(MJ_E_HPK_PH); //平胡
        HP1.CardType.emplace_back(MJ_E_HPK_LJH);//垃圾胡
        
        //必须从小到大放入；        
        Loc.HPFan.emplace_back(HP1);
        Loc.HPFan.emplace_back(HP2);
        Loc.HPFan.emplace_back(HP4);
        Loc.HPFan.emplace_back(HP8);
        Loc.HPFan.emplace_back(HP16);
    }
};

class MJ_C_Loc_SHQM:public MJ_C_LocationBase{   //上海敲痳，144，20花
public:
    MJ_C_Loc_SHQM(){set();}
    void set(){
        Loc.Type[MJ_E_HS_Feng][4]=Loc.Type[MJ_E_HS_Feng][5]=Loc.Type[MJ_E_HS_Feng][6]='H'; //中发白是花
        
        Loc.LocName="上海敲痳";
        
        MJ_S_HPFan HP4,HP2,HP1;
        
        //*4的牌型
        HP4.FanS=4;
        HP4.CardType.emplace_back(MJ_E_HPK_QYS); //清一色
        
        //*2的牌型
        HP2.FanS=2;
        HP2.CardType.emplace_back(MJ_E_HPK_PPH); //碰碰胡
        HP2.CardType.emplace_back(MJ_E_HPK_HYS); //混一色
        HP2.CardType.emplace_back(MJ_E_HPK_BQR); //不求人
        HP2.CardType.emplace_back(MJ_E_HPK_MQQ); //门前清
        
        //*1的牌型
        HP1.FanS=1;
        HP1.CardType.emplace_back(MJ_E_HPK_PH); //平胡
        HP1.CardType.emplace_back(MJ_E_HPK_LJH);//垃圾胡
        
        //必须从小到大放入；
        Loc.HPFan.emplace_back(HP1);
        Loc.HPFan.emplace_back(HP2);
        Loc.HPFan.emplace_back(HP4);
    }
};

class MJ_C_Loc_CMXZDZ:public MJ_C_LocationBase{ //川麻血战到底
public:
    MJ_C_Loc_CMXZDZ(){set();}
    void set(){
        Loc.CanEat=false;
        for (int i=0;i<7;i++) Loc.Type[3][i]='0';//不要风
        Loc.NumTimes[3][0]=Loc.NumTimes[3][1]=0;
        for (int i=0;i<8;i++) Loc.Type[4][i]='0';//不要花
        Loc.NumTimes[4][0]=Loc.NumTimes[4][1]=0;
        
        Loc.LocName="川麻-血战到底";
        
        MJ_S_HPFan HP32,HP16,HP8,HP4,HP2,HP1;
        
        //测试用
        HP32.CardType.emplace_back(MJ_E_HPK_X3Y); //小三元
        HP32.CardType.emplace_back(MJ_E_HPK_7BK); //七星不靠
        HP32.CardType.emplace_back(MJ_E_HPK_ZHL); //组合龙
        HP32.CardType.emplace_back(MJ_E_HPK_33G); //三色三步高
   //     HP4.CardType.emplace_back(MJ_E_HPK_2JK); //老少副
        HP32.CardType.emplace_back(MJ_E_HPK_32L); //三色双龙会
        
        //*32的牌型
        HP32.FanS=32;
        HP32.CardType.emplace_back(MJ_E_HPK_QLD); //清龙七对
        HP32.CardType.emplace_back(MJ_E_HPK_Q2L); //一色双龙会
        
        //*16的牌型
        HP16.FanS=16;
        HP16.CardType.emplace_back(MJ_E_HPK_1LD); //龙七对
        HP16.CardType.emplace_back(MJ_E_HPK_Q7D); //清七对
        HP16.CardType.emplace_back(MJ_E_HPK_QDY); //清幺九
        
        //*8的牌型
        HP8.FanS=8;
        HP8.CardType.emplace_back(MJ_E_HPK_QJP); //全将牌
        HP8.CardType.emplace_back(MJ_E_HPK_QJD); //将七对
        HP8.CardType.emplace_back(MJ_E_HPK_QPP); //清碰
        
        //*4的牌型
        HP4.FanS=4;
        HP4.CardType.emplace_back(MJ_E_HPK_QYS); //清一色
        HP4.CardType.emplace_back(MJ_E_HPK_HDY); //全带幺
        HP4.CardType.emplace_back(MJ_E_HPK_7ZD); //七对
        
        //*2的牌型
        HP2.FanS=2;
        HP2.CardType.emplace_back(MJ_E_HPK_PPH); //碰碰胡
        HP2.CardType.emplace_back(MJ_E_HPK_BQR); //不求人
        HP2.CardType.emplace_back(MJ_E_HPK_MQQ); //门前清
        HP2.CardType.emplace_back(MJ_E_HPK_D19); //断幺九
        
        //*1的牌型
        HP1.FanS=1;
        HP1.CardType.emplace_back(MJ_E_HPK_PH); //平胡
        HP1.CardType.emplace_back(MJ_E_HPK_LJH);//垃圾胡
        
        //必须从小到大放入；
        Loc.HPFan.emplace_back(HP1);
        Loc.HPFan.emplace_back(HP2);
        Loc.HPFan.emplace_back(HP4);
        Loc.HPFan.emplace_back(HP8);
        Loc.HPFan.emplace_back(HP16);
        Loc.HPFan.emplace_back(HP32);
    }
};

struct MJ_S_Card1{ //一张牌
    int ID,No; //这张牌的唯一码和牌桌的序号
    MJ_E_PaiXing HandStat=MJ_E_PX_End; //牌型状态
    MJ_E_CardOnTable TabStat=MJ_E_CT_Normal;
    
    MJ_S_Card1(int I, int N=0):ID(I),No(N){};
    bool operator<(const MJ_S_Card1 b) const  { return this->ID < b.ID;}
    bool operator>(const MJ_S_Card1 b) const  { return this->ID > b.ID;}
    bool operator==(const MJ_S_Card1 b) const  { return this->ID == b.ID;}
    bool operator<=(const MJ_S_Card1 b) const  { return this->ID <= b.ID;}
    bool operator>=(const MJ_S_Card1 b) const  { return this->ID >= b.ID;}
    bool operator!=(const MJ_S_Card1 b) const  { return this->ID != b.ID;}
};

class MJ_C_CardToTal:public MJ_C_Base{
public:
    char ShuZi[10]="000000000",             //数字统计
        HuaSe[MJ_E_HS_End]="0000",         //花色统计
        PaiXing[MJ_E_PX_End+1]="00000000",  //牌型统计
        FengZS[8]="0000000";                //风的张数；
    int Card4=0;                          //四归一的张数
    set <int> PLine,SLine,KLine,          //线数据：牌、顺、刻
        BaseCardSet;                      //牌张集合
    vector <int> PXs;                     //统计对象，牌型或者牌张
    bool isAllSingle=false;
    
    void Show();
    void Clear(){
        strcpy(ShuZi,"000000000");
        strcpy(HuaSe,"0000"); //有的花色统计
        strcpy(PaiXing,"00000000"); //有的牌型统计
        strcpy(FengZS,"0000000");
        Card4=0;
        isAllSingle=false;
        PLine.clear();
        SLine.clear();
        KLine.clear();
        BaseCardSet.clear();
        
        vector<int> p1;
        PXs.swap(p1);
    }
};

class MJ_C_CardsGroup:public MJ_C_Base{ //一组牌或者牌型
private:
    string Name;
    MJConstV2 MJC;
public:
    multiset <int> IDs; //牌集合,不可外部删除元素！！！！
    
    MJ_C_CardsGroup(string s):Name(s){}
    MJ_C_CardsGroup(const multiset <int> &I):Name("Temp"),IDs(I){}
    MJ_C_CardsGroup(const MJ_C_CardsGroup &I):Name(I.Name),IDs(I.IDs){}
    MJ_C_CardsGroup(const MJ_C_CardsGroup *I):Name(I->Name),IDs(I->IDs){}
    
    inline size_t GetNum(int ID){return IDs.count(ID);}
    inline bool FindPX(int P){ return (IDs.find(P)!=IDs.end());}
    inline int Size(){return (int)IDs.size();}
    inline bool isAllSingle(){ return Get4Cards(2)==-1;} //找不到2张以上的相同牌
    inline void Clear(){ IDs.clear();}
    inline void SetName(string s,string b=""){Name=s;}
    inline bool CheckShun(int ID){ return (ID<MJ_E_HS_Feng*10)?(FindPX(ID+1)&FindPX(ID+2)):false;}; //看看有没有顺,ID缺省认为在IDs中且最小
    
    void Insert(int ID, int n=1){ for (int i=0; i<n; i++) IDs.emplace(ID);} //插入几张一样的牌
    void Insert(int *H, int l){ for (int i=0; i<l; i++) IDs.emplace(H[i]);} //插入指定张数的牌
    void Insert(vector<int> &H){ for (auto n:H) IDs.emplace(n);} //插入数组牌
    void Erase(int ID, int n=1){ //删除牌，几张一样的，不够删，就删光
        if (IDs.count(ID)<=n){ IDs.erase(ID); }//不够删,全删光
        else{ for (int i=0; i<n; i++) IDs.erase(IDs.find(ID));} //删除指定次数
    }
    void EraseShun(int ID){Erase(ID);Erase(ID+1);Erase(ID+2);} //删除一顺牌，ID是最小的牌
    int Get4Cards(int Num=4){
        for (auto n: IDs) if (GetNum(n)>=Num) return n;
        return -1;
    }
    
    void GetPX(vector <int> &V) { for (auto n:IDs) V.emplace_back(n);}       //所有牌型ID
    void GetPX(set<int> &CardSet) { for (auto n:IDs) CardSet.emplace(n);}    //去重牌型;
    void GetPX(vector<string> &PX){ for (auto n:IDs) PX.emplace_back(to_string(n));} //所有牌型的string
    void GetPX(vector<pair <int,int>> &CardSetV) {  //所有牌型（ID，Num）
        set<int> C;
        GetPX(C);
        for(auto n:C) CardSetV.emplace_back(make_pair(n,GetNum(n)));
    }
    void GetBaseCards(vector <int> &V){ //所有基础牌
        MJ_C_Tools Tools;
        int l,Temp[4];
        
        for (auto n:IDs) {
            l=Tools.PX2Cards(n,Temp);
            for(int i=0; i<l; i++)
                V.emplace_back(Temp[i]);
        }
    }
    int GetbaseCards(MJ_C_CardsGroup &V){ //所有基础牌，返回根数
        MJ_C_Tools Tools;
        int C4=0;
        int l,Temp[4];
        
        for (auto n:IDs) {
            l=Tools.PX2Cards(n,Temp);
            for(int i=0; i<l; i++) {
                V.Insert(Temp[i]);
                if (V.GetNum(Temp[i])==4) C4++;
            }
        }
        return C4;
    }
    void Show();
};

class MJ_C_LineAct:public MJ_C_Base{ //牌的线型检查类
private:
    MJ_C_CardsGroup PXs;
    bitset<MJ_E_PX_End> enPX; //初始化顺序为逆序，例如"顺","00001100"
    
    int Check4Line(int A, int B, int C, int D, bool addv=true); //返回四个点的线型
    int Check3Line(int A, int B, int C, bool addv=true); //返回三个点的线型
    int Check2Line(int A, int B,bool addv=true); //返回两个点的线型
    inline void Sort3int(int &A, int &B, int &C){ //三整数从小到大排序
        if (A>B) swap(A,B);
        if (B>C) swap(B,C);
        if (A>B) swap(A,B);
    }
    set<int> LineT; //线的标识：xyzID
    //线型，xyz: x联立点数(2.3.4)，
    //          y线型(0：点，1:竖线，数字相同，2:横线，花色相同，3:斜线,>3,四个点，两两相同组成的线
    //          z数字间隔,
    //ID, 最小牌的ID;
public:
    MJ_C_LineAct(string s,string ePX):PXs(s),enPX(ePX){ Clear(); }
    bool add1PX(int Card);
    void Clear() { PXs.Clear(); LineT.clear();}
    void GetLineT(set<int> &L){ for(auto n:LineT) L.emplace(n);}
};

class MJ_C_ToTalAct:public MJ_C_Base{ //牌型统计类
private:
    void add1PX(int ID);
    MJ_C_CardToTal *CT=NULL;
    MJ_C_LineAct PLine,SLine,KLine; //牌、顺、刻线型
public:
    MJ_C_ToTalAct():PLine("牌","00000001"),SLine("顺","00001100"),KLine("刻、杠","11110000"){ Clear(); }
    void Clear(){PLine.Clear(); SLine.Clear(); KLine.Clear();}
    
    void Calc(MJ_C_CardsGroup &CG, MJ_C_CardToTal &CTT){ //依据CardGroup,计算Tot
        MJ_C_CardsGroup BaseCards("TempBaseCards");
        CT=&CTT;

        for (auto n:CG.IDs){
            add1PX(n); //逐个牌型计算统计数
            PLine.add1PX(n); //计算牌线
            SLine.add1PX(n); //计算顺线
            KLine.add1PX(n); //计算刻线
            CTT.PXs.emplace_back(n); //逐个增加牌型
        }
        CTT.Card4=CG.GetbaseCards(BaseCards); //设置根数；
        BaseCards.GetPX(CTT.BaseCardSet); //更新去重牌张
        PLine.GetLineT(CTT.PLine); //更新牌线
        SLine.GetLineT(CTT.SLine); //更新顺线
        KLine.GetLineT(CTT.KLine); //C更新刻线
        CTT.isAllSingle=(CTT.BaseCardSet.size()==CG.Size());
    }
};

class MJ_C_HPJieGuo:public MJ_C_Base{
public:
    vector<int> Kinds; //符合的胡牌种类
    MJ_C_CardsGroup CardPX; //胡的牌型
    MJ_C_CardToTal CardTot; //牌型统计数据
    int Kind=0,Fan=0; //最大牌型和翻数
    
    MJ_C_HPJieGuo(MJ_C_CardsGroup &HC):CardPX(HC){CardPX.SetName("胡牌结果详细信息");};
    void Show(){
        MJConstV2 MJC;
        
        cout << "|" << MJC.getHPHZ(Kind) << "|翻数: " << Fan << "|符合牌型: " ;
        for(auto n:Kinds)
            cout << MJC.getHPHZ(n) << '.';
        cout << endl;
        CardPX.Show();
        CardTot.Show();
        cout << endl;
    }
};

} //MJClassTools

#endif /* MJTools_hpp */
