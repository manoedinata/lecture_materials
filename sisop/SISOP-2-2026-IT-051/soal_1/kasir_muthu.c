#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

// fungsi untuk menjalankan perintah exec dengan argumen
int run_exec(const char *path, char *const argv[]) {
  pid_t pid = fork();

  if (pid < 0) {
    return -1; // error saat fork
  }

  if (pid == 0) {
    execv(path, argv);
    return 0;
  } else {
    // proses induk: tunggu proses anak selesai
    int status;
    wait(&status);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
      return -1; // error saat exec
    }
  }

  return 0; // sukses
}

int main() {
  pid_t pid;

  // buat folder dengan exec mkdir
  char *mkdir_args[] = {"mkdir", "brankas_kedai", NULL};
  if (run_exec("/usr/bin/mkdir", mkdir_args) < 0) {
    goto error;
  }

  // copy file dengan exec cp
  char *cp_args[] = {"cp", "buku_hutang.csv", "brankas_kedai", NULL};
  if (run_exec("/usr/bin/cp", cp_args) < 0) {
    goto error;
  }

  // ambil data penunggak dengan exec grep
  char *grep_args[] = {"bash", "-c",
                       "grep 'Belum Lunas' buku_hutang.csv > "
                       "brankas_kedai/daftar_penunggak.txt",
                       NULL};
  if (run_exec("/usr/bin/bash", grep_args) < 0) {
    goto error;
  }

  // zip file dengan exec zip
  char *zip_args[] = {"zip", "-r", "rahasia_muthu.zip", "brankas_kedai/", NULL};
  if (run_exec("/usr/bin/zip", zip_args) < 0) {
    goto error;
  }

  // Sukses!
  return 0;

// Kalau ada error, lompat (jump) ke sini
error:
  printf("[ERROR] Aiyaa! Proses gagal, file atau folder tidak ditemukan.");
  exit(1);
}
