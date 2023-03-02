extern unsigned char tm_1_l, tm_2_l, tm_3_l, tm_4_l;
extern unsigned char tm_1, tm_2, tm_3, tm_4;

extern unsigned char tm_data_5, tm_data_6;

extern unsigned char g1_go, g2_go, h1_go, h2_go;
extern unsigned char code_av;



void change_tm_1()
{
unsigned char num_b, tm_buf;


        num_b = 1;

        if  ((tm_1_l - tm_1) > 0)
        {
                tm_buf = tm_1_l - tm_1;
        }
        else
        {
                tm_buf = tm_1 - tm_1_l;
        }

        while (num_b <=7)
        {
                if ((tm_buf & 0x1) == 0x1)
                {
                        goto set_bit_tm1;
                }
                else
                {
                        tm_buf = tm_buf >> 1;
                }

                num_b = num_b + 1;
        }

set_bit_tm1:

        if  ((tm_1_l - tm_1) > 0)
        {
                code_av = 64 + num_b;
        }
        else
        {
                code_av = 96 + num_b;
        }

}



void change_tm_2()
{
unsigned char num_b, tm_buf;

        num_b = 1;

        if  ((tm_2_l - tm_2) > 0)
        {
                tm_buf = tm_2_l - tm_2;
        }
        else
        {
                tm_buf = tm_2 - tm_2_l;
        }

        while (num_b <=7)
        {
                if ((tm_buf & 0x1) == 0x1)
                {
                        goto set_bit_tm2;
                }
                else
                {
                        tm_buf = tm_buf >> 1;
                }

                num_b = num_b + 1;
        }

set_bit_tm2:

        if  ((tm_2_l - tm_2) > 0)
        {
                if (num_b > 6)
                {
                        code_av = 88 + num_b;
                }
                else
                {
                        code_av = 72 + num_b;
                }
        }
        else
        {
                if (num_b > 6)
                {
                        code_av = 120 + num_b;
                }
                else
                {
                        code_av = 104 + num_b;
                }
        }
}


void change_tm_3()
{

        // �1 ���
        if (((tm_3_l & 0x1) != (tm_3 & 0x1)) && ((tm_3 & 0x1)==0))
        {
                code_av = 79; // ���. ����
                g1_go = 1;
        }

        if (((tm_3_l & 0x1) != (tm_3 & 0x1)) && ((tm_3 & 0x1)!=0))
        {
                code_av = 111; // ���. ���
        }

        // �1 ���
        if (((tm_3_l & 0x2) != (tm_3 & 0x2)) && ((tm_3 & 0x2)==0))
        {
                code_av = 80;
                g1_go = 1;
        }

        if (((tm_3_l & 0x2) != (tm_3 & 0x2)) && ((tm_3 & 0x2)!=0))
        {
                code_av = 112;
        }

        // �2 ���
        if (((tm_3_l & 0x4) != (tm_3 & 0x4)) && ((tm_3 & 0x4)==0))
        {
                code_av = 81; // ���. ����
                g2_go = 1;
        }

        if (((tm_3_l & 0x4) != (tm_3 & 0x4)) && ((tm_3 & 0x4)!=0))
        {
                code_av = 113; // ���. ���
        }

        // �2 ���
        if (((tm_3_l & 0x8) != (tm_3 & 0x8)) && ((tm_3 & 0x8)==0))
        {

                code_av = 82;
                g2_go = 1;

        }

        if (((tm_3_l & 0x8) != (tm_3 & 0x8)) && ((tm_3 & 0x8)!=0))
        {
                code_av = 114;
        }

        // �1 ���
        if (((tm_3_l & 0x10) != (tm_3 & 0x10)) && ((tm_3 & 0x10)==0))
        {
                code_av = 83; // ���. ����
                h1_go = 1;
        }

        if (((tm_3_l & 0x10) != (tm_3 & 0x10)) && ((tm_3 & 0x10)!=0))
        {
                code_av = 115; // ���. ���
        }

        // �1 ���
        if (((tm_3_l & 0x20) != (tm_3 & 0x20)) && ((tm_3 & 0x20)==0))
        {
                code_av = 84;
                h1_go = 1;
        }

        if (((tm_3_l & 0x20) != (tm_3 & 0x20)) && ((tm_3 & 0x20)!=0))
        {
                code_av = 116;
        }

        // �2 ���
        if (((tm_3_l & 0x40) != (tm_3 & 0x40)) && ((tm_3 & 0x40)==0))
        {
                code_av = 85; // ���. ����
                h2_go = 1;
        }

        if (((tm_3_l & 0x40) != (tm_3 & 0x40)) && ((tm_3 & 0x40)!=0))
        {
                code_av = 117; // ���. ���
        }


        // �2 ���
        if (((tm_3_l & 0x80) != (tm_3 & 0x80)) && ((tm_3 & 0x80)==0))
        {
                code_av = 86;
                h2_go = 1;
        }

        if (((tm_3_l & 0x80) != (tm_3 & 0x80)) && ((tm_3 & 0x80)!=0))
        {
                code_av = 118;
        }
}


 void change_tm_4()
{
        // ���� ���

        if (((tm_4_l & 0x1) != (tm_4 & 0x1)) && ((tm_4 & 0x1)==0))
        {
                code_av = 87; // ���. ����
        }

        if (((tm_4_l & 0x1) != (tm_4 & 0x1)) && ((tm_4 & 0x1)!=0))
        {
                code_av = 119; // ���. ���
        }

        // ���� ���
        if (((tm_4_l & 0x2) != (tm_4 & 0x2)) && ((tm_4 & 0x2)==0))
        {
                code_av = 88;
        }

        if (((tm_4_l & 0x2) != (tm_4 & 0x2)) && ((tm_4 & 0x2)!=0))
        {
                code_av = 120;
        }

        // ���� ���
        if (((tm_4_l & 0x4) != (tm_4 & 0x4)) && ((tm_4 & 0x4)==0))
        {
                code_av = 89; // ���. ����
        }

        if (((tm_4_l & 0x4) != (tm_4 & 0x4)) && ((tm_4 & 0x4)!=0))
        {
                code_av = 121; // ���. ���
        }

        // ���� ���
        if (((tm_4_l & 0x8) != (tm_4 & 0x8)) && ((tm_4 & 0x8)==0))
        {
                code_av = 90;
        }

        if (((tm_4_l & 0x8) != (tm_4 & 0x8)) && ((tm_4 & 0x8)!=0))
        {
                code_av = 122;
        }

        // ���� ���
        if (((tm_4_l & 0x10) != (tm_4 & 0x10)) && ((tm_4 & 0x10)==0))
        {
                code_av = 91; // ���. ����
        }

        if (((tm_4_l & 0x10) != (tm_4 & 0x10)) && ((tm_4 & 0x10)!=0))
        {
                code_av = 123; // ���. ���
        }

        // ���� ���
        if (((tm_4_l & 0x20) != (tm_4 & 0x20)) && ((tm_4 & 0x20)==0))
        {
                code_av = 92;
        }

        if (((tm_4_l & 0x20) != (tm_4 & 0x20)) && ((tm_4 & 0x20)!=0))
        {
                code_av = 124;
        }

        // ����� ���
        if (((tm_4_l & 0x40) != (tm_4 & 0x40)) && ((tm_4 & 0x40)==0))
        {
                code_av = 95; // ���. ����
        }

        if (((tm_4_l & 0x40) != (tm_4 & 0x40)) && ((tm_4 & 0x40)!=0))
        {
                code_av = 127; // ���. ���
        }

        // ����� ���
        if (((tm_4_l & 0x80) != (tm_4 & 0x80)) && ((tm_4 & 0x80)==0))
        {
                code_av = 96;
        }

        if (((tm_4_l & 0x80) != (tm_4 & 0x80)) && ((tm_4 & 0x80)!=0))
        {
                code_av = 128;
        }
}

// ��������� ������� �� ���������� �����������

void analiz_vent_err()
{
        if ((tm_3 & 0x1) != 0)
        {
                code_av = 47;
        }

        if ((tm_3 & 0x2) != 0)
        {
                code_av = 48;
        }

        if ((tm_3 & 0x4) != 0)
        {
                code_av = 49 ;
        }

        if ((tm_3 & 0x8) != 0)
        {
                code_av = 50;
        }


        if ((tm_3 & 0x10) != 0)
        {
                code_av = 51;
        }

        if ((tm_3 & 0x20) != 0)
        {
                code_av = 52;
        }

        if ((tm_3 & 0x40) != 0)
        {
                code_av = 53 ;
        }

        if ((tm_3 & 0x80) != 0)
        {
                code_av = 54;
        }
}

