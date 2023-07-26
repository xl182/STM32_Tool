//
// Created by ALIEN on 2023/7/24.
//

#include "user.h"

HAL_SD_CardInfoTypeDef  SDCardInfo;
extern SD_HandleTypeDef hsd;

FATFS fs; //工作空间
FIL fil; // 文件项目

void printf_sdcard_info(void)
{
    uint64_t CardCap;      	//SD卡容量
    HAL_SD_CardCIDTypeDef SDCard_CID;

    HAL_SD_GetCardCID(&hsd,&SDCard_CID);	//获取CID
    HAL_SD_GetCardInfo(&hsd,&SDCardInfo);                    //获取SD卡信息
    CardCap=(uint64_t)(SDCardInfo.LogBlockNbr)*(uint64_t)(SDCardInfo.LogBlockSize);	//计算SD卡容量
    switch(SDCardInfo.CardType)
    {
        case CARD_SDSC:
        {
            if(SDCardInfo.CardVersion == CARD_V1_X)
                printf("Card Type:SDSC V1\r\n");
            else if(SDCardInfo.CardVersion == CARD_V2_X)
                printf("Card Type:SDSC V2\r\n");
        }
            break;
        case CARD_SDHC_SDXC:printf("Card Type:SDHC\r\n");break;
        default:break;
    }

    printf("Card ManufacturerID: %d \r\n",SDCard_CID.ManufacturerID);				//制造商ID
    printf("CardVersion:         %d \r\n",(uint32_t)(SDCardInfo.CardVersion));		//卡版本号
    printf("Class:               %d \r\n",(uint32_t)(SDCardInfo.Class));		    //
    printf("Card RCA(RelCardAdd):%d \r\n",SDCardInfo.RelCardAdd);					//卡相对地址
    printf("Card BlockNbr:       %d \r\n",SDCardInfo.BlockNbr);						//块数量
    printf("Card BlockSize:      %d \r\n",SDCardInfo.BlockSize);					//块大小
    printf("LogBlockNbr:         %d \r\n",(uint32_t)(SDCardInfo.LogBlockNbr));		//逻辑块数量
    printf("LogBlockSize:        %d \r\n",(uint32_t)(SDCardInfo.LogBlockSize));		//逻辑块大小
    printf("Card Capacity:       %d MB\r\n",(uint32_t)(CardCap>>20));				//卡容量
}

void InitFatFas(void)
{
    int retSD = f_mount(&fs, "", 0);
    if(retSD)
    {
        printf("mount error : %d \r\n",retSD);
        //  Error_Handler();
    }
    else
        printf("mount sucess!!! \r\n");
}

void creat_file(char * filename)
{
    int retSD = f_open(&fil, filename, FA_CREATE_ALWAYS | FA_WRITE); //打开文件，权限包括创建、写（如果没有该文件，会创建该文件）
    if(retSD==FR_OK) printf("\r\ncreater file sucess!!! \r\n");
    else printf("\r\ncreater file error : %d\r\n",retSD);
    // f_close(&fil); //关闭该文件
    //HAL_Delay(100);
}
void write_file(char * data,uint32_t len)
{
    uint32_t byteswritten;
    /*##-3- Write data to the text files ###############################*/
    int retSD = f_write(&fil, data, len, (void *)&byteswritten);
    if(retSD)
        printf(" write file error : %d\r\n",retSD);
    else
    {
        printf(" write file sucess!!! \r\n");
        printf(" write Data[%d] : %s\r\n",byteswritten,data);
    }
    /*##-4- Close the open text files ################################*/
    retSD = f_close(&fil);
    if(retSD)
        printf(" close error : %d\r\n",retSD);
    else
        printf(" close sucess!!! \r\n");
}



void init() {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM2_Init();
    MX_TIM6_Init();

    HAL_TIM_Base_Start_IT(&htim6);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

    gui_init();
    set_lcd_brightness(100);

    MX_SDIO_SD_Init();
    MX_FATFS_Init();
    BSP_SD_Init();
}

int main() {
    init();

    while (1) {
        touch_scan();
        if (touch.x and touch.y) {
            drawPoint(touch.x, touch.y, RED);
        }
        lv_task_handler();
    }
}
