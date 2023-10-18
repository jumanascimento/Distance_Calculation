#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from nav_msgs.msg import OccupancyGrid
import cv2
import os
import numpy as np
import yaml
from cv_bridge import CvBridge

class MapSaverNode(Node):
    def __init__(self):
        super().__init__('map_saver')
        self.map_subscription = self.create_subscription(
            OccupancyGrid,
            '/slam/map',  # Substitua pelo tópico do mapa gerado pelo ORB-SLAM3
            self.map_callback,
            10)
        self.map_subscription

        self.map_save_path = os.path.expanduser("~")
        self.bridge = CvBridge()

    def map_callback(self, msg):
        map_data = np.array(msg.data, dtype=np.uint8).reshape((msg.info.height, msg.info.width))

        map_image_file = os.path.join(self.map_save_path, 'map.pgm')
        map_yaml_file = os.path.join(self.map_save_path, 'map.yaml')

        cv2.imwrite(map_image_file, map_data)

        map_info = {
            'image': 'map.pgm',
            'resolution': msg.info.resolution,
            'origin': {
                'position': {
                    'x': msg.info.origin.position.x,
                    'y': msg.info.origin.position.y,
                    'z': msg.info.origin.position.z,
                },
                'orientation': {
                    'x': msg.info.origin.orientation.x,
                    'y': msg.info.origin.orientation.y,
                    'z': msg.info.origin.orientation.z,
                    'w': msg.info.origin.orientation.w,
                }
            },
            'negate': False,  # Ajuste conforme necessário
            'occupied_thresh': 0.65,  # Ajuste conforme necessário
            'free_thresh': 0.196  # Ajuste conforme necessário
        }

        with open(map_yaml_file, 'w') as yaml_file:
            yaml.dump(map_info, yaml_file)

        self.get_logger().info('Mapa salvo como map.pgm e map.yaml em: {}'.format(self.map_save_path))

def main(args=None):
    rclpy.init(args=args)
    node = MapSaverNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()
