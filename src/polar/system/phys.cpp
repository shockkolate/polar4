#include <polar/component/phys.h>
#include <polar/system/event.h>
#include <polar/system/phys.h>

namespace polar {
namespace system {
	void phys::init() {
		using arg_t = support::event::arg;
		auto eventM = engine->get<system::event>().lock();
		auto f      = [this](arg_t arg) {
			auto seconds = arg.float_;
			(void)seconds;

			auto range =
			    engine->objects.right.equal_range(&typeid(component::phys));
			for(auto it1 = range.first; it1 != range.second; ++it1) {
				auto id1   = it1->info.get();
				auto phys1 = static_cast<component::phys *>(id1);
				for(auto it2 = std::next(it1); it2 != range.second; ++it2) {
					auto id2 = it2->info.get();
					if(id1 != id2) {
						auto phys2 = static_cast<component::phys *>(id2);
						auto &det1 = *phys1->detector;
						auto &det2 = *phys2->detector;
						auto pair =
						    std::make_pair(&typeid(det1), &typeid(det2));
						auto search = resolvers.find(pair);
						if(search != resolvers.cend()) {
							auto b = search->second->operator()(
							    phys1->detector, phys2->detector);
							if(b) {
								debugmanager()->info("collision!");
							}
						}
					}
				}
			}
		};
		dtors.emplace_back(eventM->listenfor("integrator", "ticked", f));
	}
}
}
