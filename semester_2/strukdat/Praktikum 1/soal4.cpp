#include <iostream>
#include <string>

using namespace std;

class Character
{
protected:
    string name;
    char grade;
    int hp;
    int attackPower;

public:
    // Initialize
    Character(string name, char grade, int hp, int attackPower)
    {
        this->name = name;
        this->grade = grade;
        this->hp = hp;
        this->attackPower = attackPower;
    }

    // Getter
    string getName() { return this->name; }
    char getGrade() { return this->grade; }
    int getHP() { return this->hp; }
    int getAttackPower() { return this->attackPower; }

    // Setter
    void setGrade(char grade) { this->grade = grade; }

    // Pure virtual function, to be overriden
    virtual void displayInfoo() = 0;
    virtual void useSkill() = 0;
};

class Scientist : public Character
{
private:
    int sciencePoint;

public:
    // Initialize + inherit
    Scientist(string name, char grade, int hp, int attackPower, int sciencePoint)
        : Character(name, grade, hp, attackPower)
    {
        this->sciencePoint = sciencePoint;
    }

    // Additional getter
    int getSciencePoint() { return this->sciencePoint; }

    // Override: Displayinfo
    void displayInfoo() override
    {
        cout << "Nama: " << this->getName() << endl;
        cout << "Grade: " << this->getGrade() << endl;
        cout << "HP: " << this->getHP() << endl;
        cout << "ATK PWR: " << this->getAttackPower() << endl;
        cout << "Science Point: " << this->getSciencePoint() << endl;
    }

    // Override: Use skill
    void useSkill() override
    {
        if (sciencePoint >= 40)
        {
            sciencePoint -= 40;
            cout << this->getName() << "meracik Sulfuric Acid Bomb! 10 Billion Percent effective!" << endl;
        }
        else
        {
            cout << "Science Point " << this->getName() << " tidak cukup untuk melakukan sintesis!" << endl;
        }
    }
};

class Warrior : public Character
{
private:
    int stamina;

public:
    // Initialize + inherit
    Warrior(string name, char grade, int hp, int attackPower, int stamina)
        : Character(name, grade, hp, attackPower)
    {
        this->stamina = stamina;
    }

    // Additional getter
    int getStamina() { return this->stamina; }

    // Override: Displayinfo
    void displayInfoo() override
    {
        cout << "Nama: " << this->getName() << endl;
        cout << "Grade: " << this->getGrade() << endl;
        cout << "HP: " << this->getHP() << endl;
        cout << "ATK PWR: " << this->getAttackPower() << endl;
        cout << "Stamina: " << this->getStamina() << endl;
    }

    // Override: Use skill
    void useSkill() override
    {
        if (stamina >= 25)
        {
            stamina -= 25;
            cout << this->getName() << " menghantam musuh dengan batu besar! Primitive but OVERWHELMING!" << endl;
        }
        else
        {
            cout << "Stamina " << this->getName() << " tidak cukup! Perlu istirahat dulu" << endl;
        }
    }
};

// CHARACTER INITIALIZATION
const int MAX_CHARS = 100;
Character *roster[MAX_CHARS];
int currCharCount = 0; // penanda skrg sampe posisi array mana

// Additional functions
int isCharExistByName(string nama)
{
    for (int i = 0; i < currCharCount; i++)
    {
        if (roster[i]->getName() == nama)
            return i;
    }

    return -1;
}

// infinite loop goes brrrrr
int main()
{
    cout << "HENDRA'S IMPACT BWAHAHAHAHAH" << endl;

    cout << "Menu:" << endl;
    cout << "1. Rekrut anggota baru" << endl;
    cout << "2. Lihat semua anggota" << endl;
    cout << "3. Promote anggota" << endl;
    cout << "4. Hapus anggota" << endl;
    cout << "5. Aktivasi skill anggota" << endl;
    cout << "6. Keluar" << endl;
    cout << endl;
    cout << "Masukkan input: ";

    int inputUser;
    cin >> inputUser;

    switch (inputUser)
    {
    // Create
    case 1:
    {
        if (currCharCount >= MAX_CHARS)
        {
            cout << "Anggota sudah batas maksimal. Tidak bisa menambah lagi." << endl;
            cout << endl;
        }

        cout << "[*] Merekrut anggota..." << endl;
        cout << endl;

        // 1 Scientist || 2 Warrior
        int pilihanTipe = 0;
        cout << "    1. Scientist" << endl;
        cout << "    2. Warrior" << endl;
        cout << endl;

        while (pilihanTipe != 1 && pilihanTipe != 2)
        {
            cout << "[?] Masukkan nomor pilihan: ";
            cin >> pilihanTipe;
        }

        string nama;
        cout << "[?] Masukkan nama: ";
        cin >> nama;

        char grade;
        while (grade != 'A' && grade != 'B' && grade != 'C' && grade != 'D')
        {
            cout << "[?] Masukkan grade (A / B / C / D): ";
            cin >> grade;
        }

        int hp;
        cout << "[?] Masukkan HP: ";
        cin >> hp;

        int attackPower;
        cout << "[?] Masukkan attackPower: ";
        cin >> attackPower;

        int additionalField;
        if (pilihanTipe == 1)
        {
            // Science point
            cout << "[?] Masukkan science point: ";
            cin >> additionalField;
        }
        else if (pilihanTipe == 2)
        {
            // Stamina
            cout << "[?] Masukkan stamina: ";
            cin >> additionalField;
        }

        // bikin
        if (pilihanTipe == 1)
        { // Scientist
            roster[currCharCount] = new Scientist(nama, grade, hp, attackPower, additionalField);
        }
        else if (pilihanTipe == 2)
        { // Warrior

            roster[currCharCount] = new Warrior(nama, grade, hp, attackPower, additionalField);
        }

        currCharCount += 1;

        cout << "Sukses merekrut anggota!" << endl;

        cout << endl;
        main();
        break;
    }

    // Read
    case 2:
    {
        if (currCharCount == 0)
        {
            cout << "Tidak ada anggota yang di-rekrut!" << endl;
        }
        else
        {

            for (int i = 0; i < currCharCount; i++)
            {
                // cout << roster[i]->getName() << endl;
                roster[i]->displayInfoo();
            }
        }

        cout << endl;
        main();
        break;
    }

    // Promote
    case 3:
    {
        // Cari nama doang sebanyak ini line nya...
        // adakah cara yg lebih efisien?
        string nama;
        int charCount = -1;

        while (charCount == -1)
        {
            cout << "[?] Masukkan nama karakter: ";
            cin >> nama;
            charCount = isCharExistByName(nama);
            if (charCount == -1)
                cout << "[!] Nama tidak ditemukan." << endl;
        }

        Character *targetCharacter = roster[charCount];

        if (targetCharacter->getGrade() == 'A')
        {
            cout << targetCharacter->getName() << " sudah A!!!";
        }
        else
        {

            char old_grade = targetCharacter->getGrade();
            targetCharacter->setGrade(
                old_grade - 1 // simple arithmethic, more like meth
            );

            cout << targetCharacter->getName() << " sudah di-promote dari " << old_grade << " ke " << targetCharacter->getGrade() << endl;
        }

        cout << endl;
        main();
        break;
    }

    // Hapus
    case 4:
    {
        // Cari nama doang sebanyak ini line nya...
        // adakah cara yg lebih efisien?
        string nama;
        int charCount = -1;

        while (charCount == -1)
        {
            cout << "[?] Masukkan nama karakter: ";
            cin >> nama;
            charCount = isCharExistByName(nama);
            if (charCount == -1)
                cout << "[!] Nama tidak ditemukan." << endl;
        }

        Character *targetCharacter = roster[charCount];

        cout << "Deleting character " << targetCharacter->getName() << "..." << endl
             << endl;

        // Hapus karakter dengan menggeser karakter setelahnya ke kiri
        for (int i = charCount; i < currCharCount - 1; i++)
        {
            roster[i] = roster[i + 1];
        }
        roster[currCharCount - 1] = nullptr; // Set last pointer to nullptr
        currCharCount--;

        cout << endl;
        main();
        break;
    }

    // Aktivasi skill
    case 5:
    {
        // Cari nama doang sebanyak ini line nya...
        // adakah cara yg lebih efisien?
        string nama;
        int charCount = -1;

        while (charCount == -1)
        {
            cout << "[?] Masukkan nama karakter: ";
            cin >> nama;
            charCount = isCharExistByName(nama);
            if (charCount == -1)
                cout << "[!] Nama tidak ditemukan." << endl;
        }

        Character *targetCharacter = roster[charCount];

        cout << "Mengaktifkan skill " << targetCharacter->getName() << endl;
        targetCharacter->useSkill();

        cout << endl;
        main();
        break;
    }

    // Exit
    case 6:
    {
        cout << "Senku: 'See you on the other side of science!'" << endl;

        cout << endl;
        break;
    }

    // Invalid input
    default:
    {
        cout << "Input tidak valid!" << endl;

        cout << endl;
        main();
        break;
    }
    }

    return 0;
}
