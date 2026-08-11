/*
 * Client
 *
 * Hendra Manudinata (5027251051)
 */

#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "protocol.h"

/*
 * Admin credentials
 */
#define ADMIN_NAME "The Knights"
#define ADMIN_PASSWORD "protocol7"

/*
 * Store user info
 */
struct {
  char name[50];
  int is_admin;
} user;

/*
 * Socket file descriptor
 */
int sockfd;

/*
 * Interrupt handler to gracefully disconnect from the server.
 */
void handle_sigint(int sig) {
  char exit_msg[] = "/exit\n";
  send(sockfd, exit_msg, strlen(exit_msg), 0);
  close(sockfd);
  printf("\n[System] Disconnecting from The Wired...\n");
  exit(0);
}

/*
 * Thread function to receive messages from the server asynchronously.
 */
void *receive_handler(void *socket_desc) {
  int sock = *(int *)socket_desc;
  char buffer[1024];

  while (1) {
    memset(buffer, 0, 1024);
    int receive = recv(sock, buffer, 1024, 0);
    if (receive > 0) {
      printf("%s", buffer);
      fflush(stdout);

      // Reprint prompt after receiving a message
      // this is needed because the server's messages may interrupt the
      // user's input
      printf(user.is_admin ? "Admin >> " : "> ");
      fflush(stdout);
    }

    else if (receive == 0) {
      printf("\n[System] Koneksi ke The Wired terputus.\n");
      exit(0);
    }
  }
}

void input_user(char *name, int buffer_size, int sanitize_newline) {
  char input[buffer_size];
  fgets(input, buffer_size, stdin);

  if (sanitize_newline)
    input[strcspn(input, "\n")] = 0; // hapus newline

  strncpy(name, input, buffer_size);
}

int main() {
  /*
   * Handle SIGINT (Ctrl+C)
   */
  signal(SIGINT, handle_sigint);

  /*
   * Authenticate user and determine if they are admin or regular client.
   */

  printf("Enter your name: ");
  input_user(user.name, sizeof(user.name), 1);

  if (strcmp(user.name, ADMIN_NAME) == 0) {
    char password[50];
    printf("Enter Password: ");
    input_user(password, sizeof(password), 1);

    if (strcmp(password, ADMIN_PASSWORD) != 0) {
      printf("[System] Authentication Failed.\n");
      return 0;
    }
  }

  /*
   * Initialize socket and connect to the server
   */

  struct sockaddr_in server_addr;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(LISTEN_HOSTNAME);
  server_addr.sin_port = htons(LISTEN_PORT);

  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("Connection failed");
    exit(EXIT_FAILURE);
  }

  /*
   * Perform authentication to the server.
   */

  send(sockfd, user.name, strlen(user.name), 0);
  char response[10];
  memset(response, 0, sizeof(response));
  recv(sockfd, response, 10, 0);

  if (strcmp(response, "TAKEN") == 0) {
    printf("[System] The identity '%s' is already synchronized in The Wired.\n",
           user.name);
    close(sockfd);
    return 0; // Exit cleanly instead of reusing a dead socket
  } else {
    user.is_admin = (strcmp(user.name, ADMIN_NAME) == 0);
    if (user.is_admin) {
      printf("--- Welcome to The Wired, %s (Admin) ---\n", user.name);
      printf("=== THE KNIGHTS CONSOLE ===\n");
      printf("1. /users (Check Active Entities)\n");
      printf("2. /uptime (Check Server Uptime)\n");
      printf("3. /shutdown (Emergency Shutdown)\n");
      printf("4. /exit\n");
    } else {
      printf("--- Welcome to The Wired, %s ---\n", user.name);
    }
  }

  /*
   * Create a thread to receive messages from the server. This makes message
   * reception asynchronous.
   */
  pthread_t recv_thread;
  if (pthread_create(&recv_thread, NULL, receive_handler, (void *)&sockfd) <
      0) {
    perror("Gagal membuat receiver thread!");
    return 1;
  }

  /*
   * Handle user input and send commands to the server.
   */
  char buffer[1024];
  while (1) {
    printf(user.is_admin ? "Admin >> " : "> ");

    input_user(buffer, 1024, 0);

    if (strncmp(buffer, "/exit", 5) == 0) {
      handle_sigint(SIGINT);
    }

    send(sockfd, buffer, strlen(buffer), 0);
  }

  return 0;
}
