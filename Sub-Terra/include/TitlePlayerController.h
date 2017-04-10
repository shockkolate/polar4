#pragma once

#include "PositionComponent.h"
#include "OrientationComponent.h"
#include "PlayerCameraComponent.h"
#include "World.h"

class TitlePlayerController : public System {
private:
	IDType object;
	Point2 orientVel;
protected:
	inline void Init() override {
		engine->AddComponent<PlayerCameraComponent>(object);
	}

	inline void Update(DeltaTicks &dt) override {
		auto pos = engine->GetComponent<PositionComponent>(object);
		auto orient = engine->GetComponent<OrientationComponent>(object);
		auto camera = engine->GetComponent<PlayerCameraComponent>(object);
		auto world = engine->GetSystem<World>().lock();

		orientVel *= static_cast<float>(glm::pow(0.993, dt.Seconds() * 1000.0));

		unsigned int i = 0;
		auto average = Point2(0);
		if(world) {
			for(float x = -fieldOfView; x < fieldOfView; x += 0.5f) {
				for(float y = -fieldOfView; y < fieldOfView; y += 0.5f) {
					for(float d = 1; d < viewDistance; d += 0.5f) {
						auto abs = glm::inverse(orient->orientation) * glm::inverse(camera->orientation) * Point4(x, y, -d, 1);
						auto eval = world->Eval(pos->position.Get() + Point3(abs));
						if(eval) {
							average.x += 0.0007f * ((y >= 0) ? 1 : -1) / (glm::max(1.0f, d - 2) * 2 / viewDistance);
							average.y += 0.0007f * ((x <= 0) ? 1 : -1) / (glm::max(1.0f, d - 2) * 2 / viewDistance);
							++i;
							break;
						}
					}
				}
			}
		}

		if(i > 0) {
			average /= static_cast<float>(i);
			if(average.length() < 0.1f && average.length() >= 0) { average = Point2(0, 1); }
			if(average.length() > 0.1f && average.length() <= 0) { average = Point2(0, -1); }
			orientVel += average;
		}

		orient->orientation = glm::quat(Point3(orientVel.x, 0, 0)) * glm::quat(Point3(0, orientVel.y, 0)) * orient->orientation;

		const auto forward = glm::normalize(Point4(0, 0, -1, 1));
		auto abs = (glm::inverse(orient->orientation) * glm::inverse(camera->orientation) * forward) * velocity * dt.Seconds();

		pos->position.Derivative()->x = abs.x;
		pos->position.Derivative()->y = abs.y;
		pos->position.Derivative()->z = abs.z;
	}
public:
	const int fieldOfView = 5;
	const float viewDistance = 40.0f;
	const float velocity = 1000.0f;

	static bool IsSupported() { return true; }
	TitlePlayerController(Polar *engine, const IDType object) : System(engine), object(object) {}
};
