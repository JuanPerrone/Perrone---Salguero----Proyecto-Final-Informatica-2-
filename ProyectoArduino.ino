const int pinBotonIzquierda = 2;
const int pinBotonDerecha = 3;
const int pinConfirmarPausa = 4;

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
};

class Menu {
private:
  EstadoMenu estadoActualMenu;
  bool confirmarPresionado;

public:
  Menu() {
    estadoActualMenu = NINGUNO;
    confirmarPresionado = false;
  }

  void actualizar() {
    int estadoBotonIzquierda = digitalRead(pinBotonIzquierda);
    int estadoBotonDerecha = digitalRead(pinBotonDerecha);
    int estadoConfirmarPausa = digitalRead(pinConfirmarPausa);

    digitalWrite(pinLedIzquierda, estadoBotonIzquierda);
    digitalWrite(pinLedDerecha, estadoBotonDerecha);
    digitalWrite(pinLedConfirmar, estadoConfirmarPausa);

    if (confirmarPresionado == false) {
      if (estadoBotonIzquierda == HIGH) {
        estadoActualMenu = JUGAR;
      } else if (estadoBotonDerecha == HIGH) {
        estadoActualMenu = RECORDS;
      } else if (estadoConfirmarPausa == HIGH) {
        confirmarPresionado = true;
      }
    }
  }
  
  void reiniciarMenu() {
    estadoActualMenu = NINGUNO;
    confirmarPresionado = false;
  }

  EstadoMenu obtenerEstadoMenu() {
    return estadoActualMenu;
  }

  bool obtenerConfirmarPresionado() {
    return confirmarPresionado;
  }
};

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
  Juego(Menu &menuRef) : menu(menuRef) {
    introducirNombre = true;
    primeraIteracion = true;
    reiniciarJuego();
  }

  void actualizar() {
    if (introducirNombre == false) {
      estadoDelJuego = true;
      jugar();
    } else {
      estadoDelJuego = false;
      procesarMenu();
    }
  }

  void jugar() {
    if (menu.obtenerEstadoMenu() == JUGAR && menu.obtenerConfirmarPresionado() == true) {
      moverPatineta();
      moverPelota();
    }
  }

  void reiniciarJuego() {
    if (introducirNombre == true) {
      puntaje = 0;
      posicionPatineta = 400;
      posicionPelotaX = 0;
    }
    if (menu.obtenerEstadoMenu() == JUGAR) {
      posicionPelotaY = 0;
    }
    introducirNombre = false;
  }

  void moverPatineta() {
    if (digitalRead(pinBotonIzquierda) == HIGH && posicionPatineta > 0) {
      posicionPatineta -= 20;
    } else if (digitalRead(pinBotonDerecha) == HIGH && posicionPatineta < anchoJuego - anchoPatineta) {
      posicionPatineta += 20;
    }
  }

  void moverPelota() {
    if (primeraIteracion == true){ 
      posicionPelotaY = 0;
      posicionPelotaX = random(0, anchoJuego - diametroPelota);
    } else {
      posicionPelotaY += 15;
      verificarColision();
    }
    primeraIteracion = false;
  }

  void verificarColision() {
    if (posicionPelotaY > altoJuego) {
      introducirNombre = true;
    }
    if (posicionPelotaY + diametroPelota > altoJuego - altoPatineta && posicionPelotaX > posicionPatineta && posicionPelotaX < posicionPatineta + anchoPatineta) {
      puntaje++;
      posicionPelotaX = random(0, anchoJuego - diametroPelota);
      reiniciarJuego();
    }
  }

  void procesarMenu() {
    if (menu.obtenerEstadoMenu() == JUGAR && menu.obtenerConfirmarPresionado() == true) {
      reiniciarJuego();
      menu.reiniciarMenu();
      primeraIteracion = true;
    }
  }

  int obtenerPuntaje() {
   return puntaje; 
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

  bool obtenerEstadoDelJuego() { 
    return estadoDelJuego; 
  }

  bool obtenerIntroducirNombre () { 
    return introducirNombre; 
  }
};

Menu menu;
Juego juego(menu);

void setup() {
  pinMode(pinBotonIzquierda, INPUT);
  pinMode(pinBotonDerecha, INPUT);
  pinMode(pinConfirmarPausa, INPUT);

  pinMode(pinLedIzquierda, OUTPUT);
  pinMode(pinLedDerecha, OUTPUT);
  pinMode(pinLedConfirmar, OUTPUT);

  randomSeed(analogRead(0));  // Semilla aleatoria para la generación de la pelota
  Serial.begin(9600);
}

void loop() {
  menu.actualizar();
  if (menu.obtenerEstadoMenu() == JUGAR && menu.obtenerConfirmarPresionado() == true) {
    juego.actualizar();
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
  delay(50);
}
