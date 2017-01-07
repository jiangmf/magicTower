#include <vector>
#include <sstream>
#include <string>
#include <cstring>
#include <stdio.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

using namespace std;

// represents a vertex or a vector
struct Vertex{
    float x;
    float y;
    float z;
};
// represents a face with indexes for its
// vertexes, uvs and normals
struct Face{
    int v1;
    int v2;
    int v3;
    int t1;
    int t2;
    int t3;
    int n1;
    int n2;
    int n3;
};

// a model loaded by an obj loader
class Model{
private:
public:
    // a model has a list of vertexes, normals, uvs, faces, and the name of the obj file
    vector<Vertex> vertexes; 
    vector<Vertex> normals;
    vector<Vertex> textureCordinates;
    vector<Face> faces;
    string file_name;
    // load the model on initialization
    Model(string fname){
        file_name = fname;
        load();
    }
    void load(void){
        printf("LOADING %s ...\n", file_name.c_str());
        FILE * file = fopen(("resources/"+file_name).c_str(), "r");
        while(true){
            char lineHeader[128];
            // read the first word of the line
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
                break; // EOF = End Of File. Quit the loop.
            // parse and store the vertex
            if (strcmp( lineHeader, "v" )==0){
                Vertex vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
                vertexes.push_back(vertex);
            // parse and store the texture coordinate
            }else if ( strcmp( lineHeader, "vt" ) == 0 ){
                Vertex textureCordinate;
                fscanf(file, "%f %f\n", &textureCordinate.x, &textureCordinate.y );
                textureCordinates.push_back(textureCordinate);
            // parse and store the normal
            }else if ( strcmp( lineHeader, "vn" ) == 0 ){
                Vertex normal;
                fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
                normals.push_back(normal);
            // parse and store the face
            }else if ( strcmp( lineHeader, "f" ) == 0 ){
                Face face;
                int temp[9];
                char line[128];
                fgets (line , 128 , file);
                
                int matches = sscanf(line, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &temp[0],&temp[1],&temp[2],&temp[3],&temp[4],&temp[5],&temp[6],&temp[7],&temp[8]);

                if (matches != 9){
                    // the model might not have texture coordinates
                    int matches = sscanf(line, "%d//%d %d//%d %d//%d\n", &temp[0],&temp[2],&temp[3],&temp[5],&temp[6],&temp[8]);
                    if (matches != 6){
                        printf("Failed to load Face\n");
                    } else {
                        // subtract 1 from index as obj faces are 1 indexed
                        face.v1 = temp[0] -1;
                        face.t1 = 0;
                        face.n1 = temp[2] -1;
                        face.v2 = temp[3] -1;
                        face.t2 = 0;
                        face.n2 = temp[5] -1;
                        face.v3 = temp[6] -1;
                        face.t3 = 0;
                        face.n3 = temp[8] -1;
                    }
                } else {
                    // subtract 1 from index as obj faces are 1 indexed
                    face.v1 = temp[0] - 1;
                    face.t1 = temp[1] - 1;
                    face.n1 = temp[2] - 1;
                    face.v2 = temp[3] - 1;
                    face.t2 = temp[4] - 1;
                    face.n2 = temp[5] - 1;
                    face.v3 = temp[6] - 1;
                    face.t3 = temp[7] - 1;
                    face.n3 = temp[8] - 1;
                }
                faces.push_back(face);
            }else{
                // eat up the rest of the line
                char fuffer[1000];
                fgets(fuffer, 1000, file);
            }
        }
    }
    // draw the model
    void draw(void){
        bool hasTexture = textureCordinates.size() > 0;
        //iterate through the faces
        for(int i=0; i<faces.size(); i++){
            glBegin(GL_POLYGON);
                // set the texture coordinate and normal
                // then set the vertex, for each vertex in the face
                Face face = faces[i];
                if(hasTexture){
                    glTexCoord2f(textureCordinates[face.t1].x,  textureCordinates[face.t1].y);
                }
                glNormal3f( normals[face.n1].x,  normals[face.n1].y,  normals[face.n1].z);
                glVertex3f(vertexes[face.v1].x, vertexes[face.v1].y, vertexes[face.v1].z);
                if(hasTexture){
                    glTexCoord2f(textureCordinates[face.t2].x,  textureCordinates[face.t2].y);
                }
                glNormal3f( normals[face.n2].x,  normals[face.n2].y,  normals[face.n2].z);
                glVertex3f(vertexes[face.v2].x, vertexes[face.v2].y, vertexes[face.v2].z);
                if(hasTexture){
                    glTexCoord2f(textureCordinates[face.t3].x,  textureCordinates[face.t3].y);
                }
                glNormal3f( normals[face.n3].x,  normals[face.n3].y,  normals[face.n3].z);
                glVertex3f(vertexes[face.v3].x, vertexes[face.v3].y, vertexes[face.v3].z);
            glEnd();
        }
    }
};
// load the models
Model cubeModel = Model("cube.obj");
Model keyModel = Model("key2.obj");
Model potionModel = Model("potion2.obj");
Model gemModel = Model("gem.obj");
Model stairModel = Model("stair.obj");
Model monster1Model = Model("monster1.obj");
Model monster2Model = Model("monster2.obj");
Model monster3Model = Model("monster3.obj");
Model monster4Model = Model("monster4.obj");
Model monster5Model = Model("monster5.obj");
Model monster6Model = Model("monster6.obj");
Model monster7Model = Model("monster7.obj");
Model monster8Model = Model("monster8.obj");
Model swordModel = Model("sword.obj");
Model shieldModel = Model("shield.obj");