/*
  L02 串口收发：发送 "1"，回复"登录成功"
  用法：
    1) 打开串口监视器，波特率 115200
    2) 右下角"行结尾"必须选 **新建行(Newline)** 或 **回车(CR)**
       —— 选"无"的话板子永远收不到一条完整指令
    3) 在输入框里打 1，回车 → 板子回复"登录成功"
*/

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("==== 串口登录演示 ====");
  Serial.println("发送 1 登录");
}

void loop() {
  if (Serial.available() == 0) {     // 没有数据就立刻返回，不阻塞
    return;
  }

  // 读到换行符为止：一串完整指令
  String cmd = Serial.readStringUntil('\n');
  cmd.trim();                        // 去掉首尾的空格、回车（\r）、换行

  if (cmd.length() == 0) {           // 空行，忽略
    return;
  }

  Serial.print("收到：");
  Serial.println(cmd);

  if (cmd == "1") {
    Serial.println("登录成功");
  } else {
    Serial.println("未知指令（只认 1）");
  }
}
