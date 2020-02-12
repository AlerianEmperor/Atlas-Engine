#include <actor/MeshActor.h>
#include "RenderList.h"

#include "libraries/glm/gtx/norm.hpp"

namespace Atlas {

	RenderList::RenderList(int32_t type) : type(type) {


	}

	void RenderList::Add(Actor::MeshActor *actor) {

		auto actorBatchKey = actorBatches.find(actor->mesh);

		if (actorBatchKey != actorBatches.end()) {
			actorBatchKey->second->Add(actor);
			return;
		}

		// Create new actor batch
		auto actorBatch = new Actor::ActorBatch<Mesh::Mesh*, Actor::MeshActor*>(actor->mesh);		

		actorBatch->Add(actor);

		actorBatches[actor->mesh] = actorBatch;

		// Build up all render list batches
		std::map<int32_t, RenderListBatch> renderListBatches;

		for (auto &subData : actor->mesh->data.subData) {

			auto shaderConfig = actor->mesh->GetConfig(subData.material, type);

			auto batchKey = renderListBatches.find(shaderConfig->shaderID);

			if (batchKey != renderListBatches.end()) {
				batchKey->second.subData.push_back(&subData);
				continue;
			}

			RenderListBatch batch;
			batch.actorBatch = actorBatch;
			batch.subData.push_back(&subData);
			renderListBatches[shaderConfig->shaderID] = batch;

		}

		// Integrate the render list batches into the ordered render batches
		for (auto &renderListBatchKey : renderListBatches) {
			orderedRenderBatches[renderListBatchKey.first].push_back(renderListBatchKey.second);
		}

	}

	void RenderList::UpdateBuffers(Camera* camera) {		

		auto cameraLocation = camera->GetLocation();

		for (auto& actorBatchKey : actorBatches) {
			auto mesh = actorBatchKey.first;
			auto actorBatch = actorBatchKey.second;
			auto hasImpostor = mesh->impostor != nullptr;

			if (!actorBatch->GetSize())
				continue;

			if (!mesh->castShadow && type == AE_SHADOW_CONFIG)
				continue;

			std::vector<mat4> actorMatrices;
			std::vector<mat4> impostorMatrices;

			auto sqdDistance = powf(mesh->impostorDistance, 2.0f);

			for (auto actor : actorBatch->actors) {
				auto distance = glm::distance2(
					vec3(actor->globalMatrix[3]),
					cameraLocation);

				if (distance < sqdDistance || !hasImpostor) {
					actorMatrices.push_back(actor->globalMatrix);
				}
				else {
					impostorMatrices.push_back(actor->globalMatrix);
				}
			}

			if (actorMatrices.size()) {
				Buffer::VertexBuffer* buffer = nullptr;
				auto key = actorBatchBuffers.find(mesh);
				if (key == actorBatchBuffers.end()) {
					buffer = new Buffer::VertexBuffer(AE_FLOAT, 16,
						sizeof(mat4), actorMatrices.size(), actorMatrices.data(),
						AE_BUFFER_DYNAMIC_STORAGE);
					actorBatchBuffers[mesh] = buffer;
				}
				else {
					buffer = key->second;
					buffer->SetSize(actorMatrices.size(), actorMatrices.data());
				}				
				actorBatch->GetObject()->vertexArray.AddInstancedComponent(4,
					buffer);
			}

			if (impostorMatrices.size()) {
				Buffer::VertexBuffer* buffer = nullptr;
				auto key = impostorBuffers.find(mesh);
				if (key == impostorBuffers.end()) {
					buffer = new Buffer::VertexBuffer(AE_FLOAT, 16,
						sizeof(mat4), impostorMatrices.size(), impostorMatrices.data(),
						AE_BUFFER_DYNAMIC_STORAGE);
					impostorBuffers[mesh] = buffer;
				}
				else {
					buffer = key->second;
					buffer->SetSize(impostorMatrices.size(), impostorMatrices.data());
				}
			}

		}

	}

	void RenderList::Clear() {

		for (auto& key : actorBatches) {
			auto buffer = actorBatchBuffers.find(key.first);
			if (!buffer->second)
				continue;
			if (buffer != actorBatchBuffers.end() && buffer->second->GetSize() > 0)
				key.first->vertexArray.RemoveInstanceComponent(4);
			delete key.second;
		}

		for (auto& key : actorBatchBuffers) {
			if (!key.second)
				continue;
			key.second->SetSize(0);
		}

		for (auto& key : impostorBuffers) {
			if (!key.second)
				continue;
			key.second->SetSize(0);
		}

		actorBatches.clear();
		orderedRenderBatches.clear();

	}

}