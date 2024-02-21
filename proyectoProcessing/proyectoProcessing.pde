import processing.serial.*; // Librería para la comunicación serial
import javax.swing.JOptionPane; // Librería para ventanas emergentes
import java.io.FileReader; // Librería para leer archivos
import java.io.FileWriter; // Librería para escribir archivos
import java.io.BufferedReader; // Librería para leer archivos de texto de manera eficiente
import java.io.BufferedWriter; // Librería para escribir archivos de texto de manera eficiente
import java.io.IOException; // Librería para manejar excepciones de entrada/salida

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
boolean introducirNombre;
int puntajeAnterior;
int puntajeDisplay;
boolean bandera;


class Nodo {
  private
    String nombre;
  int puntaje;
  Nodo siguiente; //Siguiente de tipo Nodo, que representa el siguiente nodo en la lista enlazada.

  public
    Nodo(String nombre, int puntaje) {
    this.nombre = nombre; //Se asigna el nombre y puntaje pasados como parámetro al atributo nombre y puntaje del nodo actual.
    this.puntaje = puntaje;
    this.siguiente = null; //Se inicializa el atributo siguiente como null, indicando que este nodo no tiene un nodo siguiente inicialmente.
  }//Constructor
}

class ListaEnlazada {
  private
    Nodo head;

  public
    ListaEnlazada() {
    head = null;
  }//Constructor

  void insertar(String nombre, int puntaje) {
    Nodo nuevoNodo = new Nodo(nombre, puntaje);  
    if (head == null || puntaje > head.puntaje) { //Se verifica si la lista está vacía o si el puntaje del nuevo nodo es mayor que el puntaje del primer nodo.
      nuevoNodo.siguiente = head;
      head = nuevoNodo;
    } else { //Si el nuevo nodo no es el primero en la lista.
      Nodo actual = head; 
      while (actual.siguiente != null && actual.siguiente.puntaje > puntaje) { 
        actual = actual.siguiente;
      }
      nuevoNodo.siguiente = actual.siguiente; 
      actual.siguiente = nuevoNodo;
    }
  } //Se define el método insertar que agrega un nuevo nodo a la lista enlazada.

  void mostrar() {
    Nodo actual = head;
    while (actual != null) {
      println("Nombre: " + actual.nombre + ", Puntaje: " + actual.puntaje);
      actual = actual.siguiente;
    }
  }

  void guardarEnArchivo() {
    String nombreArchivo = "registros.txt";
    BufferedWriter escritor = null; //Se declara un escritor de archivos.
    try {  //Se inicia un bloque try para manejar las operaciones que pueden lanzar una excepción.
      escritor = new BufferedWriter(new FileWriter(nombreArchivo)); //Se crea un nuevo escritor de archivos para el archivo.
      Nodo actual = head; //Se inicializa un nodo actual para recorrer la lista, comenzando desde head.
      while (actual != null) {
        escritor.write("Nombre: " + actual.nombre + ", Puntaje: " + actual.puntaje);
        escritor.newLine(); //Se agrega una nueva línea en el archivo.
        actual = actual.siguiente;
      }
    }
    catch (IOException e) { //Se captura una posible excepción de tipo IOException que puede ocurrir durante la escritura del archivo y se maneja imprimiendo la traza de la pila.
      e.printStackTrace();
    }
    finally { //Se inicia un bloque finally para ejecutar código después del bloque try independientemente de si se produce una excepción o no.
      try {
        if (escritor != null) {
          escritor.close(); //Se cierra el escritor de archivos si no es nulo.
        }
      }
      catch (IOException e) { //Se captura una posible excepción de tipo IOException que puede ocurrir durante el cierre del escritor de archivos y se maneja imprimiendo la traza de la pila.
        e.printStackTrace();
      }
    }
  } //Guarda los datos de la lista en un archivo.

  void mostrarEnRectangulo(int x, int y) {
    Nodo actual = head; // Nodo actual para recorrer la lista
    while (actual != null) {
      text(actual.nombre + ": " + actual.puntaje, x, y);
      y += 30;
      actual = actual.siguiente;
    }
  }
}

class Records {
  ListaEnlazada lista;

  Records() {
    lista = new ListaEnlazada();
    cargarDesdeArchivo();
  }//Constructor

  void cargarDesdeArchivo() {
    String nombreArchivo = "registros.txt";
    BufferedReader lector = null; // Se declara un lector de archivos.
    try { //Se inicia un bloque try para manejar las operaciones que pueden lanzar una excepción.
      lector = new BufferedReader(new FileReader(nombreArchivo)); //Se crea un nuevo lector de archivos para el archivo especificado.
      String linea; //Se declara una variable para almacenar cada línea leída del archivo.
      while ((linea = lector.readLine()) != null) { 
        String[] partes = linea.split(", ");
        String nombre = partes[0].substring(8);
        int puntaje = Integer.parseInt(partes[1].substring(9));
        lista.insertar(nombre, puntaje);
      }
    }
    catch (IOException e) { //Se captura una posible excepción de tipo IOException que puede ocurrir durante la lectura del archivo y se maneja imprimiendo la traza de la pila.
      e.printStackTrace();
    }
    finally { //Se inicia un bloque finally para ejecutar código después del bloque try independientemente de si se produce una excepción o no.
      try {
        if (lector != null) {
          lector.close(); //Se cierra el lector de archivos si no es nulo.
        }
      }
      catch (IOException e) { //Se captura una posible excepción de tipo IOException que puede ocurrir durante el cierre del lector de archivos y se maneja imprimiendo la traza de la pila.
        e.printStackTrace();
      }
    }
  }

  void guardarRegistro(String nombre, int puntaje) {
    lista.insertar(nombre, puntaje);
    lista.guardarEnArchivo();
  }

  void mostrarRegistros() {
    lista.mostrar();
  }
}

Records records;

void setup() {
  size(800, 600);
  String estePuerto = Serial.list()[1];
  miPuerto = new Serial(this, estePuerto, 9600); // Inicializa el objeto Serial
  records = new Records(); // Inicializa la instancia de la clase Records
}

void draw() {
  background(255);
  if (miPuerto.available() > 0) {
    String info = miPuerto.readStringUntil('\n'); //Lee una línea del puerto serial
    if (info != null) {
      String[] partes = split(trim(info), ',');
      if (partes.length == 8) {
        estadoMenu = int(partes[0]);
        confirmarPresionado = partes[1].equals("1");
        estadoDelJuego = partes[2].equals("1");
        posicionPatineta = int(partes[3]);
        posicionPelotaX = int(partes[4]);
        posicionPelotaY = int(partes[5]);
        puntaje = int(partes[6]);
        introducirNombre = partes[7].equals("1");
      }
    }
  } //Lee los datos del puerto serial si están disponibles

  if (estadoDelJuego == true) {
    dibujarJuego();
    if (introducirNombre == true) {
      records.guardarRegistro(JOptionPane.showInputDialog("Ingrese su nombre:"), puntaje); // Solicita el nombre al usuario y guarda el registro
      introducirNombre = false;
    }
  } else {
    puntajeAnterior = 0;
    puntajeDisplay = 0;
    bandera = true;
    dibujarMenu();
  }
  if (estadoMenu == 2 && confirmarPresionado == true) {
    dibujarRecords();
  }
}

void dibujarMenu() {
  // Título
  fill(0);
  textSize(40);
  textAlign(CENTER, CENTER);
  text("LA CANASTA", width / 2, height / 4);

  // Botón "Jugar"
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
  textAlign(CENTER, CENTER);
  text("Jugar", width / 2 - 120, height / 2 + 25);

  // Botón "Records"
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
}

void dibujarJuego() {
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

  if (puntajeDisplay == 0 && bandera == true) {
    miPuerto.write(puntajeDisplay);
    bandera = false;
  }
  
  if (puntaje != puntajeAnterior) {
    puntajeDisplay++;
    miPuerto.write(puntajeDisplay);
  }
  puntajeAnterior = puntaje;
}

void dibujarRecords() {
  fill(200);
  rect(100, 100, 600, 400);
  fill(0);
  textSize(24);
  textAlign(LEFT);
  // Título
  text("Records", 110, 120);
  int y = 160; // Posición inicial del primer registro
  records.lista.mostrarEnRectangulo(110, y); // Mostrar los registros en el rectángulo a partir de la posición y

  // Botón "Volver al Menú"
  fill(200);
  rect(10, height - 50, 150, 40);
  fill(0);
  textSize(16);
  textAlign(CENTER, CENTER);
  text("Volver al Menú", 85, height - 30);
}
