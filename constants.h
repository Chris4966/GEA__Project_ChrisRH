#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

#define MAX_ENTITIES 65535
#define MAX_ENTITIES_TYPE unsigned short    //have to hold MAX_ENTITIES !!!
#define MAX_COMPONENTS 100                  //could be made dynamic by using std::vectors

namespace gsl  //Game School Lib
{
    const std::string ProjectFolderName{"../GEA__Project_ChrisRH/"};
    const std::string AssetFilePath{ProjectFolderName + "Assets/"};
    const std::string ShaderFilePath{ProjectFolderName + "Shaders/"};
    const std::string MeshFilePath{AssetFilePath + "Meshes/"};
    const std::string TextureFilePath{AssetFilePath + "Textures/"};
    const std::string SoundFilePath{AssetFilePath + "Sounds/"};
    const std::string ScriptFilePath{ProjectFolderName + "Scripts/"};

    enum Shaders
    {
        COLORSHADER = 0,
        TEXTURESHADER,
        PHONGSHADER,
        NORMALSSHADER,
        SKYBOXSHADER,
        MOUSEPICKERSHADER,
        SINGLECOLORSHADER,
        HUDSHADER,
        ENDSHADER,           //shows number of shaders
        NOSHADER = 65535
    };

    const int NumberOfShaders{8};
    const int NumberOfTextures{25};
    const int NumberOfMaterials{25};

    const int EditorAssetNumber{3};

    enum EditorAssets
    {
        GIZMO,
        GRID,
        AXIS
    };

    enum LightTypes
    {
        SPOT,
        POINT,
        DIRECTIONAL
    };

    enum Components
    {
        TRANSFORM,
        COLLIDER
    };

    enum Colliders
    {
        NOCOLLIDER = -1,
        OBB,
        SPHERE
    };
}

#endif // CONSTANTS_H
