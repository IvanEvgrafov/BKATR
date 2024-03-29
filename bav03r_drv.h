//------------------------------------------------------------------//
// Title:     ������������ ���� �������� ���������� ���03� (���)    //
// File:      bav03r_drv.h                                          //
// EDA Tool:  CodeMaster-96                                         //
// Revision:  1.0                                                   //
// Rev.Date:  13/11/2016                                            //
// Rev.Note:                                                        //
// Author(s): ������� �.�.                                          //
//------------------------------------------------------------------//
// Note:                                                            //
//------------------------------------------------------------------//
// �������� ���������� ���������������
#ifndef BAV03R_DRIVER_H
#define BAV03R_DRIVER_H

#include "pmlc_drv.h"
#include "bav03r_drv.h"

#define ADC_N_0    0x0     // ��� �����-0
#define ADC_N_1    0x1     // ��� �����-1

#define ADC_CH_0  0x00     // ���_0/1, �����-0
#define ADC_CH_1  0x01     // ���_0/1, �����-1
#define ADC_CH_2  0x02     // ���_0/1, �����-2
#define ADC_CH_3  0x03     // ���_0/1, �����-3
#define ADC_CH_4  0x04     // ���_0/1, �����-4
#define ADC_CH_5  0x05     // ���_0/1, �����-5
#define ADC_CH_6  0x06     // ���_0/1, �����-6
#define ADC_CH_7  0x07     // ���_0/1, �����-7

#define ADC_MAX_VALUE   0x0FFF      // ��� 12 �������� -> 0,..,4095
#define ADC_MAX_VOLT    5.0         // max ������� ����������=5.0V
#define ADC_DELTA      (ADC_MAX_VOLT/ADC_MAX_VALUE)    // ���������� ���

// ----------- ��������� ������� ---------------------//
// ������������� ���������� ���03�
short init_bav03r(
   PMLC_CMD_LIST *plmc_cmd_ptr,   // ��������� �� ��������� ������� ��-��
   bav03r_type *adc_struct        // ��������� �� ��������� ���������� ���03�
   );

// ����� ������ ��-�������������� (������������ ��� ���-0 � ���-1)
short select_adc_channel(
   PMLC_CMD_LIST *plmc_cmd_ptr,   // ��������� �� ��������� ������� ��-��
   unsigned char ch_num,          // ����� ������ ���
   bav03r_type *adc_struct        // ��������� �� ��������� ���������� ���03�
   );

// ��������� ��-�������������� � �������� ��������� ����� ��������������.
// ��������� - 16 ������ ����� (unsigned short), �� ��� �������� - 12 ������� ��������.
// ����� ������ ��� ������ ���� ������ �������
short get_adc_data(
   PMLC_CMD_LIST *plmc_cmd_ptr,   // ��������� �� ��������� ������� ��-��
   unsigned char adc_num,         // ����� ���
   bav03r_type *adc_struct        // ��������� �� ��������� ���������� ���03�
   );

// ������� ������� � ������� �������� ���
short adc_data_merge(
   unsigned char adc_num,      // ����� ���
   unsigned char *msb_reg,     // ��������� �� ����� �������� ������� ��������
   unsigned char *lsb_reg      // ��������� �� ����� �������� ������� ��������
   );

// �������������� ��������� ����� � ����� � ��������� �������
float short_to_float(short data);

// ��������� �������� ���������� ���������������
#endif

