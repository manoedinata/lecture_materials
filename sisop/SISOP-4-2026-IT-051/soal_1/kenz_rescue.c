#define FUSE_USE_VERSION 31
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <linux/limits.h>
#include <pthread.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// Global variable untuk menyimpan path absolut dari source_directory
char source_dir[PATH_MAX];

// Fungsi untuk membangkitkan isi file tujuan.txt on-the-fly
void get_tujuan_content(char *output) {
  strcpy(output, "Tujuan Mas Amba: ");

  // Iterasi file 1.txt sampai 7.txt
  for (int i = 1; i <= 7; i++) {
    char filepath[PATH_MAX];
    snprintf(filepath, sizeof(filepath), "%s/%d.txt", source_dir, i);

    FILE *f = fopen(filepath, "r");
    if (!f)
      continue; // Skip jika file tidak ada (seharusnya ada)

    char line[256];
    while (fgets(line, sizeof(line), f)) {
      // Cari baris yang diawali dengan "KOORD: "
      if (strncmp(line, "KOORD: ", 7) == 0) {
        char *frag = line + 7;
        // Buang karakter newline bawaan dari file sumber agar bisa digabung
        // sebaris
        frag[strcspn(frag, "\r\n")] = 0;
        strcat(output, frag);
        break;
      }
    }
    fclose(f);
  }
  // Tambahkan tepat satu newline di akhir sesuai soal
  strcat(output, "\n");
}

// Mendapatkan atribut file (dipanggil saat `ls`, `stat`, dll)
static int xmp_getattr(const char *path, struct stat *stbuf,
                       struct fuse_file_info *fi) {
  (void)fi;
  int res = 0;
  memset(stbuf, 0, sizeof(struct stat));

  // Jika yang diakses adalah file virtual tujuan.txt
  if (strcmp(path, "/tujuan.txt") == 0) {
    stbuf->st_mode = __S_IFREG | 0444; // Regular file, read-only
    stbuf->st_nlink = 1;

    char content[4096] = {0};
    get_tujuan_content(content);
    stbuf->st_size = strlen(content); // Ukuran dinamis sesuai hasil gabungan
    return 0;
  }

  // Passthrough untuk file/direktori lainnya
  char fpath[PATH_MAX];
  snprintf(fpath, sizeof(fpath), "%s%s", source_dir, path);

  res = lstat(fpath, stbuf);
  if (res == -1)
    return -errno;

  return 0;
}

// Membaca isi direktori (dipanggil saat `ls`)
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi,
                       enum fuse_readdir_flags flags) {
  DIR *dp;
  struct dirent *de;
  (void)offset;
  (void)fi;
  (void)flags;

  char fpath[PATH_MAX];
  snprintf(fpath, sizeof(fpath), "%s%s", source_dir, path);

  dp = opendir(fpath);
  if (dp == NULL)
    return -errno;

  while ((de = readdir(dp)) != NULL) {
    struct stat st;
    memset(&st, 0, sizeof(st));
    st.st_ino = de->d_ino;
    st.st_mode = de->d_type << 12;

    // FUSE 3 filler membutuhkan 5 argumen
    if (filler(buf, de->d_name, &st, 0, 0))
      break;
  }
  closedir(dp);

  // Inject file virtual tujuan.txt ke root directory mountpoint
  if (strcmp(path, "/") == 0) {
    filler(buf, "tujuan.txt", NULL, 0, 0);
  }

  return 0;
}

// Mengecek/membuka file
static int xmp_open(const char *path, struct fuse_file_info *fi) {
  // Izinkan akses ke file virtual
  if (strcmp(path, "/tujuan.txt") == 0) {
    if ((fi->flags & O_ACCMODE) != O_RDONLY)
      return -EACCES;
    return 0;
  }

  // Passthrough untuk file asli
  int res;
  char fpath[PATH_MAX];
  snprintf(fpath, sizeof(fpath), "%s%s", source_dir, path);

  res = open(fpath, fi->flags);
  if (res == -1)
    return -errno;

  close(res);
  return 0;
}

// Membaca isi file (dipanggil saat `cat`)
static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
                    struct fuse_file_info *fi) {
  int res = 0;

  // Jika membaca file virtual, bangkitkan string on-the-fly
  if (strcmp(path, "/tujuan.txt") == 0) {
    char content[4096] = {0};
    get_tujuan_content(content);
    size_t len = strlen(content);

    if (offset < len) {
      if (offset + size > len)
        size = len - offset;
      memcpy(buf, content + offset, size);
    } else {
      size = 0;
    }
    return size;
  }

  // Passthrough membaca file asli (1.txt - 7.txt)
  char fpath[PATH_MAX];
  snprintf(fpath, sizeof(fpath), "%s%s", source_dir, path);

  int fd = open(fpath, O_RDONLY);
  if (fd == -1)
    return -errno;

  res = pread(fd, buf, size, offset);
  if (res == -1)
    res = -errno;

  close(fd);
  return res;
}

static const struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .open = xmp_open,
    .read = xmp_read,
};

int main(int argc, char *argv[]) {
  // Validasi argumen kustom sesuai soal: ./kenz_rescue <source> <mount>
  if (argc < 3) {
    fprintf(stderr, "Usage: %s <source_directory> <mount_directory>\n",
            argv[0]);
    return 1;
  }

  // FUSE berjalan di background dan mengubah working directory ke "/",
  // jadi kita wajib menyimpan absolute path dari source directory
  if (realpath(argv[1], source_dir) == NULL) {
    perror("Gagal resolve absolute path dari source_directory");
    return 1;
  }

  // Modifikasi argumen yang akan di-pass ke fuse_main
  // fuse_main mengharapkan: ./program <mountpoint> [options]
  int fuse_argc = 2;
  char *fuse_argv[3];
  fuse_argv[0] = argv[0];
  fuse_argv[1] = argv[2]; // Target mount directory
  fuse_argv[2] = NULL;

  return fuse_main(fuse_argc, fuse_argv, &xmp_oper, NULL);
}
