#ifndef CV_CONTROLLER_H
#define CV_CONTROLLER_H

#include <QObject>
#include <QImage>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/face.hpp>

class cv_controller : public QObject
{
    Q_OBJECT

private:
    cv::CascadeClassifier face_cascade;

public:
    explicit cv_controller(QObject *parent = nullptr);

public slots:
    void face_recognition(const QImage & qimage);

signals:
    void sendMessageToQml(QString message);
    void sendUserInfoToQml(QString _username, int _confidence);
    void userIdentified();

};

#endif // CV_CONTROLLER_H
