#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <dnndetector.hpp>

DnnDetector detector;
cv::dnn::Net net = detector.net;
    
int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_publisher");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub = it.advertise("camera/video", 1);

    cv::VideoCapture cap;
    cap.open(0);
    cv::Mat frame;
    sensor_msgs::ImagePtr msg;
    ros::Rate loop_rate(5);
    while (ros::ok())
    {
        cap>>frame; 
        detector.thePredictor(frame, net);			// do detection
        detector.drawResult(frame,detector.out_names,detector.out_boxes,detector.out_confidences,detector.out_centers,true);
        msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
        pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }
}