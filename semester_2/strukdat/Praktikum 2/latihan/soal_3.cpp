// RAE - FURAB
// singly linked list untuk setlist lagu

#include <iostream>
#include <string>

using namespace std;

// Node untuk setiap lagu dalam Setlist
class SongNode
{
public:
    string title;
    SongNode *next;

    SongNode(string title)
    {
        this->title = title;
        this->next = nullptr;
    }
};

// Singly Linked List untuk Setlist
class Setlist
{
private:
    SongNode *head;
    SongNode *tail;

public:
    Setlist() : head(nullptr), tail(nullptr) {}

    bool isEmpty()
    {
        return head == nullptr;
    }

    // Tambah lagu di akhir Setlist
    void tambahBelakang(string judul)
    {
        SongNode *newNode = new SongNode(judul);

        // Klo list kosong, new node jadi head dan tail
        if (isEmpty())
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            // Link new node ke tail saat ini, lalu update tail ke new node
            tail->next = newNode;
            tail = newNode;
        }
    }

    // Tambah lagu di awal Setlist
    void tambahDepan(string judul)
    {
        SongNode *newNode = new SongNode(judul);

        // Klo list kosong, new node jadi head dan tail
        if (isEmpty())
        {
            head = newNode;
            tail = newNode;
        }

        else
        {
            // Link new node ke head saat ini, lalu update head ke new node
            newNode->next = head;
            head = newNode;
        }
    }

    // Hapus lagu paling awal
    void hapusDepan()
    {
        if (isEmpty())
        {
            cout << "Setlist kosong, Kamu belum pilih lagu!" << endl;
            return;
        }

        SongNode *toDelete = head;

        // Update head ke node berikutnya
        head = head->next;

        // Klo ternyata hanya ada satu node, update tail ke nullptr juga
        if (head == nullptr)
        {
            tail = nullptr;
        }

        delete toDelete;
    }

    // Hapus lagu paling akhir
    void hapusBelakang()
    {
        if (isEmpty())
        {
            cout << "Setlist kosong, Kamu belum pilih lagu!" << endl;
            return;
        }

        // Jika hanya ada satu node, hapus head dan update head & tail ke nullptr
        if (head == tail)
        {
            delete head;
            head = nullptr;
            tail = nullptr;
            return;
        }

        // Recursif cari node kedua terakhir
        SongNode *temp = head;
        while (temp->next != tail)
        {
            temp = temp->next; // hasilnya adalah node sebelum tail
        }

        delete tail;

        // Update tail ke node kedua terakhir
        tail = temp;
        tail->next = nullptr;
    }

    // Hapus lagu tertentu berdasarkan judul
    void hapusLagu(string judul)
    {
        // Case 1: Hapus di depan
        if (head->title == judul)
        {
            hapusDepan();
            return;
        }

        // Rekursif cari node sebelum node yang mau dihapus
        SongNode *temp = head;
        while (temp->next != nullptr && temp->next->title != judul)
            temp = temp->next;

        if (temp->next == nullptr)
            return;

        // Node yang mau dihapus adalah temp->next
        SongNode *toDelete = temp->next;
        temp->next = temp->next->next;

        // Klo node yang dihapus adalah tail, update tail ke node sebelumnya
        if (toDelete == tail)
            tail = temp;

        delete toDelete;
    }

    // Tampilkan semua lagu
    void tampilkan()
    {
        if (isEmpty())
        {
            cout << "Setlist kosong, Kamu belum pilih lagu!" << endl;
            return;
        }

        SongNode *temp = head;
        while (temp != nullptr)
        {
            cout << temp->title << endl;
            temp = temp->next;
        }
    }
};

int main()
{
    Setlist setlishAkuuu;

    int n;
    if (!(cin >> n))
        return 0;
    cin.ignore(); // Ignore newline after reading n

    // tampung dulu semua command
    string commands[n];
    for (int i = 0; i < n; i++)
    {
        getline(cin, commands[i]);
    }

    for (int i = 0; i < n; i++)
    {
        string command = commands[i];

        string action;
        string value = "";
        size_t spacePos = command.find(' ');

        if (spacePos != string::npos)
        {
            action = command.substr(0, spacePos);
            value = command.substr(spacePos + 1);
        }
        else
        {
            action = command;
        }

        if (action == "TAMBAH_DEPAN")
        {
            setlishAkuuu.tambahDepan(value);
        }

        else if (action == "TAMBAH_BELAKANG")
        {
            setlishAkuuu.tambahBelakang(value);
        }

        else if (action == "HAPUS_DEPAN")
        {
            setlishAkuuu.hapusDepan();
        }

        else if (action == "HAPUS_BELAKANG")
        {
            setlishAkuuu.hapusBelakang();
        }

        else if (action == "HAPUS_LAGU")
        {
            setlishAkuuu.hapusLagu(value);
        }

        else if (action == "TAMPILKAN")
        {
            setlishAkuuu.tampilkan();
        }
    }
}