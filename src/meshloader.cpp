#include "meshloader.hpp"
#include <fstream>
#include<iostream>
#include<nlohmann/json.hpp>
#include <vector>
#include "vertexbuffer.hpp"
#include "vertexarrayobject.hpp"
#include "indexbuffer.hpp"

namespace graf
{
    string ShapeType::Circle = "Circle";
    string ShapeType::Camera = "Camera";
    string ShapeType::Cylinder = "Cylinder";
    string ShapeType::Cone = "Cone";
    string ShapeType::Scene = "Scene";

    MeshLoader* MeshLoader::m_instance=nullptr;
    MeshLoader::MeshLoader()
    {
        
    }
    VertexArrayObject* MeshLoader::loadMeshFromFile(const string& fileName)
    {
        VertexArrayObject* vao=nullptr;
        auto loader = MeshLoader::getInstance();

        if(loader->m_vaos.count(fileName) == 0)
        {
            string path = "../meshes/" + fileName;
            ifstream file(path);

            if(file.is_open())
            {
                cout<<"<------MeshLoader readling file "<<fileName<<"------->"<<endl;
                using namespace nlohmann;

                json data = json::parse(file);

                /*---------------------Parsing Mesh File--------------------------*/

                string meshName = data["MeshName"];
                int vertexAttributeCount = data["VertexAttributeCount"];
                vector<string> vertexAttributes = data["VertexAttributes"];
                int triangleCount = data["TriangleCount"];
                vector<float> vertices = data["Vertices"];
                vector<unsigned int> indices = data["Indices"];
                int vertexFloatCount = getVertexFloatCount(vertexAttributes);


                /*---------------------Creating Buffers--------------------------*/
                VertexBuffer* vbo = new VertexBuffer();
                vbo->create(&vertices[0],vertices.size()*sizeof(float));

                IndexBuffer* ibo = new IndexBuffer();
                ibo->create(indices.data(), indices.size()*sizeof(unsigned int));

                vao = new VertexArrayObject();
                vao->create();
                vao->setVertexBuffer(vbo);
                vao->setIndexBuffer(ibo);
                for(auto nextAttribute:vertexAttributes)
                    vao->addAttributes(toAttributeType(nextAttribute));
                vao->activateAttributes();

                /*---------------------Printing Log Info--------------------------*/

                cout<<setw(20)<<std::left<<"MeshName"<<":"<<meshName<<endl;
                cout<<setw(20)<<std::left<<"VertexAttributeCount"<<":"<<vertexAttributeCount<<endl;
                cout<<setw(20)<<std::left<<"VertexAttributes"<<":";
                for(auto nextAttribute:vertexAttributes)
                    cout<<nextAttribute<<" ";
                cout<<endl;
                cout<<setw(20)<<std::left<<"TriangleCount"<<":"<<triangleCount<<endl;
                
                cout<<setw(30)<<"Vertices"<<":"<<endl;
                    
                for(int i=0;i<vertices.size();i+=vertexFloatCount)
                {
                    cout<<std::right<<setw(10)<<"Vertex"<<""<<setw(10)<<i/vertexFloatCount<<setw(5)<<" =>";
                    for(int j =0;j<vertexFloatCount;j++)
                    {
                        cout<<setw(6)<<std::right<<vertices[i+j];
                    }
                    cout<<endl;
                }           
            }
            else
            {
                cout<<"<------MeshLoader cannot open "<<fileName<<"------->"<<endl;
            }

        }
        else{
            vao = loader->m_vaos[fileName]; 
        }
        return vao;

    }
    
    VertexArrayObject *MeshLoader::createMesh(const string &shapeType)
    {
        if(shapeType==ShapeType::Circle)
            return createCircle();
            
        if(shapeType==ShapeType::Cone)
            return createCone();
        
        if(shapeType==ShapeType::Cylinder)
            return createCylinder();

        if(shapeType==ShapeType::Camera)
            return createCamera();

        if(shapeType==ShapeType::Scene)
            return createScene();
        return nullptr;
    }
    int MeshLoader::getVertexFloatCount(const vector<string> &attributes)
    {
        int floatCount =0;

        for(auto nextAttribute:attributes)
        {
            if( nextAttribute=="Position"||
                nextAttribute=="Normal"
                )
                floatCount+=3;
            if(nextAttribute=="Color")
                floatCount+=4;       
            if(nextAttribute=="Texture")
                floatCount+=2;            
        }

        return floatCount;
    }

    VertexArrayObject *MeshLoader::createCircle()
    {
        VertexArrayObject* vao=nullptr;

        auto loader = getInstance();

        if(loader->m_vaos.count(ShapeType::Circle)==0)
        {
            vector<VertexUnlit> vertices;
            vector<unsigned int> indices;

            float angle =10;

            int vertexCount = 360.0f/angle;

            for(int i=0;i<vertexCount;i++)
            {
                float nextAngle = i*angle;

                VertexUnlit vertex;
                vertex.position.x = cosf(glm::radians(nextAngle));
                vertex.position.y = sinf(glm::radians(nextAngle));
                vertex.position.z = 0.0f;

                vertex.texture.s = 0.5f+0.5f*cosf(glm::radians(nextAngle));
                vertex.texture.t = 0.5f+0.5f*sinf(glm::radians(nextAngle));

                cout << "---    " << i << "   ---" << endl;
                cout << "vertex.position.x: " << vertex.position.x << endl;
                cout << "vertex.position.y: " << vertex.position.y << endl;
                cout << "vertex.position.z: " << vertex.position.z << endl;
                cout << "vertex.texture.s: " << vertex.texture.s << endl;
                cout << "vertex.texture.t: " << vertex.texture.t << endl;

                vertices.push_back(vertex);
            }


            for(int i=0;i<vertexCount-2;i++)
            {
                indices.push_back(0);
                indices.push_back(i+1);
                indices.push_back(i+2);
            }


            VertexBuffer* vbo = new VertexBuffer();
            vbo->create(&vertices[0],vertices.size()*sizeof(VertexUnlit));

            IndexBuffer* ibo = new IndexBuffer();
            ibo->create(indices.data(),indices.size()*sizeof(unsigned int));


            vao = new VertexArrayObject();
            vao->create();
            vao->setVertexBuffer(vbo);
            vao->setIndexBuffer(ibo);
            vao->addAttributes(VertexAttributeType::Position);
            vao->addAttributes(VertexAttributeType::Texture);
            vao->activateAttributes();

            loader->m_vaos[ShapeType::Circle] = vao;
        }
        else
        {
            vao = loader->m_vaos[ShapeType::Circle];
        }
        return vao;
    }

    VertexArrayObject *MeshLoader::createCone()
    {
        VertexArrayObject* vao=nullptr;

        auto loader = getInstance();

        if(loader->m_vaos.count(ShapeType::Cone)==0)
        {
            vector<VertexUnlit> vertices;
            vector<unsigned int> indices;

            float angle =10;

            int vertexCount = 360.0f/angle;
            vertices.resize(vertexCount*2);

            for(int i=0;i<vertexCount;i++)
            {
                float nextAngle = i*angle;

                VertexUnlit vertex;
                vertex.position.x = cosf(glm::radians(nextAngle));
                vertex.position.y = 0.0f;
                vertex.position.z = sinf(glm::radians(nextAngle));

                vertex.texture.s = 0.5f+0.5f*cosf(glm::radians(nextAngle));
                vertex.texture.t = 0.5f+0.5f*sinf(glm::radians(nextAngle));
                vertices[i] = vertex;

                // vertex.texture.s = 0.5f+0.5f*cosf(glm::radians(nextAngle));
                // vertex.texture.t = 0.5f+0.5f*sinf(glm::radians(nextAngle));

                vertices[i+vertexCount] = vertex;
            }

            float r = sqrtf(2);
            float totalAngle = 360.0f;
            angle = totalAngle / (vertexCount-1);
            
            VertexUnlit nextVertex;
            nextVertex.position = vertices[vertexCount].position;
            vertices.push_back(nextVertex);
            
            for(int i=0;i<=vertexCount;i++)
            {
                float nextAngle = angle * i;

                vertices[i+vertexCount].texture.s = 0.5f+0.5f*cosf(glm::radians(nextAngle));
                vertices[i+vertexCount].texture.t = 0.5f+0.5f*sinf(glm::radians(nextAngle));
            }

            VertexUnlit topVertex;
            topVertex.position = glm::vec3(0.0f, 1.0f, 0.0f);
            topVertex.texture = glm::vec2(0.5f, 0.5f);
            vertices.push_back(topVertex);

            /*****************************  TABANIN INDEKSLERİ BULUNUYOR *******************************/
            for(int i=0;i<vertexCount-2;i++)
            {
                indices.push_back(0);
                indices.push_back(i+1);
                indices.push_back(i+2);
            }

            int topIndex = vertices.size() - 1;
            /*****************************  YAN YÜZEYİN INDEKSLERİ BULUNUYOR *****************************/ 
            for(int i=0;i<vertexCount-1;i++)
            {
                indices.push_back(topIndex);
                indices.push_back(vertexCount+i);
                indices.push_back(vertexCount+i+1);
            }

            indices.push_back(topIndex);
            indices.push_back(topIndex-1);
            indices.push_back(topIndex-2);

            VertexBuffer* vbo = new VertexBuffer();
            vbo->create(&vertices[0],vertices.size()*sizeof(VertexUnlit));

            IndexBuffer* ibo = new IndexBuffer();
            ibo->create(indices.data(),indices.size()*sizeof(unsigned int));


            vao = new VertexArrayObject();
            vao->create();
            vao->setVertexBuffer(vbo);
            vao->setIndexBuffer(ibo);
            vao->addAttributes(VertexAttributeType::Position);
            vao->addAttributes(VertexAttributeType::Texture);
            vao->activateAttributes();           

            loader->m_vaos[ShapeType::Cone] = vao;
        }
        else
        {
            vao = loader->m_vaos[ShapeType::Cone];
        }
        return vao;
    }

    VertexArrayObject *MeshLoader::createCylinder()
    {
        VertexArrayObject* vao=nullptr;

        auto loader = getInstance();

        if(loader->m_vaos.count(ShapeType::Cylinder)==0)
        {
            vector<VertexUnlit> vertices;
            vector<unsigned int> indices;

            float angle =90;

            int vertexCount = 360.0f/angle;
            vertices.resize(vertexCount*4);

            for(int i=0;i<vertexCount;i++)
            {
                float nextAngle = i*angle;

                VertexUnlit vertex;
                vertex.position.x = cosf(glm::radians(nextAngle));
                vertex.position.y = 2.0f;
                vertex.position.z = sinf(glm::radians(nextAngle));

                vertex.texture.s = 0.5f+0.5f*cosf(glm::radians(nextAngle));
                vertex.texture.t = 0.5f+0.5f*sinf(glm::radians(nextAngle));

                //Üst çember
                vertices[i] = vertex;

                //Üst çemberin dikdörtgendeki yansıması
                vertices[i+vertexCount] = vertex;

                //Alt çember değişecek
                vertices[i+vertexCount*2] = vertex;
                vertices[i+vertexCount*2].position.y = -2.0f;
                
                //Alt çemberin dikdörtgendeki yansıması
                vertices[i+vertexCount*3] = vertex;
                vertices[i+vertexCount*3].position.y = -2.0f;
            }
            
            for(int i=0;i<vertexCount;i++)
            {
                vertices[i+vertexCount].texture.t = 1.0f;
                vertices[i+vertexCount*3].texture.t = 0.0f;
            }

            /*****************************  ÜST ÇEMBERİN INDEKSLERİ BULUNUYOR *******************************/
            for(int i=0;i<vertexCount-2;i++)
            {
                indices.push_back(0);
                indices.push_back(i+1);
                indices.push_back(i+2);
            }

            /*****************************  ALT ÇEMBERİN INDEKSLERİ BULUNUYOR *******************************/
            for(int i=0;i<vertexCount-2;i++)
            {
                indices.push_back(vertexCount*2);
                indices.push_back(i+1+vertexCount*2);
                indices.push_back(i+2+vertexCount*2);
            }

            /*****************************  ÜST ÇEMBERİN YAN INDEKSLERİ BULUNUYOR *******************************/
            for(int i=vertexCount;i<2*vertexCount-1;i++)
            {
                indices.push_back(i);
                indices.push_back(vertexCount*2+i);
                indices.push_back(vertexCount*2+i+1);
            }

            indices.push_back(vertexCount*2-1);
            indices.push_back(vertexCount*3);
            indices.push_back(vertexCount*4-1);

            /*****************************  ALT ÇEMBERİN YAN INDEKSLERİ BULUNUYOR *******************************/
            for(int i=vertexCount;i<2*vertexCount-1;i++)
            {
                indices.push_back(i);
                indices.push_back(i+1);
                indices.push_back(vertexCount*2+i+1);
            }

             indices.push_back(vertexCount);
             indices.push_back(vertexCount*2-1);
             indices.push_back(vertexCount*3);
            
            /*****************************   *******************************/

            VertexBuffer* vbo = new VertexBuffer();
            vbo->create(&vertices[0],vertices.size()*sizeof(VertexUnlit));

            IndexBuffer* ibo = new IndexBuffer();
            ibo->create(indices.data(),indices.size()*sizeof(unsigned int));


            vao = new VertexArrayObject();
            vao->create();
            vao->setVertexBuffer(vbo);
            vao->setIndexBuffer(ibo);
            vao->addAttributes(VertexAttributeType::Position);
            vao->addAttributes(VertexAttributeType::Texture);
            vao->activateAttributes();           

            loader->m_vaos[ShapeType::Cylinder] = vao;
        }
        else
        {
            vao = loader->m_vaos[ShapeType::Cylinder];
        }
        return vao;
    }

    VertexArrayObject *MeshLoader::createCamera()
    {
        VertexArrayObject* vao=nullptr;

        auto loader = getInstance();

        if(loader->m_vaos.count(ShapeType::Camera)==0)
        {
            vector<VertexUnlit> vertices;
            vector<unsigned int> indices;

            VertexUnlit vertex;
            vertex.position.x = 0.0f;
            vertex.position.y = 0.0f;
            vertex.position.z = 0.0f;

            vertices.push_back(vertex);

            VertexBuffer* vbo = new VertexBuffer();
            vbo->create(&vertices[0],vertices.size()*sizeof(VertexUnlit));

            IndexBuffer* ibo = new IndexBuffer();
            ibo->create(indices.data(),indices.size()*sizeof(unsigned int));

            vao = new VertexArrayObject();
            vao->create();
            vao->setVertexBuffer(vbo);
            vao->setIndexBuffer(ibo);
            vao->addAttributes(VertexAttributeType::Position);
            vao->addAttributes(VertexAttributeType::Texture);
            vao->activateAttributes();

            loader->m_vaos[ShapeType::Camera] = vao;
        }
        else
        {
            vao = loader->m_vaos[ShapeType::Camera];
        }
        return vao;
    }

    VertexArrayObject *MeshLoader::createScene()
    {        
        VertexArrayObject* vao=nullptr;

        auto loader = getInstance();

        if(loader->m_vaos.count(ShapeType::Scene)==0)
        {
            vector<VertexUnlit> vertices;
            vector<unsigned int> indices;

            VertexUnlit vertex;
            vertex.position.x = 0.0f;
            vertex.position.y = 0.0f;
            vertex.position.z = 0.0f;

            vertices.push_back(vertex);

            VertexBuffer* vbo = new VertexBuffer();
            vbo->create(&vertices[0],vertices.size()*sizeof(VertexUnlit));

            IndexBuffer* ibo = new IndexBuffer();
            ibo->create(indices.data(),indices.size()*sizeof(unsigned int));

            vao = new VertexArrayObject();
            vao->create();
            vao->setVertexBuffer(vbo);
            vao->setIndexBuffer(ibo);
            vao->addAttributes(VertexAttributeType::Position);
            vao->addAttributes(VertexAttributeType::Texture);
            vao->activateAttributes();

            loader->m_vaos[ShapeType::Scene] = vao;
        }
        else
        {
            vao = loader->m_vaos[ShapeType::Scene];
        }
        return vao;
    }

   MeshLoader *MeshLoader::getInstance()
    {
        if(!m_instance)
            m_instance = new MeshLoader;
        return m_instance;
    }
}