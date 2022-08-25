#include "STC15F2K60S2.H"
#include <stdlib.h>
#include <intrins.h> 
#define uint unsigned int
#define uchar unsigned char
#define ulong unsigned long
#define cstAdcPower 0X80     /*ADC电源开关*/
#define cstAdcFlag 0X10      /*当A/D转换完成后，cstAdcFlag要软件清零*/
#define cstAdcStart 0X08     /*当A/D转换完成后，cstAdcStart会自动清零，所以要开始下一次转换，则需要置位*/
#define cstAdcSpeed90 0X60   /*ADC转换速度 90个时钟周期转换一次*/
#define cstAdcChs17 0X07     /*选择P1.7作为A/D输入*/


sbit sbtVib = P2 ^ 4;     //振动传感器
sbit sbtLedSel = P2 ^ 3;  //数码管与LED灯切换引脚
sbit sbtKey1 = P3 ^ 2;
sbit sbtKey2 = P3 ^ 3;
bit btKey3Flag;             /*key3键按下标志*/
sbit P3_5 = P3^5;						//红外线发送引脚
sbit P3_7 = P3^7;						//串口1发送引脚
uint i=0,j=0,flyCount=0,boomCount=0,turnFlag=1,findFlag,Temp,startFlag,endFlag,changFlag,HUIZHI;
uchar duanxuan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; //0-9
uchar weixuan[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};
uchar HelloSeg[]={0x00,0x00,0x76,0x79,0x38,0x38,0x3f,0x00};
uchar WinSeg[]={0x00,0x23,0x1c,0x23,0x00};
uchar LoseSeg[]={0x00,0x23,0x54,0x23,0x00};
uchar PingSeg[]={0x00,0x23,0x08,0x23,0x00};
uint arrLed[] = {0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff};
uint currLed[] = {0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe}; //当前LED为0，其余为1
uint uiLed = 0x00, flicker = 0x00,LEDP=0x00;
uint numCount=0,pageCount=3,currPage=0,currNum=0,ledCount=0,segCount=0;
uint count[]={0,0,0,0,0,0,0,0,0,0};
uint num[64];
uchar ucNavKeyCurrent;  //导航按键当前的状态
uchar ucNavKeyPast;     //导航按键前一个状态
uint mybuf,X,DispalyFlag;
uint begin_recive = 0,s_count=0;						//接收开始标志
uint RIbuf[500],RICount=0;

uint sum() {
	uint currSum=0;
	for(i=0; i<numCount; i++)
		currSum+=num[i];
	return currSum;
}

void DelCurr() {
		count[num[currNum]]--;
		for(i=currNum; i<numCount; i++)
				num[i]=num[i+1];
		numCount--;
		pageCount=numCount/8+((numCount%8!=0)?1:0);		
	if(currNum>=numCount) currNum=numCount-1;
}

void DelI() {
	count[num[i]]--;
	for(j=i; j<numCount; j++)
		num[j]=num[j+1];
	numCount--;
	pageCount=numCount/8+((numCount%8!=0)?1:0);
}

unsigned char GetADC() {
    uchar ucAdcRes;
    ADC_CONTR = cstAdcPower | cstAdcStart | cstAdcSpeed90 | cstAdcChs17;//没有将cstAdcFlag置1，用于判断A/D是否结束
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    while( !( ADC_CONTR & cstAdcFlag ) ); //等待直到A/D转换结束
    ADC_CONTR &= ~cstAdcFlag;           //cstAdcFlagE软件清0
    ucAdcRes = ADC_RES;                 //获取AD的值
    return ucAdcRes;
}

void getRandOne() {
		for(i=numCount; i>0; i--)
			num[i]=num[i-1];
		num[0]=(uint)rand()%9+1;
		while(count[num[0]]==4)
				num[0]=(uint)rand()%9+1;
		count[num[0]]++;
		numCount++;
		pageCount=numCount/8+((numCount%8!=0)?1:0);
}

void Delay5ms() {
    unsigned char i, j;
    i = 54;
    j = 199;
    do {
        while ( --j );
    }
    while ( --i );
}

void Delay200ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 9;
	j = 104;
	k = 139;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

uchar NavKeyCheck() {
    unsigned char key;
    key = GetADC();     //获取AD的值
    if( key != 255 )    //有按键按下时
    {
        Delay5ms();
        key = GetADC();
        if( key != 255 )            //按键消抖 仍有按键按下
        {
            key = key & 0xE0;       //获取高3位，其他位清零
            key = _cror_( key, 5 ); //循环右移5位 获取A/D转换高三位值，减小误差
            return key;
        }
    }
    return 0x07;        //没有按键按下时返回值0x07
}

uint shunzi() {
		if(num[currNum]>5)
			return 0;
		if(count[num[currNum]]>0&&count[num[currNum]+1]>0&&count[num[currNum]+2]>0
			&&count[num[currNum]+3]>0&&count[num[currNum]+4]>0)
			return 1;
		return 0;
}

uint feiji() {
		if(num[currNum]>8)
			return 0;
		if(count[num[currNum]]>2&&count[num[currNum]+1]>2)
			return 1;
		return 0;
}

uint boom() {
		if(count[num[currNum]]>=4)
			return 1;
		return 0;
}

void Delay(int n)	{
		while(n--);
}

void sort() {
		for(i=0; i<numCount; i++) {
				for(j=i+1; j<numCount; j++) {
						if(num[i]>num[j]) {
								Temp=num[i];
								num[i]=num[j];
								num[j]=Temp;
						}
				}
		}
}

void Vib() {
		sbtVib=1;
		Delay(40);
		if(sbtVib==0) {
				sort();
		}	
}

void UseShunzi() {
	Temp=num[currNum]+rand()%5;	
	REN = 0;	
	X=Temp;
	SBUF = 0xca;//发出发送标志0xca，触发串口中断进行数据发送
	
	Temp=num[currNum];
	DelCurr();
	for(Temp=Temp+1; Temp<=9; Temp++) {
		findFlag=0;
		for(i=0; i<numCount; i++) {
			if(num[i]==Temp){
				DelI();
				findFlag=1;
				break;
			}
		}
		if(!findFlag) break;
	}
}

void UseFeiji() {
	Temp=num[currNum];
	for(; Temp<=9; Temp++) {
		if(count[Temp]<3) break;
		findFlag=3;
		for(i=0; i<numCount; i++) {
			if(findFlag==0) break;
			if(num[i]==Temp) {
				DelI();
				i--;
				findFlag--;
			}
		}
	}
}

void UseBoom() {
	Temp=num[currNum];
	count[Temp]=0;
	DelCurr();
	for(i=0; i<numCount; i++)
		if(num[i]==Temp) {
			DelI();
			i--;
		}
}

void AddOne() {
		count[num[currNum]]--;
		if(num[currNum]==9) {
			num[currNum]=1;
		} else {
			num[currNum]++;
		}
		count[num[currNum]]++;
}

void KeyTest() {//三按键检测、特殊功能牌使用
		if(btKey3Flag&&shunzi()&&flyCount==0&&boomCount==0) {
				UseShunzi();
				turnFlag=0;
				changFlag=1;
				return;
		} 
		else if(num[currNum]!=0&&sbtKey2==0&&flyCount>0) {
				REN = 0;	
				X=num[currNum];
				DelCurr();
				SBUF = 0xca;//发出发送标志0xca，触发串口中断进行数据发送
				flyCount--;
				turnFlag--;
				changFlag=1;
				Delay200ms();
				return;
		} 
		else if(sbtKey2==0&&flyCount==0&&feiji()) {
				UseFeiji();
				turnFlag=2;
				changFlag=0;
				flyCount=2;
				Delay200ms();
				return;
		} 
		else if(num[currNum]!=0&&sbtKey1==0&&boomCount>0) {
				DelCurr();
				boomCount--;
				turnFlag--;
				changFlag=1;
				Delay200ms();
				return;
		} 
		else if(sbtKey1==0&&boomCount==0&&boom()) {
				UseBoom();
				turnFlag=2;
				changFlag=0;
				boomCount=2;
				Delay200ms();
				return;
		}
}

void NavKey_Process() {
    ucNavKeyCurrent=NavKeyCheck();    //获取当前ADC值
    if(ucNavKeyCurrent!=0x07) {      /*导航按键是否被按下 不等于0x07表示有按下*/
        ucNavKeyPast=ucNavKeyCurrent;
        while(ucNavKeyCurrent!=0x07)        //等待导航按键松开
            ucNavKeyCurrent=NavKeyCheck();
        switch(ucNavKeyPast) {
						case 0x00 :                     //K3
                btKey3Flag=1;
                break;
            case 0x04:                     //左
                if(currNum>0) {
										currNum--;
										currPage=currNum/8;
								} else {
										currNum=numCount-1;
										currPage=currNum/8;
								}
                break;
            case 0x01 :                     //右
                if(currNum<numCount-1) {
										currNum++;
										currPage=currNum/8;
								} else {
										currNum=0;
										currPage=0;
								}
                break;
						case 0x05:                     //上
                if(num[currNum]!=0/*&&turnFlag*/&&boomCount==0&&flyCount==0) {
										REN = 0;	
										X=num[currNum];
										DelCurr();
										SBUF = 0xca;//发出发送标志0xca，触发串口中断进行数据发送
										//turnFlag=0;
										changFlag=1;
										Delay200ms();
								}
                break;
            case 0x02 :                     //下
								if(/*turnFlag&&*/boomCount==0&&flyCount==0) {
										getRandOne();
										//turnFlag=0;
										changFlag=1;
								}
                break;
						case 0x03 :                       //里
								if(/*turnFlag&&*/boomCount==0&&flyCount==0) {
										AddOne();
										//turnFlag=0;
										changFlag=1;
								}
								break;
        }
    }
}

void Hello() {
		sbtLedSel = 0;
		for(j=0; j<400; j++)
			for(i=0; i<8; i++) {
					P0=0;
					P2=weixuan[i];
					P0=HelloSeg[i];
					Delay(500);
			}
		Delay(1000);
}

void showWin() {
		Temp=sum();
		sbtLedSel = 0;
		for(i=0; i<5; i++) {
			P0=0;
			P2=weixuan[i];
			P0=WinSeg[i];
			Delay(500);
		}
		P0=0;
		P2=weixuan[5];
		P0=duanxuan[(Temp/100)%10];
		Delay(500);
		P0=0;
		P2=weixuan[6];
		P0=duanxuan[(Temp/10)%10];
		Delay(500);
		P0=0;
		P2=weixuan[7];
		P0=duanxuan[Temp%10];
		Delay(500);
}

void showLose() {
		Temp=sum();
		sbtLedSel = 0;
		for(i=0; i<5; i++) {
			P0=0;
			P2=weixuan[i];
			P0=LoseSeg[i];
			Delay(500);
		}
		P0=0;
		P2=weixuan[5];
		P0=duanxuan[(Temp/100)%10];
		Delay(500);
		P0=0;
		P2=weixuan[6];
		P0=duanxuan[(Temp/10)%10];
		Delay(500);
		P0=0;
		P2=weixuan[7];
		P0=duanxuan[Temp%10];
		Delay(500);
}

//void showPing() {
//		Temp=sum();
//		sbtLedSel = 0;
//		for(i=0; i<5; i++) {
//			P0=0;
//			P2=weixuan[i];
//			P0=PingSeg[i];
//			Delay(500);
//		}
//		P0=0;
//		P2=weixuan[5];
//		P0=duanxuan[(Temp/100)%10];
//		Delay(500);
//		P0=0;
//		P2=weixuan[6];
//		P0=duanxuan[(Temp/10)%10];
//		Delay(500);
//		P0=0;
//		P2=weixuan[7];
//		P0=duanxuan[Temp%10];
//		Delay(500);
//}

void Display() {//数码管、LED的显示
		sbtLedSel = 0;
		for(i=0; i<8&&8*currPage+i<numCount; i++) {
      P0 = 0;
      P2 = weixuan[i];//选择数码管的位数
			if(i==currNum%8) {
				if(segCount<60) {
						P0 = 0;
						segCount++;
				} else if(segCount<120) {
						P0 = duanxuan[num[8*currPage+i]];
						segCount++;
				} else {
						segCount=0;
				}
			} else {
				P0 = duanxuan[num[8*currPage+i]];  //显示对应的数值
			}
			Delay(400);
    }
		P0=LEDP;
		sbtLedSel = 1;
		if(ledCount<60) {
			LEDP=uiLed&flicker|turnFlag;
			ledCount++;
		} else if(ledCount<120) {
			LEDP=uiLed|turnFlag;
			ledCount++;
		} else {
			LEDP=uiLed&flicker|turnFlag;
			ledCount=0;
		}
		Delay(200);
		uiLed=arrLed[pageCount];
		flicker=currLed[currPage];
		P0=0;
}

void Init() {
		AUXR |= 0x40;		//定时器T1为1T模式，速度是传统8051的12倍，不分频。
		TMOD &= 0x0F;		//清除T1模式位
		TMOD |= 0x20;		//设置T1模式位，使用8位自动重装模式
		TL1 = 0x70;			//设置初值
		TH1 = 0x70;			//设置T1重装值
		TR1 = 1;				//T1运行控制位置1，允许T1计数
		
		AUXR |= 0x80;		//定时器T0为1T模式，的速度是传统8051的12倍，不分频。
		TMOD &= 0xF0;		//清除T0模式位
		TMOD |= 0x02;		//设置T0模式位，使用8位自动重装模式
		TL0 = 0x70;			//设置初值
		TH0 = 0x70;			//设T0重装值
		TF0 = 0;				//T0溢出标志位清零
		TR0 = 1;				//T0运行控制位置1，允许T0计数
		
		//定时器T2用于显示和按键消抖，500us定时16位自动重装
		AUXR |= 0x04;		//定时器T2为1T模式
		T2L = 0x66;			//低位重装值
		T2H = 0xEA;			//高位重装值
		AUXR |= 0x10;		//定时器2开始计时
	
		PCON &= 0x7F;		//波特率不倍速，SMOD=0
		SCON = 0x50;		//串口1使用工作方式1，REN=1(允许串行接收)
		AUXR &= 0xFE;		//串口1选择定时器T1作为波特率发生器，S1ST2=0
		AUXR1 = 0x40;		//串口1在P3.6接收，在P3.7发送
		PS = 1;					//设置串口中断为最高优先级
	
		P0M1 = 0x00;
    P0M0 = 0xff;
    P2M1 = 0x00;
    P2M0 = 0xff;
		//P2M0 = 0x08;
	
		sbtLedSel = 0;      //选择数码管作为输出
    P1ASF = 0x80;       //P1.7作为模拟功能A/D使用
    ADC_RES = 0;        //转换结果清零
    ADC_CONTR = 0x8F;   //cstAdcPower = 1
    CLK_DIV = 0X00;     //ADRJ = 0    ADC_RES存放高八位结果
		
		ET1 = 0;			//禁止T1中断
		ET0 = 1;			//打开定时器T0中断
		ES = 1;				//打开串口1中断
		IE2 = 0X04;		//打开定时器2中断
		
	  IT0 = 0;            //设置IT0上升沿触发
    IT1 = 0;
    EA = 1;             //CPU开放中断
		LEDP=0x00;
		btKey3Flag=0;
		startFlag=1;
		endFlag=0;
		turnFlag=1;
		DispalyFlag=1;
		changFlag=1;
		HUIZHI=0;
}

void Time0() interrupt 1
{
	if(P3_7==0)			//P3_5根据P3_7的信号产生脉冲
	{
		P3_5 = ~P3_5;
	}
	else						//如果P3.7=1则P3.5输出0
		P3_5 = 0;
}


/********************************
 * 函数名：URAT1
 * 描述  ：串口1中断的操作。发送完毕TI自动置1，产生中断; 接收完毕RI值1，产生中断
 * 输入  ：无
 * 输出  ：无
********************************/
void URAT1() interrupt 4
{
	if(TI)												//判断发送中断
	{	
		TI = 0;											//发送中断请求标志位清0
		if(s_count < 1)					//发送个数小于显示个数
		{
			SBUF=X;	//继续发送
			s_count ++;
		}
		else
		{
			if(s_count == 1)			//发送个数等于显示个数
			{	
				s_count ++;
				SBUF = 0x55;						//发送结束标志
			}
			else											//发送完毕后，已发送个数清零，打开串口接收
			{
				s_count = 0;
				REN = 1;									
			}
		}
	}	
	if(RI)												//判断是否接收中断
	{
		RI = 0;											//接收中断请求标志位清0
		mybuf = SBUF;								//把这次接收到的数据存入自定义的缓存中
		if(mybuf == 0x55)						//判断接收结束
		{
			begin_recive = 0;					//接收结束
			IE2 = 0x04;								//打开定时2中断
		}
		if(begin_recive)						//如果开始了
		{	
			RIbuf[RICount] = SBUF;		//接收数据
			RICount++;								//显示个数+1
			}
		}
		if(mybuf == 0xca)						//判断开始接收标志
		{
			begin_recive = 1;					//接收开始
			IE2 = 0x00;								//关闭定时器T2中断，停止按键检测与数码管扫描
			P0 = 0;							
			RICount = 0;								//显示个数清零
		}
}

void Deal() {//发牌
		for(i=0; i<20; i++) {
			num[i]=(uint)rand()%9+1;
			if(count[num[i]]==3)
					num[i]=(uint)rand()%9+1;
			while(count[num[i]]==4)
					num[i]=(uint)rand()%9+1;
			count[num[i]]++;
		}
		numCount=20;
		pageCount=3;
		currPage=0;
		currNum=0;
}

void RICheck() {
			if(RICount==0) return;
			while(RICount>0) {
			if(RIbuf[0]==0x55||RIbuf[0]==0xca) {
			}
			else if(!startFlag&&RIbuf[0]>=10) {//游戏结束
					Temp=sum()+10;
					REN=0;	
					X=Temp;
					SBUF = 0xca;//发出发送标志0xca，触发串口中断进行数据发送
				//}
				while(1) {showLose();}
			} else if(RIbuf[0]>0&&RIbuf[0]<10){
					for(i=numCount; i>0; i--)
						num[i]=num[i-1];
						num[0]=RIbuf[0];
						count[RIbuf[0]]++;
						numCount++;
						pageCount=numCount/8+((numCount%8!=0)?1:0);	
				}
			for(j=0; j<RICount-1; j++)
				RIbuf[j]=RIbuf[j+1];
			RICount--;	
		}
}

void Timer2() interrupt 12
{
	Vib();
	RICheck();
}

void main() {
		Init();
		Deal();
		Hello();
		while(1) {
			if(turnFlag==0) turnFlag=1;
			startFlag=0;
			Vib();
			sbtVib=1;
			Display();
			P0 = 0;				//P0清零
			Vib();
			NavKey_Process();
			Vib();
			KeyTest();
			Vib();
			btKey3Flag=0;
			if(numCount==0) {
					REN = 0;	
					X=10;
					SBUF = 0xca;//发出发送标志0xca，触发串口中断进行数据发送			
					while(1) showWin();
			}
			Vib();
		}
}