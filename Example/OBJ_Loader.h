#ifndef GUARD__OBJ_LOADER_H
#define GUARD__OBJ_LOADER_H
// header file for a class to store data from a loaded
// obj file

#include <fstream>
#include <array>
#include <vector>

class Loader
{
    public:
        
        /* Not yet implemented texture coords */
        typedef std::array<float, 3>        Vector3;
        typedef std::array<size_t, 3>       FaceData;
        typedef std::array<FaceData, 3>     Face;

        bool loadFile(const std::string& filename);

        size_t numberVertices() const;
        size_t numberFaces() const;
        size_t numberNormals() const;

        template<typename Con>
        std::size_t FillTriangleBatch(Con&&);

        float vertexData(size_t i, size_t j) const;
        

    private:

        template<typename Con>
        void appendVertices(Con&&, size_t&, const Face&);
        
        std::vector<Vector3>    m_Vertices;
        std::vector<Face>       m_Faces;
        std::vector<Vector3>    m_Normals;
        // std::vector<***>     m_VTextures;
};


template<typename Con>
void Loader::appendVertices(Con&& container, size_t& index, const Face& face)
{
    // cycle through each vertex of the (triangular) face
    for(size_t i = 0; i != 3; ++i)
    {
        // cycle through each coordinate in the vertex
        for(size_t j = 0; j != 3; ++j)
            container[index][j] = m_Vertices[face[i][0] - 1] [j] * 0.25f;
        
        ++index;
    }
}

template<typename Con>
std::size_t Loader::FillTriangleBatch(Con&& container)
{
    // we'll return the number of triangles
    std::size_t triCount = 0;

    // cycle through the faces
    for(auto itr = m_Faces.begin(); itr != m_Faces.end(); ++itr)
    {
        appendVertices(std::forward<Con>(container), triCount, *itr);
    }

    return triCount;
}


#endif