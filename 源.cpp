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
*/


int wind_mode(int len, int add[], int color1[], int color2[], int speed, int amp, int bias, bool mirror)
//氛围灯灯珠数量（一次控制两个灯珠，长度算1），灯珠对应的地址（两个灯珠为一个地址），
//灯带颜色，背景颜色，速度(可以对应帧数，加延迟时间)，幅度，偏差，是否镜像
{
    //1.根据speed判断时间
    double delay_time = 0.025;   //延迟时间
    int frame = 20;     //初步定义一个微风的动效为20帧，先写从左往右的风吹拂 
    int wind_stop = len - 1;     //振幅控制，风停止的位置

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
    //2.根据amp判断风吹的幅度 
    switch (amp)
    {
    case 0:
        wind_stop = len - len/8;
    case 1:
        wind_stop = len - len/5;
    case 2:
        wind_stop = len - len/10;
    }

    //1、灯珠显示默认背景颜色
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

    //2、效果实现
    int wind_len = len / 2 ;
    std::vector<CAN_MSG> can_list;     //存储风的不同渐变颜色, 背景色逐渐变为风的颜色，再逐渐变为背景色
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

 
    //风从左往右开始吹拂
    for (int j = 1; j <= frame; j++)       //从第一帧开始
    {
        float frame_stop = float(len) / float(frame) * j ;  //当前帧在灯带中的停止位置
        if (frame_stop > wind_stop)      //1.首先判断当前帧，是否超过风停止的位置，如果超过，则灯带写入背景色
        {
            for (int i = 0; i < len; i++) //    每一帧循环写入所有的灯珠
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
        else                   //当前帧 没过风停止的位置                         
        {
            if (frame_stop <= wind_len)     //2.判断当前帧的位置，是否超过风的长度，小于风的长度写入风的渐变色
            {
                for (int i = 0; i < len; i++) //    每一帧循环写入所有的灯珠
                {
                    if (i <= frame_stop)        //判断是否小于帧停止位置，当前灯珠的值，应为 can_list中存储的渐变颜色！
                    {
                        CanMsg = can_list[i];
                        CanMsg.ID = add[i];
                        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
                    }
                    else       //大于帧停止位置应为背景色
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
            else   //当前帧的位置超过了风的长度，则帧长度-风长度的位置应为背景色，大于帧长度的位置应为背景色
            {
                for (int i = 0; i < len; i++) //    每一帧循环写入所有的灯珠
                {
                    if (i >= (frame_stop - wind_len) && i <= frame_stop) //当前灯珠位置大于帧-风，且小于风
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

    //风从右往左开始吹拂
    int windstop_left = len - wind_stop;
    for (int j = 1; j <= frame; j++)       //从第一帧开始
    {
        float frame_stop = float(len) / float(frame)* (frame - j);  //当前帧在灯带中的停止位置
        
        if (frame_stop < windstop_left)     //首先判断frame停止位置   是否超过停止位置   frame_stop < wind_stop
        {
            for (int i = 0; i < len; i++) //    每一帧循环写入所有的灯珠,全部设置为背景色
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
        else      //frame停止位置  在 wind stop位置之前
        {
            if (len - frame_stop < wind_len)  //判断当前len - fram停止位置，是否小于风的长的
            {
                for (int i = 0; i < len; i++) //    每一帧循环写入所有的灯珠
                {
                    if (i >= frame_stop)        //判断是否大于帧停止位置，当前灯珠的值，应为 can_list中存储的渐变颜色！
                    {
                        CanMsg = can_list[i - frame_stop];
                        CanMsg.ID = add[i];
                        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
                    }
                    else       //小于帧停止位置应为背景色
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
            else    //当前len - 帧停止位置 大于风的长度
            {
                for (int i = 0; i < len; i++) //    每一帧循环写入所有的灯珠
                {
                    if (i >= frame_stop && i <= wind_len + frame_stop) //当前灯珠位置大于帧，且小于风+帧
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


    //3.恢复显示黑色
    

    return 1;
}


int boDong_mode(int len, int add[], int color1[], int color2[], int speed, int amp, int bias, bool mirror)
                //氛围灯灯珠数量（一次控制两个灯珠，长度算1），灯珠对应的地址（两个灯珠为一个地址），
                //灯带颜色，背景颜色，速度(可以对应帧数，加延迟时间)，幅度，偏差，是否镜像
{
    //1.根据speed判断时间
     //1.根据speed判断时间
    double delay_time = 0.025;   //延迟时间
    int frame = 20;     //初步定义一个微风的动效为20帧，先写从左往右的风吹拂 
    int short_long = len/4;     //振幅控制，短的波动长度
    int long_long = len / 2 - 1;       //振幅控制，长的波动长度

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
    //2.根据amp判断风吹的幅度 
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

    //1、灯珠显示默认背景颜色
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


    //2.效果实现
   
    std::vector<CAN_MSG> can_list1;     //存储短波动的渐变颜色
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

    std::vector<CAN_MSG> can_list2;     //存储长波动的渐变颜色
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

    //1.短波动的实现,由中间位置两边扩散到短波动位置
    for (int i = 0; i < frame; i++) 
    {
        int frameStop_right = 2 / len + short_long / frame * i;
        int frameStop_left = 2 / len -  short_long / frame * i;
        
        for(int i = 0; i < len; i++)  //写入所有的灯珠
        {
            if (i < frameStop_left || i > frameStop_right)   //如果灯珠不在当前帧的范围内，写入背景色
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

    //2.短波动的实现,由两边短波动位置扩散回中间位置
    for (int i = 0; i < frame; i++)
    {
        int frameStop_right = (len/2 +short_long) - short_long / frame * i;
        int frameStop_left =  (len/2 - short_long) + short_long / frame * i;

        if (i < frameStop_left || i > frameStop_right)   //如果灯珠不在当前帧的范围内，写入背景色
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

    //3.长波动的实现，由中间位置扩散的长波动的位置
    for (int i = 0; i < frame; i++)
    {
        int frameStop_right = 2 / len + long_long / frame * i;
        int frameStop_left = 2 / len - long_long / frame * i;

        for (int i = 0; i < len; i++)  //写入所有的灯珠
        {
            if (i < frameStop_left || i > frameStop_right)   //如果灯珠不在当前帧的范围内，写入背景色
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

    //4.长波动的实现，由两边长波动的位置扩散回中间位置
    for (int i = 0; i < frame; i++)
    {
        int frameStop_right = (len / 2 + long_long) - long_long / frame * i;
        int frameStop_left = (len / 2 - long_long) + long_long / frame * i;

        if (i < frameStop_left || i > frameStop_right)   //如果灯珠不在当前帧的范围内，写入背景色
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

    
    //3.恢复显示黑色



    return 1;
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


    //1.测试微风动效
    int color_1[3] = { 255, 0, 0 }; // # 灯带颜色
    int color_2[3] = { 255, 255, 0 }; // # 背景颜色
    int add[14] = { 0x301, 0x302, 0x303, 0x304, 0x305, 0x306, 0x307, 0x308, 0x309, 0x30A, 0x30B, 0x30C, 0x30D,0x30E };
    int len = 14;
    int speed = 3;
    int amp = 2;
    int bias = 0;
    bool mirror = false;


    int a = wind_mode(len, add, color_1, color_2, speed, amp, bias, mirror);
    
   
    

    printf("发送完毕!\n");

    return 0;
}

