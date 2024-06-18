# **SSOO2 TRABAJO SSOO2**
**Realizado por PEDRO JOSÉ MEDINA SÁNCHEZ y CARLOS FERNÁNDEZ-APARICIO MARGOTÓN**

## **EXPLICACIÓN DEL CÓDIGO**
En este apartado haremos un pequeño análisis sobre todos los metodos utilizados en el código

#### **main.cpp**

```
void user_running(int numThreads, std::string keyword, User user, mqd_t searcher_mq)
```

- Este método simula la actividad de un usuario en el sistema.
- Crea una cola de mensajes para el usuario y envía una solicitud de búsqueda al buscado
- Espera una respuesta del buscador y actualiza el saldo del usuario según la respuesta recibida.


```
int user_generator(unsigned int n_users)
```

- Genera usuarios aleatorios con características como balance, tipo, número de hilos y palabra clave.
- Inicia un thread para cada usuario utilizando user_running.
- Lee palabras clave de un archivo llamado dictionary.txt

```
int running_payment_process() 
```

- Crea y maneja el proceso de pago.
- Inicia un proceso hijo que maneja las solicitudes de pago y envía las confirmaciones de pago.
- Utiliza una cola de mensajes para recibir solicitudes de pago.

```
int running_search_process() 
```

- Crea y maneja el proceso de búsqueda.
- Inicia un proceso hijo que se encarga de recibir y procesar las solicitudes de búsqueda.
- Usa la clase Searcher para manejar las solicitudes de búsqueda

```
void handle_sigint(int sig)  
```

- Manejador de la señal SIGINT (interrupción).
- Cierra y elimina todas las colas de mensajes registradas.
- Se asegura de que todos los recursos se liberen adecuadamente al finalizar el programa.

```
int main()
```

- Manejador de señal para SIGINT.
- Inicia los procesos de búsqueda y pago.
- Entra en un bucle infinito para mantener el programa en ejecución.

#### **Search.cpp**
 ```
 Search::Search(searchRequest_t request)
 ```
 - Inicializa un objeto Search con una solicitud de búsqueda 
 - Carga los nombres de archivos desde paths.txt para su uso posterior en la búsqueda

 ```
 void Search::FreeUserSearch(int numThreads)
 ```
 - Inicia una búsqueda para usuarios gratuitos utilizando múltiples hilos
 - Crea un número especificado de hilos, cada uno para buscar en un archivo diferente de la lista files
 - Llama al método FreeSearchInFile para cada archivo y espera a que todos los hilos terminen

 ```
 void Search::FreeSearchInFile(const std::string& file, searchRequest_t request) 
 ```
 - Realiza la búsqueda de la palabra clave específica en un archivo dado para usuarios gratuitos.
 - Lee cada línea del archivo, busca la palabra clave (ignorando la primera palabra de cada línea), y si encuentra una coincidencia, registra la ocurrencia
 - Utiliza un mutex para proteger la sección crítica (incremento de totalOccurrences)

 ```
 int Search::getTotalOccurrences() 
 ```
 - Devuelve el número total de ocurrencias encontradas hasta el momento
 - Utiliza std::atomic<int> para asegurar la lectura atómica del valor de totalOccurrences, evitando problemas de concurrencia
 
 ```
 void Search::unlimitedUserSearch(int numThreads)
 ```
 - Inicia una búsqueda ilimitada para usuarios premium sin restricciones en el número de ocurrencias
 - Crea hilos para buscar en cada archivo de files, espera a que todos los hilos terminen y luego realiza una búsqueda ilimitada adicional en cada archivo

 ```
 void Search::UnlimitedSearchInFile(const std::string& file, const std::string& keyword) 
 ```
 - Realiza la búsqueda de la palabra clave en un archivo específico sin restricciones en el número de ocurrencias
 - Similar a FreeSearchInFile, busca en cada línea del archivo y registra las ocurrencias encontradas, sin aplicar un límite de búsqueda

 ```
 void Search::PremiumSearchInFile(const std::string& file, mqd_t paymentQueue, mqd_t paymentQueueResponse) 
 ```
 - Este método está diseñado para ser invocado por múltiples hilos simultáneamente 
 - Garantiza que cada hilo realice su trabajo de búsqueda de manera independiente 
 


#### **Searcher.cpp**
```
Searcher::Searcher() 
```
- Inicializa los pesos y contadores para la gestión de solicitudes premium y gratuitas.

```
Searcher* Searcher::getInstance()
```
- La clase Searcher utiliza el patrón Singleton, garantizando que solo exista una instancia de Searcher en todo el programa.

``` 
void Searcher::receiveRequests()
```
-

#### **SistemaDePago.cpp**
#### **User.cpp**

```
User::User(int id, int balance, UserType type)
```
-Es un constructor parametrizado que inicializa un objeto User con un identificador (id), un saldo (balance) y un tipo de usuario (type).

**Métodos Getters y Setters**

- getId: Retorna el identificador del usuario.
- setId: Establece el identificador del usuario.
- getBalance: Retorna el saldo del usuario.
- setBalance: Establece el saldo del usuario.

``` 
void User::decreaseBalance()
```
- Disminuye el saldo del usuario en una unidad.

#### **utils.cpp**





## **TAREAS**

Hemos creado una tabla para que pueda ver el estado de las tareas que nos mando después de la corrección.

| Tarea            | Estado  |
|:-----------------|:-------:|
| Reducción del vídeo explicativo | ✅       |
| Solucionar interbloqueos | ✅       |
| Mejora de la coherencia en el código  | ✅         |
| README.md       | ✅       |
| Cabeceras dentro de los archivos | ✅        |
| Creación de clientes mediante hilos | ✅       |
| Control y propagación de excepciones con try-catch| ✅      |
| Número de instancias para buscar es configurable | ✅       |
| Solucion de esperas activas sin ningún sentido | ✅       |
| Solucionar sistema de pago | ✅      |
| Guardado de resultados de búsqueda en archivos para cada cliente | ✅       |
| Medición tiempo de búsqueda y se hace análisis de rendimiento | ✅     |
| Respetar el formato de salida | ✅       |
| Sincronización entre las entidades cliente-buscador-sistema de pago | ✅      |


