import rclpy
from rclpy.node import Node
from tf2_ros import StaticTransformBroadcaster
from geometry_msgs.msg import TransformStamped
from tf_transformations import quaternion_from_euler
import math

class StaticTFBroadcaster(Node):
    def __init__(self):
        super().__init__('static_tf_broadcaster')
        self.static_broadcaster = StaticTransformBroadcaster(self)
        self.publish_static_tf()
        
    def publish_static_tf(self):
        # publish base_link -> camera_link
        transform = TransformStamped()
        transform.header.frame_id = 'base_link'
        transform.child_frame_id = 'camera_link'
        transform.header.stamp = self.get_clock().now().to_msg()
        transform.transform.translation.x = 0.5
        transform.transform.translation.x = 0.3
        transform.transform.translation.x = 0.6
        q = quaternion_from_euler(math.radians(180), 0, 0)
        
        transform.transform.rotation.x = q[0]
        transform.transform.rotation.y = q[1] 
        transform.transform.rotation.z = q[2] 
        transform.transform.rotation.w = q[3] 
        
        self.static_broadcaster.sendTransform(transform)
        self.get_logger().info("publish TF:{}".format(transform))
        
def main():
    rclpy.init()
    node = StaticTFBroadcaster()
    rclpy.spin(node)
    rclpy.shutdown()