#include <scene.hpp>
#include <model.hpp>
#include <camera.hpp>
#include <shadermanager.hpp>
#include <shaderprogram.hpp>
#include <transform.hpp>
#include <stack>
#include <imgui/imgui.h>
#include <texturemanager.hpp>
#include <iostream>

namespace graf{
    int id = 1000;
    int current = 0;
    const char* items[] = {"Cube", "Pyramid", "Camera", "Cylinder"};

    void printMatrix(const glm::mat4& mtx)
{
    if (ImGui::BeginTable("Matrix", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::Text("%-.3f",mtx[0][0]);
        ImGui::TableNextColumn(); ImGui::Text("%-.3f",mtx[1][0]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[2][0]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[3][0]);
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[0][1]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[1][1]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[2][1]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[3][1]);
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[0][2]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[1][2]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[2][2]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[3][2]);
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[0][3]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[1][3]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[2][3]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[3][3]);
        ImGui::EndTable();
    }
}

    
    Scene::Scene()
    {
        m_root = new SceneObject();
        m_selectedObject = new SceneObject();
        m_root->m_name = "Root";
    }

    void Scene::drawScene(ViewPort* viewPort)
    {
        stack<SceneObject*> objects;
        objects.push(m_root);
        while(objects.empty()!=true)
        {
            auto next = objects.top();
            if(next->m_type == SceneObjectType::Camera)
                continue;
            next->transform->updateRotation();
            next->transform->update();
            next->updateChilds();
            objects.pop();
            if(next->getType()==SceneObjectType::Model)
            {
                Model* model = (Model*)next;
                string shaderName = model->getShaderName();
                string textureName = model->getTextureName();
                TextureManager::activateTexture(textureName);
                ShaderManager::getProgram(shaderName)->use();
                viewPort->activateCamera(ShaderManager::getProgram(shaderName));
                model->draw(ShaderManager::getProgram(shaderName));
                ShaderManager::getProgram(shaderName)->unuse();
            }
            for(auto nextChild:next->m_childs)
            {
                objects.push(nextChild);
            }
        }
    }

    void Scene::addViewPort(ViewPort* viewport){
        m_viewPortList.push_back(viewport); 
    }


    void Scene::drawGui()
    {
        id = 1000;
        ImGui::Begin("Scene");

        drawNextGuiNode(m_root);

        ImGui::End();

        ImGui::Begin("Properties");

        if (m_selectedObject)
        {
            ImGui::Text(m_selectedObject->getName().c_str());

            ImGui::PushID(id++);
            if (ImGui::CollapsingHeader("Transformation"))
            {
                ImGui::SliderFloat3("Position", &m_selectedObject->transform->position.x, -5.0f, 5.0f, "%.1f");
                ImGui::SliderFloat3("Rotation", &m_selectedObject->transform->euler.x, -360.0f, 360.0f, "%.1f");
                ImGui::SliderFloat3("Scale", &m_selectedObject->transform->scale.x, 0.1f, 5.0f, "%.1f");
            }

            if(ImGui::Button("Remove")){
                removeObject(m_selectedObject);
                findParentAndKill(m_selectedObject, m_root);
            }

            if(ImGui::Button("Attach to camera")){
                graf::Camera* camera = new Camera(100, 1600.0f/900.0f, 0.1f, 100.0f);
                camera->transform = m_selectedObject->transform;
                // camera.transform->position = m_selectedObject->transform->position;
                // camera.transform->euler = m_selectedObject->transform->euler;
                // camera.transform->scale = m_selectedObject->transform->scale;

                m_viewPortList[1]->attachCamera(camera);
            }

            ImGui::Combo("object", &current, items, IM_ARRAYSIZE(items));

            if(ImGui::Button("Create Shape")){
                graf::Model* model = Model::loadModel(items[current]);
                m_selectedObject->addChild(model);

                if(current == 2)
                {
                    graf::Model* modelCube = graf::Model::loadModel("Cube");
                    graf::Model* modelCylinder1 = graf::Model::loadModel("Cylinder");
                    graf::Model* modelCylinder2 = graf::Model::loadModel("Cylinder");
                    graf::Model* modelPyramid = graf::Model::loadModel("Pyramid");

                    modelCube->transform->position.x = 0.0f;
                    modelCube->transform->position.y = 0.0f;
                    modelCube->transform->position.z = -2.0f;
                    
                    modelCube->transform->euler.x = 0.0f;
                    modelCube->transform->euler.y = -90.0f;
                    modelCube->transform->euler.z = 0.0f;
                    
                    modelCube->transform->scale.x = 1.0f;
                    modelCube->transform->scale.y = 0.5f;
                    modelCube->transform->scale.z = 0.5f;

                    //        0.0,0.8,-1.5
                    modelCylinder1->transform->position.x = 0.0f;
                    modelCylinder1->transform->position.y = 0.8f;
                    modelCylinder1->transform->position.z = -1.5f;
                    
                    //        -90.0,90.0,0.0
                    modelCylinder1->transform->euler.x = -90.0f;
                    modelCylinder1->transform->euler.y = 90.0f;
                    modelCylinder1->transform->euler.z = 0.0f;
                    
                    //        0.2,0.2,0.3
                    modelCylinder1->transform->scale.x = 0.2f;
                    modelCylinder1->transform->scale.y = 0.2f;
                    modelCylinder1->transform->scale.z = 0.3f;

                    //        0.0,0.8,-2.5
                    modelCylinder2->transform->position.x = 0.0f;
                    modelCylinder2->transform->position.y = 0.8f;
                    modelCylinder2->transform->position.z = -2.5f;
                    
                    //        -90.0,90.0,0.0
                    modelCylinder2->transform->euler.x = -90.0f;
                    modelCylinder2->transform->euler.y = 90.0f;
                    modelCylinder2->transform->euler.z = 0.0f;
                    
                    //        0.2,0.2,0.3
                    modelCylinder2->transform->scale.x = 0.2f;
                    modelCylinder2->transform->scale.y = 0.2f;
                    modelCylinder2->transform->scale.z = 0.3f;

                    //        0.0,0.0,-0.5
                    modelPyramid->transform->position.x = 0.0f;
                    modelPyramid->transform->position.y = 0.0f;
                    modelPyramid->transform->position.z = -1.0f;

                    //        -90.0,0.0,0.0
                    modelPyramid->transform->euler.x = -90.0f;
                    modelPyramid->transform->euler.y = 0.0f;
                    modelPyramid->transform->euler.z = 0.0f;
                    
                    modelPyramid->transform->scale.x = 0.5f;
                    modelPyramid->transform->scale.y = 0.5f;
                    modelPyramid->transform->scale.z = 0.5f;

                    model->addChild(modelCube);
                    model->addChild(modelCylinder1);
                    model->addChild(modelCylinder2);
                    model->addChild(modelPyramid);
                }
            }

            ImGui::PopID();
        }


        ImGui::End();
    }

    void Scene::draw(){
        for(auto nextViewPort:m_viewPortList){
            nextViewPort->activateScissor();
            nextViewPort->clearBackground(nextViewPort->r, nextViewPort->g, nextViewPort->b, nextViewPort->unknown);
            nextViewPort->activate();

            drawScene(nextViewPort);
        }
    }

    void Scene::removeObject(SceneObject *sceneObject)
    {
        for (auto child : sceneObject->m_childs) {
            removeObject(child);
        }
        sceneObject->m_childs = {};
        if(m_selectedObject != sceneObject)
            delete sceneObject;
    }

    void Scene::addModelFromFile(const string &fileName)
    {
        Model* model = Model::loadModel(fileName);

        addSceneObject(model);
    }
    void Scene::addSceneObject(SceneObject *object)
    {
        m_root->addChild(object);
    }

    void Scene::drawNextGuiNode(SceneObject* current){

        ImGui::PushID(id++);
        
        if(ImGui::TreeNode(current->getName().c_str())){
            if (ImGui::IsItemClicked()) {
                m_selectedObject = current;
            }
            for(auto child:current->m_childs){
                drawNextGuiNode(child);
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
    void Scene::findParentAndKill(SceneObject *selectedObject, SceneObject* root)
    {
        for (auto child : root->m_childs) {
            if(child == selectedObject)
            {
                root->removeChild(child);
                child = nullptr;
                break;
            }
            findParentAndKill(selectedObject, child);
        }
    }
    SceneObject *Scene::findParent(SceneObject *selectedObject, SceneObject *root)
    {
        for (auto child : root->m_childs) {
            if(child == selectedObject)
            {
                return root;
            }
            findParent(selectedObject, child);
        }
    }
}