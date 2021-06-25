#include "rendersystem.h"

#include "resourcemanager.h"
#include "shader.h"
#include "gslpch.h"
#include "ecs/entity.h"
#include "scene.h"

RenderSystem::RenderSystem()
{
}

void RenderSystem::init()
{
    initializeOpenGLFunctions();
}

void RenderSystem::Render()
{
    ResourceManager &resourceMgr = ResourceManager::getInstance();
    for(unsigned int i{0}; i < mNumberOfMeshes; ++i)
    {
        Entity tempEntity = ResourceManager::getInstance().mCurrentScene->mEntities[i];
        gsl::mat4 tempTransform;

        tempTransform.setToIdentity();
        tempTransform.setPosition(tempEntity.mTransform.position);
        tempTransform.rotateX(tempEntity.mTransform.rotation.x);
        tempTransform.rotateY(tempEntity.mTransform.rotation.y);
        tempTransform.rotateZ(tempEntity.mTransform.rotation.z);
        tempTransform.scale(tempEntity.mTransform.scale);
        glBindVertexArray(tempEntity.mMesh.mVAO);

        //Set up material for this mesh:
        ecs::Material *tempMaterial = resourceMgr.mMaterial[mMaterialIDs[i].mMaterialIndex];
        resourceMgr.mShaderProgram[tempMaterial->mShader]->use(tempTransform, tempMaterial);

        if (!tempEntity.bHidden)
        {
            if(tempEntity.mMesh.mIndexCount > 0)
                glDrawElements(GL_TRIANGLES, tempEntity.mMesh.mIndexCount, GL_UNSIGNED_INT, nullptr);
            else
                glDrawArrays(GL_TRIANGLES, 0, tempEntity.mMesh.mVertexCount);
            glBindVertexArray(0);
        }
    }
}

void RenderSystem::renderNormals(GLint matrixUniform)
{
    for (unsigned int i{0}; i < mNumberOfMeshes; i++)
    {
        //TODO: temporary solution for transform!
        gsl::mat4 tempTransform;
        tempTransform.setToIdentity();
        tempTransform.setPosition(ResourceManager::getInstance().mCurrentScene->mEntities[i].mTransform.position);
        tempTransform.rotateX(ResourceManager::getInstance().mCurrentScene->mEntities[i].mTransform.rotation.x);
        tempTransform.rotateY(ResourceManager::getInstance().mCurrentScene->mEntities[i].mTransform.rotation.y);
        tempTransform.rotateZ(ResourceManager::getInstance().mCurrentScene->mEntities[i].mTransform.rotation.z);
        tempTransform.scale(ResourceManager::getInstance().mCurrentScene->mEntities[i].mTransform.scale);
        glBindVertexArray( ResourceManager::getInstance().mCurrentScene->mEntities[i].mMesh.mVAO );

        glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, tempTransform.constData());   //move to shader???
        if(ResourceManager::getInstance().mCurrentScene->mEntities[i].mMesh.mIndexCount > 0)
            glDrawElements(GL_TRIANGLES, ResourceManager::getInstance().mCurrentScene->mEntities[i].mMesh.mIndexCount, GL_UNSIGNED_INT, nullptr);
        else
            glDrawArrays(GL_TRIANGLES, 0, ResourceManager::getInstance().mCurrentScene->mEntities[i].mMesh.mVertexCount);

        glBindVertexArray(0);
    }
}

void RenderSystem::renderMousepicker(GLint matrixUniform, GLint colorUniform)
{
    for (unsigned int i{0}; i < mNumberOfMeshes; i++)
    {
        int j = i*20;   //just to make the color more visible for debug - remove when working!
        int r = (j & 0x000000FF) >>  0;
        int g = (j & 0x0000FF00) >>  8;
        int b = (j & 0x00FF0000) >> 16;

        //TODO: temporary solution for transform!
        gsl::mat4 tempTransform;
        tempTransform.setToIdentity();
        tempTransform.setPosition(ResourceManager::getInstance().mCurrentScene->mEntities[i].mTransform.position);
        tempTransform.rotateX(ResourceManager::getInstance().mCurrentScene->mEntities[i].mTransform.rotation.x);
        tempTransform.rotateY(ResourceManager::getInstance().mCurrentScene->mEntities[i].mTransform.rotation.y);
        tempTransform.rotateZ(ResourceManager::getInstance().mCurrentScene->mEntities[i].mTransform.rotation.z);
        tempTransform.scale(ResourceManager::getInstance().mCurrentScene->mEntities[i].mTransform.scale);
        glBindVertexArray( ResourceManager::getInstance().mCurrentScene->mEntities[i].mMesh.mVAO );

        glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, tempTransform.constData());   //move to shader???
        glUniform3f(colorUniform, r/255.f, g/255.f, b/255.f);


        if(ResourceManager::getInstance().mCurrentScene->mEntities[i].mMesh.mIndexCount > 0)
            glDrawElements(GL_TRIANGLES, ResourceManager::getInstance().mCurrentScene->mEntities[i].mMesh.mIndexCount, GL_UNSIGNED_INT, nullptr);
        else
            glDrawArrays(GL_TRIANGLES, 0, ResourceManager::getInstance().mCurrentScene->mEntities[i].mMesh.mVertexCount);
        glBindVertexArray(0);

    }
}
