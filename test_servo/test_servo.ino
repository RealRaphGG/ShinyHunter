

#include <Servo.h>

Servo servo_A;
Servo servo_X;
Servo servo_Up;
Servo servo_Home;

void setup() {
  // put your setup code here, to run once:

  servo_A.attach(10); // brancher le servo pour le bouton A sur la borne 10
  servo_X.attach(11); // brancher le servo pour le bouton X sur la borne 11
  servo_Home.attach(9); // brancher le servo pour le bouton Home sur la borne 9
  servo_Up.attach(6); // brancher le servo pour le bouton Avancer sur la borne 6

  Serial.begin(9600); // début de la liaison série avec le PC
}


// Appuie sur le bouton A pendant le temps indiqué
void press_A(int temps_pression) {
  servo_A.write(130); // angle d'inclinaison appuie
  delay(temps_pression); // attente
  servo_A.write(115); // angle d'inclinaison relachement
}

// Appuie sur le bouton X pendant le temps indiqué
void press_X(int temps_pression) {
  servo_X.write(100); // angle d'inclinaison appuie
  delay(temps_pression); // attente
  servo_X.write(85.00000); // angle d'inclinaison relachement
}

// Appuie sur le bouton Avancer pendant le temps indiqué
void press_Up(int temps_pression) {
  servo_Up.write(50); // angle d'inclinaison appuie
  delay(temps_pression); // attente
  servo_Up.write(65); // angle d'inclinaison relachement
}

// Appuie sur le bouton Home pendant le temps indiqué
void press_Home(int temps_pression) {
  servo_Home.write(160); // angle d'inclinaison appuie
  delay(temps_pression); // attente
  servo_Home.write(132); // angle d'inclinaison relachement
}


void loop() {

  press_Up(200); // Avancer vers le Pokemon
  delay(2000);
  press_A(200); // Passer la boite de dialogue 1
  delay(2000);
  press_X(200); // Passer la boite de dialogue 2
  delay(2000);
  press_Home(200);
  delay(2000);
}
