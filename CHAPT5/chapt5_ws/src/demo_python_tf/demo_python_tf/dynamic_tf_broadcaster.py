import rclpy
from rclpy.node import Node
from tf2_ros import TransformBroadcaster
from geometry_msgs.msg import TransformStamped
from tf_transformations import quaternion_from_euler
import math

class TFBroadcaster(Node):
    def __init__(self):
        super().__init__('tf_broadcaster')
        self.broadcaster = TransformBroadcaster(self)
        # self.publish_static_tf()
        self.timer = self.create_timer(0.01, self.publish_tf)
        
    def publish_tf(self):
        # publish base_link -> camera_link
        transform = TransformStamped()
        transform.header.frame_id = 'camera_link'
        transform.child_frame_id = 'bottle_link'
        transform.header.stamp = self.get_clock().now().to_msg()
        transform.transform.translation.x = 0.2
        transform.transform.translation.x = 0.3
        transform.transform.translation.x = 0.5
        q = quaternion_from_euler(0, 0, 0)
        
        transform.transform.rotation.x = q[0]
        transform.transform.rotation.y = q[1] 
        transform.transform.rotation.z = q[2] 
        transform.transform.rotation.w = q[3] 
        
        self.broadcaster.sendTransform(transform)
        self.get_logger().info("publish TF:{}".format(transform))
        
def main():
    rclpy.init()
    node = TFBroadcaster()
    rclpy.spin(node)
    rclpy.shutdown()