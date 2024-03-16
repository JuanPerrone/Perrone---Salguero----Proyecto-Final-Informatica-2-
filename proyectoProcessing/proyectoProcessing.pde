import processing.serial.*; // Librería para la comunicación serial.
import javax.swing.JOptionPane; // Librería para ventanas emergentes.
import java.io.FileReader; // Librería para leer archivos.
import java.io.FileWriter; // Librería para escribir archivos.
import java.io.BufferedReader; // Librería para leer archivos de texto de manera eficiente.
import java.io.BufferedWriter; // Librería para escribir archivos de texto de manera eficiente.
import java.io.IOException; // Librería para manejar excepciones de entrada/salida.

PImage imagen1, imagen2;

int diametroPelota = 40;
int anchoPatineta = 80;
int altoPatineta = 20;

int estadoMenu;
boolean confirmarPresionado;
int posicionPatineta;
int posicionPelotaX;
int posicionPelotaY;
int puntaje;
boolean introducirNombre;
int puntajeAnterior;
int puntajeDisplay;
boolean primeraIteracion;

int[] posicionPatinetaRec;
int[] posicionPelotaXRec;
int[] posicionPelotaYRec;
int cantidadRegistros;
int pasoReproduccion;
int tiempoUltimoPaso;

class Nodo {
  private
    String nombre;
  int puntaje;
  Nodo siguiente;

  public
    Nodo (String nombre, int puntaje) {
    this.nombre = nombre;
    this.puntaje = puntaje;
    this.siguiente = null;
  } // Fin del constructor Nodo.
} // Fin de la clase Nodo.

class ListaEnlazada {
  private
    Nodo head;

  public
    ListaEnlazada () {
    head = null;
  } // Fin del constructor ListaEnlazada.

  void insertar (String nombre, int puntaje) {
    Nodo nuevoNodo = new Nodo(nombre, puntaje);
    // Se verifica si la lista está vacía o si el puntaje del nuevo nodo es mayor que el puntaje del primer nodo.
    if (head == null || puntaje > head.puntaje) {
      nuevoNodo.siguiente = head;
      head = nuevoNodo;
    } else {  // Si el nuevo nodo no es el primero en la lista.
      Nodo actual = head;
      while (actual.siguiente != null && actual.siguiente.puntaje > puntaje) {
        actual = actual.siguiente;
      }
      nuevoNodo.siguiente = actual.siguiente;
      actual.siguiente = nuevoNodo;
    } // Fin de else.
  } // Fin del método insertar.

  void guardarEnArchivo() {
    String nombreArchivo = "registros.txt";
    BufferedWriter escritor = null; //Se declara un escritor de archivos.
    try {  // Se inicia un bloque try para manejar las operaciones que pueden lanzar una excepción.
      escritor = new BufferedWriter(new FileWriter(nombreArchivo));
      Nodo actual = head;
      while (actual != null) {
        escritor.write("Nombre: " + actual.nombre + ", Puntaje: " + actual.puntaje);
        escritor.newLine();
        actual = actual.siguiente;
      } // Fin de while.
    } //Fin de try.
    catch (IOException e) {  // Se captura una posible excepción de tipo IOException que puede ocurrir durante la escritura del archivo y se maneja imprimiendo la traza de la pila.
      e.printStackTrace();
    }
    finally {  // Se inicia un bloque finally para ejecutar código después del bloque try independientemente de si se produce una excepción o no.
      try {
        if (escritor != null) {
          escritor.close();
        } // Fin if.
      } // Fin de try.
      catch (IOException e) {  // Se captura una posible excepción de tipo IOException que puede ocurrir durante el cierre del escritor de archivos y se maneja imprimiendo la traza de la pila.
        e.printStackTrace();
      }
    } // Fin de finally.
  } // Fin del método guardarEnArchivo.

  void mostrarRegistro() {
    Nodo actual = head;
    int registrosX = width / 2; // Coordenada X desde la que se comienzan a mostrar los registros.
    int registrosY = 155; // Coordenada Y desde la que se comienzan a mostrar los registros.
    int cantidadMaxima = 12; // Cantidad de registros maximo que se muestran.
    int contador = 0;
    while (actual != null && contador < cantidadMaxima) {
      text(actual.nombre + ": " + actual.puntaje, registrosX, registrosY);
      registrosY += 30;
      actual = actual.siguiente;
      contador++;
    } // Fin de while.
  } // Fin del método mostrarRegistro.
} // Fin de la clase ListaEnlazada

class Records {
  public
    ListaEnlazada lista;

  Records() {
    lista = new ListaEnlazada();
    cargarDesdeArchivo();
  } // Fin del constructor

  void cargarDesdeArchivo() {
    String nombreArchivo = "registros.txt";
    BufferedReader lector = null; // Se declara un lector de archivos.
    try {  // Se inicia un bloque try para manejar las operaciones que pueden lanzar una excepción.
      lector = new BufferedReader(new FileReader(nombreArchivo));
      String linea;
      while ((linea = lector.readLine()) != null) {
        String[] partes = linea.split(", ");
        String nombre = partes[0].substring(8); // Eliminar "Nombre: " del inicio
        int puntaje = Integer.parseInt(partes[1].substring(9)); // Eliminar "Puntaje: " del inicio
        lista.insertar(nombre, puntaje);
      } // Fin de while
    } // Fin de try
    // Se captura una posible excepción de tipo IOException que puede ocurrir durante la lectura del archivo y se maneja imprimiendo la traza de la pila.
    catch (IOException e) {
      e.printStackTrace();
    }
    // Se inicia un bloque finally para ejecutar código después del bloque try independientemente de si se produce una excepción o no.
    finally {
      try {
        if (lector != null) {
          lector.close();
        } // Fin if
      } // Fin de try
      // Se captura una posible excepción de tipo IOException que puede ocurrir durante el cierre del lector de archivos y se maneja imprimiendo la traza de la pila.
      catch (IOException e) {
        e.printStackTrace();
      }
    } // Fin del finally
  } // Fin del método cargarDesdeArchivo

  void guardarRegistro(String nombre, int puntaje) {
    lista.insertar(nombre, puntaje);
    lista.guardarEnArchivo();
  } // Fin del método guardarRegistro
} // Fin de la clase Records

Serial miPuerto;
Records records;

void setup() {
  size(800, 600);
  String estePuerto = Serial.list()[1];
  miPuerto = new Serial(this, estePuerto, 9600); // Inicializa el objeto Serial
  records = new Records(); // Inicializa la instancia de la clase Records
  imagen1 = loadImage("imagenJuego1.jpg");
  imagen2 = loadImage("imagenJuego2.jpg");
} // Fin de funcion setup.

void draw() {
  background(255);
  if (miPuerto.available() > 0) {
    String info = miPuerto.readStringUntil('\n');
    if (info != null) {
      String[] partes = split(trim(info), ',');
      if (partes.length >= 7) {
        estadoMenu = int(partes[0]);
        confirmarPresionado = partes[1].equals("1");
        posicionPatineta = int(partes[2]);
        posicionPelotaX = int(partes[3]);
        posicionPelotaY = int(partes[4]);
        puntaje = int(partes[5]);
        introducirNombre = partes[6].equals("1");

        int tamañoHistorial = partes.length - 7;
        cantidadRegistros = tamañoHistorial / 3;
        posicionPatinetaRec = new int[cantidadRegistros];
        posicionPelotaXRec = new int[cantidadRegistros];
        posicionPelotaYRec = new int[cantidadRegistros];

        for (int i = 0; i < cantidadRegistros; i++) {
          int indice = 7 + i * 3;
          posicionPatinetaRec[i] = int(partes[indice]);
          posicionPelotaXRec[i] = int(partes[indice + 1]);
          posicionPelotaYRec[i] = int(partes[indice + 2]);
        } // Fin de for.
      } // Fin de if partes.length >= 7.
    } // Fin de if info != null.
  } // Fin de if miPuerto.available() > 0.

  if (estadoMenu == 0) {
    puntajeAnterior = 0;
    puntajeDisplay = 0;
    primeraIteracion = true;
    pasoReproduccion = 0;
    tiempoUltimoPaso = 0;
  } // estadoMenu == NINGUNO.
  if (estadoMenu == 1 && confirmarPresionado == true) { // estadoMenu == JUGAR
    dibujarJuego();
    if (introducirNombre == true) {
      // Solicita el nombre al usuario y guarda el registro.
      records.guardarRegistro(JOptionPane.showInputDialog("Ingrese su nombre:"), puntaje);
    }
  } else {
    dibujarMenu();
  }
  if (estadoMenu == 2 && confirmarPresionado == true) {
    dibujarRecords();
  } // estadoMenu == RECORDS.
  if (estadoMenu == 3 && confirmarPresionado == true) {
    dibujarJuego();
  } // estadoMenu == GRABACION.
} // Fin de funcion draw.

void dibujarMenu() {
  image(imagen1, 0, 0);
  // Título
  fill(255);
  textSize(55);
  textAlign(CENTER, CENTER);
  text("LA CANASTA", width / 2, height / 4);

  // Botón "Jugar".
  if (estadoMenu == 1) {
    strokeWeight(3);
    fill(0, 150, 0);
  } else {
    strokeWeight(1);
    fill(0, 255, 0);
  }
  rect(width / 2 - 220, height / 2, width / 4, 50);

  fill(0);
  textSize(24);
  text("Jugar", width / 2 - 120, height / 2 + 25);

  // Botón "Records".
  if (estadoMenu == 2) {
    strokeWeight(3);
    fill(0, 150, 0);
  } else {
    strokeWeight(1);
    fill(0, 255, 0);
  }
  rect(width / 2 + 20, height / 2, width / 4, 50);

  fill(0);
  text("Records", width / 2 + 120, height / 2 + 25);

  //Botón "Grabación".
  if (estadoMenu == 3) {
    strokeWeight(3);
    fill(0, 150, 0);
  } else {
    strokeWeight(1);
    fill(0, 255, 0);
  }
  rect(width / 2 - 100, height / 2 + 100, 200, 50);

  fill(0);
  text("Grabación", width / 2, height / 2 + 125);
} // Fin de funcion dibujarMenu.

void dibujarJuego() {
  image(imagen2, 0, 0);
  if (estadoMenu == 1) {
    // Dibuja la patineta.
    fill(255, 124, 0); // Naranja.
    rect(posicionPatineta, height - altoPatineta, anchoPatineta, altoPatineta);

    // Muestra el puntaje.
    fill(255);
    textSize(80);
    text(puntaje, width / 2, height / 2);

    // Dibuja la pelota.
    fill(255, 0, 0); // Rojo.
    ellipse(posicionPelotaX, posicionPelotaY, diametroPelota, diametroPelota);

    if (puntajeDisplay == 0 && primeraIteracion == true) {
      miPuerto.write(puntajeDisplay); // Solo se manda por el puerto el cero en la primera iteracion.
      primeraIteracion = false;
    }

    if (puntaje != puntajeAnterior) {
      puntajeDisplay++;
      miPuerto.write(puntajeDisplay);
    }
    puntajeAnterior = puntaje;
  } // Fin de if estadoMenu == 1 - (estadoMenu == JUGAR).

  if (estadoMenu == 3) {
    strokeWeight(1);
    if (cantidadRegistros == 0) {
      // Botón "Volver al Menú".
      fill(0, 255, 0);
      rect(width / 2 - 75, height / 2 - 20, 150, 40);
      fill(0);
      textSize(18);
      text("Volver al Menú", width / 2, height / 2);
    }

    // Dibuja la grabacion.
    if (pasoReproduccion < cantidadRegistros) {
      // Dibuja la patineta.
      fill(255, 124, 0); // Naranja.
      rect(posicionPatinetaRec[pasoReproduccion], height - altoPatineta, anchoPatineta, altoPatineta);

      // Dibuja la pelota.
      fill(255, 0, 0); // Rojo.
      ellipse(posicionPelotaXRec[pasoReproduccion], posicionPelotaYRec[pasoReproduccion], diametroPelota, diametroPelota);

      // Avanza al siguiente paso de reproducción si ha pasado el tiempo suficiente.
      if (millis() - tiempoUltimoPaso >= 30) {
        pasoReproduccion++;
        tiempoUltimoPaso = millis(); // Actualiza el tiempo del último paso.
      }
    } else {
      miPuerto.write(10); // Al alcanzar el ultimo registro de la grabacion, le "decimos" a arduino que deje de enviar el array.
      // Botón "Volver al Menú".
      fill(0, 255, 0);
      rect(width / 2 - 75, height / 2 - 20, 150, 40);
      fill(0);
      textSize(18);
      text("Volver al Menú", width / 2, height / 2);
    } // Fin de else.
  } // Fin de if estadoMenu == 3 - (estadoMenu == GRABACION).
} // Fin de la funcion dibujarJuego.

void dibujarRecords() {
  image(imagen1, 0, 0);
  // Título.
  fill(200); // Gris.
  rect(width / 2 - 250, height / 2 - 200, 500, 400);
  fill(0);
  textSize(24);
  text("RECORDS", width / 2, 115);  // 115 es la coordenada en Y para escribir RECORDS.
  records.lista.mostrarRegistro();

  // Botón "Volver al Menú".
  fill(0, 255, 0);
  rect(10, height - 50, 150, 40);
  fill(0);
  textSize(18);
  text("Volver al Menú", 85, height - 30);
} // Fin de la funcion dibujarRecords.
