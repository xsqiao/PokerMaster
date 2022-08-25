#include "STC15F2K60S2.H"
#include <stdlib.h>
#include <intrins.h> 
#define uint unsigned int
#define uchar unsigned char
#define ulong unsigned long
#define cstAdcPower 0X80     /*ADC��Դ����*/
#define cstAdcFlag 0X10      /*��A/Dת����ɺ�cstAdcFlagҪ�������*/
#define cstAdcStart 0X08     /*��A/Dת����ɺ�cstAdcStart���Զ����㣬����Ҫ��ʼ��һ��ת��������Ҫ��λ*/
#define cstAdcSpeed90 0X60   /*ADCת���ٶ� 90��ʱ������ת��һ��*/
#define cstAdcChs17 0X07     /*ѡ��P1.7��ΪA/D����*/


sbit sbtVib = P2 ^ 4;     //�񶯴�����
sbit sbtLedSel = P2 ^ 3;  //�������LED���л�����
sbit sbtKey1 = P3 ^ 2;
sbit sbtKey2 = P3 ^ 3;
bit btKey3Flag;             /*key3�����±�־*/
sbit P3_5 = P3^5;						//�����߷�������
sbit P3_7 = P3^7;						//����1��������
uint i=0,j=0,flyCount=0,boomCount=0,turnFlag=1,findFlag,Temp,startFlag,endFlag,changFlag,HUIZHI;
uchar duanxuan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; //0-9
uchar weixuan[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};
uchar HelloSeg[]={0x00,0x00,0x76,0x79,0x38,0x38,0x3f,0x00};
uchar WinSeg[]={0x00,0x23,0x1c,0x23,0x00};
uchar LoseSeg[]={0x00,0x23,0x54,0x23,0x00};
uchar PingSeg[]={0x00,0x23,0x08,0x23,0x00};
uint arrLed[] = {0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff};
uint currLed[] = {0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe}; //��ǰLEDΪ0������Ϊ1
uint uiLed = 0x00, flicker = 0x00,LEDP=0x00;
uint numCount=0,pageCount=3,currPage=0,currNum=0,ledCount=0,segCount=0;
uint count[]={0,0,0,0,0,0,0,0,0,0};
uint num[64];
uchar ucNavKeyCurrent;  //����������ǰ��״̬
uchar ucNavKeyPast;     //��������ǰһ��״̬
uint mybuf,X,DispalyFlag;
uint begin_recive = 0,s_count=0;						//���տ�ʼ��־
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
    ADC_CONTR = cstAdcPower | cstAdcStart | cstAdcSpeed90 | cstAdcChs17;//û�н�cstAdcFlag��1�������ж�A/D�Ƿ����
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    while( !( ADC_CONTR & cstAdcFlag ) ); //�ȴ�ֱ��A/Dת������
    ADC_CONTR &= ~cstAdcFlag;           //cstAdcFlagE�����0
    ucAdcRes = ADC_RES;                 //��ȡAD��ֵ
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
    key = GetADC();     //��ȡAD��ֵ
    if( key != 255 )    //�а�������ʱ
    {
        Delay5ms();
        key = GetADC();
        if( key != 255 )            //�������� ���а�������
        {
            key = key & 0xE0;       //��ȡ��3λ������λ����
            key = _cror_( key, 5 ); //ѭ������5λ ��ȡA/Dת������λֵ����С���
            return key;
        }
    }
    return 0x07;        //û�а�������ʱ����ֵ0x07
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
	SBUF = 0xca;//�������ͱ�־0xca�����������жϽ������ݷ���
	
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

void KeyTest() {//��������⡢���⹦����ʹ��
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
				SBUF = 0xca;//�������ͱ�־0xca�����������жϽ������ݷ���
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
    ucNavKeyCurrent=NavKeyCheck();    //��ȡ��ǰADCֵ
    if(ucNavKeyCurrent!=0x07) {      /*���������Ƿ񱻰��� ������0x07��ʾ�а���*/
        ucNavKeyPast=ucNavKeyCurrent;
        while(ucNavKeyCurrent!=0x07)        //�ȴ����������ɿ�
            ucNavKeyCurrent=NavKeyCheck();
        switch(ucNavKeyPast) {
						case 0x00 :                     //K3
                btKey3Flag=1;
                break;
            case 0x04:                     //��
                if(currNum>0) {
										currNum--;
										currPage=currNum/8;
								} else {
										currNum=numCount-1;
										currPage=currNum/8;
								}
                break;
            case 0x01 :                     //��
                if(currNum<numCount-1) {
										currNum++;
										currPage=currNum/8;
								} else {
										currNum=0;
										currPage=0;
								}
                break;
						case 0x05:                     //��
                if(num[currNum]!=0/*&&turnFlag*/&&boomCount==0&&flyCount==0) {
										REN = 0;	
										X=num[currNum];
										DelCurr();
										SBUF = 0xca;//�������ͱ�־0xca�����������жϽ������ݷ���
										//turnFlag=0;
										changFlag=1;
										Delay200ms();
								}
                break;
            case 0x02 :                     //��
								if(/*turnFlag&&*/boomCount==0&&flyCount==0) {
										getRandOne();
										//turnFlag=0;
										changFlag=1;
								}
                break;
						case 0x03 :                       //��
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

void Display() {//����ܡ�LED����ʾ
		sbtLedSel = 0;
		for(i=0; i<8&&8*currPage+i<numCount; i++) {
      P0 = 0;
      P2 = weixuan[i];//ѡ������ܵ�λ��
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
				P0 = duanxuan[num[8*currPage+i]];  //��ʾ��Ӧ����ֵ
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
		AUXR |= 0x40;		//��ʱ��T1Ϊ1Tģʽ���ٶ��Ǵ�ͳ8051��12��������Ƶ��
		TMOD &= 0x0F;		//���T1ģʽλ
		TMOD |= 0x20;		//����T1ģʽλ��ʹ��8λ�Զ���װģʽ
		TL1 = 0x70;			//���ó�ֵ
		TH1 = 0x70;			//����T1��װֵ
		TR1 = 1;				//T1���п���λ��1������T1����
		
		AUXR |= 0x80;		//��ʱ��T0Ϊ1Tģʽ�����ٶ��Ǵ�ͳ8051��12��������Ƶ��
		TMOD &= 0xF0;		//���T0ģʽλ
		TMOD |= 0x02;		//����T0ģʽλ��ʹ��8λ�Զ���װģʽ
		TL0 = 0x70;			//���ó�ֵ
		TH0 = 0x70;			//��T0��װֵ
		TF0 = 0;				//T0�����־λ����
		TR0 = 1;				//T0���п���λ��1������T0����
		
		//��ʱ��T2������ʾ�Ͱ���������500us��ʱ16λ�Զ���װ
		AUXR |= 0x04;		//��ʱ��T2Ϊ1Tģʽ
		T2L = 0x66;			//��λ��װֵ
		T2H = 0xEA;			//��λ��װֵ
		AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	
		PCON &= 0x7F;		//�����ʲ����٣�SMOD=0
		SCON = 0x50;		//����1ʹ�ù�����ʽ1��REN=1(�����н���)
		AUXR &= 0xFE;		//����1ѡ��ʱ��T1��Ϊ�����ʷ�������S1ST2=0
		AUXR1 = 0x40;		//����1��P3.6���գ���P3.7����
		PS = 1;					//���ô����ж�Ϊ������ȼ�
	
		P0M1 = 0x00;
    P0M0 = 0xff;
    P2M1 = 0x00;
    P2M0 = 0xff;
		//P2M0 = 0x08;
	
		sbtLedSel = 0;      //ѡ���������Ϊ���
    P1ASF = 0x80;       //P1.7��Ϊģ�⹦��A/Dʹ��
    ADC_RES = 0;        //ת���������
    ADC_CONTR = 0x8F;   //cstAdcPower = 1
    CLK_DIV = 0X00;     //ADRJ = 0    ADC_RES��Ÿ߰�λ���
		
		ET1 = 0;			//��ֹT1�ж�
		ET0 = 1;			//�򿪶�ʱ��T0�ж�
		ES = 1;				//�򿪴���1�ж�
		IE2 = 0X04;		//�򿪶�ʱ��2�ж�
		
	  IT0 = 0;            //����IT0�����ش���
    IT1 = 0;
    EA = 1;             //CPU�����ж�
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
	if(P3_7==0)			//P3_5����P3_7���źŲ�������
	{
		P3_5 = ~P3_5;
	}
	else						//���P3.7=1��P3.5���0
		P3_5 = 0;
}


/********************************
 * ��������URAT1
 * ����  ������1�жϵĲ������������TI�Զ���1�������ж�; �������RIֵ1�������ж�
 * ����  ����
 * ���  ����
********************************/
void URAT1() interrupt 4
{
	if(TI)												//�жϷ����ж�
	{	
		TI = 0;											//�����ж������־λ��0
		if(s_count < 1)					//���͸���С����ʾ����
		{
			SBUF=X;	//��������
			s_count ++;
		}
		else
		{
			if(s_count == 1)			//���͸���������ʾ����
			{	
				s_count ++;
				SBUF = 0x55;						//���ͽ�����־
			}
			else											//������Ϻ��ѷ��͸������㣬�򿪴��ڽ���
			{
				s_count = 0;
				REN = 1;									
			}
		}
	}	
	if(RI)												//�ж��Ƿ�����ж�
	{
		RI = 0;											//�����ж������־λ��0
		mybuf = SBUF;								//����ν��յ������ݴ����Զ���Ļ�����
		if(mybuf == 0x55)						//�жϽ��ս���
		{
			begin_recive = 0;					//���ս���
			IE2 = 0x04;								//�򿪶�ʱ2�ж�
		}
		if(begin_recive)						//�����ʼ��
		{	
			RIbuf[RICount] = SBUF;		//��������
			RICount++;								//��ʾ����+1
			}
		}
		if(mybuf == 0xca)						//�жϿ�ʼ���ձ�־
		{
			begin_recive = 1;					//���տ�ʼ
			IE2 = 0x00;								//�رն�ʱ��T2�жϣ�ֹͣ��������������ɨ��
			P0 = 0;							
			RICount = 0;								//��ʾ��������
		}
}

void Deal() {//����
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
			else if(!startFlag&&RIbuf[0]>=10) {//��Ϸ����
					Temp=sum()+10;
					REN=0;	
					X=Temp;
					SBUF = 0xca;//�������ͱ�־0xca�����������жϽ������ݷ���
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
			P0 = 0;				//P0����
			Vib();
			NavKey_Process();
			Vib();
			KeyTest();
			Vib();
			btKey3Flag=0;
			if(numCount==0) {
					REN = 0;	
					X=10;
					SBUF = 0xca;//�������ͱ�־0xca�����������жϽ������ݷ���			
					while(1) showWin();
			}
			Vib();
		}
}