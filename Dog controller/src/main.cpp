#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>



//définition des constantes, variables et fonctions concernant la communication  
String address_1 = "00001";
RF24 radioNRF(7, 8);
int com_chan = 115; // channel de communication pour l’antenne (2400+115 = 2515MHz)
String radioNRFData;
char sendData[32];
void nrf24_init(uint8_t channel);
bool nrf24_sendData(String dataToSend);

//définition des constantes,variables et fonctions concernant le joystick


int axeX = A0; // signal de l'axe X sur entrée A0 
int axeY = A1; // signal de l'axe Y sur entrée A1 
int BP7 = 7; // Bouton-poussoir en broche 7 
float dead_zone_radius = 50; // rayon de la dead zone du joystick
int offset_x = -10 + 1023/2; // ajout de 1023/2 pour centrer les coordonnées sur le point d’équilibre du joystick 
int offset_y = 8 + 1023/2;  // ajout de 1023/2 pour centrer les coordonnées sur le point d’équilibre du joystick
float angle = 0; // angle que fait le joystick par rapport au centre
float puissance = 0;

void init_joystick();
int get_mouvement(); // renvoie 0 si pas de mouvement, 1 pour avancer, -1 pour reculer, 2 pour la droite -2 pour la gauche 


// définition des constantes, variables et fonctions pour l’afficheur lcd
LiquidCrystal_I2C lcd(0x27,20,4); // création de l’objet lcd d’adresse 0x27 de taile 20x4 
void printlcd(const char ligne1[],const char ligne2[],const char ligne3[],const char ligne4[]);
void lcd_init();
const char *mouvement_print[] = {"Tourne a gauche", "Recule", "Ne bouge pas","Avance","Tourne a droite"};// déclaration des message à afficher en fonction de l’état du système
int last_state = 10; // pour comparez si il y a eu un changement afin d’éviter de raffraichir l’écran trop souvent
int last_puissance = 0;

/////////////////////////////Test////////////////////////////

int total = 0;

void test_print_joystick_angle_power(); // déclaration de l’affichage d’informations de debug pour le joystick
void test_print_joystick_x_y(int x, int y);




void setup() {
  Serial.begin(57600);
  delay(1000);
  lcd_init();
  nrf24_init(com_chan); // initialise la communication par antenne radio
  init_joystick(); // initialise le joystick
}
void print_state(int state){

  if ((last_state != state) || last_puissance != int(puissance)){
    char puissance_c[10];
    sprintf(puissance_c, "%d", int(puissance)); 
    printlcd("Le chien ",mouvement_print[state+2],puissance_c,"");
    last_state = state;
    last_puissance = int(puissance);
  }

}
void loop() {
  int state = get_mouvement();
  print_state(state);
  
  bool b = nrf24_sendData(String(state)+','+String(puissance));
  if( !b ){Serial.println("Erreur");}
  total += 1;

  
  Serial.println(String(state)+','+String(puissance));
  // test
  //Serial.println(state);
  //test_print_joystick_angle_power();
  delay(50);

}






void nrf24_init(uint8_t channel) {// fonction pour iniatialiser le module de communication NRF24
  printf_begin();
  radioNRF.begin();
  radioNRF.setChannel(channel); // communication channel (0-125)
  radioNRF.setDataRate(RF24_250KBPS); // communication speed (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS)
  radioNRF.enableDynamicPayloads();
  radioNRF.setPALevel(RF24_PA_MIN);// amplification (RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH et RF24_PA_MAX)
  radioNRF.printDetails();
  radioNRF.openWritingPipe((uint8_t *)address_1.c_str()); 
}


bool nrf24_sendData(String dataToSend) { // fonction pour envoyer les données par transimission du NRF24
  //delay(50);
  //Serial.println("Envoi de: " + dataToSend);
  radioNRF.stopListening();
  char inputChar[dataToSend.length() + 1] ;
  dataToSend.toCharArray(inputChar, dataToSend.length()+1);
  return radioNRF.write(&inputChar, sizeof(inputChar));
}




void init_joystick(){ // initialise le joystick
  pinMode (axeX, INPUT); // définition de A0 comme une entrée 
  pinMode (axeY, INPUT); // définition de A1 comme une entrée 
  pinMode (BP7, INPUT); // définition de 7 comme une entrée 
  digitalWrite(BP7, HIGH); // Activation de la résistance de Pull-Up 
}


int get_mouvement(){ // renvoie 0 si pas de mouvement, 1 pour avancer, -1 pour reculer, 2 pour la droite -2 pour la gauche 
  float x = analogRead(axeX)-(float)(offset_x);
  float y = -(analogRead(axeY)-(float)(offset_y));
  //print_joystick_x_y(x,y);
  float r = sqrt(pow(x,2) + pow(y,2)); // calcul de la distance des coordonnées par rapport au centre du joystick
  if ( r>= dead_zone_radius){
    angle = atan2(y,x)*180/PI;
    puissance = (float)r*100/512;
    if ( puissance > 100 ){
      puissance = (float)100;
    }
    if ( (float)(3*180/4) >= angle && angle > (float)(180/4)){
      return 1;
    }
    else if (( (float)(180/4) >= angle && angle > (float)(-180/4)))
    {
      return 2;
    }
    else if (( (float)(-180/4) >= angle && angle > -(float)(3*180/4)))
    {
      return -1;
    }
    else{
      return -2;
    }
  }
  puissance = 0;
  return 0;
}




void printlcd(const char ligne1[], const char ligne2[],const char ligne3[],const char ligne4[]){ // attention au type de données (possibilité d’améliorer le code en prenant d’autres types à condition de bien déclarer mouvement_print avec les types associés), la fonction peut ne pas fonctionner avec des String!!!

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(ligne1);
  lcd.setCursor(0,1);
  lcd.print(ligne2);  
  lcd.setCursor(0,2);
  lcd.print(ligne3);
  lcd.setCursor(0,4);
  lcd.print(ligne4);
  }


void lcd_init(){
  lcd.init();
  lcd.backlight();
}



///////////////////////////////////Test/////////////////////////////////////


void test_print_joystick_angle_power(){
  Serial.print("L'angle est de ");
  Serial.print(angle);
  Serial.println(" degré");
  Serial.print("La puissance est de ");
  Serial.print(puissance);
  Serial.println("%");
}

void test_print_joystick_x_y(int x, int y){
    Serial.print ("Axe X:"); 
  Serial.print (x, 4); 
  Serial.print ("V, "); 
 
  Serial.print ("Axe Y:"); 
  Serial.print (y, 4); 
  Serial.print ("V, "); 
  Serial.println();
}