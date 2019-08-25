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
*	Ultima Versión: 23 de Agosto del 2019
**************************************************************************************************************************************/
//DaGaMa_jû-san 

//>>>ASIGNACION DE PINES DEL ARDUINO UNO

	//Entradas Analógicas >> MAX A6

int fotR[1]={A0}; // Sensores fotosensibles/Fotoresistencis
int senT[1]={A1}; // Sensores de temperatura LM45
int senH[1]={A2}; // Sensores de humedad

	//Salidas Analógicas >> PINES PWM

#define led_1 3 //Iluminación 1
#define led_2 5 //Iluminación 2

#define vent_1 10 //Ventilación 1
#define vent_2 11 //Ventilación 2

	//Salidas Digitales 

#define bomAgua 4 //Bomba de agua para riego

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
//____________________________________________________________________________________________________________________________________

//CONFIGURACIÓN DE LOS PINES Y CARACTERISTICAS DEL PROGRAMA >> SET UP

void setup(){

  n.getHardware()->setBaud(500000);

  //Configuración de los pines de entrada
  for(i=0,i<1;i++){
  	pinMode(fotR[1],INPUT);
  	pinMode(senT[1],INPUT);
  	pinMode(senH[1],INPUT);
  							}//Fiin for configuración pines de entrada

  //Configuración de los pines de salida

  pinMode(led_1,OUTPUT); //Iluminación
  pinMode(led_2,OUTPUT);

  pinMode(vent_1,OUTPUT); //Ventilación
  pinMode(vent_2,OUTPUT);

  pinMode(bomAgua,OUTPUT); //Riego

}//FIN DEL SETUP
//____________________________________________________________________________________________________________________________________

//>>>FUNCIONES

	//Sistema de Iluminación
boolean sisIlm(){

/*Sistema de Iluminación el cual funciona únicamente con un comportamiento reactivo de 2 estados, en el cual responderá solamente
durante un rango específico de valores que no alteren el ciclo día y noche, y donde se esperará a fin de ciclo para el término de cambio de estado, o en caso de que se intervenga con el sistema de ventilación, utilizando una señal pwm para el control de la intensidad luminosa en todo el invernadero

Prioridad: BAJA   Orden: 1		*/

	for(i=0;i<1;i++){ //Obtención datos
		valor_Luz[i]=analogRead(fotR[i]); }

	//Posible rando de valores que oscilará entre el 60% y 80% del valor total

	if(valor_Luz[0]>=613.8 || valor_Luz[0]<=811.4){ //Activando iluminación
		digitalWrite(led_1,HIGH);
		digitalWrite(led_2,HIGH);	}

	else{
		digitalWrite(led_1,LOW);
		digitalWrite(led_2,LOW);	}
	
	return true;
}//Fin del Sistema de Iluminación

//------------------------------------------------------------------------------------------------------------------------------------

	//Sistema de Ventilación
boolean sisVen(){

/*Sistema de Ventilación el cual funciona como un comportamiento reactivo proporcional a la entrada del sensor de temperatura, esto es, que en función de la temperatura registrada en el invernadero, utilizando una señal pwm para el control de la velocidad de los ventiladores; el cual se mantendrá en un ciclo cerrado hasta que se alcancen las temperaturas óptimas internas de temperatura, lleando incluso a intervenir con el sistema de intensidad luminosa en caso de ser necesario. 

Prioridad: ALTA   Orden: 2		*/

	int temp_OK=0;

	while(temp_OK!=1){

		for(i=0;i<1;i++){ //Obtención datos
			valor_Temp[i]=analogRead(senT[i]); }

		//Posible valor límite del 80% de la temperatura

		if(valor_Temp[0]>=811.4){ //Activando iluminación

			temp_OK=1;
			digitalWrite(vent_1,HIGH);
			digitalWrite(vent_2,HIGH);

			}	

		else{
			digitalWrite(vent_1,LOW);
			digitalWrite(vent_2,LOW);
			temp_OK=0;	}

		}//fin while
	
	return true;
}//Fin del Sistema de Ventilación

//------------------------------------------------------------------------------------------------------------------------------------

	//Sistema de Riego
boolean sisRiego(){

/*Sistema de Riego el cual funciona únicamente con un comportamiento reactivo de 2 estados ligado a tiempo, el cual responderá con la humedad presente en la tierra de invernadero, activando la bomba de agua por unos instantes establecidos para el riego en función del a cantidad de agua vertida durante el riego, y con el cual podrá proseguir con el ciclo completo del programa.

Prioridad: MEDIA   Orden: 3		*/

	for(i=0;i<1;i++){ //Obtención datos
		valor_Hum[i]=analogRead(senH[i]); }

	//Posible valor límite de humedad menor de 60%

	if(valor_Hum[0]<=613.8){ //Activando iluminación
		digitalWrite(bomAgua,HIGH);
		delay(1000) }

	else{
		digitalWrite(bomAgua,LOW);}
	
	return true;

}//Fin del Sistema de Riego

//------------------------------------------------------------------------------------------------------------------------------------
//____________________________________________________________________________________________________________________________________

//>>>FUNCION PRINCIPAL


void loop(){

	sisIlm(); //Sistema de iluminación
	sisVen(); //Sistema de Ventilación
	sisRiego(); //Sistema e Riego

	delay(500);
	
}//Fin funcion principal




