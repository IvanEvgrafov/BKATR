//------------------------------------------------------------------//
// Title:     ������������ ���� �������� ��-�� ���������� ��01�     //
// File:      pmlc_drv.h                                            //
// EDA Tool:  CodeMaster-96                                         //
// Revision:  1.0                                                   //
// Rev.Date:  08/12/2016                                            //
// Rev.Date:  24/07/2017                                            //
// Rev.Date:  04/08/2017                                            //
// Rev.Date:  29/09/2017                                            //
// Rev.Note:                                                        //
// Author(s): ������� �.�.                                          //
//------------------------------------------------------------------//
// Note: ����������:                                                //
//       ��-�� - ���������� ����� ������������� ������              //
//       PMLC - Processor Module Link Controller                    //
//       ��-������������ ����������                                 //
//       PDev - Periferal DEVice                                    //
//       ��-�� - ���������� ����� ������������� ����������          //
//       PDLC - Peripheral Device Link Controller                   //
//------------------------------------------------------------------//
// Note2: ���������� �� ������������ !!!!!                          //
//------------------------------------------------------------------//
// �������� ���������� ���������������
#ifndef PMLC_DRIVER_H
#define PMLC_DRIVER_H

// ����� ��-�� (�, ��������������, ��)
#define NOM_PDEV          0x00    // �������� ��
#define RSV_PDEV          0x80    // ��������� ��

// ����� ������������� ����������. ������������ ��� ������.
#define PDEV_0       0x00    // ����������-0
#define PDEV_1       0x01    // ����������-0
#define PDEV_2       0x02    // ����������-0
#define PDEV_3       0x03    // ����������-0
#define PDEV_4       0x04    // ����������-0

// ���� ������ ��-��
#define CMD_NOP      0x00    // ������� NOP ("��� ��������")
#define CMD_WRITE    0x04    // ������� WRITE ("������ ������")
#define CMD_READ     0x08    // ������� READ ("������ ������")
#define CMD_DIAG     0x0C    // ������� DIAG ("���������������")

// ���� ��������� ����������/��������� ��-�� (bits field masks)
#define SPI_BUSY     0x01    // �������� ������ ������
#define XFR_DONE     0x02    // ���������� �������� ������ ������
#define IRQ_EN       0x04    // ���������� ���������� ����� ���������� �������� ������ ������
#define IRQ          0x08    // ���������� ����� ���������� �������� ������ ������
#define CRC_IRQ_EN   0x20    // ���������� ���������� ��� ������ CRC-����� ������ ������
#define CRC_STATUS   0x40    // ��������� �������� CRC-����� ������ ������

// �������� ��������
#define R_12MBPS     0x0     // 12 ����/�
#define R_6MBPS      0x1     //  6 ����/�
#define R_3MBPS      0x2     //  3 ����/�
#define R_1MBPS      0x3     // 1.5 ����/�

// ������ ���������� ������� ��-��
// � �������� ������������ �������� PMLC_cmd_exec (��� �������� ������������)
#define EXEC_OK      0x00    // ������� ��������� �������
#define EXEC_FAIL    0xFF    // ������� ��������� � �������

// �������� ������������ ���������� �� (��� �������� ������������-short/int)
#define PMLC_CMD_OK   0      // ������� ��-�� ��������� �������
                             // ��� ������������ ������������� ������
#define PMLC_CMD_ERR  -1     // ������� ��-�� ��������� � �������

// ������ ��������� ������� ��-��.
//������������ ������������ ��������� ������� ��-�� !!!
typedef struct {
   unsigned char CMD_EXEC_STATUS;   // ���� ���������� �������:0=OK/FF=������
   unsigned char SLAVE_SEL;         // ������� ������� ������ ��
   unsigned char BAUD_RATE;         // ������� ������� �������� ��������
   unsigned char STATUS;            // ������� ��������� ��-��
   unsigned char CTRL;              // ������� ���������� ��-��
   unsigned char SEND_ADDR;         // ������� ������/������� �����������
   unsigned char SEND_DATA;         // ������� ������ �����������
   unsigned char RCV_ADDR_CMD;      // ������� ������/������� ��������
   unsigned char RCV_DATA;          // ������� ������ ��������
   unsigned char RCV_CRC;           // ������� CRC-����� ��������
   } PMLC_CMD_LIST;

// ������ ��������� ��-��
#define ADDR_IO_PORT_0    0x00    // ���� �����-������ 0
#define ADDR_IO_PORT_1    0x10    // ���� �����-������ 1
#define ADDR_IO_PORT_2    0x20    // ���� �����-������ 2
#define ADDR_IO_PORT_3    0x30    // ���� �����-������ 3
#define ADDR_IO_PORT_4    0x40    // ���� �����-������ 4
#define ADDR_IO_PORT_5    0x50    // ���� �����-������ 5
#define ADDR_IO_CFG_REG   0x80    // ������� ���������������� ������ �����-������
#define ADDR_ADC_GEN_0    0xA0    // ������� ��������� ������� ���-0
#define ADDR_ADC_GEN_1    0xB0    // ������� ��������� ������� ���-1
#define ADDR_ADC_GEN_2    0xC0    // ������� ��������� ������� ���-2
#define ADDR_CTRL_REG     0xE0    // ������� ���������� ��-��
#define ADDR_STATUS_REG   0xE0    // ������� ��������� ��-��
#define ADDR_ID_REG       0xF0    // ������� �������������

// ���� ��������� ����������/��������� ��-�� (bits field masks)
#define CRC_CHK           0x01     // ��������� �������� CRC-����� ��������� ������ ������
#define MODE              0x02     // ����� ������ ��-��
#define ID_REG_VALUE      0x5A     // �������� �������� ������������� ��-�� ����� ������

// Reference ������ ��������� ��������� ��������� ������������� ����������
// ������������ ��� ������� ��� ������� ��-�� ������� ������������� ���������.
// �������� �����-�� ������! �������������� ����-�������(���������������).
typedef struct {
   unsigned char IO_PORT_0;       // ������� ����� �����-������ 0
   unsigned char IO_PORT_1;       // ������� ����� �����-������ 1
   unsigned char IO_PORT_2;       // ������� ����� �����-������ 2
   unsigned char IO_PORT_3;       // ������� ����� �����-������ 3
   unsigned char IO_PORT_4;       // ������� ����� �����-������ 4
   unsigned char IO_PORT_5;       // ������� ����� �����-������ 5
   unsigned char IO_CFG_REG;      // ������� ���������������� ������ �����-������
   unsigned char ADC_GEN_0;       // ������� ��������� ������� ���-0
   unsigned char ADC_GEN_1;       // ������� ��������� ������� ���-1
   unsigned char ADC_GEN_2;       // ������� ��������� ������� ���-2
   unsigned char CTRL_REG;        // ������� ���������� ��-��
   unsigned char STATUS_REG;      // ������� ��������� ��-��
   unsigned char ID_REG;          // ������� �������������
   } pdev_status;


// ������ ��������� ��������� ��������� ���������� ���03�
typedef struct {
   unsigned char IO_PORT_0;       // ������� ����� �����-������ 0
   unsigned char IO_PORT_1;       // ������� ����� �����-������ 1
   unsigned char IO_PORT_2;       // ������� ����� �����-������ 2
   unsigned char IO_PORT_3;       // ������� ����� �����-������ 3
   unsigned char IO_PORT_4;       // ������� ����� �����-������ 4
//   unsigned char IO_PORT_5;       // ������� ����� �����-������ 5
   unsigned char IO_CFG_REG;      // ������� ���������������� ������ �����-������
//   unsigned char ADC_GEN_0;       // ������� ��������� ������� ���-0
//   unsigned char ADC_GEN_1;       // ������� ��������� ������� ���-1
//   unsigned char ADC_GEN_2;       // ������� ��������� ������� ���-2
   unsigned char CTRL_REG;        // ������� ���������� ��-��
   unsigned char STATUS_REG;      // ������� ��������� ��-��
   unsigned char ID_REG;          // ������� �������������
   } bcv03r_type;

// ������ ��������� ��������� ��������� ���������� ���04�
typedef struct {
//   unsigned char IO_PORT_0;       // ������� ����� �����-������ 0
   unsigned char IO_PORT_1;       // ������� ����� �����-������ 1
//   unsigned char IO_PORT_2;       // ������� ����� �����-������ 2
//   unsigned char IO_PORT_3;       // ������� ����� �����-������ 3
//   unsigned char IO_PORT_4;       // ������� ����� �����-������ 4
//   unsigned char IO_PORT_5;       // ������� ����� �����-������ 5
   unsigned char IO_CFG_REG;      // ������� ���������������� ������ �����-������
//   unsigned char ADC_GEN_0;       // ������� ��������� ������� ���-0
//   unsigned char ADC_GEN_1;       // ������� ��������� ������� ���-1
//   unsigned char ADC_GEN_2;       // ������� ��������� ������� ���-2
   unsigned char CTRL_REG;        // ������� ���������� ��-��
   unsigned char STATUS_REG;      // ������� ��������� ��-��
   unsigned char ID_REG;          // ������� �������������
   } bcv04r_type;

// ������ ��������� ��������� ��������� ���������� ���03�
typedef struct {
   unsigned char IO_PORT_0;       // ������� ����� �����-������ 0
   unsigned char IO_PORT_1;       // ������� ����� �����-������ 1
   unsigned char IO_PORT_2;       // ������� ����� �����-������ 2
   unsigned char IO_PORT_3;       // ������� ����� �����-������ 3
   unsigned char IO_PORT_4;       // ������� ����� �����-������ 4
   unsigned char IO_PORT_5;       // ������� ����� �����-������ 5
   unsigned char IO_CFG_REG;      // ������� ���������������� ������ �����-������
   unsigned char ADC_GEN_0;       // ������� ��������� ������� ���-0
   unsigned char ADC_GEN_1;       // ������� ��������� ������� ���-1
//   unsigned char ADC_GEN_2;       // ������� ��������� ������� ���-2
   unsigned char CTRL_REG;        // ������� ���������� ��-��
   unsigned char STATUS_REG;      // ������� ��������� ��-��
   unsigned char ID_REG;          // ������� �������������
   } bav03r_type;

// ������ ��������� ��������� ��������� ���������� ���05�
typedef struct {
   unsigned char IO_PORT_0;       // ������� ����� �����-������ 0
   unsigned char IO_PORT_1;       // ������� ����� �����-������ 1
   unsigned char IO_PORT_2;       // ������� ����� �����-������ 2
   unsigned char IO_PORT_3;       // ������� ����� �����-������ 3
   unsigned char IO_PORT_4;       // ������� ����� �����-������ 4
//   unsigned char IO_PORT_5;       // ������� ����� �����-������ 5
   unsigned char IO_CFG_REG;      // ������� ���������������� ������ �����-������
//   unsigned char ADC_GEN_0;       // ������� ��������� ������� ���-0
//   unsigned char ADC_GEN_1;       // ������� ��������� ������� ���-1
//   unsigned char ADC_GEN_2;       // ������� ��������� ������� ���-2
   unsigned char CTRL_REG;        // ������� ���������� ��-��
   unsigned char STATUS_REG;      // ������� ��������� ��-��
   unsigned char ID_REG;          // ������� �������������
   } bta05r_type;

// ������ ��������� ��������� ��������� ���������� ���06�
typedef struct {
   unsigned char IO_PORT_0;       // ������� ����� �����-������ 0
   unsigned char IO_PORT_1;       // ������� ����� �����-������ 1
   unsigned char IO_PORT_2;       // ������� ����� �����-������ 2
   unsigned char IO_PORT_3;       // ������� ����� �����-������ 3
   unsigned char IO_PORT_4;       // ������� ����� �����-������ 4
   unsigned char IO_PORT_5;       // ������� ����� �����-������ 5
   unsigned char IO_CFG_REG;      // ������� ���������������� ������ �����-������
//   unsigned char ADC_GEN_0;       // ������� ��������� ������� ���-0
//   unsigned char ADC_GEN_1;       // ������� ��������� ������� ���-1
//   unsigned char ADC_GEN_2;       // ������� ��������� ������� ���-2
   unsigned char CTRL_REG;        // ������� ���������� ��-��
   unsigned char STATUS_REG;      // ������� ��������� ��-��
   unsigned char ID_REG;          // ������� �������������
   } bta06r_type;

//******************************************************************//
//                 ��������� ������� ��������                       //
//******************************************************************//
// ����� ������������� ����������
void PMLC_select_device (
    unsigned char pk_num,       // ��� ��: 0=��������/1=���������
    unsigned char pdev_num,     // ����� ������������� ����������
    PMLC_CMD_LIST *list_ptr     // ��������� �� ��������� ������� ��-��
    );

// ����� �������� �������� ��-��
void PMLC_baud_rate (
    unsigned char baud_rate,    // �������� �������� ������ ������
    PMLC_CMD_LIST *list_ptr     // ��������� �� ��������� ������� ��-��
    );

// ���������� ������� ��-��
unsigned char PMLC_cmd_exec (
    PMLC_CMD_LIST *list_ptr,    // ��������� �� ��������� ������� ��-��
    unsigned char addr,         // ����� �������� ��
    unsigned char cmd,          // ������� ��-��
    unsigned char data,         // ������
    unsigned char *pdev_reg     // ��������� �� ����� ����������� �������� ��
    );


// ��������� �������� ���������� ���������������
#endif
