//
//  MJTools.cpp
//  MJ
//
//  Created by Bob Yin on 2022/7/1.
//

#include <iostream>       // std::cout, std::hex
#include <string>         // std::string, std::u32string
#include <locale>         // std::wstring_convert
#include <codecvt>        // std::codecvt_utf8
#include <cstdint>        // std::uint_least32_t

using namespace std;

#include "MJTools.hpp"
using namespace MJClassTools;

void MJ_C_CardsGroup::Show(){
    vector<pair <int,int>> CS;
    vector<string> c(MJ_E_HS_End*10,"  ");
    string s;
    
    if (IDs.size()==0) return; //没有就不画
    GetPX(CS);
    cout << "----【" << Name << "】----" ;
    cout << endl << "  |1.2.3.4.5.6.7.8.9 |" << endl;
    for (auto n:CS) {
        switch (n.first/100){
            case MJ_E_PX_Dui:
                s="D";
                break;
            case MJ_E_PX_AKe:
                s="k";
                break;
            case MJ_E_PX_MKe:
                s="K";
                break;
            case MJ_E_PX_AGang:
                s="g";
                break;
            case MJ_E_PX_MGang:
                s="G";
                break;
            case MJ_E_PX_AShun:
                s="s";
                break;
            case MJ_E_PX_MShun:
                s="S";
                break;
            default:
                s=" ";
                break;
        }
        c[n.first%100]=to_string(n.second)+s;
    }
    for (int i=0; i<MJ_E_HS_Hua; i++){
        cout << MJC.getHuaSeHZ(i) << '|';
        for (int j=0; j<9; j++)
            cout << c[i*10+j];
        cout << '|' << endl;
    }
}


int MJ_C_LineAct::Check2Line(int A, int B, bool addv){ //返回两个点的线型
    //x联立数(2.3.4)，
    //y线型(0：点，1:竖线，数字相同，2:横线，花色相同，3:斜线
    //z数字间隔
    int R=0,Y,Z=abs(A-B);
    int T[2]={min(A,B)%100,max(A,B)%100};
    
    if (Z==0) R=20000+T[0]; //点
    else {
        if (T[1]/10==MJ_E_HS_Feng) return 0; //风向只记录点
        
        Z=abs(T[1]-T[0]); //数字差
        if (A==B) Y=0; //点
        else {
            Y=3; //缺省斜线
            if (T[0]%10==T[1]%10) Y=1;//数字相同
            if (T[0]/10==T[1]/10) Y=2;//花色相同
        }
        R=(200+Y*10+Z)*100+T[0];
    }
    if (addv) LineT.insert(R);
    return R;
}

int MJ_C_LineAct::Check3Line(int A, int B, int C, bool addv){ //返回三个点的线型
    //x联立点数(2.3.4)，
    //y线型(0：点，1:竖线，数字相同，2:横线，花色相同，3:斜线
    //z数字间隔
    int Y,Z,i1,i2,R;
    int T[3]={A%100,B%100,C%100};
    
    Sort3int(T[0], T[1], T[2]);
    if ((T[0]==T[1])&&(T[1]==T[2])) R=30000+T[0]; //点
    else{
        if ((T[0]==T[1])||(T[1]==T[2])) return 0; //两个相同的点+一个外部的点
        //根据斜率公式计算是否是直线
        if (((T[0]%10-T[1]%10)*(T[1]/10-T[2]/10))==((T[1]%10-T[2]%10)*(T[0]/10-T[1]/10))) {
            //是直线
            i1=T[1]%10-T[0]%10;
            i2=T[2]%10-T[1]%10;
            if (i1!=i2) return 0;
            Z=i1; //三点点间隔相同
            Y=3; //缺省斜线
            if (T[0]%10==T[1]%10) Y=1;//数字相同
            if (T[0]/10==T[1]/10) Y=2;//花色相同
        }else return 0;
        if ((Y!=0)&&(T[2]>=MJ_E_HS_Feng*10)) return 0; //风向不统计顺
        R=(300+Y*10+Z)*100+T[0];
    }
    if (addv) LineT.insert(R);
    return R;
}

int MJ_C_LineAct::Check4Line(int A, int B, int C, int D, bool addv) { //返回四个点的线型
    int t1,t2,R,T[4]={A%100,B%100,C%100,D%100};
    
    sort(T,T+4);
    if ((T[0]==T[1])&&(T[2]==T[3])){ //AB是一个点，CD是一个点
        if (T[0]==T[3]) R=400*100+T[0]/100; //一个点 ABCD一个点
        else {
            t1=Check2Line(T[0], T[3], false); //两个点组成的线段
            R=(t1+2000)+T[0]/100;
        }
    }else { //不同的点组成的线段
        t1=Check3Line(T[0], T[1], T[2], false);
        t2=Check3Line(T[1], T[2], T[3], false);
        if ((t1==0)||(t2==0)||((t1/100)!=(t2/100))) return 0; //不是线或者不是连在一起的直线
        R=(400+(t1/100)%100)*100+T[0];
    }
    if (addv)  LineT.insert(R);
    return R;
}

bool MJ_C_LineAct::add1PX(int Card){
    if (enPX[Card/100]) {
        if (PXs.Size()<1){ //不足1个ID,先插入一个
            PXs.Insert(Card);
            return true;
        }
        vector <int> C;
        PXs.GetPX(C);
        for (auto n:C) Check2Line(n, Card); //计算两点线
        if (C.size()>=2){ //计算三点线
            C.emplace_back(C[0]);//打环
            for (int i=1;i<C.size();i++)
                Check3Line(C[i-1],C[i],Card);
        }
        if (C.size()>=3){ //计算四点线
            C.emplace_back(C[1]);//打环
            for (int i=2;i<C.size();i++)
                Check4Line(C[i-2],C[i-1],C[i],Card);
        }
        PXs.Insert(Card);
    }
    return true;
}

void MJ_C_ToTalAct::add1PX(int ID){
    int p=ID/100; //牌型
    int h=(ID%100)/10; //花色
    int s=ID%10; //数字
//int c=ID%100; //牌ID

    CT->PaiXing[p]++; //牌型计算
    switch (p) { //牌型区分
        case MJ_E_PX_NULL: //非牌型牌，只有一张牌
            CT->HuaSe[h]++; //花色张数计算；
        switch (h) { //花色区分
            case MJ_E_HS_Wan:  //万
            case MJ_E_HS_Tong: //筒
            case MJ_E_HS_Tiao: //条
                CT->ShuZi[s]++; //计算数字
                break;
            case MJ_E_HS_Feng: //风
                CT->FengZS[s]++; //每种风的张数++
                break;
            default:
                break;
            }
            break;
        case MJ_E_PX_Dui: //对
            CT->HuaSe[h]+=2; //花色张数计算；
            switch (h) { //花色区分
                case MJ_E_HS_Wan:  //万
                case MJ_E_HS_Tong: //筒
                case MJ_E_HS_Tiao: //条
                    CT->ShuZi[s]+=2; //计算数字
                    break;
                case MJ_E_HS_Feng: //风
                    CT->FengZS[s]+=2; //每种风的张数++
                    break;
                default:
                    break;
            }
            break;
        case MJ_E_PX_MShun: //明顺
        case MJ_E_PX_AShun: //暗顺
            CT->HuaSe[h]+=3; //花色张数计算；
            switch (h) { //花色区分
                case MJ_E_HS_Wan:  //万
                case MJ_E_HS_Tong: //筒
                case MJ_E_HS_Tiao: //条
                    CT->ShuZi[s]++; //计算数字
                    CT->ShuZi[s+1]++;
                    CT->ShuZi[s+2]++;
                    break;
                default:
                    break;
            }
            break;
        case MJ_E_PX_MKe: //明刻
        case MJ_E_PX_AKe: //暗刻
            CT->HuaSe[h]+=3; //花色张数计算；
            switch (h) { //花色区分
                case MJ_E_HS_Wan:  //万
                case MJ_E_HS_Tong: //筒
                case MJ_E_HS_Tiao: //条
                    CT->ShuZi[s]+=3; //计算数字
                    break;
                case MJ_E_HS_Feng: //风
                    CT->FengZS[s]+=3; //每种风的张数++
                    break;
                default:
                    break;
            }
            break;
        case MJ_E_PX_MGang: //明杠
        case MJ_E_PX_AGang: //暗杠
            CT->HuaSe[h]+=4; //花色张数计算；
            switch (h) { //花色区分
                case MJ_E_HS_Wan:  //万
                case MJ_E_HS_Tong: //筒
                case MJ_E_HS_Tiao: //条
                    CT->ShuZi[s]+=4; //计算数字
                    break;
                case MJ_E_HS_Feng: //风
                    CT->FengZS[s]+=4; //每种风的张数++
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
} //增加一个牌型或者牌

void MJ_C_CardToTal::Show(){
    MJConstV2 MJC;
    int i;
    
    cout << "----【统计数据】----" << endl;
    cout << "牌型：|" ;
    for (i=0; i<MJ_E_PX_End; i++)
        if (PaiXing[i]!='0')
            cout << (int)(PaiXing[i]-'0') << '.' << MJC.getPaiXingHZ(i) << '|';
    cout << endl;
    cout << "数字：|" ;
    for (i=0; i<9; i++)
        if (ShuZi[i]!='0')
            cout << (int)(ShuZi[i]-'0') << '.' << MJC.getShuZiHZ(i) << '|';
    cout << endl;
    cout << "花色：|" ;
    for (i=0; i<MJ_E_HS_End-1; i++)
        if (HuaSe[i]!='0')
            cout << (int)(HuaSe[i]-'0') << '.' << MJC.getHuaSeHZ(i) << '|';
    cout << endl;
    if (Card4>0)
        cout << "根数：|" << Card4 << '|' << endl;
    cout << "风向：|" ;
    for (i=0; i<7; i++)
        if (FengZS[i]!='0')
            cout << (int)(FengZS[i]-'0') << '.' << MJC.getCardHZ(MJ_E_HS_Feng*10+i) << '|';
    cout << endl;
    cout << "牌型：|" ;
    for (auto n:PXs)
            cout << MJC.getCardHZ(n) << '.';
    cout << endl;
    if (BaseCardSet.size()>0){
        cout << "牌张：|" ;
        for (auto n:BaseCardSet)
            cout << MJC.getCardHZ(n) << '.';
        cout << endl;
    }
    cout <<  "----【线】----" << endl;
    if (PLine.size()>0){
        cout << "  牌：|";
        for (auto n:PLine)
            if (n>=30000) //牌不显示2个以下的线型
                cout << n/100 << ':' << MJC.getCardHZ(n%100) << '|';
        cout << endl;
    }
    if (SLine.size()>0){
        cout << "  顺：|";
        for (auto n:SLine)
            cout << n/100 << ':' << MJC.getCardHZ(n%100) << '|';
        cout << endl;
    }
    if (KLine.size()>0){
        cout << "  刻：|";
        for (auto n:KLine)
            cout << n/100 << ':' << MJC.getCardHZ(n%100) << '|';
        cout << endl;
    }
    cout << "----【统计数据（Over）】----" << endl;
}

string MJ_C_Tools::Ke2Str(vector<int> IDs){
    //对于给定的IDS，生成所有刻和杠的要求牌型字符串，“,”分割
    string s="";
    
    for (auto n:IDs){
        s+=to_string(MJ_E_PX_MKe*100+n)+",";
        s+=to_string(MJ_E_PX_AKe*100+n)+",";
        s+=to_string(MJ_E_PX_MGang*100+n)+",";
        s+=to_string(MJ_E_PX_AGang*100+n)+",";
    }
    return s;
}

string MJ_C_Tools::Shun2Str(vector<int> IDs){
    //对于给定的ID，生成所有包含ID的顺牌型字符串，","分割
    string s="";
    for (auto n:IDs){
        switch (n%10){
            case 0:
                s+=to_string(MJ_E_PX_MShun*100+n)+",";
                s+=to_string(MJ_E_PX_AShun*100+n)+",";
                break;
            case 1:
                s+=to_string(MJ_E_PX_MShun*100+n-1)+",";
                s+=to_string(MJ_E_PX_AShun*100+n-1)+",";
                s+=to_string(MJ_E_PX_MShun*100+n)+",";
                s+=to_string(MJ_E_PX_AShun*100+n)+",";
                break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                s+=to_string(MJ_E_PX_MShun*100+n-2)+",";
                s+=to_string(MJ_E_PX_AShun*100+n-2)+",";
                s+=to_string(MJ_E_PX_MShun*100+n-1)+",";
                s+=to_string(MJ_E_PX_AShun*100+n-1)+",";
                s+=to_string(MJ_E_PX_MShun*100+n)+",";
                s+=to_string(MJ_E_PX_AShun*100+n)+",";
                break;
            case 7:
                s+=to_string(MJ_E_PX_MShun*100+n-2)+",";
                s+=to_string(MJ_E_PX_AShun*100+n-2)+",";
                s+=to_string(MJ_E_PX_MShun*100+n-1)+",";
                s+=to_string(MJ_E_PX_AShun*100+n-1)+",";
                break;
            case 8:
                s+=to_string(MJ_E_PX_MShun*100+n-2)+",";
                s+=to_string(MJ_E_PX_AShun*100+n-2)+",";
                break;
        }
    }
    return s;
}

/*
string MJ_C_HandTools::TypeCompose(vector<int> IDs, int n){
    //对于给定的牌型列表，生成n个牌型的组合
    return "";
}*/

