#include "pmlc_drv.h"

extern unsigned char num_AB;
extern unsigned char SYS_WDT_START, SYS_WDT_REPEAT;

#pragma data_alignment=2
PMLC_CMD_LIST PMLC_TASK = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

// ������� � ������������� ��������� ��������� ��������� ���������� ���
#pragma data_alignment=2
bta05r_type BTA05R_AN = {0x0,0x0,0x0,0x0,0x0,0x0,0x0};

// ������� � ������������� ��������� ��������� ��������� ���������� ���06
#pragma data_alignment=2
bta06r_type BTA06R_AN = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00,0x00};


// ��������� �������� ��� ������ � ��

void delay_switch_ta()
{
    unsigned short i;

    for (i = 1; i < 1000; i++)
    {
        __NOP();
    }
}


// ������ ������� ���������, 1-16 - ���06, 17-32 - ���05

// tm_data_1 + tm_data_2

// 1 - �� 1 ��
// 2 - �� 1 ��
// 3 - �� 2 ��
// 4 - �� 2 ��
// 5 - �� 3 ��
// 6 - �� 3 ��
// 7 - �� 4 ��
// 8 - �� 4 ��


// tm_data_3

// 9 - ��-� ��
// 10 - ��-� ��
// 11 - ��-� ��
// 12 - ��-� ��


// tm_data_4

// 13 - ���� ��
// 14 - ���� ��
// 15 - ������ ��
// 16 - ������ ��


// tm_data_5

// 17 - ��� �1 ��
// 18 - ��� �1 ��
// 19 - ��� �2 ��
// 20 - ��� �2 ��


// tm_data_6

// 21 - ��� �1 ��
// 22 - ��� �1 ��
// 23 - ��� �2 ��
// 24 - ��� �2 ��


// tm_data_7

// 25 - ���� ��
// 26 - ���� ��
// 27 - ��� ��
// 28 - ��� ��


// tm_data_8

// 29 - ���� ��
// 30 - ���� ��
// 31 - ���� ��
// 32 - ���� ��


// ��������� ��������� �� �� ��������� ������

unsigned short on_ta(unsigned char num_ta)
{

        unsigned char  mask_ta_port, data_port;
        unsigned short ta_t;

        unsigned char sdvig;
        unsigned char indata;
        unsigned char AB;

        // ��� ������� ����� ��� 06

        if ((num_ta >= 1) && (num_ta <=16))
        {
                if (num_ta <= 8)
                {
                        if (num_AB == 0)
                        {
                                // ����� ������� �, �� = 4
                                PMLC_select_device(NOM_PDEV, PDEV_4, &PMLC_TASK);

                                AB = 0;
                        }
                        else
                        {
                                // ����� ������� �, �� = 4
                                PMLC_select_device(RSV_PDEV, PDEV_4, &PMLC_TASK);

                                AB = 1;
                        }
                }
                else
                {
                        if (num_AB == 0)
                        {
                                // ����� ������� �, �� = 4
                                PMLC_select_device(RSV_PDEV, PDEV_4, &PMLC_TASK);

                                AB = 1;
                        }
                        else
                        {
                                // ����� ������� �, �� = 4
                                PMLC_select_device(NOM_PDEV, PDEV_4, &PMLC_TASK);

                                AB = 0;
                        }

                        num_ta = num_ta - 8;
                }

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_CFG_REG, CMD_WRITE, 0x33, &(BTA06R_AN.IO_CFG_REG));

                if (num_ta >= 5)
                {

                        sdvig = num_ta - 5;
                        sdvig = sdvig * 2;

                        data_port =  1 << sdvig;
                        mask_ta_port = 3 << sdvig;


                        if (AB == 0)
                        {
                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_1, CMD_WRITE, data_port, &(BTA06R_AN.IO_PORT_1));
                        }
                        else
                        {
                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_5, CMD_WRITE, data_port, &(BTA06R_AN.IO_PORT_5));
                        }

                        delay_switch_ta();


                        if (AB == 0)
                        {
                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_1, CMD_WRITE, 0, &(BTA06R_AN.IO_PORT_1));
                        }
                        else
                        {
                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_5, CMD_WRITE, 0, &(BTA06R_AN.IO_PORT_5));
                        }


                        for (ta_t = 0; ta_t < 30000; ta_t ++)
                        {

                                // ��������� ������ �� 3-�� ����� (����������)

                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_3, CMD_READ, 0x00, &(BTA06R_AN.IO_PORT_3));

                                indata = ~BTA06R_AN.IO_PORT_3;

                                indata = (indata & mask_ta_port);

                                if (indata == mask_ta_port)
                                {
                                        return 0;
                                }

                                __NOP();
                                __NOP();
                                __NOP();
                                __NOP();
                                __NOP();

                        }

                        return 1;
                }
                else
                {
                        sdvig = num_ta - 1;
                        sdvig = sdvig * 2;

                        data_port =  1 << sdvig;
                        mask_ta_port = 3 << sdvig;


                        if (AB == 0)
                        {
                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, data_port, &(BTA06R_AN.IO_PORT_0));
                        }
                        else
                        {
                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, data_port, &(BTA06R_AN.IO_PORT_4));
                        }

                        delay_switch_ta();

                        if (AB == 0)
                        {
                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA06R_AN.IO_PORT_0));
                        }
                        else
                        {
                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA06R_AN.IO_PORT_4));
                        }


                        for (ta_t = 0; ta_t < 30000; ta_t ++)
                        {

                                // ��������� ������ �� 2-�� ����� (����������)

                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_2, CMD_READ, 0x00, &(BTA06R_AN.IO_PORT_2));

                                indata = ~BTA06R_AN.IO_PORT_2;

                                indata = (indata & mask_ta_port);

                                if (indata == mask_ta_port)
                                {
                                        return 0;
                                }

                                __NOP();
                                __NOP();
                                __NOP();
                                __NOP();
                                __NOP();
                        }

                        return 1;
                }
        }
        else


        // ��� ������� ����� ��� 05

        if ((num_ta > 16) &&  (num_ta <=20))
        {
                // �������� �, ��-3

                if (num_AB == 0)
                {
                        PMLC_select_device(NOM_PDEV, PDEV_3, &PMLC_TASK);
                        AB = 0;
                }
                else
                {
                         PMLC_select_device(RSV_PDEV, PDEV_3, &PMLC_TASK);
                         AB = 1;
                }

                num_ta = num_ta - 16;
        }

        if ((num_ta > 20) &&  (num_ta <=24))
        {
                // �������� �, ��-2

                if (num_AB == 0)
                {
                        PMLC_select_device(NOM_PDEV, PDEV_2, &PMLC_TASK);
                        AB = 0;
                }
                else
                {
                        PMLC_select_device(RSV_PDEV, PDEV_2, &PMLC_TASK);
                        AB = 1;
                }

                num_ta = num_ta - 20;
        }


        if ((num_ta > 24) && (num_ta <=28))
        {

                // �������� �, ��-2

                if (num_AB == 0)
                {
                        PMLC_select_device(RSV_PDEV, PDEV_2, &PMLC_TASK);
                        AB = 1;
                }
                else
                {
                         PMLC_select_device(NOM_PDEV, PDEV_2, &PMLC_TASK);
                         AB = 0;
                }

                num_ta = num_ta - 24;
        }


        if ((num_ta > 28) &&  (num_ta <=32))
        {
                // �������� �, ��-3

                if (num_AB == 0)
                {
                        PMLC_select_device(RSV_PDEV, PDEV_3, &PMLC_TASK);
                        AB = 1;
                }
                else
                {
                        PMLC_select_device(NOM_PDEV, PDEV_3, &PMLC_TASK);
                        AB = 0;
                }

                num_ta = num_ta - 28;
        }


        sdvig = num_ta - 1;
        sdvig = sdvig * 2;

        data_port =  1 << sdvig;
        mask_ta_port = 3 << sdvig;


        if (AB == 0)
        {
                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, data_port, &(BTA05R_AN.IO_PORT_0));
        }
        else
        {
                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, data_port, &(BTA05R_AN.IO_PORT_4));
        }

        delay_switch_ta();

        if (AB == 0)
        {
                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
        }
        else
        {
                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
        }


        for (ta_t = 0; ta_t < 30000; ta_t ++)
        {
                // ��������� ������ �� 2-�� ����� (����������)

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_2, CMD_READ, 0x00, &(BTA05R_AN.IO_PORT_2));

                indata = ~BTA05R_AN.IO_PORT_2;

                indata = (indata & mask_ta_port);

                if (indata == mask_ta_port)
                {
                        return 0;
                }

                __NOP();
                __NOP();
                __NOP();
                __NOP();
                __NOP();
        }

        return 1;
}


// ��������� ���������� �� �� ��������� ������

unsigned short off_ta(unsigned char num_ta)
{

        unsigned char  mask_ta_port, data_port;
        unsigned short ta_t;

        unsigned char sdvig;
        unsigned char indata;
        unsigned char AB;

        // ��� ������� ����� ��� 06

        if ((num_ta >= 1) && (num_ta <=16))
        {
                if (num_ta <= 8)
                {
                        if (num_AB == 0)
                        {
                                // ����� ������� �, �� = 4
                                PMLC_select_device(NOM_PDEV, PDEV_4, &PMLC_TASK);

                                AB = 0;
                        }
                        else
                        {
                                // ����� ������� �, �� = 4
                                PMLC_select_device(RSV_PDEV, PDEV_4, &PMLC_TASK);

                                AB = 1;
                        }
                }
                else
                {
                        if (num_AB == 0)
                        {
                                // ����� ������� �, �� = 4
                                PMLC_select_device(RSV_PDEV, PDEV_4, &PMLC_TASK);

                                AB = 1;
                        }
                        else
                        {
                                // ����� ������� �, �� = 4
                                PMLC_select_device(NOM_PDEV, PDEV_4, &PMLC_TASK);

                                AB = 0;
                        }

                        num_ta = num_ta - 8;
                }

                if (num_ta >= 5)
                {
                        sdvig = num_ta - 5;
                        sdvig = sdvig * 2;

                        data_port =  2 << sdvig;
                        mask_ta_port = 3 << sdvig;

                        if (AB == 0)
                        {
                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_1, CMD_WRITE, data_port, &(BTA06R_AN.IO_PORT_1));
                        }
                        else
                        {
                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_5, CMD_WRITE, data_port, &(BTA06R_AN.IO_PORT_5));
                        }

                        delay_switch_ta();

                        if (AB == 0)
                        {
                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_1, CMD_WRITE, 0, &(BTA06R_AN.IO_PORT_1));
                        }
                        else
                        {
                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_5, CMD_WRITE, 0, &(BTA06R_AN.IO_PORT_5));
                        }


                        for (ta_t = 0; ta_t < 32000; ta_t ++)
                        {
                                // ��������� ������ �� 3-�� ����� (����������)

                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_3, CMD_READ, 0x00, &(BTA06R_AN.IO_PORT_3));

                                indata = BTA06R_AN.IO_PORT_3;

                                indata = indata & mask_ta_port;

                                if (indata == mask_ta_port)
                                {
                                        return 0;
                                }

                                __NOP();
                                __NOP();
                                __NOP();
                                __NOP();
                                __NOP();

                                // ������� WDT

                                __DI();

                                SYS_WDT_START = 0x3C;
                                SYS_WDT_REPEAT = 0xA5;

                                __EI();
                         }

                        return 1;
                }
                else
                {
                        sdvig = num_ta - 1;
                        sdvig = sdvig * 2;

                        data_port =  2 << sdvig;
                        mask_ta_port = 3 << sdvig;


                        if (AB == 0)
                        {
                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, data_port, &(BTA06R_AN.IO_PORT_0));
                        }
                        else
                        {
                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, data_port, &(BTA06R_AN.IO_PORT_4));
                        }

                        delay_switch_ta();


                        if (AB == 0)
                        {
                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA06R_AN.IO_PORT_0));
                        }
                        else
                        {
                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA06R_AN.IO_PORT_4));
                        }


                        for (ta_t = 0; ta_t < 32000; ta_t ++)
                        {

                                // ��������� ������ �� 2-�� ����� (����������)

                                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_2, CMD_READ, 0x00, &(BTA06R_AN.IO_PORT_2));

                                indata = BTA06R_AN.IO_PORT_2;

                                indata = indata & mask_ta_port;

                                if (indata == mask_ta_port)
                                {
                                        return 0;
                                }

                                __NOP();
                                __NOP();
                                __NOP();
                                __NOP();
                                __NOP();

                                // ������� WDT

                                __DI();

                                SYS_WDT_START = 0x3C;
                                SYS_WDT_REPEAT = 0xA5;

                                __EI();
                        }

                        return 1;
                }
        }


        // ��� ������� ����� ��� 05

        if ((num_ta > 16) &&  (num_ta <=20))
        {

                // �������� �, ��-3

                if (num_AB == 0)
                {
                        PMLC_select_device(NOM_PDEV, PDEV_3, &PMLC_TASK);
                        AB = 0;
                }
                else
                {
                         PMLC_select_device(RSV_PDEV, PDEV_3, &PMLC_TASK);
                         AB = 1;
                }

                num_ta = num_ta - 16;
        }

        if ((num_ta > 20) &&  (num_ta <=24))
        {
                // �������� �, ��-2

                if (num_AB == 0)
                {
                        PMLC_select_device(NOM_PDEV, PDEV_2, &PMLC_TASK);
                        AB = 0;
                }
                else
                {
                        PMLC_select_device(RSV_PDEV, PDEV_2, &PMLC_TASK);
                        AB = 1;
                }

                num_ta = num_ta - 20;
        }


        if ((num_ta > 24) &&  (num_ta <=28))
        {

                // �������� �, ��-2

                if (num_AB == 0)
                {
                        PMLC_select_device(RSV_PDEV, PDEV_2, &PMLC_TASK);
                        AB = 1;
                }
                else
                {
                         PMLC_select_device(NOM_PDEV, PDEV_2, &PMLC_TASK);
                         AB = 0;
                }

                num_ta = num_ta - 24;
        }


        if ((num_ta > 28) &&  (num_ta <=32))
        {

                // �������� �, ��-3

                if (num_AB == 0)
                {
                        PMLC_select_device(RSV_PDEV, PDEV_3, &PMLC_TASK);
                        AB = 1;
                }
                else
                {
                        PMLC_select_device(NOM_PDEV, PDEV_3, &PMLC_TASK);
                        AB = 0;
                }

                num_ta = num_ta - 28;
        }

        data_port =  2 << ((num_ta - 1)*2);
        mask_ta_port = 2 << ((num_ta - 1)*2);


        if (AB == 0)
        {
                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, data_port, &(BTA05R_AN.IO_PORT_0));
        }
        else
        {
                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, data_port, &(BTA05R_AN.IO_PORT_4));
        }

        delay_switch_ta();


        if (AB == 0)
        {
                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
        }
        else
        {
                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
        }


        for (ta_t = 0; ta_t < 32000; ta_t ++)
        {
                // ��������� ������ �� 2-�� ����� (����������)

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_2, CMD_READ, 0x00, &(BTA05R_AN.IO_PORT_2));

                indata = BTA05R_AN.IO_PORT_2;

                indata = indata & mask_ta_port;

                if (indata == mask_ta_port)
                {
                        return 0;
                }

                __NOP();
                __NOP();
                __NOP();
                __NOP();
                __NOP();

                // ������� WDT

                __DI();

                SYS_WDT_START = 0x3C;
                SYS_WDT_REPEAT = 0xA5;

                __EI();

        }

        return 1;
}

