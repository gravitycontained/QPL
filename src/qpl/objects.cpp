#include <qpl/objects.hpp>
#if defined(QPL_USE_VULKAN) || defined(QPL_USE_ALL)

namespace qpl {
    std::array<VkVertexInputBindingDescription, static_cast<qpl::size>(qpl::vk::vertex_binding::count)> qpl::vk::get_binding_descriptions() {
       std::array<VkVertexInputBindingDescription, static_cast<qpl::size>(qpl::vk::vertex_binding::count)> attribute_descriptions = {};

       attribute_descriptions[static_cast<qpl::size>(qpl::vk::vertex_binding::vertex_3D_tex)].binding = static_cast<qpl::u32>(qpl::vk::vertex_binding::vertex_3D_tex);
       attribute_descriptions[static_cast<qpl::size>(qpl::vk::vertex_binding::vertex_3D_tex)].stride = sizeof(qpl::vertex_3D_tex);
       attribute_descriptions[static_cast<qpl::size>(qpl::vk::vertex_binding::vertex_3D_tex)].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

       attribute_descriptions[static_cast<qpl::size>(qpl::vk::vertex_binding::vertex_2D)].binding = static_cast<qpl::u32>(qpl::vk::vertex_binding::vertex_2D);
       attribute_descriptions[static_cast<qpl::size>(qpl::vk::vertex_binding::vertex_2D)].stride = sizeof(qpl::vertex_2D);
       attribute_descriptions[static_cast<qpl::size>(qpl::vk::vertex_binding::vertex_2D)].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

       attribute_descriptions[static_cast<qpl::size>(qpl::vk::vertex_binding::vertex_3D)].binding = static_cast<qpl::u32>(qpl::vk::vertex_binding::vertex_3D);
       attribute_descriptions[static_cast<qpl::size>(qpl::vk::vertex_binding::vertex_3D)].stride = sizeof(qpl::vertex_3D);
       attribute_descriptions[static_cast<qpl::size>(qpl::vk::vertex_binding::vertex_3D)].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

       attribute_descriptions[static_cast<qpl::size>(qpl::vk::vertex_binding::vertex_2D_tex)].binding = static_cast<qpl::u32>(qpl::vk::vertex_binding::vertex_2D_tex);
       attribute_descriptions[static_cast<qpl::size>(qpl::vk::vertex_binding::vertex_2D_tex)].stride = sizeof(qpl::vertex_2D_tex);
       attribute_descriptions[static_cast<qpl::size>(qpl::vk::vertex_binding::vertex_2D_tex)].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;


       return attribute_descriptions;
   }

   std::array<VkVertexInputAttributeDescription, 3> qpl::vk::get_attribute_descriptions() {
       std::array<VkVertexInputAttributeDescription, 3> attribute_descriptions = {};

       qpl::u32 ctr = 0u;



       ////vertex_2D
       //attribute_descriptions[ctr].binding = static_cast<qpl::u32>(qpl::vk::vertex_binding::vertex_2D);
       //attribute_descriptions[ctr].location = 0u;
       //attribute_descriptions[ctr].format = VK_FORMAT_R32G32B32_SFLOAT;
       //attribute_descriptions[ctr].offset = offsetof(qpl::vertex_2D, qpl::vertex_2D::position);
       //++ctr;
       //
       //attribute_descriptions[ctr].binding = static_cast<qpl::u32>(qpl::vk::vertex_binding::vertex_2D);
       //attribute_descriptions[ctr].location = 1u;
       //attribute_descriptions[ctr].format = VK_FORMAT_R32G32B32_SFLOAT;
       //attribute_descriptions[ctr].offset = offsetof(qpl::vertex_2D, qpl::vertex_2D::color);
       //++ctr;
       //
       //
       ////vertex_3D
       //attribute_descriptions[ctr].binding = static_cast<qpl::u32>(qpl::vk::vertex_binding::vertex_3D);
       //attribute_descriptions[ctr].location = 0u;
       //attribute_descriptions[ctr].format = VK_FORMAT_R32G32B32_SFLOAT;
       //attribute_descriptions[ctr].offset = offsetof(qpl::vertex_3D, qpl::vertex_3D::position);
       //++ctr;
       //
       //attribute_descriptions[ctr].binding = static_cast<qpl::u32>(qpl::vk::vertex_binding::vertex_3D);
       //attribute_descriptions[ctr].location = 1u;
       //attribute_descriptions[ctr].format = VK_FORMAT_R32G32B32_SFLOAT;
       //attribute_descriptions[ctr].offset = offsetof(qpl::vertex_3D, qpl::vertex_3D::color);
       //++ctr;
       //
       //
       ////vertex_2D_tex
       //attribute_descriptions[ctr].binding = static_cast<qpl::u32>(qpl::vk::vertex_binding::vertex_2D_tex);
       //attribute_descriptions[ctr].location = 0u;
       //attribute_descriptions[ctr].format = VK_FORMAT_R32G32B32_SFLOAT;
       //attribute_descriptions[ctr].offset = offsetof(qpl::vertex_2D_tex, qpl::vertex_2D_tex::position);
       //++ctr;
       //
       //attribute_descriptions[ctr].binding = static_cast<qpl::u32>(qpl::vk::vertex_binding::vertex_2D_tex);
       //attribute_descriptions[ctr].location = 1u;
       //attribute_descriptions[ctr].format = VK_FORMAT_R32G32B32_SFLOAT;
       //attribute_descriptions[ctr].offset = offsetof(qpl::vertex_2D_tex, qpl::vertex_2D_tex::color);
       //++ctr;
       //
       //attribute_descriptions[ctr].binding = static_cast<qpl::u32>(qpl::vk::vertex_binding::vertex_2D_tex);
       //attribute_descriptions[ctr].location = 2u;
       //attribute_descriptions[ctr].format = VK_FORMAT_R32G32B32_SFLOAT;
       //attribute_descriptions[ctr].offset = offsetof(qpl::vertex_2D_tex, qpl::vertex_2D_tex::tex_coord);
       //++ctr;


       //vertex_3D_tex
       attribute_descriptions[ctr].binding = static_cast<qpl::u32>(qpl::vk::vertex_binding::vertex_3D_tex);
       attribute_descriptions[ctr].location = 0u;
       attribute_descriptions[ctr].format = VK_FORMAT_R32G32B32_SFLOAT;
       attribute_descriptions[ctr].offset = offsetof(qpl::vertex_3D_tex, qpl::vertex_3D_tex::position);
       ++ctr;

       attribute_descriptions[ctr].binding = static_cast<qpl::u32>(qpl::vk::vertex_binding::vertex_3D_tex);
       attribute_descriptions[ctr].location = 1u;
       attribute_descriptions[ctr].format = VK_FORMAT_R32G32B32_SFLOAT;
       attribute_descriptions[ctr].offset = offsetof(qpl::vertex_3D_tex, qpl::vertex_3D_tex::color);
       ++ctr;

       attribute_descriptions[ctr].binding = static_cast<qpl::u32>(qpl::vk::vertex_binding::vertex_3D_tex);
       attribute_descriptions[ctr].location = 2u;
       attribute_descriptions[ctr].format = VK_FORMAT_R32G32B32_SFLOAT;
       attribute_descriptions[ctr].offset = offsetof(qpl::vertex_3D_tex, qpl::vertex_3D_tex::tex_coord);
       ++ctr;

       return attribute_descriptions;
   }
}

#endif