#pragma once

#include "Mesh.h"

/**
 * load a dem mesh.
 * \param path dem file path.
 * \param max_height out max height.
 * \return a dem mesh.
 */
Mesh load_dem(const char* path, float &max_height);



