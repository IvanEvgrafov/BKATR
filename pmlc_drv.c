#include "pmlc_drv.h"


unsigned char PMLC_STATUS_REG;
unsigned char PMLC_SLAVE_REG;
unsigned char PMLC_BAUD_REG;
unsigned char PMLC_TRAN_CMD_REG;
unsigned char PMLC_TRAN_DATA_REG;
unsigned char PMLC_RECV_CMD_REG;
unsigned char PMLC_RECV_DATA_REG;
unsigned char PMLC_RECV_CRC_REG;

#pragma locate PMLC_STATUS_REG 0xFE80 noinit
#pragma locate PMLC_SLAVE_REG 0xFE88 noinit
#pragma locate PMLC_BAUD_REG 0xFE90 noinit
#pragma locate PMLC_TRAN_CMD_REG 0xFE98 noinit
#pragma locate PMLC_TRAN_DATA_REG 0xFEA0 noinit
#pragma locate PMLC_RECV_CMD_REG 0xFEA8 noinit
#pragma locate PMLC_RECV_DATA_REG 0xFEB0 noinit
#pragma locate PMLC_RECV_CRC_REG 0xFEB8 noinit


//  выбор периферийного устройства , тип: 0=основные/1=резервные

void PMLC_select_device
(
    unsigned char pk_num,
    unsigned char pdev_num,     // номер периферийного устройства
    PMLC_CMD_LIST *list_ptr
)
{
        unsigned char pk_slave_sel;

        // проверка отсутствия передачи

        while ((PMLC_STATUS_REG & SPI_BUSY) == SPI_BUSY);

        // задание номера полукомплекта и ПУ

        pk_slave_sel = pk_num | pdev_num;
        PMLC_SLAVE_REG = pk_slave_sel;
        (*list_ptr).SLAVE_SEL = pk_slave_sel;
}


//  выбор скорости передачи КС-ПМ

void PMLC_baud_rate
(
    unsigned char baud_rate,
    PMLC_CMD_LIST *list_ptr
)
{

        // проверка отсутствия передачи

        while ((PMLC_STATUS_REG & SPI_BUSY) == SPI_BUSY);

        // задание скорости передачи
        PMLC_BAUD_REG = baud_rate;
        (*list_ptr).BAUD_RATE = baud_rate;
}

// выполнение команды КС-ПМ

unsigned char PMLC_cmd_exec
(
    PMLC_CMD_LIST *list_ptr,
    unsigned char addr,
    unsigned char cmd,
    unsigned char data,
    unsigned char *pdev_reg
)
{
        unsigned char addr_cmd;
        unsigned char stat;
        unsigned char i;

        // вычисление байта адрес+команда

        addr_cmd = addr | cmd;

        for (i=0; i < 3; i=i++)
        {

                PMLC_TRAN_CMD_REG = addr_cmd;
                PMLC_TRAN_DATA_REG = data;

                (*list_ptr).SEND_ADDR = addr_cmd;
                (*list_ptr).SEND_DATA = data;

                do {
                        (*list_ptr).STATUS = PMLC_STATUS_REG;
                } while (((*list_ptr).STATUS & XFR_DONE) != XFR_DONE);

                // проверка выполнения команды

                stat = ((*list_ptr).STATUS & CRC_STATUS);
                if (stat)
                        stat = EXEC_FAIL;
                else break;
        }


        cmd = cmd | stat;

        if (cmd == CMD_READ)
        *pdev_reg = PMLC_RECV_DATA_REG;
        if (cmd == CMD_WRITE)
        *pdev_reg = data;


        (*list_ptr).CMD_EXEC_STATUS = stat;

        return (stat);
}



