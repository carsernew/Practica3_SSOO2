# **SSOO2 TRABAJO SSOO2**
**Realizado por PEDRO JOSÉ MEDINA SÁNCHEZ y CARLOS FERNÁNDEZ-APARICIO MARGOTÓN**

## **METODOS**
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

#### **Search.cpp**
#### **Searcher.cpp**
#### **SistemaDePago.cpp**
#### **User.cpp**
#### **utils.cpp**





## **TAREAS**

Hemos creado una tabla para que pueda ver el estado de las tareas que nos mando después de la corrección.

| Tarea            | Estado  |
|:-----------------|:-------:|
| Reducción del vídeo explicativo | ✅       |
| Solucionar interbloqueos | ✅       |
| Mejora de la coherencia en el código  | ❌       |
| README.md       | ✅       |
| Cabeceras dentro de los archivos | ❌      |
| Creación de clientes mediante hilos | ✅       |
| Control y propagación de excepciones con try-catch| ✅      |
| Número de instancias para buscar es configurable | ✅       |
| Solucion de esperas activas sin ningún sentido | ✅       |
| Solucionar sistema de pago | ✅      |
| Guardado de resultados de búsqueda en archivos para cada cliente | ✅       |
| Medición tiempo de búsqueda y se hace análisis de rendimiento | ✅     |
| Respetar el formato de salida | ✅       |
| Sincronización entre las entidades cliente-buscador-sistema de pago | ✅      |


