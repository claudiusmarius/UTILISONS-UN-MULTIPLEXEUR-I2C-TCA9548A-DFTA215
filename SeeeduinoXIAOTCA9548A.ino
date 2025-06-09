  // DFTA215
  // Petit programme utilisant un microcontroleur Seeeduino XIAO associé à un multiplexeur I2C 8 voies TCA9548A et un mini écran Oled 0.91
  // Le but est d'apréhender le pilotage de multiples dispositifs I2C avec adresses identiques
  // Ma chaîne Youtube : https://www.youtube.com/c/ClaudeDufourmont
  // Mon GitHub : https://github.com/claudiusmarius
  // Le lien de la vidéo concernant ce code est ci-dessous, il vous permettra de remonter jusque min GitHub
  // https://youtu.be/iU3s3-w5zC8
  // Précautions de câblage :
  // SEEEDUINO XIAO :       SDA A4  à brancher sur SDA écran
  //                        SCL A5  à brancher sur SCK écran
  // l'adresse I2C du TCA9548A est celle à défaut cad lorsque A0 à A2 ne sont pas câblés (en cas de difficulté placer des résistances de pull-up)
  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // Afin de m'encourager à continuer à PARTAGER tous mes projets, vous pouvez si vous le souhaitez LIKER, COMMENTER, PARTAGER cette vidéo et même vous ABONNER à ma chaîne
  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //                                                      ENORME MERCI A TOUTES ET A TOUS
  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------- 
 
  #include <Wire.h>                                                           // Bibliothèque I2C
  #include <Adafruit_GFX.h>                                                   // Bibliothèque graphique (texte, formes, etc.)
  #include <Adafruit_SSD1306.h>                                               // Contrôle des écrans OLED SSD1306

  // === Définition des constantes ===
  #define SCREEN_WIDTH 128                                                    // Largeur de l'écran OLED
  #define SCREEN_HEIGHT 32                                                    // Hauteur de l'écran OLED
  #define OLED_ADDR 0x3C                                                      // Adresse I2C des écrans OLED
  #define TCA_ADDR 0x70                                                       // Adresse I2C du multiplexeur TCA9548A

  // ==================================================== Création d’un objet écran unique, réutilisé pour chaque canal ==================================================
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
  // =====================================================================================================================================================================
  
  // ============================================================ Tableau de chaînes à afficher (le générique) ===========================================================
  const char* text[] = 
  {
  "IL Y A PEU DE TEMPS,",
  "DANS UNE GALAXIE",
  "PROCHE, TRES PROCHE..",
  "CLAUDE",
  "DUFOURMONT",
  "REALISA",
  "UN PETIT PROJET",
  "SEEEDUINO XIAO",
  "AVEC",
  "UN TCA9548A",
  "ET SIX",
  "SSD 1306 0.91",
  "CLAUDIUSMARIUS",
  "FIT DEFILER",
  "DU TEXTE !",
  "LE TCA9548A",
  "MULTIPLEXEUR I2C",
  "8 VOIES",
  "PERMET DE",
  "DRIVER",
  "8 DISPOSITIFS I2C",
  "AVEC ADRESSE",
  "IDENTIQUE",
  "UN PETIT PAS",
  "POUR",
  "CLAUDIUSMARIUS",
  " ??????????",
  "SCHEMA",
  "ET CODE",
  "DISPONIBLES",
  "DANS MON GITHUB",
  "DEPUIS LE",
  "DESCRIPTIF",
  "VIDEO",
  "A BIENTOT",
  "SUR D AUTRES",
  "VIDEOS"
  };
  // =====================================================================================================================================================================
  
  const int numScreens = 6;                                                   // Nombre d’écrans connectés
  const int numLines = sizeof(text) / sizeof(text[0]);                        // Nombre total de lignes de texte

  // ============================================================= Fonction pour sélectionner un canal du TCA9548A =======================================================
  void selectChannel(uint8_t ch) 
  {
  Wire.beginTransmission(TCA_ADDR);                                           // Début communication I2C avec le TCA9548A
  Wire.write(1 << ch);                                                        // Activation du canal ch (bit shift pour sélectionner la voie)
  Wire.endTransmission();                                                     // Fin de transmission
  }
  // =====================================================================================================================================================================
  
  
  // ======================================================= Affiche une ligne centrée horizontalement sur l’écran spécifié ==============================================
  void showLineOnScreen(uint8_t screen, const char* line) 
  {
  selectChannel(screen);                                                      // Activation du canal correspondant à l'écran
  display.clearDisplay();                                                     // Effacement du contenu de l'écran
  display.setTextSize(1);                                                     // Taille du texte (1 = standard)
  display.setTextColor(SSD1306_WHITE);                                        // Couleur : blanc sur fond noir
  // =====================================================================================================================================================================
  
  // ============================================================= Calcul de la largeur du texte pour le centrer =========================================================
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(line, 0, 0, &x1, &y1, &w, &h);
  int16_t xCentered = (SCREEN_WIDTH - w) / 2;
  int16_t yPosition = 10;                                                     // Position verticale fixe (milieu de l'écran)

  display.setCursor(xCentered, yPosition);
  display.println(line);                                                      // Affichage de la ligne
  display.display();                                                          // Rafraîchissement de l'écran OLED
  }
  // =====================================================================================================================================================================


  // ============================================================== Animation finale : flashs alternés en damier =========================================================
  void flashAlternatingScreens(int flashes = 2, int delayTime = 300) 
  {
  for (int f = 0; f < flashes; f++)                                           // Nombre de cycles d'animation
  {         
  for (int phase = 0; phase < 2; phase++)                                     // Phase 0 : écrans pairs, Phase 1 : impairs
  { 
  for (int i = 0; i < numScreens; i++) 
  {
  selectChannel(i);                                                           // Activation de l'écran i
  display.clearDisplay();                                                     // Vidage de l'écran

  if ((i + phase) % 2 == 0)                                                   // Alterne les écrans allumés selon la phase
  {          
  display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);         // Écran tout blanc
  }

  display.display();                                                          // Affichage du résultat
  }
  delay(delayTime);                                                           // Pause entre chaque flash
  }
  }
  
  for (int i = 0; i < numScreens; i++)                                        // Nettoyage : tous les écrans sont remis à zéro
  {
  selectChannel(i);
  display.clearDisplay();
  display.display();
  }
  }
  // =====================================================================================================================================================================


  // ================================================================ Initialisation de tous les écrans ==================================================================
  void setup() 
  {
  Wire.begin();                                                               // Initialisation du bus I2C
  Serial.begin(115200);                                                       // Initialisation du port série (debug)

  for (int i = 0; i < numScreens; i++) 
  {
  selectChannel(i);                                                           // Activation chaque écran un par un
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR); // Initialise l’écran
  display.clearDisplay();                                                     // Vidage de l’écran
  display.display();                                                          // Nettoyage
  }
  }
  // =====================================================================================================================================================================
  

  // ======================================================== Boucle principale : gestion du générique + animation =======================================================
  void loop() 
  {
  for (int step = 0; step < numLines; step++) 
  {
  // Affiche une "pile" de lignes du bas vers le haut sur les 6 écrans
  for (int i = 0; i < numScreens; i++) 
  {
  int textIndex = step - i;                                                   // Calcul de la ligne correspondante
  if (textIndex >= 0) 
  {
  showLineOnScreen(numScreens - 1 - i, text[textIndex]);                      // Empilement du bas vers le haut
  } 
  else 
  {
  showLineOnScreen(numScreens - 1 - i, "");                                   // Ligne vide si en début de défilement
  }
  }
  //delay(2000);                                                                // Attente entre chaque ligne (2 secondes)
  delay(1500);
  }
  
  for (int i = 0; i < numScreens; i++)                                        // Nettoyage des écrans après le générique
  {
  selectChannel(i);
  display.clearDisplay();
  display.display();
  }

  // Lancement de l'animation de flashs alternés
  flashAlternatingScreens(20, 200);                                           // 20 cycles, 200ms entre chaque inversion

  delay(1500);                                                                // Pause avant de recommencer
  }
