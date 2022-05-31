#ifndef HF_SIMULATOR_HPP
#define HF_SIMULATOR_HPP

//─────────────────────────────────────────────────────────────────────────────────────────────────

#define HF_SIMULATOR_VERSION_MAJOR 1
#define HF_SIMULATOR_VERSION_MINOR 0

//─────────────────────────────────────────────────────────────────────────────────────────────────

// Simulator configuration

#define HF_DEVICE_FIELDS_AS_ARRAYS true

// Debugging configuration

#define HF_SYNCHRONIZE_AT_KERNELS      false
#define HF_SYNCHRONIZE_AT_MEM_TRANSFER false

// Interop configuration

#define HF_SUPPORT_OPENGL_INTEROP true
#define HF_SUPPORT_D3D9_INTEROP   false
#define HF_SUPPORT_D3D10_INTEROP  false
#define HF_SUPPORT_D3D11_INTEROP  false
#define HF_SUPPORT_VULKAN_INTEROP true

// GLM configuration

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_CUDA
#define GLM_FORCE_SWIZZLE

//─────────────────────────────────────────────────────────────────────────────────────────────────

// STL headers.

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

// CUDA headers.

#include <cuda.h>
#include <cuda_runtime.h>

// GLM headers.

#if defined(__CUDACC__)
#pragma push
#pragma diag_suppress=2906 
#endif
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_aligned.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/component_wise.hpp>
#if defined(__CUDACC__)
#pragma pop
#endif

// OpenGL headers (if supported).

#if HF_SUPPORT_OPENGL_INTEROP == true
#define NOMINMAX
#include <GL/glcorearb.h>
#include <cuda_gl_interop.h>
#endif

// Core framework headers

#include <Simulator/Utility/Macros.hpp>
#include <Simulator/Utility/Types.hpp>
#include <Simulator/Utility/Functions.hpp>
#include <Simulator/Utility/Assertions.hpp>

//─────────────────────────────────────────────────────────────────────────────────────────────────

#endif /* HF_SIMULATOR_HPP */
