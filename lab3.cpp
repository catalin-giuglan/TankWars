#include "lab_m1/tema1/lab3.h"
#include <vector>
#include <iostream>
#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"

using namespace std;
using namespace m1;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, -100, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // Variabile folosite pentru functia de calculare si desenare a terenului
    float Amplitude1 = 80;
    float Amplitude2 = 40;
    float Frequency1 = 0.012;
    float Frequency2 = 0.025;

    for (float i = 0; i < resolution.x; i++) {
        float y = Amplitude1 * sin(Frequency1 * i) + Amplitude2 * sin(Frequency2 * i) + 180;
        Terrain.push_back(y);
    }
}

void Tema1::FrameStart()
{
    glClearColor(0.3f, 0.6f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();
    float windowWidth = (float)resolution.x;

    // Vector de varfuri pentru teren
    std::vector<VertexFormat> vertices;

    // Extindem terenul pe toata fereastra
    for (float x = 0; x < resolution.x; x++) {
        vertices.push_back(VertexFormat(glm::vec3(x, Terrain[(int)(x * Terrain.size() / windowWidth)], 0), glm::vec3(0, 1, 0)));
        vertices.push_back(VertexFormat(glm::vec3(x, -100, 0), glm::vec3(0, 1, 0)));
    }

    // Mesh pentru teren
    Mesh* terrainMesh = new Mesh("terrain");
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < 2 * resolution.x; i++) {
        indices.push_back(i);
    }
    // Am folosit metoda cu TRIANGLE_STRIP pentru a desena terenul
    terrainMesh->SetDrawMode(GL_TRIANGLE_STRIP);
    terrainMesh->InitFromData(vertices, indices);

    RenderMesh(terrainMesh, shaders["VertexColor"], glm::mat4(1));

    // Randare tank jucator
    if (playerTankHealth > 0) {
        float terrainHeightPlayer = Terrain[(int)(tankX * Terrain.size() / windowWidth)];
        RenderTank(tankX, playerTankHealth, terrainHeightPlayer, tankAngle, barrelAngle,
            glm::vec3(0.6f, 0.2f, 0.2f), glm::vec3(0.7f, 0.3f, 0.3f), glm::vec2(5, 37),
            50, "playerBase", "playerTurret", "playerBarrel", "playerWheels", 5.0f, glm::vec3(1, 1, 1));
    }

    // Randare tank oponent
    if (opponentTankHealth > 0) {
        float terrainHeightOpponent = Terrain[(int)(opponentX * Terrain.size() / windowWidth)];
        RenderTank(opponentX, opponentTankHealth, terrainHeightOpponent, opponentTankAngle, opponentBarrelAngle,
            glm::vec3(0.2f, 0.5f, 0.4f), glm::vec3(0.3f, 0.6f, 0.5f), glm::vec2(5, 37),
            50, "opponentBase", "opponentTurret", "opponentBarrel", "opponentWheels", 5.0f, glm::vec3(1, 1, 1));
    }

    // Daca tankul se afla pe o panta, viteza lui e afectata de unghiul pantei

    if (window->KeyHold(GLFW_KEY_A)) {
        tankX -= 100 * deltaTimeSeconds * cos(tankAngle);
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        tankX += 100 * deltaTimeSeconds * cos(tankAngle);
    }

    if (window->KeyHold(GLFW_KEY_LEFT)) {
        opponentX -= 100 * deltaTimeSeconds * cos(opponentTankAngle);
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        opponentX += 100 * deltaTimeSeconds * cos(opponentTankAngle);
    }

    // Am pus conditia ca barrel-ul sa se poata roti doar in jurul turetei

    if (window->KeyHold(GLFW_KEY_W)) {
        barrelAngle += 1.0f * deltaTimeSeconds;
        if (barrelAngle > glm::radians(180.0f)) {
            barrelAngle = glm::radians(180.0f);
        }
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        barrelAngle -= 1.0f * deltaTimeSeconds;
        if (barrelAngle < glm::radians(0.0f)) {
            barrelAngle = glm::radians(0.0f);
        }
    }

    if (window->KeyHold(GLFW_KEY_UP)) {
        opponentBarrelAngle -= 1.0f * deltaTimeSeconds;
        if (opponentBarrelAngle < glm::radians(0.0f)) {
            opponentBarrelAngle = glm::radians(0.0f);
        }
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        opponentBarrelAngle += 1.0f * deltaTimeSeconds;
        if (opponentBarrelAngle > glm::radians(180.0f)) {
            opponentBarrelAngle = glm::radians(180.0f);
        }
    }

    // Actualizez pozitia fiecarui proiectil
    for (int i = 0; i < playerProjectileX.size(); i++) {
        // Actualizez viteza pe axa Y cu gravitatia
        playerProjectileVelocityY[i] += gravity * deltaTimeSeconds;

        // Actualizarea pozitiilor proiectilelor in functie de viteza
        playerProjectileX[i] += playerProjectileVelocityX[i] * deltaTimeSeconds;
        playerProjectileY[i] += playerProjectileVelocityY[i] * deltaTimeSeconds;

        // Randarea proiectilului la noua pozitie
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(playerProjectileX[i], playerProjectileY[i], 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0));
        RenderMesh(object2D::CreateCircle("projectile", glm::vec3(0, 0, 0), 5, glm::vec3(0, 0, 0)), shaders["VertexColor"], modelMatrix);

        // Verific coliziunea cu tancul oponent (daca cercul proiectilului se intersecteaza cu cercul tancului)
        float opponentY = Terrain[(int)(opponentX * Terrain.size() / window->GetResolution().x)];
        float dx = playerProjectileX[i] - opponentX;
        float dy = playerProjectileY[i] - opponentY;
        float distanceSquared = dx * dx + dy * dy;
        float collisionRadiusSquared = (playerProjectileRadius + opponentTankRadius) * (playerProjectileRadius + opponentTankRadius);

        if (distanceSquared < collisionRadiusSquared) {
            opponentTankHealth -= 10.0f; // Reduc viata adversarului cu 10
            // Sterg proiectilul din vectori
            playerProjectileX.erase(playerProjectileX.begin() + i);
            playerProjectileY.erase(playerProjectileY.begin() + i);
            playerProjectileVelocityX.erase(playerProjectileVelocityX.begin() + i);
            playerProjectileVelocityY.erase(playerProjectileVelocityY.begin() + i);
            i--;
            continue;
        }

        // Verific daca proiectilul a cazut sub nivelul solului
        float terrainHeight = Terrain[(int)(playerProjectileX[i] * Terrain.size() / window->GetResolution().x)];
        if (playerProjectileY[i] < terrainHeight) {
            // In locul impactului se face o "groapa", semn ca a facut intersectie cu solul
            int craterRadius = 40;
            int craterCenter = (int)(playerProjectileX[i] * Terrain.size() / window->GetResolution().x);
            for (int j = -craterRadius; j <= craterRadius; j++) {
                int index = craterCenter + j;
                if (index >= 0 && index < Terrain.size()) {
                    Terrain[index] -= craterRadius - abs(j);
                }
            }
            // Sterg proiectilul din toti vectorii
            playerProjectileX.erase(playerProjectileX.begin() + i);
            playerProjectileY.erase(playerProjectileY.begin() + i);
            playerProjectileVelocityX.erase(playerProjectileVelocityX.begin() + i);
            playerProjectileVelocityY.erase(playerProjectileVelocityY.begin() + i);
            i--;
        }
    }

    // Actualizez pozitia fiecarui proiectil pentru tancul oponent
    for (int i = 0; i < opponentProjectileX.size(); i++) {
        // Actualizam viteza pe axa Y cu gravitația
        opponentProjectileVelocityY[i] += gravity * deltaTimeSeconds;

        // Actualizarea pozitiilor proiectilelor in functie de viteza
        opponentProjectileX[i] += opponentProjectileVelocityX[i] * deltaTimeSeconds;
        opponentProjectileY[i] += opponentProjectileVelocityY[i] * deltaTimeSeconds;

        // Randarea proiectilului la noua pozitie
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(opponentProjectileX[i], opponentProjectileY[i], 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0));
        RenderMesh(object2D::CreateCircle("opponent_projectile", glm::vec3(0, 0, 0), 5, glm::vec3(0, 0, 0)), shaders["VertexColor"], modelMatrix);

        // Verific coliziunea cu tancul oponent (daca cercul proiectilului se intersecteaza cu cercul tancului)
        float tankY = Terrain[(int)(tankX * Terrain.size() / window->GetResolution().x)];
        float dx = opponentProjectileX[i] - tankX;
        float dy = opponentProjectileY[i] - tankY;
        float distanceSquared = dx * dx + dy * dy;
        float collisionRadiusSquared = (opponentProjectileRadius + playerTankRadius) * (opponentProjectileRadius + playerTankRadius);

        if (distanceSquared < collisionRadiusSquared) {
            playerTankHealth -= 10.0f; // Reduc viata adversarului cu 10
            // Sterg proiectilul din vectori
            opponentProjectileX.erase(opponentProjectileX.begin() + i);
            opponentProjectileY.erase(opponentProjectileY.begin() + i);
            opponentProjectileVelocityX.erase(opponentProjectileVelocityX.begin() + i);
            opponentProjectileVelocityY.erase(opponentProjectileVelocityY.begin() + i);
            i--;
            continue;
        }

        // Verific daca proiectilul a cazut sub nivelul solului
        float terrainHeight = Terrain[(int)(opponentProjectileX[i] * Terrain.size() / window->GetResolution().x)];
        if (opponentProjectileY[i] < terrainHeight) {
            // Se creaza un crater la locul impactului
            int craterRadius = 40;
            int craterCenter = (int)(opponentProjectileX[i] * Terrain.size() / window->GetResolution().x);
            for (int j = -craterRadius; j <= craterRadius; j++) {
                int index = craterCenter + j;
                if (index >= 0 && index < Terrain.size()) {
                    Terrain[index] -= craterRadius - abs(j);
                }
            }
            // Sterg proiectilul din toti vectorii
            opponentProjectileX.erase(opponentProjectileX.begin() + i);
            opponentProjectileY.erase(opponentProjectileY.begin() + i);
            opponentProjectileVelocityX.erase(opponentProjectileVelocityX.begin() + i);
            opponentProjectileVelocityY.erase(opponentProjectileVelocityY.begin() + i);
            i--;
        }
    }


    // Bara de viata a jucatorului
    float terrainHeightPlayer = Terrain[(int)(tankX * Terrain.size() / windowWidth)];
    RenderHealthBar(tankX, terrainHeightPlayer, playerTankHealth, 100.0f,
        100.0f, 5.0f, 70.0f, glm::vec3(0, 1, 0), "playerHealthBar");

    // Bara de viata a oponentului
    float terrainHeightOpponent = Terrain[(int)(opponentX * Terrain.size() / windowWidth)];
    RenderHealthBar(opponentX, terrainHeightOpponent, opponentTankHealth, 100.0f,
        100.0f, 5.0f, 70.0f, glm::vec3(0, 1, 0), "opponentHealthBar");

}

// Functia in care creez si randez tankurile
void Tema1::RenderTank(float tankX, float tankHealth, float terrainHeight, float& tankAngle,
    float barrelAngle, const glm::vec3& baseColor, const glm::vec3& turretColor,
    const glm::vec2& turretOffset, float barrelLength, const std::string& baseMeshName,
    const std::string& turretMeshName, const std::string& barrelMeshName,
    const std::string& wheelsMeshName, float offsetX, const glm::vec3& trajectoryColor)
{
    // Calcularea pantei terenului la pozitia tankului
    float x1 = tankX;
    float y1 = terrainHeight;
    float x2 = tankX + offsetX;
    float y2 = Terrain[(int)(x2 * Terrain.size() / window->GetResolution().x)];
    tankAngle = atan2(y2 - y1, x2 - x1);

    // Aplic rotatia pentru baza tankului
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(tankX, terrainHeight, 0));
    modelMatrix = glm::rotate(modelMatrix, tankAngle, glm::vec3(0, 0, 1));

    // Creez baza tankului
    Mesh* base = object2D::CreateTrapezoid(baseMeshName, glm::vec3(-50, 5, 0), 100, 25, baseColor, true);
    RenderMesh(base, shaders["VertexColor"], modelMatrix);

    // Creez semicercul pe care se misca turela
    Mesh* semicircle = object2D::CreateSemicircle(turretMeshName, glm::vec3(0, 30, 0), 20, turretColor, true);
    RenderMesh(semicircle, shaders["VertexColor"], modelMatrix);

    // Creez turela tankului
    glm::mat4 barrelMatrix = modelMatrix;
    barrelMatrix = glm::translate(barrelMatrix, glm::vec3(turretOffset.x, turretOffset.y, 0));
    barrelMatrix = glm::rotate(barrelMatrix, barrelAngle, glm::vec3(0, 0, 1));
    barrelMatrix = glm::translate(barrelMatrix, glm::vec3(-turretOffset.x, -turretOffset.y, 0));
    Mesh* barrel = object2D::CreateRectangle(barrelMeshName, glm::vec3(5, 35, 0), barrelLength, 5, glm::vec3(0, 0, 0), true);
    RenderMesh(barrel, shaders["VertexColor"], barrelMatrix);

    // Creez "senilele" tankului
    glm::vec3 wheelsPosition(40, 0, 0);
    Mesh* wheels = object2D::CreateTrapezoid(wheelsMeshName, wheelsPosition, -80, 5, glm::vec3(0, 0, 0), true);
    RenderMesh(wheels, shaders["VertexColor"], modelMatrix);

    // Functie care calculeaza si randeaza traiectoria pe care o vor urma proiectilele
    CalculateAndRenderTrajectory(tankX, tankAngle, barrelAngle, turretOffset, trajectoryColor, barrelLength);
}

// Functie care lanseaza proiectilul
void Tema1::LaunchProjectile(
    float tankX, float tankAngle, float barrelAngle, const glm::vec2& turretOffset,
    const std::vector<float>& Terrain, float windowWidth,
    std::vector<float>& projectileX, std::vector<float>& projectileY,
    std::vector<float>& velocityX, std::vector<float>& velocityY,
    float initialVelocity, float barrelLength)
{
    // Pozitia curenta a tankului pe axa Y
    float tankHeight = Terrain[(int)(tankX * Terrain.size() / windowWidth)];

    // Atunci cand lansez proiectilul tin cont atat de unghiul turelei, cat si de unghiul la care se afla tankul fata de sol
    float totalAngle = barrelAngle + tankAngle;

    float turretOffsetRotatedX = turretOffset.x * cos(tankAngle) - turretOffset.y * sin(tankAngle);
    float turretOffsetRotatedY = turretOffset.x * sin(tankAngle) + turretOffset.y * cos(tankAngle);

    // Ii dau proiectilului pozitia de start la capatul turelei
    float startX = tankX + turretOffsetRotatedX + barrelLength * cos(totalAngle);
    float startY = tankHeight + turretOffsetRotatedY + barrelLength * sin(totalAngle);

    // Vitezele initiale pe axele X si Y
    float velX = initialVelocity * cos(totalAngle);
    float velY = initialVelocity * sin(totalAngle);

    // Adaug proiectilul in vectori
    projectileX.push_back(startX);
    projectileY.push_back(startY);
    velocityX.push_back(velX);
    velocityY.push_back(velY);
}

// Functie care randeaza bara de viata a unui tank
void Tema1::RenderHealthBar(float tankX, float terrainHeight, float health, float maxHealth,
    float healthBarWidth, float healthBarHeight, float healthBarOffsetY,
    const glm::vec3& color, const std::string& meshName)
{
    // Calculez pozitia pentru bara de viata, deasupra tankului
    float healthBarX = tankX - healthBarWidth / 2;
    float healthBarY = terrainHeight + healthBarOffsetY;

    glm::mat4 healthBarMatrix = glm::mat4(1);
    healthBarMatrix = glm::translate(healthBarMatrix, glm::vec3(healthBarX, healthBarY, 0));

    // Calculez procentul de viata ramas
    float healthPercentage = glm::clamp(health / maxHealth, 0.0f, 1.0f);

    // Matrice care scaleaza bara de viata in functie de procentul de viata ramas
    glm::mat4 healthBarForegroundMatrix = healthBarMatrix;
    healthBarForegroundMatrix = glm::scale(healthBarForegroundMatrix, glm::vec3(healthPercentage, 1.0f, 1.0f));

    Mesh* healthBar = object2D::CreateRectangle(meshName, glm::vec3(0, 1, 0), healthBarWidth, healthBarHeight, color, true);
    RenderMesh(healthBar, shaders["VertexColor"], healthBarForegroundMatrix);
}


void Tema1::FrameEnd()
{
}

// Functie care calculeaza traiectoria proiectilului si o randeaza
// Randeaza traiectoria in functie de vectorul rezultat de CalculateProjectileTrajectory
void Tema1::CalculateAndRenderTrajectory(float tankX, float tankAngle, float barrelAngle, const glm::vec2& turretOffset,
    const glm::vec3& color, float barrelLength) {
    glm::ivec2 resolution = window->GetResolution();
    float windowWidth = (float)resolution.x;

    float tankHeight = Terrain[(int)(tankX * Terrain.size() / windowWidth)];
    float totalAngle = barrelAngle + tankAngle;

    float turretOffsetRotatedX = turretOffset.x * cos(tankAngle) - turretOffset.y * sin(tankAngle);
    float turretOffsetRotatedY = turretOffset.x * sin(tankAngle) + turretOffset.y * cos(tankAngle);

    // Calculez punctul de pornire al traiectoriei
    float startX = tankX + turretOffsetRotatedX + barrelLength * cos(totalAngle);
    float startY = tankHeight + turretOffsetRotatedY + barrelLength * sin(totalAngle);

    // Calculez vitezele inițiale
    float initialVelocity = 300.0f;
    float velocityX = initialVelocity * cos(totalAngle);
    float velocityY = initialVelocity * sin(totalAngle);

    std::vector<glm::vec2> trajectory = CalculateProjectileTrajectory(startX, startY, velocityX, velocityY, gravity, 100, 0.1f);
    RenderTrajectory(trajectory, color);
}

// Functie care calculeaza traiectoria proiectilului
// O returneaza sub forma de vector de puncte
std::vector<glm::vec2> Tema1::CalculateProjectileTrajectory(float startX, float startY, float velocityX, float velocityY, float gravity, int numPoints, float timeStep) {
    std::vector<glm::vec2> trajectory;

    float t = 0.0f;
    for (int i = 0; i < numPoints; i++) {
        float x = startX + velocityX * t;
        float y = startY + velocityY * t + 0.5f * gravity * t * t;

        trajectory.push_back(glm::vec2(x, y));

        t += timeStep;
    }

    return trajectory;
}

// Functie care randeaza traiectoria proiectilului
// Primeste ca parametrii un vector de puncte si culoarea cu care sa fie desenata
// Deseneaza linii intre punctele consecutive din vector
void Tema1::RenderTrajectory(const std::vector<glm::vec2>& trajectory, const glm::vec3& color) {
    for (size_t i = 1; i < trajectory.size(); i++) {
        RenderMesh(object2D::CreateLine("trajectory", trajectory[i - 1], trajectory[i], color), shaders["VertexColor"], glm::mat4(1));
    }
}

void Tema1::OnKeyPress(int key, int mods)
{
    // Apelez functia de lansare a proiectilului cand apas pe SPACE sau ENTER
    if (key == GLFW_KEY_SPACE && playerTankHealth > 0) {
        LaunchProjectile(
            tankX, tankAngle, barrelAngle, glm::vec2(5, 35), Terrain,
            window->GetResolution().x,
            playerProjectileX, playerProjectileY,
            playerProjectileVelocityX, playerProjectileVelocityY,
            300.0f, 50.0f);
    }

    if (key == GLFW_KEY_ENTER && opponentTankHealth > 0) {
        LaunchProjectile(
            opponentX, opponentTankAngle, opponentBarrelAngle, glm::vec2(5, 33), Terrain,
            window->GetResolution().x,
            opponentProjectileX, opponentProjectileY,
            opponentProjectileVelocityX, opponentProjectileVelocityY,
            300.0f, 50.0f);
    }

    // Ca bonus am adaugat functionalitatea de a reseta viata jucatorului si a oponentului
    if (key == GLFW_KEY_R) {
        playerTankHealth = 100;
    }
    if (key == GLFW_KEY_RIGHT_CONTROL) {
        opponentTankHealth = 100;
    }
}
