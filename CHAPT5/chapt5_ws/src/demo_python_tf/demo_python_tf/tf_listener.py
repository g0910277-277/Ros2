import rclpy
from rclpy.node import Node
import rclpy.time
from tf2_ros import TransformListener, Buffer
from tf_transformations import euler_from_quaternion
import math

class TFBroadcaster(Node): 
    def __init__(self):
        super().__init__('tf_listener')
        self.buffer = Buffer()
        self.listener = TransformListener(self.buffer, self)
        self.timer = self.create_timer(1.0, self.get_transform)
        
    def get_transform(self):
        # query
        try:
            result = self.buffer.lookup_transform('base_link', 'bottle_link', rclpy.time.Time(seconds=0.0), rclpy.time.Duration(seconds=1.0))
            transform = result.transform
            self.get_logger().info('translation: {}'.format(transform.translation))
            self.get_logger().info('rotation: {}'.format(transform.rotation))
            rotation_euler = euler_from_quaternion([transform.rotation.x, transform.rotation.y, transform.rotation.z, transform.rotation.w])
            self.get_logger().info('rotation RPY: {}'.format(rotation_euler))
        except Exception as e:
            self.get_logger().warn('get failed: {}'.format(str(e)))

        
def main():
    rclpy.init()
    node = TFBroadcaster()
    rclpy.spin(node)
    rclpy.shutdown()