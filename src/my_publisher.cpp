#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sstream>
#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>


using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_publisher");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::Publisher pub = it.advertise("/camera/image_raw", 1);

  if(argc) cout<<argv[1]<<endl;
  
  ros::Rate loop_rate(25);
  //string path = "/home/samlau/Downloads/";
  //path = path + argv[1];//用户自己添加视频文件名字

  VideoCapture cap("rtsp://admin:bmw12345@172.169.10.88:554/live/0/MAIN");//open video from the path
  double fps = cap.get(5)/10000;
  
  if(!cap.isOpened())
  {
    std::cout<<"open video failed!"<<std::endl;
    return -1;
  }
  else
    std::cout<<"open video success!"<<std::endl;
 
  Mat frame;//this is an image
  bool isSuccess = true;

  while(nh.ok())
  {

      isSuccess = cap.read(frame);
      if(!isSuccess)//if the video ends, then break
      {
        std::cout<<"video ends"<<std::endl;
        break;
      }
      resize(frame,frame,Size(1280,720));

      //cv::imshow("frameshow",frame);
      //cv::waitKey(30);
      // 将opencv的图片转换成ros的sensor_msgs，然后才能发布。
      //改了
      sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", frame).toImageMsg();
      //原本如下
      //sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
      pub.publish(msg);
      ros::spinOnce();
      loop_rate.sleep();

  }

  return 0;

}
