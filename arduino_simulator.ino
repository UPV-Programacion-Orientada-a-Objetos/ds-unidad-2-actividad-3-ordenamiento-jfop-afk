/*
 * Simulador de Arduino para E-Sort
 * Genera valores aleatorios y los envia por serial
 * 
 * Configuracion:
 * - Baud Rate: 9600
 * - Board: Arduino Uno (o compatible)
 */

const int TOTAL_NUMBERS = 5000;  // Total de numeros a generar
const int DELAY_MS = 10;         // Delay entre envios (ms)

int numbersSent = 0;

void setup() {
  // Inicializar comunicacion serial a 9600 baud
  Serial.begin(9600);
  
  // Esperar a que se abra el puerto serial
  while (!Serial) {
    ; // Esperar
  }
  
  // Inicializar semilla aleatoria
  randomSeed(analogRead(0));
  
  // Mensaje de inicio
  delay(2000);
  Serial.println(999999); // Valor inicial de sincronizacion
}

void loop() {
  if (numbersSent < TOTAL_NUMBERS) {
    // Generar numero aleatorio entre 1 y 10000
    int randomNumber = random(1, 10001);
    
    // Enviar el numero
    Serial.println(randomNumber);
    
    numbersSent++;
    
    // Pequeño delay para no saturar
    delay(DELAY_MS);
  } else {
    // Terminar transmision
    delay(5000);
    // No enviar mas datos
    while(1) {
      delay(1000);
    }
  }
}