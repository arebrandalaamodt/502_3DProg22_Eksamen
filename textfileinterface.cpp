//#include <iostream>
#include <fstream>

#include "textfileinterface.h"

//TextFileInterface::TextFileInterface()
//{

//}


std::vector<Vertex> TextFileInterface::VertexReadFromFile()
{
    std::vector<Vertex> a;





    return a;
}

void TextFileInterface::VertexWriteToFile(std::vector<Vertex> vectorToFile, std::string fileName)
{

    std::ofstream file;
    file.open("../3Dprog22/txt_files/"+fileName);
    if(file.is_open())
    {
        std::cout << "Write File open" << std::endl;
        file << vectorToFile.size() << std::endl;
        for(int i = 0; i < vectorToFile.size(); i++)
        {
            file << vectorToFile[i] << std::endl;
        }
        file.close();
    }
    else
    {
        std::cout << "TextFileInterface::VertexWriteToFile | Failed" << std::endl;
    }


    ////~~~~~~~~~~ Incremental File Names = x_fileName
    ////          Reading counterFile to create new filename & increment counterFile (counter.txt)
    //    if (bIncrementFileName)
    //    {
    //        int counter{0};
    //        std::fstream counterFile;
    //        std::string counterString{0};
    //        counterFile.open("../3Dprog22/txt_files/counter.txt", std::ios::in);
    //        if (counterFile.is_open())
    //        {
    //            std::cout << "CounterFile read correctly" << std::endl;
    //            counterFile >> counter;
    //            counterFile.close();
    //        }
    //        counterFile.open("../3Dprog22/txt_files/counter.txt", std::ios::out);
    //        if (counterFile.is_open())
    //        {
    //            std::cout << "CounterFile incremented correctly" << std::endl;
    //            counterFile << (counter+1);
    //            counterFile.close();
    //        }
    //        if(counter != 0)
    //        {
    //            counterString = std::to_string(counter);
    //        }
    //        else
    //        {
    //            std::cout << "CounterFile = 0" << std::endl;
    //        }
    //        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    //        std::ofstream file;
    //        std::string combinedFileName = counterString + "_" + fileName;
    //        file.open("../3Dprog22/txt_files/"+combinedFileName);
    //        if(file.is_open())
    //        {
    //            std::cout << "Write File open" << std::endl;
    //            file << mVertices.size() << std::endl;
    //            for(auto i = 0; i < mVertices.size(); i++)
    //            {
    //                file << mVertices[i] << std::endl;
    //            }
    //            file.close();
    //        }
    //    }
    //    else
    //    {
    //        std::ofstream file;
    //        file.open("../3Dprog22/txt_files/"+fileName);
    //        if(file.is_open())
    //        {
    //            std::cout << "Write File open" << std::endl;
    //            file << mVertices.size() << std::endl;
    //            for(auto i = 0; i < mVertices.size(); i++)
    //            {
    //                file << mVertices[i] << std::endl;
    //            }
    //            file.close();
    //        }
    //    }



}
