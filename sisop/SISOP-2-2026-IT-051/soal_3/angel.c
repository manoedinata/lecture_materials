#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

//=====================================
// Base64
//=====================================
static const char b64_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static unsigned char b64_reverse_table[256];
static int b64_reverse_table_initialized = 0;

static void init_b64_reverse_table(void) {
  if (b64_reverse_table_initialized)
    return;

  memset(b64_reverse_table, 0x80, sizeof(b64_reverse_table));
  for (int i = 0; i < 64; i++) {
    b64_reverse_table[(unsigned char)b64_table[i]] = (unsigned char)i;
  }
  b64_reverse_table[(unsigned char)'='] = 0;
  b64_reverse_table_initialized = 1;
}

char *base64_encode(const unsigned char *data, size_t input_length) {
  size_t output_length = 4 * ((input_length + 2) / 3);
  char *encoded_data = malloc(output_length + 1);
  if (encoded_data == NULL)
    return NULL;

  for (size_t i = 0, j = 0; i < input_length;) {
    uint32_t octet_a = i < input_length ? data[i++] : 0;
    uint32_t octet_b = i < input_length ? data[i++] : 0;
    uint32_t octet_c = i < input_length ? data[i++] : 0;

    uint32_t triple = (octet_a << 16) | (octet_b << 8) | octet_c;

    encoded_data[j++] = b64_table[(triple >> 18) & 0x3F];
    encoded_data[j++] = b64_table[(triple >> 12) & 0x3F];
    encoded_data[j++] = b64_table[(triple >> 6) & 0x3F];
    encoded_data[j++] = b64_table[triple & 0x3F];
  }

  for (int i = 0; i < (3 - (input_length % 3)) % 3; i++)
    encoded_data[output_length - 1 - i] = '=';

  encoded_data[output_length] = '\0';
  return encoded_data;
}

unsigned char *base64_decode(const char *data, size_t *output_length) {
  init_b64_reverse_table();

  size_t input_length = strlen(data);
  if (input_length % 4 != 0)
    return NULL;

  size_t out_len = input_length / 4 * 3;
  if (input_length >= 1 && data[input_length - 1] == '=')
    out_len--;
  if (input_length >= 2 && data[input_length - 2] == '=')
    out_len--;

  unsigned char *decoded_data = malloc(out_len + 1);
  if (decoded_data == NULL)
    return NULL;

  for (size_t i = 0, j = 0; i < input_length;) {
    uint32_t sextet_a =
        data[i] == '=' ? 0 : b64_reverse_table[(unsigned char)data[i]];
    uint32_t sextet_b =
        data[i + 1] == '=' ? 0 : b64_reverse_table[(unsigned char)data[i + 1]];
    uint32_t sextet_c =
        data[i + 2] == '=' ? 0 : b64_reverse_table[(unsigned char)data[i + 2]];
    uint32_t sextet_d =
        data[i + 3] == '=' ? 0 : b64_reverse_table[(unsigned char)data[i + 3]];

    if ((sextet_a & 0x80) || (sextet_b & 0x80) || (sextet_c & 0x80) ||
        (sextet_d & 0x80)) {
      free(decoded_data);
      return NULL;
    }

    uint32_t triple =
        (sextet_a << 18) | (sextet_b << 12) | (sextet_c << 6) | sextet_d;

    if (j < out_len)
      decoded_data[j++] = (triple >> 16) & 0xFF;
    if (data[i + 2] != '=' && j < out_len)
      decoded_data[j++] = (triple >> 8) & 0xFF;
    if (data[i + 3] != '=' && j < out_len)
      decoded_data[j++] = triple & 0xFF;

    i += 4;
  }

  decoded_data[out_len] = '\0';
  if (output_length)
    *output_length = out_len;

  return decoded_data;
}

//=====================================
// Helper functions
//=====================================

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

void write_log(const char *nama_proses, const char *status) {
  FILE *log_fp;

  // Ambil waktu saat ini
  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);
  char time_str[64];

  // Format waktu sesuai soal: [dd:mm:yyyy]-[hh:mm:ss]
  strftime(time_str, sizeof(time_str), "[%d:%m:%Y]-[%H:%M:%S]", tm_info);

  // Tulis ke file dengan format: [waktu]_nama-proses_STATUS
  char log_entry[128];
  snprintf(log_entry, sizeof(log_entry), "%s_%s_%s\n", time_str, nama_proses,
           status);
  writeTextToFile(log_fp, "ethereal.log", log_entry, 1);
}

// get random status
const char *getRandomStatus() {
  const char *availableStatus[] = {
      "aku akan fokus ada diriku sendiri",
      "aku mencintaimu dari sekarang hingga selamanya",
      "aku akan menjauh darimu, hingga takdir mempertemukan kita di versi kita "
      "yang terbaik.",
      "kalau aku dilahirkan kembali, aku tetap akan terus menyayangi dirimu"};

  int random_index = rand() % 4;
  return availableStatus[random_index];
}

// decrypt
void handle_decrypt() {
  write_log("decrypt", "RUNNING");

  FILE *fp = fopen("LoveLetter.txt", "r");
  if (fp == NULL) {
    printf("Error: File LoveLetter.txt tidak ditemukan!\n");
    write_log("decrypt", "ERROR");
    return;
  }

  fseek(fp, 0, SEEK_END);
  long fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  if (fsize == 0) {
    printf("Error: File LoveLetter.txt kosong.\n");
    fclose(fp);
    write_log("decrypt", "ERROR");
    return;
  }

  char *encoded_data = malloc(fsize + 1);
  fread(encoded_data, 1, fsize, fp);
  fclose(fp);
  encoded_data[fsize] = '\0';

  size_t out_len;
  unsigned char *decoded_data = base64_decode(encoded_data, &out_len);
  free(encoded_data);

  if (decoded_data != NULL) {
    FILE *out_fp = fopen("LoveLetter.txt", "w");
    if (out_fp) {
      fprintf(out_fp, "%s", decoded_data);
      fclose(out_fp);
      printf("Berhasil decrypt LoveLetter.txt\n");
      write_log("decrypt", "SUCCESS");
    }

    else {
      write_log("decrypt", "ERROR");
    }
    free(decoded_data);
  } else {
    printf("Error: Gagal melakukan decrypt.\n");
    write_log("decrypt", "ERROR");
  }
}

// kill
void handle_kill() {
  write_log("kill", "RUNNING");

  // Gunakan popen untuk menjalankan command bash dan mengambil output PID
  FILE *cmd = popen("ps aux | grep '[m]aya' | awk '{print $2}'", "r");
  if (cmd == NULL) {
    // anggap daemon belum berjalan
    printf("Error: Program daemon maya belum berjalan.\n");
    write_log("kill", "ERROR");
    return;
  }

  pid_t target_pid = -1;
  char pid_str[32];

  // Baca output dari perintah popen tadi
  if (fgets(pid_str, sizeof(pid_str), cmd) != NULL) {
    target_pid = atoi(pid_str); // Ubah string PID jadi integer
  }
  pclose(cmd);

  if (target_pid > 0) {
    // Kirim sinyal SIGTERM untuk mematikan proses
    if (kill(target_pid, SIGTERM) == 0) {
      printf("Berhasil menghentikan proses maya (PID: %d)\n", target_pid);
      write_log("kill", "SUCCESS");
    } else {
      printf("Error: Gagal menghentikan proses.\n");
      write_log("kill", "ERROR");
    }
  } else {
    printf("Error: Program daemon maya belum berjalan.\n");
    write_log("kill", "ERROR");
  }
}

//=====================================
// Main code
//=====================================

int main(int argc, char *argv[]) {
  // Inisialisasi seed random
  srand(time(NULL));

  // 1. Pengecekan Argumen harus dilakukan PERTAMA KALI
  if (argc <= 1) {
    printf("Penggunaan:\n");
    printf("  ./angel -daemon     : jalankan sebagai daemon (nama proses: "
           "maya)\n");
    printf("  ./angel -decrypt    : decrypt LoveLetter.txt\n");
    printf("  ./angel -kill       : kill proses\n");
    exit(EXIT_SUCCESS);
  }

  if (strcmp(argv[1], "-daemon") == 0) {
    // Lanjut ke pembuatan daemon di bawah
    write_log("daemon", "RUNNING");
  }

  else if (strcmp(argv[1], "-decrypt") == 0) {
    handle_decrypt();
    exit(EXIT_SUCCESS);
  }

  else if (strcmp(argv[1], "-kill") == 0) {
    handle_kill();
    exit(EXIT_SUCCESS);
  }

  else {
    printf("Penggunaan:\n");
    printf("  ./angel -daemon     : jalankan sebagai daemon (nama proses: "
           "maya)\n");
    printf("  ./angel -decrypt    : decrypt LoveLetter.txt\n");
    printf("  ./angel -kill       : kill proses\n");
    exit(EXIT_SUCCESS);
  }

  // ====================================
  // DAEMON SETUP
  // ====================================
  pid_t pid, sid;

  pid = fork();
  if (pid < 0) {
    printf("Gagal bikin daemon!\n");
    write_log("daemon", "ERROR");
    exit(EXIT_FAILURE);
  }
  if (pid > 0) {
    printf("Daemon process dibuat dengan PID: %d\n", pid);
    exit(EXIT_SUCCESS);
  }

  umask(0);
  sid = setsid();
  if (sid < 0) {
    write_log("daemon", "ERROR");
    exit(EXIT_FAILURE);
  }

  //   Ganti nama proses menjadi "maya"
  memset(argv[0], '\0', strlen(argv[0]));
  strcpy(argv[0], "maya");

  write_log("daemon", "SUCCESS");

  int sleep_counter = 10;

  // Loop Utama Daemon
  while (1) {
    if (sleep_counter >= 10) { // Gunakan >= untuk keamanan
      write_log("secret", "RUNNING");

      const char *pikiranDikaRaw = getRandomStatus();
      const char *pikiranDika = base64_encode(
          (const unsigned char *)pikiranDikaRaw, strlen(pikiranDikaRaw));

      // Tulis (overwrite) ke LoveLetter.txt
      int status = writeTextToFile(NULL, "LoveLetter.txt", pikiranDika, 0);
      if (status == 0) {
        write_log("secret", "SUCCESS");
      } else {
        write_log("secret", "ERROR");
      }

      sleep_counter = 0; // langsung reset counter
    }

    sleep(1);
    sleep_counter++;
  }

  return 0;
}