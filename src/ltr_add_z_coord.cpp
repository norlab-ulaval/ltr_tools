#include <iostream>
#include <fstream>

const std::string TRAJECTORY_DELIMITER = "#############################";

void loadLTR(const std::string &inputFileName, const std::string &outputFileName)
{
    std::ifstream ltrFile(inputFileName);
    std::ofstream outputFile(outputFileName);

    std::string line;
    bool parsingMap = true;
    std::cout << "Parsing map..." << std::endl;
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
            }
            else
            {
                outputFile << line << std::endl;
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

            outputFile << x << "," << y << ",0.0," << qx << "," << qy << "," << qz << "," << qw << std::endl;
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

    loadLTR(inputFilename, outputFilename);
    return 0;
}
