# SISOP-4-2026-IT-051

- [SISOP-4-2026-IT-051](#sisop-4-2026-it-051)
  - [Soal 1](#soal-1)
    - [Deskripsi Soal](#deskripsi-soal)
    - [Compile \& Run](#compile--run)
    - [Penjelasan](#penjelasan)
    - [Kendala](#kendala)

## Soal 1

### Deskripsi Soal

Soal meminta kita untuk membuat filesistem FUSE sederhana yang mempunyai operasi:

- Get attr
- Read dir
- Read file
- Open file

Fungsi FUSE pada soal ini adalah menampilkan file virtual `tujuan.txt` yang berisi koordinat dari seluruh file
di direktori asli. File `tujuan.txt` akan selalu diperbarui setiap kali ada panggilan baca (getattr & read file) dengan fungsi `get_tujuan_content()`.

### Compile & Run

```bash
gcc -o kenz_rescue kenz_rescue.c `pkg-config fuse --cflags --libs`
./kenz_rescue <src_dir> <mount_point>
```

### Penjelasan

Program ini menggunakan FUSE dengan 4 operasi utama: `getattr`, `readdir`, `open`, dan `read`.

```c
static const struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .open = xmp_open,
    .read = xmp_read,
};
```

Pada `getattr`, kita memeriksa apakah path yang diminta adalah root (`/`) atau `tujuan.txt`. Jika root, kita mengembalikan atribut direktori. Jika `tujuan.txt`, kita mengembalikan atribut file dengan ukuran yang sesuai dengan konten yang dihasilkan oleh `get_tujuan_content()`.

```c
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
```

Pada `readdir`, kita menambahkan entri untuk `tujuan.txt` jika path adalah root.

```c
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
```

Pada `open`, kita memeriksa apakah file yang dibuka adalah `tujuan.txt`. Jika ya, kita mengizinkan pembukaan file.

```c
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
```

Pada `read`, kita memeriksa apakah file yang dibaca adalah `tujuan.txt`. Jika ya, kita menghasilkan konten terbaru dengan memanggil `get_tujuan_content()` dan mengembalikan bagian yang diminta.

```c
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
```

### Kendala

_Tidak ada_
