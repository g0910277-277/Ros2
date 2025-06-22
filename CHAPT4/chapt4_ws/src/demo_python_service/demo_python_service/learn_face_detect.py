import face_recognition
import cv2
# get absolute path of share
from ament_index_python.packages import get_package_share_directory
import os

def main():
    # get pic path (share)
    default_image_path = os.path.join(get_package_share_directory('demo_python_service'), 'resource/default.jpg')
    print("path:{}".format(default_image_path))
    image = cv2.imread(default_image_path)
    face_locations = face_recognition.face_locations(image, 1, model='hog')
    # plot
    for top,right,bottom,left in face_locations:
        cv2.rectangle(image, (left, top), (right, bottom), (255, 0, 0), 4)
        
    cv2.imshow('Face detect result', image)
    cv2.waitKey(0)
    