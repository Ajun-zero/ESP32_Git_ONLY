/*
  L04 按键开关小灯 —— 外接 LED 与板载彩灯同步
  接线（和 L03 一样）：
    按键：一脚 → GPIO4，另一脚 → GND
    LED ：长脚 → 220Ω → GPIO5，短脚 → GND
  现象：按一下，外接 LED 和板载 WS2812B 彩灯同时亮；再按一下同时灭。
*/

const uint8_t PIN_BTN = 4;   // 按键
const uint8_t PIN_LED = 5;   // 外接 LED

const unsigned long DEBOUNCE_MS = 30;

bool ledOn = false;
bool stableLevel = HIGH;
bool lastRaw = HIGH;
unsigned long lastEdgeMs = 0;

// 把"开/关"这件事集中在一个函数里，以后加灯只要改这里
void setLamp(bool on) {
  digitalWrite(PIN_LED, on ? HIGH : LOW);      // 外接 LED
  rgbLedWrite(48, on ? 30 : 0, on ? 64 : 0, on ? 40 : 0);  // 板载彩灯（白，亮度 64）

  // 想换成别的颜色，就把上面那行换成下面这行（R, G, B 各 0~255）：
  // rgbLedWrite(48, on ? 0 : 0, on ? 64 : 0, on ? 64 : 0);   // 青色
}

void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP);   // 内部上拉
  pinMode(PIN_LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);     // 板载彩灯
  setLamp(false);                  
  Serial.begin(115200);
  delay(1000);
  Serial.println("按键同步控制：外接 LED + 板载彩灯（按一下开，再按一下关）");
}

void loop() {
  bool raw = digitalRead(PIN_BTN);

  if (raw != lastRaw) {                        // 电平变化，记时间
    lastRaw = raw;
    lastEdgeMs = millis();
  }

  if (millis() - lastEdgeMs >= DEBOUNCE_MS && raw != stableLevel) {
    stableLevel = raw;                         // 抖动结束

    if (stableLevel == LOW) {                  // 只处理"按下"
      ledOn = !ledOn;
      setLamp(ledOn);                          // 两盏灯一起变
      Serial.printf("灯：%s\n", ledOn ? "开" : "关");
    }
  }
}
