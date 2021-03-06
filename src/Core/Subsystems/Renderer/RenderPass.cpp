#include "RenderPass.h"
#include "Swapchain.h"
#include "LogicalDevice.h"

#include <array>

extern std::unique_ptr<Swapchain>       g_swapchain;
extern std::unique_ptr<LogicalDevice>   g_logicalDevice;

RenderPass::RenderPass()
{
    const VkAttachmentDescription colorAttachment =
    {
        .flags = 0,
        .format = g_swapchain->format(),
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    };
    const VkAttachmentReference colorAttachmentRef =
    {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };
    const VkAttachmentDescription depthAttachment =
    {
        .flags = 0,
        .format = VK_FORMAT_D32_SFLOAT,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };
    const VkAttachmentReference depthAttachmentRef =
    {
        .attachment = 1,
        .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };
    const VkSubpassDescription subpass =
    {
        .flags = 0,
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .inputAttachmentCount = 0,
        .pInputAttachments = VK_NULL_HANDLE,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentRef,
        .pResolveAttachments = VK_NULL_HANDLE,
        .pDepthStencilAttachment = &depthAttachmentRef,
        .preserveAttachmentCount = 0,
        .pPreserveAttachments = VK_NULL_HANDLE,
    };
    const VkSubpassDependency dependency =
    {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .srcAccessMask = 0,
        .dstAccessMask= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    };
    std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
    const VkRenderPassCreateInfo renderPassInfo =
    {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = VK_NULL_HANDLE,
        .flags = 0,
        .attachmentCount = static_cast<uint32_t>(attachments.size()),
        .pAttachments = attachments.data(),
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 1,
        .pDependencies = &dependency,
    };

    CHECK("Render pass", vkCreateRenderPass(g_logicalDevice->vkDevice(), &renderPassInfo, VK_NULL_HANDLE, &_vkRenderPass));
}

RenderPass::~RenderPass()
{
    vkDestroyRenderPass(g_logicalDevice->vkDevice(), _vkRenderPass, VK_NULL_HANDLE);
}

const VkRenderPass& RenderPass::vkRenderPass() const
{
    return _vkRenderPass;
}

void RenderPass::begin(const VkCommandBuffer vkCommandBuffer, const VkFramebuffer vkFramebuffer) const
{
    const std::array<VkClearValue, 2> clearValues =
    {
        VkClearValue
        {
            .color = 
            VkClearColorValue
            {
                .float32 = {0.0f, 0.0f, 0.0f, 1.0f},
            }
        },
        {
            .depthStencil = 
            VkClearDepthStencilValue
            {
                .depth = 1.0f,
                .stencil = 0,
            }
        }
    };
    const VkRenderPassBeginInfo renderPassBeginInfo =
    {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext = VK_NULL_HANDLE,
        .renderPass = _vkRenderPass,
        .framebuffer = vkFramebuffer,
        .renderArea = 
        {
            .offset = {0, 0},
            .extent = g_swapchain->extent(),
        },
        .clearValueCount = static_cast<uint32_t>(clearValues.size()),
        .pClearValues = clearValues.data(),
    };

    vkCmdBeginRenderPass(vkCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void RenderPass::end(const VkCommandBuffer vkCommandBuffer) const
{
    vkCmdEndRenderPass(vkCommandBuffer);
}
