#include <IRremote.h>
#include <Servo.h>
#include <Ultrasonic.h>

int IN1 = 2;  // Выводы управления мотором
int IN2 = 3;
int IN3 = 4;
int IN4 = 7;
int ENA = 5;
int ENB = 6;

Servo UServo;
int i = 0;
int j = 0;
int start = 90;  //начальное положение серво в градусах
int pos = 180;  //крайнее положение серво в градусах
bool stepUp;  //флаг для управления движением серво

//Ультразвуковой сенсор HC-SR04
const int echoPin = A1;
const int trigPin = A0;
Ultrasonic URF(A0, A1);
float distance;

// RU: Вход контроллера, к которму подключен ИК-приёмник.
const int IR_PIN = 12;
// RU: Создаём объект ИК-приёмник.
IRrecv irrecv(IR_PIN);
decode_results results;

void moveForward()  //Вперед
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMoving()  //Остановка
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}

void moveBack()  //Назад
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnLeft()  //Влево
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight()  //Вправо
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void autoMode()  //Автоматический режим с предотвращением врезания в препятствия
{
  UServo.write(90);
  for (;;) {
    distance = URF.distanceRead();
    Serial.println(distance);
    while (distance >= 25 || distance == 0) {
      distance = URF.distanceRead();
      moveForward();

      delay(10);
    }
    stopMoving();
    UServo.write(135);

    while (distance <= 40 || distance == 0) {
      distance = URF.distanceRead();
      moveBack();

      delay(10);
    }

    stopMoving();

    UServo.write(150);
    delay(500);
    int rightDistance = URF.distanceRead();

    UServo.write(30);
    delay(500);
    int leftDistance = URF.distanceRead();

    UServo.write(90);
    delay(500);

    if (rightDistance > leftDistance) {
      for (int t = 0; t < 7000; t++) {
        turnLeft();
      }
    } else {
      for (int t = 0; t < 7000; t++) {
        turnRight();
      }
    }
    delay(500);
  }

  // Serial.println("Run...");
}

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  UServo.attach(11);
  UServo.write(start);  //устанавливаем начальное положение серво
  j = start;  //
  stepUp = true;
  irrecv.enableIRIn();
  Serial.begin(115200);
  delay(2000);
}
void loop() {
  autoMode();
}
