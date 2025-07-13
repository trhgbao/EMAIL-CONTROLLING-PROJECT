#include "webcam.h"
using namespace cv;
using namespace std;
using namespace std::chrono;
const size_t CHUNK_SIZE = 1024;

// Global variables
mutex cameraMutex;
atomic<bool> isCameraOpen(false);
atomic<bool> isRecording(false);
atomic<bool> shouldStopCamera(false);
VideoCapture cap;

// Check if directory is writable
bool CanWriteToDirectory(const string& path) {
    try {
        string testFilePath = path + "\\test_write_permission.txt";

        ofstream testFile(testFilePath);
        if (!testFile) {
            cout << "Cannot create file. Write permission denied." << endl;
            return false;
        }

        testFile.close();
        return true;
    }
    catch (const exception& e) {
        cout << "Permission check error: " << e.what() << endl;
        return false;
    }
}

// Close the camera safely
void CloseCamera() {
    lock_guard<mutex> lock(cameraMutex);

    if (!isCameraOpen) {
        cout << "Camera is not open" << endl;
        return;
    }
    shouldStopCamera = true;
    cap.release();
    destroyAllWindows();
    isCameraOpen = false;
    cout << "Camera closed" << endl;
}

// Thread for streaming the camera feed
void CameraStream() {
    Mat frame;

    while (!shouldStopCamera) {
        {
            lock_guard<mutex> lock(cameraMutex);

            if (!cap.isOpened()) break;
            cap >> frame;

            if (frame.empty()) {
                cout << "No captured frame" << endl;
                break;
            }
            imshow("Camera", frame);
        }

        int key = waitKey(1);
        if (key == 27 || shouldStopCamera) {  // ESC key or stop flag
            break;
        }
    }

    CloseCamera();
}

// Open the camera
void OpenCamera() {
    lock_guard<mutex> lock(cameraMutex);

    if (isCameraOpen) {
        cout << "Camera is already open" << endl;
        return;
    }

    cap.open(0);  // Open default camera (index 0)
    if (!cap.isOpened()) {
        cout << "Cannot open camera" << endl;
        return;
    }

    isCameraOpen = true;
    shouldStopCamera = false;
    cout << "Camera opened successfully" << endl;
}

// Function to record the screen using FFmpeg
void RecordScreen(int duration) {
    string output_file = "output.mp4";

    // FFmpeg command for screen recording
    string ffmpeg_command =
        "ffmpeg -probesize 50M -f gdigrab -framerate 20 -i desktop -vcodec libx264 -vf format=yuv420p -preset ultrafast -t "
        + to_string(duration) + " -y " + output_file;

    cout << "Recording screen for " << duration << " seconds..." << endl;
    int result = system(ffmpeg_command.c_str());

    if (result == 0) {
        cout << "Video saved at: " << output_file << endl;
    }
    else {
        cerr << "Error occurred while recording the screen!" << endl;
    }
}
