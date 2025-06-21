import rclpy
from rclpy import Node
import requests

class NovelPubNode(Node):
    def __init__(self, node_name):
        super().__init__(node_name)
        self.get_logger().info(f'{node_name}, start')
        

def main():
    rclpy.init()
    node = NovelPubNode('novel pub')
    rclpy.spin(node)
    rclpy.shutdown()