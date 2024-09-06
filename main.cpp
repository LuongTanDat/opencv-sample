#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

std::mutex mtx; // Mutex for synchronizing access to the frame
cv::Mat frame;  // Shared frame variable
std::atomic<bool> stop_flag(false); // Atomic flag to stop the threads

// Function for capturing video frames
void captureFrame(cv::VideoCapture &cap) {
    while (!stop_flag) {
        cv::Mat temp_frame;
        cap >> temp_frame;

        if (temp_frame.empty()) {
            stop_flag = true;
            break;
        }

        // Lock mutex before accessing the shared frame
        std::lock_guard<std::mutex> lock(mtx);
        frame = temp_frame.clone();
    }
}

// Function for processing and displaying frames
void processFrame() {
    while (!stop_flag) {
        cv::Mat temp_frame;

        // Lock mutex before accessing the shared frame
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (!frame.empty()) {
                temp_frame = frame.clone();
            }
        }

        // If we have a valid frame, process it (flip and display)
        if (!temp_frame.empty()) {
            cv::flip(temp_frame, temp_frame, 1);
            cv::imshow("Frame", temp_frame);

            // Exit on 'ESC' key press
            char c = (char)cv::waitKey(1);
            if (c == 27) {
                stop_flag = true;
                break;
            }
        }
    }
}

int main() {
    // Open video capture (0 for default camera)
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open the camera." << std::endl;
        return -1;
    }

    // Start threads for capturing and processing frames
    std::thread capture_thread(captureFrame, std::ref(cap));
    std::thread process_thread(processFrame);

    // Wait for both threads to finish
    capture_thread.join();
    process_thread.join();

    // Release resources
    cap.release();
    cv::destroyAllWindows();

    return 0;
}

// g++ main.cpp -o app -std=c++11 `pkg-config --cflags --libs opencv` -lpthread
