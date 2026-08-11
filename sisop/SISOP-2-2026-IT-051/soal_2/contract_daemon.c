#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

// generate timestamp
void getCurrentTimestamp(char *buffer, size_t buffer_size) {
  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);
  strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", tm_info);
}

// fungsi untuk menulis text ke file, dengan opsi append atau overwrite
int writeTextToFile(FILE *fptr, const char *filename, const char *text,
                    int append) {
  if (append == 1) {
    fptr = fopen(filename, "a");
  } else {
    fptr = fopen(filename, "w");
  }

  if (fptr == NULL) {
    return -1; // error saat membuka file
  }

  fprintf(fptr, "%s", text);
  fclose(fptr);

  return 0; // sukses
}

// fungsi untuk menulis isi contract.txt, dengan opsi status restore atau create
int writeContract(FILE *fptr, int restore) {
  int write_status;
  write_status =
      writeTextToFile(fptr, "contract.txt",
                      "A promise to keep going, even when unseen.\n\n", 0);
  if (write_status < 0) {
    return -1; // error saat menulis file
  }

  // build timestamp untuk contract
  char timestamp[20];
  getCurrentTimestamp(timestamp, sizeof(timestamp));

  char contract_text[256];
  if (restore == 0) {
    snprintf(contract_text, sizeof(contract_text), "created-at: %s\n",
             timestamp);
    write_status = writeTextToFile(fptr, "contract.txt", contract_text, 1);
  } else {
    snprintf(contract_text, sizeof(contract_text), "restored-at: %s\n",
             timestamp);
    write_status = writeTextToFile(fptr, "contract.txt", contract_text, 1);
  }

  if (write_status < 0) {
    return -1;
  }

  return 0;
}

// get random status
const char *getRandomStatus() {
  const char *availableStatus[] = {"[awake]", "[drifting]", "[numbness]"};

  int random_index = rand() % 3; // Random index antara 0, 1, atau 2
  return availableStatus[random_index];
}

void handle_sigint(int sig) {
  writeTextToFile(NULL, "work.log", "We really weren't meant to be together",
                  1);

  _exit(0);
}

int main() {
  pid_t pid, sid;

  pid = fork();
  if (pid < 0) {
    printf("Gagal bikin daemon!\n");
    exit(EXIT_FAILURE);
  }
  if (pid > 0) {
    printf("Daemon process dibuat dengan PID: %d\n", pid);
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  // Handle SIGINT (Ctrl+C)
  signal(SIGINT, handle_sigint);

  // Set random seed untuk getRandomStatus
  srand(time(NULL) ^ getpid()); // Seed yang aman menggunakan waktu + PID

  FILE *fptr;

  /*
   * Hitung berapa kali sudah sleep selama 5 detik. Jika == 5, tulis ke
   * work.log, dan reset counter.
   */
  int sleep_counter = 5;

  /*
   * Cek apakah contract.txt sudah dibuat.
   * 0: belum dibuat, 1: sudah dibuat
   */
  int contract_created = 0;

  /*
   * Cek apakah ini run pertama.
   * 1: run pertama, 0: run selanjutnya
   */
  int first_run = 1;

  char log_text[1024] = {0};
  char previous_log[1024] = {0};

  while (1) {
    if (sleep_counter == 5) {
      char work_text[256];
      snprintf(work_text, sizeof(work_text), "still working... %s\n",
               getRandomStatus());
      writeTextToFile(fptr, "work.log", work_text, 1);

      // langsung reset counter
      sleep_counter = 0;
    }

    /*
     * Cek apakah contract.txt ada. Kalau tidak ada, buat ulang. Kalau ada, baca
     * isinya dan cek apakah berubah dari sebelumnya. Kalau berubah, berarti
     * contract dilanggar, tulis ke work.log dan buat ulang contract.txt dengan
     * status restore.
     */

    fptr = fopen("contract.txt", "r");
    if (fptr == NULL) {
      // File dihapus / tidak ada, buat ulang
      if (contract_created == 0) {
        writeContract(fptr, 0);
        contract_created = 1;
      } else {
        writeContract(fptr, 1);
      }
    }

    else {
      log_text[0] = '\0'; // Reset log_text sebelum membaca ulang

      char log_buffer[256];
      while (fgets(log_buffer, sizeof(log_buffer), fptr) != NULL) {
        strcat(log_text, log_buffer);
      }
      fclose(fptr);

      // Cek apakah log berubah
      if (strcmp(log_text, previous_log) != 0 && first_run == 0) {
        // Log berubah, tulis ke work.log
        writeTextToFile(fptr, "work.log", "contract violated\n", 1);
        writeContract(fptr, 1);

        // Update log_text ke state yang baru di-restore
        log_text[0] = '\0';
        fptr = fopen("contract.txt", "r");
        if (fptr != NULL) {
          while (fgets(log_buffer, sizeof(log_buffer), fptr) != NULL) {
            strcat(log_text, log_buffer);
          }
          fclose(fptr); // Jangan lupa close lagi
        }
      }

      // Update previous_log dengan log_text yang baru dibaca
      strcpy(previous_log, log_text);

      if (first_run == 1) {
        first_run = 0;
      }
    }

    sleep(1);
    sleep_counter++;
  }

  return 0;
}
