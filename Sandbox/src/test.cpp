#include "pch.h"
#include <prev.h>

using namespace prev;

struct Position {
	Position(float sx, float sy) : 
		x(sx), y(sy) {}
	float x, y;
};

struct Renderable {

};

struct NoobEvent {
	NoobEvent(entityx::Entity entity) {
		printf("GGEZ\n");
	}
};

struct MovementSystem : public entityx::System<MovementSystem> {
	virtual void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override {
		entities.each<Position>([&events, dt](entityx::Entity entity, Position & pos)->void {
			pos.x += 2.0f;
			pos.y += 2.0f;
			if (pos.x > 2000.0f) {
				events.emit<NoobEvent>(entity);
				system("PAUSE");
			}
		});
	}
};

int main() {
	Log::ShouldShowLogOnTerminal(false);
	auto app = new Application();
	app->GetWindow().SetVSync(true);
	app->Run();
	delete app;

	entityx::EntityX ex;
	entityx::Entity entity = ex.entities.create();
	entity.assign<Position>(10, 10);

	ex.systems.add<MovementSystem>();
	ex.systems.configure();

	return 0;
}