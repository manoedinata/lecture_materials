# Hendra Manudinata - 5027251051

### Mendapatkan direktori tempat script ini berada
### Ref: https://askubuntu.com/questions/893911/when-writing-a-bash-script-how-do-i-get-the-absolute-path-of-the-location-of-th
SCRIPT_DIR=$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")

### INITIALIZE WORKTREE
mkdir -p data log rekap sampah

### INITIALIZE DB
DB_FILE="$SCRIPT_DIR/data/penghuni.csv"
if [ ! -f "$DB_FILE" ]; then
    echo "Nama,Kamar,Harga Sewa,Tanggal Masuk,Status" >"$DB_FILE"
fi

### INITIALIZE SAMPAH
SAMPAH_FILE="$SCRIPT_DIR/sampah/history_hapus.csv"
if [ ! -f "$SAMPAH_FILE" ]; then
    echo "Nama,Kamar,Harga Sewa,Tanggal Masuk,Status" >"$SAMPAH_FILE"
fi

### INITIALIZE LAPORAN
LAPORAN_FILE="$SCRIPT_DIR/rekap/laporan_bulanan.txt"

### INITIALIZE LOG
LOG_FILE="$SCRIPT_DIR/log/tagihan.log"

##### FUNGSI HELPER AWK #####
AWK_UTILS='
# Fungsi format Rupiah di dalam AWK
function format_rp(angka) {
    if (angka == 0 || angka == "") return "Rp0"
    str_angka = angka ""
    len = length(str_angka)
    hasil = ""
    for(i=1; i<=len; i++) {
        hasil = hasil substr(str_angka, i, 1)
        if ((len - i) % 3 == 0 && i != len) {
            hasil = hasil "."
        }
    }
    return "Rp" hasil
}
'

##### FUNCTIONS #####
tambah_penghuni() {
    echo "================================================="
    echo "                 TAMBAH PENGHUNI                 "
    echo "================================================="

    # Nama
    read -p "Masukkan Nama: " nama

    # Kamar & Validasi Unik
    while true; do
        read -p "Masukkan Kamar: " kamar
        # Mengecek apakah nomor kamar sudah ada di kolom ke-2 pada file CSV
        # contoh regex: ^[^,]*,nomorkamar,
        if grep -q "^[^,]*,${kamar}," "$DB_FILE"; then
            echo -e "\n[!] Kamar $kamar sudah terisi! Silakan pilih kamar lain.\n"
        else
            break
        fi
    done

    # Harga Sewa & Validasi Angka Positif
    while true; do
        read -p "Masukkan Harga Sewa: " harga

        if [[ "$harga" =~ ^[0-9]+$ ]] && (("$harga" > 0)); then
            break
        else
            echo -e "\n[!] Harga sewa harus berupa angka positif!\n"
        fi
    done

    # Tanggal & Validasi
    while true; do
        read -p "Masukkan Tanggal Masuk (YYYY-MM-DD): " tanggal

        # Cek apakah format bisa dibaca oleh sistem
        status=$(date -d "$tanggal" +%Y-%m-%d 2>/dev/null)
        if [ $status ]; then
            # Konversi tanggal input dan hari ini ke UNIX timestamp (detik) untuk membandingkan
            input_epoch=$(date -d "$tanggal" +%s)
            now_epoch=$(date +%s)

            if (("$input_epoch" > "$now_epoch")); then
                echo -e "\n[!] Tanggal tidak boleh melebihi hari ini (masa depan)!\n"
            else
                break
            fi

        else
            echo -e "\n[!] Format tanggal salah! Gunakan format YYYY-MM-DD (Contoh: 2026-03-06).\n"
        fi

    done

    # Input Status & Validasi
    while true; do
        read -p "Masukkan Status Awal (Aktif/Menunggak): " status
        status=${status,,} # jadikan lowercase

        if [[ "$status" == "aktif" || "$status" == "menunggak" ]]; then
            break
        else
            echo -e "\n[!] Status tidak valid! Harap ketik 'Aktif' atau 'Menunggak' (huruf kapital dibebaskan).\n"
        fi
    done

    # Simpan ke dalam file CSV
    echo "$nama,$kamar,$harga,$tanggal,$status" >>"$DB_FILE"

    echo ""
    echo "[√] Penghuni \"$nama\" berhasil ditambahkan ke Kamar $kamar dengan status ${status^}." # ${status^} untuk kapitalisasi pertama
    echo ""
}

hapus_penghuni() {
    clear
    echo "================================================="
    echo "                 HAPUS PENGHUNI                  "
    echo "================================================="

    read -p "Masukkan nama penghuni yang akan dihapus: " nama_hapus

    # Cek apakah nama ada di database
    if grep -q "^${nama_hapus}," "$DB_FILE"; then

        tanggal_hapus=$(date +%Y-%m-%d)

        # Salin line dari db_file ke sampah_file
        awk -F, -v nama="$nama_hapus" -v tgl="$tanggal_hapus" '$1 == nama {print $0","tgl}' "$DB_FILE" >>"$SAMPAH_FILE"

        # Hapus line dari db_file
        # bisa juga pakai awk & mv, tapi sed lebih simpel
        # awk -F, -v nama="$nama_hapus" '$1 != nama' "$DB_FILE" > laporan_temp.csv && mv laporan_temp.csv "$DB_FILE"
        sed -i "/^${nama_hapus},/d" "$DB_FILE"

        echo -e "\n[√] Data penghuni \"$nama_hapus\" berhasil diarsipkan ke $SAMPAH_FILE.\n"
    else
        # Jika nama tidak ditemukan
        echo -e "\n[x] Data penghuni \"$nama_hapus\" tidak ditemukan di sistem.\n"
    fi
}

tampilkan_penghuni() {
    clear
    awk "$AWK_UTILS"'
    BEGIN {
        # Set Field Separator menjadi koma
        FS=","

        # Cetak Header Tabel
        print "=========================================================================="
        print "                       DAFTAR PENGHUNI KOST SLEBEW                        "
        print "=========================================================================="
        printf "%-3s | %-15s | %-7s | %-17s | %-10s\n", "No", "Nama", "Kamar", "Harga Sewa", "Status"
        print "--------------------------------------------------------------------------"

        # Inisialisasi variabel penghitung
        total = 0
        aktif = 0
        menunggak = 0
    }

    NR > 1 {
        # Abaikan baris kosong jika ada
        if ($0 ~ /^[[:space:]]*$/) next 

        total++
        if ($5 == "Aktif") aktif++
        if ($5 == "Menunggak") menunggak++

        # Cetak Data
        # (No, Nama, Kamar, Harga_Rp, Status)
        printf "%-3d | %-15s | %-7s | %-17s | %-10s\n", total, $1, $2, format_rp($3), $5
    }

    END {
        # Cetak Footer Tabel
        print "--------------------------------------------------------------------------"
        printf "Total: %d penghuni | Aktif: %d | Menunggak: %d\n", total, aktif, menunggak
        print "==========================================================================\n"
    }
    ' "$DB_FILE"
}

update_status() {
    clear
    echo "================================================="
    echo "                 UPDATE STATUS                   "
    echo "================================================="

    read -p "Masukkan Nama Penghuni: " nama_update

    # Cek apakah nama ada di database
    # format regex: ^nama,
    if ! grep -q "^${nama_update}," "$DB_FILE"; then
        echo -e "\n[x] Penghuni dengan nama \"$nama_update\" tidak ditemukan.\n"
        return
    fi

    # Input status dengan pengecekan case-insensitive
    while true; do
        read -p "Masukkan Status Baru (Aktif/Menunggak): " status_baru
        status_baru=${status_baru,,} # jadikan lowercase

        if [[ "$status_baru" == "aktif" || "$status_baru" == "menunggak" ]]; then
            break
        else
            echo -e "\n[!] Status tidak valid! Harap masukkan 'Aktif' atau 'Menunggak'.\n"
        fi
    done

    # Gunakan AWK untuk menimpa kolom ke-5 berdasarkan nama
    # FS = Field Separator, OFS = Output Field Separator
    awk -v nama="$nama_update" -v status="${status_baru^}" '
    BEGIN { FS=","; OFS="," }
    {
        if ($1 == nama) {
            $5 = status
        }
        print $0
    }
    ' "$DB_FILE" >laporan_temp.csv && mv laporan_temp.csv "$DB_FILE"

    echo -e "\n[√] Status $nama_update berhasil diubah menjadi: ${status_baru^}\n"
}

cetak_laporan() {
    clear

    # Gunakan AWK untuk menghitung total dan mencetaknya ke file
    awk -v file_out="$LAPORAN_FILE" "$AWK_UTILS"'
    BEGIN {
        FS=","
        total_aktif = 0
        total_menunggak = 0
        jumlah_kamar = 0
        count_menunggak = 0
        daftar_menunggak = ""
    }

    NR > 1 {
        # Abaikan baris kosong jika ada
        if ($0 ~ /^[[:space:]]*$/) next

        jumlah_kamar++
        
        if ($5 == "Aktif") {
            total_aktif += $3
        } else if ($5 == "Menunggak") {
            total_menunggak += $3
            daftar_menunggak = daftar_menunggak "  - " $1 " (Kamar " $2 ")\n"
            count_menunggak++
        }
    }

    END {
        # Siapkan teks laporan
        laporan = ""
        laporan = laporan "======================================\n"
        laporan = laporan "          LAPORAN KEUANGAN            \n"
        laporan = laporan "======================================\n"
        laporan = laporan sprintf("%-24s: %s\n", "Total pemasukan (Aktif)", format_rp(total_aktif))
        laporan = laporan sprintf("%-24s: %s\n", "Total tunggakan", format_rp(total_menunggak))
        laporan = laporan sprintf("%-24s: %d\n", "Jumlah kamar terisi", jumlah_kamar)
        laporan = laporan "-------------------------------------------------\n"
        laporan = laporan "\n"
        laporan = laporan "Daftar penghuni menunggak:\n"

        # Cek apakah ada yang menunggak
        if (count_menunggak == 0) {
            laporan = laporan "  Tidak ada, yey!\n"
        } else {
            laporan = laporan daftar_menunggak
        }
        
        laporan = laporan "=================================================\n"

        # Simpan teks ke file
        printf "%s", laporan > file_out
        
        # Tampilkan teks
        printf "%s\n", laporan
        print "[√] Laporan berhasil disimpan ke " file_out
    }
    ' "$DB_FILE"

    echo ""
}

##### END OF FUNCTIONS #####

##### CRON #####

kelola_cron() {
    # Mendapatkan path absolut (lengkap) dari script ini agar dikenali cron
    SCRIPT_PATH=$(realpath "$0")
    CRON_CMD="$SCRIPT_PATH --check-tagihan"

    while true; do
        clear
        cat <<"EOF"
=================================================
               MENU KELOLA CRON
=================================================
 1. Lihat Cron Job Aktif
 2. Daftarkan Cron Job Pengingat
 3. Hapus Cron Job Pengingat
 4. Kembali
=================================================
EOF
        read -p "Pilih [1-4]: " pilihan_cron

        case $pilihan_cron in
        1)
            echo ""
            echo "--- Daftar Cron Job Pengingat Tagihan ---"
            # Menampilkan cron job yang hanya memuat script kita
            crontab -l 2>/dev/null | grep "$CRON_CMD" || echo "Belum ada jadwal yang didaftarkan."
            echo ""
            read -p "Tekan [ENTER] untuk kembali ke menu..."
            ;;
        2)
            echo ""
            read -p "Masukkan Jam (0-23): " jam
            read -p "Masukkan Menit (0-59): " menit

            # Menghapus jadwal cron lama milik script ini (agar tidak double/overwrite)
            (crontab -l 2>/dev/null | grep -v "$CRON_CMD") | crontab -

            # Mendaftarkan jadwal baru
            # Format cron: menit jam * * * perintah
            #   1. ambil semua cron job yang sudah ada (jika ada),
            #   2. lalu tambahkan baris baru untuk script kita, dan daftarkan ulang semuanya
            (
                crontab -l 2>/dev/null
                echo "$menit $jam * * * $CRON_CMD"
            ) | crontab -

            read -p "Tekan [ENTER] untuk kembali ke menu..."
            ;;
        3)
            echo ""
            # Menghapus jadwal cron milik script ini tanpa menyentuh cron sistem lain
            (crontab -l 2>/dev/null | grep -v "$CRON_CMD") | crontab -
            echo -e "[√] Cron job pengingat tagihan berhasil dihapus.\n"
            read -p "Tekan [ENTER] untuk kembali ke menu..."
            ;;
        4)
            # Kembali ke menu utama (keluar dari loop)
            return
            ;;
        *)
            echo -e "\n[x] Pilihan tidak valid."
            sleep 1
            ;;
        esac
    done
}

# Jika script dijalankan dengan argumen --check-tagihan (oleh sistem Cron)
if [ "$1" == "--check-tagihan" ]; then
    echo "[>] Menjalankan: Cek Tagihan"

    timestamp=$(date +"%Y-%m-%d %H:%M:%S")

    # Cek database dan catat yang menunggak ke log
    awk -F, -v ts="$timestamp" -v logfile="$LOG_FILE" '
    NR > 1 && $5 == "Menunggak" {
        print "[" ts "] Pengingat: Penghuni " $1 " (Kamar " $2 ") masih menunggak tagihan." >> logfile
    }
    ' "$DB_FILE"

    # Bail out setelah selesai
    exit 0
fi

##### END OF CRON #####

ascii_art=$(
    cat <<'EOF'
  _  __         _      _____               _ _ 
 | |/ /        | |    |  __ \             | (_)
 | ' / ___  ___| |_   | |__) |   _ ___  __| |_ 
 |  < / _ \/ __| __|  |  _  / | | / __|/ _` | |
 | . \ (_) \__ \ |_   | | \ \ |_| \__ \ (_| | |
 |_|\_\___/|___/\__|  |_|  \_\__,_|___/\__,_|_|

EOF
)

show_menu() {
    cat <<"EOF"
=================================================
          SISTEM MANAJEMEN KOST RUSDI
=================================================
ID | OPTION
-------------------------------------------------
 1 | Tambah Penghuni Baru
 2 | Hapus Penghuni
 3 | Tampilkan Daftar Penghuni
 4 | Update Status Penghuni
 5 | Cetak Laporan Keuangan
 6 | Kelola Cron (Pengingat Tagihan)
 7 | Exit Program
=================================================

EOF
}

while true; do
    clear
    echo "$ascii_art"
    show_menu

    read -p "Masukkan pilihan [1-7]: " option
    case $option in

    1)
        tambah_penghuni
        read -p "Tekan [ENTER] untuk kembali ke menu..."
        ;;
    2)
        hapus_penghuni
        read -p "Tekan [ENTER] untuk kembali ke menu..."
        ;;
    3)
        tampilkan_penghuni
        read -p "Tekan [ENTER] untuk kembali ke menu..."
        ;;
    4)
        update_status
        read -p "Tekan [ENTER] untuk kembali ke menu..."
        ;;
    5)
        cetak_laporan
        read -p "Tekan [ENTER] untuk kembali ke menu..."
        ;;
    6)
        kelola_cron
        ;;
    7 | 0)
        echo -e "\n[!] Keluar dari program. Terima kasih!"
        exit 0
        ;;
    *)
        # Error bjir
        echo -e "\n[x] Pilihan tidak valid anjj."
        sleep 1.5
        ;;
    esac
done
