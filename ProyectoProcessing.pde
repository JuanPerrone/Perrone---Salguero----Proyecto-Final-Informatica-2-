import processing.serial.*;

Serial miPuerto;  // Objeto Serial para la comunicación con Arduino
int diametroPelota = 40;
int anchoPatineta = 80;
int altoPatineta = 20;
int estadoMenu;
boolean confirmarPresionado;
boolean estadoDelJuego;
int posicionPatineta;
int posicionPelotaX;
int posicionPelotaY;
int puntaje;

class Menu {
  public
  void dibujar() {
    // Título
    fill(0);
    textSize(32);
    textAlign(CENTER, CENTER);
    text("LA CANASTA", width / 2, height / 4);

    // Botón "Jugar"
    if (estadoMenu == 1) {
      fill(255, 0, 0);
    } else {
      fill(0, 255, 0);
    }
    rect(width / 4, height / 2, width / 2, 50);

    fill(0);
    textSize(24);
    textAlign(CENTER, CENTER);
    text("Jugar", width / 2, height / 2 + 25);

    // Botón "Records"
    if (estadoMenu == 2) {
      fill(255, 0, 0);
    } else {
      fill(0, 255, 0);
    }
    rect(width / 4, height / 2 + 60, width / 2, 50);

    fill(0);
    text("Records", width / 2, height / 2 + 85);
  }
}

class Juego {
  public void dibujar() {
    // Dibuja la patineta
    fill(0, 0, 255); // Azul
    rect(posicionPatineta, height - altoPatineta, anchoPatineta, altoPatineta);

    // Dibuja la pelota
    fill(255, 0, 0); // Rojo
    ellipse(posicionPelotaX, posicionPelotaY, diametroPelota, diametroPelota);

    // Muestra el puntaje
    fill(0);
    textSize(20);
    textAlign(RIGHT, TOP);
    text("Puntaje: " + puntaje, width - 10, 10);
  }
}

Menu menu;
Juego juego;

void setup() {
  /*for (String puerto : Serial.list()) {
   println("Puerto disponible: " + puerto);
   }*/
  size(800, 600);
  String estePuerto = Serial.list()[1];
  miPuerto = new Serial(this, estePuerto, 9600);
  menu = new Menu(); // Inicializa la instancia de la clase Menu
  juego = new Juego(); // Inicializa la instancia de la clase Juego
}

void draw() {
  background(255);

  // Lee datos del puerto serial
  if (miPuerto.available() > 0) {
    String info = miPuerto.readStringUntil('\n');
    //println("Info recibida: " + info);
    if (info != null) {
      // Divide los datos en partes
      String[] partes = split(trim(info), ',');
      if (partes.length == 7) {
        estadoMenu = int(partes[0]);
        confirmarPresionado = partes[1].equals("1");
        estadoDelJuego = partes[2].equals("1");
        posicionPatineta = int(partes[3]);
        posicionPelotaX = int(partes[4]);
        posicionPelotaY = int(partes[5]);
        puntaje = int(partes[6]);
      }
    }
  }
  // Dibuja el juego o el menú según el estado
  if (estadoDelJuego == true) {
    juego.dibujar(); // Dibuja el juego
  } else {
    menu.dibujar(); // Dibuja el menú
  }
}
