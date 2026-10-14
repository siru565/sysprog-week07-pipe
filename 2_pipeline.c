#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char **cmd1 = &argv[1];
    char **cmd2 = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], ":") == 0) {
            argv[i] = NULL;        // 여기서 배열을 자른다
            cmd2 = &argv[i + 1];
            break;
        }
    }
    if (cmd2 == NULL || cmd1[0] == NULL || cmd2[0] == NULL) {
        fprintf(stderr, "사용법: %s 명령1 ... : 명령2 ...\n", argv[0]);
        return 1;
    }

    int fd[2];   /* fd[0] = 읽기 끝, fd[1] = 쓰기 끝 */
    if (pipe(fd) == -1) { perror("pipe"); return 1; }

    /* 첫 번째 자식: 명령1 을 실행하고 출력을 파이프로 보낸다 */
    if (fork() == 0) {   /* 반환값 0 이면 자식 쪽 코드 경로 */
        close(fd[0]);                 /* 읽기 끝은 쓰지 않는다 */
        dup2(fd[1], STDOUT_FILENO);   /* 표준출력을 파이프 쓰기 끝으로 교체 */
        close(fd[1]);                 /* 원본 디스크립터는 이제 필요 없다 */
        execvp(cmd1[0], cmd1);
        perror("execvp cmd1");        /* exec 가 실패했을 때만 여기 도달 */
        _exit(127);                 /* _exit: 부모의 버퍼·자원을 다시 정리하지 않고 즉시 종료 */
    }

    /* 두 번째 자식: 명령2 를 실행하고 입력을 파이프에서 받는다 */
    if (fork() == 0) {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);    /* 표준입력을 파이프 읽기 끝으로 교체 */
        close(fd[0]);
        execvp(cmd2[0], cmd2);
        perror("execvp cmd2");
        _exit(127);
    }

    /* 부모는 양쪽 다 닫아야 한다. 안 닫으면 명령2 가 EOF 를 못 봐서 끝나지 않는다. */
    close(fd[0]);
    close(fd[1]);

    wait(NULL);   /* 자식 두 개를 각각 기다려 좀비로 남지 않게 한다 */
    wait(NULL);
    return 0;
}
