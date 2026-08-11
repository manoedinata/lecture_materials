/*
 * Server
 *
 * Hendra Manudinata (5027251051)
 */

#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <time.h>
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
struct Client {
  int fd;
  char name[50];
  int is_admin;
};

/*
 * Handle 20 clients simultaneously.
 */
struct Client clients[20];
const int max_clients = sizeof(clients) / sizeof(clients[0]);

time_t start_time;
int server_fd;

/*
 * Write log entries
 */
void write_log(const char *entity, const char *action) {
  FILE *log_file = fopen("history.log", "a");
  if (log_file == NULL)
    return;

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  char time_str[30];
  strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);

  fprintf(log_file, "[%s] [%s] [%s]\n", time_str, entity, action);
  fclose(log_file);
}

/*
 * Handle SIGINT (Ctrl+C) to gracefully disconnect from the server.
 */
void handle_sigint(int sig) {
  write_log("System", "EMERGENCY SHUTDOWN INITIATED");
  for (int i = 0; i < max_clients; i++) {
    if (clients[i].fd == 0)
      continue;
    send(clients[i].fd, "\n[System] Server is shutting down...\n", 37, 0);
    close(clients[i].fd);
  }
  close(server_fd);
  printf("\n[System] Server Offline.\n");
  exit(0);
}

/*
 * Broadcast a message to all connected clients.
 */
void broadcast_message(const char *message, int sender_fd) {
  for (int i = 0; i < max_clients; i++) {
    if (clients[i].fd != 0 && clients[i].fd != sender_fd) {
      send(clients[i].fd, message, strlen(message), 0);
    }
  }
}

/*
 * Sanitize input by removing newline characters.
 */
void sanitize_input(char *input) {
  input[strcspn(input, "\n")] = 0; // Hapus newline
}

/*
 * ADMIN COMMANDS
 */
void handle_admin_command(const char *buffer, int sd) {
  if (strncmp(buffer, "/users", 6) == 0) {
    write_log("Admin", "RPC_GET_USERS");
    int count = 0;
    for (int j = 0; j < max_clients; j++)
      if (clients[j].fd > 0 && !clients[j].is_admin)
        count++;
    char reply[50];
    sprintf(reply, "[System] Active NAVI: %d\n", count);
    send(sd, reply, strlen(reply), 0);
  } else if (strncmp(buffer, "/uptime", 7) == 0) {
    write_log("Admin", "RPC_GET_UPTIME");
    char reply[50];
    sprintf(reply, "[System] Uptime: %ld seconds\n", time(NULL) - start_time);
    send(sd, reply, strlen(reply), 0);
  } else if (strncmp(buffer, "/shutdown", 9) == 0) {
    write_log("Admin", "RPC_SHUTDOWN");
    handle_sigint(SIGINT);
  }
}

/*
 * Handle regular messages from clients and broadcast to others.
 */
void handle_regular(struct Client *client, int max_clients, const char *buffer,
                    int sd) {
  char format_msg[1024 + 100];
  sprintf(format_msg, "[%s]: %s\n", client->name, buffer);
  broadcast_message(format_msg, sd);

  char log_action[1024 + 100];
  sprintf(log_action, "[%s]: %s", client->name, buffer);
  write_log("User", log_action);
}

int main() {
  /*
   * Handle SIGINT (Ctrl+C)
   */
  signal(SIGINT, handle_sigint);

  start_time = time(NULL);

  /*
   * Initialize clients
   */
  for (int i = 0; i < max_clients; i++)
    clients[i].fd = 0;

  /*
   * Initialize socket connection
   */
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[1024];

  /*
   * Create socket and bind to address
   */
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket failed");
    exit(EXIT_FAILURE);
  }
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  /*
   * Bind and listen on the specified hostname and port.
   */
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(LISTEN_HOSTNAME);
  address.sin_port = htons(LISTEN_PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 10) < 0) {
    perror("Listen failed");
    exit(EXIT_FAILURE);
  }

  printf("The Wired Server berjalan di %s:%d\n", LISTEN_HOSTNAME, LISTEN_PORT);
  write_log("System", "SERVER ONLINE");

  /*
   * Handle incoming connections and messages from clients
   */

  fd_set readfds;
  int max_sd, sd, activity, new_socket;

  while (1) {
    FD_ZERO(&readfds);
    FD_SET(server_fd, &readfds);
    max_sd = server_fd;

    for (int i = 0; i < max_clients; i++) {
      sd = clients[i].fd;
      if (sd > 0)
        FD_SET(sd, &readfds);
      if (sd > max_sd)
        max_sd = sd;
    }

    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

    /*
     * New incoming connection
     */

    if (FD_ISSET(server_fd, &readfds)) {

      new_socket =
          accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
      if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
      }

      /*
       * Retrieve the client's name for authentication
       */
      memset(buffer, 0, 1024);
      recv(new_socket, buffer, 1024, 0);
      sanitize_input(buffer);

      /*
       * Is Admin?
       */
      int is_admin = 0;
      if (strcmp(buffer, ADMIN_NAME) == 0) {
        is_admin = 1;
      }

      /*
       * No, just a regular client.
       */
      else {

        /*
         * Check existing names to prevent duplicates.
         */
        int name_exists = 0;
        for (int i = 0; i < max_clients; i++) {
          if (clients[i].fd != 0 && strcmp(clients[i].name, buffer) == 0) {
            name_exists = 1;
            break;
          }
        }
        if (name_exists) {
          send(new_socket, "TAKEN", 5, 0);
          close(new_socket);
          continue;
        }
      }

      send(new_socket, "OK", 2, 0);

      /*
       * Save client information in the clients array for future reference.
       */
      for (int i = 0; i < max_clients; i++) {
        if (clients[i].fd == 0) {
          clients[i].fd = new_socket;
          strcpy(clients[i].name, buffer);
          clients[i].is_admin = is_admin;
          break;
        }
      }

      char log_msg[100];
      sprintf(log_msg, "User '%s' connected", buffer);
      write_log("System", log_msg);
    }

    /*
     * Handle incoming messages from clients
     */
    for (int i = 0; i < max_clients; i++) {
      sd = clients[i].fd;
      if (!FD_ISSET(sd, &readfds))
        continue;

      memset(buffer, 0, 1024);
      int valread = recv(sd, buffer, 1024, 0);

      /*
       * Client disconnection
       */
      if (valread == 0 || strncmp(buffer, "/exit", 5) == 0) {
        char log_msg[100];
        sprintf(log_msg, "User '%s' disconnected", clients[i].name);
        write_log("System", log_msg);
        close(sd);
        clients[i].fd = 0;
      }

      else {
        sanitize_input(buffer);

        /*
         * Hello, Admin!
         */
        if (clients[i].is_admin && buffer[0] == '/') {
          handle_admin_command(buffer, sd);
        }

        /*
         * Aku admin kau member awowkowwoko
         */
        else if (!clients[i].is_admin) {
          handle_regular(&clients[i], max_clients, buffer, sd);
        }
      }
    }
  }

  return 0;
}
