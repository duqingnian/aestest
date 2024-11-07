#include "dialog.h"
#include "./ui_dialog.h"

#include <QByteArray>
#include <QCryptographicHash>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}


// Encryption function
QString Dialog::aes256Encrypt(const QString &str)
{
    QByteArray data = str.toUtf8();
    QByteArray encryptedData;
    encryptedData.resize(data.size());

    // Create and initialize the context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        QMessageBox::critical(NULL,"error","Failed to create EVP_CIPHER_CTX");
        return QString();
    }

    int outlen;
    int num = 0;

    // Initialize the encryption operation with AES-256 CFB mode
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cfb128(), nullptr,
                                reinterpret_cast<const unsigned char *>(key.data()),
                                reinterpret_cast<const unsigned char *>(iv.data()))) {
        QMessageBox::critical(NULL,"error","EVP_EncryptInit_ex failed");
        EVP_CIPHER_CTX_free(ctx);
        return QString();
    }

    // Perform the encryption
    if (1 != EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char *>(encryptedData.data()),
                               &outlen, reinterpret_cast<const unsigned char *>(data.data()),
                               data.size())) {
        QMessageBox::critical(NULL,"error","EVP_EncryptUpdate failed");
        EVP_CIPHER_CTX_free(ctx);
        return QString();
    }

    // Finalize the encryption
    int tmplen;
    if (1 != EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(encryptedData.data()) + outlen, &tmplen)) {
        QMessageBox::critical(NULL,"error","EVP_EncryptFinal_ex failed");
        EVP_CIPHER_CTX_free(ctx);
        return QString();
    }

    outlen += tmplen;
    encryptedData.resize(outlen);

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    // Convert to Base64 for easy storage/display
    return QString::fromLatin1(encryptedData.toBase64());
}

// Decryption function
QString Dialog::aes256Decrypt(const QString &str)
{
    QByteArray encryptedData = QByteArray::fromBase64(str.toLatin1());
    QByteArray decryptedData;
    decryptedData.resize(encryptedData.size());

    // Create and initialize the context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        QMessageBox::critical(NULL,"error","Failed to create EVP_CIPHER_CTX");
        return QString();
    }

    int outlen;
    int num = 0;

    // Initialize the decryption operation with AES-256 CFB mode
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cfb128(), nullptr,
                                reinterpret_cast<const unsigned char *>(key.data()),
                                reinterpret_cast<const unsigned char *>(iv.data()))) {
        QMessageBox::critical(NULL,"error","EVP_DecryptInit_ex failed");
        EVP_CIPHER_CTX_free(ctx);
        return QString();
    }

    // Perform the decryption
    if (1 != EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char *>(decryptedData.data()),
                               &outlen, reinterpret_cast<const unsigned char *>(encryptedData.data()),
                               encryptedData.size())) {
        QMessageBox::critical(NULL,"error","EVP_DecryptUpdate failed");
        EVP_CIPHER_CTX_free(ctx);
        return QString();
    }

    // Finalize the decryption
    int tmplen;
    if (1 != EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(decryptedData.data()) + outlen, &tmplen)) {
        QMessageBox::critical(NULL,"error","EVP_DecryptFinal_ex failed");
        EVP_CIPHER_CTX_free(ctx);
        return QString();
    }

    outlen += tmplen;
    decryptedData.resize(outlen);

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    // Convert decrypted data back to QString
    return QString::fromUtf8(decryptedData);
}

// Slot for encode button
void Dialog::on_btn_encode_clicked()
{
    QString txt = ui->editA->toPlainText();
    if("" != txt)
    {
        ui->editB->setPlainText(this->aes256Encrypt(txt));
    }
}

// Slot for decode button
void Dialog::on_btn_decode_clicked()
{
    QString encoded_string = ui->editB->toPlainText();
    if (!encoded_string.isEmpty())
    {
        ui->editC->setPlainText(this->aes256Decrypt(encoded_string));
    }
}
