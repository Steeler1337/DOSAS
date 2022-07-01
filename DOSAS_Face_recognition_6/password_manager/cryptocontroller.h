#ifndef CRYPTOCONTROLLER_H
#define CRYPTOCONTROLLER_H

#include <QObject>
#include <QString>

class cryptoController : public QObject
{
    Q_OBJECT
public:
    explicit cryptoController(QObject *parent = nullptr);
    ~cryptoController() {};

public slots:
    bool check_password(QString password);
    void decrypt_db_file(int record_id=-1, int is_password=-1);

private:
    unsigned char * iv = (unsigned char *)("77411230141456889873614547987324");
    void get_login_and_password(QString db_decrypted, int record_id, int is_password);
    QString key = "qwerty12300000000000000000000000";

signals:
    // Сигнал для передачи данных в qml-интерфейс
    void sendDbToQml(QString db_decrypted);
    void sendMessageToQml(QString message);

};

#endif // CRYPTOCONTROLLER_H
