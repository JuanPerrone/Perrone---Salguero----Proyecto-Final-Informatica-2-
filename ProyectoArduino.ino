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
    int estadoConfirmarPausa = digitalRead(pinConfirmar);

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

  EstadoMenu obtenerEstadoMenu() {
    return estadoActualMenu;
  }

  bool obtenerConfirmarPresionado() {
    return confirmarPresionado;
  }

  void reiniciarMenu() {
    estadoActualMenu = NINGUNO;
    confirmarPresionado = false;
  }
};

Menu menu;

void setup() {
  pinMode(pinBotonIzquierda, INPUT);
  pinMode(pinBotonDerecha, INPUT);
  pinMode(pinConfirmar, INPUT);

  pinMode(pinLedIzquierda, OUTPUT);
  pinMode(pinLedDerecha, OUTPUT);
  pinMode(pinLedConfirmar, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  menu.actualizar();

  // Enviamos la información a través de la comunicación serial
  Serial.print(menu.obtenerEstadoMenu());
  Serial.print(",");
  Serial.print(menu.obtenerConfirmarPresionado());
  Serial.println();
  delay(50);
}