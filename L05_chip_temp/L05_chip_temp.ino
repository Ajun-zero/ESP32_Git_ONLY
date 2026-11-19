/*
  L05 读取芯片内部温度
  板卡：ESP32-S3-WROOM-1-N16R8
  作用：每秒读一次 ESP32-S3 芯片内部温度传感器的值，打印到串口，
        同时记录本次上电以来的最低/最高温度。
  说明：测的是"芯片自己的温度"，不是室温；比室温高 5~15 ℃ 是正常的。
*/

unsigned long lastMs = 0;      // 上一次读取的时刻
float tMin = 999.0;            // 本次上电以来的最低温度
float tMax = -999.0;           // 本次上电以来的最高温度

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println("==== ESP32-S3 芯片内部温度 ====");
}

void loop() {
  if (millis() - lastMs < 1000) {   // 每 1000ms 读一次，不阻塞
    return;
  }
  lastMs = millis();

  float t = temperatureRead();      // 核心自带的函数，单位 ℃

  if (isnan(t)) {                   // 读失败（极少见）
    Serial.println("读取失败：temperatureRead() 返回 NaN");
    return;
  }

  if (t < tMin) tMin = t;
  if (t > tMax) tMax = t;

  Serial.printf("温度：%5.2f ℃    最低：%5.2f    最高：%5.2f\n", t, tMin, tMax);
}
