#include <iostream>
#include <string>

#include "opencv2\core.hpp"
#include "opencv2\face.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\objdetect.hpp"
#include "opencv2\opencv.hpp"
#include<direct.h>

//file handling
#include <fstream>
#include <sstream>

using namespace std;
using namespace cv;
using namespace cv::face;

//Создание объекта класса CascadeClassifier для обнаружения объектов
CascadeClassifier face_cascade;

string filename;
string directory_name;
string name;
int id;
int filenumber = 0;
bool facesFound = true;

void detectAndDisplay(Mat frame)
{
	vector<Rect> faces;
	Mat frame_gray;
	Mat crop;
	Mat res;
	string text;
	stringstream sstm;

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);

	face_cascade.detectMultiScale(frame_gray, faces);

	Rect roi_b;

	size_t ic = 0;
	int ac = 0;

	size_t ib = 0;
	int ab = 0;

	if (faces.size() == 0) {
		facesFound = false;
		cout << "No faces found." << endl;
	}

	for (ic = 0; ic < faces.size(); ic++)
	{
		roi_b.x = faces[ib].x;
		roi_b.y = faces[ib].y;
		roi_b.width = (faces[ib].width);
		roi_b.height = (faces[ib].height);

		crop = frame(roi_b);

		resize(crop, res, Size(200, 200));
		cvtColor(res, res, COLOR_BGR2GRAY);

		stringstream ssfn;
		filename = "C:\\opencv\\Faces\\";
		ssfn << filename.c_str() << name << filenumber << ".jpg";
		filename = ssfn.str();
		imwrite(filename, res);
		filenumber++;
	}

}

void addFace()
{
	cout << "\nEnter Your Name:  ";
	cin >> name;
	cout << "\nEnter your id:  ";
	cin >> id;
	cout << "\nEnter the directory name in C:\\opencv\\faces\\";
	cin >> directory_name;
	filename = "C:\\opencv\\faces\\" + directory_name;

    if (!face_cascade.load("C:\\opencv\\opencv_bin\\install\\etc\\haarcascades\\haarcascade_frontalface_alt.xml"))
    {
        cout << "error" << endl;
        return ;
    };

	vector<cv::String> fn;
	glob(filename, fn, false);
	size_t count = fn.size(); 

    Mat frame;

    for (size_t i = 0; i < count; i++)
    {
		cout << fn[i] << endl;
		frame = imread(fn[i]);

		detectAndDisplay(frame);
    }

    return;
}

static void dbread(vector<Mat>& images, vector<int>& labels) {
	vector<cv::String> fn;
	filename = "C:\\opencv\\faces\\" + name + "*.jpg";
	glob(filename, fn, false);
	for (int i = 0; i < fn.size(); i++) {
		cout << fn[i] << endl;
	}
	
	size_t count = fn.size();

	for (size_t i = 0; i < count; i++)
	{
		Mat frame_i = imread(fn[i], 0);
		images.push_back(frame_i);
		labels.push_back(id);
	}
}

void FaceTrainer() {
	vector<Mat> images;
	vector<int> labels;
	dbread(images, labels);
    cout << "size of the images is " << images.size() << endl;
	cout << "size of the labels is " << labels.size() << endl;
	cout << "Training begins...." << endl;

	Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create(1,8,8,8);

	try
	{
		model->read("C:\\opencv\\faces.yml");
	}
	catch (const std::exception& ex)
	{
		cout << "Model is empty.";
	}

	try
	{
		model->update(images, labels);
	}
	catch (const cv::Exception& ex)
	{
		cout << ex.what() << endl;
		cout << ex.err << endl;
		cout << ex.code << endl;
		return;
	}


	try
	{
		model->write("C:\\opencv\\faces.yml");
	}
	catch (const cv::Exception& ex)
	{
		cout << ex.what() << endl;
		cout << ex.err << endl;
		cout << ex.code << endl;
		return;
	}

	cout << "Training finished...." << endl;  
	fstream fs;
	fs.open("C:\\opencv\\db.txt", ios_base::app);
	fs << id << "/" << name << "\n";
	fs.close();
	waitKey(5000);
}

void  FaceRecognition() {

	cout << "start recognizing..." << endl;

	//load pre-trained data sets
	Ptr<FaceRecognizer>  model = LBPHFaceRecognizer::create();
	model->read("C:\\opencv\\faces.yml");
	
	int img_width = 150;
	int img_height = 150;


	string window = "Capture - face detection";

	if (!face_cascade.load("C:\\opencv\\opencv_bin\\install\\etc\\haarcascades\\haarcascade_frontalface_alt.xml")) {
		cout << " Error loading file" << endl;
		return;
	}

	VideoCapture cap(1);

	if (!cap.isOpened())
	{
		cout << "exit" << endl;
		return;
	}

	namedWindow(window, 1);
	long count = 0;
	string Pname= "";

	while (true)
	{
		vector<Rect> faces;
		Mat frame;
		Mat graySacleFrame;
		Mat original;

		cap >> frame;
		//cap.read(frame);
		count = count + 1;//count frames;

		if (!frame.empty()) {
			original = frame.clone();

			//конвертация картинки в серый
			cvtColor(original, graySacleFrame, COLOR_BGR2GRAY);
			equalizeHist(graySacleFrame, graySacleFrame);

			//находим лица на серой картинке
			face_cascade.detectMultiScale(graySacleFrame, faces, 1.1, 3, 0, cv::Size(90, 90));

			//number of faces detected
			//cout << faces.size() << " faces detected" << endl;
			std::string frameset = std::to_string(count);
			std::string faceset = std::to_string(faces.size());

			int width = 0, height = 0;

			//region of interest
			//cv::Rect roi;

			for (int i = 0; i < faces.size(); i++)
			{
				//region of interest
				Rect face_i = faces[i];

				//crop the roi from gray image
				Mat face = graySacleFrame(face_i);

				//resizing the cropped image to suit to database image sizes
				Mat face_resized;
				cv::resize(face, face_resized, Size(img_width, img_height), 1.0, 1.0, INTER_CUBIC);

				//recognizing what faces detected
				int label = -1; double confidence = 0;
				model->predict(face_resized, label, confidence);
				//TODO завести файл с соответствием между label и нормальным никнеймом

				cout << " confidence " << confidence << " Label: " << label << endl;
				
				Pname = to_string(label); //TODO имя вместо label

				//drawing green rectagle in recognize face
				rectangle(original, face_i, CV_RGB(0, 255, 0), 1);
				string text = Pname;

				int pos_x = std::max(face_i.tl().x - 10, 0);
				int pos_y = std::max(face_i.tl().y - 10, 0);

				//name the person who is in the image
				cv::putText(original, text, Point(pos_x, pos_y), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 255, 0), 1.0);

			}


			cv::putText(original, "Frames: " + frameset, Point(30, 60), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 255, 0), 1.0);
			cv::putText(original, "No. of Persons detected: " + to_string(faces.size()), Point(30, 90), FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 255, 0), 1.0);
			//display to the winodw
			cv::imshow(window, original);

			//cout << "model infor " << model->getDouble("threshold") << endl;

		}
		if (waitKey(30) >= 0) break;
	}
}
	