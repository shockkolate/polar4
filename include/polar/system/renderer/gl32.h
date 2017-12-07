#pragma once

#include <array>
#include <boost/container/flat_set.hpp>
#include <functional>
#include <polar/asset/font.h>
#include <polar/asset/shaderprogram.h>
#include <polar/component/model.h>
#include <polar/component/sprite/base.h>
#include <polar/component/text.h>
#include <polar/property/gl32/model.h>
#include <polar/property/gl32/sprite.h>
#include <polar/support/gl32/fontcache.h>
#include <polar/support/gl32/pipelinenode.h>
#include <polar/system/renderer/base.h>
#include <polar/util/gl.h>
#include <polar/util/sdl.h>
#include <polar/util/sharedptr_less.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace polar::system::renderer {
	class gl32 : public base {
		using pipelinenode = support::gl32::pipelinenode;
		using fontcache_t  = support::gl32::fontcache;
		using model_p      = property::gl32::model;
		using sprite_p     = property::gl32::sprite;

	  private:
		bool inited     = false;
		bool capture    = false;
		bool fullscreen = false;

		SDL_Window *window;
		SDL_GLContext context;
		std::vector<std::string> pipelineNames;
		std::vector<pipelinenode> nodes;
		boost::container::flat_multiset<std::shared_ptr<model_p>,
		                                sharedptr_less<model_p>>
		    modelPropertyPool;
		std::unordered_map<std::shared_ptr<polar::asset::font>, fontcache_t>
		    fontCache;

		GLuint viewportVAO;
		GLuint spriteProgram;

		std::shared_ptr<core::destructor> fpsDtor;
		IDType fpsID = 0;

		std::unordered_map<std::string, glm::uint32> uniformsU32;
		std::unordered_map<std::string, Decimal> uniformsFloat;
		std::unordered_map<std::string, Point3> uniformsPoint3;

		std::vector<std::string> changedUniformsU32;
		std::vector<std::string> changedUniformsFloat;
		std::vector<std::string> changedUniformsPoint3;

		void init() override final;
		void update(DeltaTicks &) override final;
		void rendersprite(IDType);
		void rendertext(IDType);

		std::shared_ptr<model_p> getpooledmodelproperty(const GLsizei required);

		void uploadmodel(std::shared_ptr<component::model> model);

		void componentadded(IDType id, std::type_index ti,
		                    std::weak_ptr<component::base> ptr) override final;
		void componentremoved(IDType id, std::type_index ti) override final;

		Mat4 calculate_projection();
		void project(GLuint programID);

		void initGL();
		void handleSDL(SDL_Event &);
		void makepipeline(const std::vector<std::string> &) override final;
		GLuint makeprogram(std::shared_ptr<polar::asset::shaderprogram>);

	  public:
		Decimal fps = 60.0;

		static bool supported();
		gl32(core::polar *engine, const std::vector<std::string> &names)
		    : base(engine), pipelineNames(names) {}
		~gl32();

		void setmousecapture(bool capture) override final;
		void setfullscreen(bool fullscreen) override final;
		void setpipeline(const std::vector<std::string> &names) override final;
		void setclearcolor(const Point4 &color) override final;

		Decimal getuniform_decimal(const std::string &name,
		                           const Decimal def) override final;
		Point3 getuniform_point3(const std::string &name,
		                         const Point3 def) override final;

		void setuniform(const std::string &name, glm::uint32 x,
		                bool force = false) override final;
		void setuniform(const std::string &name, Decimal x,
		                bool force = false) override final;
		void setuniform(const std::string &name, Point3 p,
		                bool force = false) override final;

		bool uploaduniform(GLuint program, const std::string &name,
		                   glm::int32 x);
		bool uploaduniform(GLuint program, const std::string &name,
		                   glm::uint32 x);
		bool uploaduniform(GLuint program, const std::string &name, Decimal x);
		bool uploaduniform(GLuint program, const std::string &name, Point2 p);
		bool uploaduniform(GLuint program, const std::string &name, Point3 p);
		bool uploaduniform(GLuint program, const std::string &name, Point4 p);
		bool uploaduniform(GLuint program, const std::string &name, Mat4 m);
	};
} // namespace polar::system::renderer
