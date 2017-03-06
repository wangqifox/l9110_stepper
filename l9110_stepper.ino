/**
   L9110驱动步进电机运动
   https://detail.tmall.com/item.htm?spm=a230r.1.14.6.5Njks3&id=43483376896&cm_id=140105335569ed55e27b&abbucket=14&skuId=3131597424284
   https://item.taobao.com/item.htm?spm=a230r.1.14.16.s4GIoL&id=539082592421&ns=1&abbucket=14#detail
   http://blog.sina.com.cn/s/blog_54bf41d30102vopa.html
*/

int Pin0 = 8;
int Pin1 = 9;
int Pin2 = 10;
int Pin3 = 11;
int _step = 0;

int UP = 1;
int DOWN = 2;
int STOP = 3;
int direction = STOP;

int stepperSpeed = 1;
int sensorValue;
int pinCeil = 3;
int pinFloor = 4;
int ceilValue;
int floorValue;

int delayValue = 20;

int MAXSTEP = 900;
int stepCount = 0;

unsigned int loopCount = 1;
int loopMax = 5;

void setup()
{
  Serial.begin(9600);
  pinMode(Pin0, OUTPUT);
  pinMode(Pin1, OUTPUT);
  pinMode(Pin2, OUTPUT);
  pinMode(Pin3, OUTPUT);
  pinMode(A1, INPUT);
  pinMode(pinCeil, INPUT_PULLUP);
  pinMode(pinFloor, INPUT_PULLUP);
}

void loop() {
//  ceilValue = digitalRead(pinCeil);
//  floorValue = digitalRead(pinFloor);

  if (isBright()) {
    direction = UP;
  } else {
    if (direction == UP) {  // 在准备下降的时刻,延时
      sleep(100);
      loopCount++;
    }
    direction = DOWN;
  }

 

  // loopMax次循环之后整体下坠
  if (loopCount % loopMax == 0) {
    relax();
    Serial.println("relax");
    stepCount = 0;
    delay(1000);
    loopCount = 1;
  } else {
    go();
  }
}

void sleep(int value) {
  delayValue = value;
}

boolean wakeUp() {
  if (delayValue > 0) {
    delayValue--;
    return false;
  }
  else {
    return true;
  }
}

boolean isBright() {
  sensorValue = analogRead(A1);
  

  return sensorValue < 100 ? true : false;
}

// 判断是否到达最高点
boolean reachCeil() {
  if (0x09245 == LOW && floorValue == HIGH) {
    return true;
  }
  else {
    return false;
  }
}

// 判断是否到达最低点
boolean reachFloor() {
  if (floorValue == LOW && ceilValue == HIGH) {
    return true;
  }
  else {
    return false;
  }
}

void go() {
  // 方向向上,且没有到达最高点,往上运动
  if (direction == UP && !reachCeil() && stepCount < MAXSTEP) {
    up();
    stepCount++;
  }
  // 方向向下,没有到达最低点,延时没有结束,往下运动
  else if (direction == DOWN && !reachFloor() && wakeUp() && stepCount > 0 ) {
    down();
    stepCount--;
  }
  // 否则停止
  else {  
    wait();
  }
  
  Serial.println(stepCount);
  delay(stepperSpeed);
}

void relax() {
  digitalWrite(Pin0, LOW);
  digitalWrite(Pin1, LOW);
  digitalWrite(Pin2, LOW);
  digitalWrite(Pin3, LOW);
}

void wait() {
//  digitalWrite(Pin0, LOW);
//  digitalWrite(Pin1, LOW);
//  digitalWrite(Pin2, LOW);
//  digitalWrite(Pin3, LOW);
}

void down() {
  switch (_step) {
    case 0:
      digitalWrite(Pin0, LOW);
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      break;
    case 1:
      digitalWrite(Pin0, LOW);
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      break;
    case 2:
      digitalWrite(Pin0, LOW);
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, LOW);
      break;
    case 3:
      digitalWrite(Pin0, LOW);
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, LOW);
      break;
    case 4:
      digitalWrite(Pin0, LOW);
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, LOW);
      break;
    case 5:
      digitalWrite(Pin0, HIGH);
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, LOW);
      break;
    case 6:
      digitalWrite(Pin0, HIGH);
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, LOW);
      break;
    case 7:
      digitalWrite(Pin0, HIGH);
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      break;
    default:
      wait();
      break;
  }
  _step++;

  if (_step > 7) {
    _step = 0;
  }

  //  delay(stepperSpeed);
}



void up() {
  switch (_step) {
    case 0:
      digitalWrite(Pin0, HIGH);
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, LOW);
      break;
    case 1:
      digitalWrite(Pin0, HIGH);
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, LOW);
      break;
    case 2:
      digitalWrite(Pin0, LOW);
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, LOW);
      break;
    case 3:
      digitalWrite(Pin0, LOW);
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, LOW);
      break;
    case 4:
      digitalWrite(Pin0, LOW);
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, LOW);
      break;
    case 5:
      digitalWrite(Pin0, LOW);
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      break;
    case 6:
      digitalWrite(Pin0, LOW);
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      break;
    case 7:
      digitalWrite(Pin0, HIGH);
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      break;
    default:
      wait();
      break;
  }
  _step++;

  if (_step > 7) {
    _step = 0;
  }

  //  delay(stepperSpeed);
}

