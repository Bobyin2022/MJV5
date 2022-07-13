//
//  MJClassCoder.cpp
//
//
//  Created by Bob Yin on 2022/7/1.
//

#include <iostream>       // std::cout, std::hex
#include <string>         // std::string, std::u32string
#include <locale>         // std::wstring_convert
#include <codecvt>        // std::codecvt_utf8
#include <cstdint>        // std::uint_least32_t

using namespace std;

#include "MJCoder.hpp"
using namespace MJClassTools;
using namespace MJClassCoder;

void MJ_C_CoderTools::TestTimeDemo(){
/*
 测试程序执行时间的Demo
 */
    double sum = 0;
    double add = 1;

    // Start measuring time
    //auto begin = std::chrono::high_resolution_clock::now();
    auto begin=chrono::high_resolution_clock::now();
    
    int iterations = 1000*1000*1000;
    for (int i=0; i<iterations; i++) {
        sum += add;
        add /= 2.0;
    }
    
    // Stop measuring time and calculate the elapsed time
    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    
    printf("Result: %.20f\n", sum);
    
    printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);
}

uint32_t  MJ_C_CoderTools::enCode32(int *N){
/* 给出单花色N-N(下标数字[1,9]，内容张数)，返回编码后的int数字
 编码方案，定长编码 A+B区，共（27+4）=31位，
 0区：1位，标识是否是七对或者风向
 A区：27位
    三位标识一个数字的张数，由低到高牌
    比如：手里牌是1112345678999+8，九莲宝灯牌，
        数字：123456789
        张数：311111123
        以数字为序号，张数3位二进制编码
        数字： 1   2   3   4   5   6   7   8   9
        张数：011 001 001 001 001 001 001 010 011
 B区：4位，本次编码的张数
    比如： 14张，为1100
 例子,手中牌是1112345678999+8筒，结果为
     区域：---------------A------------------- -BB-
     数字： 1   2   3   4   5   6   7   8   9    14
     张数：011 001 001 001 001 001 001 010 011 1100
 */
    uint32_t Code=0; //返回值
    int CardNum=0; //张数
    //A区
    for(int i=0;i<9;i++){
        if ((N[i]<0)||(N[i]>4)) return -1;
        Code<<=3; //左移让出空位
        Code|=N[i]; //低三位置数量
        CardNum+=N[i];
    }
    //B区
    if ((CardNum==0)||(CardNum>14)) return 0; //张数不对
    Code<<=4; //左移4位
    Code|=CardNum; //放入本手张数
    
    return Code;
}

uint32_t MJ_C_CoderTools::encode32(uint32_t Comb, int m){ //根据组合数，返回编码
/*
 组合数为1-9或者1-7的循环编码，
 */
    uint32_t Code=0,T,C1,Num=0; //返回值和mask
    
    while(Comb>0){
        T=Comb&((1<<m)-1); Comb>>=m;
        C1=0;
        for (int i=0;i<m; i++){ //完成本组数据的对位+1
            C1<<=3; C1|=(T&1);  Num+=(T&1); T>>=1;
        }
        Code+=C1;
    }
    Code<<=((9-m)+(9-m)+(9-m));
    Code<<=4;
    Code|=Num;
    
    return Code;
}

bool MJ_C_CoderTools::CheckCode32(uint32_t R){
 // 检查Code编码的有效性
    
    if (getCodeCardNum(R)>14) return false; //溢出
    
    //校验对位溢出
    uint32_t R1=R>>4;
    
    int t3;
    for (int i=0; i<9; i++){
        t3=R1&7;
        if (t3>4) return false;
        R1>>=3;
    }
    return true;
}

bool MJ_C_CoderTools::CodeAdd32(uint32_t &R, uint32_t C1, uint32_t C2){
//Code相加 R=C1+C2,返回是否有效
//  算法：R=C1+C2,然后对位检查是否溢出
  
    R=C1+C2; //直接相加
    
    if (getCodeCardNum(R)>14) return false; //溢出
    
    //校验对位溢出
    uint32_t R1=R>>4;
    C1>>=4; C2>>=4;
    
    int t3,t2,t1;
    for (int i=0; i<9; i++){
        t3=R1&7;t1=C1&7;t2=C2&7;
        if ((t3>4)||(t3!=(t1+t2))) return false;
        R1>>=3;C1>>=3;C2>>=3;
    }
    return true;
} 

bool MJ_C_CoderTools::deCode32(int *CardsNo, uint32_t Code, MJ_E_PaiXing PX){
/* 根据牌型，解码Card为牌ID，返回是否解码成功
 CardsNo: 解码后的牌ID；
 Code: 编码
 PX: 牌型
 例如，解码Code后的N-N表为,假设花色为筒
 No:  0 1 2 3 4 5 6 7 8
 Num: 0 3 3 3 0 0 0 0 0}
 如果解码牌型为：顺，解码后CodeNo: {1,1,1};2筒顺，2筒顺，2筒顺
 如果解码牌型为：刻，解码后CodeNo: {1,2,3};2筒刻，3筒刻，4筒刻
 */
    bool OK=true;
    int J=0,NN[9],len=((PX==MJ_E_PX_AShun)||(PX==MJ_E_PX_MShun))?7:9; //CardsNo下标
    
    deCode32(NN, Code);
    for (int i=0; i<len; i++){
        if (NN[i]>0) {
            //刻、杠减一次
            if ((PX==MJ_E_PX_AKe)||(PX==MJ_E_PX_MKe)){ CardsNo[J++]=i; NN[i]-=3; continue;}
            if ((PX==MJ_E_PX_AGang)||(PX==MJ_E_PX_MGang)){ CardsNo[J++]=i; NN[i]-=4; continue;}
            if (PX==MJ_E_PX_NULL){ CardsNo[J++]=i; NN[i]--; continue;}
            //对、顺减到0
            while (NN[i]>0) {
                CardsNo[J++]=i;
                if ((PX==MJ_E_PX_AShun)||(PX==MJ_E_PX_MShun)){ NN[i]--;NN[i+1]--;NN[i+2]--;}
                if (PX==MJ_E_PX_Dui) NN[i]-=2;
            }
        }
        if (NN[i]<0) OK=false;
    }
    return OK;
}

uint32_t MJ_C_CoderTools::enCode32(int *IDs, int Len){//根据给定的一组牌（一种花色），返回编码后的Code
    int Cards[9]={0,0,0,0,0,0,0,0,0};
    
    for(int i=0;i<Len;i++) Cards[IDs[i]%10]++;
    return enCode32(Cards);
}

uint32_t MJ_C_CoderTools::enCode32(vector<int> &Ids){//根据给定的一组牌（一种花色），返回编码后的Code
    int Cards[9]={0,0,0,0,0,0,0,0,0};
    
    for(auto n:Ids) Cards[n%10]++;
    return enCode32(Cards);
}

void MJ_C_CoderTools::enCodeList32(uint32_t *CodeN,int *Ids, int Len){
    //根据给定的一组牌，返回编码后的数组，以花色为下标
    int Cards[MJ_E_HS_Hua][9]={{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};
    
    for(int i=0;i<Len;i++) Cards[(Ids[i]%100)/10][(Ids[i]%100)%10]++;
    for(int i=0;i<MJ_E_HS_Hua;i++) CodeN[i]=enCode32(Cards[i]);
 //   ShowCode32(CodeN[MJ_E_HS_Feng]); //Code: 920256526
 //   CodeN[MJ_E_HS_Feng]|=(1<<31); //风向高区置1
 //   ShowCode32(CodeN[MJ_E_HS_Feng]); //Code: 3067740174
}


int MJ_C_CoderTools::deCode32(int *N,uint32_t Code){
/* 根据编码规则，对Coder解码为N-N数组(下标数字，内容张数)，
 返回值:
    第0位，标识位风牌
    第1-4位，真实张数
 */
    
    int CardNum=Code&15;
    Code>>=4;
    
    for(int i=8;i>=0;i--){
        N[i]=(Code&7); //取出低区3Bit，7是111
        Code>>=3; //右移3位
    }
    return CardNum;
}

void MJ_C_CoderTools::getCombBit(int n, int m, vector<uint32_t> &ComList){
 /*   C(n,m),从n个数中选取m个的组合，返回以位标识的Comlist, n<=32
 算法：先创建一个Bitset<n>，其下标表示 1 到 n 个数，值为1表示其下标代表的数被选中，为0则没选中。
    1.首先初始化，将数组前n个元素置1，表示第一个组合为前n个数。
    2.然后从左到右扫描数组元素值的“10”组合，找到第一个“10”组合后将其变为“01”组合，同时将其左边的所有“1”全部移动到数组的最左端。
    3.当第一个“1”移动到数组的n-m的位置，即n个“1”全部移动到最右端时，就得到了最后一个组合。
      例如求 5 中选 3 的组合：
      1     1     1     0     0     //1,2,3
      1     1     0     1     0     //1,2,4
      1     0     1     1     0     //1,3,4
      0     1     1     1     0     //2,3,4
      1     1     0     0     1     //1,2,5
      1     0     1     0     1     //1,3,5
      0     1     1     0     1     //2,3,5
      1     0     0     1     1     //1,4,5
      0     1     0     1     1     //2,4,5
      0     0     1     1     1     //3,4,5
  */
    if ((n>(sizeof(uint32_t)*8))||(n<m)||(n<0)||(m<=0)) return;
    
    bitset<sizeof(uint32_t)*8> Bits((1<<m)-1); //低区m个位置1
    //初始化第一个数
   // Bits.reset(); //全部置0
    //for(int i=0;i<m;i++) Bits[i]=1; //低区m个位置1
    ComList.emplace_back((uint32_t)Bits.to_ulong()); //输出一个组合结果
    
    //生成全部组合数
    int Left1=0, nMask,nMaskBase=(1<<n)-1,Left1Mask; //第一个1的下标
    //nmaskbase: C(n,m)的有效区域
    //Left1Mask, 存放左边1的连续值
    bool out1=false; //成功一个数
    while (Left1!=(n-m)){ //结束条件，第一个1的下标为n-m
        Left1=0; //重置left1,
        nMask=nMaskBase;Left1Mask=0;
        out1=false;
        for (int i=0; i<n ;i++) {
            nMask<<=1;
            if (Bits[i]){ //是1
                if (Left1==0) Left1=i; //第一个1，记录下标到Left1中
                if ((!Bits[i+1])&&(i!=(n-1))) { //不是最后一个数，检查是10组合
                    Bits[i]=0;Bits[i+1]=1;//转换为01
                    uint32_t C=Left1Mask|(Bits.to_ulong()&nMask);
                    Bits=C;
                    ComList.emplace_back(C); //输出一个组合结果
                    out1=true;
                }else Left1Mask=((Left1Mask<<1)|1); //设定连续1
            }
            if (out1) break; //成功输出
        }
    }
}

void MJ_C_CoderTools::getCombBit64(int n, int m, vector<uint64_t> &ComList){
 /*   C(n,m),从n个数中选取m个的组合，返回以位标识的Comlist, n<=64
 算法：先创建一个Bitset<n>，其下标表示 1 到 n 个数，值为1表示其下标代表的数被选中，为0则没选中。
    1.首先初始化，将数组前n个元素置1，表示第一个组合为前n个数。
    2.然后从左到右扫描数组元素值的“10”组合，找到第一个“10”组合后将其变为“01”组合，同时将其左边的所有“1”全部移动到数组的最左端。
    3.当第一个“1”移动到数组的n-m的位置，即n个“1”全部移动到最右端时，就得到了最后一个组合。
      例如求 5 中选 3 的组合：
      1     1     1     0     0     //1,2,3
      1     1     0     1     0     //1,2,4
      1     0     1     1     0     //1,3,4
      0     1     1     1     0     //2,3,4
      1     1     0     0     1     //1,2,5
      1     0     1     0     1     //1,3,5
      0     1     1     0     1     //2,3,5
      1     0     0     1     1     //1,4,5
      0     1     0     1     1     //2,4,5
      0     0     1     1     1     //3,4,5
  */
    if ((n>(sizeof(uint64_t)*8))||(n<m)||(n<0)||(m<=0)) return;
    
    bitset<sizeof(uint64_t)*8> Bits;
    //初始化第一个数
    Bits.reset(); //全部置0
    for(int i=0;i<m;i++) Bits[i]=1; //低区m个位置1
    ComList.emplace_back((uint64_t)Bits.to_ulong()); //输出一个组合结果
    
    //生成全部组合数
    int Left1=0, Left1Num; //第一个1的下标, 左边1个数（10组合前1的数量）
    bool out1=false; //成功一个数
    while (Left1!=(n-m)){ //结束条件，第一个1的下标为n-m
        Left1=0;Left1Num=0; //重置left1, left1Num
        out1=false;
        for (int i=0; i<n ;i++) {
            if (Bits[i]){ //是1
                if (Left1==0) Left1=i; //第一个1，记录下标到Left1中
                if ((!Bits[i+1])&&(i!=(n-1))) { //不是最后一个数，检查是10组合
                    Bits[i]=0;Bits[i+1]=1;//转换为01
                    for (int j=0;j<i;j++) //将左边的所有“1”全部移动到数组的最左端,中间填0。
                        Bits[j]=(j<Left1Num)?1:0;
                    ComList.emplace_back((uint64_t)Bits.to_ulong()); //输出一个组合结果
                    out1=true;
                }else Left1Num++; //左边1个数增加
            }
            if (out1) break; //成功输出
        }
    }
}

uint32_t MJ_C_CoderTools::CombBit2Code2(uint32_t Comb, MJ_E_PaiXing PX, MJ_E_HuaSe HS){
/*    //组合数还原为No-Num（NN）数组，返回Code
 Comb: 组合数；
 PX: 牌型；
 HS: 花色；
 m:  C(n,m)中的m, [1,4]：对[1,2]；刻[9]；顺[1,4]
 */
    
    if ((HS==MJ_E_HS_Feng)&&((PX==MJ_E_PX_AShun)||(PX==MJ_E_PX_MShun))) return 0; //风向不还原顺
    
    uint32_t R=encode32(Comb,(HS==MJ_E_HS_Feng)?7:9),N=getCodeCardNum(R),R1;
    
    switch (PX){
        case MJ_E_PX_AShun:  //处理顺
        case MJ_E_PX_MShun:
            R1=encode32(Comb,7)>>4;
            R1=R1+(R1>>3)+(R1>>6);
            R=(R1<<4)|(N+N+N);
            break;
        case MJ_E_PX_Dui:   //处理对
            R+=R;
            break;
        case MJ_E_PX_MKe:   //处理刻
        case MJ_E_PX_AKe:
            R=R+R+R;
            break;
        case MJ_E_PX_AGang: //处理杠
        case MJ_E_PX_MGang:
            R=R+R+R+R;
            break;
        default:
            break;
    } //switch
 
    return R;
}

void MJ_C_CoderTools::showCombBit(vector<uint32_t> &ComList, bool SD, int bl){
    cout << "------CombBit------" << endl;
    cout << "size: " << ComList.size() << endl;
    if (SD)
        for (auto n:ComList){
            cout << "Number: " << n << endl;
            bitset<sizeof(int)*8> Bits(n);
            if (bl==9) cout << "54321987654321987654321987654321" << endl;
            else cout << "00007654321765432176543217654321" << endl;
            cout << Bits << endl;
        }
}

void MJ_C_CoderTools::showCombBit(uint32_t Comb, int bl){
    cout << "Number: " << Comb << endl;
    bitset<sizeof(int)*8> Bits(Comb);
    if (bl==9) cout << "54321987654321987654321987654321" << endl;
    else cout << "00007654321765432176543217654321" << endl;
    cout << Bits << endl;
}

void MJ_C_CoderTools::ShowCode32(uint32_t Code){ //显示编码内容
    int N[9],CardNum;
    
    cout << "------ Coder -----" << endl;
    cout << "Code: " << Code << endl;
    cout << "No.|   1  2  3  4  5  6  7  8  9" << endl;
    cout << "Num|  ";
    
    bitset<32> Bit(Code);
    CardNum=deCode32(N, Code);
    for(int i=0;i<9;i++)
        cout << ' ' << N[i] << ' ';
    cout << ((CardNum>9)?" ":"  ") << CardNum << endl;
    cout << "Bit|" << Bit << endl;
    cout << "------ Coder end -----" << endl;
}

void MJ_C_CoderTools::showValue(int Value){
    MJ_S_CodeHPPX PX;
    getValuePX(Value, PX, MJ_E_HS_Tong);

    cout << Value << '|' << PX.V << '.';
    cout << PX.Ds << '-' << PX.Ks << '-' << PX.Ss << ' ';
    sort(PX.NCID_PXs.begin(),PX.NCID_PXs.end());
    for(auto m:PX.NCID_PXs)
        cout << MJC.getNCID_HZ(m) << '.';
    cout << endl;
}

void MJ_C_OKTable::Init(int zs){
    DataN.clear();
    DataF.clear();
    vector<MJ_C_DKSCombList> Ds;
    
    DKSs.swap(Ds);
    //相同张数的，有顺的先加入，最后加入全对或者刻的牌
    if (zs>=2) { addDKSList(1,0,0); }//1对
    if (zs>=3) { addDKSList(0,0,1); addDKSList(0,1,0); } //1刻、1顺
    if (zs>=4) { addDKSList(2,0,0); }//2对
    if (zs>=5) { addDKSList(1,0,1); addDKSList(1,1,0); } //1对+1刻，1对+1顺
    if (zs>=6) { addDKSList(0,0,2); addDKSList(0,1,1); addDKSList(3,0,0); addDKSList(0,2,0); }//2顺、1刻+1顺、3对、2刻
    if (zs>=8) { addDKSList(1,0,2); addDKSList(1,1,1); addDKSList(4,0,0); addDKSList(1,2,0); }//(2顺、1刻+1顺)+1对,4对、2刻+1对
    if (zs>=9) { addDKSList(0,0,3); addDKSList(0,2,1); addDKSList(0,1,2); addDKSList(0,3,0); }//3顺、2刻+1顺、1刻+2顺、3刻
    if (zs>=10){ addDKSList(5,0,0); }//5对
    if (zs>=11){ addDKSList(1,0,3); addDKSList(1,2,1); addDKSList(1,1,2); addDKSList(1,3,0); }//(3顺、2刻+1顺、1刻+2顺、3刻)+1对
    if (zs>=12){ addDKSList(0,0,4); addDKSList(0,1,3); addDKSList(0,2,2);//4顺、1刻+3顺、2刻+2顺、
                 addDKSList(0,3,1); addDKSList(6,0,0); addDKSList(0,4,0);}//3刻+1顺、6对、4刻、
    if (zs>=14){ addDKSList(1,0,4); addDKSList(1,1,3); addDKSList(1,2,2); //1对+4顺、1对+1刻+3顺、1对+2刻+2顺、
                 addDKSList(1,3,1); addDKSList(7,0,0); addDKSList(1,4,0);}//1对+3刻+1顺、7对、1对+4刻、
    //生成所有的组合数
    CreatDKS();
    appendSpecRec();
}

void MJ_C_OKTable::Show(bool d){
    cout << "--------" << Name << " OK Table-------" << endl;
    cout << "DKS Array:" << endl;
    for (auto D:DKSs) D.Show();
    cout << "OK Table (Normal) Size:" << DataN.size() << endl;
    cout << "OK Table (Feng)   Size:" << DataF.size() << endl;
    if (d) {
        for (auto n:DataN)
            cout << "K-V:" << n.first << '.' << n.second << endl;
        cout << dec << endl;
        for (auto n:DataF)
            cout << "K-V:" << n.first << '.' << n.second << endl;
        cout << dec << endl;
    }
    cout << "--------" << Name << " OK Table-------" << endl;
}

void MJ_C_OKTable::CreatDKS(){
/* 根据DKSs的组合数列表，追加Oktable的牌型
 */
    uint32_t T,F; //牌的Code,最终牌型 F
    int baseLen=0;
    
    for (auto DKS:DKSs){ //DKS循环
        baseLen=(DKS.isFeng)?7:9;
        if (((DKS.getShunN()+DKS.getKeN()+DKS.getDuiN())==0)||
            ((DKS.getShunN()+DKS.getKeN())>(MJ_MaxPXNum))) //刻+顺超标
            continue;//所有DKS数量为0,或者数量超过标准
        
        //只有对
        if ((DKS.getShunN()+DKS.getKeN())==0){
            for(auto D:DKS.DuiCodes) //对CodeList
                append(D, DKS, D, 0, 0); //插入hash表
            continue;
        }
        
        //只有刻
        if ((DKS.getShunN()+DKS.getDuiN())==0){
            for(auto K:DKS.KeCodes) //刻CodeList
                append(K, DKS, 0, K, 0); //插入hash表
            continue;
        }
        
        //只有顺
        if ((DKS.getKeN()+DKS.getDuiN())==0){
            for(auto S:DKS.ShunCodes) //刻CodeList
                append(S, DKS, 0, 0, S); //插入hash表
            continue;
        }
        
        //只有顺、刻
        if (DKS.getDuiN()==0){
            for (auto K:DKS.KeCodes)
                for (auto S:DKS.ShunCodes)
                    if (CT.CodeAdd32(F, K, S))
                        append(F, DKS, 0, K, S);
            continue;
        }
        
        //只有对、刻
        if (DKS.getShunN()==0){
            for (auto D:DKS.DuiCodes)
                for (auto K:DKS.KeCodes)
                    if (CT.CodeAdd32(F, D, K)){
                        append(F, DKS, D, K, 0);
                    }
            continue;
        }
        
        //只有对、顺
        if (DKS.getKeN()==0){
            for (auto D:DKS.DuiCodes)
                for (auto S:DKS.ShunCodes)
                    if (CT.CodeAdd32(F, D, S))
                        append(F, DKS, D, 0, S);
            continue;
        }
        
        //对、刻、顺都有
        for (auto D:DKS.DuiCodes)
            for (auto K:DKS.KeCodes){
                if (CT.CodeAdd32(T, D, K))
                    for (auto S:DKS.ShunCodes)
                        if (CT.CodeAdd32(F, T, S))
                            append(F, DKS, D, K, S);
            }
    }//DKS循环
}

int MJ_C_OKTable::append(int Code, MJ_C_DKSCombList &DKS, uint32_t DC,uint32_t KC, uint32_t SC){
/* Data表中追加Key-Value数据,返回值：-1为错误，其余位正确的value
 Code: Key,7对牌型最高位置1，
 DC,KC,SC: 对、顺、刻的组合数；
 V: 本牌型的value，0：3n牌型，1：2+3n牌型，2：2n(n>1)牌型
 
 Value: A+B+C区组成，从高到底区分，以C区的V决定AB区的分解
 ----------C区，V区--------------
 2位，{0，1，2}的二进制数
 {0，1}：3n+2牌型；
 {2}：7对牌型；
 
 如果V={0，1}，则A+B区一共4+(1+4)*4=24位，分别为
    ----------A区，对区（麻将头）--------------
    4位，标识数字，{0，9}的二进制数据
    ----------B区，刻、顺区--------------
    5位，第1位 0，刻；1：顺
    第2、3、4、5位，{0，9}的二进制数据
    一共5*4=20位；

 如果V={2}，则A+B区一共4*7=28位，分别为
    ----------对数字区（麻将）--------------
    4位，对数字区，{0，9}的二进制数字
    一共4*7=28位
 */
    int Value=0; //设置Value初值
    int V=(DKS.getDuiN()>1)?2:DKS.getDuiN();
    
    if (V==2) { //7对牌型
        int AB=0; //AB区初值
        int DNo[7]={15,15,15,15,15,15,15},j=0; //7个对的缓存数组,9为无效数字
        CT.deCode32(DNo, DC, MJ_E_PX_Dui);
        for(j=0;j<7;j++){ //设置AB区；
            AB<<=4; //左移四位
            if (DNo[j]>0) AB|=DNo[j]; //有数据，加数据
        }
        Value=((AB<<2)|V); //设置最终结果
        Code|=(1<<31); //七对牌型，放入7对区
    }
    
    if (V<2) { //3n+2牌型
        int B=0,j=0,k=0; //AB区初值
        int DNo[1]={15},KNo[4]={15,15,15,15},SNo[4]={15,15,15,15},KSNo[4]={15,15,15,15};
        
        if (V==1)  CT.deCode32(DNo, DC, MJ_E_PX_Dui);//有对
        if (KC!=0) CT.deCode32(KNo, KC, MJ_E_PX_AKe);//有刻
        if (SC!=0) CT.deCode32(SNo, SC, MJ_E_PX_AShun);//有顺
        for(j=0;j<4;j++){ //设置B区；
            if (KNo[j]<15) KSNo[k++]=KNo[j];//有刻，左移5位，高位置0，低四位置数字；
            if (SNo[j]<15) KSNo[k++]=(1<<4)|SNo[j]; //有顺，左移5位，高位置1，低四位置数字；
        }
        for(j=0;j<4;j++){ //设置B区；
            B<<=5;
            B|=KSNo[j];
        }
        Value=(((DNo[0]<<20)|B)<<2|V); //设置最终结果
    }
    if (Code==524289)
        Code=Code;
    
    DataN[Code]=Value; //基础区OKTable
    if (SC==0) DataF[Code]=Value; //顺为0，风表也加入
    if ((V==1)&&(KC==0)&&(SC==0)) {
        DataN[(1<<31|Code)]=Value; //只有1对，7对区也加入；
        DataF[(1<<31|Code)]=Value; //顺为0，风表也加入
    }
    return Value;
}

void MJ_C_OKTable::appendSpecRec(){
    //增加特殊胡牌类型数据
    //全不靠牌型的数字组合
    vector<vector<int>> SpecCardsList={
        {0},{3},{6},{0,3},{0,6},{3,6},{0,3,6}, //1-4-7
        {2},{5},{8},{2,5},{2,8},{5,8},{2,5,8}, //3-6-9
        {1},{4},{7},{1,4},{1,7},{4,7},{1,4,7}, //2-5-8
    };
    vector<int> SpeFeng={0,1,2,3,4,5,6}; //七星不靠的全部风
    //加入全不靠的风向组合
    vector<uint32_t> FengCombList,NorCodeList,FengCodeList;
  //  CT.getCombBit(7, 4, FengCombList); //听牌，风向最少需要13-9=4张
    CT.getCombBit(7, 5, FengCombList); //胡牌，风向最少需要14-9=5张
    CT.getCombBit(7, 6, FengCombList); //胡牌，风向最少需要14-9=5张

    //给牌型编码
    for (auto n:SpecCardsList)
        NorCodeList.emplace_back(CT.enCode32(n));
    //加入OKtable
    for(auto n:NorCodeList){
        int AB=0; //AB区初值
        int CNo[7]={15,15,15,15,15,15,15},j=0; //7张牌的缓存数组,15为无效数字
        CT.deCode32(CNo, n, MJ_E_PX_NULL);
        for(j=0;j<7;j++){ //设置AB区；
            AB<<=4; //左移四位
            if (CNo[j]>0) AB|=CNo[j]; //有数据，加数据
        }
        DataN[n]=((AB<<2)|3); //设置最终结果,插入OKTable
    }
    
    FengCodeList.emplace_back(CT.enCode32(SpeFeng));
    CT.CombList2Codes(FengCombList, FengCodeList, MJ_E_PX_NULL, MJ_E_HS_Feng);
    for(auto n:FengCodeList){
        int AB=0; //AB区初值
        int CNo[7]={15,15,15,15,15,15,15},j=0; //7张牌的缓存数组,15为无效数字
        CT.deCode32(CNo, n, MJ_E_PX_NULL);
        for(j=0;j<7;j++){ //设置AB区；
            AB<<=4; //左移四位
            if (CNo[j]>0) AB|=CNo[j]; //有数据，加数据
        }
        DataF[n]=((AB<<2)|3); //设置最终结果,插入OKTable
    }
}

bool MJ_C_HandCoder::isHu(MJ_C_CoderHPPX &HPD){
/* 计算手中牌是否胡牌
 胡牌判定：
 1、每种花色的enCoder，得到Code[4]，对应筒条万风（MJ_E_HuaSe）
 2、查表获得每种花色的V，
 3、是否胡牌：
    a)所有花色都在7对区找到，7对牌型胡牌
    b)所有花色V相加等于1，3n+2牌型
 */
    int ValueNor[MJ_E_HS_Hua],Value7D[MJ_E_HS_Hua]; //Normal和7D的Value
    MJ_S_CodeHPPX PXNor, PX7D; //Normal和7D的HP数据
    int VNor=0, V7D=0;
    
    //查表
    for(int i=0;i<MJ_E_HS_Hua;i++){
        if (HSCode[i]<2) continue; //本花色没有编码数据
        //查Normal表
        if ((ValueNor[i]=OKTable->Find(HSCode[i],(MJ_E_HuaSe)i))!=-1) {
            CT.getValuePX(ValueNor[i], PXNor, i);
            VNor+=PXNor.V;
        }else VNor+=-99;
        
        //查7d表
        if ((Value7D[i]=OKTable->Find(((1<<31)|HSCode[i]),(MJ_E_HuaSe)i))!=-1) {
            CT.getValuePX(Value7D[i], PX7D, i);
            V7D+=PX7D.V;
        }else V7D+=-99;
    }
    
    if (VNor==1) HPD.HPData.emplace_back(PXNor); //normal胡牌
    if (VNor==12) HPD.HPData.emplace_back(PXNor); //特殊牌
    if (V7D>0){ PX7D.V=2; HPD.HPData.emplace_back(PX7D); } //7D胡牌
    
    return HPD.HPData.size();
}

void MJ_C_HandCoder::Show(){
    MJConstV2 MJC;
    int n[9];
    
    cout << "--------HandCoder Detail---------" << endl;
    for (int i=0; i<MJ_E_HS_Hua; i++){
        cout << MJC.getHuaSeHZ(i) << ":";
        //显示牌和张数，风向跳过数字89
        CT.deCode32(n, HSCode[i]);
        for (int j=0;j<9;j++){
            if (n[j]>0)
                if (!((i==MJ_E_HS_Feng)&&(j>6)))
                    cout << MJC.getCardHZ(i*10+j) << ':' << n[j] << '|';
        }
        cout << endl;
    }
    
    cout << "------HandCoder Detail End-------" << endl;
}

void MJ_C_DKSCombList::Show(){
    cout << "CNum:" << getCardNum() << '|';
    cout << (int) DuiNum << '.' << (int) KeNum  << '.' <<  (int) ShunNum << '|' ;
    cout << "CombNum:" << DuiCodes.size() << '.' << KeCodes.size()  << '.' << ShunCodes.size();
    cout << endl;
}

bool MJ_C_DKSCombList::setDKSNum(int D,int K,int S){
/* 设置对、刻、顺的数量，并确定组合数列表
 要求，D、S、K都要大于零
 1、(K+S)<=MJ_MaxPXNum, 刻和顺的数量不能大于最大牌型数
 2、D<=((K+S)*3+2)/2, 对的数量不能大于最大牌数的一半
 3、风不能计算顺组合数
 */
    if (((D<0)||(K<0)||(S<0))|| //DSK>=0
        ((K+S)>MJ_MaxPXNum)||   //(K+S)<=MJ_MaxPXNum
        (D>((MJ_MaxPXNum)*3+2)>>1)||     //D<=((K+S)*3+2)/2,
        ((S>0)&&isFeng)) //是风，也要算顺
        return false;
    
    vector<uint32_t> DuiCombList,KeCombList,ShunCombList; //对应的组合数列表
    DuiNum=D;KeNum=K;ShunNum=S;
    int baseLen=(isFeng)?7:9; //基础长度设置，风向为7，其余花色为9
    
    //设置对组合数，只选一个对，就从基础长度中选取，两个以上，需要从2倍基础长度中选取，因为可以有4张
    if (D>0) {
        CT.getCombBit(baseLen*((D==1)?1:2), D, DuiCombList);
        CT.CombList2Codes(DuiCombList, DuiCodes, MJ_E_PX_Dui, (isFeng?MJ_E_HS_Feng:MJ_E_HS_Tong));
    }
    
    //设置刻组合数，只需要从baselen中选取
    if (K>0) {
        CT.getCombBit(baseLen, K, KeCombList);
        CT.CombList2Codes(KeCombList, KeCodes, MJ_E_PX_AKe, (isFeng?MJ_E_HS_Feng:MJ_E_HS_Tong));
    }
    
    //设置顺组合数，基础长度置为7（只记录顺的最小数字），几个顺就是几倍基础长度，最大不超过4（4张），因为可以有完全相同的顺
    if (S>0){
        baseLen=7;
        CT.getCombBit(baseLen*((S<=4)?S:4), S, ShunCombList);
        CT.CombList2Codes(ShunCombList, ShunCodes, MJ_E_PX_AShun, (isFeng?MJ_E_HS_Feng:MJ_E_HS_Tong));
    }
    
    return true;
}

bool MJ_C_HandCoder::isTing(set<int> &Ids){
 /*   计算手中牌是否上听，返回是否，如果上听，Ids存放听的牌
  算法：
  1、张数为3n+1；
  2、至少有三个花色查到
  3、对于没查到的，增加1-9，看是否能查到，查到加入结果集
  4、加入{1，9}后都没查到，则不听
  */
    MJConstV2 MJC;
    
    if (CardNum%3!=1) return false; //张数不对
    MJ_E_HuaSe HS7D=MJ_E_HS_End, HSNor=MJ_E_HS_End;
    int t1=0;
    uint8_t HSVBit[MJ_E_HS_Hua]={0,0,0,0}; //三位标识哪种牌型找到
    
    //查找手中牌所有花色的Value；先找7对
    int V7D=0,VNor=0,VSp=0;
    for (int i=0; i<MJ_E_HS_Hua; i++){
        uint32_t Value;
        if (HSCode[i]) {
           // CT.ShowCode32(HSCode[i]);
            Value=OKTable->Find(((1<<31)|HSCode[i]),(MJ_E_HuaSe)i); //7对
            if (Value!=-1) HSVBit[i]|=(1<<1); //7d 1位 找到
            else HS7D=(MJ_E_HuaSe)i;
            Value=OKTable->Find(HSCode[i],(MJ_E_HuaSe)i); //常规
            if (Value!=-1){
                if (CT.getValveV(Value)==3) {HSNor=(MJ_E_HuaSe)i; HSVBit[i]|=(1<<2);} //特殊牌，2位
                else HSVBit[i]|=1; //3n+2，0位，
            }
            else HSNor=(MJ_E_HuaSe)i;
            VSp +=(HSVBit[i]&4);
            V7D +=(HSVBit[i]&2);
            VNor+=(HSVBit[i]&1);
        }else t1++;
    }
    
    bool is7D=(V7D==(3-t1)*2), //只有一个花色没找到
        isNor=(VNor==(3-t1)), //只有一个花色没找到
        isSP=((VSp==16)||(VSp==12)); //一个或者没有花色没找到
    bool Found7D=false, FoundNor=false, FoundSP=false;
    if (CardNum<13) is7D=isSP=false; //有吃出的牌，肯定不是7对和不靠；
    //7对牌型；
    if (is7D) {
        for (int i=0;i<9;i++){
            uint32_t NCode=CT.CodeAdd1No(HSCode[HS7D], i);
            if (NCode>0){
                if (OKTable->Find(((1<<31)|NCode),HS7D)!=-1) {
                    Ids.insert(MJC.getNewCardID(i, HS7D, MJ_E_PX_NULL));
                    Found7D=true;
                }
            }
        }
    }
    if (isNor) {
        for (int i=0;i<9;i++){
            uint32_t NCode=CT.CodeAdd1No(HSCode[HSNor], i),SPV;
            if (NCode>0){
                if ((SPV=OKTable->Find(NCode,HSNor))!=-1) {
    //                CT.ShowCode32(NCode);
   //                 CT.showValue(SPV);
                    if (CT.getValveV(SPV)<2){
                        Ids.insert(MJC.getNewCardID(i, HSNor, MJ_E_PX_NULL));
                        FoundNor=true;
                    }
                }
            }
        }
    }
    if (isSP) {
        for (int j=0; j<MJ_E_HS_Hua; j++){
            for (int i=0;i<9;i++){
                uint32_t NCode=CT.CodeAdd1No(HSCode[j], i),SPV;
                if (NCode>0){
  //                  if (NCode==306783239)
    //                    CT.ShowCode32(NCode);
                    if ((SPV=OKTable->Find(NCode,(MJ_E_HuaSe)j))!=-1) {
                        if (CT.getValveV(SPV)==3){
      //                      CT.ShowCode32(NCode);
        //                    CT.showValue(SPV);
                            Ids.insert(MJC.getNewCardID(i, (MJ_E_HuaSe)j, MJ_E_PX_NULL));
                            FoundSP=true;
                        }
                    }
                }
            }
        }
    }
    
    return Found7D|FoundSP|FoundNor;
}
