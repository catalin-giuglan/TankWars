#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void OnKeyPress(int key, int mods) override;

        void RenderTrajectory(const std::vector<glm::vec2>& trajectory, const glm::vec3& color);
        std::vector<glm::vec2> CalculateProjectileTrajectory(float startX, float startY, float velocityX, float velocityY, float gravity, int numPoints, float timeStep);

        void CalculateAndRenderTrajectory(float tankX, float tankAngle, float barrelAngle, const glm::vec2& turretOffset, const glm::vec3& color, float barrelLength);

        void LaunchProjectile(
            float tankX, float tankAngle, float barrelAngle, const glm::vec2& turretOffset,
            const std::vector<float>& Terrain, float windowWidth,
            std::vector<float>& projectileX, std::vector<float>& projectileY,
            std::vector<float>& velocityX, std::vector<float>& velocityY,
            float initialVelocity, float barrelLength);

        void RenderHealthBar(float tankX, float terrainHeight, float health, float maxHealth,
            float healthBarWidth, float healthBarHeight, float healthBarOffsetY,
            const glm::vec3& color, const std::string& meshName);

        void RenderTank(float tankX, float tankHealth, float terrainHeight, float& tankAngle,
            float barrelAngle, const glm::vec3& baseColor, const glm::vec3& turretColor,
            const glm::vec2& turretOffset, float barrelLength, const std::string& baseMeshName,
            const std::string& turretMeshName, const std::string& barrelMeshName,
            const std::string& wheelsMeshName, float offsetX, const glm::vec3& trajectoryColor);

        /*void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;*/

    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;

        float rotation = 0;
        bool direction;
        bool ok;

        std::vector<float> Terrain;

        float tankX = 250;
        float barrelAngle = glm::radians(0.0f);
        const float barrelLength = 50.0f;
        float tankAngle = 0;

        float opponentX = 1200;
        float opponentBarrelAngle = glm::radians(180.0f);
        float opponentTankAngle = 0;

        std::vector<float> playerProjectileX, playerProjectileY;
        std::vector<float> playerProjectileVelocityX, playerProjectileVelocityY;

        float gravity = -300.0f;

        std::vector<float> opponentProjectileX, opponentProjectileY;
        std::vector<float> opponentProjectileVelocityX, opponentProjectileVelocityY;

        float playerTankHealth = 100.0f;
        float opponentTankHealth = 100.0f;

        float playerProjectileRadius = 5.0f;
        float opponentTankRadius = 40.0f;
        float playerTankRadius = 40.0f;
        float opponentProjectileRadius = 5.0f;
    };
}   // namespace m1
