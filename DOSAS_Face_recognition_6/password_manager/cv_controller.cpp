#include "cv_controller.h"
#include <QObject>
#include <QDebug>
#include <QString>
#include <QFile>
#include <vector>
#include <stdio.h>
#include <string>
#include "drawLandmarks.hpp"
#include <fstream>
#include <QThread>

using namespace std;
using namespace cv;
using namespace cv::face;

cv_controller::cv_controller(QObject *parent)
    : QObject{parent}
{

}

void cv_controller::face_recognition(const QImage & qimage)
{
    Mat frame(qimage.height(), qimage.width(), QImage::Format_Grayscale8, (uchar*)qimage.bits(), qimage.bytesPerLine());
//    imshow("mat" , frame);

    Ptr<FaceRecognizer>  model = LBPHFaceRecognizer::create(1,8,8,8);
    model->read("C:\\opencv\\faces.yml");

    int img_width = 200;
    int img_height = 200;

    if (!face_cascade.load("C:\\opencv\\opencv_bin\\install\\etc\\haarcascades\\haarcascade_frontalface_alt.xml")) {
        qDebug() << " Error loading file";
        return;
    }

    vector<Rect> faces;
    Mat graySacleFrame;
    Mat original;

    if (!frame.empty()) {
        original = frame.clone();

        cvtColor(original, graySacleFrame, COLOR_BGR2GRAY);
//        equalizeHist(graySacleFrame, graySacleFrame);

        face_cascade.detectMultiScale(graySacleFrame, faces);

        fstream db_file;
        if (!faces.size() == 0) {
            db_file.open("C:\\opencv\\db.txt");
        } else {
            emit sendUserInfoToQml("-", -1);
            emit sendMessageToQml("No faces found!");
        }

        for (int i = 0; i < faces.size(); i++) {
            //region of interest
            Rect face_i = faces[i];

            //crop the roi from gray image
            Mat face = graySacleFrame(face_i);

            //resizing the cropped image to suit to database image sizes
            Mat face_resized;
            cv::resize(face, face_resized, Size(img_width, img_height));

            //recognizing what faces detected
            int label = -1; double confidence = 0;
            string name;
            model->predict(face_resized, label, confidence);

            string line;
            while(getline(db_file, line)) {
                int pos = line.find(to_string(label) + "/", 0);

                if (pos != string::npos) {
                    int pos2 = line.find("\n", pos);
                    name = line.substr(to_string(label).length() + 1, pos2 - (to_string(label).length() + 1 + 1));
                    break;
                }
            }

            qDebug() << " confidence " << confidence << " Label: " << label <<  " Name: " << QString::fromStdString(name);
            emit sendUserInfoToQml(QString::fromStdString(name), confidence);
            if((int)confidence <= 60) {
                emit userIdentified();
            }
            else {
                emit sendMessageToQml("User is not identified");
            }
        }

    }
}

