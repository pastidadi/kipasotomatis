// Pin sensor PIR
const int pirPin = 2;

// Pin motor driver L298N
const int in1Pin = 3;
const int in2Pin = 4;
const int enablePin = 5;

// Status pintu
bool doorOpen = false;

// Waktu jeda dalam milidetik (5 detik = 5000 milidetik)
const unsigned long doorDelay = 2000;
// Waktu terakhir gerakan terdeteksi
unsigned long lastMovementTime = 0;

// Kecepatan motor (0-255)
const int motorSpeed = 200;

void setup() {
  // Konfigurasi pin
  pinMode(pirPin, INPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  
  // Matikan motor
  motorOff();
  
  // Mulai dengan pintu tertutup
  doorOpen = false;
  
  // Mulai serial monitor
  Serial.begin(9600);
}

void loop() {
  // Baca status sensor PIR
  int pirStatus = digitalRead(pirPin);
  
  if (pirStatus == HIGH) {
    // Jika ada gerakan terdeteksi
    lastMovementTime = millis(); // Perbarui waktu terakhir gerakan terdeteksi
    
    if (!doorOpen) {
      // Jika pintu tertutup dan ada gerakan, buka pintu
      openDoor();
      doorOpen = true;
    } else {
      // Jika pintu terbuka dan ada gerakan, putar balik motor
      reverseMotor();
    }
  } else {
    // Jika tidak ada gerakan dan pintu terbuka, cek waktu jeda
    if (doorOpen && (millis() - lastMovementTime >= doorDelay)) {
      // Jika sudah mencapai waktu jeda, tutup pintu
      closeDoor();
      doorOpen = false;
    }
  }
  
  // Jika pintu tertutup dan tidak ada gerakan setelah waktu jeda, matikan motor
  if (!doorOpen && (millis() - lastMovementTime >= doorDelay)) {
    motorOff();
  }
  
  // Tunda sedikit sebelum membaca ulang sensor
  delay(100);
}

// Fungsi untuk membuka pintu
void openDoor() {
  motorOn();
  
  Serial.println("Pintu terbuka");
}

// Fungsi untuk menutup pintu
void closeDoor() {
  motorOn();
  delay(2000); // Tunda selama 2 detik agar pintu sepenuhnya tertutup
  
  motorOff();
  
  Serial.println("Pintu tertutup");
}

// Fungsi untuk menghidupkan motor
void motorOn() {
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  analogWrite(enablePin, motorSpeed);
}

// Fungsi untuk mematikan motor
void motorOff() {
  digitalWrite(enablePin, LOW);
}

// Fungsi untuk memutar motor ke arah sebaliknya
void reverseMotor() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  analogWrite(enablePin, motorSpeed);
}
