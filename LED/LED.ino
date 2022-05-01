void setup() {
  // ตั้งค่าสถานะพิน ให้เป็นโหมดส่งออก หมายเลขพิน ไม่ต้องใส่ IO
  // เช่น IO17 ก็ใส่หมายเลข 17 ไปเลย
  pinMode(17, OUTPUT);    // Bluetooth
  pinMode(2, OUTPUT);     // Wifi
  pinMode(15, OUTPUT);    // NTP
  pinMode(12, OUTPUT);    // IoT
}

void loop() {
  // เปิด LED
  digitalWrite(17,HIGH);
  delay(100);
  digitalWrite(2,HIGH);
  delay(100);
  digitalWrite(15,HIGH);
  delay(100);
  digitalWrite(12,HIGH);
  delay(300);
 
  // ปิด LED
  digitalWrite(17,LOW);
  delay(100);
  digitalWrite(2,LOW);
  delay(100);
  digitalWrite(15,LOW);
  delay(100);
  digitalWrite(12,LOW);
  delay(300);
}
