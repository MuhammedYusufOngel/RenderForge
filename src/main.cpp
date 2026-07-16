#define GLM_ENABLE_EXPERIMENTAL
#include "glwindow.hpp"

#include<glm/glm.hpp>
#include "shaderprogram.hpp"
#include "vertexbuffer.hpp"
#include "vertexarrayobject.hpp"
#include "meshloader.hpp"
#include "model.hpp"
#include "texturemanager.hpp"
#include <glm/gtx/transform.hpp>
#include <transform.hpp>
#include <shadermanager.hpp>
#include <camera.hpp>
#include <viewport.hpp>
#include <scene.hpp>
#include<imgui/imgui.h>
using namespace std;

glm::mat4 projectionMatrix(float fov,float aspect,float n,float f)
{
    glm::mat4 mtxProj(0);
    float tanHalfFov = tan(fov/2.0f);
    mtxProj[0][0] = 1.0f/(aspect*tanHalfFov); //parantezi unutma sonuç değişiyor.
    mtxProj[1][1] = 1.0f/tanHalfFov;
    mtxProj[2][2] = (f+n)/(f-n);
    mtxProj[2][3] = 1.0f;
    mtxProj[3][2] = -2*f*n/(f-n);
    return mtxProj;
}

int main(int arc,char** argv)
{
    float width = 1600;
    float height = 900;
    graf::GLWindow window;
    window.create(width,height);

    graf::Model* sceneModel = graf::Model::loadModel("Scene");
    graf::Model* floor = graf::Model::loadModel("Floor");
    graf::Model* wall = graf::Model::loadModel("Wall");
    graf::Model* wall2 = graf::Model::loadModel("Wall2");
    graf::Model* wall3 = graf::Model::loadModel("Wall3");
    graf::Model* wall4 = graf::Model::loadModel("Wall4");

    graf::Model* circle = graf::Model::loadModel("Circle_UnlitTextured");
    graf::Model* cylinder = graf::Model::loadModel("Cylinder");

    graf::Transform transform;
    graf::Transform cameraTransform;
    graf::Camera camera(100, width/height, 0.1f, 100.0f);
    graf::Camera camera1(100, width/height, 0.1f, 100.0f);
    graf::ViewPort* viewport[2];
    graf::Scene scene;

    viewport[0] = new graf::ViewPort(0, 0, width,height, 0.0f, 0.4f, 0.7f, 1.0f);
    viewport[0]->attachCamera(&camera);
    scene.addViewPort(viewport[0]);
    
    viewport[1] = new graf::ViewPort(width-300.0f, height-200.0f, 300.0f,200.0f, 0.0f, 0.0f, 0.7f, 1.0f);
    viewport[1]->attachCamera(&camera1);
    scene.addViewPort(viewport[1]);

    sceneModel->addChild(floor);
    sceneModel->addChild(wall);
    sceneModel->addChild(wall2);
    sceneModel->addChild(wall3);
    sceneModel->addChild(wall4);

    scene.addSceneObject(sceneModel);

    scene.addSceneObject(cylinder);

    float angle = 0.0f; 

    //---------------------------------------------------------------------------
    //                  KLAVYE FONKSİYONU
    //---------------------------------------------------------------------------    

    window.setKeyboardFunction([&](int key,int action){
        if(key==GLFW_KEY_A)       camera.transform->moveLeft()     ;
        if(key==GLFW_KEY_D)         camera.transform->moveRight()   ;
        if(key==GLFW_KEY_W)           camera.transform->moveForward() ;
        if(key==GLFW_KEY_S)        camera.transform->moveBackward()    ;      

    });

    
    window.setMouseMoveFunction([&](double x, double y){

        if(window.isMouseRightButtonPressed()){
            camera.transform->euler.y += x;
            camera.transform->euler.x += y;
        }
    });
    

    window.setGuiFunction([&](){
        scene.drawGui();
    });


    window.setRenderFunction([&](){
        scene.draw();
    });

    window.begin();
    exit(EXIT_SUCCESS);
}