/*************************************************************************************************************************************
*	SIGNOS INGENIERÍA	<PROYECTO INVERNADERO>
*
*	ENCARGADO: Daniel Garcés Marín
*
*	Programación para la creación de un invernadero automático con el control de las siguientes características:
*		-Control de la luz ambiental, sensada por un sensor fotosensible, por medio de iluminación led.
*		-Control de temperatura ambiental, sensada por medio de un transistor de sensor de temperatura, por medio de ventiladores.
*		-Control de humedad, sensado por medio de un sensor de humedad, por medio de una bomba de agua para su riego.
*
*	Ultima Versión: 25 de Agosto del 2019
**************************************************************************************************************************************/
//DaGaMa_jû-san 

//>>>BIBLIOTECAS
#include <LiquidCrystal.h> //Uso de una pantalla LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //Configurando pantalla LCD 2X16

//>>>ASIGNACION DE PINES DEL ARDUINO UNO

	//Entradas Analógicas >> MAX A6

int fotR[1]={A0}; // Sensores fotosensibles/Fotoresistencis
int senT[1]={A1}; // Sensores de temperatura LM45
int senH[1]={A2}; // Sensores de humedad

	//Salidas Analógicas >> PINES PWM

#define led_1 6 //Iluminación 1
#define led_2 9 //Iluminación 2

#define vent_1 10 //Ventilación 1
//#define vent_2 11 //Ventilación 2

	//Salidas Digitales 
#define bomAgua 7 //Bomba de agua para riego

//------------------------------------------------------------------------------------------------------------------------------------

//>>>VARIABLES GLOBALES

	//Variables de almacenamiento de información

float valor_Luz[1]={0}; //Almacenamiento de los valores de luz obtenidas
float valor_Temp[1]={0}; //Almacenamiento de los valores de temperaturas obtenidos
float valor_Hum[1]={0}; //Almacenamiento de los valores de humedad necesarios

	//Variables de informacion a desplegar

float intensidad_Led[1]={0}; //Intensidad luminosa con la que se prenderán los leds
float vel_Vent[1]={0}; //Velocidad con la que se accionarán los ventiladores

	//Varios
int i=0,j=0,k=0; //Contadores

	//Variables de control del ambiente

float luz_Sup=811.4; //Intensidad mínima regular de luz solar, 80% del valor total
float luz_inf=613.8; //Intensidad máxima regular de ciclo nocturno, 60% del valor total

//float temp_lim=811.4; //Temperatura límite superior del ambiente interior, 80% del valor total
float temp_lim= 25.0; //Temepratura límite superior del ambiente interior, en grados centígrados

float hum_lim=613.8; //Humedad mínima de la tierra, 60% del valor total
float tiempo_riego= 1; //Tiempo de riego medido en segundos

//____________________________________________________________________________________________________________________________________

//CONFIGURACIÓN DE LOS PINES Y CARACTERISTICAS DEl PROGRAMA >> SET UP

void setup(){

  Serial.begin(9600);  //Iniciando comunicación serial

  lcd.begin(16,2); //Inicializando LCD

  //Configuración de los pines de entrada
  for(i=0;i<1;i++){
  	pinMode(fotR[i],INPUT);
  	pinMode(senT[i],INPUT);
  	pinMode(senH[i],INPUT);
  							}//Fiin for configuración pines de entrada

  //Configuración de los pines de salida

  pinMode(led_1,OUTPUT); //Iluminación
  pinMode(led_2,OUTPUT);

  pinMode(vent_1,OUTPUT); //Ventilación
  //pinMode(vent_2,OUTPUT);

  pinMode(bomAgua,OUTPUT); //Riego

  lcd.print("Inicializando Invenadero SIGNOS INGENIERÍA\n");

}//FIN DEL SETUP
//____________________________________________________________________________________________________________________________________

//>>>FUNCIONES

	//Sistema de Iluminación
int sisIlm(float l_Sup,float l_inf){
	int luz =0;

	for(i=0;i<1;i++){ //Obtención datos
		valor_Luz[i]=analogRead(fotR[i]); }

	luz=valor_Luz[0];

	//Posible rando de valores que oscilará entre el 60% y 80% del valor total

	if(valor_Luz[0]>=613.8 || valor_Luz[0]<=811.4){ //Activando iluminación
		digitalWrite(led_1,HIGH);
		digitalWrite(led_2,HIGH);	}

	else{
		digitalWrite(led_1,LOW);
		digitalWrite(led_2,LOW);	}
	
	return luz;
}//Fin del Sistema de Iluminación

//------------------------------------------------------------------------------------------------------------------------------------

	//Sistema de Ventilación
float sisVen(float max_Tempt){

	int temp_OK=0;
	float tempt=0.0;

	while(temp_OK!=1){

		for(i=0;i<1;i++){ //Obtención datos
			valor_Temp[i]=analogRead(senT[i]); }

		//Transformación del valor leído a temperatura, sensor TMP36

			//Grados Centígrados
			tempt = ( (valor_Temp[0]*0.004882814) -5) * 100.0;

			//Grados Farenheit
			//tempt = (  ( (valor_Temp[0]*0.004882814) -5) * 100.0)*(9.0/5.0) + 32.0;

		//Transformación del valor leído a temperatura, sensor LM35

			//Grados Centígrados
			//tempt=(5.0*valor_Temp[0]*100.0)/1024.0;

			//Grados Farenheit
			//tempt = (  ( (5.0*valor_Temp[0]*100.0)/1024.0)*(9.0/5.0) + 32.0;

		//Posible valor límite del 80% de la temperatura

		if(tempt>=max_Tempt){ //Activando iluminación

			lcd.clear();

			lcd.setCursor(0,0);
			lcd.print(">>Activando Sistema de Ventilación, Temperatura:\t");
			lcd.print(tempt);
			digitalWrite(vent_1,HIGH);
			//digitalWrite(vent_2,HIGH);

			//Apagando iluminación
			digitalWrite(led_1,LOW);
			digitalWrite(led_2,LOW);

			temp_OK=1;							}//Fin Sistema de ventilación ACTIVADO

		else{
			digitalWrite(vent_1,LOW);
			//digitalWrite(vent_2,LOW);
			temp_OK=0;	}

		}//fin while
	
	return tempt;
}//Fin del Sistema de Ventilación

//------------------------------------------------------------------------------------------------------------------------------------

	//Sistema de Riego
float sisRiego(float hum_MAX,float tiempo_R){

	float humedad=0.0;
	int tiempo_Riego=0;

	for(i=0;i<1;i++){ //Obtención datos
		valor_Hum[i]=analogRead(senH[i]); }

	//Trasnformación para la medida de Humedad
	humedad=valor_Hum[1];

	//Transformación para el tiempo de riego
	tiempo_Riego=tiempo_R*1000;

	//Posible valor límite de humedad menor de 60%

	if(humedad<=hum_MAX){ //Activando iluminación
		digitalWrite(bomAgua,HIGH);

		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print(">>Activando Sistema de Riego, Humedad:\t");
		lcd.print(humedad);
		delay(tiempo_Riego); }

	else{
		digitalWrite(bomAgua,LOW);}
	
	return humedad;

}//Fin del Sistema de Riego

//------------------------------------------------------------------------------------------------------------------------------------
//____________________________________________________________________________________________________________________________________

//>>>FUNCION PRINCIPAL


void loop(){

	int ilum_actual=0;
	float tempt_actual=0.0;
	float humedad_actual=0.0;

	lcd.clear();

	lcd.setCursor(0,0);
	lcd.print("Invenadero SIGNOS ING");
	lcd.print("DATOS:");

	ilum_actual=sisIlm(luz_Sup,luz_inf); //Sistema de iluminación
	tempt_actual=sisVen(temp_lim); //Sistema de Ventilación
	humedad_actual=sisRiego(hum_lim,tiempo_riego); //Sistema e Riego

	lcd.setCursor(0,1);
	lcd.setCursor(7,1);
	lcd.print("I:");
	lcd.print(ilum_actual);
	lcd.print("T:");
	lcd.print(tempt_actual);
	lcd.print("H:");
	lcd.print(humedad_actual);


	delay(500);
	
}//Fin funcion principal




