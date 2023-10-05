
#include "mik32_hal_otp.h"

#include "uart_lib.h"
#include "xprintf.h"


OTP_HandleTypeDef hotp;

void SystemClock_Config(void);
static void OTP_Init(void);


int main()
{    

    // PM->CLK_AHB_SET = 0xFFFFFFFF;
    // PM->CLK_APB_P_SET = 0xFFFFFFFF;
    // PM->CLK_APB_M_SET = 0xFFFFFFFF;

    SystemClock_Config();

    UART_Init(UART_0, 3333, UART_CONTROL1_TE_M | UART_CONTROL1_M_8BIT_M, 0, 0);

    OTP_Init();

    uint8_t address = 0;

    uint32_t otp_data_writeTestRow = 0x12345678;
    uint32_t otp_data_writeTestColumn[8] = {0b1, 0b0, 0b1, 0b1, 0b1, 0b1, 0b0, 0b1};
    uint32_t otp_data_writeTestBit = 1;
    uint32_t otp_data_write[8] = {0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC, 0xDDDDDDDD, 0xEEEEEEEE, 0xFFFFFFFF, 0xAAAABBBB, 0xCCCCDDDD};

    uint32_t otp_data_readTestRow = 0;
    uint32_t otp_data_readTestColumn[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint32_t otp_data_readTestBit = 0;
    uint32_t otp_data_read[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint32_t data_length = sizeof(otp_data_read)/sizeof(*otp_data_read);

    /*********************************** Запись ***********************************/
    
    xprintf("Write\n");
    
    HAL_OTP_Write(&hotp, address, otp_data_write, data_length);
    HAL_OTP_WriteTestRow(&hotp, otp_data_writeTestRow);
    HAL_OTP_WriteTestColumn(&hotp, address, otp_data_writeTestColumn, data_length);
    HAL_OTP_WriteTestBit(&hotp, otp_data_writeTestBit);

    for (uint32_t i = 0; i < data_length; i++)
    {
        xprintf("otp_data_write[%d] = 0x%08x\n", i, otp_data_write[i]);
    }

    xprintf("\notp_data_writeTestRow = 0x%08x\n\n", otp_data_writeTestRow);

    for (uint32_t i = 0; i < data_length; i++)
    {
        xprintf("otp_data_writeTestColumn[%d] = 0x%08x\n", i, otp_data_writeTestColumn[i]);
    }

    xprintf("\notp_data_writeTestBit = 0x%08x\n\n", otp_data_writeTestBit);

    /* После окончания операции записи(чтения) до начала следующей за ней операции чтения(записи) должно пройти не менее 1 мкс */
    for (volatile int i = 0; i < 1000000; i++); 

    /*********************************** Чтение ***********************************/
    xprintf("Read\n");

    
    HAL_OTP_Read(&hotp, address, otp_data_read, data_length);
    otp_data_readTestRow = HAL_OTP_ReadTestRow(&hotp);
    HAL_OTP_ReadTestColumn(&hotp, address, otp_data_readTestColumn, data_length);
    otp_data_readTestBit = HAL_OTP_ReadTestBit(&hotp);

    for (uint32_t i = 0; i < data_length; i++)
    {
        xprintf("otp_data_read[%d] = 0x%08x\n", i, otp_data_read[i]);
    }

    xprintf("\notp_data_readTestRow = 0x%08x\n\n", otp_data_readTestRow);

    for (uint32_t i = 0; i < data_length; i++)
    {
        xprintf("otp_data_readTestColumn[%d] = 0x%08x\n", i, otp_data_readTestColumn[i]);
    }

    xprintf("\notp_data_readTestBit = 0x%08x\n", otp_data_readTestBit);

    while (1)
    {    

    }
       
}

void SystemClock_Config(void)
{
    PCC_OscInitTypeDef PCC_OscInit = {0};

    PCC_OscInit.OscillatorEnable = PCC_OSCILLATORTYPE_OSC32K | PCC_OSCILLATORTYPE_OSC32M;   
    PCC_OscInit.OscillatorSystem = PCC_OSCILLATORTYPE_OSC32M;                          
    PCC_OscInit.AHBDivider = 0;                             
    PCC_OscInit.APBMDivider = 0;                             
    PCC_OscInit.APBPDivider = 0;                             
    PCC_OscInit.HSI32MCalibrationValue = 0;                  
    PCC_OscInit.LSI32KCalibrationValue = 0;
    PCC_OscInit.RTCClockSelection = PCC_RTCCLKSOURCE_NO_CLK;
    PCC_OscInit.RTCClockCPUSelection = PCC_RTCCLKCPUSOURCE_NO_CLK;
    HAL_PCC_OscConfig(&PCC_OscInit);
}

static void OTP_Init(void)
{
    hotp.Instance = OTP;

    hotp.ReadMode = OPT_READ_3STAGES;

    HAL_OTP_Init(&hotp);
}
