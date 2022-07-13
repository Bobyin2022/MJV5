//
//  MJCLassV5.cpp
//  MJ
//
//  Created by Bob Yin on 2022/7/13.
//

#include "MJCLassV5.hpp"

using  namespace MJClassV5;

//选要打出的牌，先取出最后一张牌
int MJ_HandCard::chooseCard(){
    return *CardIDs.IDs.rbegin();
}

//碰出，返回要出的一张牌, -1为没有碰出
int MJ_HandCard::pengOut(int ID){
    if (!checkMPeng(ID)) {
        SetErrMsg("牌："+to_string(ID)+"不能碰出");
        return -1;
    }
    OnTableIDs.Insert(MJ_E_PX_MKe*100+ID); //桌上牌加一刻
    CardIDs.Erase(ID,2); //手里牌删2张
    int R=chooseCard(); //选一张牌
    CardIDs.Erase(R); //打出
    return R;
}

//吃出，返回要出的一张牌，-1为没有吃出,How =0,随机吃，1：45吃3，2：35吃4，3：34吃5
int MJ_HandCard::chiOut(int ID, int How){
    if ((How>3)||(How<0)) {
        SetErrMsg("未定义的吃法："+to_string(How));
        return -1;
    }
    int H=checkChi(ID);
    if (H==0) return -1;
    H=(How==0)?H:How;
    
    int i=-1;
    MoPai(ID); //摸进ID
    if (H==1) i=ID; //89吃7；
    if (H==2) i=ID-1; //35吃4；
    if (H==3) i=ID-2; //12吃3；
    OnTableIDs.Insert(MJ_E_PX_MShun*100+i); //桌面加一顺
    CardIDs.EraseShun(i); //手里删除一顺
    int R=chooseCard(); //选一张牌
    CardIDs.Erase(R); //打出
    return R;
}

//杠ID出，补一张ID2，返回要打出的牌，-1为没有碰出，T是要杠的类型
int MJ_HandCard::GangOut(MJ_E_PaiXing T,int ID,int ID2){
    int T1=-1;
    bool OK=false;
    if (T==MJ_E_PX_MGang){  //明杠
        if (checkMGang(ID)) { //手里牌明杠
            OnTableIDs.Insert(T*100+ID);
            CardIDs.Erase(ID,3); //去除手里3张
            OK=true;
        }
        if (checkBGang(ID)) { //桌上牌补杠
            OnTableIDs.Erase(MJ_E_PX_MKe*100+ID); //删除明刻
            OnTableIDs.Insert(MJ_E_PX_MGang*100+ID); //增加明杠
            OK=true;
        }
    }
    if ((T==MJ_E_PX_AGang)&&checkAGang(T1)){ //暗杠
        OK=true;
        OnTableIDs.Insert(T*100+ID);
        CardIDs.Erase(T1,4); //暗杠出
    }
    if (OK) {
        MoPai(ID2);
        T1=chooseCard();
        CardIDs.Erase(T1);
    } else SetErrMsg("牌："+to_string(ID)+"不能杠出");
    return T1;
}

//递归检查剩下的手中牌，是否可以都为一顺或者1刻；
void MJ_HPCalc::Hu3(MJ_C_CardsGroup &HC,MJ_C_CardsGroup &TC,bool SorK){
    if (HC.Size()==0) return;  //胡牌
    
    int T=*HC.IDs.begin();
    if (HC.GetNum(T)==3){ //第一张牌是3张
        if (SorK) { //优先查顺
            if (HC.CheckShun(T)) {//优先查顺是一顺,放在桌面上，再继续,递归
                TC.Insert(MJ_E_PX_AShun*100+T);
                HC.EraseShun(T);
                Hu3(HC,TC,SorK);
            } 
        }else { //优先查刻
            TC.Insert(MJ_E_PX_AKe*100+T);
            HC.Erase(T,3);
            Hu3(HC,TC,SorK);
        }
    }
    else { //第一张牌有只有1.2.4张，
        if (T>=MJ_E_HS_Feng*10) return; //是风，不胡
        if (HC.CheckShun(T)) {//要胡牌，必是一顺，
            TC.Insert(MJ_E_PX_AShun*100+T); //是一顺,放在桌面上，再继续,递归
            HC.EraseShun(T);
            Hu3(HC,TC,SorK);
        }
        return;  //不是顺，不胡；
    }
}

//手中牌是否胡牌，缺省认为桌上牌都是成型的
bool MJ_HPCalc::isHu(MJ_HandCard &Cards, vector<MJ_C_HPJieGuo> &HPData){
    //检查Cards是否胡牌，如果是，添加到HPJG中
    bool R=false;
    
    if ((Cards.CardIDs.Size()%3)!=2) {
        SetErrMsg("手里牌张数不对。");
        return false;
    } //手里牌张数是不是3x+2；
    
    vector<pair<int,int>> TempHandCN; //把手中牌按照牌和张数取出，CN=Card-Num
    Cards.CardIDs.GetPX(TempHandCN);

    //检查不靠牌型
    if (Cards.OnTableIDs.Size()==0) {  //都在手里,按照张数组合成对、刻牌型
        MJ_C_CardsGroup TC("HPCalc-TempTableCard");
        for (auto n:TempHandCN){ //组成对和刻的牌型
            switch (n.second) {
                case 1:
                    TC.Insert(n.first);
                    break;
                case 2:
                    TC.Insert(MJ_E_PX_Dui*100+n.first);
                    break;
                case 3:
                    TC.Insert(MJ_E_PX_AKe*100+n.first);
                    break;
                case 4: //四张拆成两个对
                    TC.Insert(MJ_E_PX_Dui*100+n.first);
                    TC.Insert(MJ_E_PX_Dui*100+n.first);
                    break;
                default:
                    break;
            }
        }
        R=CheckHPKind(TC,HPData,true);//检查特殊胡牌牌型
    }
     
    //遍历所有的将牌，检查剩余牌是否能符合胡牌
    for (auto t:TempHandCN){
        if (t.second<2) continue; //小于2张不能组成对，跳过
        int i=t.first; //可以组成对，取出牌
        
        MJ_C_CardsGroup HC(Cards.CardIDs), TC(Cards.OnTableIDs); //临时数据1，拷贝原始手中牌和桌上牌
        
        //将牌放在桌上
        TC.Insert(MJ_E_PX_Dui*100+i);HC.Erase(i, 2);
        if (HC.Size()==0){ //手里只有两张,胡牌
            R=CheckHPKind(TC,HPData);
            continue;
        }
        
        //遍历剩下的牌
        MJ_C_CardsGroup HC2(HC), TC2(TC); //临时数据2，拷贝临时手中牌和桌上牌
        Hu3(HC,TC); //如有3张先查顺
        if (HC.Size()==0) R=CheckHPKind(TC,HPData); //胡了，确认胡牌牌型
        else{ //先查顺没胡
            Hu3(HC2,TC2,false); //3张先查刻；
            if (HC2.Size()==0) R=CheckHPKind(TC2,HPData); //胡了
        }
    }
    if (R) SetErrMsg("可以胡牌。");
    else SetErrMsg("不胡牌。");
    return R;
}

bool MJ_HPCalc::CheckHPKind(MJ_C_CardsGroup &TC,vector<MJ_C_HPJieGuo> &HPData,bool CheckS){
/*参数说明
 MJ_C_CardsGroup &TC：计算胡牌的牌型数组
 vector<MJ_C_HPJieGuo> &HPData：存放通过胡牌牌型检查的结果数据
 bool CheckS：true，检查特殊牌型（7对、不靠等）；false，检查普通牌型
 */
    bool R=false;
    MJ_C_HPJieGuo HPJG(TC);
    MJ_C_ToTalAct CTA;
    
    CTA.Calc(TC, HPJG.CardTot); //计算统计值
    for (auto i:Loc.HPFan) //遍历所有翻数，缺省从小到大
        for (auto j:i.CardType){ //遍历本翻数允许的所有的牌型
            if ((CheckS&(!Patts.Base[j].SType))| //只查特殊牌型
                ((!CheckS)&Patts.Base[j].SType)) //只查基本牌型
                continue;
            
            if (Patts.Base[j].Check(HPJG.CardTot,HPJG.Kinds)){ //本牌型检查通过，加入符合牌型序号结果
                HPJG.Fan=i.FanS;
                HPJG.Kind=j;
                HPJG.Kinds.emplace_back(j);
                R=true;
            }
        }
    if (R) {
        HPJG.CardPX.SetName("胡牌结果#"+ to_string(HPData.size())); //把本次检查的结果，加入结果数据
        HPData.emplace_back(HPJG);
    }else SetErrMsg("没找到牌型。");
    return R;
}
