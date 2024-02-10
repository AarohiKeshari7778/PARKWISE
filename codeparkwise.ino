#include <LiquidCrystal.h>
#include <Servo.h>
#include <IRremote.h>



int RECV_PIN = 6;
IRrecv irrecv(RECV_PIN);
decode_results results;

Servo myservo;


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define t1 10
#define e1 13
#define t2 9
#define e2 8


int distanceThreshold = 100;
long duration;

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0,0);  
  Serial.begin (9600);  
  irrecv.enableIRIn();
}

long readDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT); 
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return duration;
}


void loop()
{ 
  int flag = 0;
  
  float d1 = readDistance(t1, e1) * (0.034) ; 
  float d2 = readDistance(t2, e2) * (0.034) ;
  
  
  Serial.println("d1 = " + String(d1) + "cm");
  Serial.println("d2 = " + String(d2) + "cm");
  
  if(d1>100 & d2>100)
  {
    lcd.setCursor(0,0);
	lcd.print("2 Slots Free");
	lcd.setCursor(0,1);
	lcd.print("Slot 1 2 Free");
	delay(500);
  }
  
  else if(d1>100 & d2<100)
  {
    lcd.setCursor(0,0);
	lcd.print("1 Slot Free");
	lcd.setCursor(0,1);
	lcd.print("Slot 1 Free");
	delay(500);
  }
  
  else if(d1<100 & d2>100)
  {
    lcd.setCursor(0,0);
	lcd.print("1 Slot Free");
	lcd.setCursor(0,1);
	lcd.print("Slot 2 Free");
	delay(500);
  }
  
  else if(d1<100 & d2<100){
    flag = 1;
    lcd.setCursor(0,0);
	lcd.print("No Slots Free");
	lcd.setCursor(0,1);
	lcd.print("Parking Full");
	delay(500);
  }

  delay(100);
  
if (flag != 1)
  {
  if (irrecv.decode(&results)) 
  {
    switch (results.value)
    {
      case 0xFD00FF:
  		myservo.attach(7);
        Serial.println("Start");
        break;
      case 0xFD08F7:
		myservo.write(-90);
		Serial.println("Counter Clockwise");
        break;
      case 0xFD8877:
		myservo.write(90);
		Serial.println("Clockwise");
        break; 
      default:
        Serial.print("Unrecognized code received: 0x");
        Serial.println(results.value, HEX);
        break;        
    }    
    irrecv.resume(); 
  }
   }
  
}
