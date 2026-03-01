// Définition des broches de commande du L298N
const int IN1 = 23;  // Direction 1
const int IN2 = 22;  // Direction 2
const int ENA = 21;  // Contrôle de la vitesse (PWM)

// Variables pour ajuster la vitesse
int vitesse = 255; // Valeur PWM de 0 à 255 (vitesse maximale)

// Fonction d'initialisation
void setup() {
  // Initialiser les broches comme sorties
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  
  // Activer le contrôle PWM pour ENA
  analogWrite(ENA, vitesse); // Utiliser PWM pour contrôler la vitesse
}

// Fonction de boucle
void loop() {
  // Faire tourner le moteur dans un sens (par exemple sens horaire)
  digitalWrite(IN1, HIGH); // Active la direction
  digitalWrite(IN2, LOW);  // Inhibe l'autre direction

  delay(2000);  // Attendre 2 secondes

  // Faire tourner le moteur dans l'autre sens (sens anti-horaire)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  delay(2000);  // Attendre 2 secondes
}
