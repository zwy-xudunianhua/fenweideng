#include <iostream>
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

int CAN1 = 0;
int DevHandle[10];
int SendCANIndex = 0;//0-CAN1,1-CAN2
int ReadCANIndex = 1;//0-CAN1,1-CAN2



int wind_mode(int len,int add[], int color1[], int color2[], int speed, int amp, int bias, bool mirror)
              //氛围灯灯珠数量（一次控制两个灯珠，长度算1），灯珠对应的地址（两个灯珠为一个地址），
              //灯带颜色，背景颜色，速度(可以对应帧数，加延迟时间)，幅度，偏差，是否镜像
{
    //1.根据speed判断时间
    double delay_time = 0.025;   //延迟时间
    int frame = 20;     //初步定义一个微风的动效为20帧，先写从左往右的风吹拂 
    int wind_stop = len / 4 * 3;     //振幅控制，风停止的位置
   
    switch (speed) 
    {
    case 0 :
        delay_time = 0.1;
        frame = 10;
    case 1:
        delay_time = 0.075;
        frame = 15;
    case 2:
        delay_time = 0.050;
        frame = 20;
    case 3:
        delay_time = 0.025;
        frame = 25;
    }
    //2.根据amp判断风吹的幅度
    switch (amp)
    {
    case 0:
        wind_stop = len / 2;
    case 1:
        wind_stop = len / 4 * 3;
    case 2:
        wind_stop= len / 5 * 4;
    }

    //1、灯珠显示默认背景颜色
    CAN_MSG CanMsg;
    CanMsg.ExternFlag = 0;
    CanMsg.RemoteFlag = 0;
    CanMsg.DataLen = 8;
    unsigned char canmsg_data[] = { 0x0FE, 0x00, 0xFE, 0xFE, 0xFE, 0xFE, 0x00, 0xFE };
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
    int wind_len = len / 4;
    std::vector<CAN_MSG> can_list;     //存储风的不同渐变颜色, 背景色逐渐变为风的颜色，再逐渐变为背景色
    for (int i = 0; i < wind_len; i++) 
    {
        if (i < wind_len / 2) 
        {
            unsigned char canmsg_data1[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            int p1 = (wind_len / 2 - i) / (wind_len / 2);
            int p2 = 1 - p1;
            canmsg_data1[0] = int(p1 * color2[0] + p2 * color1[0]);
            canmsg_data1[1] = int(p1 * color2[1] + p2 * color1[1]);
            canmsg_data1[2] = int(p1 * color2[2] + p2 * color1[2]);
            canmsg_data1[4] = int(p1 * color2[0] + p2 * color1[0]);
            canmsg_data1[5] = int(p1 * color2[1] + p2 * color1[1]);
            canmsg_data1[6] = int(p1 * color2[2] + p2 * color1[2]);
            memcpy(CanMsg.Data, canmsg_data1, sizeof(unsigned char) * 8);
            can_list.push_back(CanMsg);
        }
        else
        {
            unsigned char canmsg_data1[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
            int p1 = (wind_len / 2 - i) / (wind_len / 2);
            int p2 = 1 - p1;
            canmsg_data1[0] = int(p1 * color1[0] + p2 * color2[0]);
            canmsg_data1[1] = int(p1 * color1[1] + p2 * color2[1]);
            canmsg_data1[2] = int(p1 * color1[2] + p2 * color2[2]);
            canmsg_data1[4] = int(p1 * color1[0] + p2 * color2[0]);
            canmsg_data1[5] = int(p1 * color1[1] + p2 * color2[1]);
            canmsg_data1[6] = int(p1 * color1[2] + p2 * color2[2]);
            memcpy(CanMsg.Data, canmsg_data1, sizeof(unsigned char) * 8);
            can_list.push_back(CanMsg);
        }                
    }

    
    //unsigned char canmsg_data[] = { 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE };
    canmsg_data[0] = color2[0];
    canmsg_data[1] = color2[1];
    canmsg_data[2] = color2[2];
    canmsg_data[4] = color2[0];
    canmsg_data[5] = color2[1];
    canmsg_data[6] = color2[2];

    //风从左往右开始吹拂
    for (int j = 1; j <= frame; j++)       //从第一帧开始
    {
        int frame_stop = (len / frame) * j;  //当前帧在灯带中的停止位置
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
    for (int j = 1; j <= frame; j++)       //从第一帧开始
    {
        int frame_stop = (len / frame) * (frame - j);  //当前帧在灯带中的停止位置
        wind_stop = len - wind_stop;
        if (frame_stop < wind_stop)     //首先判断frame停止位置   是否超过停止位置   frame_stop < wind_stop
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
    return 1;
}




/*
int main() 
{

//---------------------------------------------------------------------------------------------------------------------
    //设置打开氛围灯！

#if GET_FIRMWARE_INFO
    DEVICE_INFO DevInfo;
#endif

    //打开dll文件编译日期
    char dllBuildDateTime[64] = { 0 };
    DEV_GetDllBuildTime(dllBuildDateTime);
    printf("DLL Build Date Time = %s\n", dllBuildDateTime);
    //扫描查找设备
    int ret = USB_ScanDevice(DevHandle);
    if (ret <= 0) {
        printf("No device connected!\n");
        return 0;
    }
    //打开设备
    bool state = USB_OpenDevice(DevHandle[0]);
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
 
 //---------------------------------------------------------------------------------------------------------------------

    //2.测试微风动效
    int color_1[3] = { 255, 211, 0 }; // # 灯带颜色
    int color_2[3] = { 255, 0, 0 }; // # 背景颜色
    int add[14] = { 0x101, 0x102, 0x103, 0x104, 0x105, 0x106, 0x107, 0x108, 0x109, 0x10A, 0x10B, 0x10C, 0x10D,0x10E };
    int len = 14;
    int speed = 3;
    int amp = 2;
    int bias = 0;
    bool mirror = false;

    
    int a = wind_mode(len, add, color_1, color_2, speed, amp, bias, mirror);
    std::cout << a;
    
    
	return 0;
}

*/