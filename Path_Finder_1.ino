#define MOTOR1_IN1  27
#define MOTOR1_IN2  26
#define MOTOR1_EN   14

#define MOTOR2_IN1  33
#define MOTOR2_IN2  32
#define MOTOR2_EN   25

#define FRONT_TRIG  19
#define FRONT_ECHO  18

#define LEFT_TRIG   5
#define LEFT_ECHO   4

#define RIGHT_TRIG  23
#define RIGHT_ECHO  22

const int baseSpeedLeft = 230;
const int baseSpeedRight = 170;
const float Kp = 0.3; // Proportional gain
// float Ki = 0.2;   // (commented for now)

// No integral needed unless proven helpful
// float integral = 0;

void setup() {
  Serial.begin(115200);
  pinMode(MOTOR1_IN1, OUTPUT); pinMode(MOTOR1_IN2, OUTPUT); pinMode(MOTOR1_EN, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT); pinMode(MOTOR2_IN2, OUTPUT); pinMode(MOTOR2_EN, OUTPUT);

  pinMode(FRONT_TRIG, OUTPUT); pinMode(FRONT_ECHO, INPUT);
  pinMode(LEFT_TRIG, OUTPUT); pinMode(LEFT_ECHO, INPUT);
  pinMode(RIGHT_TRIG, OUTPUT); pinMode(RIGHT_ECHO, INPUT);
}

void loop() {
  const float WALL_THRESHOLD = 20;

  // Only read front sensor during forward movement
  float front = readDistance(FRONT_TRIG, FRONT_ECHO);
  Serial.print("F:"); Serial.println(front);

  if (front < WALL_THRESHOLD) {

    stopMotors(); 
    delay(100);

    float left = readDistance(LEFT_TRIG, LEFT_ECHO);
    float right = readDistance(RIGHT_TRIG, RIGHT_ECHO);

    Serial.print("L:"); Serial.print(left); Serial.print(" R:"); Serial.println(right);

    if (left > right) {
      turnLeft();
    } else {
      turnRight();
    }

  } else {
    // Front clear, follow wall using P controller
      // float left = readDistance(LEFT_TRIG, LEFT_ECHO);
      // float right = readDistance(RIGHT_TRIG, RIGHT_ECHO);

    moveForward(baseSpeedLeft, baseSpeedRight);
    // wallFollowP(left, right, WALL_THRESHOLD);
  }

  delay(100);
}

// void wallFollowP(float leftDist, float rightDist, int WALL_THRESHOLD) {
//   int leftSpeed = baseSpeedLeft;
//   int rightSpeed = baseSpeedRight;

//   if (leftDist < WALL_THRESHOLD || rightDist < WALL_THRESHOLD) {
//     float error = leftDist - rightDist;
//     float correction = Kp * error;

//     leftSpeed = constrain(baseSpeedLeft - correction, 0, 255);
//     rightSpeed = constrain(baseSpeedRight + correction, 0, 255);
//   }

//   moveForward(leftSpeed, rightSpeed);
// }

void moveForward(int speedLeft, int speedRight) {
  // Left Motor Forward
  digitalWrite(MOTOR1_IN1, HIGH); 
  digitalWrite(MOTOR1_IN2, LOW);
  analogWrite(MOTOR1_EN, speedLeft);

  // Right Motor Forward
  digitalWrite(MOTOR2_IN1, LOW); 
  digitalWrite(MOTOR2_IN2, HIGH);
  analogWrite(MOTOR2_EN, speedRight);
}

void stopMotors() {
  analogWrite(MOTOR1_EN, 0); analogWrite(MOTOR2_EN, 0);
  delay(100);
}

void turnLeft() {
  // Left motor moves forward, right motor stops
  digitalWrite(MOTOR1_IN1, LOW);
  digitalWrite(MOTOR1_IN2, HIGH);  // Left motor forward

  digitalWrite(MOTOR2_IN1, LOW);
  digitalWrite(MOTOR2_IN2, LOW);   // Right motor stop

  analogWrite(MOTOR1_EN, 230);   // Speed control
  analogWrite(MOTOR2_EN, 0);

  delay(380);  // Adjust for precise 90° turn
  stopMotors();
}

void turnRight() {
  // Right motor moves forward, left motor stops
  digitalWrite(MOTOR1_IN1, LOW);
  digitalWrite(MOTOR1_IN2, LOW);   // Left motor stop

  digitalWrite(MOTOR2_IN1, HIGH);
  digitalWrite(MOTOR2_IN2, LOW);   // Right motor forward

  analogWrite(MOTOR1_EN, 0);
  analogWrite(MOTOR2_EN, 170);   // Speed control

  delay(380);  // Adjust for precise 90° turn
  stopMotors();
}


float readDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // Timeout = 30ms
  if (duration == 0) return 1000.0;
  return duration * 0.034 / 2;
}
