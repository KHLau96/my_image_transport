#!/usr/bin/env python
import cv2
import sys

if sys.version > '3':
	import queue as Queue
else:
	import Queue
from sensor_msgs.msg import Image
import time
import threading
import rospy
from cv_bridge import CvBridge, CvBridgeError

q=Queue.Queue()

def Receive():
    print("start Reveive")
    cap = cv2.VideoCapture("rtsp://admin:bmw12345@172.169.10.88:554/live/0/MAIN")
    ret = cap.set(3,1080)
    ret = cap.set(4,720)
    ret, frame = cap.read()
    q.put(frame)
    while ret:
        ret, frame = cap.read()
        q.put(frame)
 
 
def Display():
     print("Start Displaying")
     while True:
         if q.empty() !=True:
            frame=q.get()
            msg = bridge.cv2_to_imgmsg(frame, encoding="bgr8")
            img_pub.publish(msg)
         if cv2.waitKey(1) & 0xFF == ord('q'):
                break
 
if __name__=='__main__':
    
    rospy.init_node('python_publisher1',anonymous=True)

    
    img_pub = rospy.Publisher('/camera/image_raw', Image, queue_size=3)
    rate = rospy.Rate(20)

    p1=threading.Thread(target=Receive)

    scaling_factor = 0.5
    bridge = CvBridge()


    
        
    
    count = 0

    p2 = threading.Thread(target=Display)
    p1.start()
    p2.start()
