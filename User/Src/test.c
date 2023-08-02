//
// Created by ALIEN on 2023/7/27.
//
#include "test.h"

#define BLOCK_START_ADDR         0x00800     /* Block start address      */
#define NUM_OF_BLOCKS            5   /* Total number of blocks   */
#define BUFFER_WORDS_SIZE        ((BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */

uint8_t Buffer_Tx[512], Buffer_Rx[512] = {0};
uint32_t i;

void printf_fatfs_error(FRESULT fresult) {
    switch (fresult) {
        case FR_OK:
            printf("!!The operation was successful.\r\n");
            break;
        case FR_DISK_ERR:
            printf("!!Hardware I / O driver error.\r\n");
            break;
        case FR_INT_ERR:
            printf("!!Assertion error.\r\n");
            break;
        case FR_NOT_READY:
            printf("!!The physical device is not working.\r\n");
            break;
        case FR_NO_FILE:
            printf("!!Unable to find file.\r\n");
            break;
        case FR_NO_PATH:
            printf("!!The path could not be found.\r\n");
            break;
        case FR_INVALID_NAME:
            printf("!!Invalid pathname.\r\n");
            break;
        case FR_DENIED:
        case FR_EXIST:
            printf("!!Access denied.\r\n");
            break;
        case FR_INVALID_OBJECT:
            printf("!!Invalid file or path.\r\n");
            break;
        case FR_WRITE_PROTECTED:
            printf("!!Logical device write protection.\r\n");
            break;
        case FR_INVALID_DRIVE:
            printf("!!Invalid logical device.\r\n");
            break;
        case FR_NOT_ENABLED:
            printf("!!Invalid workspace.\r\n");
            break;
        case FR_NO_FILESYSTEM:
            printf("!!Invalid file system.\r\n");
            break;
        case FR_MKFS_ABORTED:
            printf("!!Fmkfs function operation failed due to function parameter problem.\r\n");
            break;
        case FR_TIMEOUT:
            printf("!!The operation timed out.\r\n");
            break;
        case FR_LOCKED:
            printf("!!The file is protected.\r\n");
            break;
        case FR_NOT_ENOUGH_CORE:
            printf("!!Long file name support failed to get heap space.\r\n");
            break;
        case FR_TOO_MANY_OPEN_FILES:
            printf("!!Too many files open.\r\n");
            break;
        case FR_INVALID_PARAMETER:
            printf("!!Invalid parameter.\r\n");
            break;
    }
}

void sd_test() {
    FRESULT f_res;                    /* 文件操作结果 */
    UINT fnum;                                  /* 文件成功读写数量 */
    BYTE ReadBuffer[1024] = {0};        /* 读缓冲区 */
    BYTE WriteBuffer[] = "Welcome to the wildfire STM32 development board. Today is a good day to create new file system test files\r\n";
    // 注册一个FatFS设备：SD卡 //
    //在SD卡挂载文件系统，文件系统挂载时会对SD卡初始化
    f_res = f_mount(&SDFatFS, (TCHAR const *) "0:", 1);
    printf_fatfs_error(f_res);
    //----------------------- 格式化测试 ---------------------------//
    // 如果没有文件系统就格式化创建创建文件系统 */
    if (f_res == FR_NO_FILESYSTEM) {
        printf("The SD card does not have a file system yet and will be formatted...\r\n");
        // 格式化 //
        f_res = f_mkfs((TCHAR const *) SDPath, 0, 0);

        if (f_res == FR_OK) {
            printf("The SD card has successfully formatted the file system\r\n");
            // 格式化后，先取消挂载 //
            f_res = f_mount(NULL, (TCHAR const *) SDPath, 1);
            // 重新挂载	//
            f_res = f_mount(&SDFatFS, (TCHAR const *) SDPath, 1);
        } else {
            printf("Format failed\r\n");
        }
    } else if (f_res != FR_OK) {
        printf("The SD card failed to mount the file system.(%d)\r\n", f_res);
        printf_fatfs_error(f_res);
    } else {
        printf("The file system is mounted successfully and can be read and written\r\n");
    }

    //----------------------- 文件系统测试：写测试 -----------------------------//
    // 打开文件，如果文件不存在则创建它 //
    printf("****** A file write test will be performed... ******\r\n");
    f_res = f_open(&SDFile, "0:/unit_sd_rw_test.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if (f_res == FR_OK) {
        printf(
                "Open / create FatFs read / write test file. TXT file successfully, write data to the file.\r\n");
        // 将指定存储区内容写入到文件内 //
        f_res = f_write(&SDFile, WriteBuffer, sizeof(WriteBuffer), &fnum);
        if (f_res == FR_OK) {
            printf("File write success, write byte data:%d\r\n", fnum);
            printf("The data written to the file is: \r\n%s\r\n", WriteBuffer);
        } else {
            printf(" File write failure:(%d)\r\n", f_res);
        }
        // 不再读写，关闭文件 //
        f_close(&SDFile);
    } else {
        printf("Failed to open / create file.\r\n");
    }

    //------------------- 文件系统测试：读测试 ------------------------------------//
    printf("****** File read test is about to take place... ******\r\n");
    f_res = f_open(&SDFile, "0:unit_sd_rw_test.txt", FA_OPEN_EXISTING | FA_READ);
    if (f_res == FR_OK) {
        printf("File opened successfully.\r\n");
        f_res = f_read(&SDFile, ReadBuffer, sizeof(ReadBuffer), &fnum);
        if (f_res == FR_OK) {
            printf("File read successfully, read byte data: %d\r\n", fnum);
            printf("The obtained file data are as follows:\r\n%s \r\n", ReadBuffer);
        } else {
            printf("File read failure:(%d)\r\n", f_res);
        }
    } else {
        printf("Failed to open file.\r\n");
    }
    // 不再读写，关闭文件 //
    f_close(&SDFile);

    f_open(&SDFile, "copy.txt", FA_CREATE_ALWAYS | FA_WRITE);
    f_write(&SDFile, ReadBuffer, sizeof(ReadBuffer), &fnum);
    f_close(&SDFile);

    // 不再使用，取消挂载 //
    f_res = f_mount(NULL, (TCHAR const *) "0", 1);
}


void sdio_test() {
    printf("Micro SD Card Test...\r\n");
/* 检测SD卡是否正常（处于数据传输模式的传输状态） */
    if (HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER) {
        printf("Initialize SD card successfully!\r\n");
// 打印SD卡基本信息
        printf(" SD card information! \r\n");
        printf(" CardCapacity  : %llu \r\n",
                   (unsigned long long) hsd.SdCard.BlockSize * hsd.SdCard.BlockNbr);// 显示容量
        printf(" CardBlockSize : %d \r\n", hsd.SdCard.BlockSize);   // 块大小
        printf(" LogBlockNbr   : %d \r\n", hsd.SdCard.LogBlockNbr);    // 逻辑块数量
        printf(" LogBlockSize  : %d \r\n", hsd.SdCard.LogBlockSize);// 逻辑块大小
        printf(" RCA           : %d \r\n", hsd.SdCard.RelCardAdd);  // 卡相对地址
        printf(" CardType      : %d \r\n", hsd.SdCard.CardType);    // 卡类型
// 读取并打印SD卡的CID信息
        HAL_SD_CardCIDTypeDef sdcard_cid;
        HAL_SD_GetCardCID(&hsd, &sdcard_cid);
        printf(" ManufacturerID: %d \r\n", sdcard_cid.ManufacturerID);
    } else {
        printf("SD card init fail!\r\n");
    }

/* 擦除SD卡块 */
    printf("------------------- Block Erase -------------------------------\r\n");
    if (HAL_SD_Erase(&hsd, BLOCK_START_ADDR, NUM_OF_BLOCKS) == HAL_OK) {
/* Wait until SD cards are ready to use for new operation */
        while (HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER) {
        }
        printf("\r\nErase Block Success!\r\n");
    } else {
        printf("\r\nErase Block Failed!\r\n");
    }
/* 填充缓冲区数据 */
    memset(Buffer_Tx, 0x15, sizeof(Buffer_Tx));
/* 向SD卡块写入数据 */
    printf("------------------- Write SD card block data Test ------------------\r\n");
    if (HAL_SD_WriteBlocks(&hsd, Buffer_Tx, BLOCK_START_ADDR, NUM_OF_BLOCKS, 10) == HAL_OK) {
        while (HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER) {
        }
        printf("\r\nWrite Block Success!\r\n");
        printf("\r\n");
    } else {
        printf("\r\nWrite Block Failed!\r\n");
    }
/* 读取操作之后的数据 */
    printf("------------------- Read SD card block data after Write ------------------\r\n");
    if (HAL_SD_ReadBlocks(&hsd, Buffer_Rx, BLOCK_START_ADDR, NUM_OF_BLOCKS, 10) == HAL_OK) {
        while (HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER) {
        }
        printf("\r\nRead Block Success!\r\n");
        for(i = 0; i < sizeof(Buffer_Rx); i++)
        {
            printf("%d %d ", i, Buffer_Rx[i]);
        }
        printf("\r\n");
    } else {
        printf("\r\nRead Block Failed!\r\n");
    }
}

void lv_fs_test() {
#include "guider_customer_fonts.h"
    static lv_style_t label_style;									// 创建一个风格
    lv_style_init(&label_style);									// 初始化风格
    lv_style_set_text_font(&label_style, LV_STATE_DEFAULT, &JetBrainsMono);// 设置风格的字体

    lv_obj_t * textlabel = lv_label_create(lv_scr_act(), NULL);			// 在主屏幕创建一个标签
    lv_label_set_long_mode(textlabel, LV_LABEL_LONG_BREAK);				// 标签长内容框，保持控件宽度，内容过长就换行
    lv_obj_set_width(textlabel, 320);									// 设置标签宽度
    lv_label_set_recolor(textlabel, true);								// 使能字符命令重新对字符上色
    lv_label_set_text(textlabel, "123abcABC");	// 设置显示文本
    lv_obj_align(textlabel, NULL, LV_ALIGN_CENTER, 0, -60);				// 对齐到中心偏下
    lv_obj_add_style(textlabel, LV_LABEL_PART_MAIN, &label_style);		// 应用效果风格

//    printf("begin read\r\n");
//    uint8_t text_buff[144];//如bin文件存在SPI FLASH可使用此buff
//    lv_fs_file_t text_file;
//    uint32_t font_res_count = 0;
//    uint8_t res;
//    res = lv_fs_open(&text_file, "0:/unit_sd_rw_test.txt", LV_FS_MODE_RD);
//    printf("open file: %d\r\n", res);
//    res = lv_fs_read(&text_file, text_buff, 100, &font_res_count);
//    printf("read file: %d\r\n", res);
//    lv_fs_close(&text_file);
//    printf("%s\n", text_buff);
//    printf("end read\r\n");
}
