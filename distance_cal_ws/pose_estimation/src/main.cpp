#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

// Função para calcular a distância
float calculateDistance(float focal_length_px, float object_height_mm, float object_height_px, float marker_height_from_camera_m, float camera_height_from_ground_m) {
    float distance_to_camera_m = marker_height_from_camera_m - camera_height_from_ground_m;
    return (focal_length_px * object_height_mm * camera_height_from_ground_m) / (object_height_px * 1000.0 * distance_to_camera_m);
}

int main() {

    // Definir os parâmetros da câmera
    float focal_length_px = 695.802000; // Distância focal em pixels
    float object_height_mm = 103.0; // Altura do objeto em milímetros
    float camera_height_from_ground_m = 0.75; // Altura da câmera em relação ao chão (em metros)

    cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << 695.802000, 0.000000, 328.281893,
                                                        0.000000, 696.308095, 233.812776,
                                                        0.000000, 0.000000, 1.000000);
                                                        
    cv::Mat distortion_coefficients = (cv::Mat_<double>(1, 5) << 0.69365370528850312, -5.9352692601414603, 0.0052417087198828854,       -0.0067740281703556546, 12.639189765328977);

 
    
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME(16));

    cv::VideoCapture in_video(2);
    if (!in_video.isOpened()) {
        std::cerr << "failed to open video input" << std::endl;
        return 1;
    }

    cv::Mat image, image_copy;

    int wait_time = 10; // Wait for 10 milliseconds between frames
    
    // Vector to store collected distances
    std::vector<std::vector<float>> collected_distances;
    
    std::time_t now = std::time(nullptr);
    std::stringstream ss;
    ss << "distances_" << std::put_time(std::localtime(&now), "%Y%m%d_%H%M%S") << ".csv";
    std::string filename = ss.str();


    while (true) { // Keep collecting distances until manually interrupted
        in_video.grab();
        in_video.retrieve(image);
        
        // Undistort the image
        cv::Mat undistorted_image;
        cv::undistort(image, undistorted_image, camera_matrix, distortion_coefficients);
        
        undistorted_image.copyTo(image_copy);

        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f>> corners;
        cv::aruco::detectMarkers(undistorted_image, dictionary, corners, ids);

        std::vector<float> distances; // Collect distances for this frame
        
        for (size_t i = 0; i < ids.size(); ++i) {
            // Calculate distance to the ArUco using the height of the ArUco from the camera and camera height from the ground
            float marker_height_from_camera_m = 1.10; // Altura da ArUco em relação à câmera (em metros)
            float object_height_px = corners[i][0].y - corners[i][2].y; // Assuming vertical markers
            float distance = calculateDistance(focal_length_px, object_height_mm, object_height_px, marker_height_from_camera_m, camera_height_from_ground_m);

            std::cout << "Distance to ArUco " << ids[i] << ": " << distance << " m" << std::endl;
            distances.push_back(distance); // Store the distance for this ArUco
        }

        collected_distances.push_back(distances); // Store distances for this frame
        
        cv::imshow("Pose estimation", image_copy);
        char key = (char)cv::waitKey(wait_time);
        if (key == 27) { // Exit when the 'Esc' key is pressed
            break;
        }
    }

    in_video.release();

    return 0;
}
