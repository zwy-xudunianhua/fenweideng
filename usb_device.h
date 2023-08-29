#pragma once
/**
  ******************************************************************************
  * @file    usb_device.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   �豸������غ������������Ͷ���.
  ******************************************************************************
  * @attention
  *
  *<center><a href="http:\\www.toomoss.com">http://www.toomoss.com</a></center>
  *<center>All Rights Reserved</center></h3>
  *
  ******************************************************************************
  */
#ifndef __USB_DEVICE_H_
#define __USB_DEVICE_H_

#include <stdint.h>
#ifndef OS_UNIX
#include <Windows.h>
#else
#include <unistd.h>
#ifndef WINAPI
#define WINAPI
#endif
#endif
  //�����豸��Ϣ
typedef struct _DEVICE_INFO
{
    char    FirmwareName[32];   //�̼������ַ���
    char    BuildDate[32];      //�̼�����ʱ���ַ���
    int     HardwareVersion;    //Ӳ���汾��
    int     FirmwareVersion;    //�̼��汾��
    int     SerialNumber[3];    //���������к�
    int     Functions;          //��������ǰ�߱��Ĺ���
}DEVICE_INFO;
//Ӳ����Ϣ
typedef struct
{
    unsigned char   McuModel[16];       ///<��оƬ�ͺ�
    unsigned char   ProductModel[16];   ///<��Ʒ�ͺ�
    unsigned int    Version;            ///<Ӳ���汾��

    unsigned char   CANChannelNum;      ///<CANͨ����
    unsigned char   LINChannelNum;      ///<LINͨ����
    unsigned char   PWMChannelNum;      ///<PWMͨ����������LIN��DO�������
    unsigned char   HaveCANFD;          ///<�Ƿ�֧��CANFD����

    unsigned char   DIChannelNum;       ///<����DIͨ������������LINͨ��
    unsigned char   DOChannelNum;       ///<����DOͨ������������LINͨ��
    unsigned char   HaveIsolation;      ///<�Ƿ�֧�ֵ�Ÿ���
    unsigned char   ExPowerSupply;      ///<�Ƿ�֧���ⲿ��Դ����

    unsigned char   IsOEM;              ///<�ǿͻ����ư汾��
    unsigned char   EECapacity;         ///<EEPROM֧����������λΪKByte,0��ʾû��EEPROM
    unsigned char   SPIFlashCapacity;   ///<Flash��������λΪMByte,0��ʾû��Flash
    unsigned char   TFCardSupport;      ///<�Ƿ�֧��TF����װ

    unsigned char   ProductionDate[12];  ///<��������

    unsigned char   USBControl;         ///<֧��ͨ��USB����
    unsigned char   SerialControl;      ///<֧�ִ��ڿ���
    unsigned char   EthControl;         ///<֧�����ڿ���
    unsigned char   VbatChannel;        ///<���Կ��Ƶ�VBAT���ͨ����
}HARDWARE_INFO;

//�����ѹ���ֵ
#define POWER_LEVEL_NONE    0   //�����
#define POWER_LEVEL_1V8     1   //���1.8V
#define POWER_LEVEL_2V5     2   //���2.5V
#define POWER_LEVEL_3V3     3   //���3.3V
#define POWER_LEVEL_5V0     4   //���5.0V

#ifdef __cplusplus
extern "C"
{
#endif

    /**
      * @brief  ��ʼ��USB�豸����ɨ���豸���������������
      * @param  pDevHandle ÿ���豸���豸�Ŵ洢��ַ
      * @retval ɨ�赽���豸����
      */
#ifdef __OS_ANDROID_BACK
    int WINAPI USB_ScanDevice(int* pDevHandle, int* pFd, int DevNum);
#else
    int  WINAPI USB_ScanDevice(int* pDevHandle);
#endif
    /**
      * @brief  ���豸���������
      * @param  DevHandle �豸������
      * @retval ���豸��״̬
      */
    unsigned char WINAPI USB_OpenDevice(int DevHandle);

    /**
      * @brief  �ر��豸
      * @param  DevHandle �豸������
      * @retval �ر��豸��״̬
      */
    unsigned char WINAPI USB_CloseDevice(int DevHandle);

    /**
      * @brief  ��λ�豸���򣬸�λ����Ҫ���µ���USB_ScanDevice��USB_OpenDevice����
      * @param  DevHandle �豸������
      * @retval ��λ�豸��״̬
      */
    unsigned char WINAPI USB_ResetDevice(int DevHandle);
    /**
      * @brief  ��ȡ�豸��Ϣ�������豸���ƣ��̼��汾�ţ��豸��ţ��豸����˵���ַ�����
      * @param  DevHandle �豸������
      * @param  pDevInfo �豸��Ϣ�洢�ṹ��ָ��
      * @param  pFunctionStr �豸����˵���ַ���
      * @retval ��ȡ�豸��Ϣ��״̬
      */
    unsigned char WINAPI DEV_GetDeviceInfo(int DevHandle, DEVICE_INFO* pDevInfo, char* pFunctionStr);
    /**
      * @brief  ��ȡ�豸�̼���Ϣ��ע�⣬�ϰ汾���豸�����޷�������ȡ
      * @param  DevHandle �豸������
      * @param  pHardwareInfo �豸Ӳ����Ϣ�洢�ṹ��ָ��
      * @retval ��ȡ�豸��Ϣ��״̬
      */
    unsigned char WINAPI DEV_GetHardwareInfo(int DevHandle, HARDWARE_INFO* pHardwareInfo);
    /**
      * @brief  �����û�������
      * @param  DevHandle �豸������
      * @retval �û������ݲ���״̬
      */
    unsigned char WINAPI DEV_EraseUserData(int DevHandle);

    /**
      * @brief  ���û�����д���û��Զ������ݣ�д������֮ǰ��Ҫ���ò������������ݲ���
      * @param  DevHandle �豸������
      * @param  OffsetAddr ����д��ƫ�Ƶ�ַ����ʼ��ַΪ0x00���û���������Ϊ0x10000�ֽڣ�Ҳ����64KBye
      * @param  pWriteData �û����ݻ������׵�ַ
      * @param  DataLen ��д��������ֽ���
      * @retval д���û��Զ�������״̬
      */
    unsigned char WINAPI DEV_WriteUserData(int DevHandle, int OffsetAddr, unsigned char* pWriteData, int DataLen);

    /**
      * @brief  ���û��Զ�����������������
      * @param  DevHandle �豸������
      * @param  OffsetAddr ����д��ƫ�Ƶ�ַ����ʼ��ַΪ0x00���û���������Ϊ0x10000�ֽڣ�Ҳ����64KBye
      * @param  pReadData �û����ݻ������׵�ַ
      * @param  DataLen �������������ֽ���
      * @retval �����û��Զ������ݵ�״̬
      */
    unsigned char WINAPI DEV_ReadUserData(int DevHandle, int OffsetAddr, unsigned char* pReadData, int DataLen);

    /**
      * @brief  ���ÿɱ��ѹ������������ѹֵ
      * @param  DevHandle �豸������
      * @param  PowerLevel �����ѹֵ
      * @retval ���������ѹ״̬
      */
    unsigned char WINAPI DEV_SetPowerLevel(int DevHandle, char PowerLevel);

    /**
      * @brief  ����CAN����LIN��ʱ���ԭʼֵ
      * @param  DevHandle �豸������
      * @param  pTimestamp ʱ���ָ��
      * @retval ��ȡʱ���״̬
      */
    unsigned char WINAPI DEV_GetTimestamp(int DevHandle, char BusType, unsigned int* pTimestamp);

    /**
      * @brief  ��λCAN/LINʱ�������Ҫ�ڳ�ʼ��CAN/LIN֮�����
      * @param  DevHandle �豸������
      * @retval ��λʱ���״̬
      */
    unsigned char WINAPI DEV_ResetTimestamp(int DevHandle);

    unsigned char WINAPI DEV_EraseSchData(int DevHandle, unsigned int Addr);
    unsigned char WINAPI DEV_WriteSchData(int DevHandle, int OffsetAddr, unsigned char* pWriteData, int DataLen);
    unsigned char WINAPI DEV_ReadSchData(int DevHandle, int OffsetAddr, unsigned char* pReadData, int DataLen);
    unsigned char WINAPI DEV_GetDllBuildTime(char* pDateTime);
#ifdef __cplusplus
}
#endif

#endif

