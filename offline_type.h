#pragma once
/**
  ******************************************************************************
  * @file    offline_type.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   ���߲�����غ������������Ͷ���.
  ******************************************************************************
  * @attention
  *
  *<center><a href="http:\\www.toomoss.com">http://www.toomoss.com</a></center>
  *<center>All Rights Reserved</center></h3>
  *
  ******************************************************************************
  */
#ifndef __OFFLINE_TYPE_H_
#define __OFFLINE_TYPE_H_

#include <stdint.h>

#define OFFLINE_MARK    0x7BB0F5C0

#define OFFLINE_ERROR_NONE      0x00
#define OFFLINE_ERROR_ERASE     0x01
#define OFFLINE_ERROR_WRITE     0x02
#define OFFLINE_ERROR_READ      0x03
#define OFFLINE_ERROR_VERIFY    0x04

#define OFFLINE_TYPE_LIN_SCH     0x00010001//LIN����ģʽ���߷�������
#define OFFLINE_TYPE_CAN2LIN     0x00020001//CAN��LIN�����໥ת��
#define OFFLINE_TYPE_CAN_SCH     0x00030001//CAN���߷�������
#define OFFLINE_TYPE_CAN_REAY    0x00030002//CAN�����м�
#define OFFLINE_TYPE_CAN_REAY_ONLINE  0x00030003//CAN�����м�
#define OFFLINE_TYPE_CAN2PWM     0x00040001//CANתPWM
#define OFFLINE_TYPE_PWM_SCH     0x00050001//PWM���߷���

#define CAN2LIN_TYPE_CAN2MLINW  0x00  //CANתLIN����д����
#define CAN2LIN_TYPE_MLINR2CAN  0x01  //����LIN������תCAN
#define CAN2LIN_TYPE_SLINR2CAN  0x02  //�ӻ�LIN��������תCAN
#define CAN2LIN_TYPE_CAN2SLINW  0x03  //CANתLIN�ӻ���������

#define CAN2LIN_DATA_TYPE_DIRECT  0x00  //ֱ��ת�����յ�������
#define CAN2LIN_DATA_TYPE_SPECIAL 0x01  //ת���ƶ�������

#define CAN_RELAY_TYPE_CAN1_2_CAN2      0x00//CAN1->CAN2
#define CAN_RELAY_TYPE_CAN2_2_CAN1      0x01//CAN2->CAN1
#define CAN_RELAY_TYPE_CAN12_2_CAN21    0x02//CAN1<->CAN2

#define CAN_RELAY_DATA_TYPE_DIRECT  0x00  //ֱ��ת�����յ�������
#define CAN_RELAY_DATA_TYPE_SPECIAL 0x01  //ת���ƶ�������
#define CAN_RELAY_DATA_TYPE_DISCARD 0x02  //������֡
  /*******************************��������ͷ��ض���*******************************/
typedef struct {
    uint32_t Mark;      //���߹��ܱ�ʶ�����̼�ͨ�����ñ�ʶ����Ϊ��������Ч�����߹�������
    uint32_t Type;      //���߹������Ͷ��壬�ο�CAN2LIN_TYPE_����
    uint32_t DataLen;   //���߹�����Ч�����ֽ���
}OFFLINE_TYPE_HEAD;
/*******************************LIN���߷�����ض���*******************************/
//���߷���LIN��ʼ���ṹ��
typedef struct {
    uint32_t BaudRate;
    uint8_t MasterMode;
    uint8_t ChannelEnable;//0-����ʼ����ͨ����1-��ʼ����ͨ��
}OFFLINE_LIN_INIT;
//���߷���LIN֡ͷ
typedef struct {
    OFFLINE_LIN_INIT LINInit[2];
    struct {
        uint8_t Key : 2;      //1-LIN1��Ϊ������LIN2�������ݣ�2-LIN2��Ϊ������LIN1�������ݣ�����ֵ-���жϰ�����ֱ�ӷ���
        uint8_t KeyType : 2;  //0-�͵�ƽ���ͣ�1-�����ط��ͣ�2-�½��ط��ͣ�3-�ߵ�ƽ����
    }SendType;
    uint32_t MsgLen;
}OFFLINE_LIN_HEAD;
//���߷���LIN���ݶ���
typedef struct {
    struct {
        uint16_t MsgType : 3;      //֡����
        uint16_t CheckType : 1;    //У������
        uint16_t DataLen : 4;      //LIN���ݶ���Ч�����ֽ���
        uint16_t Ch : 1;
    }Head;
    uint8_t Sync;         //�̶�ֵ��0x55
    uint8_t PID;          //֡ID
    uint8_t Data[8];      //����
    uint8_t Check;        //У��,ֻ��У����������ΪLIN_EX_CHECK_USER��ʱ�����Ҫ�û���������
    uint32_t  Timestamp;    //ʱ���,��λΪ100us
}OFFLINE_LIN_DATA;
/*******************************CAN���߷�����ض���*******************************/
//����CAN�����ʲ�����
typedef struct {
    uint16_t  SJW;
    uint16_t  BS1;
    uint16_t  BS2;
    uint16_t  BRP;
}OFFLINE_CAN_BAUD;
//���߷���CAN��ʼ���ṹ��
typedef struct {
    OFFLINE_CAN_BAUD  NBT;
    OFFLINE_CAN_BAUD  DBT;
    uint8_t ISOCRCEnable;//0-��ֹISO CRC,1-ʹ��ISO CRC
    uint8_t ResEnable;//0-�������ڲ�120ŷ�ն˵��裬1-�����ڲ�120ŷ�ն˵���
    uint8_t ChannelEnable;//0-����ʼ����ͨ����1-��ʼ����ͨ��
    uint8_t __Res;
}OFFLINE_CAN_INIT;
typedef struct {
    OFFLINE_CAN_INIT CANInit[2];
    struct {
        uint8_t Key : 2;      //1-LIN1��Ϊ������LIN2�������ݣ�2-LIN2��Ϊ������LIN1�������ݣ�����ֵ-���жϰ�����ֱ�ӷ���
        uint8_t KeyType : 2;  //0-�͵�ƽ���ͣ�1-�����ط��ͣ�2-�½��ط��ͣ�3-�ߵ�ƽ����
    }SendType;
    uint32_t MsgLen;
}OFFLINE_CAN_HEAD;
//���߷���CAN���ݶ���
typedef struct {
    struct {
        uint32_t ID : 29;
        uint32_t : 1;
        uint32_t RTR : 1;
        uint32_t IDE : 1;
    }Head;
    uint32_t TimeStamp;
    struct {
        uint8_t BRS : 1;//1-CANFD֡
        uint8_t ESI : 1;
        uint8_t FDF : 1;//1-���������
        uint8_t Ch : 2;
        uint8_t RXD : 1;
    }Flag;
    uint8_t DLC;
    uint8_t Data[64];
}OFFLINE_CAN_DATA;
//CAN�м�ͷ
typedef struct {
    OFFLINE_CAN_INIT CANInit[2];
    uint32_t OtherMsgFilterOut;//�����м���Ϣ���е���Ϣ����ʽ��0-ԭʼת����1-���˵�����ת��
    uint32_t MsgLen;//CAN_RELAY_DATA��Ϣ֡��
}CAN_RELAY_HEAD;
typedef struct {
    OFFLINE_CAN_DATA CANData[2];
    uint8_t ConvertType;//�ο�CAN_RELAY_TYPE��ͷ�ĺ궨��
    uint8_t DataType;   //����ת�����ͣ��ο�CAN_RELAY_DATA_TYPE��ͷ�궨��
    uint32_t PeriodMs;  //����ת�����ڣ���λΪ����
}CAN_RELAY_DATA;
/*******************************CAN&LIN��ת��ض���*******************************/
//����ʵ��CAN&LIN��ת
typedef struct {
    OFFLINE_LIN_INIT LINInit[2];
    OFFLINE_CAN_INIT CANInit[2];
    uint32_t MsgLen;
}OFFLINE_CAN2LIN_HEAD;
//CAN&LIN��ת����
typedef struct {
    uint8_t ConvertType;
    uint8_t DataType;
    uint32_t PeriodMs;
    OFFLINE_CAN_DATA CANMsg;
    OFFLINE_LIN_DATA LINMsg;
}OFFLINE_CAN2LIN_DATA;

/*******************************CAN2PWM��ת��ض���*******************************/
typedef struct {
    OFFLINE_CAN_INIT CANInit[2];
    uint32_t ID;
    uint8_t IDE;
}OFFLINE_CAN2PWM_HEAD;

/*******************************����PWM��ض���*******************************/
typedef struct {
    uint16_t Prescaler;  //Ԥ��Ƶ��
    uint16_t Precision;  //ռ�ձȵ��ھ���,ʵ��Ƶ�� = 200MHz/(Prescaler*Precision)
    uint16_t Pulse;      //ռ�ձȣ�ʵ��ռ�ձ�=(Pulse/Precision)*100%
    uint16_t Phase;      //������λ��ȡֵ0��Precision-1
    uint8_t  Polarity;   //���μ��ԣ�ȡֵ0����1
    uint8_t  ChannelMask;//ͨ���ţ���Ҫʹ��ĳ��ͨ�������ӦλΪ1�����λ��Ӧͨ��0
    uint8_t  EN12VOut;
    uint8_t __Res;
}OFFLINE_PWM_DATA;



#endif

