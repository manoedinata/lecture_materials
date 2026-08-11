int cursor = 0;
char color = 0x07;

void putInMemory(int segment, int address, char character);
int getChar();

/*
 * Final Challenge
 *
 * Commands:
 * - check
 * - add <a> <b>
 * - sub <a> <b>
 * - fac <n>
 * - season <name>
 * - triangle <n>
 * - clear
 * - about
 *
 * Season list:
 * - winter
 * - spring
 * - summer
 * - fall
 * - radiant
 *
 * Restrictions:
 * - no stdlib
 * - avoid division (/)
 * - avoid modulo (%)
 */

 // Di-define duluan biar bisa dipanggil di fungsi lain
void clearScreen();

void newline() {
    int next_line = 0;
    while (next_line <= cursor) {
        next_line += 80;
    }
    cursor = next_line;
}

void printChar(char c) {
    int next_line;

    // Handle Enter/Newline
    if (c == '\n' || c == '\r') {
        next_line = 0;
        while (next_line <= cursor) {
            next_line += 80;
        }
        cursor = next_line;
    }
    // Handle Backspace
    else if (c == '\b') {
        if (cursor > 0) {
            cursor--;
            putInMemory(0xB800, cursor * 2, ' ');
            putInMemory(0xB800, (cursor * 2) + 1, color);
        }
    }
    // Handle standard characters
    else {
        putInMemory(0xB800, cursor * 2, c);
        putInMemory(0xB800, (cursor * 2) + 1, color);
        cursor++;
    }

    // Reset to top if we hit the bottom of the screen (80 cols * 25 rows = 2000)
    if (cursor >= 2000) {
        clearScreen();
    }
}

void printString(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        printChar(str[i]);
        i++;
    }
}

void clearScreen() {
    int i = 0;
    while (i < 2000) {
        // Multiply by 2 to hit the character byte, +1 for color byte
        putInMemory(0xB800, i * 2, ' ');
        putInMemory(0xB800, (i * 2) + 1, color);
        i++;
    }
    cursor = 0;
}

void readString(char* str) {
    int i = 0;
    char c = 0;

    while (1) {
        c = getChar(); // Wait for keypress

        if (c == '\r' || c == '\n') {
            str[i] = '\0'; // Null-terminate the string
            printChar('\n');
            break;
        }
        else if (c == '\b') {
            if (i > 0) {
                i--;
                printChar('\b'); // Visually erase the character
            }
        }
        else {
            str[i] = c;
            printChar(c);    // Echo the character to the screen
            i++;
        }
    }
}

int strcmp(char* str1, char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return 0; // Characters don't match
        }
        i++;
    }
    // If both strings ended at the exact same time, they are identical
    if (str1[i] == '\0' && str2[i] == '\0') {
        return 1;
    }
    return 0;
}

// 6. startsWith
int startsWith(char* str, char* prefix) {
    int i = 0;
    while (prefix[i] != '\0') {
        if (str[i] != prefix[i]) {
            return 0; // false
        }
        i++;
    }
    return 1; // true
}

// 7. atoi (ASCII to Integer)
int atoi(char* str) {
    int res = 0;
    int i = 0;
    int sign = 1;

    // Handle negative numbers just in case
    if (str[i] == '-') {
        sign = -1;
        i++;
    }

    // Convert characters to integer until we hit a space or null terminator
    while (str[i] >= '0' && str[i] <= '9') {
        res = (res * 10) + (str[i] - '0');
        i++;
    }
    return res * sign;
}

// 8. intToString (The "No Modulo/Division" implementation)
void intToString(int n, char* str) {
    char temp[16];
    int i = 0;
    int j = 0;
    int is_negative = 0;
    int current, quotient;

    if (n == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    if (n < 0) {
        is_negative = 1;
        n = -n;
    }

    while (n > 0) {
        current = n;
        quotient = 0;

        // Simulate: quotient = n / 10; current = n % 10;
        while (current >= 10) {
            current -= 10;
            quotient++;
        }

        temp[i++] = current + '0';
        n = quotient;
    }

    if (is_negative) {
        temp[i++] = '-';
    }

    // The string is currently backwards in temp array, reverse it into str
    while (i > 0) {
        i--;
        str[j++] = temp[i];
    }
    str[j] = '\0';
}

// 9. factorial
int factorial(int n) {
    int res = 1;
    int i = 1;

    if (n <= 1) {
        return 1;
    }

    while (i <= n) {
        res = res * i;
        i++;
    }

    return res;
}


void main() {

    char cmd[64];

    clearScreen();

    printString("Welcome to Assistant's Last Gift");
    newline();

    printString("type 'help'");
    newline();
    newline();

    while (1) {
        printString("> ");
        readString(cmd);

        /*
         * Command handlers
         */

        if (strcmp(cmd, "check")) {
            printString("ok");
        }
        else if (strcmp(cmd, "help")) {
            printString("Commands: check, add, sub, fac, season, triangle, clear, about");
        }
        else if (strcmp(cmd, "clear")) {
            clearScreen();
            continue; // Skip the newline after command execution
        }

        /*
        * Mathematical operations (add, sub, fac)
        */

        else if (startsWith(cmd, "add ")) {
            // 1. Declare ALL variables at the top of the block
            int i = 4;
            int a, b, res;
            char resStr[16];

            // 2. Then do the logic
            a = atoi(cmd + i);

            while (cmd[i] != ' ' && cmd[i] != '\0') {
                i++;
            }
            if (cmd[i] == ' ') i++;

            b = atoi(cmd + i);
            res = a + b;

            intToString(res, resStr);
            printString(resStr);
        }

        else if (startsWith(cmd, "sub ")) {
            int i = 4;
            int a, b, res;
            char resStr[16];

            a = atoi(cmd + i);

            while (cmd[i] != ' ' && cmd[i] != '\0') {
                i++;
            }
            if (cmd[i] == ' ') i++;

            b = atoi(cmd + i);
            res = a - b;

            intToString(res, resStr);
            printString(resStr);
        }

        /*
         * Factorial handler
        */
        else if (startsWith(cmd, "fac ")) {
            int i = 4;
            int n, res;
            char resStr[16];

            n = atoi(cmd + i);

            // 3. 16-bit limit check (7! = 5040, 8! = 40320 which overflows 32767)
            if (n > 7 || n < 0) {
                printString("know your limit little bro.");
            }
            else {
                // 4. Calculate and print using the new helper function
                res = factorial(n);

                intToString(res, resStr);
                printString(resStr);
            }
        }

        /*
         * Season handler (color mode)
        */
        else if (startsWith(cmd, "season ")) {
            int i = 7; // Skip the "season " prefix
            char* arg = cmd + i;

            if (strcmp(arg, "winter")) {
                color = 0x09; // Light Blue
                printString("winter mode");
            }
            else if (strcmp(arg, "spring")) {
                color = 0x0A; // Light Green
                printString("spring mode");
            }
            else if (strcmp(arg, "summer")) {
                color = 0x0E; // Yellow
                printString("summer mode");
            }
            else if (strcmp(arg, "fall")) {
                color = 0x06; // Brown
                printString("fall mode");
            }
            else if (strcmp(arg, "radiant")) {
                color = 0x0D; // Light Magenta
                printString("radiant mode");
            }
            else {
                printString("unknown season");
            }
        }

        /*
         * Triangle
        */
        else if (startsWith(cmd, "triangle ")) {
            // 1. Declare ALL variables at the top
            int i = 9; // Skip the "triangle " prefix
            int n, row, col;

            // 2. Parse the input
            n = atoi(cmd + i);

            // 3. Draw the triangle
            row = 1;
            while (row <= n) {
                col = 1;
                while (col <= row) {
                    printChar('x');
                    col++;
                }
                newline();
                row++;
            }
        }


        newline();

    }
}
