import processing.serial.*;
Serial puertoSerie;

int posicionPatineta = 400;  // Posición inicial de la patineta
int anchoPatineta = 80;      // Ancho inicial de la patineta
int alturaPatineta = 20;     // Alto inicial de la patineta
int puntaje = 0;
int posicionPelotaX, posicionPelotaY;
boolean pelotaCapturada = false;
boolean finJuego = false;

void setup() {
  size(800, 600);
  textAlign(CENTER);
  textSize(40);
  puertoSerie = new Serial(this, "COM3", 9600);
  resetearJuego();
}

void draw() {
  background(255);
  
  // Dibujar pelota
  ellipse(posicionPelotaX, posicionPelotaY, 40, 40);
  
  // Dibujar patineta
  fill(0);
  rect(posicionPatineta - anchoPatineta / 2, height - alturaPatineta, anchoPatineta, alturaPatineta);
  
  // Mostrar puntaje
  fill(0);
  text("Puntaje: " + puntaje, width/2, 100);  // Texto centrado
  
  if (finJuego) {
    fill(255, 0, 0);
    text("Fin del juego\nPuntaje: " + puntaje + "\nPresiona 'R' para reiniciar", width/2, height/2);
  } else {
    actualizarJuego(); // Actualizar el juego en cada fotograma
  }
}

void resetearJuego() {
  pelotaCapturada = false;
  posicionPelotaX = int(random(40, width - 40));  // Posición aleatoria, teniendo en cuenta el tamaño
  posicionPelotaY = 0;
}

void dejarCaerPelota() {
  posicionPelotaY += 5; // Velocidad de caída de la pelota
  
  if (posicionPelotaY > height - alturaPatineta && 
      abs(posicionPelotaX - posicionPatineta) < anchoPatineta / 2 + 20) {  // Ajuste para colisión
    if (!pelotaCapturada) {
      puntaje++;
      pelotaCapturada = true;
      resetearJuego();
    }
  } else if (posicionPelotaY > height - alturaPatineta) {
    finJuego = true;
  }
}

void serialEvent(Serial puertoSerie) {
  String mensaje = puertoSerie.readStringUntil('\n');
  if (mensaje != null) {
    mensaje = trim(mensaje);
    posicionPatineta = Integer.parseInt(mensaje);
  }
}

void keyPressed() {
  if (finJuego && key == 'r') {
    finJuego = false;
    puntaje = 0;
    posicionPatineta = 400; // Posición inicial de la patineta
    resetearJuego();
  }
}

void actualizarJuego() {
  dejarCaerPelota();
}
