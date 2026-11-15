#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("Error creando pipe");
        return 1;
    }

    pid_t pid_collector = fork();
    if (pid_collector < 0) {
        perror("Error en fork para collector");
        return 1;
    }

    if (pid_collector == 0) {
        // Proceso recolector: lee desde pipefd[0] conectado a STDIN
        close(pipefd[1]);
        if (dup2(pipefd[0], STDIN_FILENO) < 0) {
            perror("Error en dup2");
            _exit(1);
        }
        close(pipefd[0]);

        execl("./bin/collector", "collector", (char *)NULL);
        perror("Error en execl de collector");
        _exit(1);
    }

    // Proceso principal: no lee del pipe
    close(pipefd[0]);

    // 3 solicitudes completas: la 3ra incluye anomalia de latencia (modo 1)
    const char *traces[3] = {"trace-1", "trace-2", "trace-3"};
    int anomalies[3] = {0, 0, 1};

    for (int i = 0; i < 3; i++) {
        pid_t pid_a = fork();
        if (pid_a < 0) {
            perror("Error en fork para service_a");
            return 1;
        }

        if (pid_a == 0) {
            char fd_str[16];
            char anom_str[16];
            snprintf(fd_str, sizeof(fd_str), "%d", pipefd[1]);
            snprintf(anom_str, sizeof(anom_str), "%d", anomalies[i]);

            execl("./bin/service_a", "service_a", traces[i], fd_str, anom_str, (char *)NULL);
            perror("Error en execl de service_a");
            _exit(1);
        }

        int status_a = 0;
        waitpid(pid_a, &status_a, 0);
    }

    // Cerrar extremo de escritura para que el collector reciba EOF y muestre resultados
    close(pipefd[1]);

    int status_c = 0;
    waitpid(pid_collector, &status_c, 0);

    return 0;
}
