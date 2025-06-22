import rclpy
from rclpy.node import Node
from chapt4_interfaces.srv import FaceDetector
import face_recognition
import cv2
from ament_index_python.packages import get_package_share_directory
import os
from cv_bridge import CvBridge
import time
from rcl_interfaces.msg import SetParametersResult
class FaceDetectNode(Node):
    def __init__(self):
        super().__init__('face_detect_node')
        # use parameter
        self.declare_parameter("number_of_times_to_upsample", 1)
        self.declare_parameter("model", "hog")
        self.number_of_times_to_upsample = self.get_parameter("number_of_times_to_upsample").value
        self.model = self.get_parameter("model").value
        
        self.service_ = self.create_service(FaceDetector, 'face_detect', self.detect_face_callback)
        self.add_on_set_parameters_callback(self.parameter_callback)
        
        self.bridge = CvBridge()
        self.default_image_path = os.path.join(get_package_share_directory('demo_python_service'), 'resource/default.jpg')
       
    def parameter_callback(self, parameters):
        for parameter in parameters:
            self.get_logger().info("{}->{}".format(parameter.name, parameter.value))
            if parameter.name == "number_of_times_to_upsample":
                self.number_of_times_to_upsample = parameter.value
            if parameter.name == "model":
                self.model = parameter.value
        return SetParametersResult(successful = True)
    
    def detect_face_callback(self, request, response):
        if request.image.data:
            cv_image = self.bridge.imgmsg_to_cv2(request.image)
        else:
            cv_image = cv2.imread(self.default_image_path)
        start_time = time.time()
        face_locations = face_recognition.face_locations(cv_image, self.number_of_times_to_upsample, self.model)
        response.use_time = time.time() - start_time
        response.number = len(face_locations)
        for top,right,bottom,left in face_locations:
            response.top.append(top)
            response.right.append(right)
            response.bottom.append(bottom)
            response.left.append(left)
        return response
    
    
    

def main():
    rclpy.init()
    node = FaceDetectNode()
    rclpy.spin(node)
    rclpy.shutdown()