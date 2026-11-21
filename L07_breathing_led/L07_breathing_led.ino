/*
  L07 进阶版：正弦 + 伽马校正的呼吸灯
  硬件：外接 LED（GPIO5 + 330Ω 电阻）与板载 WS2812B 彩灯（GPIO48）同步呼吸
  要点：
    1) 用 (1-cos)/2 生成 0→1→0 的平滑曲线，两端速度自然为 0，不会"硬折返"
    2) 再套 gamma 2.2：人眼对亮度是非线性感知的，这样看起来才是匀速明暗
    3) 5 kHz / 12 位 PWM：频率远高于人眼闪烁阈值，分辨率 4096 级
    4) 全程序无 delay()，每 20 ms 更新一次
*/

#include <math.h>

const uint8_t PIN_LED = 5;    // 外接 LED
const uint8_t PIN_RGB = 48;   // 板载 WS2812B

const uint32_t PWM_FREQ = 5000;                 // 5 kHz
const uint8_t  PWM_RES  = 12;                   // 12 位 → 0..4095
const uint32_t PWM_MAX  = (1u << PWM_RES) - 1;

const float PERIOD_MS = 4000.0f;                // 一次完整呼吸 4 秒
const float GAMMA     = 2.2f;                   // 人眼校正系数

// 板载彩灯的颜色（呼吸时会按亮度比例缩放这个颜色）
const uint8_t RGB_R = 0;  //对颜色进行更改
const uint8_t RGB_G = 96;
const uint8_t RGB_B = 160;                      // 偏青蓝，和外面白灯区分开

uint32_t lastTick = 0;

void setup() {
  Serial.begin(115200);

  ledcAttach(PIN_LED, PWM_FREQ, PWM_RES);       // 让 GPIO5 输出 5kHz PWM

  Serial.println("正弦呼吸灯：外接 LED + 板载彩灯");
}

void loop() {
  uint32_t now = millis();
  if (now - lastTick < 20) {                    // 每 20ms 更新一次（50Hz 刷新）
    return;
  }
  lastTick = now;

  // 1) 归一化时间 0..1
  float t = (float)(now % (uint32_t)PERIOD_MS) / PERIOD_MS;

  // 2) 余弦曲线：0 → 1 → 0，两端斜率为 0（最平滑的呼吸）
  float s = 0.5f - 0.5f * cosf(2.0f * (float)PI * t);

  // 3) 伽马校正：把"物理亮度"换成"人眼感觉的亮度"
  float level = powf(s, GAMMA);

  // 4) 输出
  ledcWrite(PIN_LED, (uint32_t)(level * PWM_MAX));

  rgbLedWrite(PIN_RGB,
              (uint8_t)(level * RGB_R),
              (uint8_t)(level * RGB_G),
              (uint8_t)(level * RGB_B));
}
