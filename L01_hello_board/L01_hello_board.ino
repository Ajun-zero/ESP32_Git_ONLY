/*
  L01 认识你的板子（第 1 课）
  板卡：ESP32-S3-WROOM-1-N16R8（16MB Flash + 8MB OPI PSRAM）
  作用：
    1) 串口打印芯片信息（主频 / Flash / PSRAM / MAC / 复位原因）
    2) 让板载 WS2812B 彩灯（GPIO48）循环变色
  使用方法：打开串口监视器，波特率 115200
*/

void setup() {
  Serial.begin(115200);
  delay(1500);                       // 等串口监视器连上，避免前几行丢失

  Serial.println();
  Serial.println("================ 板子信息 ================");
  Serial.printf("芯片型号 : %s\n", ESP.getChipModel());
  Serial.printf("芯片版本 : v%d.%d\n", ESP.getChipRevision() / 100,
                ESP.getChipRevision() % 100);
  Serial.printf("核心数量 : %d\n", ESP.getChipCores());
  Serial.printf("主频     : %lu MHz\n", getCpuFrequencyMhz());
  Serial.printf("Flash    : %lu MB\n", ESP.getFlashChipSize() / 1024 / 1024);
  Serial.printf("PSRAM    : %lu MB\n", ESP.getPsramSize() / 1024 / 1024);
  Serial.printf("剩余内存 : %lu 字节\n", ESP.getFreeHeap());

  // MAC 地址存在芯片的 eFuse 里，直接读，不用开 Wi-Fi
  uint64_t mac = ESP.getEfuseMac();
  Serial.printf("MAC 地址 : %02X:%02X:%02X:%02X:%02X:%02X\n",
                (uint8_t)(mac),       (uint8_t)(mac >> 8),  (uint8_t)(mac >> 16),
                (uint8_t)(mac >> 24), (uint8_t)(mac >> 32), (uint8_t)(mac >> 40));

  Serial.printf("复位原因 : %d\n", (int)esp_reset_reason());
  Serial.println("=========================================");
  Serial.println("板载彩灯（GPIO48）开始循环变色……");
}

void loop() {
  // rgbLedWrite(引脚, 红, 绿, 蓝)，每路 0~255
  rgbLedWrite(48, 32, 0, 0);   // 红
  delay(500);
  rgbLedWrite(48, 0, 32, 0);   // 绿
  delay(500);
  rgbLedWrite(48, 0, 0, 32);   // 蓝
  delay(500);
  rgbLedWrite(48, 0, 0, 0);    // 灭
  delay(500);
}
