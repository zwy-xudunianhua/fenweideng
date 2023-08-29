#pragma once
/**
  ******************************************************************************
  * @file    usb2can.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   usb2can��غ������������Ͷ���.
  ******************************************************************************
  * @attention
  *
  *<center><a href="http:\\www.toomoss.com">http://www.toomoss.com</a></center>
  *<center>All Rights Reserved</center></h3>
  *
  ******************************************************************************
  */
#ifndef __USB2CAN_H_
#define __USB2CAN_H_

#include <stdint.h>
#include "offline_type.h"
#ifndef OS_UNIX
#include <Windows.h>
#else
#include <unistd.h>
#ifndef WINAPI
#define WINAPI
#endif
#endif

  //1.CAN��Ϣ֡���������Ͷ���
typedef  struct  _CAN_MSG
{
    unsigned int    ID;           //����ID��
    unsigned int    TimeStamp;    //���յ���Ϣ֡ʱ��ʱ���ʶ����CAN ��������ʼ����ʼ��ʱ����λΪ100us
    unsigned char   RemoteFlag;   //bit[0]-�Ƿ���Զ��֡,bit[6..5]-��ǰ֡ͨ���ţ�bit[7]-����֡��־
    unsigned char   ExternFlag;   //�Ƿ�����չ֡
    unsigned char   DataLen;      //���ݳ���(<=8)����Data �ĳ��ȡ�
    unsigned char   Data[8];      //���ĵ����ݡ�
    unsigned char   TimeStampHigh;//ʱ�����λ
}CAN_MSG, * PCAN_MSG;

//2.��ʼ��CAN���������Ͷ���
typedef struct _CAN_INIT_CONFIG
{  
    //CAN������ = 100MHz/(CAN_BRP)/(CAN_SJW+CAN_BS1+CAN_BS2)
    unsigned int    CAN_BRP;    //ȡֵ��Χ1~1024
    unsigned char   CAN_SJW;    //ȡֵ��Χ1~4
    unsigned char   CAN_BS1;    //ȡֵ��Χ1~16
    unsigned char   CAN_BS2;    //ȡֵ��Χ1~8
    unsigned char   CAN_Mode;   //CAN����ģʽ��0-����ģʽ��1-����ģʽ��2-��Ĭģʽ��3-��Ĭ����ģʽ��bit7Ϊ1������������ڲ��ն˵��裬���򲻽���
    unsigned char   CAN_ABOM;   //�Զ����߹���0-��ֹ��1-ʹ��
    unsigned char   CAN_NART;   //�����ط�����0-ʹ�ܱ����ش���1-��ֹ�����ش�
    unsigned char   CAN_RFLM;   //FIFO��������0-�±��ĸ��Ǿɱ��ģ�1-�����±���
    unsigned char   CAN_TXFP;   //�������ȼ�����0-��ʶ��������1-��������˳�����
}CAN_INIT_CONFIG, * PCAN_INIT_CONFIG;


//3.CAN �˲��������������Ͷ���
typedef struct _CAN_FILTER_CONFIG {
    unsigned char   Enable;         //ʹ�ܸù�������1-ʹ�ܣ�0-��ֹ
    unsigned char   FilterIndex;    //�����������ţ�ȡֵ��ΧΪ0��13
    unsigned char   FilterMode;     //������ģʽ��0-����λģʽ��1-��ʶ���б�ģʽ
    unsigned char   ExtFrame;       //���˵�֡���ͱ�־��Ϊ1 ����Ҫ���˵�Ϊ��չ֡��Ϊ0 ����Ҫ���˵�Ϊ��׼֡��
    unsigned int    ID_Std_Ext;     //������ID
    unsigned int    ID_IDE;         //������IDE
    unsigned int    ID_RTR;         //������RTR
    unsigned int    MASK_Std_Ext;   //������ID������ֻ���ڹ�����ģʽΪ����λģʽʱ����
    unsigned int    MASK_IDE;       //������IDE������ֻ���ڹ�����ģʽΪ����λģʽʱ����
    unsigned int    MASK_RTR;       //������RTR������ֻ���ڹ�����ģʽΪ����λģʽʱ����
} CAN_FILTER_CONFIG, * PCAN_FILTER_CONFIG;
//4.CAN����״̬�������Ͷ���
typedef struct _CAN_STATUS {
    unsigned int    TSR;
    unsigned int    ESR;
    unsigned char   RECounter;  //CAN ���������մ���Ĵ�����
    unsigned char   TECounter;  //CAN ���������ʹ���Ĵ�����
    unsigned char   LECode;     //���Ĵ������
}CAN_STATUS, * PCAN_STATUS;
//5.����CAN Bootloader�����б�
typedef  struct  _CBL_CMD_LIST {
    //Bootloader�������
    unsigned char   Erase;          //����APP������������
    unsigned char   WriteInfo;      //���ö��ֽ�д������ز�����д��ʼ��ַ����������
    unsigned char   Write;          //�Զ��ֽ���ʽд����
    unsigned char   Check;          //���ڵ��Ƿ����ߣ�ͬʱ���ع̼���Ϣ
    unsigned char   SetBaudRate;    //���ýڵ㲨����
    unsigned char   Excute;         //ִ�й̼�
    //�ڵ㷵��״̬
    unsigned char    CmdSuccess;      //����ִ�гɹ�
    unsigned char    CmdFaild;        //����ִ��ʧ��
} CBL_CMD_LIST, * PCBL_CMD_LIST;

//6.�������ش�����붨��
#define CAN_SUCCESS             (0)   //����ִ�гɹ�
#define CAN_ERR_NOT_SUPPORT     (-1)  //��������֧�ָú���
#define CAN_ERR_USB_WRITE_FAIL  (-2)  //USBд����ʧ��
#define CAN_ERR_USB_READ_FAIL   (-3)  //USB������ʧ��
#define CAN_ERR_CMD_FAIL        (-4)  //����ִ��ʧ��
#define CAN_BL_ERR_CONFIG       (-20) //�����豸����
#define CAN_BL_ERR_SEND         (-21) //�������ݳ���
#define CAN_BL_ERR_TIME_OUT     (-22) //��ʱ����
#define CAN_BL_ERR_CMD          (-23) //ִ������ʧ��

#define CAN_BOOT_ERR_CONFIG       (-30) //�����豸����
#define CAN_BOOT_ERR_SEND         (-31) //�������ݳ���
#define CAN_BOOT_ERR_TIME_OUT     (-32) //��ʱ����
#define CAN_BOOT_ERR_CMD          (-33) //ִ������ʧ��
#define CAN_BOOT_ERR_BAUD         (-34) //�����ʲ����Զ���ȡʧ��
#define CAN_BOOT_ERR_BUFFER       (-35) //���豸���ؽ������ݻ�������СΪ0
//7.CAN Bootloader�̼�����
#define CAN_BL_BOOT     0x55555555
#define CAN_BL_APP      0xAAAAAAAA
//8.�м�ģʽ����
#define CAN_RELAY_NONE      0x00    //�ر��м̹���
#define CAN_RELAY_CAN1TO2   0x01    //CAN1 --> CAN2 CAN1�յ����ݺ�ͨ��CAN2ת����ȥ
#define CAN_RELAY_CAN2TO1   0x10    //CAN2 --> CAN1 CAN2�յ����ݺ�ͨ��CAN1ת����ȥ
#define CAN_RELAY_CANALL    0x11    //CAN1 <-> CAN2 CAN1�յ����ݺ�ͨ��CAN2ת����ȥ,CAN2�յ����ݺ�ͨ��CAN1ת����ȥ
#define CAN_RELAY_ONLINE    0x88	//�����м����ݽ�������ת������Ҫ����CAN_SetOnlineRelayData����
#ifdef __cplusplus
extern "C"
{
#endif
    int WINAPI CAN_GetCANSpeedArg(int DevHandle, CAN_INIT_CONFIG* pCanConfig, unsigned int SpeedBps);
    int WINAPI CAN_Init(int DevHandle, unsigned char CANIndex, CAN_INIT_CONFIG* pCanConfig);
    int WINAPI CAN_Filter_Init(int DevHandle, unsigned char CANIndex, CAN_FILTER_CONFIG* pFilterConfig);
    int WINAPI CAN_StartGetMsg(int DevHandle, unsigned char CANIndex);
    int WINAPI CAN_StopGetMsg(int DevHandle, unsigned char CANIndex);
    int WINAPI CAN_SendMsg(int DevHandle, unsigned char CANIndex, CAN_MSG* pCanSendMsg, unsigned int SendMsgNum);
    int WINAPI CAN_GetMsg(int DevHandle, unsigned char CANIndex, CAN_MSG* pCanGetMsg);
    int WINAPI CAN_GetMsgWithSize(int DevHandle, unsigned char CANIndex, CAN_MSG* pCanGetMsg, int BufferSize);
    int WINAPI CAN_ClearMsg(int DevHandle, unsigned char CANIndex);
    int WINAPI CAN_GetStatus(int DevHandle, unsigned char CANIndex, PCAN_STATUS pCANStatus);
    //�Ե��ȱ�ķ�ʽ�������ݣ����Ծ�ȷ�Ŀ���ÿ֮֡��ļ��ʱ��
    /**
      * @brief  ����CAN���ȱ�����
      * @param  DevHandle �豸������
      * @param  CANIndex CANͨ���ţ�ȡֵ0����1
      * @param  pCanMsgTab CAN���ȱ��б��׵�ַ
      * @param  pMsgNum ���ȱ��б���ÿ�����ȱ������Ϣ֡��
      * @param  pSendTimes ÿ�����ȱ�����֡���ʹ�������Ϊ0xFFFF����ѭ�����ͣ�ͨ������CAN_StopSchedule����ֹͣ����
      * @param  MsgTabNum ���ȱ���
      * @retval ����ִ��״̬��С��0����ִ�г���
      */
    int WINAPI CAN_SetSchedule(int DevHandle, unsigned char CANIndex, PCAN_MSG pCanMsgTab, unsigned char* pMsgNum, unsigned short* pSendTimes, unsigned char MsgTabNum);
    /**
      * @brief  �������ȱ�
      * @param  DevHandle �豸������
      * @param  CANIndex CANͨ���ţ�ȡֵ0����1
      * @param  MsgTabIndex CAN���ȱ�������
      * @param  TimePrecMs ���ȱ�ʱ�侫�ȣ�������ȱ�������С֡����Ϊ10ms����ô�ͽ�������Ϊ10
      * @param  OrderSend ����Ϊ1��˳���͵��ȱ������֡������Ϊ0���з��͵��ȱ������֡
      * @retval ����ִ��״̬��С��0����ִ�г���
      */
    int WINAPI CAN_StartSchedule(int DevHandle, unsigned char CANIndex, unsigned char MsgTabIndex, unsigned char TimePrecMs, unsigned char OrderSend);
    /**
      * @brief  ֹͣ���ȷ�ʽ��������
      * @param  DevHandle �豸������
      * @param  CANIndex CANͨ���ţ�ȡֵ0����1
      * @param  MsgTabIndex CAN���ȱ�������
      * @retval ����ִ��״̬��С��0����ִ�г���
      */
    int WINAPI CAN_StopSchedule(int DevHandle, unsigned char CANIndex);


    //CAN�м���غ���
    int WINAPI CAN_SetRelayData(int DevHandle, CAN_RELAY_HEAD* pCANRelayHead, CAN_RELAY_DATA* pCANRelayData);
    int WINAPI CAN_GetRelayData(int DevHandle, CAN_RELAY_HEAD* pCANRelayHead, CAN_RELAY_DATA* pCANRelayData);
    int WINAPI CAN_SetRelay(int DevHandle, unsigned char RelayState);
    //�ϰ汾�ӿں�������������ʹ��
    int WINAPI CAN_BL_Init(int DevHandle, int CANIndex, CAN_INIT_CONFIG* pInitConfig, CBL_CMD_LIST* pCmdList);
    int WINAPI CAN_BL_NodeCheck(int DevHandle, int CANIndex, unsigned short NodeAddr, unsigned int* pVersion, unsigned int* pType, unsigned int TimeOut);
    int WINAPI CAN_BL_Erase(int DevHandle, int CANIndex, unsigned short NodeAddr, unsigned int FlashSize, unsigned int TimeOut);
    int WINAPI CAN_BL_Write(int DevHandle, int CANIndex, unsigned short NodeAddr, unsigned int AddrOffset, unsigned char* pData, unsigned int DataNum, unsigned int TimeOut);
    int WINAPI CAN_BL_Excute(int DevHandle, int CANIndex, unsigned short NodeAddr, unsigned int Type);
    int WINAPI CAN_BL_SetNewBaudRate(int DevHandle, int CANIndex, unsigned short NodeAddr, CAN_INIT_CONFIG* pInitConfig, unsigned int NewBaudRate, unsigned int TimeOut);

    int WINAPI  CAN_DecodeListFile(char* pFileName, unsigned int* pIgnoreIDList, int IgnoreIDListLen);
    int WINAPI  CAN_GetListFileMsg(int MsgIndex, int MsgLen, CAN_MSG* pCANMsg);

#ifdef __cplusplus
}
#endif
#endif
