#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace graf{

    class Camera;
    class ShaderProgram;
    class ViewPort{
        public:
            ViewPort(int x, int y, int width, int height, float r, float g, float b, float unknown);

            void activate();
            void activateCamera(ShaderProgram* program);
            void activateScissor();
            void clearBackground(float r, float g, float b, float a);
            void attachCamera(Camera* camera);
            Camera* getCamera();

            int x;
            int y;
            int width;
            int height;
            float r;
            float g;
            float b;
            float unknown;

        private:
            Camera* m_camera;
    };

    using ViewPortList = std::vector<ViewPort*>;
}