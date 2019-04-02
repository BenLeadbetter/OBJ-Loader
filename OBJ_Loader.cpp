// source file for OBJ_Loader.h

#include <algorithm>
#include "OBJ_Loader.h"

using namespace std;

bool Loader::loadFile(const std::string& filename)
{
    // open the file
    ifstream fs(filename);
    if(!fs.is_open())
        return false;

    std::string curWord;
    
    ///////////////////////////////////////////////////////////
    // first read in the vertices

    // find the first vertex
    while(curWord != "v" && fs >> curWord);

    // read the data
    while(curWord == "v")
    {
        Vector3 vertex;
        for(size_t i = 0; i != 3; ++i)
        {
            fs >> curWord;
            vertex[i] = std::stof(curWord);
        }

        m_Vertices.push_back(vertex);

        fs >> curWord;
    }


    ///////////////////////////////////////////////////////////
    // here to read texture coords - when I get round to it!


    ///////////////////////////////////////////////////////////
    // here read normals

    // find the first normal
    while(curWord != "vn" && fs >> curWord);


    // read the data
    while(curWord == "vn")
    {
        Vector3 normal;
        for(size_t i = 0; i != 3; ++i)
        {
            fs >> curWord;
            normal[i] = std::stof(curWord);
        }

        m_Normals.push_back(normal);

        fs >> curWord;
    }


    ///////////////////////////////////////////////////////////
    // here read custom parameters


    ///////////////////////////////////////////////////////////
    // here read triangle face data

    // find the first face
    while(curWord != "f" && fs >> curWord);

    // read the data
    while(curWord == "f")
    {
        Face face;

        // read the three data
        for(std::size_t i = 0; i != 3; ++i)
        {
            fs >> curWord;
            FaceData fData;

            // first is vertex index 
            auto strItr1 = curWord.begin();
            auto strItr2 = find(curWord.begin(), curWord.end(), '/');
            if(strItr1 != strItr2)
                fData[0] = stoi(string(strItr1, strItr2));

            strItr1 = ++strItr2;

            // then texture index
            strItr2 = find(strItr1, curWord.end(), '/');
            if(strItr1 != strItr2)
                fData[1] = stoi(string(strItr1, strItr2));
            else
                fData[1] = 0;
            
            strItr1 = ++strItr2;

            // then the normal
            if(strItr1 != curWord.end())
                fData[2] = stoi(string(strItr1, curWord.end()));
            else
                fData[2] = 0;
            
            face[i] = fData;
        }

        m_Faces.push_back(face);
        fs >> curWord;
    } 

    return true;
}

float Loader::vertexData(size_t i, size_t j) const
{
    if(i < m_Vertices.size() && j < 3)
        return m_Vertices[i][j];
    else 
        return 0.0f;
}

size_t Loader::numberVertices() const
{
    return m_Vertices.size();
}

size_t Loader::numberFaces() const
{
    return m_Faces.size();
}

size_t Loader::numberNormals() const
{
    return m_Normals.size();
}
