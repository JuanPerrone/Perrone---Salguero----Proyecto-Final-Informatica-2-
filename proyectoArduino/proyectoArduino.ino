#define memoriaArray 250

const int pinBotonIzquierda = 2;
const int pinBotonDerecha = 3;
const int pinBotonConfirmar = 4;
const int pinBotonRec = A4;

const int pinLedIzquierda = 5;
const int pinLedDerecha = 6;
const int pinLedConfirmar = 7;
const int pinLedRec = A5;

int segmentos[7] = { 8, 9, 10, 11, 12, 13, A1 };
int numeroDigitos[11][7] = {
  { 1, 1, 1, 1, 1, 1, 0 },  // 0
  { 0, 1, 1, 0, 0, 0, 0 },  // 1
  { 1, 1, 0, 1, 1, 0, 1 },  // 2
  { 1, 1, 1, 1, 0, 0, 1 },  // 3
  { 0, 1, 1, 0, 0, 1, 1 },  // 4
  { 1, 0, 1, 1, 0, 1, 1 },  // 5
  { 1, 0, 1, 1, 1, 1, 1 },  // 6
  { 1, 1, 1, 0, 0, 0, 0 },  // 7
  { 1, 1, 1, 1, 1, 1, 1 },  // 8
  { 1, 1, 1, 1, 0, 1, 1 },  // 9
  { 0, 0, 0, 0, 0, 0, 1 }   // -
};

enum EstadoMenu {
  NINGUNO,
  JUGAR,
  RECORDS,
  GRABACION
};  // Fin de la definicion del enumerador.

struct DatosPartida {
  int posicionPatinetaRec;
  int posicionPelotaXRec;
  int posicionPelotaYRec;
};  // Define una estructura para almacenar los datos de cada partida. - Rec = recording = grabacion.

DatosPartida grabacion[memoriaArray];  // Array para almacenar las estructuras DatosPartida.
int indiceGrabacion;                   // Índice para controlar la posición en el array.
bool enviandoGrabacion = true;
bool controlIndiceGrabacion = false;

class Menu {
private:
  EstadoMenu estadoActualMenu;
  bool confirmarPresionado;

public:
  Menu() {
    estadoActualMenu = NINGUNO;
    confirmarPresionado = false;
  }  // Fin del constructor.

  void actualizar() {
    int estadoBotonIzquierda = digitalRead(pinBotonIzquierda);
    int estadoBotonDerecha = digitalRead(pinBotonDerecha);
    int estadoBotonConfirmar = digitalRead(pinBotonConfirmar);
    int estadoBotonRec = digitalRead(pinBotonRec);

    digitalWrite(pinLedIzquierda, estadoBotonIzquierda);
    digitalWrite(pinLedDerecha, estadoBotonDerecha);
    digitalWrite(pinLedConfirmar, estadoBotonConfirmar);
    digitalWrite(pinLedRec, estadoBotonRec);

    if (confirmarPresionado == false) {
      if (estadoBotonIzquierda == HIGH) {
        estadoActualMenu = JUGAR;
      } else if (estadoBotonDerecha == HIGH) {
        estadoActualMenu = RECORDS;
      } else if (estadoBotonRec == HIGH) {
        estadoActualMenu = GRABACION;
      } else if (estadoBotonConfirmar == HIGH) {
        confirmarPresionado = true;
        if (estadoActualMenu == NINGUNO) {
          confirmarPresionado = false;
        }
        if (estadoActualMenu == JUGAR) {
          controlIndiceGrabacion = !controlIndiceGrabacion;
        }
      }  // Fin if estadoBotonConfirmar == HIGH.
    }    // Fin if confirmarPresionado == false.
  }      // Fin del método actualizar - Lee el estado de los botones y actualiza el estado del menú, manejo de los pines de leds por el estado de los botones.

  void reiniciarMenu() {
    estadoActualMenu = NINGUNO;
    confirmarPresionado = false;
  }  // Restablece el estado del menú y el estado del botón de confirmación a su valor inicial.

  EstadoMenu obtenerEstadoMenu() {
    return estadoActualMenu;
  }

  bool obtenerConfirmarPresionado() {
    return confirmarPresionado;
  }
};  // Fin de la clase Menu.

class Juego {
private:
  int anchoJuego = 800;
  int altoJuego = 600;
  int anchoPatineta = 80;
  int altoPatineta = 20;
  int diametroPelota = 40;
  Menu &menu;
  int puntaje;
  int posicionPatineta;
  int posicionPelotaX;
  int posicionPelotaY;
  bool introducirNombre;
  bool primeraIteracion;

public:
  Juego(Menu &menuReferencia)
    : menu(menuReferencia) {
    introducirNombre = true;
    primeraIteracion = true;
    reiniciarJuego();
  }  // Fin del constructor. - Se recibe por referencia a un objeto Menu.

  void actualizar() {
    if (introducirNombre == true) {
      procesarMenu();
    } else {
      jugar();
    }
  }  // Si el juego ha terminado, introducimos el nombre y procesamos el menu, sino el juego sigue.

  void jugar() {
    moverPatineta();
    moverPelota();
  }  // Control de la patineta y la pelota durante el juego.

  void reiniciarJuego() {
    if (introducirNombre == true) {
      puntaje = 0;
      posicionPatineta = anchoJuego / 2 - anchoPatineta / 2;  // posicionPatineta = 360
      posicionPelotaX = 0;
    }
    if (menu.obtenerEstadoMenu() == JUGAR) {
      posicionPelotaY = 0;
    }
    introducirNombre = false;
  }  // Maneja las variables del juego, en caso de haber terminado la partida o no.

  void moverPatineta() {
    if (digitalRead(pinBotonIzquierda) == HIGH && posicionPatineta > 0) {
      posicionPatineta -= 15;
    } else if (digitalRead(pinBotonDerecha) == HIGH && posicionPatineta < anchoJuego - anchoPatineta) {
      posicionPatineta += 15;
    }
  }  // Lee el estado de los botones de la patineta y mueve la patineta.

  void moverPelota() {
    if (primeraIteracion == true) {
      posicionPelotaY = 0;
      posicionPelotaX = random(diametroPelota / 2, anchoJuego - diametroPelota / 2);
    } else {
      posicionPelotaY += 10;
      verificarColision();
    }
    primeraIteracion = false;
  }  // Controla el movimiento de la pelota en el juego y la detección de colisiones.

  void verificarColision() {
    if (posicionPelotaY + diametroPelota / 2 > altoJuego) {
      introducirNombre = true;
    }
    if (posicionPelotaY + diametroPelota / 2 > altoJuego - altoPatineta
        && posicionPatineta + anchoPatineta > posicionPelotaX - diametroPelota / 2
        && posicionPatineta < posicionPelotaX + diametroPelota / 2) {
      puntaje++;
      posicionPelotaX = random(diametroPelota / 2, anchoJuego - diametroPelota / 2);
      reiniciarJuego();
    }
  }  // Comprueba si ha ocurrido alguna colisión entre la pelota y la patineta.

  void procesarMenu() {
    reiniciarJuego();
    menu.reiniciarMenu();
    primeraIteracion = true;
  }  // Se reinician las variables del juego y se procesa el menu.

  int obtenerPosicionPatineta() {
    return posicionPatineta;
  }

  int obtenerPosicionPelotaX() {
    return posicionPelotaX;
  }

  int obtenerPosicionPelotaY() {
    return posicionPelotaY;
  }

  int obtenerPuntaje() {
    return puntaje;
  }

  bool obtenerIntroducirNombre() {
    return introducirNombre;
  }
};  // Fin de la clase Juego.

class VolverMenu {
private:
  Menu &menu;
  bool confirmarVolver;
  bool bandera;

public:
  VolverMenu(Menu &menuReferencia)
    : menu(menuReferencia) {
    confirmarVolver = false;
    bandera = false;
  }  // Fin del constructor.

  void confirmarVolverAlMenu() {
    if (digitalRead(pinBotonConfirmar) == LOW) {
      bandera = true;
    }

    if (bandera == true) {
      confirmarVolver = digitalRead(pinBotonConfirmar);
      if (confirmarVolver == HIGH) {
        menu.reiniciarMenu();
        bandera = false;
        enviandoGrabacion = true;
      }
    }  // Fin if bandera == true.
  }    // Fin del método confirmarVolverAlMenu.
};     // Fin de la clase VolverMenu.

Menu menu;
Juego juego(menu);
VolverMenu volverMenu(menu);

void setup() {
  pinMode(pinBotonIzquierda, INPUT);
  pinMode(pinBotonDerecha, INPUT);
  pinMode(pinBotonConfirmar, INPUT);
  pinMode(pinBotonRec, INPUT);

  pinMode(pinLedIzquierda, OUTPUT);
  pinMode(pinLedDerecha, OUTPUT);
  pinMode(pinLedConfirmar, OUTPUT);
  pinMode(pinLedRec, OUTPUT);

  for (int i = 0; i < 7; i++) {
    pinMode(segmentos[i], OUTPUT);
    digitalWrite(segmentos[i], LOW);
  }  // Apaga todos los segmentos inicialmente

  // Semilla aleatoria para la generación de la pelota
  randomSeed(analogRead(0));

  // Inicialización de la comunicación serial
  Serial.begin(9600);
}

void loop() {

  menu.actualizar();

  if (menu.obtenerEstadoMenu() == JUGAR && menu.obtenerConfirmarPresionado() == true) {

    if (controlIndiceGrabacion == true) {
      indiceGrabacion = 0;
      controlIndiceGrabacion = false;
    }  // Define indiceGrabacion = 0, para "sobreescribir" en el array la informacion cada vez que empezamos un juego nuevo.

    juego.actualizar();

    if (indiceGrabacion < memoriaArray) {
      grabacion[indiceGrabacion].posicionPatinetaRec = juego.obtenerPosicionPatineta();
      grabacion[indiceGrabacion].posicionPelotaXRec = juego.obtenerPosicionPelotaX();
      grabacion[indiceGrabacion].posicionPelotaYRec = juego.obtenerPosicionPelotaY();
      indiceGrabacion++;
    }  // Guarda los datos de un juego en el array, haciendo uso de la "memoria de arduino", hasta cierto valor.

  } else {
    for (int contador = 0; contador < 7; contador++) {
      if (numeroDigitos[10][contador] == 1) {
        digitalWrite(segmentos[contador], HIGH);
      } else {
        digitalWrite(segmentos[contador], LOW);
      }
    }  // Fin de for.
  }    // Fin de else.

  if (menu.obtenerEstadoMenu() == RECORDS && menu.obtenerConfirmarPresionado() == true) {
    volverMenu.confirmarVolverAlMenu();
  }

  if (Serial.available() > 0) {
    int puntajeDisplay = Serial.read();

    if (puntajeDisplay >= 0 && puntajeDisplay <= 9) {
      for (int contador = 0; contador < 7; contador++) {
        if (numeroDigitos[puntajeDisplay][contador] == 1) {
          digitalWrite(segmentos[contador], HIGH);
        } else {
          digitalWrite(segmentos[contador], LOW);
        }
      }  // Fin del for.
    } else {
      for (int contador = 0; contador < 7; contador++) {
        if (numeroDigitos[10][contador] == 1) {
          digitalWrite(segmentos[contador], HIGH);
        } else {
          digitalWrite(segmentos[contador], LOW);
        }
      }  // Fin del for.
    }    // Fin del else.
  }      // Fin de if Serial.available() > 0 - Si se recibe informacion desde Processing sobre el puntaje, lo mostramos en el display.

  // Enviamos la información a través de la comunicación serial
  Serial.print(menu.obtenerEstadoMenu());
  Serial.print(",");
  Serial.print(menu.obtenerConfirmarPresionado());
  Serial.print(",");
  Serial.print(juego.obtenerPosicionPatineta());
  Serial.print(",");
  Serial.print(juego.obtenerPosicionPelotaX());
  Serial.print(",");
  Serial.print(juego.obtenerPosicionPelotaY());
  Serial.print(",");
  Serial.print(juego.obtenerPuntaje());
  Serial.print(",");
  Serial.print(juego.obtenerIntroducirNombre());

  if (menu.obtenerEstadoMenu() == GRABACION && menu.obtenerConfirmarPresionado() == true) {
    if (enviandoGrabacion == true) {
      // Envio por comunicacion serial informacion del array grabacion.
      for (int contador = 0; contador < indiceGrabacion; contador++) {
        Serial.print(",");
        Serial.print(grabacion[contador].posicionPatinetaRec);
        Serial.print(",");
        Serial.print(grabacion[contador].posicionPelotaXRec);
        Serial.print(",");
        Serial.print(grabacion[contador].posicionPelotaYRec);
      }  // Fin del for.
    }    // Fin del if.

    if (Serial.available() > 0) {
      int detener = Serial.read();  // Si recibo un 10, detengo el envio de informacion del array grabacion.
      if (detener == 10) {
        enviandoGrabacion = false;
      }
    }  // Fin del if Serial.available() > 0.

    volverMenu.confirmarVolverAlMenu();
  }  // Fin de if menu.obtenerEstadoMenu() == GRABACION && menu.obtenerConfirmarPresionado() == true.

  Serial.println();
  delay(30);
}
