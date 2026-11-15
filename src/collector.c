#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "trace_common.h"

#define MAX_SPANS 256

int main(void) {
    Span spans[MAX_SPANS];
    int count = 0;

    while (count < MAX_SPANS) {
        Span s;
        ssize_t n = read(STDIN_FILENO, &s, sizeof(Span));
        if (n <= 0) {
            break;
        }
        if ((size_t)n == sizeof(Span)) {
            spans[count++] = s;
        }
    }

    printf("\n=== RECOLECTOR TRACELITE (SPANS AGRUPADOS POR TRACE ID) ===\n\n");

    int printed[MAX_SPANS] = {0};

    for (int i = 0; i < count; i++) {
        if (printed[i]) {
            continue;
        }

        const char *current_trace = spans[i].trace_id;
        printf("Trace ID: %s\n", current_trace);
        printf("--------------------------------------------------------------------------------\n");

        for (int j = i; j < count; j++) {
            if (strcmp(spans[j].trace_id, current_trace) == 0) {
                printed[j] = 1;
                printf("  Span ID: %-10s | Parent: %-10s | Servicio: %-10s | Op: %-14s | Estado: %-5s | Duracion: %.2f ms\n",
                       spans[j].span_id,
                       spans[j].parent_span_id[0] ? spans[j].parent_span_id : "(raiz)",
                       spans[j].service,
                       spans[j].operation,
                       spans[j].status,
                       spans[j].duration_ms);
            }
        }
        printf("\n");
    }

    return 0;
}
