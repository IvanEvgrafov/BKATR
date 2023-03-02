#include "pmlc_drv.h"
#include "bav03r_drv.h"

extern PMLC_CMD_LIST PMLC_TASK;

extern void delay_7000();

// ������� � ������������� ��������� ��������� ��������� ���������� ���03�
#pragma data_alignment=2
bav03r_type bav03r_N = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};


//************** ������������� ���������� ���03� *******************//
short init_bav03r(
   PMLC_CMD_LIST *plmc_cmd_ptr,   // ��������� �� ��������� ������� ��-��
   bav03r_type *bav03r_ptr        // ��������� �� ��������� ���������� ���03�
   )
{
//--- ���������������� ������ �����-������
PMLC_cmd_exec((PMLC_CMD_LIST*)plmc_cmd_ptr, ADDR_IO_CFG_REG, CMD_WRITE, 0x04, &(bav03r_ptr->IO_CFG_REG));
if ((*plmc_cmd_ptr).CMD_EXEC_STATUS)
   return (1);     // ������� ��-�� ��������� � �������
else return (0);    // ������� ��-�� ��������� �������
}


// ����� ������ ��-�������������� (������������ ��� ���-0 � ���-1)
short select_adc_channel(

   PMLC_CMD_LIST *plmc_cmd_ptr,   // ��������� �� ��������� ������� ��-��
   unsigned char ch_num,          // ����� ������ ���
   bav03r_type *bav03r_ptr        // ��������� �� ��������� ���������� ���03�
   )
{
        // ����� ������ ��-��������������
        PMLC_cmd_exec((PMLC_CMD_LIST*)plmc_cmd_ptr, ADDR_IO_PORT_2, CMD_WRITE, ch_num, &(bav03r_ptr->IO_PORT_2));
        if ((*plmc_cmd_ptr).CMD_EXEC_STATUS)
        return (1);     // ������� ��-�� ��������� � �������

                else
        return (0);    // ������� ��-�� ��������� �������
}


// ��������� ��-�������������� � �������� ��������� ����� ��������������.
// ��������� - 16 ������ ����� (unsigned short), �� ��� �������� - 12 ������� ��������.
// ����� ������ ��� ������ ���� ������ �������
short get_adc_data(
        PMLC_CMD_LIST *plmc_cmd_ptr,   // ��������� �� ��������� ������� ��-��
        unsigned char adc_num,         // ����� ���
        bav03r_type *bav03r_ptr        // ��������� �� ��������� ���������� ���03�
        )

{
        unsigned short adc_data, d1,d2;


        if (!adc_num)
        {
                // ���-0

                // ������ ���-0
                PMLC_cmd_exec((PMLC_CMD_LIST*)plmc_cmd_ptr, ADDR_ADC_GEN_0, CMD_WRITE, 0x55, &(bav03r_ptr->ADC_GEN_0));

                //--- ������ ������� �������� ���_0 (D[3:0])
                PMLC_cmd_exec((PMLC_CMD_LIST*)plmc_cmd_ptr, ADDR_IO_PORT_4, CMD_READ, 0x0, &(bav03r_ptr->IO_PORT_4));

                //--- ������ ������� �������� ���_0 (D[11:4])
                PMLC_cmd_exec((PMLC_CMD_LIST*)plmc_cmd_ptr, ADDR_IO_PORT_5, CMD_READ, 0x0, &(bav03r_ptr->IO_PORT_5));


                d1 = ((bav03r_ptr->IO_PORT_4) >> 4);

                d2 = ((bav03r_ptr->IO_PORT_5) << 4);


                // ������� ������� � ������� �������� ���-0

                adc_data = d2 | d1 ;

        }
        else
        {                // ���-1

                // ������ ���-1
                PMLC_cmd_exec((PMLC_CMD_LIST*)plmc_cmd_ptr, ADDR_ADC_GEN_1, CMD_WRITE, 0xAA, &(bav03r_ptr->ADC_GEN_1));

                //--- ������ ������� �������� ���_1 (D[7:0])
                PMLC_cmd_exec((PMLC_CMD_LIST*)plmc_cmd_ptr, ADDR_IO_PORT_0, CMD_READ, 0x0, &(bav03r_ptr->IO_PORT_0));

                //--- ������ ������� �������� ���_1 (D[11:8])
                PMLC_cmd_exec((PMLC_CMD_LIST*)plmc_cmd_ptr, ADDR_IO_PORT_1, CMD_READ, 0x0, &(bav03r_ptr->IO_PORT_1));


                d1 = (bav03r_ptr->IO_PORT_0);

                d2 = ((bav03r_ptr->IO_PORT_1) << 8);

                adc_data = d2 | d1 ;

        }

        return (adc_data);    // ������� ��-�� ��������� �������->������������ ������������� ������

}


// ��������� ���������� ������ � ���

short unsigned read_adc(unsigned char num_chan)
{
        unsigned short adc;

        if (num_chan > 5)
        {
                num_chan = num_chan - 6;

                select_adc_channel(&PMLC_TASK, num_chan,&bav03r_N);

                delay_7000();

                adc = get_adc_data(&PMLC_TASK, 1,&bav03r_N);
        }
        else
        {
                select_adc_channel(&PMLC_TASK, num_chan,&bav03r_N);

                delay_7000();

                adc = get_adc_data(&PMLC_TASK, 0,&bav03r_N);
        };

        return (adc);
}


// ��������� ���������� ������ � ������������ ���

short unsigned read_adc_control(unsigned char num_chan)
{

        unsigned short adc;

        if (num_chan == 14)
        {
                select_adc_channel(&PMLC_TASK, 7,&bav03r_N);

                adc = get_adc_data(&PMLC_TASK, 0,&bav03r_N);
        }
        else
        {
                select_adc_channel(&PMLC_TASK, 7,&bav03r_N);

                adc = get_adc_data(&PMLC_TASK, 1,&bav03r_N);

        };

        return (adc);
}

