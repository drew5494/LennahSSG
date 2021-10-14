#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sys/stat.h>
#include "Config.h"
#include "FileReader.h"
#define VERSION 1.0

using namespace std;

void help_message();
void version_message();
void inputManager(string input, string output);
void createDir(string dirPath);

int main(int argc, char **argv)
{
    Config config;
    //Checking number of arguments
    if (argc < 2)
    {
        help_message();
        return 1;
    }
    else if (argc == 2)
    {
        std::string arg = argv[1];
        if ((arg == "-v") || (arg == "--version"))
        {
            version_message();
            return 0;
        }
        else if ((arg == "-h") || (arg == "--help"))
        {
            help_message();
            return 0;
        }
        else
        {
            cout << "Not using proper arguments";
        }
    }
    else if (argc >= 3)
    {
        std::string arg = argv[1];
        std::string argDetail = argv[2];
        
        //Check if config argument is given and what config file is called
        string configFilePath = config.getConfig(argc, argv);
        if (configFilePath != "")
        {
            if (configFilePath.find(".json") != string::npos)
            {
                cout << "Reading Config: " << configFilePath << endl;
                config.readConfig(configFilePath);
                inputManager(config.getInput(), config.getOutput());
            }
            else
            {
                cout << "Config file must be a JSON file." << endl;
            }
        }
        else if ((arg == "-i") || (arg == "--input"))
        {
            inputManager(argDetail, "./dist/");
        }
    }

    return 0;
}

/*
 * inputManager - checks input type and converts file(s)
 */
void inputManager(string input, string output)
{
    FileReader reader;
    int fileType;

    //Making Folder
    createDir(output);

    if (input.find(".txt") != string::npos)
    {
        cout << "Converting: " << input << endl;
        cout << "Outputting to: " << output << endl;
        fileType = 1;
        reader.convertFile(input, output, fileType);
    }
    else if (input.find(".md") != string::npos)
    {
        cout << "Converting: " << input << endl;
        cout << "Outputting to: " << output << endl;
        fileType = 2;
        reader.convertFile(input, output, fileType);
    }
    else
    {
        using fileIterator = filesystem::recursive_directory_iterator;
        for (const auto& dirEntry : fileIterator(input))
        {
            string path = dirEntry.path().string();
            if (path.find(".txt") != string::npos || path.find(".md") != string::npos)
            {
                if (path.find(".txt") != string::npos)
                {
                    cout << "Converting: " << path << endl;
                    fileType = 1;
                    reader.convertFile(path, output, fileType);
                }
                else if (path.find(".md") != string::npos)
                {
                    cout << "Converting: " << path << endl;
                    fileType = 2;
                    reader.convertFile(path, output, fileType);
                }
            }
        }
        cout << "Outputting to: " << output << endl;
    }

}

/*
 * createDir - deletes given directory if already exists and creates a new directory
 */
void createDir(string dirPath)
{
    //Delete folder if already exists
    std::filesystem::remove_all(dirPath);
    std::filesystem::create_directory(dirPath);
}

/*
 * help_message - Displays the arguments
 */
void help_message()
{
    std::cout << "LENNAH - a static site generator" << endl;
    std::cout << "-----------------------------------" << endl;
    std::cout << "Arguments:" << endl;
    std::cout << "-i/--input <file/folder path>" << endl;
    std::cout << "-c/--config <file>" << endl;
    std::cout << "-h/--help" << endl;
    std::cout << "-v/--version" << endl;
}

/*
 * version_message - Displays the version number of the application
 */
void version_message()
{
    std::cout << "LENNAH V" << VERSION;
}
