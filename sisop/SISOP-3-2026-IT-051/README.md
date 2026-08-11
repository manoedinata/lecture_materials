# SISOP-3-2026-IT-051

<details>
<summary>Daftar Isi</summary>

- [SISOP-3-2026-IT-051](#sisop-3-2026-it-051)
  - [Soal 1: The Wired — Client-Server Chat System](#soal-1-the-wired--client-server-chat-system)
    - [Deskripsi Soal](#deskripsi-soal)
    - [Menjalankan Program](#menjalankan-program)
    - [Penjelasan Soal](#penjelasan-soal)
      - [1. Protokol dan Konfigurasi (`protocol.h`)](#1-protokol-dan-konfigurasi-protocolh)
      - [2. Server: I/O Multiplexing dengan `select()`](#2-server-io-multiplexing-dengan-select)
      - [3. Autentikasi Klien](#3-autentikasi-klien)
      - [4. Broadcast Pesan](#4-broadcast-pesan)
      - [5. Admin Commands](#5-admin-commands)
      - [6. Client: Thread Penerima Pesan Asinkron](#6-client-thread-penerima-pesan-asinkron)
      - [7. Logging Aktivitas](#7-logging-aktivitas)
      - [8. Handler SIGINT](#8-handler-sigint)
    - [Kendala](#kendala)
    - [Revisi](#revisi)

</details>

## Soal 1: The Wired — Client-Server Chat System

### Deskripsi Soal

Program ini mengimplementasikan sistem chat client-server menggunakan socket TCP di bahasa C. Server mampu menangani hingga **20 klien secara simultan** menggunakan I/O multiplexing dengan `select()`. Fitur utama yang diimplementasikan:

1. Server multi-klien dengan `select()` untuk memantau koneksi masuk dan pesan dari klien
2. Autentikasi nama pengguna — mencegah duplikasi nama aktif
3. Role admin dengan perintah khusus (`/users`, `/uptime`, `/shutdown`)
4. Broadcast pesan dari satu klien ke semua klien lainnya
5. Client menggunakan `pthread` untuk menerima pesan secara asinkron
6. Logging semua aktivitas ke `history.log`
7. Graceful shutdown dengan handler `SIGINT`

### Menjalankan Program

```bash
$ cd soal_1
$ gcc server.c -o server
$ gcc navi.c -o navi -lpthread

# Jalankan server terlebih dahulu
$ ./server

# Kemudian jalankan client (di terminal terpisah)
$ ./navi
```

Jika sukses:

- Server berjalan di `127.0.0.1:8080`
- File `history.log` mencatat semua aktivitas
- Klien dapat mengirim pesan dan admin dapat menjalankan perintah khusus

### Penjelasan Soal

#### 1. Protokol dan Konfigurasi (`protocol.h`)

File header bersama yang mendefinisikan konfigurasi koneksi:

```c
#define LISTEN_HOSTNAME "127.0.0.1"
#define LISTEN_PORT 8080
```

Baik server maupun client meng-include file ini agar menggunakan alamat dan port yang sama.

#### 2. Server: I/O Multiplexing dengan `select()`

Server menggunakan `select()` untuk memantau banyak file descriptor secara bersamaan tanpa perlu thread per klien:

```c
fd_set readfds;
FD_ZERO(&readfds);
FD_SET(server_fd, &readfds);

for (int i = 0; i < max_clients; i++) {
  sd = clients[i].fd;
  if (sd > 0)
    FD_SET(sd, &readfds);
}

activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
```

Pola ini memungkinkan server:

1. Mendeteksi koneksi baru via `server_fd`
2. Mendeteksi pesan masuk dari klien yang sudah terhubung
3. Semua dalam satu thread tanpa blocking

#### 3. Autentikasi Klien

Saat klien baru terhubung, server menerima nama pengguna dan melakukan validasi:

```c
recv(new_socket, buffer, 1024, 0);
sanitize_input(buffer);

// Cek duplikasi nama
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
```

Jika nama sudah dipakai, server mengirim `"TAKEN"` dan menutup koneksi. Jika tersedia, mengirim `"OK"` dan menyimpan info klien ke array `clients[]`.

#### 4. Broadcast Pesan

Pesan dari klien biasa disebar ke semua klien lain kecuali pengirim:

```c
void broadcast_message(const char *message, int sender_fd) {
  for (int i = 0; i < max_clients; i++) {
    if (clients[i].fd != 0 && clients[i].fd != sender_fd) {
      send(clients[i].fd, message, strlen(message), 0);
    }
  }
}
```

Format pesan yang di-broadcast: `[nama_pengirim]: pesan\n`

#### 5. Admin Commands

Admin (user dengan nama `"The Knights"`) memiliki akses ke perintah khusus:

| Perintah    | Fungsi                                         |
| ----------- | ---------------------------------------------- |
| `/users`    | Menampilkan jumlah klien aktif (non-admin)     |
| `/uptime`   | Menampilkan durasi server berjalan dalam detik |
| `/shutdown` | Mematikan server secara graceful               |

Implementasi:

```c
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
  }
  // ... perintah lainnya
}
```

#### 6. Client: Thread Penerima Pesan Asinkron

Client menggunakan `pthread` agar bisa menerima pesan dari server sambil tetap menunggu input pengguna:

```c
void *receive_handler(void *socket_desc) {
  int sock = *(int *)socket_desc;
  char buffer[1024];

  while (1) {
    memset(buffer, 0, 1024);
    int receive = recv(sock, buffer, 1024, 0);
    if (receive > 0) {
      printf("%s", buffer);
      fflush(stdout);
      // Cetak ulang prompt setelah menerima pesan
      printf(user.is_admin ? "Admin >> " : "> ");
      fflush(stdout);
    }
  }
}
```

Thread ini dibuat di `main()`:

```c
pthread_t recv_thread;
pthread_create(&recv_thread, NULL, receive_handler, (void *)&sockfd);
```

#### 7. Logging Aktivitas

Semua aktivitas dicatat ke `history.log` dengan format:

```text
[YYYY-MM-DD HH:MM:SS] [entity] [action]
```

Implementasi:

```c
void write_log(const char *entity, const char *action) {
  FILE *log_file = fopen("history.log", "a");
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  char time_str[30];
  strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);
  fprintf(log_file, "[%s] [%s] [%s]\n", time_str, entity, action);
  fclose(log_file);
}
```

Event yang dicatat meliputi: server online/offline, koneksi/diskoneksi klien, pesan pengguna, dan perintah admin.

#### 8. Handler SIGINT

Baik server maupun client menangani `SIGINT` (Ctrl+C) untuk shutdown graceful:

**Server:**

```c
void handle_sigint(int sig) {
  write_log("System", "EMERGENCY SHUTDOWN INITIATED");
  for (int i = 0; i < max_clients; i++) {
    if (clients[i].fd == 0) continue;
    send(clients[i].fd, "\n[System] Server is shutting down...\n", 37, 0);
    close(clients[i].fd);
  }
  close(server_fd);
  printf("\n[System] Server Offline.\n");
  exit(0);
}
```

**Client:**

```c
void handle_sigint(int sig) {
  char exit_msg[] = "/exit\n";
  send(sockfd, exit_msg, strlen(exit_msg), 0);
  close(sockfd);
  printf("\n[System] Disconnecting from The Wired...\n");
  exit(0);
}
```

### Kendala

- `select()` memerlukan tracking manual terhadap semua file descriptor aktif dan menghitung `max_sd` setiap iterasi.
- Thread penerima pesan di client perlu reprint prompt (`> ` atau `Admin >> `) setelah mencetak pesan masuk, agar input pengguna tidak tercampur dengan output server.
- Redirection output (`>`) tidak bisa diproses langsung oleh `execv`, sehingga perlu dibungkus `bash -c`. _(catatan: dari modul sebelumnya)_

### Revisi

- Menambahkan interrupt signal handler untuk shutdown di server dan client
- Memperbaiki format log agar lebih konsisten dan informatif
- Memastikan thread penerima pesan di client selalu mencetak prompt setelah menerima pesan baru
