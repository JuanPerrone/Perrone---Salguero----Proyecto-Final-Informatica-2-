const int botonIzquierda = 2;
const int botonDerecha = 3;
int posicionPatineta = 400; // Posición inicial de la patineta
bool juegoIniciado = false;

void setup() {
  pinMode(botonIzquierda, INPUT_PULLUP);
  pinMode(botonDerecha, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(botonIzquierda) == HIGH) {
    posicionPatineta -= 15; // Mover la patineta hacia la izquierda
    if (posicionPatineta < 40) {
      posicionPatineta = 40;
    }
    Serial.println(posicionPatineta); // Enviar posición a Processing
  }

  if (digitalRead(botonDerecha) == HIGH) {
    posicionPatineta += 15; // Mover la patineta hacia la derecha
    if (posicionPatineta > 760) {
      posicionPatineta = 760;
    }
    Serial.println(posicionPatineta); // Enviar posición a Processing
  }

  if (!juegoIniciado) {
    // Enviar la posición inicial al iniciar el juego desde Processing
    Serial.println(400); // Valor predeterminado para la posición inicial
    juegoIniciado = true;
  }
  
  delay(50); // Pequeña pausa para estabilidad
}

