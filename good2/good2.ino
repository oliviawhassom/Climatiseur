#include <WiFi.h>            // Inclure la bibliothèque pour le WiFi  
#include <WebServer.h>       // Inclure la bibliothèque pour le serveur web
#include <DHT.h>             // Inclure la bibliothèque pour le capteur DHT11
#include <LiquidCrystal_I2C.h>  // Inclure la bibliothèque pour l'écran LCD I2C

// Définir le nom et le mot de passe du point d'accès WiFi
const char* ssid = "ESP32_Access_Point";  // Nom du réseau WiFi créé par l'ESP32
const char* password = "123456789";      // Mot de passe pour se connecter à ce réseau

WebServer server(80);       // Créer un serveur web sur le port 80

const int relayPin = 22;        // Broche GPIO utilisée pour le relais (peut être ajusté)
const int dhtPin = 4;           // Broche GPIO utilisée pour le DHT11

DHT dht(dhtPin, DHT11);         // Initialiser le capteur DHT11

// Initialisation de l'écran LCD (adresse I2C 0x27, 16 colonnes et 2 lignes)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Fonction pour gérer la requête de la page principale
void handleRoot() {
  // Lire la température du capteur DHT11
  float temperature = dht.readTemperature();

  if (isnan(temperature)) { // Vérifier si la lecture a échoué
    Serial.println("Erreur de lecture du capteur DHT11.");
    server.send(200, "text/html", "Erreur de lecture du capteur DHT11! <a href='/'>Retour</a>");
    return;
  }

  // Créer la page HTML avec la température actuelle
  String html = R"rawliteral(
    <html>
      <body style="text-align:center;">
        <h1>Controle du relais</h1>
        <p>Température actuelle: )rawliteral" + String(temperature) + R"rawliteral( °C</p>
        <button style="padding: 10px 20px; margin: 10px; font-size: 18px;" onclick="location.href='/on'">Allumer le relais</button>
        <button style="padding: 10px 20px; margin: 10px; font-size: 18px;" onclick="location.href='/off'">Eteindre le relais</button>
      </body>
    </html>
  )rawliteral"; // Utilisation d'un texte brut pour éviter des erreurs de guillemets

  server.send(200, "text/html", html); // Envoie la page HTML
  Serial.println("Page principale envoyée au client");
}

// Fonction appelée lorsque la route "/on" est accédée (allume le relais)
void handleOn() {
  // Lire la température du capteur DHT11
  float temperature = dht.readTemperature();

  if (isnan(temperature)) { // Vérifier si la lecture a échoué
    Serial.println("Erreur de lecture du capteur DHT11.");
    server.send(200, "text/html", "Erreur de lecture du capteur DHT11! <a href='/'>Retour</a>");
    return;
  }

  // Vérifier si la température est supérieure à 18°C et que le relais n'est pas déjà activé
  if (temperature > 18.0) { 
    digitalWrite(relayPin, HIGH);  // Ferme le relais (active)
    Serial.println("Relais fermé (activé) à cause de la température.");
    server.send(200, "text/html", "Relais fermé (activé) à cause de la température! <a href='/'>Retour</a>");
  } else {
    server.send(200, "text/html", "Température trop basse pour fermer le relais. <a href='/'>Retour</a>");
  }
}

// Fonction appelée lorsque la route "/off" est accédée (éteint le relais)
void handleOff() {
  digitalWrite(relayPin, LOW);  // Ouvre le relais (désactive)
  Serial.println("Relais ouvert (désactivé) via commande web");
  server.send(200, "text/html", "Relais ouvert (désactivé)! <a href='/'>Retour</a>");
}

// Fonction pour gérer les erreurs 404 (routes non définies)
void handleNotFound() {
  Serial.println("Requête pour une page non trouvée");
  server.send(404, "text/plain", "Erreur 404 : Page non trouvée");
}

void setup() {
  Serial.begin(115200);  // Initialiser la communication série pour les logs
  Serial.println("\nDémarrage de l'ESP32");

  pinMode(relayPin, OUTPUT);      // Configurer la broche GPIO comme sortie pour le relais
  digitalWrite(relayPin, LOW);    // Assurer que le relais est ouvert au démarrage
  Serial.println("Broche GPIO configurée pour le relais");

  // Démarrer le point d'accès WiFi
  if (WiFi.softAP(ssid, password)) {
    Serial.println("Point d'accès WiFi créé avec succès");
    Serial.print("Adresse IP du point d'accès : ");
    Serial.println(WiFi.softAPIP());  // Afficher l'adresse IP du point d'accès
  } else {
    Serial.println("Échec de la création du point d'accès WiFi");
    while (true); // Stoppe le programme si le WiFi ne démarre pas
  }

  // Initialiser le capteur DHT11
  dht.begin();

  // Initialiser l'écran LCD
  lcd.begin(16, 2);  // Initialiser l'écran LCD avec 16 colonnes et 2 lignes
  lcd.backlight();   // Activer le rétroéclairage
  lcd.setCursor(0, 0);  // Positionner le curseur à la première ligne
  lcd.print("Température:");

  // Configurer les routes du serveur
  server.on("/", handleRoot);     // Route pour la page principale
  server.on("/on", handleOn);     // Route pour allumer le relais
  server.on("/off", handleOff);   // Route pour éteindre le relais
  server.onNotFound(handleNotFound); // Gestion des erreurs 404

  server.begin();  // Démarrer le serveur web
  Serial.println("Serveur web démarré");
}

void loop() {
  server.handleClient(); // Gérer les requêtes entrantes

  // Lire la température du capteur DHT11 et afficher sur LCD
  float temperature = dht.readTemperature();
  if (isnan(temperature)) { // Vérifier si la lecture a échoué
    lcd.setCursor(0, 1);
    lcd.print("Erreur lecture");
  } else {
    lcd.setCursor(0, 1);  // Positionner le curseur à la ligne 2
    lcd.print(temperature);  // Afficher la température sur LCD
    lcd.print(" C");
  }

  // Ouvrir le relais automatiquement si la température descend en dessous de 18°C
  if (temperature < 18.0) {
    digitalWrite(relayPin, LOW);  // Ouvre le relais (désactive)
    Serial.println("Relais ouvert (désactivé) car la température est trop basse.");
  }

  delay(3000);  // Attendre 3 secondes avant de lire à nouveau la température
}
