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

int lightid1[14] = { 0x101, 0x102, 0x103, 0x104, 0x105, 0x106, 0x107, 0x108, 0x109, 0x10A, 0x10B, 0x10C, 0x10D,
0x10E };

int lightid2[14] = { 0x201, 0x202, 0x203, 0x204, 0x205, 0x206, 0x207, 0x208, 0x209, 0x20A, 0x20B, 0x20C, 0x20D,
0x20E };

int lightid4[14] = { 0x101, 0x102, 0x103, 0x104, 0x105, 0x106, 0x107, 0x108, 0x109, 0x10A, 0x10B, 0x10C, 0x10D,
0x10E };

int color_1[3] = { 255, 211, 0 }; //# 波动颜色 黄色
int color_2[3] = {120, 3, 241}; // # 背景颜色  配色3

int CAN1 = 0;

void delay(int milliseconds) {
    clock_t start_time = clock();
    while ((clock() - start_time) * 1000 / CLOCKS_PER_SEC < milliseconds) {
        // 空循环，等待时间流逝
    }
}

double round(double num, int decimal_places) {
    double multiplier = pow(10, decimal_places);
    double rounded_value = num * multiplier;
    double rounded_integer = (num >= 0) ? floor(rounded_value + 0.5) : ceil(rounded_value - 0.5);
    return rounded_integer / multiplier;
}

//雨滴
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
    // 创建一个均匀分布的整数分布器，范围为1到12
    std::uniform_int_distribution<int> dis(1, 12);
    // 生成随机数并返回
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


//涟漪
void light_ripple_mode1(int move_speed, int  sleep_time) {
    std::vector<CAN_MSG> can_list1;
    int ripple_color1[3] = { 255, 211, 0 }; //# 波动颜色 黄色
    int ripple_color2[3] = { 120, 3, 241 }; // # 背景颜色  配色3
    CAN_MSG CanMsg;
    CanMsg.ExternFlag = 0; // 是否是扩展帧
    CanMsg.RemoteFlag = 0; // 是否是远程帧
    CanMsg.DataLen = 8; //数据长度(<= 8)，即Data 的长度。
    CAN_MSG CanMsg1;  // 创建CAN报文数组
    CanMsg1.ExternFlag = 0;// # 是否是扩展帧
    CanMsg1.RemoteFlag = 0;//  # 是否是远程帧
    CanMsg1.DataLen = 8; // 数据长度(<= 8)，即Data 的长度。
    CanMsg.ID = 0x10; //# 报文ID
    CanMsg.Data[0] = 0x00;
    CanMsg.Data[1] = 0x00;
    CanMsg.Data[2] = 0x00;
    CanMsg.Data[3] = 0x00;
    CanMsg.Data[4] = 0x00;
    CanMsg.Data[5] = 0x00;
    CanMsg.Data[6] = 0x00;
    CanMsg.Data[7] = 0x00;// # 广播 分别控制
    CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);// # 发送指令
    unsigned char canmsg_data[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
    canmsg_data[0] = ripple_color2[0];
    canmsg_data[1] = ripple_color2[1];
    canmsg_data[2] = ripple_color2[2];
    canmsg_data[4] = ripple_color2[0];
    canmsg_data[5] = ripple_color2[1];
    canmsg_data[6] = ripple_color2[2];
    //CanMsg.Data = tuple(canmsg_data)

    memcpy(CanMsg.Data, canmsg_data, sizeof(unsigned char) * 8);
    //CanMsg.Data= canmsg_data;
   // can_list1.push_back(CanMsg);
    for (int j = 0; j < 14; j++)// # 设为背景色
    {
        CanMsg.ID = lightid1[j];
        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);// # 发送指令
    }

    int num = 0;
    int rate_list[28] = { 0 };//  # 颜色比例列表
    rate_list[13] = 1;
    rate_list[14] = 1;
    float color_change_rate = 1 / move_speed;

    for (int k = 0; k < move_speed; k++)
    {
        if ((14 + num + 1) <= 27)
        {
            rate_list[13 - num] = round(max(rate_list[13 - num] - color_change_rate, 0), 2);
            rate_list[13 - num - 1] = round(min(rate_list[13 - num - 1] + color_change_rate, 1), 2);
            rate_list[14 + num] = round(max(rate_list[14 + num] - color_change_rate, 0), 2);
            rate_list[14 + num + 1] = round(min(rate_list[14 + num + 1] + color_change_rate, 1), 2);
        }
        if (num > 4)
        {
            if ((14 + num - 4 + 1) <= 27) {
                rate_list[13 - num + 4] = round(max(rate_list[13 - num + 4] - color_change_rate * 0.6, 0), 2);
                rate_list[13 - num + 4 - 1] = round(min(rate_list[13 - num + 4 - 1] + color_change_rate * 0.6, 0.6), 2);
                rate_list[14 + num - 4] = round(max(rate_list[14 + num - 4] - color_change_rate * 0.6, 0), 2);
                rate_list[14 + num - 4 + 1] = round(min(rate_list[14 + num - 4 + 1] + color_change_rate * 0.6, 0.6), 2);
            }

        }
        if (num > 8)
        {
            rate_list[13 - num + 8] = round(max(rate_list[13 - num + 8] - color_change_rate * 0.2, 0), 2);
            rate_list[13 - num + 8 - 1] = round(min(rate_list[13 - num + 8 - 1] + color_change_rate * 0.2, 0.2), 2);
            rate_list[14 + num - 8] = round(max(rate_list[14 + num - 8] - color_change_rate * 0.2, 0), 2);
            rate_list[14 + num - 8 + 1] = round(min(rate_list[14 + num - 8 + 1] + color_change_rate * 0.2, 0.2), 2);
        }
        if ((14 + num + 1) == 28)
        {
            rate_list[0] = round(max(rate_list[0] - color_change_rate, 0), 2);  //# 第一个位置上的灯珠
            rate_list[27] = round(max(rate_list[27] - color_change_rate, 0), 2); // # 最后一个位置上的灯珠
        }
        if ((14 + num - 4 + 1) == 28)
        {
            rate_list[0] = round(max(rate_list[0] - color_change_rate * 0.6, 0), 2);  //# 第一个位置上的灯珠
            rate_list[27] = round(max(rate_list[27] - color_change_rate * 0.6, 0), 2); // # 最后一个位置上的灯珠
        }
        for (int i = 0; i < 14; i++) {
            CanMsg1.ID = lightid1[i];
            unsigned char canmsg_data2[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            canmsg_data2[0] = int(ripple_color1[0] * rate_list[2 * i] + ripple_color2[0] * (1 - rate_list[2 * i]));
            canmsg_data2[1] = int(ripple_color1[1] * rate_list[2 * i] + ripple_color2[1] * (1 - rate_list[2 * i]));
            canmsg_data2[2] = int(ripple_color1[2] * rate_list[2 * i] + ripple_color2[2] * (1 - rate_list[2 * i]));
            canmsg_data2[4] = int(ripple_color1[0] * rate_list[2 * i + 1] + ripple_color2[0] * (1 - rate_list[2 * i + 1]));
            canmsg_data2[5] = int(ripple_color1[1] * rate_list[2 * i + 1] + ripple_color2[1] * (1 - rate_list[2 * i + 1]));
            canmsg_data2[6] = int(ripple_color1[2] * rate_list[2 * i + 1] + ripple_color2[2] * (1 - rate_list[2 * i + 1]));
            memcpy(CanMsg1.Data, canmsg_data2, sizeof(unsigned char) * 8);
            //CanMsg.Data= canmsg_data;
            can_list1.push_back(CanMsg1);
            CAN_SendMsg(DevHandle[0], CAN1, &CanMsg1, 1);
        }
        delay(sleep_time);
    }

    num += 1;
    if ((14 + num - 8 + 1) > 27)
    {
        for (int k = 0; k < move_speed; k++)
        {
            rate_list[0] = round(max(rate_list[0] - color_change_rate * 0.2, 0), 2);  //# 第一个位置上的灯珠
            rate_list[27] = round(max(rate_list[27] - color_change_rate * 0.2, 0), 2);  //# 最后一个位置上的灯珠
            for (int i = 0; i < 14; i++) {
                CanMsg1.ID = lightid1[i];
                unsigned char canmsg_data3[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
                canmsg_data3[0] = int(ripple_color1[0] * rate_list[2 * i] + ripple_color2[0] * (1 - rate_list[2 * i]));
                canmsg_data3[1] = int(ripple_color1[1] * rate_list[2 * i] + ripple_color2[1] * (1 - rate_list[2 * i]));
                canmsg_data3[2] = int(ripple_color1[2] * rate_list[2 * i] + ripple_color2[2] * (1 - rate_list[2 * i]));
                canmsg_data3[4] = int(ripple_color1[0] * rate_list[2 * i + 1] + ripple_color2[0] * (1 - rate_list[2 * i + 1]));
                canmsg_data3[5] = int(ripple_color1[1] * rate_list[2 * i + 1] + ripple_color2[1] * (1 - rate_list[2 * i + 1]));
                canmsg_data3[6] = int(ripple_color1[2] * rate_list[2 * i + 1] + ripple_color2[2] * (1 - rate_list[2 * i + 1]));
                memcpy(CanMsg1.Data, canmsg_data3, sizeof(unsigned char) * 8);
                //CanMsg.Data= canmsg_data;
                can_list1.push_back(CanMsg1);
                CAN_SendMsg(DevHandle[0], CAN1, &CanMsg1, 1);
            }
            delay(sleep_time);
            num = 0;
        }
    }
}

//呼吸
void light_breathing_mode(int color_num, int sleep_time) {
    std::vector<CAN_MSG> can_list1;
    std::vector<CAN_MSG> can_list2;
    std::vector<CAN_MSG> can_list3;

    double m = 1.0;
    double n = 1.0;
    double q = 1.0;
    int breathing_color_1[3] = { 255, 48, 48 };
    int breathing_color_2[3] = {60, 255, 40};
    int breathing_color_3[3] = { 0, 197, 205 };
    for (int i = 0; i < 20; i++)
    {
        CAN_MSG CanMsg;
        CanMsg.ExternFlag = 0;
        CanMsg.RemoteFlag = 0;
        CanMsg.DataLen = 8;
        CanMsg.ID = 0x10;
        if (i < 10)
        {
            double p1 = (19 - i) / 19.0;
            double p2 = 1 - p1;
            unsigned char canmsg_data0[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            canmsg_data0[0] = int((breathing_color_1[0] * p1 + breathing_color_2[0] * p2) / m);
            canmsg_data0[1] = int((breathing_color_1[1] * p1 + breathing_color_2[1] * p2) / m);
            canmsg_data0[2] = int((breathing_color_1[2] * p1 + breathing_color_2[2] * p2) / m);
            memcpy(CanMsg.Data, canmsg_data0, sizeof(unsigned char) * 8);
            //CanMsg.Data= canmsg_data;
            can_list1.push_back(CanMsg);
            m += 0.2;
        }
        else
        {
            double p1 = (19 - i) / 19.0;
            double p2 = 1 - p1;
            unsigned char canmsg_data0[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            canmsg_data0[0] = int((breathing_color_1[0] * p1 + breathing_color_2[0] * p2) / m);
            canmsg_data0[1] = int((breathing_color_1[1] * p1 + breathing_color_2[1] * p2) / m);
            canmsg_data0[2] = int((breathing_color_1[2] * p1 + breathing_color_2[2] * p2) / m);
            memcpy(CanMsg.Data, canmsg_data0, sizeof(unsigned char) * 8);
            //CanMsg.Data= canmsg_data;
            can_list1.push_back(CanMsg);
            m -= 0.2;
        }
    }
    for (int j = 0; j < 20; j++)
    {
        CAN_MSG CanMsg1;
        CanMsg1.ExternFlag = 0;
        CanMsg1.RemoteFlag = 0;
        CanMsg1.DataLen = 8;
        CanMsg1.ID = 0x10;
        if (j < 10)
        {
            double p1 = (19 - j) / 19.0;
            double p2 = 1 - p1;
            unsigned char canmsg_data1[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            canmsg_data1[0] = int((breathing_color_2[0] * p1 + breathing_color_3[0] * p2) / n);
            canmsg_data1[1] = int((breathing_color_2[1] * p1 + breathing_color_3[1] * p2) / n);
            canmsg_data1[2] = int((breathing_color_2[2] * p1 + breathing_color_3[2] * p2) / n);
            memcpy(CanMsg1.Data, canmsg_data1, sizeof(unsigned char) * 8);
            //CanMsg.Data= canmsg_data;
            can_list1.push_back(CanMsg1);
            n += 0.2;
        }
        else
        {
            double p1 = (19 - j) / 19.0;
            double p2 = 1 - p1;
            unsigned char canmsg_data1[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            canmsg_data1[0] = int((breathing_color_2[0] * p1 + breathing_color_3[0] * p2) / n);
            canmsg_data1[1] = int((breathing_color_2[1] * p1 + breathing_color_3[1] * p2) / n);
            canmsg_data1[2] = int((breathing_color_2[2] * p1 + breathing_color_3[2] * p2) / n);
            memcpy(CanMsg1.Data, canmsg_data1, sizeof(unsigned char) * 8);
            //CanMsg.Data= canmsg_data;
            can_list1.push_back(CanMsg1);
            n -= 0.2;
        }
    }
    for (int k = 0; k < 20; k++)
    {
        CAN_MSG CanMsg2;
        CanMsg2.ExternFlag = 0;
        CanMsg2.RemoteFlag = 0;
        CanMsg2.DataLen = 8;
        CanMsg2.ID = 0x10;
        if (k < 10)
        {
            double p1 = (19 - k) / 19.0;
            double p2 = 1 - p1;
            unsigned char canmsg_data2[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            canmsg_data2[0] = int((breathing_color_3[0] * p1 + breathing_color_1[0] * p2) / q);
            canmsg_data2[1] = int((breathing_color_3[1] * p1 + breathing_color_1[1] * p2) / q);
            canmsg_data2[2] = int((breathing_color_3[2] * p1 + breathing_color_1[2] * p2) / q);
            memcpy(CanMsg2.Data, canmsg_data2, sizeof(unsigned char) * 8);
            //CanMsg.Data= canmsg_data;
            can_list1.push_back(CanMsg2);
            q += 0.2;
        }
        else
        {
            double p1 = (19 - k) / 19.0;
            double p2 = 1 - p1;
            unsigned char canmsg_data2[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            canmsg_data2[0] = int((breathing_color_3[0] * p1 + breathing_color_1[0] * p2) / q);
            canmsg_data2[1] = int((breathing_color_3[1] * p1 + breathing_color_1[1] * p2) / q);
            canmsg_data2[2] = int((breathing_color_3[2] * p1 + breathing_color_1[2] * p2) / q);
            memcpy(CanMsg2.Data, canmsg_data2, sizeof(unsigned char) * 8);
            //CanMsg.Data= canmsg_data;
            can_list1.push_back(CanMsg2);
            q -= 0.2;
        }
    }
    CAN_MSG CanMsg3;
    if (color_num == 0)
    {
        for (int i = 0; i < 20; i++)
        {
            CanMsg3 = can_list1[i];
            CAN_SendMsg(DevHandle[0], CAN1, &CanMsg3, 1); // 发送指令
            delay(sleep_time);
        }
    }
    else if(color_num == 1)
    {
        for (int i = 0; i < 20; i++)
        {
            CanMsg3 = can_list2[i];
            CAN_SendMsg(DevHandle[0], CAN1, &CanMsg3, 1); // 发送指令
            delay(sleep_time);
        }
    }
    else if(color_num == 2)
    {
        for (int i = 0; i < 20; i++)
        {
            CanMsg3 = can_list3[i];
            CAN_SendMsg(DevHandle[0], CAN1, &CanMsg3, 1); // 发送指令
            delay(sleep_time);
        }
    }


}

//海洋
void sea_mode1(int sleep_time) {

    int sea_color1[3] = { 255, 255, 255 };  //# 浪花
    int sea_color2[3] = { 0, 64, 255 }; //# 大海
    int sea_color3[3] = { 70, 60, 47 }; //# 沙滩

    CAN_MSG CanMsg0;
    CanMsg0.ExternFlag = 0;
    CanMsg0.RemoteFlag = 0;
    CanMsg0.DataLen = 8;
    CAN_MSG CanMsg1;
    CanMsg1.ExternFlag = 0;
    CanMsg1.RemoteFlag = 0;
    CanMsg1.DataLen = 8;
    CAN_MSG CanMsg2;
    CanMsg2.ExternFlag = 0;
    CanMsg2.RemoteFlag = 0;
    CanMsg2.DataLen = 8;
    CanMsg1.ID = 0x10;
    unsigned char canmsg_data2[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };//广播分别控制
    memcpy(CanMsg0.Data, canmsg_data2, sizeof(unsigned char) * 8);
    CAN_SendMsg(DevHandle[0], CAN1, &CanMsg0, 1); //# 发送指令

    unsigned char canmsg_data1[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
    canmsg_data1[0] = sea_color3[0];
    canmsg_data1[1] = sea_color3[1];
    canmsg_data1[2] = sea_color3[2];
    canmsg_data1[4] = sea_color2[0];
    canmsg_data1[5] = sea_color2[1];
    canmsg_data1[6] = sea_color2[2];
    memcpy(CanMsg1.Data, canmsg_data2, sizeof(unsigned char) * 8);//左沙滩右大海

    canmsg_data1[0] = sea_color2[0];
    canmsg_data1[1] = sea_color2[1];
    canmsg_data1[2] = sea_color2[2];
    canmsg_data1[4] = sea_color3[0];
    canmsg_data1[5] = sea_color3[1];
    canmsg_data1[6] = sea_color3[2];
    memcpy(CanMsg1.Data, canmsg_data2, sizeof(unsigned char) * 8);//左大海右沙滩

    std::vector<CAN_MSG> sea_list1;
    for (int i = 0; i < 20; i++)
    {
        CAN_MSG CanMsg;
        CanMsg.ExternFlag = 0;
        CanMsg.RemoteFlag = 0;
        CanMsg.DataLen = 8;
        CanMsg.ID = 0x10;
        if (i < 10)
        {
            double p1 = (9 - i) / 9.0;
            double p2 = 1 - p1;
            unsigned char canmsg_data0[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            canmsg_data0[0] = int(sea_color3[0] * p1 + sea_color1[0] * p2) ;
            canmsg_data0[1] = int(sea_color3[1] * p1 + sea_color1[1] * p2) ;
            canmsg_data0[2] = int(sea_color3[2] * p1 + sea_color1[2] * p2) ;
            canmsg_data0[4] = sea_color3[0];
            canmsg_data0[5] = sea_color3[1];
            canmsg_data0[6] = sea_color3[2];
            memcpy(CanMsg.Data, canmsg_data0, sizeof(unsigned char) * 8);
            //CanMsg.Data= canmsg_data;
            sea_list1.push_back(CanMsg);
        }
        else
        {
            double p1 = (i-10) / 9.0;
            double p2 = 1 - p1;
            unsigned char canmsg_data0[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            canmsg_data0[0] = int(sea_color1[0] * p1 + sea_color3[0] * p2);
            canmsg_data0[1] = int(sea_color1[1] * p1 + sea_color3[1] * p2);
            canmsg_data0[2] = int(sea_color1[2] * p1 + sea_color3[2] * p2);
            memcpy(CanMsg.Data, canmsg_data0, sizeof(unsigned char) * 8);
            //CanMsg.Data= canmsg_data;
            sea_list1.push_back(CanMsg);
        }
    }

    std::vector<CAN_MSG> sea_list2;
    for (int i = 0; i < 20; i++)
    {
        CAN_MSG CanMsg;
        CanMsg.ExternFlag = 0;
        CanMsg.RemoteFlag = 0;
        CanMsg.DataLen = 8;
        CanMsg.ID = 0x10;
        if (i < 10)
        {
            double p1 = (9 - i) / 9.0;
            double p2 = 1 - p1;
            unsigned char canmsg_data1[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            canmsg_data1[0] = sea_color2[0];
            canmsg_data1[1] = sea_color2[1];
            canmsg_data1[2] = sea_color2[2];
            canmsg_data1[4] = int(sea_color2[0] * p1 + sea_color1[0] * p2);
            canmsg_data1[5] = int(sea_color2[1] * p1 + sea_color1[1] * p2);
            canmsg_data1[6] = int(sea_color2[2] * p1 + sea_color1[2] * p2);
            memcpy(CanMsg.Data, canmsg_data1, sizeof(unsigned char) * 8);
            //CanMsg.Data= canmsg_data;
            sea_list2.push_back(CanMsg);
        }
        else
        {
            double p1 = (i - 10) / 9.0;
            double p2 = 1 - p1;
            unsigned char canmsg_data0[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            canmsg_data1[0] = int(sea_color1[0] * p1 + sea_color3[0] * p2);
            canmsg_data1[1] = int(sea_color1[1] * p1 + sea_color3[1] * p2);
            canmsg_data1[2] = int(sea_color1[2] * p1 + sea_color3[2] * p2);
            canmsg_data1[4] = sea_color1[0];
            canmsg_data1[5] = sea_color1[1];
            canmsg_data1[6] = sea_color1[2];
            memcpy(CanMsg.Data, canmsg_data0, sizeof(unsigned char) * 8);
            //CanMsg.Data= canmsg_data;
            sea_list2.push_back(CanMsg);
        }
    }

    std::vector<CAN_MSG> sea_list3;
    for (int i = 0; i < 20; i++)
    {
        CAN_MSG CanMsg;
        CanMsg.ExternFlag = 0;
        CanMsg.RemoteFlag = 0;
        CanMsg.DataLen = 8;
        CanMsg.ID = 0x10;
        if (i < 10)
        {
            double p1 = (9 - i) / 9.0;
            double p2 = 1 - p1;
            unsigned char canmsg_data2[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            canmsg_data2[0] = int(sea_color1[0] * p1 + sea_color3[0] * p2);
            canmsg_data2[1] = int(sea_color1[1] * p1 + sea_color3[1] * p2);
            canmsg_data2[2] = int(sea_color1[2] * p1 + sea_color3[2] * p2);
            canmsg_data2[4] = sea_color3[0];
            canmsg_data2[5] = sea_color3[1];
            canmsg_data2[6] = sea_color3[2];
            memcpy(CanMsg.Data, canmsg_data2, sizeof(unsigned char) * 8);
            //CanMsg.Data= canmsg_data;
            sea_list3.push_back(CanMsg);
        }
        else
        {
            double p1 = (i - 10) / 9.0;
            double p2 = 1 - p1;
            unsigned char canmsg_data2[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            canmsg_data2[0] = sea_color1[0];
            canmsg_data2[1] = sea_color1[1];
            canmsg_data2[2] = sea_color1[2];
            canmsg_data2[4] = int(sea_color3[0] * p1 + sea_color1[0] * p2);
            canmsg_data2[5] = int(sea_color3[1] * p1 + sea_color1[1] * p2);
            canmsg_data2[6] = int(sea_color3[2] * p1 + sea_color1[2] * p2);
            memcpy(CanMsg.Data, canmsg_data2, sizeof(unsigned char) * 8);
            //CanMsg.Data= canmsg_data;
            sea_list3.push_back(CanMsg);
        }
    }

    std::vector<CAN_MSG> sea_list4;
    for (int i = 0; i < 20; i++)
    {
        CAN_MSG CanMsg;
        CanMsg.ExternFlag = 0;
        CanMsg.RemoteFlag = 0;
        CanMsg.DataLen = 8;
        CanMsg.ID = 0x10;
        if (i < 10)
        {
            double p1 = (9 - i) / 9.0;
            double p2 = 1 - p1;
            unsigned char canmsg_data3[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            canmsg_data3[0] = int(sea_color1[0] * p1 + sea_color2[0] * p2);
            canmsg_data3[1] = int(sea_color1[1] * p1 + sea_color2[1] * p2);
            canmsg_data3[2] = int(sea_color1[2] * p1 + sea_color2[2] * p2);
            canmsg_data3[4] = sea_color1[0];
            canmsg_data3[5] = sea_color1[1];
            canmsg_data3[6] = sea_color1[2];
            memcpy(CanMsg.Data, canmsg_data3, sizeof(unsigned char) * 8);
            //CanMsg.Data= canmsg_data;
            sea_list4.push_back(CanMsg);
        }
        else
        {
            double p1 = (i - 10) / 9.0;
            double p2 = 1 - p1;
            unsigned char canmsg_data2[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            canmsg_data2[0] = sea_color1[0];
            canmsg_data2[1] = sea_color1[1];
            canmsg_data2[2] = sea_color1[2];
            canmsg_data2[4] = int(sea_color3[0] * p1 + sea_color1[0] * p2);
            canmsg_data2[5] = int(sea_color3[1] * p1 + sea_color1[1] * p2);
            canmsg_data2[6] = int(sea_color3[2] * p1 + sea_color1[2] * p2);
            memcpy(CanMsg.Data, canmsg_data2, sizeof(unsigned char) * 8);
            //CanMsg.Data= canmsg_data;
            sea_list4.push_back(CanMsg);
        }
    }

    for (int i = 0; i < 14; i++) {
        CanMsg1.ID = lightid1[i];
        unsigned char canmsg_data3[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
        canmsg_data3[0] = sea_color3[0];
        canmsg_data3[1] = sea_color3[1];
        canmsg_data3[2] = sea_color3[2];
        canmsg_data3[4] = sea_color3[0];
        canmsg_data3[5] = sea_color3[1];
        canmsg_data3[6] = sea_color3[2];
        memcpy(CanMsg1.Data, canmsg_data3, sizeof(unsigned char) * 8);
        //CanMsg.Data= canmsg_data;
        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg1, 1);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    // 创建一个均匀分布的整数分布器，范围为1到12
    std::uniform_int_distribution<int> dis(5, 13);
    // 生成随机数并返回
    int r = dis(gen);

    for (int a = 0; a < 10; a += 2) {  //# 左沙滩变浪花，右沙滩
        CanMsg1.ID = lightid1[0];
        CanMsg1= sea_list1[a];
        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg1, 1);
        delay(sleep_time);
    }
    for (int b = 0; b < 10; b += 2) {  //# 左浪花，右沙滩变浪花
        CanMsg1.ID = lightid1[0];
        CanMsg1 = sea_list1[b+10];
        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg1, 1);
        delay(sleep_time);
    }

    for (int i = 0; i < r; i++) {
        if ((r - i) <= 2) {
            for (int j = 0; j < 10; j++)
            {
                CanMsg1.ID = lightid1[i];
                CanMsg2.ID = lightid1[i + 1];
                CanMsg1 = sea_list4[j];//  # 左浪花变大海，右浪花
                CanMsg2= sea_list1[j];// # 左沙滩变浪花，右沙滩
                CAN_SendMsg(DevHandle[0], CAN1, &CanMsg1, 1);//  # 发送指令
                CAN_SendMsg(DevHandle[0], CAN1, &CanMsg2, 1);//  # 发送指令
                delay(sleep_time*2*(3-r+i));
            }
            for (int j = 0; j < 10; j++)
            {
                CanMsg1.ID = lightid1[i];
                CanMsg2.ID = lightid1[i + 1];
                CanMsg1 = sea_list4[j + 10];  //# 左浪花变大海，右浪花
                CanMsg2 = sea_list1[j + 10];//左沙滩变浪花，右沙滩
                CAN_SendMsg(DevHandle[0], CAN1, &CanMsg1, 1);//  # 发送指令
                CAN_SendMsg(DevHandle[0], CAN1, &CanMsg2, 1);//  # 发送指令
                delay(sleep_time * 2 * (3 - r + i));
            }
        }
        else
        {
            for (int j = 0; j < 10; j += 2)
            {
                CanMsg1.ID = lightid1[i];
                CanMsg2.ID = lightid1[i + 1];
                CanMsg1 = sea_list4[j];//  # 左浪花变大海，右浪花
                CanMsg2 = sea_list1[j];// # 左沙滩变浪花，右沙滩
                CAN_SendMsg(DevHandle[0], CAN1, &CanMsg1, 1);//  # 发送指令
                CAN_SendMsg(DevHandle[0], CAN1, &CanMsg2, 1);//  # 发送指令
                delay(sleep_time);
            }
            for (int j = 0; j < 10; j += 2)
            {
                CanMsg1.ID = lightid1[i];
                CanMsg2.ID = lightid1[i + 1];
                CanMsg1 = sea_list4[j + 10];  //# 左浪花变大海，右浪花
                CanMsg2 = sea_list1[j + 10];//左沙滩变浪花，右沙滩
                CAN_SendMsg(DevHandle[0], CAN1, &CanMsg1, 1);//  # 发送指令
                CAN_SendMsg(DevHandle[0], CAN1, &CanMsg2, 1);//  # 发送指令
                delay(sleep_time * 2 * (3 - r + i));
            }
        }
    }
    delay(sleep_time * 8);

    for (int m = 0; m < r; m++) {
    }

}







int main(int argc, const char* argv[])
{

#if GET_FIRMWARE_INFO
    DEVICE_INFO DevInfo;
#endif

    //打开dll文件编译日期
    char dllBuildDateTime[64] = { 0 };
    DEV_GetDllBuildTime(dllBuildDateTime);
    printf("DLL Build Date Time = %s\n", dllBuildDateTime);
    //扫描查找设备
    ret = USB_ScanDevice(DevHandle);
    if (ret <= 0) {
        printf("No device connected!\n");
        return 0;
    }
    //打开设备
    state = USB_OpenDevice(DevHandle[0]);
    if (!state) {
        printf("Open device error!\n");
        return 0;
    }
#if GET_FIRMWARE_INFO
    char FunctionStr[256] = { 0 };
    //获取固件信息
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
    //初始化配置CAN,波特率为500000
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
    CANConfig.CAN_Mode = 1;//环回模式
#else
    CANConfig.CAN_Mode = 0x80;//正常模式
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
    //配置过滤器，必须配置，否则可能无法收到数据
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
    printf("开始发送!\n");

    //雨滴test
    int a;
    for (size_t i = 0; i < 9; i++)
    {
         a = light_rain_mode1(100, 255, 255, 255, 0xFE) ;
         if (a != 2)
         {
             printf("failed!\n");
             return;
         }
    }

    //涟漪test
    light_ripple_mode1(1,20);

    printf("发送完毕!\n");

    return 0;
}


//int SendMsg(int Color_R, int Color_G, int Color_B)
//{
//
//#if CAN_SEND_MSG//发送CAN帧
//
//    while (true)
//    {
//        CAN_MSG CanMsg[1];
//
//        for (int i = 0; i < 1; i++) {
//            CanMsg[i].ExternFlag = 0;
//            CanMsg[i].RemoteFlag = 0;
//            CanMsg[i].ID = 0x10;
//            CanMsg[i].DataLen = 8;
//            CanMsg[i].Data[0] = Color_R;
//            CanMsg[i].Data[1] = Color_G;
//            CanMsg[i].Data[2] = Color_B;
//            CanMsg[i].Data[3] = 0xFE;
//            CanMsg[i].Data[4] = 0x01;
//            CanMsg[i].Data[5] = 0x00;
//            CanMsg[i].Data[6] = 0x00;
//            CanMsg[i].Data[7] = 0x00;
//        }
//        CAN_SendMsg(DevHandle[0], SendCANIndex, CanMsg, 1);
//        Sleep(1000);
//
//        for (int i = 0; i < 1; i++) {
//            CanMsg[i].ExternFlag = 0;
//            CanMsg[i].RemoteFlag = 0;
//            CanMsg[i].ID = 0x10;
//            CanMsg[i].DataLen = 8;
//            CanMsg[i].Data[0] = 0x00;
//            CanMsg[i].Data[1] = 0x00;
//            CanMsg[i].Data[2] = 0x00;
//            CanMsg[i].Data[3] = 0xFE;
//            CanMsg[i].Data[4] = 0x01;
//            CanMsg[i].Data[5] = 0x00;
//            CanMsg[i].Data[6] = 0x00;
//            CanMsg[i].Data[7] = 0x00;
//        }
//        CAN_SendMsg(DevHandle[0], SendCANIndex, CanMsg, 1);
//        Sleep(1000);
//
//    }
//    return 0;
//
//#endif
//}


    /*
    for(int t=0;t<5;t++){
        int SendedNum = CAN_SendMsg(DevHandle[0],SendCANIndex,CanMsg,5);
        if(SendedNum >= 0){
            printf("Success send frames:%d\n",SendedNum);
        }else{
            printf("Send CAN data failed! %d\n",SendedNum);
        }
    }
    
#endif
#if CAN_GET_STATUS
    CAN_STATUS CANStatus;
    ret = CAN_GetStatus(DevHandle[0], SendCANIndex, &CANStatus);
    if (ret == CAN_SUCCESS) {
        printf("TSR = %08X\n", CANStatus.TSR);
        printf("ESR = %08X\n", CANStatus.ESR);
    }
    else {
        printf("Get CAN status error!\n");
    }
#endif
    //延时
#ifndef OS_UNIX
    Sleep(100);
#else
    usleep(100 * 1000);
#endif
#if CAN_GET_MSG
    CAN_MSG CanMsgBuffer[10240];
    int CanNum = CAN_GetMsg(DevHandle[0], ReadCANIndex, CanMsgBuffer);
    if (CanNum > 0) {
        printf("CanNum = %d\n", CanNum);
        for (int i = 0; i < CanNum; i++) {
            printf("CanMsg[%d].ID = %d\n", i, CanMsgBuffer[i].ID);
            printf("CanMsg[%d].TimeStamp = %d\n", i, CanMsgBuffer[i].TimeStamp);
            printf("CanMsg[%d].Data = ", i);
            for (int j = 0; j < CanMsgBuffer[i].DataLen; j++) {
                printf("%02X ", CanMsgBuffer[i].Data[j]);
            }
            printf("\n");
        }
    }
    else if (CanNum == 0) {
        printf("No CAN data!\n");
    }
    else {
        printf("Get CAN data error!\n");
    }
#endif
#if CAN_SCH
    CAN_MSG CanSchMsg[5];
    for (int i = 0; i < 5; i++) {
        CanSchMsg[i].ExternFlag = 0;
        CanSchMsg[i].RemoteFlag = 0;
        CanSchMsg[i].ID = (i << 4) | (i + 1);
        CanSchMsg[i].DataLen = 8;
        for (int j = 0; j < CanMsg[i].DataLen; j++) {
            CanSchMsg[i].Data[j] = j;
        }
        CanSchMsg[i].TimeStamp = 10;//帧间隔时间为10ms,必须设置，若其中的某一帧数据该项为0，则这帧发送完毕后不会继续进行后续的数据发送了
    }
    ret = CAN_StartSchedule(DevHandle[0], SendCANIndex, CanSchMsg, 5);
    if (ret == CAN_SUCCESS) {
        printf("Start CAN Schedule Success!\n");
    }
    else {
        printf("Start CAN Schedule Failed!\n");
    }
    Sleep(10000);
    CAN_StopSchedule(DevHandle[0], SendCANIndex);
#endif

    //关闭设备
    USB_CloseDevice(DevHandle[0]);
    //return 0;
}*/

