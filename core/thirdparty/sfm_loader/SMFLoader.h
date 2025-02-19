#ifndef SFM_LOADER_H
#define SFM_LOADER_H

class SMFLoader {
public:
    SMFLoader();
    ~SMFLoader() { clear(); }

    void load(const char* filename);

    float* getVertices();
    int* getFaces();
    float* getColors();

    // Get a specific face's vertex positions as an array of 9 floats
    float* getFace(int index);

    // Get a specific face's vertex colors as an array of 9 floats
    float* getFaceColor(int index);

    // Get a specific face's vertex normal as an array of 9 floats
    float* getNormal(int index);

    // Getters for sizes of vertices, faces, and colors
    int getVertexSize();
    int getFaceSize();
    int getColorSize();
    
private:
    float* vertex; 
    int* face;     
    float* color;  
    float* normal;  

    int vertexSize; // Number of vertices
    int faceSize;   // Number of faces
    int colorSize;  // Number of color entries
    int normalSize;  // Number of normal entries

    void clear();
};

#endif