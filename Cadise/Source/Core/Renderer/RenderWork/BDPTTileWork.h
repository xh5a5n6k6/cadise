#pragma once

#include "Core/Renderer/RenderWork/TileWork.h"

#include "Core/Gear/ConnectEvent.h"

#include <vector>

// forward declaration
namespace cadise
{
    class Camera;
    class Sampler;
    class Scene;
}

namespace cadise
{

class BDPTTileWork : public TileWork
{
public:
    BDPTTileWork(
        const Scene* const   scene,
        const Camera* const  camera,
        const Sampler* const sampler);

    void work() const override;

    void setConnectEvents(std::vector<ConnectEvent>* const connectEvents);

private:
    const Scene*   _scene;
    const Camera*  _camera;
    const Sampler* _sampler;

    std::vector<ConnectEvent>* _connectEvents;

    const static std::size_t MAX_PATH_LENGTH = 16;
};

} // namespace