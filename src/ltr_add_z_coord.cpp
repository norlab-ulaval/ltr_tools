#include <iostream>
#include <fstream>
#include <pointmatcher/PointMatcher.h>
#include <nabo/nabo.h>

typedef Nabo::NearestNeighbourSearch<float> NNS;
typedef PointMatcher<float> PM;
const std::string TRAJECTORY_DELIMITER = "#############################";

void loadLTR(const std::string &inputFileName, const std::string &outputFileName, bool computeZ, float inZ)
{
    const std::string tmpMapFileName = "/tmp/map.vtk";
    std::ifstream ltrFile(inputFileName);
    std::ofstream tmpMapFile(tmpMapFileName);
    std::ofstream outputFile(outputFileName);

    int knn = 10;
    PM::DataPoints map;
    PM::DataPoints trajPoint;
    trajPoint.features = PM::Matrix::Ones(4, 1);
    trajPoint.featureLabels.push_back(PM::DataPoints::Label("x", 1));
    trajPoint.featureLabels.push_back(PM::DataPoints::Label("y", 1));
    trajPoint.featureLabels.push_back(PM::DataPoints::Label("z", 1));
    trajPoint.featureLabels.push_back(PM::DataPoints::Label("w", 1));

    PM::Matches matches(PM::Matches::Dists(knn, 1), PM::Matches::Ids(knn, 1));
    std::shared_ptr<NNS> nns;

    std::string line;
    bool parsingMap = true;
    std::cout << "Parsing map..." << std::endl;
    float prev_z = 0.0;
    while(std::getline(ltrFile, line))
    {
        if(parsingMap)
        {
            if(line.find(TRAJECTORY_DELIMITER) != std::string::npos)
            {
                outputFile << TRAJECTORY_DELIMITER << std::endl;
                std::getline(ltrFile, line); // frame id line
                outputFile << line << std::endl;
                parsingMap = false;
                std::cout << "Map done" << std::endl;
                tmpMapFile.close();
                map = PM::DataPoints::load(tmpMapFileName);
            }
            else
            {
                outputFile << line << std::endl;
                tmpMapFile << line << std::endl;
            }
        }
        else
        {
            int cursorPosition = line.find(',');
            float const x = std::stod(line.substr(0, cursorPosition));
            int previousCursorPosition = cursorPosition + 1;
            cursorPosition = line.find(",", previousCursorPosition);
            float const y = std::stod(line.substr(previousCursorPosition, cursorPosition));
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

            float z = prev_z;
            if (computeZ) {
                trajPoint.features(0, 0) = x;
                trajPoint.features(1, 0) = y;
                trajPoint.features(2, 0) = z;
                trajPoint.features(3, 0) = 1.0;
                std::shared_ptr<PM::DataPointsFilter> boundingBoxFilter =
                        PM::get().DataPointsFilterRegistrar.create(
                                "BoundingBoxDataPointsFilter",
                                {
                                        {"xMin",         std::to_string(x - 0.25)},
                                        {"xMax",         std::to_string(x + 0.25)},
                                        {"yMin",         std::to_string(y - 0.25)},
                                        {"yMax",         std::to_string(y + 0.25)},
                                        {"zMin",         std::to_string(z - 100000.0)},
                                        {"zMax",         std::to_string(z - 100000.0)},
                                        {"removeInside", std::to_string(0)}
                                }
                        );

                auto bb = boundingBoxFilter->filter(map);
                if (bb.getNbPoints() != 0) {
                    nns = std::shared_ptr<NNS>(NNS::create(bb.features, bb.features.rows() - 1,
                                                           NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));
                    nns->knn(trajPoint.features, matches.ids, matches.dists, knn, 0);
                    for (int i = 0; i < matches.ids.size(); ++i) {
                        z += map.features(2, matches.ids(i));
                    }
                    z /= matches.ids.size();
                } else {
                    z = prev_z;
                }
                prev_z = z;
            }
            else
            {
                z = inZ;
            }
            outputFile << x << "," << y << "," << z << "," << qx << "," << qy << "," << qz << "," << qw << std::endl;
        }
    }

    std::cout << "Closing files..." << std::endl;
    ltrFile.close();
    outputFile.close();
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        std::cerr << "No ltr input filename provided" << std::endl;
        return -1;
    }

    const std::string inputFilename = argv[1];
    std::string outputFilename = "/tmp/output.ltr";

    if (argc <= 2)
    {
        std::cerr << "No ltr output filename provided, using " << outputFilename << std::endl;
    }
    else
    {
        outputFilename = argv[2];
    }

    bool computeZ = true;
    float z = 0.0;
    if (argc == 4)
    {
        computeZ = false;
        z = std::stof(argv[3]);
    }
    loadLTR(inputFilename, outputFilename, computeZ, z);
    return 0;
}
