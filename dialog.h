#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_btn_encode_clicked();

    void on_btn_decode_clicked();

private:
    Ui::Dialog *ui;

    QByteArray key = "12345678901234567890123456789012";
    QByteArray iv = "1234567890123456";

    QString aes256Encrypt(const QString &str);
    QString aes256Decrypt(const QString &str);
};
#endif // DIALOG_H
