const int pinBotonIzquierda = 2;
const int pinBotonDerecha = 3;
const int pinConfirmar = 4;

const int pinLedIzquierda = 5;
const int pinLedDerecha = 6;
const int pinLedConfirmar = 7;

const int anchoJuego = 800;
const int altoJuego = 600;
const int anchoPatineta = 80;
const int altoPatineta = 20;
const int diametroPelota = 40;

enum EstadoMenu {
  NINGUNO,
  JUGAR,
  RECORDS
};  //Definicion de un enumerador, EstadoMenu, con tres valores posibles.

class Menu {
private:
  EstadoMenu estadoActualMenu;
  bool confirmarPresionado;

public:
  Menu() {
    estadoActualMenu = NINGUNO;
    confirmarPresionado = false;
  }  //Constructor.

  void actualizar() {
    int estadoBotonIzquierda = digitalRead(pinBotonIzquierda);
    int estadoBotonDerecha = digitalRead(pinBotonDerecha);
    int estadoConfirmar = digitalRead(pinConfirmar);

    digitalWrite(pinLedIzquierda, estadoBotonIzquierda);
    digitalWrite(pinLedDerecha, estadoBotonDerecha);
    digitalWrite(pinLedConfirmar, estadoConfirmar);

    if (confirmarPresionado == false) {
      if (estadoBotonIzquierda == HIGH) {
        estadoActualMenu = JUGAR;
      } else if (estadoBotonDerecha == HIGH) {
        estadoActualMenu = RECORDS;
      } else if (estadoConfirmar == HIGH) {
        confirmarPresionado = true;
      }
    }
  }  //Lee el estado de los botones y actualiza el estado del menú, manejo de pines por el estado de los botones.

  void reiniciarMenu() {
    estadoActualMenu = NINGUNO;
    confirmarPresionado = false;
  }  //Restablece el estado del menú y el estado del botón de confirmación a su valor inicial.

  EstadoMenu obtenerEstadoMenu() {
    return estadoActualMenu;
  }

  bool obtenerConfirmarPresionado() {
    return confirmarPresionado;
  }
};  //Se define la clase Menu.

class Juego {
private:
  Menu &menu;
  int puntaje;
  int posicionPatineta;
  int posicionPelotaX;
  int posicionPelotaY;
  bool introducirNombre;
  bool primeraIteracion;
  bool estadoDelJuego = false;

public:
  Juego(Menu &menuReferencia)
    : menu(menuReferencia) {
    introducirNombre = true;
    primeraIteracion = true;
    reiniciarJuego();
  }  //Constructor, se recibe por referencia a un objeto Menu.

  void actualizar() {
    if (introducirNombre == false) {
      estadoDelJuego = true;
      jugar();
    } else {
      estadoDelJuego = false;
      procesarMenu();
    }
  }  //Controla si el juego está en modo de juego o de procesar el menu.

  void jugar() {
    moverPatineta();
    moverPelota();
  }  //En modo de juego se mueve la patineta y la pelota.

  void reiniciarJuego() {
    if (introducirNombre == true) {
      puntaje = 0;
      posicionPatineta = anchoJuego / 2 - anchoPatineta / 2;  //360
      posicionPelotaX = 0;
    }
    if (menu.obtenerEstadoMenu() == JUGAR) {
      posicionPelotaY = 0;
    }
    introducirNombre = false;
  }  //Maneja las variables del juego dependiendo de cada situacion.

  void moverPatineta() {
    if (digitalRead(pinBotonIzquierda) == HIGH && posicionPatineta > 0) {
      posicionPatineta -= 15;
    } else if (digitalRead(pinBotonDerecha) == HIGH && posicionPatineta < anchoJuego - anchoPatineta) {
      posicionPatineta += 15;
    }
  }  //Lee el estado de los botones de la patineta y mueve la patineta.

  void moverPelota() {
    if (primeraIteracion == true) {
      posicionPelotaY = 0;
      posicionPelotaX = random(diametroPelota / 2, anchoJuego - diametroPelota / 2);
    } else {
      //if (puntaje < 4) {
      posicionPelotaY += 10;
      /*} else {
        posicionPelotaY += ;
      }*/
      verificarColision();
      //}
    }
    primeraIteracion = false;
  }  //Controla el movimiento de la pelota en el juego y la detección de colisiones .

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
  }  //Comprueba si ha ocurrido alguna colisión entre la pelota y la patineta.

  void procesarMenu() {
    reiniciarJuego();
    menu.reiniciarMenu();
    primeraIteracion = true;
  }  //Procesa el menu.

  bool obtenerEstadoDelJuego() {
    return estadoDelJuego;
  }

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
};

class Records {
private:
  Menu &menu;
  bool volver;
  bool confirmarPresionado;
  bool bandera;

public:
  Records(Menu &menuReferencia)
    : menu(menuReferencia) {
    confirmarPresionado = false;
    bandera = false;
  }  //Constructor.

public:
  void volverAlMenu() {
    if (digitalRead(pinConfirmar) == LOW) {
      bandera = true;
    }
    if (bandera == true) {
      volver = digitalRead(pinConfirmar);
      if (volver == HIGH) {
        menu.reiniciarMenu();
        bandera = false;
        delay(200);
      }
    }
  }
};

Menu menu;
Juego juego(menu);
Records records(menu);

void setup() {
  pinMode(pinBotonIzquierda, INPUT);
  pinMode(pinBotonDerecha, INPUT);
  pinMode(pinConfirmar, INPUT);

  pinMode(pinLedIzquierda, OUTPUT);
  pinMode(pinLedDerecha, OUTPUT);
  pinMode(pinLedConfirmar, OUTPUT);

  // Semilla aleatoria para la generación de la pelota
  randomSeed(analogRead(0));
  // Inicialización de la comunicación serial
  Serial.begin(9600);
}

void loop() {

  menu.actualizar();

  if (menu.obtenerEstadoMenu() == JUGAR && menu.obtenerConfirmarPresionado() == true) {
    juego.actualizar();
  }

  if (menu.obtenerEstadoMenu() == RECORDS && menu.obtenerConfirmarPresionado() == true) {
    records.volverAlMenu();
  }

  // Enviamos la información a través de la comunicación serial
  Serial.print(menu.obtenerEstadoMenu());
  Serial.print(",");
  Serial.print(menu.obtenerConfirmarPresionado());
  Serial.print(",");
  Serial.print(juego.obtenerEstadoDelJuego());
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
  Serial.println();
  delay(30);
}
