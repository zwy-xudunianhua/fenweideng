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
              //��Χ�Ƶ���������һ�ο����������飬������1���������Ӧ�ĵ�ַ����������Ϊһ����ַ����
              //�ƴ���ɫ��������ɫ���ٶ�(���Զ�Ӧ֡�������ӳ�ʱ��)�����ȣ�ƫ��Ƿ���
{
    //1.����speed�ж�ʱ��
    double delay_time = 0.025;   //�ӳ�ʱ��
    int frame = 20;     //��������һ��΢��Ķ�ЧΪ20֡����д�������ҵķ紵�� 
    int wind_stop = len / 4 * 3;     //������ƣ���ֹͣ��λ��
   
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
    //2.����amp�жϷ紵�ķ���
    switch (amp)
    {
    case 0:
        wind_stop = len / 2;
    case 1:
        wind_stop = len / 4 * 3;
    case 2:
        wind_stop= len / 5 * 4;
    }

    //1��������ʾĬ�ϱ�����ɫ
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

    //2��Ч��ʵ��
    int wind_len = len / 4;
    std::vector<CAN_MSG> can_list;     //�洢��Ĳ�ͬ������ɫ, ����ɫ�𽥱�Ϊ�����ɫ�����𽥱�Ϊ����ɫ
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

    //��������ҿ�ʼ����
    for (int j = 1; j <= frame; j++)       //�ӵ�һ֡��ʼ
    {
        int frame_stop = (len / frame) * j;  //��ǰ֡�ڵƴ��е�ֹͣλ��
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
    for (int j = 1; j <= frame; j++)       //�ӵ�һ֡��ʼ
    {
        int frame_stop = (len / frame) * (frame - j);  //��ǰ֡�ڵƴ��е�ֹͣλ��
        wind_stop = len - wind_stop;
        if (frame_stop < wind_stop)     //�����ж�frameֹͣλ��   �Ƿ񳬹�ֹͣλ��   frame_stop < wind_stop
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
    return 1;
}




/*
int main() 
{

//---------------------------------------------------------------------------------------------------------------------
    //���ô򿪷�Χ�ƣ�

#if GET_FIRMWARE_INFO
    DEVICE_INFO DevInfo;
#endif

    //��dll�ļ���������
    char dllBuildDateTime[64] = { 0 };
    DEV_GetDllBuildTime(dllBuildDateTime);
    printf("DLL Build Date Time = %s\n", dllBuildDateTime);
    //ɨ������豸
    int ret = USB_ScanDevice(DevHandle);
    if (ret <= 0) {
        printf("No device connected!\n");
        return 0;
    }
    //���豸
    bool state = USB_OpenDevice(DevHandle[0]);
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
 
 //---------------------------------------------------------------------------------------------------------------------

    //2.����΢�綯Ч
    int color_1[3] = { 255, 211, 0 }; // # �ƴ���ɫ
    int color_2[3] = { 255, 0, 0 }; // # ������ɫ
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