#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "trace_common.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Uso: %s <trace_id> <collector_write_fd> <anomaly_mode>\n", argv[0]);
        return 1;
    }

    const char *trace_id = argv[1];
    int collector_fd = atoi(argv[2]);
    int anomaly_mode = atoi(argv[3]);

    Span span;
    memset(&span, 0, sizeof(Span));
    snprintf(span.trace_id, ID_LEN, "%s", trace_id);
    snprintf(span.span_id, ID_LEN, "span-a");
    span.parent_span_id[0] = '\0';
    snprintf(span.service, NAME_LEN, "service_a");
    snprintf(span.operation, NAME_LEN, "recibir_solicitud");
    snprintf(span.status, STATUS_LEN, "OK");

    span.start_ms = now_ms();
    usleep(8000); // 8 ms base
    span.end_ms = now_ms();
    span.duration_ms = span.end_ms - span.start_ms;

    if (write(collector_fd, &span, sizeof(Span)) != sizeof(Span)) {
        perror("Error escribiendo span de service_a");
        return 1;
    }

    pid_t pid_b = fork();
    if (pid_b < 0) {
        perror("Error en fork para service_b");
        return 1;
    }

    if (pid_b == 0) {
        char fd_str[16];
        char anom_str[16];
        snprintf(fd_str, sizeof(fd_str), "%d", collector_fd);
        snprintf(anom_str, sizeof(anom_str), "%d", anomaly_mode);

        execl("./bin/service_b", "service_b", trace_id, span.span_id, fd_str, anom_str, (char *)NULL);
        perror("Error en execl de service_b");
        _exit(1);
    }

    int status = 0;
    waitpid(pid_b, &status, 0);
    close(collector_fd);

    return 0;
}
