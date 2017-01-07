#include <vector>

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

// the material class store the ambient diffuse specular emission and shiny properties
// of a material
class Material {
private:
    vector<float> mat_amb; 
    vector<float> mat_dif;
    vector<float> mat_spe;
    vector<float> mat_emi;
    float mat_shi;
public:
    Material(
        float amb1, float amb2, float amb3, float amb4,
        float dif1, float dif2, float dif3, float dif4,
        float spe1, float spe2, float spe3, float spe4,
        float emi1, float emi2, float emi3, float emi4,
        float shi
    ) {

        mat_amb.insert(mat_amb.end(), {amb1, amb2, amb3, amb4});
        mat_dif.insert(mat_dif.end(), {dif1, dif2, dif3, dif4});
        mat_spe.insert(mat_spe.end(), {spe1, spe2, spe3, spe4});
        mat_emi.insert(mat_emi.end(), {emi1, emi2, emi3, emi4});
        mat_shi = shi;
    }
    // set the current material to this material
    void set(){
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  &mat_amb[0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  &mat_dif[0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &mat_spe[0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &mat_emi[0]);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shi);
    }
};

// define the materials

Material playerMaterial = Material(
    0.0215f,  0.1745f,   0.0215f,  0.55f,
    0.07568f, 0.61424f,  0.07568f, 0.55f,
    0.633f,   0.727811f, 0.633f,   0.55f,
    0.0f,     0.0f,      0.0f,     0.0f,
    76.8f
);
Material wallMaterial = Material(
    0.19225f, 0.19225f, 0.19225f, 1.0f,
    0.50754f, 0.50754f, 0.50754f, 1.0f,
    0.508273f, 0.508273f, 0.508273f, 1.0f,
    0,0,0,0,
    51.2f
);
Material floorMaterial = Material(
    0.02f, 0.02f, 0.02f, 1.0f,
    0.01f, 0.01f, 0.01f, 1.0f,
    0.4f,  0.4f,  0.4f,  1.0f,
    0.0f,  0.0f,  0.0f,  0.0f,
    10.0f
);
Material yellowMaterial = Material(
    1.00f, 0.92f, 0.23f, 1.00f,
    0.75f, 0.61f, 0.23f, 1.00f,
    0.63f, 0.56f, 0.37f, 1.00f,
    0.00f, 0.00f, 0.00f, 0.00f,
    51.2f
);
Material blueMaterial = Material(
    0.25f, 0.32f, 0.71f, 0.55f,
    0.10f, 0.14f, 0.49f, 0.55f,
    0.77f, 0.79f, 0.91f, 0.55f,
    0.00f, 0.00f, 0.00f, 0.00f,
    76.8f
);
Material redMaterial = Material(
    0.96f, 0.26f, 0.21f, 0.55f,
    0.61f, 0.04f, 0.04f, 0.55f,
    0.73f, 0.63f, 0.63f, 0.55f,
    0.00f, 0.00f, 0.00f, 0.00f,
    76.8f
);

Material yellowKeyMaterial = Material(
    0.75f, 0.61f, 0.23f, 1.00f,
    0.75f, 0.61f, 0.23f, 1.00f,
    0.75f, 0.61f, 0.23f, 1.00f,
    0.75f, 0.61f, 0.23f, 1.00f,
    51.2f
);
Material blueKeyMaterial = Material(
    0.25f, 0.32f, 0.71f, 0.55f,
    0.10f, 0.14f, 0.49f, 0.55f,
    0.77f, 0.79f, 0.91f, 0.55f,
    0.10f, 0.14f, 0.49f, 0.55f,
    76.8f
);
Material redKeyMaterial = Material(
    0.96f, 0.26f, 0.21f, 0.55f,
    0.61f, 0.04f, 0.04f, 0.55f,
    0.73f, 0.63f, 0.63f, 0.55f,
    0.61f, 0.04f, 0.04f, 0.55f,
    76.8f
);
Material defaultMaterial = Material(
    0.20f, 0.20f, 0.20f, 1.00f,
    0.80f, 0.80f, 0.80f, 1.00f,
    0.00f, 0.00f, 0.00f, 1.00f,
    0.0f,  0.0f,  0.0f,  0.0f,
    0.00f
);