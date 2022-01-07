/*
Authors: RaphGG
*/

// Pour utiliser les servo-moteurs
#include <Servo.h>

// création des servo
Servo servo_A;
Servo servo_X;
Servo servo_Up;
Servo servo_Home;

bool keepLooping = true;
int temps_pression = 200;

void setup() {
  servo_A.attach(10); // brancher le servo pour le bouton A sur la borne 10
  servo_X.attach(11); // brancher le servo pour le bouton X sur la borne 11
  servo_Home.attach(9); // brancher le servo pour le bouton Home sur la borne 9
  servo_Up.attach(6); // brancher le servo pour le bouton Avancer sur la borne 6
  
  Serial.begin(9600); // début de la liaison série avec le PC
}

// Appuie sur le bouton A pendant le temps indiqué
void press_A(int temps_pression) {
  servo_A.write(137); // angle d'inclinaison appuie
  delay(temps_pression); // attente
  servo_A.write(115); // angle d'inclinaison relachement
}

// Appuie sur le bouton X pendant le temps indiqué
void press_X(int temps_pression) {
  servo_X.write(103); // angle d'inclinaison appuie
  delay(temps_pression); // attente
  servo_X.write(82); // angle d'inclinaison relachement
}

// Appuie sur le bouton Avancer pendant le temps indiqué
void press_Up(int temps_pression) {
  servo_Up.write(45); // angle d'inclinaison appuie
  delay(temps_pression); // attente
  servo_Up.write(65); // angle d'inclinaison relachement
}

// Appuie sur le bouton Home pendant le temps indiqué
void press_Home(int temps_pression) {
  servo_Home.write(170); // angle d'inclinaison appuie
  delay(temps_pression); // attente
  servo_Home.write(132); // angle d'inclinaison relachement
}

void loop() {
  
  //delay(5000); // Attendre 5 seconde avant le démarrage du programme
  
  if (keepLooping) {
    Serial.println("Starting Loop");
    
    press_Up(temps_pression); // Avancer vers le Pokemon
    delay(2500);
    press_A(temps_pression); // Passer la boite de dialogue 1
    delay(1000);
    press_A(temps_pression); // Passer la boite de dialogue 2
    delay(16650);
    
    
    Serial.println("Flushing leftover data");
    
    // flush the input buffer in case there is any leftover data
    while (Serial.available() != 0) {
      char t = Serial.read();
    }
    
    // Envoie du message au script Python
    Serial.println("Command: checkIfShiny");
    
    // Attente d'une réponse du script
    while (Serial.available() == 0) {
      delay(200);
    }
    
    Serial.println("Lecture de la reponse du script");
    
    // Lecture de la réponse du script
    char inByte = Serial.read();
    
    // Vérifie si le pokemon est shiny (yes ou no)
    if (inByte == 'y') {
      Serial.println("Le pokemon est Shiny !");
      keepLooping = false; // Si le pokémon est shiny, on arrête la boucle, et donc le programme
    }
    else { 
      // Le pokémon n'est pas shiny, on relance le programme !
      Serial.println("Le pokemon n'est pas Shiny, go Soft reset !");

      //Soft Reset
      press_Home(temps_pression); // Menu Home
      delay(1500);
      press_X(temps_pression); // Quitter le logiciel en cours d'utilisation ?
      delay(1500);
      press_A(temps_pression); // Quitter
      delay(1500);
      press_A(temps_pression); // Choix du jeu
      delay(1500);
      press_A(temps_pression); // Choix utilisateur
      delay(23000);
      press_A(temps_pression); // Passer l'intro
      delay(4000);
      press_A(temps_pression); // Ecran d'accueil
      delay(12000);
      
    }
  }
}
