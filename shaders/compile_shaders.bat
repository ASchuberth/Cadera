%VULKAN_SDK%\Bin32\glslangValidator.exe -V %1/vertex.vert     -o %1/vert.spv
%VULKAN_SDK%\Bin32\glslangValidator.exe -V %1/gridvertex.vert -o %1/gridvert.spv
%VULKAN_SDK%\Bin32\glslangValidator.exe -V %1/fragment.frag   -o %1/frag.spv
%VULKAN_SDK%\Bin32\glslangValidator.exe -V %1/text.vert       -o %1/textvert.spv
%VULKAN_SDK%\Bin32\glslangValidator.exe -V %1/text.frag       -o %1/textfrag.spv

pause