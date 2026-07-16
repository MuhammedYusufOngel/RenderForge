#include "sceneobject.hpp"
#include "transform.hpp"
#include <algorithm>
#include <iostream>

namespace graf{

    SceneObject::SceneObject()
    {
        transform = new Transform();
        m_name = "Empty";
    }

    SceneObject::~SceneObject()
    {
        // delete transform;
        // std::cout << "Deleted object successfully:::" << m_name << std::endl;
    }

    void SceneObject::addChild(SceneObject *child)
    {
        m_childs.push_back(child);
    }

    void SceneObject::removeChild(SceneObject *child)
    {
        auto it = std::find(m_childs.begin(), m_childs.end(), child);
        m_childs.erase(it);
    }

    SceneObjectType SceneObject::getType() const
    {
        return m_type;
    }
    string SceneObject::getName() const
    {
        return m_name;
    }
    void SceneObject::updateChilds()
    {
        auto combinedMatrix = transform->parentCombinedMatrix*transform->worldMatrix;

        for(auto nextChild:m_childs){
            nextChild->transform->parentCombinedMatrix = combinedMatrix;
            nextChild->updateChilds();
        }
    }
}