/*
  L03 按键开关小灯（按一下开，再按一下关）
  接线：
    按键：一脚 → GPIO4，另一脚 → GND（用芯片内部上拉，不用外接电阻）
    LED ：长脚 → 220Ω 电阻 → GPIO5，短脚 → GND
  现象：每按一次，灯的状态翻转一次；串口同时打印"开/关"
*/

const uint8_t PIN_BTN = 4;   // 按键
const uint8_t PIN_LED = 5;   // LED

const unsigned long DEBOUNCE_MS = 30;  // 消抖时间：30 毫秒内的抖动都当没发生

bool ledOn = false;          // 灯当前是开还是关
bool stableLevel = HIGH;     // 消抖后确认的按键电平（上拉：松开=HIGH，按下=LOW）
bool lastRaw = HIGH;         // 上一次读到的原始电平（用来发现"变化"）
unsigned long lastEdgeMs = 0; // 最后一次电平变化的时刻

void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP);  // 内部上拉到 3.3V：松开读到 HIGH，按下读到 LOW
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);      // 上电先灭

  Serial.begin(115200);
  delay(1000);
  Serial.println("按键开关灯：按一下开，再按一下关");
}

void loop() {
  bool raw = digitalRead(PIN_BTN);          // 1) 读当前电平

  if (raw != lastRaw) {                     // 2) 电平变了？记下时间，等它稳定
    lastRaw = raw;
    lastEdgeMs = millis();
  }

  // 3) 距上次变化已超过 30ms，且电平确实和"已确认状态"不同 → 这是一次真实的动作
  if (millis() - lastEdgeMs >= DEBOUNCE_MS && raw != stableLevel) {
    stableLevel = raw;

    if (stableLevel == LOW) {               // 4) 只处理"按下"这一瞬间（松开不做事）
      ledOn = !ledOn;                       // 翻转状态
      digitalWrite(PIN_LED, ledOn ? HIGH : LOW);
      Serial.printf("灯：%s\n", ledOn ? "开" : "关");
    }
  }
}
