#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "trace_common.h"

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Uso: %s <trace_id> <parent_span_id> <collector_write_fd> <anomaly_mode>\n", argv[0]);
        return 1;
    }

    const char *trace_id = argv[1];
    const char *parent_span_id = argv[2];
    int collector_fd = atoi(argv[3]);
    int anomaly_mode = atoi(argv[4]);

    Span span;
    memset(&span, 0, sizeof(Span));
    snprintf(span.trace_id, ID_LEN, "%s", trace_id);
    snprintf(span.span_id, ID_LEN, "span-b");
    snprintf(span.parent_span_id, ID_LEN, "%s", parent_span_id);
    snprintf(span.service, NAME_LEN, "service_b");
    snprintf(span.operation, NAME_LEN, "procesar_datos");

    span.start_ms = now_ms();

    if (anomaly_mode == 1) {
        // Anomalia de latencia elevada (120 ms)
        usleep(120000);
        snprintf(span.status, STATUS_LEN, "OK");
    } else if (anomaly_mode == 2) {
        // Anomalia de error en procesamiento
        usleep(15000);
        snprintf(span.status, STATUS_LEN, "ERROR");
    } else {
        // Ejecucion normal (~12 ms)
        usleep(12000);
        snprintf(span.status, STATUS_LEN, "OK");
    }

    span.end_ms = now_ms();
    span.duration_ms = span.end_ms - span.start_ms;

    if (write(collector_fd, &span, sizeof(Span)) != sizeof(Span)) {
        perror("Error escribiendo span de service_b");
        close(collector_fd);
        return 1;
    }

    close(collector_fd);
    return 0;
}
