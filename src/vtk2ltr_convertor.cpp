#include <iostream>
#include <fstream>
#include <filesystem>
#include <Eigen/Geometry>
#include <pointmatcher/PointMatcher.h>

typedef PointMatcher<float> PM;
const std::string TRAJECTORY_DELIMITER = "#############################";

void toLTR(const std::string &mapFilename, const std::string &trajFilename, const std::string &ltrFilename) {
    std::cout << mapFilename << " " << trajFilename << " " << ltrFilename << std::endl;

    std::cout << "Copying map..." << std::endl;
    std::filesystem::copy_file(mapFilename, ltrFilename, std::filesystem::copy_options::overwrite_existing);

    std::cout << "Processing trajectory..." << std::endl;
    std::ofstream ltrFile(ltrFilename, std::ios::app);
    PM::DataPoints trajectory = PM::DataPoints::load(trajFilename);
    ltrFile << TRAJECTORY_DELIMITER << std::endl;
    ltrFile << "frame_id : map" << std::endl;
    int oldDirection = -1;
    for (int i = 0; i < trajectory.getNbPoints(); ++i) {
        float x = trajectory.features(0, i);
        float y = trajectory.features(1, i);
        float z = trajectory.features(2, i);

//      get the quaternion
        Eigen::Vector3f v1(1.0, 0.0, 0.0);
        Eigen::Vector3f v2(trajectory.descriptors(1, i), trajectory.descriptors(2, i), trajectory.descriptors(3, i));
        v1.normalize();
        v2.normalize();
        const auto a = v1.cross(v2);
        const float w = 1 + v1.dot(v2); // assuming that the size of v1,v2 = 1
        Eigen::Quaternion<float> q(w, a(0), a(1), a(2));
        q.normalize();

//      get the direction
        int direction = (int) trajectory.descriptors(4, i);
        if (oldDirection == -1)
            oldDirection = direction;
        if (direction != oldDirection) {
            ltrFile << "changing direction" << std::endl;
            oldDirection = direction;
        }

        ltrFile << x << "," << y << "," << z << "," << q.coeffs()[0] << "," << q.coeffs()[1] << "," << q.coeffs()[2]
                << "," << q.coeffs()[3] << std::endl;
    }
    std::cout << "Closing files..." << std::endl;
    ltrFile.close();
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        std::cerr << "No vtk map filename provided" << std::endl;
        return -1;
    }

    if (argc <= 2) {
        std::cerr << "No vtk trajectory filename provided" << std::endl;
        return -1;
    }

    std::string outputFilename = "/tmp/output.ltr";
    if (argc <= 3) {
        std::cerr << "No ltr output filename provided, using " << outputFilename << std::endl;
    } else {
        outputFilename = argv[3];
    }
    const std::string mapFilename = argv[1];
    const std::string trajFilename = argv[2];

    toLTR(mapFilename, trajFilename, outputFilename);
    return 0;
}
