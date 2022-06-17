#include "cryptocontroller.h"
#include <QString>
#include <openssl/evp.h>
#include <QFile>
#include <QIODevice>
#include <QObject>
#include <openssl/conf.h>
#include <QQmlContext>
#include <QDebug>
#include <QClipboard>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QByteArray>
#include <QProcess>
#include <QClipboard>
#include <atlstr.h>
#include <Windows.h>
#include <intrin.h>
#include <iostream>
#include <string.h>

using namespace std;

cryptoController::cryptoController(QObject *parent) : QObject(parent)
{

}

//функция, заносит текст в буфер обмена
//IN: текст; pin
void cryptoController::text_to_clipboard(QString text, QString pin) {
    text = decrypt_login_or_password(text, pin);//расшифровываем текст
    QClipboard *clipboard = QGuiApplication::clipboard();//инициализируем буфер
    QString originalText = clipboard->text();
    clipboard->setText(text);//передаем в буфер текст
}

// функция для шифрования паролей и логинов: на IN поступает пароль password (пин-код), ф-ия вызывается иускусственно для зашифровки пароля
// out.txt - файл, в который записываем вручную текст до выполнения функции
// in.txt - файл, куда записываются зашифрованные данные после выполнения фукнции
void cryptoController::encrypt_login_or_password(QString password) {
    EVP_CIPHER_CTX *ctx;
    if(!(ctx = EVP_CIPHER_CTX_new())){
        return;
    }

    //пароль расширяется до ключа из 32 символов
    QString key = password + ((QString)"0").repeated(32 - password.length());

    //инициализирующий вектор для шифра
    iv = (unsigned char*) key.data();

    //инициализация шифра
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cfb(), NULL, (unsigned char*) key.data(), iv))
    {
        return;
    }

    unsigned char ciphertext[256] = {0};//зашифрованные данные
    unsigned char plaintext[256] = {0};//открытый текст
    int len = 0, plaintext_len = 256;//длина блока расшифрования

    //файл, в который записываем вручную текст
    QFile out("C:\\Lab_2\\out.txt");
    bool is_opened = out.open(QIODevice::ReadOnly);
    if(!is_opened) {
        return;
    }

    //читаем файл порционно, по 256 символов, записываем текст в plaintext,
    //переменная plaintext_len хранит число полученных символов
    plaintext_len = out.read((char *)plaintext, 256);

    //файл, куда записываются зашифрованные данные
    QFile in("C:\\Lab_2\\in.txt");
    in.open(QIODevice::WriteOnly);

    QByteArray ciphertext_qbyte;//промежуточное звено, которое будет хранить в себе зашифрованные

    //зашифрование текста
    while(plaintext_len > 0){
        if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) {
            return;
        }

        ciphertext_qbyte = QByteArray(((char *) ciphertext), len);//преобразование из char в QByteArray
        in.write(ciphertext_qbyte.toBase64());//переводим в формат Base64 и записываем в файл
        plaintext_len = out.read((char *)plaintext, 256);//берем следующие 256 символов
    }

    //закрываем оба файла
    in.close();
    out.close();

    EVP_CIPHER_CTX_free(ctx);

}

//функция, к которой мы обращаемся, когда нажимаем на логин или пароль для копирования в буфер обмена
//расшифровывает и возвращает логин или пароль
//IN: логин или пароль; ПИН
QString cryptoController::decrypt_login_or_password(QString text, QString password) {
    EVP_CIPHER_CTX *ctx;
    if(!(ctx = EVP_CIPHER_CTX_new())){ //EVP_CIPHER_CTX_new() creates a cipher context.
        return "";
    }

    //пароль расширяется до ключа из 32 символов
    QString key = password + ((QString)"0").repeated(32 - password.length());

    //инициализирующий вектор для шифра
    iv = (unsigned char*) key.data();

    //инициализация шифра
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cfb(), NULL, (unsigned char*) key.data(), iv))
    {
        return "";
    }

    unsigned char deciphertext[256] = {0};//расшифрованный текст
    unsigned char ciphertext[256] = {0};//открытый текст
    int len = 0, ciphertext_len = 256;//длина блока расшифрования


    QByteArray line = QByteArray::fromBase64(text.toUtf8()); // из base64 переводим в формат QByteArray, поэтому в line теперь хранятся нечитаемые символы
    int s = line.size();//получаем длину строки
    //если длина строки меньше 256 символов, то присваеваем длине блока длину строки
    if (s < 256)
        ciphertext_len = s;
    memcpy(ciphertext, line.toStdString().c_str(), ciphertext_len);//записываем в plaintext строку


    //QFile file("C:\\Lab_2\\out.txt");
    //file.open(QIODevice::WriteOnly);
    QByteArray deciphertext_qbyte;//переменная, которая будет хранить в себе расшифрованный текст в формате QByteArray

    //расшифрование текста
    while(s > 0){
        if(1 != EVP_DecryptUpdate(ctx, deciphertext, &len, ciphertext, ciphertext_len))
        {
            return "";
        }

        //перевод из char в QByteArray
        deciphertext_qbyte = QByteArray(((char *) deciphertext), len);
        //file.write(ciphertext_qbyte);

        line.remove(0, 256);
        s-=256;
        if (s<0)
            break;
        if (s<256)
            ciphertext_len=s;
        memcpy(ciphertext, line.toStdString().c_str(), ciphertext_len);
    }

    //file.close();



    EVP_CIPHER_CTX_free(ctx);

    return deciphertext_qbyte;
}


//функция проверяет введенный в первое окно пин-код
bool cryptoController::check_password(QString password) {
    //encrypt_db_file("1568"); // второй слой
    //encrypt_login_or_password("1568"); // первый слой
    EVP_CIPHER_CTX *ctx;
    if(!(ctx = EVP_CIPHER_CTX_new())){
        return 0;
    }

    QString key = password + ((QString)"0").repeated(32 - password.length()); // берется пинкод, к нему до 32 символов добавляются нули, получается ключ

    iv = (unsigned char*) key.data(); // инициализирующий вектор, состоит также из 32 символов (особенность AES)

    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cfb(), NULL, (unsigned char*) key.data(), iv)) //инициализация шифра
    {
        return false;
    }

    unsigned char ciphertext[256] = {0}; // переменные зашифр. и чистого текста по стндарту AES
    unsigned char plaintext[256] = {0};
    int len = 0, plaintext_len = password.length();

    memcpy(plaintext, password.toStdString().c_str(), password.size()); // перекладываем из переменной password в plaintext для шифра


    //шифруем введенный ПИН
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) // ctx - объект шифра
    {
        return false;
    }

    QString password_cipher = (char *)ciphertext; // из char зашифрованный ПИН переводится в QString
    QByteArray password_cipher_qbyte = (char*) ciphertext; // из char зишифрованный ПИН переводится в QByteArray

    char buffer[256] = {0}; // буфер, который будет использоваться для хранения взятого из password_crypt.txt зашифрованного пароля
    QFile source_file("C:\\Lab_2\\password_crypt.txt");
    bool is_opened = source_file.open(QIODevice::ReadOnly);
    //source_file.write(password_cipher_qbyte.toBase64());
    source_file.read(buffer, 256); // считывание в буфер


    source_file.close(); // закрываем файл
    EVP_CIPHER_CTX_free(ctx); // "уничтожаем" объект шифра

    // сравнение toBase64 шифра из файла и введенного шифра
    return QString(buffer) == password_cipher_qbyte.toBase64();
}

//расшифрование db_crypt.json файла
void cryptoController::decrypt_db_file(QString password, int record_id, int is_password) // record_id и is_password - константы из qml
{
        EVP_CIPHER_CTX *ctx;
        if(!(ctx = EVP_CIPHER_CTX_new())){
            return;
        }

        //пароль расширяется до ключа из 32 символов
        QString key = password + ((QString)"0").repeated(32 - password.length());

        //инициализируем вектор
        iv = (unsigned char*) key.data();

        //инициализация шифра
        if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cfb(), NULL, (unsigned char*) key.data(), iv))
        {
            return;
        }

        unsigned char deciphertext[256] = {0};//расшифрованный текст
        unsigned char ciphertext[256] = {0};//зашифрованный текст
        int len = 0, ciphertext_len = 0;//длина блока расшифрования
        QString db_decrypted = "";// здесь будет расшифрованный текст
        QFile source_file(":/db_crypt.json");
        bool is_opened = source_file.open(QIODevice::ReadOnly);
        if(!is_opened) {
            return;
        }

        //читаем файл порционно, по 256 символов, записываем в db_decrypted,
        ciphertext_len = source_file.read((char *)ciphertext, 256); // число полученных символов

        //расшифрование текста
        while(ciphertext_len > 0){
            if(1 != EVP_DecryptUpdate(ctx, deciphertext, &len, ciphertext, ciphertext_len))
            {
                return;
            }

            QByteArray deciphertext_qbyte = QByteArray(((char *) deciphertext), len);
            db_decrypted += deciphertext_qbyte;

            ciphertext_len = source_file.read((char *)ciphertext, 256);
        }

        EVP_CIPHER_CTX_free(ctx);

        if(is_password == -1)
            emit sendDbToQml(db_decrypted); // передача в QML расшифрованной БД
}

void cryptoController::encrypt_db_file(QString password, int record_id, int is_password)
{

    EVP_CIPHER_CTX *ctx;
        if(!(ctx = EVP_CIPHER_CTX_new())){ //EVP_CIPHER_CTX_new() creates a cipher context.
            return;
        }

        //пароль расширяется до ключа из 32 символов
        QString key = password + ((QString)"0").repeated(32 - password.length());

        //инциализируем вектор
        iv = (unsigned char*) key.data();

        //зашифрование данных
        if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cfb(), NULL, (unsigned char*) key.data(), iv))
        {
            return;
        }

        unsigned char ciphertext[256] = {0};//зашифрованный текст
        unsigned char plaintext[256] = {0};//открытый текст
        int len = 0, plaintext_len = 0;//длина блока зашифрования

        QFile source_file(":/db.json");
        bool is_opened = source_file.open(QIODevice::ReadOnly);
        if(!is_opened) {
            return;
        }

        //читаем файл порционно, по 256 символов, записываем текст в plaintext,
        //переменная plaintext_len хранит число полученных символов
        plaintext_len = source_file.read((char *)plaintext, 256);

        QFile file("C:\\Lab_2\\db_crypt.json");
        file.open(QIODevice::WriteOnly);

        //зашифрование данных
        while(plaintext_len > 0){
            if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
            {
                return;
            }

            QByteArray ciphertext_qbyte = QByteArray(((char *) ciphertext), len);

            file.write((char *)ciphertext, len);
            plaintext_len = source_file.read((char *)plaintext, 256);
        }

        //file.close(); //на случай использования encrypt_db_file("1568")

        EVP_CIPHER_CTX_free(ctx);
}
