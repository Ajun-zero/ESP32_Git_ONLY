/*
  L06 舵机扫动（GS90 / SG90 类 180° 舵机）
  接线：
    舵机 棕/黑(GND)  → 外部 5V 电源负极，并且必须连到 ESP32 的 GND
    舵机 红(VCC)     → 外部 5V 电源正极（4.8~6V，不要接 3.3V，不要从板子 5V 排针取电）
    舵机 橙/黄(信号) → GPIO6（左排第 6 孔）
  现象：上电先回中位 90°，然后 0° → 180° → 0° 来回扫动。
*/

const int PIN_SERVO = 6;      // 信号线接的 GPIO
const int PWM_FREQ  = 50;     // 舵机标准频率 50Hz（周期 20ms）
const int PWM_RES   = 14;     // 分辨率 14 位：把 20ms 切成 16384 份
const int US_MIN    = 500;    // 0°   对应的脉宽（微秒）
const int US_MAX    = 2400;   // 180° 对应的脉宽（微秒；SG90 常见 500~2400，别超）

// 把角度（0~180）换算成 PWM 占空比并输出
void setAngle(int deg) {
  deg = constrain(deg, 0, 180);
  int us = map(deg, 0, 180, US_MIN, US_MAX);          // 角度 → 脉宽(us)
  uint32_t duty = (uint32_t)us * (1u << PWM_RES) / 20000u;  // 脉宽 → 占空比
  ledcWrite(PIN_SERVO, duty);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  ledcAttach(PIN_SERVO, PWM_FREQ, PWM_RES);   // 占用 GPIO6 输出 50Hz PWM

  setAngle(90);                                // 上电先回中位，方便装舵机臂
  Serial.println("舵机已回中位 90°");
  delay(800);
}

void loop() {
  // 0° → 180°
  for (int a = 0; a <= 180; a += 2) {
    setAngle(a);
    delay(20);
  }
  // 180° → 0°
  for (int a = 180; a >= 0; a -= 2) {
    setAngle(a);
    delay(20);
  }
}
