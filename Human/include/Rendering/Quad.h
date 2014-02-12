#ifndef OPENGINE_HUMAN_RENDERER_QUAD
#define OPENGINE_HUMAN_RENDERER_QUAD

#include "./Human/include/Rendering/Buffer.h"
#include "./Human/include/Rendering/Mesh.h"
#include "./Human/include/Rendering/MeshPacker.h"
#include "./Human/include/Rendering/MeshPacked.h"
#include "./Math/include/Vector2.h"
#include "./Core/include/Types.h"



// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
//   _____ _                   _       
//  / ____| |                 | |      
// | (___ | |_ _ __ _   _  ___| |_ ___ 
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/


//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _               
// |  __ \                   |  __ \                |  __ \(_)             | | (_)              
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___ 
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/   

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPmesh       OPquadCreate();
OPmesh OPquadCreateCustom(OPfloat width, OPfloat height, OPvec2 offset, OPvec2 texcoordStart, OPvec2 texcoordEnd);
OPmeshPacked OPquadCreatePacked();
OPmesh       OPquadNormCreate();
OPmeshPacked OPquadNormCreatePacked();

// prevent name mangling if compiling with c++
#ifdef __cplusplus
};
#endif

#endif
 
