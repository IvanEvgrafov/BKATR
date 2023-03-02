#include "pmlc_drv.h"
#include "bav03r_drv.h"

extern PMLC_CMD_LIST PMLC_TASK;

extern void delay_7000();

// задание и инициализация структуры состояния регистров устройства БАВ03Р
#pragma data_alignment=2
bav03r_type bav03r_N = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};


//************** Инициализация устройства БАВ03Р *******************//
short init_bav03r(
   PMLC_CMD_LIST *plmc_cmd_ptr,   // указатель на структуру команды КС-ПМ
   bav03r_type *bav03r_ptr        // указатель на структуру устройства БАВ03Р
   )
{
//--- конфигурирование портов ввода-вывода
PMLC_cmd_exec((PMLC_CMD_LIST*)plmc_cmd_ptr, ADDR_IO_CFG_REG, CMD_WRITE, 0x04, &(bav03r_ptr->IO_CFG_REG));
if ((*plmc_cmd_ptr).CMD_EXEC_STATUS)
   return (1);     // команда КС-ПМ выполнена с ошибкой
else return (0);    // команда КС-ПМ выполнена успешно
}


// Выбор канала АЦ-преобразования (одновременно для АЦП-0 и АЦП-1)
short select_adc_channel(

   PMLC_CMD_LIST *plmc_cmd_ptr,   // указатель на структуру команды КС-ПМ
   unsigned char ch_num,          // номер канала АЦП
   bav03r_type *bav03r_ptr        // указатель на структуру устройства БАВ03Р
   )
{
        // выбор канала АЦ-преобразования
        PMLC_cmd_exec((PMLC_CMD_LIST*)plmc_cmd_ptr, ADDR_IO_PORT_2, CMD_WRITE, ch_num, &(bav03r_ptr->IO_PORT_2));
        if ((*plmc_cmd_ptr).CMD_EXEC_STATUS)
        return (1);     // команда КС-ПМ выполнена с ошибкой

                else
        return (0);    // команда КС-ПМ выполнена успешно
}


// Выполнить АЦ-преобразование и получить результат этого преобразования.
// Результат - 16 битное слово (unsigned short), из них значащие - 12 младших разрядов.
// Номер канала АЦП должен быть выбран заранее
short get_adc_data(
        PMLC_CMD_LIST *plmc_cmd_ptr,   // указатель на структуру команды КС-ПМ
        unsigned char adc_num,         // номер АЦП
        bav03r_type *bav03r_ptr        // указатель на структуру устройства БАВ03Р
        )

{
        unsigned short adc_data, d1,d2;


        if (!adc_num)
        {
                // АЦП-0

                // запуск АЦП-0
                PMLC_cmd_exec((PMLC_CMD_LIST*)plmc_cmd_ptr, ADDR_ADC_GEN_0, CMD_WRITE, 0x55, &(bav03r_ptr->ADC_GEN_0));

                //--- чтение младших разрядов АЦП_0 (D[3:0])
                PMLC_cmd_exec((PMLC_CMD_LIST*)plmc_cmd_ptr, ADDR_IO_PORT_4, CMD_READ, 0x0, &(bav03r_ptr->IO_PORT_4));

                //--- чтение старших разрядов АЦП_0 (D[11:4])
                PMLC_cmd_exec((PMLC_CMD_LIST*)plmc_cmd_ptr, ADDR_IO_PORT_5, CMD_READ, 0x0, &(bav03r_ptr->IO_PORT_5));


                d1 = ((bav03r_ptr->IO_PORT_4) >> 4);

                d2 = ((bav03r_ptr->IO_PORT_5) << 4);


                // слияние старших и младших разрядов АЦП-0

                adc_data = d2 | d1 ;

        }
        else
        {                // АЦП-1

                // запуск АЦП-1
                PMLC_cmd_exec((PMLC_CMD_LIST*)plmc_cmd_ptr, ADDR_ADC_GEN_1, CMD_WRITE, 0xAA, &(bav03r_ptr->ADC_GEN_1));

                //--- чтение младших разрядов АЦП_1 (D[7:0])
                PMLC_cmd_exec((PMLC_CMD_LIST*)plmc_cmd_ptr, ADDR_IO_PORT_0, CMD_READ, 0x0, &(bav03r_ptr->IO_PORT_0));

                //--- чтение старших разрядов АЦП_1 (D[11:8])
                PMLC_cmd_exec((PMLC_CMD_LIST*)plmc_cmd_ptr, ADDR_IO_PORT_1, CMD_READ, 0x0, &(bav03r_ptr->IO_PORT_1));


                d1 = (bav03r_ptr->IO_PORT_0);

                d2 = ((bav03r_ptr->IO_PORT_1) << 8);

                adc_data = d2 | d1 ;

        }

        return (adc_data);    // команда КС-ПМ выполнена успешно->возвращаются запрашиваемые данные

}


// процедура считывания данных с АЦП

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


// процедура считывания данных с контрольного АЦП

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

