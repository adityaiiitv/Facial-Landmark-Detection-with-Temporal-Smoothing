#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <opencv2/opencv.hpp>

using namespace dlib;
using namespace std;
using namespace cv;

int main(int argc, char** argv)
{  
  
  // Get the face detector
  frontal_face_detector faceDetector = get_frontal_face_detector();

  // The landmark
  shape_predictor landmarkDetector;

  // Load the landmark model
  deserialize("shape_predictor_68_face_landmarks.dat") >> landmarkDetector;

  VideoCapture cap("lop.mp4");
  if (!cap.isOpened())  // check if we succeeded
  {
	  return -1;
  }
  int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
  VideoWriter video("output.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height));
  cv::Mat im[7];	// 3 frames front and 3 back
  cap >> im[6];
  im[0] = im[6].clone();
  im[1] = im[6].clone();
  im[2] = im[6].clone();
  im[3] = im[6].clone();
  im[4] = im[6].clone();
  im[5] = im[6].clone();
  int old_x[100][6];	// To store old values of the landmarks
  int old_y[100][6];
  int old_flag = 0;
  for (;;)
  {
	  // Convert OpenCV image format to Dlib's image format
	  cv_image<bgr_pixel> dlibIm(im[3]);

	  // Detect faces in the image
	  std::vector<dlib::rectangle> faceRects = faceDetector(dlibIm);

	  // Loop over all detected face rectangles
	  int len1 = faceRects.size();
	  full_object_detection landmarks;
	  int flag = 0;
	  for (int i = 0; i < len1; i++)
	  {
		  landmarks = landmarkDetector(dlibIm, faceRects[i]);
	  }
	  // Draw landmarks on face
	  int len2 = landmarks.num_parts();
	  int px[100], py[100];
	  if (old_flag == 0)
	  {
		  old_flag = 1;
		  for (int i = 0; i < len2; i++)
		  {
			  for (int j = 0; j < 6; j++)
			  {
				  old_x[i][j] = landmarks.part(i).x();	//	Initialize old values
				  old_y[i][j] = landmarks.part(i).y();
			  }
		  }
	  }
	  for (int i = 0; i < len2; i++)
	  {
		  old_x[i][0] = old_x[i][1];	// Recycle values
		  old_x[i][1] = old_x[i][2];
		  old_x[i][2] = old_x[i][3];
		  old_x[i][3] = old_x[i][4];
		  old_x[i][4] = old_x[i][5];
		  old_x[i][5] = landmarks.part(i).x();
		  old_y[i][0] = old_y[i][1];
		  old_y[i][1] = old_y[i][2];
		  old_y[i][2] = old_y[i][3];
		  old_y[i][3] = old_y[i][4];
		  old_y[i][4] = old_y[i][5];
		  old_y[i][5] = landmarks.part(i).y();
		  px[i] = (old_x[i][0] + old_x[i][1] + old_x[i][2] + old_x[i][3] + old_x[i][4] + old_x[i][5]) / 6;
		  py[i] = (old_y[i][0] + old_y[i][1] + old_y[i][2] + old_y[i][3] + old_y[i][4] + old_y[i][5]) / 6;
		  // Smooth out the points to plot
		  circle(im[3], Point(px[i], py[i]), 1, Scalar(0, 0, 255), 2, CV_AA);
	  }
	  // Display image
	  cv::imshow("Facial Landmark Detector", im[3]);
	  video.write(im[3]);
	  if (waitKey(30) >= 0) break;
	  im[0] = im[1].clone();	// Recycle images
	  im[1] = im[2].clone();
	  im[2] = im[3].clone();
	  im[3] = im[4].clone();
	  im[4] = im[5].clone();
	  im[5] = im[6].clone();
	  cap >> im[6];
  }
  return EXIT_SUCCESS;
}

