#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "usb_device.h"
#include "usb2can.h"
#include <vector>
#include <random>
#include <time.h>
#include <math.h>


#pragma comment(lib,"Winmm.lib")
#define PI 3.14159265358979323846
#define GET_FIRMWARE_INFO   1
#define CAN_MODE_LOOPBACK   0
#define CAN_SEND_MSG        1
#define CAN_GET_MSG         1
#define CAN_GET_STATUS      0
#define CAN_SCH             0


int DevHandle[10];
int SendCANIndex = 0;//0-CAN1,1-CAN2
int ReadCANIndex = 1;//0-CAN1,1-CAN2
bool state;
int ret;


int lightid[14] = { 0x101, 0x102, 0x103, 0x104, 0x105, 0x106, 0x107, 0x108, 0x109, 0x10A, 0x10B, 0x10C, 0x10D,
0x10E };

   

int color_1[3] = { 255, 211, 0 }; //# 波动颜色 黄色
int color_2[3] = { 120, 3, 241 }; // # 背景颜色  配色3

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

//微风

int wind_mode(int len, int color1[], int color2[], int speed, int amplitude, int add[])
              //氛围灯灯珠数量， 灯带颜色，背景颜色，速度，幅度，add？
{
    //1.len的默认值为28  后续思考如何修改
    CAN_MSG CanMsg;
    CanMsg.ExternFlag = 0;
    CanMsg.RemoteFlag = 0;
    CanMsg.DataLen = 8;

    CAN_MSG CanMsg1;
    CanMsg1.ExternFlag = 0;
    CanMsg1.RemoteFlag = 0;
    CanMsg1.DataLen = 8;

    CanMsg.ID = 0x10;
    unsigned char canmsg_data[] = { 0x00, 0x00, 0x0, 0xFE, 0x00, 0x00, 0x00, 0xFE };
    memcpy(CanMsg.Data, canmsg_data, sizeof(unsigned char) * 8);
    CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);

    //2.设置背景颜色，并初始化
    canmsg_data[0] = color2[0];
    canmsg_data[1] = color2[1];
    canmsg_data[2] = color2[2];
    canmsg_data[4] = color2[0];
    canmsg_data[5] = color2[1];
    canmsg_data[6] = color2[2];
    memcpy(CanMsg.Data, canmsg_data, sizeof(unsigned char) * 8);
    CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
    for (int i = 0; i < 14; i++) {
        CanMsg.ID = lightid[i];
        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
    }
    delay(0.02);

    //3.设计流水动效模式
    std::vector<CAN_MSG> can_list1;

    for (int i = 0; i < 20; i++) 
    {
        unsigned char canmsg_data1[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
        if (i < 10)   //左背景变为流水，右边为背景
        {
            int p1 = (9 - i) / 9;
            int p2 = 1 - p1;
            canmsg_data1[0] = int(color2[0] * p1 + color1[0] * p2);
            canmsg_data1[1] = int(color2[1] * p1 + color1[1] * p2);
            canmsg_data1[2] = int(color2[2] * p1 + color1[2] * p2);
            canmsg_data1[4] = color2[0];
            canmsg_data1[5] = color2[1];
            canmsg_data1[6] = color2[2];
            memcpy(CanMsg.Data, canmsg_data1, sizeof(unsigned char) * 8);
            //CanMsg.ID = lightid[i];
           //CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            can_list1.push_back(CanMsg);
        }
        else      ////左流水变为背景，右流水
        {
            int p1 = (i - 10) / 9;
            int p2 = 1 - p1;
            canmsg_data1[0] = int(color2[0] * p1 + color1[0] * p2);
            canmsg_data1[1] = int(color2[1] * p1 + color1[1] * p2);
            canmsg_data1[2] = int(color2[2] * p1 + color1[2] * p2);
            canmsg_data1[4] = color1[0];
            canmsg_data1[5] = color1[1];
            canmsg_data1[6] = color1[2];
            memcpy(CanMsg.Data, canmsg_data1, sizeof(unsigned char) * 8);
            //CanMsg.ID = lightid[i];
            //CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            can_list1.push_back(CanMsg);
        }
    }
    std::vector<CAN_MSG> can_list2;
    for (int i = 0; i < 20; i++)
    {
        unsigned char canmsg_data2[] = { 0x00, 0x00, 0x0, 0xFE, 0x00, 0x00, 0x00, 0xFE };
        if (i < 10)   //左背景，右背景变为流水
        {
            int p1 = (9 - i) / 9;
            int p2 = 1 - p1;
            canmsg_data2[0] = color2[0];
            canmsg_data2[1] = color2[1];
            canmsg_data2[2] = color2[2];
            canmsg_data2[4] = int(color2[0] * p1 + color1[0] * p2);
            canmsg_data2[5] = int(color2[1] * p1 + color1[1] * p2);
            canmsg_data2[6] = int(color2[2] * p1 + color1[2] * p2);
            memcpy(CanMsg.Data, canmsg_data2, sizeof(unsigned char) * 8);
            //CanMsg.ID = lightid[i];
           //CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            can_list2.push_back(CanMsg);
        }
        else    //左流水，右流水变为背景
        {
            int p1 = (i - 10) / 9;
            int p2 = 1 - p1;
            canmsg_data2[0] = color1[0];
            canmsg_data2[1] = color1[1];
            canmsg_data2[2] = color1[2];
            canmsg_data2[4] = int(color2[0] * p1 + color1[0] * p2);
            canmsg_data2[5] = int(color2[1] * p1 + color1[1] * p2);;
            canmsg_data2[6] = int(color2[2] * p1 + color1[2] * p2);;
            memcpy(CanMsg.Data, canmsg_data2, sizeof(unsigned char) * 8);
            //CanMsg.ID = lightid[i];
            //CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            can_list2.push_back(CanMsg);
        }
    }
    std::vector<CAN_MSG> can_list3;
    for (int i = 0; i < 20; i++)
    {
        unsigned char canmsg_data3[] = { 0x00, 0x00, 0x0, 0xFE, 0x00, 0x00, 0x00, 0xFE };
        if (i < 10)           //左背景，右流水变为背景
        {
            int p1 = (9 - i) / 9;
            int p2 = 1 - p1;
            canmsg_data3[0] = color2[0];
            canmsg_data3[1] = color2[1];
            canmsg_data3[2] = color2[2];
            canmsg_data3[4] = int(color2[0] * p2 + color1[0] * p1);
            canmsg_data3[5] = int(color2[1] * p2 + color1[1] * p1);
            canmsg_data3[6] = int(color2[2] * p2 + color1[2] * p1);
            memcpy(CanMsg.Data, canmsg_data3, sizeof(unsigned char) * 8);
            //CanMsg.ID = lightid[i];
           //CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            can_list3.push_back(CanMsg);
        }
        else        //左流水，右背景变为流水
        {
            int p1 = (i - 10) / 9;
            int p2 = 1 - p1;
            canmsg_data3[0] = color1[0];
            canmsg_data3[1] = color1[1];
            canmsg_data3[2] = color1[2];
            canmsg_data3[4] = int(color2[0] * p2 + color1[0] * p1);
            canmsg_data3[5] = int(color2[1] * p2 + color1[1] * p1);;
            canmsg_data3[6] = int(color2[2] * p2 + color1[2] * p1);;
            memcpy(CanMsg.Data, canmsg_data3, sizeof(unsigned char) * 8);
            //CanMsg.ID = lightid[i];
            //CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            can_list3.push_back(CanMsg);
        }
    }

    std::vector<CAN_MSG> can_list4;
    for (int i = 0; i < 20; i++)
    {
        unsigned char canmsg_data4[] = { 0x00, 0x00, 0x0, 0xFE, 0x00, 0x00, 0x00, 0xFE };
        if (i < 5)           //背景变流水
        {
            int p1 = (i + 1) / 5;
            int p2 = 1 - p1;
            canmsg_data4[0] = int(color2[0] * p2 + color1[0] * p1);
            canmsg_data4[1] = int(color2[1] * p2 + color1[1] * p1);
            canmsg_data4[2] = int(color2[2] * p2 + color1[2] * p1);
            canmsg_data4[4] = int(color2[0] * p2 + color1[0] * p1);
            canmsg_data4[5] = int(color2[1] * p2 + color1[1] * p1);
            canmsg_data4[6] = int(color2[0] * p2 + color1[0] * p1);
            memcpy(CanMsg.Data, canmsg_data4, sizeof(unsigned char) * 8);
            //CanMsg.ID = lightid[i];
           //CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            can_list3.push_back(CanMsg);
        }
        else        //左流水，右背景变为流水
        {
            int p1 = (10 - i) / 5;
            int p2 = 1 - p1;
            canmsg_data4[0] = int(color2[0] * p2 + color1[0] * p1);
            canmsg_data4[1] = int(color2[1] * p2 + color1[1] * p1);
            canmsg_data4[2] = int(color2[2] * p2 + color1[2] * p1);;
            canmsg_data4[4] = int(color2[0] * p2 + color1[0] * p1);
            canmsg_data4[5] = int(color2[1] * p2 + color1[1] * p1);;
            canmsg_data4[6] = int(color2[2] * p2 + color1[2] * p1);;
            memcpy(CanMsg.Data, canmsg_data4, sizeof(unsigned char) * 8);
            //CanMsg.ID = lightid[i];
            
            can_list4.push_back(CanMsg);
        }
    }

    unsigned char canmsg_data5[] = { 0x00, 0x00, 0x0, 0xFE, 0x00, 0x00, 0x00, 0xFE };
    canmsg_data5[0] = color2[0];
    canmsg_data5[1] = color2[1];
    canmsg_data5[2] = color2[2];
    canmsg_data5[4] = color2[0];
    canmsg_data5[5] = color2[1];
    canmsg_data5[6] = color2[2];
    memcpy(CanMsg1.Data, canmsg_data5, sizeof(unsigned char) * 8);
    
    for (int i = 0; i < 14; i++)      //循环便利所有灯珠,开始全部设置为背景色
    {
        CanMsg1.ID = lightid[i];   
        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
    }
    if (wind_num == 0)  //windnum 表示的是什么？
    {
        for (int j = 2; j <= 12; j++)
        {
            CanMsg1 = can_list4[j - 2];
            CanMsg1.ID = lightid[j];
            CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            CanMsg1 = can_list2[0];
            CanMsg1.ID = lightid[j];
            CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            delay(0.02);
        }
        CanMsg1 = can_list2[0];
        CanMsg1.ID = lightid[11];
        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
    }
    else
    {
        for (int j = 2; j <= 12; j++)
        {
            CanMsg1 = can_list4[j-2];
            CanMsg1.ID = lightid[13 - j];
            CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            CanMsg1 = can_list2[0];
            CanMsg1.ID = lightid[13 - j + 1];
            CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
            delay(0.02);
        }
        CanMsg1 = can_list2[0];
        CanMsg1.ID = lightid[2];
        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
  
    }
    

}


//微风1
int wind_model(int len, int color1[], int color2[], int speed, int amplitude, int add[])
{
    //1.len的默认值为28  后续思考如何修改
    CAN_MSG CanMsg;
    CanMsg.ExternFlag = 0;
    CanMsg.RemoteFlag = 0;
    CanMsg.DataLen = 8;
    
    CanMsg.ID = 0x10;
    unsigned char canmsg_data[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    memcpy(CanMsg.Data, canmsg_data, sizeof(unsigned char) * 8);
    CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
    
    //2.设置背景颜色，并初始化
    canmsg_data[0] = color2[0];
    canmsg_data[1] = color2[1];
    canmsg_data[2] = color2[2];
    canmsg_data[4] = color2[0];
    canmsg_data[5] = color2[1];
    canmsg_data[6] = color2[2];
    memcpy(CanMsg.Data, canmsg_data, sizeof(unsigned char) * 8);
    CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
    for (int i = 0; i < 14; i++) {
        CanMsg.ID = lightid[i];
        CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
    }
    delay(0.02);

    //3.微风实现
    float rand_num = rand() / (RAND_MAX + 1.0);
    int pos = rand_num * 23 + 2; //风出现在哪里
    int wind_direct = 1;         //风吹的方向
    int wind_k = 0;              //风力系数
    int wind_t = 0;              //持续时间
    
    if (pos < 13.5)
    {
        wind_direct = 1;
    }
    else 
    {
        wind_direct = -1;        
    }

    while (pos >= -1 && pos <= 29)
    {
        int color_original[28] = { 0 };
        if (int(pos / 1) - 1 >= 0 && int(pos / 1) - 1 <= 27)
        {
            color_original[int(pos / 1 - 1)] = int(pos / 1) - (pos - 1);
        }
        if (int(pos / 1) >= 0 && int(pos / 1 <= 27))
        {
            color_original[int(pos/1)] = 1;
        }
        if (int(pos / 1) + 1 >= 0 && int(pos / 1) + 1 <= 27)
        {
            color_original[int(pos / 1) + 1] = pos + 1 - int(pos / 1 + 1);
        }
        int color[28] = { 0 };

        for (int i = 0; i < sizeof(color_original); i++)
        {
            color[i] = wind_k * color_original[i];
        }

        for (int i = 0; i < 14; i++) //循环数组大小，控制所有灯珠
        {
            canmsg_data[0] = int(color[2 * i] * color1[0] + (1 - color[2 * i]) * color2[0]);
            canmsg_data[1] = int(color[2 * i] * color1[1] + (1 - color[2 * i]) * color2[1]);
            canmsg_data[2] = int(color[2 * i] * color1[2] + (1 - color[2 * i]) * color2[2]);
            canmsg_data[4] = int(color[2 * i + 1] * color1[0] + (1 - color[2 * i] + 1) * color2[0]);
            canmsg_data[5] = int(color[2 * i + 1] * color1[1] + (1 - color[2 * i] + 1) * color2[1]);
            canmsg_data[6] = int(color[2 * i + 1] * color1[2] + (1 - color[2 * i] + 1) * color2[2]);
            memcpy(CanMsg.Data, canmsg_data, sizeof(unsigned char) * 8);
            CanMsg.ID = lightid[i];
            CAN_SendMsg(DevHandle[0], CAN1, &CanMsg, 1);
        }

        pos += wind_direct * 0.1;
        wind_k = cos(wind_t * PI / 80) / 2 + 0.5;
        wind_t += 1;
        delay(0.02);
    }
}


   //流火！！

int wind_model(int len, int color1[], int color2[], int speed, int amplitude, int add[])
{


}



//int main() 
//{
//    CAN_MSG CanMsg;
//    CanMsg.ExternFlag = 0; // 是否是扩展帧
//    CanMsg.RemoteFlag = 0; // 是否是远程帧
//    CanMsg.DataLen = 8; //数据长度(<= 8)，即Data 的长度。
//
//    int color1[4] = { 160, 175, 40,12 };
//    int color2[4] = { 0, 0, 0,12 };
//
//
//}