#include "cryptocontroller.h"
#include <openssl/evp.h>
#include <QFile>
#include <QIODevice>
#include <QObject>
#include <openssl/conf.h>
#include <QDebug>
#include <QClipboard>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QByteArray>

cryptoController::cryptoController(QObject *parent) : QObject(parent)
{

}

bool cryptoController::check_password(QString password) {

    EVP_CIPHER_CTX *ctx;
    if(!(ctx = EVP_CIPHER_CTX_new())){ //EVP_CIPHER_CTX_new() creates a cipher context.
        return 0;
    }

//    QString key = password + ((QString)"0").repeated(32 - password.length());

    iv = (unsigned char*) key.data();

    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cfb(), NULL, (unsigned char*) key.data(), iv))
    //EVP_EncryptInit_ex() sets up cipher context "ctx" for encryption with cipher "EVP_aes_256_cfb()"
    //from ENGINE "NULL" (If impl is NULL then the default implementation is used.)
    {
        return false;
    }

    unsigned char ciphertext[256] = {0};
    unsigned char plaintext[256] = {0};
    int len = 0, plaintext_len = password.length();

    memcpy(plaintext, password.toStdString().c_str(), password.size());

    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        /*
         EVP_EncryptUpdate() encrypts "plaintext_len" bytes from the buffer "plaintext"
            and writes the encrypted version to "ciphertext".
        */
    {
        return false;
    }

    QByteArray password_cipher_qbyte = (char*) ciphertext;
//    qDebug() << password_cipher_qbyte.toBase64();

    char buffer[256] = {0};
    QFile sourсe_file(":/password_crypt.txt");
    bool is_opened = sourсe_file.open(QIODevice::ReadOnly);
//    qDebug() << is_opened;
    sourсe_file.read(buffer, 256);

    sourсe_file.close();
    EVP_CIPHER_CTX_free(ctx);

    return QString(buffer) == password_cipher_qbyte.toBase64();
}

void cryptoController::decrypt_db_file(int record_id, int is_password)
{
    EVP_CIPHER_CTX *ctx;
    if(!(ctx = EVP_CIPHER_CTX_new())){ //EVP_CIPHER_CTX_new() creates a cipher context.
        emit sendMessageToQml("Произошла ошибка.");
        return;
    }

//    QString key = password + ((QString)"0").repeated(32 - password.length());

    iv = (unsigned char*) key.data();

    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cfb(), NULL, (unsigned char*) key.data(), iv))
    //EVP_EncryptInit_ex() sets up cipher context "ctx" for encryption with cipher "EVP_aes_256_cfb()"
    //from ENGINE "NULL" (If impl is NULL then the default implementation is used.)
    {
        return;
    }

    unsigned char ciphertext[256] = {0};
    unsigned char plaintext[256] = {0};
    int len = 0, plaintext_len = 0;
    QString db_decrypted = "";

    QFile sourсe_file(":/db_crypt.json");
    bool is_opened = sourсe_file.open(QIODevice::ReadOnly);
    if(!is_opened) {
        emit sendMessageToQml("Ошибка при открытии файла с учетными данными.");
        return;
    }

    plaintext_len = sourсe_file.read((char *)plaintext, 256);

    while(plaintext_len > 0){
        if(1 != EVP_DecryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        {
            emit sendMessageToQml("Произошла ошибка.");
            return;
        }

        QByteArray ciphertext_qbyte = QByteArray(((char *) ciphertext), len);
        db_decrypted += ciphertext_qbyte;

        plaintext_len = sourсe_file.read((char *)plaintext, 256);
    }

    EVP_CIPHER_CTX_free(ctx);

    if(record_id != -1 && is_password != -1)
        get_login_and_password(db_decrypted, record_id, is_password);
    else
        emit sendDbToQml(db_decrypted);
}

void cryptoController::get_login_and_password(QString db_decrypted, int record_id, int is_password)
{
    QJsonDocument jsonResponse = QJsonDocument::fromJson(db_decrypted.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();

//    qDebug() << jsonObject.value("sites").type();

    if((jsonObject.value("sites")).type() == QJsonValue::Array) {
        QJsonArray qjsonarray = jsonObject.value("sites").toArray();
        QJsonValue val = is_password ? qjsonarray.at(record_id).toObject().value("password") : qjsonarray.at(record_id).toObject().value("login");

//        decryptData(val.toString());
//        qDebug() << (qjsonarray.at(record_id)).type();
//        qDebug() << val.toString();

        QByteArray cipher_data = QByteArray::fromBase64(val.toString().toUtf8());

        EVP_CIPHER_CTX *ctx;
        if(!(ctx = EVP_CIPHER_CTX_new())){ //EVP_CIPHER_CTX_new() creates a cipher context.
            emit sendMessageToQml("Произошла ошибка.");
            return;
        }

        iv = (unsigned char*) key.data();
        if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cfb(), NULL, (unsigned char*) key.data(), iv))
        {
            emit sendMessageToQml("Произошла ошибка.");
            return;
        }

        unsigned char ciphertext[256] = {0};
        unsigned char plaintext[256] = {0};
        int len = 0;
        memcpy(plaintext, cipher_data.toStdString().c_str(), cipher_data.size());

        if(1 != EVP_DecryptUpdate(ctx, ciphertext, &len, plaintext, cipher_data.size()))
        {
            emit sendMessageToQml("Произошла ошибка.");
            return;
        }

        QByteArray ciphertext_qbyte = QByteArray(((char *) ciphertext), len);
//        qDebug() << ciphertext_qbyte;

        QClipboard* clp = QGuiApplication::clipboard();
        clp->setText(ciphertext_qbyte);
        emit sendMessageToQml("Скопировано в буфер обмена.");

    } else return;
}


