# TraceLite — Avance Parcial

Primera versión funcional de un sistema ligero de trazabilidad distribuida en **C** sobre **GNU/Linux**, sin utilizar SDKs oficiales de OpenTelemetry ni Grafana Tempo.

## Estructura del repositorio

```text
tracelite_avance/
├── .gitignore
├── Makefile
├── README.md
├── docs/
│   └── informe_avance.md
└── src/
    ├── trace_common.h
    ├── main.c
    ├── service_a.c
    ├── service_b.c
    └── collector.c
```

## Compilación y ejecución

```bash
make
./bin/tracelite
```

Para limpiar los archivos compilados:

```bash
make clean
```

## Checklist de avance

- [x] Creación de proceso principal y servicios hijos con `fork()` y `exec()` (`execl()`).
- [x] Registro de `Trace ID`, `Span ID`, `Parent Span ID`, servicio, operación, inicio, fin, duración en ms (`CLOCK_MONOTONIC`) y estado (`OK`/`ERROR`).
- [x] Propagación de contexto (`Trace ID` y `Span ID` padre) entre `service_a` y `service_b`.
- [x] Transmisión de spans hacia `collector` mediante `pipe()` anónimo.
- [x] Visualización en terminal agrupada por `Trace ID`.
- [x] Prueba de al menos 3 solicitudes completas incluyendo anomalía de latencia.
- [ ] Pendiente para entrega final: dashboard con actualización periódica y ampliación de características finales.
