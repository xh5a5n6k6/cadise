#pragma once

#include "core/renderer/render-work/tileWork.h"

#include "core/integral-tool/connectEvent.h"

#include <vector>

namespace cadise { class Camera; }
namespace cadise { class Sampler; }
namespace cadise { class Scene; }

namespace cadise 
{

class BdptTileWork : public TileWork 
{
public:
    BdptTileWork(
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