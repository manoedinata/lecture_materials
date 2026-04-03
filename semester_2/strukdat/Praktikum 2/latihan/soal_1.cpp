// RKAA - One More Node, One More Chance
// double linked list untuk stasiun kereta Takaki

#include <iostream>
#include <string>

using namespace std;

class Node
{
public:
    string data;
    Node *next;
    Node *prev;

    Node(string data)
    {
        this->data = data;
        this->next = nullptr;
        this->prev = nullptr;
    }
};

class KeretaTakaki
{
public:
    Node *head;
    Node *current;

    KeretaTakaki()
    {
        head = nullptr;
        current = nullptr;
    }

    void addStation(string stationName)
    {
        Node *newNode = new Node(stationName);
        if (head == nullptr)
        {
            head = newNode;
            current = head; // Jika stasiun pertama, Takaki otomatis di sini
        }

        else
        {
            Node *temp = head;
            while (temp->next != nullptr)
            {
                temp = temp->next;
            }
            temp->next = newNode;
            newNode->prev = temp;
        }
    }

    void insertEmergency(int index, string stationName)
    {
        // Hitung jumlah stasiun saat ini
        int count = 0;
        Node *temp = head;
        while (temp != nullptr)
        {
            count++;
            temp = temp->next;
        }

        // Abaikan jika indeks tidak valid
        if (index < 0 || index > count)
            return;

        Node *newNode = new Node(stationName);

        // Insert di depan
        if (index == 0)
        {
            if (head == nullptr)
            {
                head = newNode;
                current = head; // Jika stasiun pertama, Takaki otomatis di sini
            }
            else
            {
                newNode->next = head;
                head->prev = newNode;
                head = newNode;
            }
        }

        // Insert di tengah atau akhir
        else
        {
            temp = head;
            for (int j = 0; j < index - 1; j++)
            {
                temp = temp->next;
            }
            newNode->next = temp->next;
            newNode->prev = temp;
            if (temp->next != nullptr)
            {
                temp->next->prev = newNode;
            }
            temp->next = newNode;
        }
    }

    void removeStation(string stationName)
    {
        Node *temp = head;
        while (temp != nullptr)
        {
            if (temp->data == stationName)
            {
                // Cek apakah stasiun yang dihapus adalah stasiun tempat Takaki berada
                if (temp == current)
                {
                    if (current->next != nullptr)
                    {
                        current = current->next;
                    }
                    else
                    {
                        current = current->prev;
                    }
                }

                // Putus koneksi Node dari list
                if (temp->prev != nullptr)
                {
                    temp->prev->next = temp->next;
                }
                else
                {
                    head = temp->next; // Update head jika yang dihapus node pertama
                }

                if (temp->next != nullptr)
                {
                    temp->next->prev = temp->prev;
                }

                delete temp;
                break; // Selesai menghapus, keluar dari loop
            }
            temp = temp->next;
        }
    }

    void moveForward()
    {
        if (current == nullptr || current->next == nullptr)
        {
            cout << "Kereta tidak bisa bergerak, salju terlalu tebal!\n";
        }
        else
        {
            current = current->next; // Geser Takaki ke depan
        }
    }

    void moveBackward()
    {
        if (current == nullptr || current->prev == nullptr)
        {
            cout << "Kereta tidak bisa bergerak, salju terlalu tebal!\n";
        }
        else
        {
            current = current->prev; // Geser Takaki ke belakang
        }
    }

    void whereAmI()
    {
        if (current == nullptr)
        {
            cout << "Takaki tersesat di tengah badai salju...\n";
        }
        else
        {
            cout << "Takaki sekarang berada di: " << current->data << "\n";
        }
    }
};

int main()
{
    KeretaTakaki kereta;

    int n;
    if (!(cin >> n))
        return 0;
    cin.ignore(); // Ignore newline after reading n

    for (int i = 0; i < n; i++)
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

        if (action == "ADD_STATION")
        {
            kereta.addStation(value);
        }

        else if (action == "INSERT_EMERGENCY")
        {
            size_t secondSpacePos = value.find(' ');
            if (secondSpacePos == string::npos)
                continue;

            int index = stoi(value.substr(0, secondSpacePos));
            string stationName = value.substr(secondSpacePos + 1);

            kereta.insertEmergency(index, stationName);
        }

        else if (action == "REMOVE_STATION")
        {
            kereta.removeStation(value);
        }

        else if (action == "MOVE_FORWARD")
        {
            kereta.moveForward();
        }

        else if (action == "MOVE_BACKWARD")
        {
            kereta.moveBackward();
        }

        else if (action == "WHERE_AM_I")
        {
            kereta.whereAmI();
        }
    }

    return 0;
}
