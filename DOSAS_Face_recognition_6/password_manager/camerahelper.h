#ifndef CAMERAHELPER_H
#define CAMERAHELPER_H

#include <QObject>
#include <QCamera>
#include <QCameraImageCapture>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

class CameraHelper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* qcamera READ qcamera WRITE setQcamera NOTIFY qcameraChanged)
    using QObject::QObject;

public:
    explicit CameraHelper(QObject *parent = nullptr);

    QObject *qcamera() const{
        return q_qcamera;
    }
    void setQcamera(QObject *qcamera)
    {
        if (q_qcamera == qcamera)
            return;
        if(m_capture){
            m_capture->deleteLater();
        }
        q_qcamera = qcamera;
        if(q_qcamera){
            if(QCamera *camera = qvariant_cast<QCamera *>(q_qcamera->property("mediaObject"))){
                m_capture = new QCameraImageCapture(camera, this);
                m_capture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
                connect(m_capture, &QCameraImageCapture::imageCaptured, this, &CameraHelper::handleImageCaptured);
            }
            else
                m_capture = nullptr;
        }
        emit qcameraChanged();
    }

public slots:
    void capture(){
        if(m_capture)
            m_capture->capture();
    }

signals:
    void qcameraChanged();
    void imageChanged(const QImage & image);

private:
    void handleImageCaptured(int , const QImage &preview){
        emit imageChanged(preview);
    }
    QPointer<QObject> q_qcamera;
    QCameraImageCapture *m_capture = nullptr;
};

#endif // CAMERAHELPER_H
