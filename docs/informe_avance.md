# Informe Técnico — Avance Parcial TraceLite

## 1. Descripción General y Alcance
TraceLite es un prototipo ligero de trazabilidad distribuida implementado en lenguaje C para GNU/Linux. En este avance parcial se demuestra el ciclo básico de generación de spans, propagación de contexto entre dos servicios (`service_a` y `service_b`), recolección mediante un `pipe()` anónimo en un proceso recolector (`collector`) y visualización agrupada por `Trace ID` en terminal, sin depender de SDKs de OpenTelemetry ni Grafana Tempo.

## 2. Arquitectura de Software
El sistema está compuesto por cuatro ejecutables:
- `tracelite` (`src/main.c`): proceso principal que crea el canal IPC (`pipe`), lanza `collector` y orquesta tres solicitudes secuenciales mediante `service_a`.
- `service_a` (`src/service_a.c`): genera el span raíz (`parent_span_id` vacío), envía su span al recolector y lanza `service_b` propagando el `trace_id` y su `span_id`.
- `service_b` (`src/service_b.c`): recibe `trace_id` y `parent_span_id`, mide el tiempo de procesamiento (incluyendo un escenario anómalo con mayor latencia) y envía su span al recolector.
- `collector` (`src/collector.c`): lee estructuras `Span` desde su entrada estándar redirigida al extremo de lectura del `pipe` y las imprime agrupadas por `trace_id`.

## 3. Modelo de Procesos
1. El proceso principal (`main`) invoca `pipe()` y realiza `fork()` + `execl()` para iniciar `./bin/collector`, redirigiendo el extremo de lectura al descriptor `STDIN_FILENO` mediante `dup2()`.
2. Para cada solicitud (`trace-1`, `trace-2`, `trace-3`), `main` realiza `fork()` + `execl()` para ejecutar `./bin/service_a` y espera su finalización con `waitpid()`.
3. `service_a` realiza `fork()` + `execl()` para ejecutar `./bin/service_b`, pasándole el `trace_id` y `span-a` como `parent_span_id`, y espera con `waitpid()`.
4. Al terminar las solicitudes, `main` cierra `pipefd[1]` para enviar `EOF` a `collector` y espera su cierre con `waitpid()`.

## 4. Mecanismo IPC y Formato de Mensajes
Se eligió un **`pipe()` anónimo** porque comunica procesos emparentados de forma directa, síncrona y sencilla de depurar en GNU/Linux sin requerir rutas de archivos ni sockets adicionales. Los mensajes enviados son estructuras binarias de tipo `Span` definidas en `src/trace_common.h`, que contienen `trace_id`, `span_id`, `parent_span_id`, `service`, `operation`, `start_ms`, `end_ms`, `duration_ms` y `status`.

## 5. Medición de Tiempos y Pruebas de Anomalía
Las marcas de tiempo se obtienen con `clock_gettime(CLOCK_MONOTONIC, &ts)` para evitar saltos del reloj del sistema y calcular la duración en milisegundos (`end_ms - start_ms`). En la ejecución base se procesan 3 trazas (`trace-1`, `trace-2` y `trace-3`), aplicando en `trace-3` una demora adicional (`usleep(120000)`) para evidenciar la anomalía de latencia en el span de `service_b`.

## 6. Evidencia de Ejecución y Checklist
*(Completa esta sección pegando la captura o salida real de `./bin/tracelite` ejecutada en tu terminal GNU/Linux).*

### Checklist de Avance
- [x] Arquitectura de procesos con `fork()` y `exec()`.
- [x] Spans con identificadores, tiempos (`CLOCK_MONOTONIC`) y estados.
- [x] Propagación de contexto entre servicios.
- [x] Comunicación IPC mediante `pipe()`.
- [x] Visualización agrupada por `Trace ID` en terminal y prueba de 3 solicitudes con anomalía.
- [ ] Funcionalidades pendientes para la entrega final (actualización periódica/dashboard y mejoras finales).
