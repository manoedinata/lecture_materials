// REI - Menyusun Jejak, Mengingat Lelah
// single linked list

#include <iostream>
#include <string>

using namespace std;

class Node
{
public:
    string data;
    Node *next;

    Node(string data)
    {
        this->data = data;
        this->next = nullptr;
    }
};

class LinkedList
{
public:
    Node *kegiatan;
    Node *energi;
    int size;

    LinkedList()
    {
        kegiatan = nullptr;
        energi = nullptr;
        size = 0;
    }

    int getLastNode(Node *&head, Node *&target)
    {
        Node *temp = head;
        int count = 0;
        while (temp->next != nullptr)
        {
            temp = temp->next;
            count++;
        }
        target = temp;

        return count;
    }

    void addToEnd(string namaKegiatan, string energiValue)
    {
        Node *newKegiatan = new Node(namaKegiatan);
        Node *newEnergi = new Node(energiValue);

        // If list is empty, new node becomes head
        if (kegiatan == nullptr)
        {
            kegiatan = newKegiatan;
            energi = newEnergi;
        }

        // Otherwise, find the last node and append the new node
        else
        {
            Node *temp;

            getLastNode(kegiatan, temp);
            temp->next = newKegiatan;

            getLastNode(energi, temp);
            temp->next = newEnergi;
        }

        size++;

        cout << "Kegiatan " << namaKegiatan << " berhasil ditambahkan!" << endl;
    }

    void addToFront(string namaKegiatan, string energiValue)
    {
        Node *newKegiatan = new Node(namaKegiatan);
        Node *newEnergi = new Node(energiValue);

        // Insert as head if list is empty
        if (kegiatan == nullptr)
        {
            kegiatan = newKegiatan;
            energi = newEnergi;
        }

        // Otherwise, insert at front
        // New node's next points to current head, then update head to new node
        else
        {
            newKegiatan->next = kegiatan;
            kegiatan = newKegiatan;

            newEnergi->next = energi;
            energi = newEnergi;
        }

        size++;
        cout << "Kegiatan " << namaKegiatan << " berhasil ditambahkan di depan!" << endl;
    }

    void insertAt(string namaKegiatan, string energiValue, int index)
    {
        Node *newKegiatan = new Node(namaKegiatan);
        Node *newEnergi = new Node(energiValue);

        Node *temp;

        // Abaikan jika indeks tidak valid
        if (index < 0 || index > size)
        {
            cout << "Index tidak valid!" << endl;
            return;
        }

        // Case 1: Insert at front
        if (index == 0)
        {
            newKegiatan->next = kegiatan;
            kegiatan = newKegiatan;

            newEnergi->next = energi;
            energi = newEnergi;
        }

        // Case 2: Insert at end
        else if (index == size)
        {
            getLastNode(kegiatan, temp);
            temp->next = newKegiatan;

            getLastNode(energi, temp);
            temp->next = newEnergi;
        }

        else
        {

            // Case 3: Insert in the middle
            Node *temp;

            temp = kegiatan;
            for (int j = 0; j < index - 1; j++)
            {
                temp = temp->next;
            }
            newKegiatan->next = temp->next;
            temp->next = newKegiatan;

            temp = energi;
            for (int j = 0; j < index - 1; j++)
            {
                temp = temp->next;
            }
            newEnergi->next = temp->next;
            temp->next = newEnergi;
        }

        size++;
        cout << "Kegiatan berhasil disisipkan!" << endl;
    }

    void removeAt(LinkedList &list, int index)
    {
        Node *temp;

        // Abaikan jika indeks tidak valid
        if (index < 0 || index >= size)
        {
            cout << "Index tidak ditemukan!" << endl;
            return;
        }

        // Case 1: Remove at front
        if (index == 0)
        {
            Node *toDeleteKegiatan = kegiatan;
            kegiatan = kegiatan->next;
            delete toDeleteKegiatan;

            Node *toDeleteEnergi = energi;
            energi = energi->next;
            delete toDeleteEnergi;
        }

        // Case 2: Remove at end
        else if (index == size - 1)
        {
            Node *temp;

            getLastNode(kegiatan, temp);
            Node *toDeleteKegiatan = temp;
            temp = kegiatan;
            while (temp->next != toDeleteKegiatan)
            {
                temp = temp->next;
            }
            temp->next = nullptr;
            delete toDeleteKegiatan;

            getLastNode(energi, temp);
            Node *toDeleteEnergi = temp;
            temp = energi;
            while (temp->next != toDeleteEnergi)
            {
                temp = temp->next;
            }
            temp->next = nullptr;
            delete toDeleteEnergi;
        }

        // Case 3: Remove in the middle
        else
        {
            Node *toDeleteKegiatan;
            Node *toDeleteEnergi;

            temp = kegiatan;
            for (int j = 0; j < index - 1; j++)
            {
                temp = temp->next;
            }
            toDeleteKegiatan = temp->next;
            temp->next = temp->next->next;

            temp = energi;
            for (int j = 0; j < index - 1; j++)
            {
                temp = temp->next;
            }
            toDeleteEnergi = temp->next;
            temp->next = temp->next->next;

            delete toDeleteKegiatan;
            delete toDeleteEnergi;
        }

        size--;
        cout << "Kegiatan berhasil dihapus!" << endl;
    }

    void printAll()
    {
        Node *tempKegiatan = kegiatan;
        Node *tempEnergi = energi;

        cout << endl
             << "Daftar Kegiatan" << endl;

        size_t count = 0;
        while (tempKegiatan != nullptr && tempEnergi != nullptr)
        {
            cout << "[" << count << "] " << tempKegiatan->data << " (Energi: " << tempEnergi->data << ")" << endl;

            tempKegiatan = tempKegiatan->next;
            tempEnergi = tempEnergi->next;
            count++;
        }

        if (count == 0)
        {
            cout << "Belum ada kegiatan." << endl;
        }
    }

    void calculateTotalEnergy()
    {
        Node *tempEnergi = energi;
        int totalEnergi = 0;

        while (tempEnergi != nullptr)
        {
            // cout << "CURRENT ENERGI: " << tempEnergi->data << endl;
            totalEnergi += stoi(tempEnergi->data);
            tempEnergi = tempEnergi->next;
        }

        cout << "Total Energi yang dibutuhkan: " << totalEnergi << endl;
    }
};

int main()
{
    LinkedList myListGweh;

    while (true)
    {
        string command;
        getline(cin, command);

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

        // KEGIATAN: Add to end
        // Format: KEGIATAN <nama> <energi>
        if (action == "KEGIATAN")
        {
            // Parse nama dan energi
            size_t secondSpacePos = value.find(' ');
            if (secondSpacePos == string::npos)
                continue;

            string namaKegiatan = value.substr(0, secondSpacePos);
            string energiValue = value.substr(secondSpacePos + 1);

            myListGweh.addToEnd(namaKegiatan, energiValue);
        }

        // TAMBAH: Add to front
        // Format: TAMBAH <nama> <energi>
        else if (action == "TAMBAH")
        {
            // Parse nama dan energi
            size_t secondSpacePos = value.find(' ');
            if (secondSpacePos == string::npos)
                continue;

            string namaKegiatan = value.substr(0, secondSpacePos);
            string energiValue = value.substr(secondSpacePos + 1);

            myListGweh.addToFront(namaKegiatan, energiValue);
        }

        // SISIPKAN: Add at specific index
        // Format: SISIPKAN <nama> <energi> <no>
        else if (action == "SISIPKAN")
        {
            // Parse nama, energi, dan index
            size_t firstSpacePos = value.find(' ');
            if (firstSpacePos == string::npos)
                continue;

            size_t secondSpacePos = value.find(' ', firstSpacePos + 1);
            if (secondSpacePos == string::npos)
                continue;

            string namaKegiatan = value.substr(0, firstSpacePos);
            string energiValue = value.substr(firstSpacePos + 1, secondSpacePos - firstSpacePos - 1);
            int index = stoi(value.substr(secondSpacePos + 1));

            myListGweh.insertAt(namaKegiatan, energiValue, index);
        }

        // HAPUS: Remove at specific index
        // Format: HAPUS <no>
        else if (action == "HAPUS")
        {
            int index = stoi(value);
            myListGweh.removeAt(myListGweh, index);
        }

        // HISTORI: Print all nodes
        else if (action == "HISTORI")
        {
            myListGweh.printAll();
        }

        // STATUS: Calculate total energy
        else if (action == "STATUS")
        {
            myListGweh.calculateTotalEnergy();
        }

        else if (action == "KELUAR")
        {
            break;
        }

        else
        {
            cout << "Perintah tidak dikenali!" << endl;
        }
    }

    return 0;
}
