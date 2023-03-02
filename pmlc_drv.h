//------------------------------------------------------------------//
// Title:     Заголовочный файл драйвера КС-ПМ устройства БК01Р     //
// File:      pmlc_drv.h                                            //
// EDA Tool:  CodeMaster-96                                         //
// Revision:  1.0                                                   //
// Rev.Date:  08/12/2016                                            //
// Rev.Date:  24/07/2017                                            //
// Rev.Date:  04/08/2017                                            //
// Rev.Date:  29/09/2017                                            //
// Rev.Note:                                                        //
// Author(s): Сазонов С.В.                                          //
//------------------------------------------------------------------//
// Note: Сокращения:                                                //
//       КС-ПМ - Контроллер Связи Процессорного Модуля              //
//       PMLC - Processor Module Link Controller                    //
//       ПУ-периферийное устройство                                 //
//       PDev - Periferal DEVice                                    //
//       КС-ПУ - Контроллер Связи Периферийного Устройства          //
//       PDLC - Peripheral Device Link Controller                   //
//------------------------------------------------------------------//
// Note2: Прерывания не используются !!!!!                          //
//------------------------------------------------------------------//
// проверка повторного переопределения
#ifndef PMLC_DRIVER_H
#define PMLC_DRIVER_H

// Номер КС-ПУ (и, соответственно, ПУ)
#define NOM_PDEV          0x00    // Основные ПУ
#define RSV_PDEV          0x80    // Резервные ПУ

// Номер периферийного устройства. Используется как шаблон.
#define PDEV_0       0x00    // устройство-0
#define PDEV_1       0x01    // устройство-0
#define PDEV_2       0x02    // устройство-0
#define PDEV_3       0x03    // устройство-0
#define PDEV_4       0x04    // устройство-0

// Типы команд КС-ПМ
#define CMD_NOP      0x00    // команда NOP ("нет операции")
#define CMD_WRITE    0x04    // команда WRITE ("запись данных")
#define CMD_READ     0x08    // команда READ ("чтение данных")
#define CMD_DIAG     0x0C    // команда DIAG ("самодиагностика")

// биты регистров управления/состояния КС-ПМ (bits field masks)
#define SPI_BUSY     0x01    // Передача пакета данных
#define XFR_DONE     0x02    // Завершение передачи пакета данных
#define IRQ_EN       0x04    // Разрешение прерывания после завершения передачи пакета данных
#define IRQ          0x08    // Прерывание после завершения передачи пакета данных
#define CRC_IRQ_EN   0x20    // Разрешение прерывания при ошибке CRC-суммы пакета данных
#define CRC_STATUS   0x40    // Результат проверки CRC-суммы пакета данных

// скорость передачи
#define R_12MBPS     0x0     // 12 МБит/с
#define R_6MBPS      0x1     //  6 МБит/с
#define R_3MBPS      0x2     //  3 МБит/с
#define R_1MBPS      0x3     // 1.5 МБит/с

// статус выполнения команды КС-ПМ
// и значение возвращаемое функцией PMLC_cmd_exec (для внешнего употребления)
#define EXEC_OK      0x00    // команда выполнена успешно
#define EXEC_FAIL    0xFF    // команда выполнена с ошибкой

// значение возвращаемые драйверами ПУ (для внешнего употребления-short/int)
#define PMLC_CMD_OK   0      // команда КС-ПМ выполнена успешно
                             // или возвращаются запрашиваемые данные
#define PMLC_CMD_ERR  -1     // команда КС-ПМ выполнена с ошибкой

// Шаблон структуры команды КС-ПМ.
//Используется ЕДИНСТВЕННЫЙ экземпляр команды КС-ПМ !!!
typedef struct {
   unsigned char CMD_EXEC_STATUS;   // флаг завершения команды:0=OK/FF=ошибка
   unsigned char SLAVE_SEL;         // регистр задания номера ПУ
   unsigned char BAUD_RATE;         // регистр задания скорости передачи
   unsigned char STATUS;            // регистр состояния КС-ПМ
   unsigned char CTRL;              // регистр управления КС-ПМ
   unsigned char SEND_ADDR;         // регистр адреса/команды передатчика
   unsigned char SEND_DATA;         // регистр данных передатчика
   unsigned char RCV_ADDR_CMD;      // регистр адреса/команды приёмника
   unsigned char RCV_DATA;          // регистр данных приёмника
   unsigned char RCV_CRC;           // регистр CRC-суммы приёмника
   } PMLC_CMD_LIST;

// Адреса регистров КС-ПУ
#define ADDR_IO_PORT_0    0x00    // Порт ввода-вывода 0
#define ADDR_IO_PORT_1    0x10    // Порт ввода-вывода 1
#define ADDR_IO_PORT_2    0x20    // Порт ввода-вывода 2
#define ADDR_IO_PORT_3    0x30    // Порт ввода-вывода 3
#define ADDR_IO_PORT_4    0x40    // Порт ввода-вывода 4
#define ADDR_IO_PORT_5    0x50    // Порт ввода-вывода 5
#define ADDR_IO_CFG_REG   0x80    // Регистр конфигурирования портов ввода-вывода
#define ADDR_ADC_GEN_0    0xA0    // Регистр тактового сигнала АЦП-0
#define ADDR_ADC_GEN_1    0xB0    // Регистр тактового сигнала АЦП-1
#define ADDR_ADC_GEN_2    0xC0    // Регистр тактового сигнала АЦП-2
#define ADDR_CTRL_REG     0xE0    // Регистр управления КС-ПУ
#define ADDR_STATUS_REG   0xE0    // Регистр состояния КС-ПУ
#define ADDR_ID_REG       0xF0    // Регистр идентификации

// биты регистров управления/состояния КС-ПУ (bits field masks)
#define CRC_CHK           0x01     // Результат проверки CRC-суммы принятого пакета данных
#define MODE              0x02     // Режим работы КС-ПУ
#define ID_REG_VALUE      0x5A     // значение регистра идентификации КС-ПУ после сброса

// Reference шаблон структуры состояния регистров периферийного устройства
// Используется как образец для КАЖДОГО КС-ПУ КАЖДОГО периферийного устроства.
// Названия полей-НЕ МЕНЯТЬ! Неиспользуемые поля-удалить(закоментировать).
typedef struct {
   unsigned char IO_PORT_0;       // Регистр порта ввода-вывода 0
   unsigned char IO_PORT_1;       // Регистр порта ввода-вывода 1
   unsigned char IO_PORT_2;       // Регистр порта ввода-вывода 2
   unsigned char IO_PORT_3;       // Регистр порта ввода-вывода 3
   unsigned char IO_PORT_4;       // Регистр порта ввода-вывода 4
   unsigned char IO_PORT_5;       // Регистр порта ввода-вывода 5
   unsigned char IO_CFG_REG;      // Регистр конфигурирования портов ввода-вывода
   unsigned char ADC_GEN_0;       // Регистр тактового сигнала АЦП-0
   unsigned char ADC_GEN_1;       // Регистр тактового сигнала АЦП-1
   unsigned char ADC_GEN_2;       // Регистр тактового сигнала АЦП-2
   unsigned char CTRL_REG;        // Регистр управления КС-ПУ
   unsigned char STATUS_REG;      // Регистр состояния КС-ПУ
   unsigned char ID_REG;          // Регистр идентификации
   } pdev_status;


// шаблон структуры состояния регистров устройства БЦВ03Р
typedef struct {
   unsigned char IO_PORT_0;       // Регистр порта ввода-вывода 0
   unsigned char IO_PORT_1;       // Регистр порта ввода-вывода 1
   unsigned char IO_PORT_2;       // Регистр порта ввода-вывода 2
   unsigned char IO_PORT_3;       // Регистр порта ввода-вывода 3
   unsigned char IO_PORT_4;       // Регистр порта ввода-вывода 4
//   unsigned char IO_PORT_5;       // Регистр порта ввода-вывода 5
   unsigned char IO_CFG_REG;      // Регистр конфигурирования портов ввода-вывода
//   unsigned char ADC_GEN_0;       // Регистр тактового сигнала АЦП-0
//   unsigned char ADC_GEN_1;       // Регистр тактового сигнала АЦП-1
//   unsigned char ADC_GEN_2;       // Регистр тактового сигнала АЦП-2
   unsigned char CTRL_REG;        // Регистр управления КС-ПУ
   unsigned char STATUS_REG;      // Регистр состояния КС-ПУ
   unsigned char ID_REG;          // Регистр идентификации
   } bcv03r_type;

// шаблон структуры состояния регистров устройства БЦВ04Р
typedef struct {
//   unsigned char IO_PORT_0;       // Регистр порта ввода-вывода 0
   unsigned char IO_PORT_1;       // Регистр порта ввода-вывода 1
//   unsigned char IO_PORT_2;       // Регистр порта ввода-вывода 2
//   unsigned char IO_PORT_3;       // Регистр порта ввода-вывода 3
//   unsigned char IO_PORT_4;       // Регистр порта ввода-вывода 4
//   unsigned char IO_PORT_5;       // Регистр порта ввода-вывода 5
   unsigned char IO_CFG_REG;      // Регистр конфигурирования портов ввода-вывода
//   unsigned char ADC_GEN_0;       // Регистр тактового сигнала АЦП-0
//   unsigned char ADC_GEN_1;       // Регистр тактового сигнала АЦП-1
//   unsigned char ADC_GEN_2;       // Регистр тактового сигнала АЦП-2
   unsigned char CTRL_REG;        // Регистр управления КС-ПУ
   unsigned char STATUS_REG;      // Регистр состояния КС-ПУ
   unsigned char ID_REG;          // Регистр идентификации
   } bcv04r_type;

// шаблон структуры состояния регистров устройства БАВ03Р
typedef struct {
   unsigned char IO_PORT_0;       // Регистр порта ввода-вывода 0
   unsigned char IO_PORT_1;       // Регистр порта ввода-вывода 1
   unsigned char IO_PORT_2;       // Регистр порта ввода-вывода 2
   unsigned char IO_PORT_3;       // Регистр порта ввода-вывода 3
   unsigned char IO_PORT_4;       // Регистр порта ввода-вывода 4
   unsigned char IO_PORT_5;       // Регистр порта ввода-вывода 5
   unsigned char IO_CFG_REG;      // Регистр конфигурирования портов ввода-вывода
   unsigned char ADC_GEN_0;       // Регистр тактового сигнала АЦП-0
   unsigned char ADC_GEN_1;       // Регистр тактового сигнала АЦП-1
//   unsigned char ADC_GEN_2;       // Регистр тактового сигнала АЦП-2
   unsigned char CTRL_REG;        // Регистр управления КС-ПУ
   unsigned char STATUS_REG;      // Регистр состояния КС-ПУ
   unsigned char ID_REG;          // Регистр идентификации
   } bav03r_type;

// шаблон структуры состояния регистров устройства БТА05Р
typedef struct {
   unsigned char IO_PORT_0;       // Регистр порта ввода-вывода 0
   unsigned char IO_PORT_1;       // Регистр порта ввода-вывода 1
   unsigned char IO_PORT_2;       // Регистр порта ввода-вывода 2
   unsigned char IO_PORT_3;       // Регистр порта ввода-вывода 3
   unsigned char IO_PORT_4;       // Регистр порта ввода-вывода 4
//   unsigned char IO_PORT_5;       // Регистр порта ввода-вывода 5
   unsigned char IO_CFG_REG;      // Регистр конфигурирования портов ввода-вывода
//   unsigned char ADC_GEN_0;       // Регистр тактового сигнала АЦП-0
//   unsigned char ADC_GEN_1;       // Регистр тактового сигнала АЦП-1
//   unsigned char ADC_GEN_2;       // Регистр тактового сигнала АЦП-2
   unsigned char CTRL_REG;        // Регистр управления КС-ПУ
   unsigned char STATUS_REG;      // Регистр состояния КС-ПУ
   unsigned char ID_REG;          // Регистр идентификации
   } bta05r_type;

// шаблон структуры состояния регистров устройства БТА06Р
typedef struct {
   unsigned char IO_PORT_0;       // Регистр порта ввода-вывода 0
   unsigned char IO_PORT_1;       // Регистр порта ввода-вывода 1
   unsigned char IO_PORT_2;       // Регистр порта ввода-вывода 2
   unsigned char IO_PORT_3;       // Регистр порта ввода-вывода 3
   unsigned char IO_PORT_4;       // Регистр порта ввода-вывода 4
   unsigned char IO_PORT_5;       // Регистр порта ввода-вывода 5
   unsigned char IO_CFG_REG;      // Регистр конфигурирования портов ввода-вывода
//   unsigned char ADC_GEN_0;       // Регистр тактового сигнала АЦП-0
//   unsigned char ADC_GEN_1;       // Регистр тактового сигнала АЦП-1
//   unsigned char ADC_GEN_2;       // Регистр тактового сигнала АЦП-2
   unsigned char CTRL_REG;        // Регистр управления КС-ПУ
   unsigned char STATUS_REG;      // Регистр состояния КС-ПУ
   unsigned char ID_REG;          // Регистр идентификации
   } bta06r_type;

//******************************************************************//
//                 Прототипы функций драйвера                       //
//******************************************************************//
// выбор периферийного устройства
void PMLC_select_device (
    unsigned char pk_num,       // тип ПУ: 0=основные/1=резервные
    unsigned char pdev_num,     // номер периферийного устройства
    PMLC_CMD_LIST *list_ptr     // указатель на структуру команды КС-ПМ
    );

// выбор скорости передачи КС-ПМ
void PMLC_baud_rate (
    unsigned char baud_rate,    // скорость передачи пакета данных
    PMLC_CMD_LIST *list_ptr     // указатель на структуру команды КС-ПМ
    );

// выполнение команды КС-ПМ
unsigned char PMLC_cmd_exec (
    PMLC_CMD_LIST *list_ptr,    // указатель на структуру команды КС-ПМ
    unsigned char addr,         // адрес регистра ПУ
    unsigned char cmd,          // команда КС-ПМ
    unsigned char data,         // данные
    unsigned char *pdev_reg     // указатель на адрес конкретного регистра ПУ
    );


// окончание проверки повторного переопределения
#endif
