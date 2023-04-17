#include <iostream>
#include <fstream>
#include <Eigen/Geometry>
#include <pointmatcher/PointMatcher.h>

typedef PointMatcher<float> PM;
const std::string TRAJECTORY_DELIMITER = "#############################";

struct TrajectoryPoint {
    TrajectoryPoint(const float x, const float y, const float z, float roll, float pitch, float yaw, bool direction)
        : x(x), y(y), z(z), roll(roll), pitch(pitch), yaw(yaw), direction(direction) {}
    float x;
    float y;
    float z;
    float roll;
    float pitch;
    float yaw;
    bool direction;
};

void loadLTR(const std::string &fileName)
{
    std::ofstream mapFile("/tmp/map.vtk");
    std::ifstream ltrFile(fileName);

    std::vector<TrajectoryPoint> trajectoryVec;
    std::string line;
    bool parsingMap = true;
    bool direction = false;
    std::cout << "Parsing map..." << std::endl;
    int ctr = 0;
    while(std::getline(ltrFile, line))
    {
        if(parsingMap)
        {
            if(line.find(TRAJECTORY_DELIMITER) != std::string::npos)
            {
                std::getline(ltrFile, line); // skip frame id line
                parsingMap = false;
            }
            else
            {
                mapFile << line << std::endl;
            }
        }
        else
        {
            if(line.find("changing direction") != std::string::npos)
            {
                direction = !direction;
                continue;
            }

            int cursorPosition = line.find(',');
            float const x = std::stod(line.substr(0, cursorPosition));
            int previousCursorPosition = cursorPosition + 1;
            cursorPosition = line.find(",", previousCursorPosition);
            float const y = std::stod(line.substr(previousCursorPosition, cursorPosition));
            previousCursorPosition = cursorPosition + 1;
            cursorPosition = line.find(",", previousCursorPosition);
            float const z = std::stod(line.substr(previousCursorPosition, cursorPosition));
            previousCursorPosition = cursorPosition + 1;
            cursorPosition = line.find(",", previousCursorPosition);
            float const qx = std::stod(line.substr(previousCursorPosition, cursorPosition));
            previousCursorPosition = cursorPosition + 1;
            cursorPosition = line.find(",", previousCursorPosition);
            float const qy = std::stod(line.substr(previousCursorPosition, cursorPosition));
            previousCursorPosition = cursorPosition + 1;
            cursorPosition = line.find(",", previousCursorPosition);
            float const qz = std::stod(line.substr(previousCursorPosition, cursorPosition));
            previousCursorPosition = cursorPosition + 1;
            cursorPosition = line.find("\n", previousCursorPosition);
            float const qw = std::stod(line.substr(previousCursorPosition, cursorPosition));

            const Eigen::Quaternion<float> quat(qw, qx, qy, qz);

            auto orientPoint = (quat * Eigen::Vector3f {1, 0, 0});
            trajectoryVec.emplace_back(x , y , z, orientPoint[0], orientPoint[1], orientPoint[2], direction);
        }
    }

    std::cout << "Parsing trajectory..." << std::endl;
    PM::DataPoints trajectory;
    size_t numberOfPoints = trajectoryVec.size();
    trajectory.features = PM::Matrix::Ones(4, numberOfPoints);
//  normalized coordinates
    trajectory.featureLabels.push_back(PM::DataPoints::Label("x", 1));
    trajectory.featureLabels.push_back(PM::DataPoints::Label("y", 1));
    trajectory.featureLabels.push_back(PM::DataPoints::Label("z", 1));
    trajectory.featureLabels.push_back(PM::DataPoints::Label("w", 1));
    trajectory.descriptors = PM::Matrix::Ones(5, numberOfPoints);
    trajectory.addDescriptor("id", PM::Matrix::Constant(1, numberOfPoints, 0));
    trajectory.addDescriptor("orientation", PM::Matrix::Constant(3, numberOfPoints, 0));
    trajectory.addDescriptor("direction", PM::Matrix::Constant(1, numberOfPoints, 0));

    auto ids{ trajectory.getDescriptorViewByName("id") };
    auto orientations{ trajectory.getDescriptorViewByName("orientation") };
    auto directions{ trajectory.getDescriptorViewByName("direction") };
    for(int i = 0; i < numberOfPoints; ++i)
    {
        const auto &trajectoryPoint = trajectoryVec[i];
        trajectory.features(0, i) = trajectoryPoint.x;
        trajectory.features(1, i) = trajectoryPoint.y;
        trajectory.features(2, i) = trajectoryPoint.z;
        trajectory.features(3, i) = 1;
        ids(0, i) = i;
        orientations(0, i) = trajectoryPoint.roll;
        orientations(1, i) = trajectoryPoint.pitch;
        orientations(2, i) = trajectoryPoint.yaw;
        directions(0, i) = trajectoryPoint.direction;
    }

    trajectory.save("/tmp/traj.vtk", false);

    std::cout << "Closing files..." << std::endl;
    ltrFile.close();
    mapFile.close();
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        std::cerr << "No ltr filename provided" << std::endl;
        return -1;
    }

    const std::string filename = argv[1];
    loadLTR(filename);
    return 0;
}
