#include "pmlc_drv.h"

extern unsigned char g1_go, g2_go, h1_go, h2_go;
extern unsigned char code_av;

extern unsigned char vent_gh, no_off_ta, tr;

extern unsigned char ta_g,ta_h;

extern unsigned char ta_tm,tm1,tm2,tm3,tm4;

extern PMLC_CMD_LIST PMLC_TASK;

extern bta05r_type BTA05R_AN;
extern bta06r_type BTA06R_AN;

extern void delay_switch_ta();

extern unsigned char num_AB;

extern unsigned char tm_2;

extern unsigned short on_ta(unsigned char num_ta);
extern unsigned short off_ta(unsigned char num_ta);


// Команда включения заданного автомата в горячем контуре

void kontur_g()
{
        int c;

        tr = 1;

        if ((ta_g == 0x3) || (ta_g == 0xC) || (ta_g == 0x30) || (ta_g == 0xC0))
        {
                goto sel_dev_g;
        }
        else
        {
                ta_g = 0x3;
        }

sel_dev_g:


        if (num_AB == 0)
        {
                PMLC_select_device(NOM_PDEV, PDEV_3, &PMLC_TASK);
                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, (ta_g & 0x55), &(BTA05R_AN.IO_PORT_0));
                delay_switch_ta();
                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
        }
        else
        {
                PMLC_select_device(RSV_PDEV, PDEV_3, &PMLC_TASK);
                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, (ta_g & 0x55), &(BTA05R_AN.IO_PORT_4));
                delay_switch_ta();
                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
        }

        // ожидаем включения автомата

        c = 0;

        while (c<32000)
        {
                c = c + 1;

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_2, CMD_READ, 0x00, &(BTA05R_AN.IO_PORT_2));

                ta_tm = ~BTA05R_AN.IO_PORT_2;

                if ((ta_tm & ta_g) == ta_g)
                {
                        break;
                }
        }

        if ((ta_tm & ta_g) != ta_g)
        {
                if (ta_g == 0x3)
                {
                        code_av = 15;

                        ta_g = 0xC;

                        off_ta(17);

                        goto second_operation_g;
                }

                if (ta_g == 0xC)
                {
                        code_av = 16;

                        ta_g = 0x3;

                        off_ta(18);

                        goto second_operation_g;
                }

                if (ta_g == 0x30)
                {
                        code_av = 17;

                        ta_g = 0xC0;

                        off_ta(19);

                        goto second_operation_g;
                }

                if (ta_g == 0xC0)
                {
                        code_av = 18;

                        off_ta(20);

                        ta_g = 0x30;
                }

// если не включился нужный автомат, то включаем запасной

second_operation_g:


                if (num_AB == 0)
                {
                        PMLC_select_device(NOM_PDEV, PDEV_3, &PMLC_TASK);
                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, (ta_g & 0x55), &(BTA05R_AN.IO_PORT_0));
                        delay_switch_ta();
                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
                }
                else
                {
                        PMLC_select_device(RSV_PDEV, PDEV_3, &PMLC_TASK);
                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, (ta_g & 0x55), &(BTA05R_AN.IO_PORT_4));
                        delay_switch_ta();
                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
                }

                // ожидание выполнения операции

                c = 0;

                while (c<32000)
                {
                        c = c + 1;

                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_2, CMD_READ, 0x00, &(BTA05R_AN.IO_PORT_2));

                        ta_tm = ~BTA05R_AN.IO_PORT_2;

                        if ((ta_tm & ta_g) == ta_g)
                        {
                                break;
                        }
                }


                if ((ta_tm & ta_g) != ta_g)
                {
                        if (ta_g == 0x3)
                        {
                                code_av = 15;

                                off_ta(17);

                                g1_go = 1;
                        }

                        if (ta_g == 0xC)
                        {
                                code_av = 16;

                                off_ta(18);

                                g1_go = 1;
                        }

                        if (ta_g == 0x30)
                        {
                                code_av = 17;

                                off_ta(19);

                                g2_go = 1;
                        }

                        if (ta_g == 0xC0)
                        {
                                code_av = 18;

                                off_ta(20);

                                g2_go = 1;
                        }
                }
                else
                {
                        goto kontur_g_ok;
                }
        }
        else
        {

kontur_g_ok:

//   признак удачного выполнения операции

                tr = 0;
        }
}


// ????????? ???????? ?? ???????? ?????? ?? ???????? ??? ?????????

void kontur_h()
{
        int c;

        tr = 1;


        if ((ta_h == 0x3) || (ta_h == 0xC) || (ta_h == 0x30) || (ta_h == 0xC0))
        {
                goto sel_dev_h;
        }
        else
        {
                ta_h = 0x3;
        }

sel_dev_h:

        if (num_AB == 0)
        {
                PMLC_select_device(NOM_PDEV, PDEV_2, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, (ta_h & 0x55), &(BTA05R_AN.IO_PORT_0));
                delay_switch_ta();
                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
        }
        else
        {
                PMLC_select_device(RSV_PDEV, PDEV_2, &PMLC_TASK);
                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, (ta_h & 0x55), &(BTA05R_AN.IO_PORT_4));
                delay_switch_ta();
                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
        }
        // ожидаем включения

        c = 0;


        while (c<32000)
        {
                c = c + 1;

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_2, CMD_READ, 0x00, &(BTA05R_AN.IO_PORT_2));

                ta_tm = ~BTA05R_AN.IO_PORT_2;

                if ((ta_tm & ta_h) == ta_h)
                {
                        break;
                }
        }

        if ((ta_tm & ta_h) != ta_h)
        {
                if (ta_h == 0x3)
                {
                        code_av = 19;

                        off_ta(21);

                        ta_h = 0xC;

                        goto second_operation_h;
                }

                if (ta_h == 0xC)
                {
                        code_av = 20;

                        off_ta(22);

                        ta_h = 0x3;

                        goto second_operation_h;
                }

                if (ta_h == 0x30)
                {
                        code_av = 21;

                        off_ta(23);

                        ta_h = 0xC0;

                        goto second_operation_h;

                }

                if (ta_h == 0xC0)
                {
                        code_av = 22;

                        off_ta(24);

                        ta_h = 0x30;
                }

// включаем запасной автомат в случае невключения первого

second_operation_h:

                if (num_AB == 0)
                {
                        PMLC_select_device(NOM_PDEV, PDEV_2, &PMLC_TASK);
                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, (ta_h & 0x55), &(BTA05R_AN.IO_PORT_0));
                        delay_switch_ta();
                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
                }
                else
                {
                        PMLC_select_device(RSV_PDEV, PDEV_2, &PMLC_TASK);
                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, (ta_h & 0x55), &(BTA05R_AN.IO_PORT_4));
                        delay_switch_ta();
                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
                }
        // ??????? ?????????? ? ?????

                c = 0;

                while (c<32000)
                {
                        c = c + 1;

                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_2, CMD_READ, 0x00, &(BTA05R_AN.IO_PORT_2));

                        ta_tm = ~BTA05R_AN.IO_PORT_2;

                        if ((ta_tm & ta_h) == ta_h)
                        {
                                break;
                        }
                }

                if ((ta_tm & ta_h) != ta_h)
                {
                        if (ta_h == 0x3)
                        {
                                code_av = 19;

                                off_ta(21);

                                h1_go = 1;
                        }

                        if (ta_h == 0xC)
                        {
                                code_av = 20;

                                off_ta(22);

                                h1_go = 1;
                        }

                        if (ta_h == 0x30)
                        {
                                code_av = 21;

                                off_ta(23);

                                h2_go = 1;
                        }

                        if (ta_h == 0xC0)
                        {
                                code_av = 22;

                                off_ta(24);

                                h2_go = 1;
                        }

                }
                else
                {
                        goto kontur_h_ok;
                }

        }
        else
        {

kontur_h_ok:
                tr = 0;
        }
}


// отключение горячего контура

void otkl_kontur_g()
{

        if (num_AB == 0)
        {
                PMLC_select_device(NOM_PDEV, PDEV_3, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0xAA, &(BTA05R_AN.IO_PORT_0));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
        }
        else
        {

                PMLC_select_device(RSV_PDEV, PDEV_3, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0xAA, &(BTA05R_AN.IO_PORT_4));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
        }
}


// отключение холодного контура

void otkl_kontur_h()
{

        if (num_AB == 0)
        {
                PMLC_select_device(NOM_PDEV, PDEV_2, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0xAA, &(BTA05R_AN.IO_PORT_0));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
        }
        else
        {
                PMLC_select_device(RSV_PDEV, PDEV_2, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0xAA, &(BTA05R_AN.IO_PORT_4));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
        }
}


// отключение Г1

void otkl_kont_g1()
{

        if (num_AB == 0)
        {
                PMLC_select_device(NOM_PDEV, PDEV_3, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0xA, &(BTA05R_AN.IO_PORT_0));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
        }
        else
        {
                PMLC_select_device(RSV_PDEV, PDEV_3, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0xA, &(BTA05R_AN.IO_PORT_4));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
        }
}


// отключение Х1

void otkl_kont_h1()
{

        if (num_AB == 0)
        {
                PMLC_select_device(NOM_PDEV, PDEV_2, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0xA, &(BTA05R_AN.IO_PORT_0));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
        }
        else
        {
                PMLC_select_device(RSV_PDEV, PDEV_2, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0xA, &(BTA05R_AN.IO_PORT_4));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
        }
}


// отключение Г2

void otkl_kont_g2()
{

        if (num_AB == 0)
        {
                PMLC_select_device(NOM_PDEV, PDEV_3, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0xA0, &(BTA05R_AN.IO_PORT_0));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
        }
        else
        {
                PMLC_select_device(RSV_PDEV, PDEV_3, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0xA0, &(BTA05R_AN.IO_PORT_4));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
        }
}


// отключение Х2

void otkl_kont_h2()
{

        if (num_AB == 0)
        {
                PMLC_select_device(NOM_PDEV, PDEV_2, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0xA0, &(BTA05R_AN.IO_PORT_0));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
        }
        else
        {
                PMLC_select_device(RSV_PDEV, PDEV_2, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0xA0, &(BTA05R_AN.IO_PORT_4));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
        }
}



void vkl_kont_g1()
{
        int c;

        tr = 1;

        if (num_AB == 0)
        {
                PMLC_select_device(NOM_PDEV, PDEV_3, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0x1, &(BTA05R_AN.IO_PORT_0));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
        }
        else
        {
                PMLC_select_device(RSV_PDEV, PDEV_3, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0x1, &(BTA05R_AN.IO_PORT_4));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
        }


        c = 0;

        while (c<32000)
        {
                c = c + 1;

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_2, CMD_READ, 0x00, &(BTA05R_AN.IO_PORT_2));

                ta_tm = ~BTA05R_AN.IO_PORT_2;

                if ((ta_tm & 0x3) == 0x3)
                {
                        break;
                }
        }

        // ожидаем выполнения команды

        if ((ta_tm & 0x3) != 0x3)
        {
                code_av = 15;

                off_ta(17);


                if (num_AB == 0)
                {
                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0x4, &(BTA05R_AN.IO_PORT_0));

                        delay_switch_ta();

                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
                }
                else
                {
                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0x4, &(BTA05R_AN.IO_PORT_4));

                        delay_switch_ta();

                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
                }


                c = 0;

                while (c<32000)
                {
                        c = c + 1;

                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_2, CMD_READ, 0x00, &(BTA05R_AN.IO_PORT_2));

                        ta_tm = ~BTA05R_AN.IO_PORT_2;

                        if ((ta_tm & 0xC) == 0xC)
                        {
                                break;
                        }
                }

                if ((ta_tm & 0xC) != 0xC)
                {
                        code_av = 16;

                        off_ta(18);

                        g1_go = 1;
                }
                else
                {
                        goto vkl_g1_ok;
                }
        }
        else
        {

vkl_g1_ok:

                tr = 0;
        }
}


void vkl_kont_h1()
{
        int c;

        tr = 1;

        if (num_AB == 0)
        {
                PMLC_select_device(NOM_PDEV, PDEV_2, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0x1, &(BTA05R_AN.IO_PORT_0));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
        }
        else
        {
                PMLC_select_device(RSV_PDEV, PDEV_2, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0x1, &(BTA05R_AN.IO_PORT_4));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
        }

        c = 0;

        while (c<32000)
        {
                c = c + 1;

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_2, CMD_READ, 0x00, &(BTA05R_AN.IO_PORT_2));

                ta_tm = ~BTA05R_AN.IO_PORT_2;

                if ((ta_tm & 0x3) == 0x3)
                {
                        break;
                }
        }

        // ожидание

        if ((ta_tm & 0x3) != 0x3)
        {
                code_av = 19;

                off_ta(21);

                if (num_AB == 0)
                {
                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0x4, &(BTA05R_AN.IO_PORT_0));

                        delay_switch_ta();

                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
                }
                else
                {
                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0x4, &(BTA05R_AN.IO_PORT_4));

                        delay_switch_ta();

                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
                }

                c = 0;

                while (c<32000)
                {
                        c = c + 1;

                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_2, CMD_READ, 0x00, &(BTA05R_AN.IO_PORT_2));

                        ta_tm = ~BTA05R_AN.IO_PORT_2;

                        if ((ta_tm & 0xC) == 0xC)
                        {
                                break;
                        }
                }

                if ((ta_tm & 0xC) != 0xC)
                {
                        code_av = 20;

                        off_ta(22);

                        h1_go = 1;
                }
                else
                {
                        goto vkl_h1_ok;
                }
        }
        else
        {

vkl_h1_ok:

                tr = 0;
        }
}



void vkl_kont_g2()
{
        int c;

        tr = 1;

        if (num_AB == 0)
        {
                PMLC_select_device(NOM_PDEV, PDEV_3, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0x10, &(BTA05R_AN.IO_PORT_0));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
        }
        else
        {
                PMLC_select_device(RSV_PDEV, PDEV_3, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0x10, &(BTA05R_AN.IO_PORT_4));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
        }

        c = 0;

        while (c<32000)
        {
                c = c + 1;

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_2, CMD_READ, 0x00, &(BTA05R_AN.IO_PORT_2));

                ta_tm = ~BTA05R_AN.IO_PORT_2;

                if ((ta_tm & 0x30) == 0x30)
                {
                        break;
                }
        }


        if ((ta_tm & 0x30) != 0x30)
        {
                code_av = 17;

                off_ta(19);

                if (num_AB == 0)
                {
                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0x40, &(BTA05R_AN.IO_PORT_0));

                        delay_switch_ta();

                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
                }
                else
                {
                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0x40, &(BTA05R_AN.IO_PORT_4));

                        delay_switch_ta();

                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
                }


                c = 0;

                while (c<32000)
                {
                        c = c + 1;

                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_2, CMD_READ, 0x00, &(BTA05R_AN.IO_PORT_2));

                        ta_tm = ~BTA05R_AN.IO_PORT_2;

                        if ((ta_tm & 0xC0) == 0xC0)
                        {
                                break;
                        }
                }

                if ((ta_tm & 0xC0) != 0xC0)
                {
                        code_av = 18;

                        off_ta(20);

                        g2_go = 1;
                }
                else
                {
                        goto vkl_g2_ok;
                }
        }
        else
        {

vkl_g2_ok:

                tr = 0;
        }
}



void vkl_kont_h2()
{
        int c;

        tr = 1;

        if (num_AB == 0)
        {
                PMLC_select_device(NOM_PDEV, PDEV_2, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0x10, &(BTA05R_AN.IO_PORT_0));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
        }
        else
        {
                PMLC_select_device(RSV_PDEV, PDEV_2, &PMLC_TASK);

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0x10, &(BTA05R_AN.IO_PORT_4));

                delay_switch_ta();

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
        }

        c = 0;

        while (c<32000)
        {
                c = c + 1;

                PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_2, CMD_READ, 0x00, &(BTA05R_AN.IO_PORT_2));

                ta_tm = ~BTA05R_AN.IO_PORT_2;

                if ((ta_tm & 0x30) == 0x30)
                {
                        break;
                }
        }


        if ((ta_tm & 0x30) != 0x30)
        {
                code_av = 21;

                off_ta(23);

                if (num_AB == 0)
                {
                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0x40, &(BTA05R_AN.IO_PORT_0));

                        delay_switch_ta();

                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_0, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_0));
                }
                else
                {
                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0x40, &(BTA05R_AN.IO_PORT_4));

                        delay_switch_ta();

                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_4, CMD_WRITE, 0, &(BTA05R_AN.IO_PORT_4));
                }

                c = 0;

                while (c<32000)
                {
                        c = c + 1;

                        PMLC_cmd_exec(&PMLC_TASK, ADDR_IO_PORT_2, CMD_READ, 0x00, &(BTA05R_AN.IO_PORT_2));

                        ta_tm = ~BTA05R_AN.IO_PORT_2;

                        if ((ta_tm & 0xC0) == 0xC0)
                        {
                                break;
                        }
                }

                if ((ta_tm & 0xC0) != 0xC0)
                {
                        code_av = 22;

                        off_ta(24);

                        h2_go = 1;
                }
                else
                {
                        goto vkl_h2_ok;
                }
        }
        else
        {

vkl_h2_ok:

                tr = 0;
        }
}



void off_all_nag()
{

                if ((tm_2 & 0x1) != 0)
                {
                        if (off_ta(9) != 0)
                        {
                                code_av = 41;
                        }
                }

                if ((tm_2 & 0x2) != 0)
                {
                        if (off_ta(10) != 0)
                        {
                                code_av = 42;
                        }
                }


                if ((tm_2 & 0x4) != 0)
                {
                        if (off_ta(11) != 0)
                        {
                                code_av = 43;
                        }
                }

                if ((tm_2 & 0x8) != 0)
                {
                        if (off_ta(12) != 0)
                        {
                                code_av = 44;
                        }
                }
}









