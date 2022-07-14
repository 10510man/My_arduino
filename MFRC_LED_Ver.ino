#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);  // rfid 객체 생성

 

void setup()
{
  Serial.begin(9600);   // 시리얼 통신 시작
  SPI.begin();             // SPI 통신 시작
  rfid.PCD_Init();         // rfid(MFRC522) 초기화
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  pinMode(8, OUTPUT);
}

 

void loop()
{
  // 새 카드 접촉이 없으면 돌아감
  if ( ! rfid.PICC_IsNewCardPresent()) {
    delay(500);  // 0.5초 대기
    return;
  }
  // 카드 태그를 읽지 못했으면 돌아감
  if ( ! rfid.PICC_ReadCardSerial()) {
    delay(500);    // 읽지 못했으면 0.5초 대기
    return;
  }
  // UID 값을 16진 값으로 읽어서 시리얼 모니터에 표시함
  Serial.print("UID tag :");
  String content= "";
  byte letter;
 
  for (byte i = 0; i < rfid.uid.size; i++)
  {
     Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(rfid.uid.uidByte[i], HEX);
     content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(rfid.uid.uidByte[i], HEX));
  }
 
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
 
  if (content.substring(1)="A0 CB 62 80" || content.substring(1)=="D6 20 C3 C7" )// 승인할 UID들 비교
  {
   // 인증 성공하면...
    Serial.println("Authorized access");
    Serial.println();
    digitalWrite(8, HIGH);   
  }
  else { // 인증 실패하면...  
    Serial.println(" Access denied"); // 그외 UID는 승인 거부 표시
    delay(1000);  // 카드 인증 실패 시 1초 대기
  }
}

//https://blog.daum.net/sualchi/13720768 확인함.
