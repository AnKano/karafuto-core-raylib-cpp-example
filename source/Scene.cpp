#include "Scene.hpp"

#include "rlgl.h"

static Mesh CreateMeshFromGridMeshData(KCore::GridMesh *data) {
    Mesh mesh = {
            .vertexCount = (int) data->getVertices().size(),
            .triangleCount = (int) data->getIndices().size() / 3,
            .vertices = (float *) new float[mesh.vertexCount * 3],
            .texcoords = (float *) new float[mesh.vertexCount * 2],
            .normals = (float *) new float[mesh.vertexCount * 3],
            .indices = (unsigned short *) new unsigned int[mesh.triangleCount * 3],
    };

    auto vtxs = data->getVertices();
    auto uvs = data->getUVs();
    auto nrm = data->getNormals();
    auto ind = data->getIndices();

    for (int i = 0; i < mesh.vertexCount; i++) {
        mesh.vertices[3 * i] = vtxs[i].x;
        mesh.vertices[3 * i + 1] = vtxs[i].y;
        mesh.vertices[3 * i + 2] = vtxs[i].z;
    }

    for (int i = 0; i < mesh.vertexCount; i++) {
        mesh.texcoords[2 * i] = uvs[i].x;
        mesh.texcoords[2 * i + 1] = 1.0f - uvs[i].y;
    }

    for (int i = 0; i < mesh.vertexCount; i++) {
        mesh.normals[3 * i] = nrm[i].x;
        mesh.normals[3 * i + 1] = nrm[i].y;
        mesh.normals[3 * i + 2] = nrm[i].z;
    }

    for (int i = 0; i < mesh.triangleCount * 3; i++)
        mesh.indices[i] = ind[i];

    UploadMesh(&mesh, false);

    return mesh;
}

Scene::Scene() : mCamera(glm::vec3{1.0f, 5.0f, 10.0f},
                         glm::vec3{0.0f, 0.0f, 0.0f},
                         glm::vec3{0.0f, 1.0f, 0.0f},
                         60.0f) {
    mLayer = KCore::CreateTileLayer(46.9181f, 142.7189f);

    mElevation = KCore::CreateSRTMElevationSource();
    // enumerate filenames
    {
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N45E141.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N45E142.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N46E141.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N46E142.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N46E143.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N47E141.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N47E142.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N47E143.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N48E141.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N48E142.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N48E144.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N49E142.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N49E143.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N49E144.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N50E142.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N50E143.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N51E141.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N51E142.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N51E143.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N52E141.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N52E142.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N52E143.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N53E141.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N53E142.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N53E143.hgt", KCore::SourceType::SourceFile);
        KCore::AddSRTMPiece(mElevation, "../assets/elevation/N54E142.hgt", KCore::SourceType::SourceFile);
    }
}

void Scene::update() {
    mCamera.update();

    auto projectionMatrix = mCamera.getProjectionMatrix(1920.0f / 1080.0f);
    KCore::UpdateProjectionMatrix(mLayer, glm::value_ptr(projectionMatrix));

    auto viewMatrix = mCamera.getViewMatrix();
    KCore::UpdateViewMatrix(mLayer, glm::value_ptr(viewMatrix));

    KCore::Calculate(mLayer);

    auto *coreEvents = KCore::GetCoreEventsVector(mLayer);
    int length = 0;
    auto *events = KCore::EjectEventsFromVector(coreEvents, length);

    for (std::size_t idx = 0; idx < length; idx++) {
        auto &item = events[idx];

        if (item.type == KCore::InFrustum) {
            auto *data = static_cast<KCore::TilePayloadEvent *>(item.payload);
            if (data->visibility != KCore::Visible) continue;

            mTiles[item.quadcode] = Tile(*data);

            auto *mesh = KCore::CreateTileMeshQuadcode(mElevation, item.quadcode, 16, 16, false, false);
            mTiles[item.quadcode].setModel(LoadModelFromMesh(CreateMeshFromGridMeshData(mesh)));
        }
        if (item.type == KCore::NotInFrustum) {
            if (mTiles.contains(item.quadcode)) {
                mTiles.erase(item.quadcode);
            }
        }
    }

    auto *imageEvents = KCore::GetImageEventsVector(mLayer);
    length = 0;
    events = KCore::EjectEventsFromVector(imageEvents, length);

    for (std::size_t idx = 0; idx < length; idx++) {
        auto &item = events[idx];
        if (item.type == KCore::ImageReady) {
            auto *data = (KCore::ImagePayloadEvent *) item.payload;

            if (!mTiles.contains(item.quadcode))
                continue;

            mTiles[item.quadcode].setTexture(*data);
        }
    }
}

void Scene::draw() const {
    BeginMode3D(mCamera.getRawCamera());

    auto events = mLayer->getCoreEvents();

    for (const auto &[key, item]: mTiles) {
        if (item.getModel().has_value()) {
            rlDisableBackfaceCulling();
            item.draw();
        }
    }

    EndMode3D();
}
