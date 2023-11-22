/*#include <iostream>
#include <string>
#include <fstream>

using namespace std;

typedef unsigned __int16 WORD;
#pragma pack(push, 1)
typedef struct {
    WORD   bfType;         // 0x4d42 | 0x4349 | 0x5450
    int    bfSize;         // размер файла
    int    bfReserved;     // 0
    int    bfOffBits;      // смещение до поля данных,
    int    biSize;         // размер струкуры в байтах:
    int    biWidth;        // ширина в точках
    int    biHeight;       // высота в точках
    WORD   biPlanes;       // всегда должно быть 1
    WORD   biBitCount;     // 0 | 1 | 4 | 8 | 16 | 24 | 32
    int    biCompression;  // BI_RGB | BI_RLE8 | BI_RLE4 |
    int    biSizeImage;    // Количество байт в поле данных
    int    biXPelsPerMeter;// горизонтальное разрешение, точек на дюйм
    int    biYPelsPerMeter;// вертикальное разрешение, точек на дюйм
    int    biClrUsed;      // Количество используемых цветов
    int    biClrImportant; // Количество существенных цветов.
} BMPheader;
#pragma pack(pop)

int main()
{
    setlocale(LC_ALL, "rus");

    string fileInput;
    string fileOutput;

    cout << "Напишите путь к файлу с которго нужно считать это: ";
    getline(cin, fileInput);
    cout << "Напишите путь к файлу в который нужно записать: ";
    getline(cin, fileOutput);

    ifstream inputFile("C:\\Users\\ASUS\\Desktop\\Таск\\C++\\Task 3\\x64\\Debug\\" + fileInput + ".bmp");

    if (!inputFile.is_open())
    {
        cout << "Ошибка! Мы не смогли прочитать BMP";
        return -1;
    }

    BMPheader bmpheader;
    inputFile.read(reinterpret_cast<char*>(&bmpheader), sizeof(BMPheader));

    ofstream outputFile ("C:\\Users\\ASUS\\Desktop\\Таск\\C++\\Task 3\\x64\\Debug\\" + fileOutput + ".txt");
   
    if (!ofstream()) {
        cout << "Ошибка! Мы не смогли прочитать TXT фаил";
        return 1;
    }

    outputFile << "Размер файла " << bmpheader.biSize << endl;
    outputFile << "Ширина изображения: " << bmpheader.biWidth << endl;
    outputFile << "Высота изображения: " << bmpheader.biHeight << endl;
    outputFile << "Количество цветов: " << bmpheader.biClrUsed << endl;

    inputFile.close();
    outputFile.close();

    return 0;
}
*/

/*
    Написать в 4 такске не одну структуру а несколько также обединение
*/


#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <seccomp.h>
#include <syscall.h>

char spy_msg[0x100];

void __attribute__((constructor)) seccomp()
{
    alarm(5);

    scmp_filter_ctx ctx;
    ctx = seccomp_init(SCMP_ACT_KILL);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, __NR_read, 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, __NR_write, 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, __NR_exit_group, 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, __NR_newfstatat, 1, SCMP_A0(SCMP_CMP_EQ, 0));
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, __NR_lseek, 1, SCMP_A0(SCMP_CMP_EQ, 0));
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, __NR_openat, 1, SCMP_A2(SCMP_CMP_EQ, 0));
    seccomp_load(ctx);
}

void setup() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    strcpy(spy_msg, "-- . --. .- .-.. .. - .... / ... . .-. ...- . .-. / .. ... / .-.. --- -.-. .- - . -.. / .. -. / - .... . / ... . -.-. --- -. -.. / ... . -.-. - --- .-. / --- ..-. / -- . - .-. .- / ...- . . -.- .... .. -- --..-- / .---- -....- ...-- .-.-.- ..---");
}

void menu() {
    printf("Type 1 to send a message\nType 2 to get a message from CVR\nType 3 to read a message\nChoice: ");
}

int read_option() {
    int x = 0;
    scanf("%d", &x);
    return x;
}

void spy_write() {
    printf("Enter text of the message: ");
    unsigned long long text = 0;
    scanf("%llx", &text);
    printf("Enter destination of the message: ");
    unsigned long long destination = 0;
    scanf("%llx", &destination);
    *(unsigned long long*)(destination) = text;
    puts("I've sent your message!");
}

void spy_open() {
    int fd = open("/flag.txt", O_RDONLY);
    read(fd, spy_msg, 0x100);
    *strchrnul(spy_msg, '\n') = '\0';
    puts("I've got the message from CVR!");
}

void spy_print() {
    puts(spy_msg);
}

void incorrect_option() {
    puts("I can't do that");
}

int main() {
    setup();
    menu();
    int option = read_option();

    if (option == 1) {
        spy_write();
    }
    else if (option == 2) {
        spy_open();
    }
    else if (option == 3) {
        spy_print();
    }
    else {
        incorrect_option();
    }
}
