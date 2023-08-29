/*
******************************************************************************
* @file     : USB2XXXCANTest.cpp
* @Copyright: usbxyz
* @Revision : ver 1.0
* @Date     : 2014/12/19 9:33
* @brief    : USB2XXX IIC test demo
******************************************************************************
* @attention
*
* Copyright 2009-2014, usbxyz.com
* http://www.usbxyz.com/
* All Rights Reserved
*
******************************************************************************
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <windows.h>
#include "usb_device.h"
#include "usb2can.h"
#include<vector>
#include <random>
#include <time.h>
#include <math.h>


#pragma comment(lib,"Winmm.lib")

#define GET_FIRMWARE_INFO   1
#define CAN_MODE_LOOPBACK   0
#define CAN_SEND_MSG        1
#define CAN_GET_MSG         1
#define CAN_GET_STATUS      0
#define CAN_SCH             0




/*
void PASCAL OneMilliSecondProc(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2)
{
    static CAN_MSG CanMsg;
    CanMsg.ID++;
    memset(&CanMsg, 0, sizeof(CAN_MSG));
    CAN_SendMsg(dwUser, 0, &CanMsg, 1);
}
*/
int DevHandle[10];
int SendCANIndex = 0;//0-CAN1,1-CAN2
int ReadCANIndex = 1;//0-CAN1,1-CAN2
bool state;
int ret;


//void insertCharAtEnd(char arr[], CAN_MSG c) {
//    int len = strlen(arr);
//    arr[len] = c;
//    arr[len + 1] = '\0';
//}
/*
int lightid1[14] = { 0x101, 0x102, 0x103, 0x104, 0x105, 0x106, 0x107, 0x108, 0x109, 0x10A, 0x10B, 0x10C, 0x10D,
0x10E };

int lightid2[14] = { 0x201, 0x202, 0x203, 0x204, 0x205, 0x206, 0x207, 0x208, 0x209, 0x20A, 0x20B, 0x20C, 0x20D,
0x20E };

int lightid4[14] = { 0x101, 0x102, 0x103, 0x104, 0x105, 0x106, 0x107, 0x108, 0x109, 0x10A, 0x10B, 0x10C, 0x10D,
0x10E };
*/

int lightid1[14] = { 0x301, 0x302, 0x303, 0x304, 0x305, 0x306, 0x307, 0x308, 0x309, 0x30A, 0x30B, 0x30C, 0x30D,
0x30E };

int color_1[3] = { 255, 211, 0 }; //# ������ɫ ��ɫ
int color_2[3] = {120, 3, 241}; // # ������ɫ  ��ɫ3

int CAN1 = 0;

void delay(int milliseconds) {
    clock_t start_time = clock();
    while ((clock() - start_time) * 1000 / CLOCKS_PER_SEC < milliseconds) {
        // ��ѭ�����ȴ�ʱ������
    }
}

double round(double num, int decimal_places) {
    double multiplier = pow(10, decimal_places);
    double rounded_value = num * multiplier;
    double rounded_integer = (num >= 0) ? floor(rounded_value + 0.5) : ceil(rounded_value - 0.5);
    return rounded_integer / multiplier;
}

//���
/*
int light_rain_mode1(int sleep_time,char red,char blue,char green ,char bright) {

    char rain_color1[3] = { 240, 250, 254 };
    rain_color1[0] =  red;
    rain_color1[1] = blue;
    rain_color1[2] = green;
    char liangdu = bright;
    liangdu = 0xFE;
    int rain_color2[3] = { 1, 3, 13 };
    std::vector<CAN_MSG> can_list1;
    //char can_list1[200];
    for (int i = 0; i < 20; i++) {
        CAN_MSG CanMsg;
        CanMsg.ExternFlag = 0;
        CanMsg.RemoteFlag = 0;
        CanMsg.DataLen = 8;
        CanMsg.ID = 0x10;
        double p1 = (19 - i) / 19.0;
        double p2 = 1 - p1;
        unsigned char canmsg_data0 []= { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
        canmsg_data0[0] = static_cast<unsigned char>(rain_color1[0] * p1 + rain_color2[0] * p2);
        canmsg_data0[1] = static_cast<unsigned char>(rain_color1[1] * p1 + rain_color2[1] * p2);
        canmsg_data0[2] = static_cast<unsigned char>(rain_color1[2] * p1 + rain_color2[2] * p2);
        canmsg_data0[4] = static_cast<unsigned char>(rain_color1[0] * p1 + rain_color2[0] * p2);
        canmsg_data0[5] = static_cast<unsigned char>(rain_color1[1] * p1 + rain_color2[1] * p2);
        canmsg_data0[6] = static_cast<unsigned char>(rain_color1[2] * p1 + rain_color2[2] * p2);
        memcpy(CanMsg.Data, canmsg_data0, sizeof(unsigned char)*8);
        //CanMsg.Data= canmsg_data0;
        //insertCharAtEnd(can_list1, CanMsg);
        can_list1.push_back(CanMsg);
    }
    CAN_MSG CanMsg1;
    CanMsg1.ExternFlag = 0;
    CanMsg1.RemoteFlag = 0;
    CanMsg1.DataLen = 8;
    CAN_MSG CanMsg2;
    CanMsg2.ExternFlag = 0;
    CanMsg2.RemoteFlag = 0;
    CanMsg2.DataLen = 8;
    unsigned char canmsg_data1 [] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
    canmsg_data1[0] = rain_color2[0];
    canmsg_data1[1] = rain_color2[1];
    canmsg_data1[2] = rain_color2[2];
    canmsg_data1[4] = rain_color2[0];
    canmsg_data1[5] = rain_color2[1];
    canmsg_data1[6] = rain_color2[2];

    memcpy(CanMsg2.Data, canmsg_data1, sizeof(unsigned char) * 8);

    CanMsg1.ID = 0x10;
    unsigned char canmsg_data2[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    memcpy(CanMsg1.Data, canmsg_data2, sizeof(unsigned char) * 8);

    CAN_SendMsg(DevHandle[0], CAN1, &CanMsg1, 1);
    for (int i = 0; i < 14; i++) {
        CanMsg2.ID = lightid1[i];
        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg2, 1);
        CanMsg2.ID = lightid2[i];
        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg2, 1);
        CanMsg2.ID = lightid4[i];
        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg2, 1);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    // ����һ�����ȷֲ��������ֲ�������ΧΪ1��12
    std::uniform_int_distribution<int> dis(1, 12);
    // ���������������
    int rain_point1 = dis(gen);
    for (int i = 0; i < 20; i += 1) {
        CAN_MSG CanMsg = can_list1[i];
        CanMsg.ID = lightid1[rain_point1];
        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
        CanMsg.ID = lightid2[rain_point1];
        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
        CanMsg.ID = lightid4[rain_point1];
        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
        delay(sleep_time);
        //delay_func(sleep_time);
    }
    CanMsg2.ID = lightid1[rain_point1];
    CAN_SendMsg(DevHandle[0], CAN1, &CanMsg2, 1);
    CanMsg2.ID = lightid2[rain_point1];
    CAN_SendMsg(DevHandle[0], CAN1, &CanMsg2, 1);
    CanMsg2.ID = lightid4[rain_point1];
    CAN_SendMsg(DevHandle[0], CAN1, &CanMsg2, 1);

    return 2;
}
*/


int wind_mode(int len, int add[], int color1[], int color2[], int speed, int amp, int bias, bool mirror)
//��Χ�Ƶ���������һ�ο����������飬������1���������Ӧ�ĵ�ַ����������Ϊһ����ַ����
//�ƴ���ɫ��������ɫ���ٶ�(���Զ�Ӧ֡�������ӳ�ʱ��)�����ȣ�ƫ��Ƿ���
{
    //1.����speed�ж�ʱ��
    double delay_time = 0.025;   //�ӳ�ʱ��
    int frame = 20;     //��������һ��΢��Ķ�ЧΪ20֡����д�������ҵķ紵�� 
    int wind_stop = len - 1;     //������ƣ���ֹͣ��λ��

    switch (speed)
    {
    case 0:
        delay_time = 100;
        frame = 10;
    case 1:
        delay_time = 100;
        frame = 10;
    case 2:
        delay_time = 100;
        frame = 10;
    case 3:
        delay_time = 100;
        frame = 10;
    }
    //2.����amp�жϷ紵�ķ��� 
    switch (amp)
    {
    case 0:
        wind_stop = len - len/8;
    case 1:
        wind_stop = len - len/5;
    case 2:
        wind_stop = len - len/10;
    }

    //1��������ʾĬ�ϱ�����ɫ
    CAN_MSG CanMsg;
    CanMsg.ExternFlag = 0;
    CanMsg.RemoteFlag = 0;
    CanMsg.DataLen = 8;
    unsigned char canmsg_data[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
    canmsg_data[0] = color2[0];
    canmsg_data[1] = color2[1];
    canmsg_data[2] = color2[2];
    canmsg_data[4] = color2[0];
    canmsg_data[5] = color2[1];
    canmsg_data[6] = color2[2];
    memcpy(CanMsg.Data, canmsg_data, sizeof(unsigned char) * 8);
    CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
    for (int i = 0; i < len; i++) {
        CanMsg.ID = add[i];
        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
    }
    delay(delay_time);

    //2��Ч��ʵ��
    int wind_len = len / 2 ;
    std::vector<CAN_MSG> can_list;     //�洢��Ĳ�ͬ������ɫ, ����ɫ�𽥱�Ϊ�����ɫ�����𽥱�Ϊ����ɫ
    for (int i = 0; i <= wind_len; i++)
    {
        if (i < wind_len / 2)
        {
            unsigned char canmsg_data1[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            int p1 = (wind_len / 2 - i) / (wind_len / 2);
            int p2 = 1 - p1;
            canmsg_data1[0] = ( p2 * color1[0]);
            canmsg_data1[1] = ( p2 * color1[1]);
            canmsg_data1[2] = ( p2 * color1[2]);
            canmsg_data1[4] = ( p2 * color1[0]);
            canmsg_data1[5] = ( p2 * color1[1]);
            canmsg_data1[6] = ( p2 * color1[2]);
            memcpy(CanMsg.Data, canmsg_data1, sizeof(unsigned char) * 8);
            //CanMsg.ID = add[i];
            //CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            //delay(delay_time);
            can_list.push_back(CanMsg);
        }
        else
        {
            unsigned char canmsg_data1[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            int p1 = (i - wind_len/2) / (wind_len/2);
            int p2 = 1 - p1;
            canmsg_data1[0] = (p2 * color1[0] );
            canmsg_data1[1] = (p2 * color1[1] );
            canmsg_data1[2] = (p2 * color1[2] );
            canmsg_data1[4] = (p2 * color1[0] );
            canmsg_data1[5] = (p2 * color1[1] );
            canmsg_data1[6] = (p2 * color1[2] );
            memcpy(CanMsg.Data, canmsg_data1, sizeof(unsigned char) * 8);
           // CanMsg.ID = add[i];
            //CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            //delay(delay_time);
            can_list.push_back(CanMsg);
        }
    }

 
    //��������ҿ�ʼ����
    for (int j = 1; j <= frame; j++)       //�ӵ�һ֡��ʼ
    {
        float frame_stop = float(len) / float(frame) * j ;  //��ǰ֡�ڵƴ��е�ֹͣλ��
        if (frame_stop > wind_stop)      //1.�����жϵ�ǰ֡���Ƿ񳬹���ֹͣ��λ�ã������������ƴ�д�뱳��ɫ
        {
            for (int i = 0; i < len; i++) //    ÿһ֡ѭ��д�����еĵ���
            {
                canmsg_data[0] = color2[0];
                canmsg_data[1] = color2[1];
                canmsg_data[2] = color2[2];
                canmsg_data[4] = color2[0];
                canmsg_data[5] = color2[1];
                canmsg_data[6] = color2[2];
                memcpy(CanMsg.Data, canmsg_data, sizeof(unsigned char) * 8);
                CanMsg.ID = add[i];
                CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            }
            delay(delay_time);
        }
        else                   //��ǰ֡ û����ֹͣ��λ��                         
        {
            if (frame_stop <= wind_len)     //2.�жϵ�ǰ֡��λ�ã��Ƿ񳬹���ĳ��ȣ�С�ڷ�ĳ���д���Ľ���ɫ
            {
                for (int i = 0; i < len; i++) //    ÿһ֡ѭ��д�����еĵ���
                {
                    if (i <= frame_stop)        //�ж��Ƿ�С��ֹ֡ͣλ�ã���ǰ�����ֵ��ӦΪ can_list�д洢�Ľ�����ɫ��
                    {
                        CanMsg = can_list[i];
                        CanMsg.ID = add[i];
                        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
                    }
                    else       //����ֹ֡ͣλ��ӦΪ����ɫ
                    {
                        canmsg_data[0] = color2[0];
                        canmsg_data[1] = color2[1];
                        canmsg_data[2] = color2[2];
                        canmsg_data[4] = color2[0];
                        canmsg_data[5] = color2[1];
                        canmsg_data[6] = color2[2];
                        memcpy(CanMsg.Data, canmsg_data, sizeof(unsigned char) * 8);
                        CanMsg.ID = add[i];
                        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
                    }
                }
                delay(delay_time);
            }
            else   //��ǰ֡��λ�ó����˷�ĳ��ȣ���֡����-�糤�ȵ�λ��ӦΪ����ɫ������֡���ȵ�λ��ӦΪ����ɫ
            {
                for (int i = 0; i < len; i++) //    ÿһ֡ѭ��д�����еĵ���
                {
                    if (i >= (frame_stop - wind_len) && i <= frame_stop) //��ǰ����λ�ô���֡-�磬��С�ڷ�
                    {
                        CanMsg = can_list[i - (frame_stop - wind_len)];
                        CanMsg.ID = add[i];
                        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
                    }
                    else
                    {
                        canmsg_data[0] = color2[0];
                        canmsg_data[1] = color2[1];
                        canmsg_data[2] = color2[2];
                        canmsg_data[4] = color2[0];
                        canmsg_data[5] = color2[1];
                        canmsg_data[6] = color2[2];
                        memcpy(CanMsg.Data, canmsg_data, sizeof(unsigned char) * 8);
                        CanMsg.ID = add[i];
                        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
                    }
                }
                delay(delay_time);
            }
        }
    }

    //���������ʼ����
    int windstop_left = len - wind_stop;
    for (int j = 1; j <= frame; j++)       //�ӵ�һ֡��ʼ
    {
        float frame_stop = float(len) / float(frame)* (frame - j);  //��ǰ֡�ڵƴ��е�ֹͣλ��
        
        if (frame_stop < windstop_left)     //�����ж�frameֹͣλ��   �Ƿ񳬹�ֹͣλ��   frame_stop < wind_stop
        {
            for (int i = 0; i < len; i++) //    ÿһ֡ѭ��д�����еĵ���,ȫ������Ϊ����ɫ
            {
                canmsg_data[0] = color2[0];
                canmsg_data[1] = color2[1];
                canmsg_data[2] = color2[2];
                canmsg_data[4] = color2[0];
                canmsg_data[5] = color2[1];
                canmsg_data[6] = color2[2];
                memcpy(CanMsg.Data, canmsg_data, sizeof(unsigned char) * 8);
                CanMsg.ID = add[i];
                CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            }
            delay(delay_time);
        }
        else      //frameֹͣλ��  �� wind stopλ��֮ǰ
        {
            if (len - frame_stop < wind_len)  //�жϵ�ǰlen - framֹͣλ�ã��Ƿ�С�ڷ�ĳ���
            {
                for (int i = 0; i < len; i++) //    ÿһ֡ѭ��д�����еĵ���
                {
                    if (i >= frame_stop)        //�ж��Ƿ����ֹ֡ͣλ�ã���ǰ�����ֵ��ӦΪ can_list�д洢�Ľ�����ɫ��
                    {
                        CanMsg = can_list[i - frame_stop];
                        CanMsg.ID = add[i];
                        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
                    }
                    else       //С��ֹ֡ͣλ��ӦΪ����ɫ
                    {
                        canmsg_data[0] = color2[0];
                        canmsg_data[1] = color2[1];
                        canmsg_data[2] = color2[2];
                        canmsg_data[4] = color2[0];
                        canmsg_data[5] = color2[1];
                        canmsg_data[6] = color2[2];
                        memcpy(CanMsg.Data, canmsg_data, sizeof(unsigned char) * 8);
                        CanMsg.ID = add[i];
                        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
                    }
                }
                delay(delay_time);
            }
            else    //��ǰlen - ֹ֡ͣλ�� ���ڷ�ĳ���
            {
                for (int i = 0; i < len; i++) //    ÿһ֡ѭ��д�����еĵ���
                {
                    if (i >= frame_stop && i <= wind_len + frame_stop) //��ǰ����λ�ô���֡����С�ڷ�+֡
                    {
                        CanMsg = can_list[i - frame_stop];
                        CanMsg.ID = add[i];
                        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
                    }
                    else
                    {
                        canmsg_data[0] = color2[0];
                        canmsg_data[1] = color2[1];
                        canmsg_data[2] = color2[2];
                        canmsg_data[4] = color2[0];
                        canmsg_data[5] = color2[1];
                        canmsg_data[6] = color2[2];
                        memcpy(CanMsg.Data, canmsg_data, sizeof(unsigned char) * 8);
                        CanMsg.ID = add[i];
                        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
                    }
                }
                delay(delay_time);
            }
        }
    }


    //3.�ָ���ʾ��ɫ
    

    return 1;
}


int boDong_mode(int len, int add[], int color1[], int color2[], int speed, int amp, int bias, bool mirror)
                //��Χ�Ƶ���������һ�ο����������飬������1���������Ӧ�ĵ�ַ����������Ϊһ����ַ����
                //�ƴ���ɫ��������ɫ���ٶ�(���Զ�Ӧ֡�������ӳ�ʱ��)�����ȣ�ƫ��Ƿ���
{
    //1.����speed�ж�ʱ��
     //1.����speed�ж�ʱ��
    double delay_time = 0.025;   //�ӳ�ʱ��
    int frame = 20;     //��������һ��΢��Ķ�ЧΪ20֡����д�������ҵķ紵�� 
    int short_long = len/4;     //������ƣ��̵Ĳ�������
    int long_long = len / 2 - 1;       //������ƣ����Ĳ�������

    switch (speed)
    {
    case 0:
        delay_time = 1000;
        frame = 20;
    case 1:
        delay_time = 1000;
        frame = 20;
    case 2:
        delay_time = 1000;
        frame = 20;
    case 3:
        delay_time = 1000;
        frame = 20;
    }
    //2.����amp�жϷ紵�ķ��� 
    switch (amp)
    {
    case 0:
        short_long = len / 8;
        long_long = len / 4;

    case 1:
        short_long = len / 4;
        long_long = len / 2 - 3;
    case 2:
        short_long = len / 4 + 2;
        long_long = len / 2 - 1;
    }

    //1��������ʾĬ�ϱ�����ɫ
    CAN_MSG CanMsg;
    CanMsg.ExternFlag = 0;
    CanMsg.RemoteFlag = 0;
    CanMsg.DataLen = 8;
    unsigned char canmsg_data[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
    canmsg_data[0] = color2[0];
    canmsg_data[1] = color2[1];
    canmsg_data[2] = color2[2];
    canmsg_data[4] = color2[0];
    canmsg_data[5] = color2[1];
    canmsg_data[6] = color2[2];
    memcpy(CanMsg.Data, canmsg_data, sizeof(unsigned char) * 8);
    CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
    for (int i = 0; i < len; i++) {
        CanMsg.ID = add[i];
        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
    }
    delay(delay_time);


    //2.Ч��ʵ��
   
    std::vector<CAN_MSG> can_list1;     //�洢�̲����Ľ�����ɫ
    for (int i = 0; i <= short_long; i++)
    {
        unsigned char canmsg_data1[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
        int p1 = i / short_long;
        int p2 = 1 - p1;
        canmsg_data1[0] = (p2 * color1[0]);
        canmsg_data1[1] = (p2 * color1[1]);
        canmsg_data1[2] = (p2 * color1[2]);
        canmsg_data1[4] = (p2 * color1[0]);
        canmsg_data1[5] = (p2 * color1[1]);
        canmsg_data1[6] = (p2 * color1[2]);
        memcpy(CanMsg.Data, canmsg_data1, sizeof(unsigned char) * 8);
        //CanMsg.ID = add[i];
        //CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
        //delay(delay_time);
        can_list1.push_back(CanMsg);            
    }

    std::vector<CAN_MSG> can_list2;     //�洢�������Ľ�����ɫ
    for (int i = 0; i <= long_long; i++)
    {
        unsigned char canmsg_data2[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
        int p1 = i / short_long;
        int p2 = 1 - p1;
        canmsg_data2[0] = (p2 * color1[0]);
        canmsg_data2[1] = (p2 * color1[1]);
        canmsg_data2[2] = (p2 * color1[2]);
        canmsg_data2[4] = (p2 * color1[0]);
        canmsg_data2[5] = (p2 * color1[1]);
        canmsg_data2[6] = (p2 * color1[2]);
        memcpy(CanMsg.Data, canmsg_data2, sizeof(unsigned char) * 8);
        //CanMsg.ID = add[i];
        //CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
        //delay(delay_time);
        can_list2.push_back(CanMsg);
    }

    //1.�̲�����ʵ��,���м�λ��������ɢ���̲���λ��
    for (int i = 0; i < frame; i++) 
    {
        int frameStop_right = 2 / len + short_long / frame * i;
        int frameStop_left = 2 / len -  short_long / frame * i;
        
        for(int i = 0; i < len; i++)  //д�����еĵ���
        {
            if (i < frameStop_left || i > frameStop_right)   //������鲻�ڵ�ǰ֡�ķ�Χ�ڣ�д�뱳��ɫ
            {
                canmsg_data[0] = color2[0];
                canmsg_data[1] = color2[1];
                canmsg_data[2] = color2[2];
                canmsg_data[4] = color2[0];
                canmsg_data[5] = color2[1];
                canmsg_data[6] = color2[2];
                memcpy(CanMsg.Data, canmsg_data, sizeof(unsigned char) * 8);
                CanMsg.ID = add[i];
                CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            }
            else if (i >= frameStop_left && i < len / 2)
            {
                CanMsg = can_list1[len/2 - i];
                CanMsg.ID = add[i];
                CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            }
            else
            {
                CanMsg = can_list1[i - len/2];
                CanMsg.ID = add[i];
                CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            }
        }
        delay(delay_time);
    }

    //2.�̲�����ʵ��,�����߶̲���λ����ɢ���м�λ��
    for (int i = 0; i < frame; i++)
    {
        int frameStop_right = (len/2 +short_long) - short_long / frame * i;
        int frameStop_left =  (len/2 - short_long) + short_long / frame * i;

        if (i < frameStop_left || i > frameStop_right)   //������鲻�ڵ�ǰ֡�ķ�Χ�ڣ�д�뱳��ɫ
        {
            canmsg_data[0] = color2[0];
            canmsg_data[1] = color2[1];
            canmsg_data[2] = color2[2];
            canmsg_data[4] = color2[0];
            canmsg_data[5] = color2[1];
            canmsg_data[6] = color2[2];
            memcpy(CanMsg.Data, canmsg_data, sizeof(unsigned char) * 8);
            CanMsg.ID = add[i];
            CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
        }
        else if (i >= frameStop_left && i < len / 2)
        {
            CanMsg = can_list1[len / 2 - i];
            CanMsg.ID = add[i];
            CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
        }
        else
        {
            CanMsg = can_list1[i - len / 2];
            CanMsg.ID = add[i];
            CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
        }
        
        delay(delay_time);
    }

    //3.��������ʵ�֣����м�λ����ɢ�ĳ�������λ��
    for (int i = 0; i < frame; i++)
    {
        int frameStop_right = 2 / len + long_long / frame * i;
        int frameStop_left = 2 / len - long_long / frame * i;

        for (int i = 0; i < len; i++)  //д�����еĵ���
        {
            if (i < frameStop_left || i > frameStop_right)   //������鲻�ڵ�ǰ֡�ķ�Χ�ڣ�д�뱳��ɫ
            {
                canmsg_data[0] = color2[0];
                canmsg_data[1] = color2[1];
                canmsg_data[2] = color2[2];
                canmsg_data[4] = color2[0];
                canmsg_data[5] = color2[1];
                canmsg_data[6] = color2[2];
                memcpy(CanMsg.Data, canmsg_data, sizeof(unsigned char) * 8);
                CanMsg.ID = add[i];
                CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            }
            else if (i >= frameStop_left && i < len / 2)
            {
                CanMsg = can_list2[len / 2 - i];
                CanMsg.ID = add[i];
                CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            }
            else
            {
                CanMsg = can_list2[i - len / 2];
                CanMsg.ID = add[i];
                CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            }
        }
        delay(delay_time);
    }

    //4.��������ʵ�֣������߳�������λ����ɢ���м�λ��
    for (int i = 0; i < frame; i++)
    {
        int frameStop_right = (len / 2 + long_long) - long_long / frame * i;
        int frameStop_left = (len / 2 - long_long) + long_long / frame * i;

        if (i < frameStop_left || i > frameStop_right)   //������鲻�ڵ�ǰ֡�ķ�Χ�ڣ�д�뱳��ɫ
        {
            canmsg_data[0] = color2[0];
            canmsg_data[1] = color2[1];
            canmsg_data[2] = color2[2];
            canmsg_data[4] = color2[0];
            canmsg_data[5] = color2[1];
            canmsg_data[6] = color2[2];
            memcpy(CanMsg.Data, canmsg_data, sizeof(unsigned char) * 8);
            CanMsg.ID = add[i];
            CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
        }
        else if (i >= frameStop_left && i < len / 2)
        {
            CanMsg = can_list2[len / 2 - i];
            CanMsg.ID = add[i];
            CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
        }
        else
        {
            CanMsg = can_list2[i - len / 2];
            CanMsg.ID = add[i];
            CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
        }

        delay(delay_time);
    }

    
    //3.�ָ���ʾ��ɫ



    return 1;
}
                 

           


      


int main(int argc, const char* argv[])
{

#if GET_FIRMWARE_INFO
    DEVICE_INFO DevInfo;
#endif

    //��dll�ļ���������
    char dllBuildDateTime[64] = { 0 };
    DEV_GetDllBuildTime(dllBuildDateTime);
    printf("DLL Build Date Time = %s\n", dllBuildDateTime);
    //ɨ������豸
    ret = USB_ScanDevice(DevHandle);
    if (ret <= 0) {
        printf("No device connected!\n");
        return 0;
    }
    //���豸
    state = USB_OpenDevice(DevHandle[0]);
    if (!state) {
        printf("Open device error!\n");
        return 0;
    }
#if GET_FIRMWARE_INFO
    char FunctionStr[256] = { 0 };
    //��ȡ�̼���Ϣ
    state = DEV_GetDeviceInfo(DevHandle[0], &DevInfo, FunctionStr);
    if (!state) {
        printf("Get device infomation error!\n");
        return 0;
    }
    else {
        printf("Firmware Info:\n");
        printf("Firmware Name:%s\n", DevInfo.FirmwareName);
        printf("Firmware Build Date:%s\n", DevInfo.BuildDate);
        printf("Firmware Version:v%d.%d.%d\n", (DevInfo.FirmwareVersion >> 24) & 0xFF, (DevInfo.FirmwareVersion >> 16) & 0xFF, DevInfo.FirmwareVersion & 0xFFFF);
        printf("Hardware Version:v%d.%d.%d\n", (DevInfo.HardwareVersion >> 24) & 0xFF, (DevInfo.HardwareVersion >> 16) & 0xFF, DevInfo.HardwareVersion & 0xFFFF);
        printf("Firmware Functions:%s\n", FunctionStr);
    }
#endif
    //��ʼ������CAN,������Ϊ500000
    CAN_INIT_CONFIG CANConfig;
    ret = CAN_GetCANSpeedArg(DevHandle[0], &CANConfig, 500000);
    if (ret != CAN_SUCCESS) {
        printf("Get CAN Speed Arg Failed!\n");
        return 0;
    }
    else {
        printf("Get CAN Speed Arg Success!\n");
    }
#if CAN_MODE_LOOPBACK
    CANConfig.CAN_Mode = 1;//����ģʽ
#else
    CANConfig.CAN_Mode = 0x80;//����ģʽ
#endif
    ret = CAN_Init(DevHandle[0], SendCANIndex, &CANConfig);
    if (ret != CAN_SUCCESS) {
        printf("Config CAN failed!\n");
        return 0;
    }
    else {
        printf("Config CAN Success!\n");
    }
    ret = CAN_Init(DevHandle[0], ReadCANIndex, &CANConfig);
    if (ret != CAN_SUCCESS) {
        printf("Config CAN failed!\n");
        return 0;
    }
    else {
        printf("Config CAN Success!\n");
    }
    //���ù��������������ã���������޷��յ�����
    CAN_FILTER_CONFIG CANFilter;
    CANFilter.Enable = 1;
    CANFilter.ExtFrame = 0;
    CANFilter.FilterIndex = 0;
    CANFilter.FilterMode = 0;
    CANFilter.MASK_IDE = 0;
    CANFilter.MASK_RTR = 0;
    CANFilter.MASK_Std_Ext = 0;
    ret = CAN_Filter_Init(DevHandle[0], ReadCANIndex, &CANFilter);
    if (ret != CAN_SUCCESS) {
        printf("Config CAN Filter failed!\n");
        return 0;
    }
    else {
        printf("Config CAN Filter Success!\n");
    }

    delay(100);
    printf("��ʼ����!\n");


    //1.����΢�綯Ч
    int color_1[3] = { 255, 0, 0 }; // # �ƴ���ɫ
    int color_2[3] = { 255, 255, 0 }; // # ������ɫ
    int add[14] = { 0x301, 0x302, 0x303, 0x304, 0x305, 0x306, 0x307, 0x308, 0x309, 0x30A, 0x30B, 0x30C, 0x30D,0x30E };
    int len = 14;
    int speed = 3;
    int amp = 2;
    int bias = 0;
    bool mirror = false;


    int a = wind_mode(len, add, color_1, color_2, speed, amp, bias, mirror);
    
   
    

    printf("�������!\n");

    return 0;
}

