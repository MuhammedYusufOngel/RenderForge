#pragma once
#include <sceneobject.hpp>
#include <viewport.hpp>

namespace graf{

    class Model;
    class Scene{
        private:
            ViewPortList m_viewPortList;
            SceneObject* m_root;
            SceneObject* m_selectedObject;

            void drawNextGuiNode(SceneObject* current);
            void findParentAndKill(SceneObject* selectedObject, SceneObject* root);
            SceneObject* findParent(SceneObject* selectedObject, SceneObject* root);

        public:
            Scene();

            void draw();
            void removeObject(SceneObject* sceneObject);
            void drawScene(ViewPort* viewPort);
            void drawGui();
            void addModelFromFile(const string& fileName);
            void addSceneObject(SceneObject* object);
            void addViewPort(ViewPort* viewport);

    };
}