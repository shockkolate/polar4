#pragma once

#include <polar/component/sprite/base.h>

namespace polar::component::sprite {
	class box : public base {
	  public:
		void render_me() override;
	};
} // namespace polar::component::sprite
