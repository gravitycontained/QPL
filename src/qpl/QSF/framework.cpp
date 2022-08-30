#include <qpl/QSF/framework.hpp>
#if defined QPL_INTERN_SFML_USE

#if !defined QPL_INTERN_STATIC_NO_GLEW
#include <GL/glew.h>
#include <qpl/QGL/shader.hpp>
#endif

namespace qsf {
	qsf::framework::framework() {
		this->set_title(" ");
		this->set_dimension({ 1280, 720 });
		this->set_style(sf::Style::Default);
#if !defined QPL_INTERN_STATIC_NO_GLEW
		this->disable_gl();
#endif
		this->created = false;
	}
	qsf::framework::~framework() {
#if !defined QPL_INTERN_STATIC_NO_GLEW
		if (this->created_gl) {
			this->destroy_gl();
		}
#endif
	}

	void qsf::framework::draw_call() {
		if (this->states.back()->is_clear_allowed()) {
			this->states.back()->clear();
			for (auto& i : this->render_textures) {
				if (i.second.is_clear_with_window_enabled()) {
					i.second.clear();
				}
			}
		}
		this->states.back()->drawing();
		if (this->states.back()->is_display_allowed()) {
			this->window.display();
		}
	}

	void qsf::framework::init_back() {
		if (this->states.size() && !this->states.back()->is_initalized) {
			this->states.back()->init();
			this->states.back()->is_initalized = true;
		}
	}
	void qsf::framework::display() {
		this->window.display();
	}
	void qsf::framework::internal_update() {

		this->frametime = this->frametime_clock.elapsed_reset();

		this->event.reset(this->window);
		sf::Event event;
		while (this->window.pollEvent(event)) {
			this->event.update(event);
		}
		qpl::f64 f = this->frametime.nsecs() * this->speed_factor;
		if (this->states.size()) {
			f *= this->states.back()->speed_factor;
		}
		this->event.m_frame_time = qpl::time(qpl::u64_cast(f));

		if (this->event.resized()) {
			auto new_dimension = this->event.resized_size();
			sf::FloatRect view(0.0f, 0.0f, static_cast<float>(new_dimension.x), static_cast<float>(new_dimension.y));
			this->window.setView(sf::View(view));
			this->dimension = new_dimension;
			this->event.m_screen_dimension = this->dimension;

			for (auto& i : this->render_textures) {
				if (i.second.is_resize_with_window_enabled()) {
					i.second.resize(new_dimension, true);
				}
			}

#if !defined QPL_INTERN_STATIC_NO_GLEW
			if (this->use_gl && this->states.back()->use_gl) {
				this->resize_gl();
			}
#endif

			this->states.back()->call_on_resize();
		}
		qsf::update_sounds();
		qpl::update_tasks();
		auto focus_before = this->focus;
		auto focus = this->window.hasFocus();
		this->focus = focus;
		this->lost_focus = (focus_before && !focus);
		this->gained_focus = (!focus_before && focus);
		if (this->lost_focus) {
			this->no_focus_timer.reset();
		}
		if (this->gained_focus) {
			this->no_focus_time = this->no_focus_timer.elapsed();
		}
	}
	bool qsf::framework::game_loop_segment() {
		if (!this->is_created()) {
			this->create();
		}

		auto focus_before = this->focus;
		this->internal_update();

		if (this->state_size_before != this->states.size()) {
			this->states.back()->call_on_activate();
			this->state_size_before = this->states.size();
		}
		
		if (this->update_if_no_focus || this->focus || (focus_before != this->focus)) {
			this->states.back()->updating();
			this->states.back()->last_dimension = this->dimension;
			++this->states.back()->frame_ctr;
		}
		this->states.back()->update_close_window();
		
		bool allow_draw = this->states.size() == this->state_size_before;
		if (this->states.back()->is_pop_this_state) {
			this->states.pop_back();
			if (this->states.empty()) {
				return false;
			}
			this->init_back();
			this->states.back()->call_on_activate();
			this->state_size_before = this->states.size();
			allow_draw = false;
			if (this->states.back()->last_dimension != this->dimension) {
				this->states.back()->call_on_resize();
			}
		}
		if (allow_draw) {
			this->draw_call();
		}
		return true;
	}
	void qsf::framework::game_loop() {
		if (!this->is_created()) {
			this->create();
		}

		while (this->is_open()) {
			if (!this->game_loop_segment()) {
				break;
			}
		}
	}

#if !defined QPL_INTERN_STATIC_NO_GLEW
	void qsf::framework::enable_gl() {
		this->context_settings.depthBits = 24;
		this->context_settings.sRgbCapable = false;
		this->context_settings.antialiasingLevel = 12u;
		this->use_gl = true;
	}
	void qsf::framework::disable_gl() {
		this->context_settings.depthBits = 0;
		this->context_settings.antialiasingLevel = 12u;
		this->use_gl = false;
	}
	void qsf::framework::create_gl() {
		if (!this->created) {
			this->create();
		}
		if (this->created_gl) {
			return;
		}
		this->created_gl = true;
		qgl::gl::enable(GL_DEPTH_TEST);
		qgl::gl::enable(GL_CULL_FACE);

		switch (this->gl_cull_face) {
		case cull_face::front: qgl::gl::cull_face(GL_FRONT); break;
			case cull_face::back: qgl::gl::cull_face(GL_BACK); break;
			case cull_face::front_and_back: qgl::gl::cull_face(GL_FRONT_AND_BACK); break;
		};

		qgl::gl::front_face(GL_CW);
		qgl::gl::enable_glew_experimental();

		this->resize_gl();
	}
	void qsf::framework::destroy_gl() {
		for (auto& shader : qgl::shaders) {
			shader.second.destroy();
		}
	}
	void qsf::framework::resize_gl() {
		qgl::gl::viewport(0, 0, this->dimension.x, this->dimension.y);
	}
#endif
	void qsf::framework::enable_vsync() {
		this->use_vsync = true;
		this->window.setVerticalSyncEnabled(this->use_vsync);
	}
	void qsf::framework::disable_vsync() {
		this->use_vsync = false;
		this->window.setVerticalSyncEnabled(this->use_vsync);
	}
	bool qsf::framework::is_vsync_enabled() {
		return this->use_vsync;
	}
	void qsf::framework::set_active(bool active) {
		this->window.setActive(active);
	}
	void qsf::framework::push_gl_states() {
		this->window.pushGLStates();
	}
	void qsf::framework::pop_gl_states() {
		this->window.popGLStates();
	}

	void qsf::framework::set_framerate_limit(qpl::u32 value) {
		this->framerate_limit = value;
	}
	qpl::u32 qsf::framework::get_framerate_limit() const {
		return this->framerate_limit;
	}
	void qsf::framework::disable_framerate_limit() {
		this->framerate_limit = 0u;
	}
	void qsf::framework::enable_update_if_no_focus() {
		this->update_if_no_focus = true;
	}
	void qsf::framework::disable_update_if_no_focus() {
		this->update_if_no_focus = false;
	}
	bool qsf::framework::is_update_if_no_focus_enabled() const {
		return this->update_if_no_focus;
	}
	bool qsf::framework::has_focus() const {
		return this->focus;
	}
	bool qsf::framework::has_gained_focus() const {
		return this->gained_focus;
	}
	bool qsf::framework::has_lost_focus() const {
		return this->lost_focus;
	}
	void qsf::framework::close() {
		this->window.close();
	}
	qpl::time qsf::framework::get_no_focus_time() const {
		if (this->focus || this->gained_focus) {
			return this->no_focus_time;
		}
		else {
			return qpl::time(0);
		}
	}
	qpl::time qsf::framework::run_time() const {
		return this->run_time_clock.elapsed();
	}
	qpl::time qsf::framework::frame_time() const {
		return this->frametime;
	}
	void qsf::framework::add_render(const std::string& name, bool smooth) {
		this->render_textures[name].set_antialiasing(this->context_settings.antialiasingLevel);
		this->render_textures[name].resize(this->dimension, true);
		this->render_textures[name].set_smooth(smooth);
	}
	qsf::render_texture& qsf::framework::get_render(const std::string& name) {
		if (this->render_textures.find(name) == this->render_textures.cend()) {
			throw std::runtime_error(qpl::to_string("couldn't find render with name \"", name, "\""));
		}
		return this->render_textures[name];
	}
	const qsf::render_texture& qsf::framework::get_render(const std::string& name) const {
		if (this->render_textures.find(name) == this->render_textures.cend()) {
			throw std::runtime_error(qpl::to_string("couldn't find render with name \"", name, "\""));
		}
		return this->render_textures.at(name);
	}

	void qsf::framework::play_sound(const std::string& name, qpl::f32 volume, qpl::f32 speed) {
		qsf::play_sound(name, volume, speed);
	}
	void qsf::framework::add_font(const std::string& name, const std::string& path) {
		qsf::add_font(name, path);
	}
	void qsf::framework::add_sound(const std::string& name, const std::string& path) {
		qsf::add_sound(name, path);
	}
	void qsf::framework::add_texture(const std::string& name, const std::string& path) {
		qsf::add_texture(name, path);
	}
	void qsf::framework::add_image(const std::string& name, const std::string& path) {
		qsf::add_image(name, path);
	}
	void qsf::framework::add_sprite(const std::string& name, const std::string& path) {
		qsf::add_sprite(name, path);
	}
	void qsf::framework::add_sprite(const std::string& name, sf::Texture& texture) {
		qsf::add_sprite(name, texture);
	}
	void qsf::framework::add_shader(const std::string& name, const std::string& path, sf::Shader::Type shader_type) {
		qsf::add_shader(name, path, shader_type);
	}
	void qsf::framework::add_shader(const std::string& name, const std::string& path) {
		qsf::add_shader(name, path);
	}
	void qsf::framework::add_text(const std::string& name) {
		qsf::add_text(name);
	}


	void qsf::framework::add_font_from_memory(const std::string& name, const std::string& memory) {
		qsf::add_font_from_memory(name, memory);
	}
	void qsf::framework::add_sound_from_memory(const std::string& name, const std::string& memory) {
		qsf::add_sound_from_memory(name, memory);
	}
	void qsf::framework::add_texture_from_memory(const std::string& name, const std::string& memory) {
		qsf::add_texture_from_memory(name, memory);
	}
	void qsf::framework::add_shader_from_memory(const std::string& name, const std::string& memory, sf::Shader::Type shader_type) {
		qsf::add_shader_from_memory(name, memory, shader_type);
	}


	sf::Font& qsf::framework::get_font(const std::string& name) {
		return qsf::get_font(name);
	}
	sf::SoundBuffer& qsf::framework::get_sound(const std::string& name) {
		return qsf::get_sound(name);
	}
	sf::Texture& qsf::framework::get_texture(const std::string& name) {
		return qsf::get_texture(name);
	}
	sf::Sprite& qsf::framework::get_sprite(const std::string& name) {
		return qsf::get_sprite(name);
	}
	sf::Image& qsf::framework::get_image(const std::string& name) {
		return qsf::get_image(name);
	}
	sf::Shader& qsf::framework::get_shader(const std::string& name) {
		return qsf::get_shader(name);
	}
	qsf::text& qsf::framework::get_text(const std::string& name) {
		return qsf::get_text(name);
	}

	const sf::Font& qsf::framework::get_font(const std::string& name) const {
		return qsf::get_font(name);
	}
	const sf::SoundBuffer& qsf::framework::get_sound(const std::string& name) const {
		return qsf::get_sound(name);
	}
	const sf::Texture& qsf::framework::get_texture(const std::string& name) const {
		return qsf::get_texture(name);
	}
	const sf::Image& qsf::framework::get_image(const std::string& name) const {
		return qsf::get_image(name);
	}
	const sf::Sprite& qsf::framework::get_sprite(const std::string& name) const {
		return qsf::get_sprite(name);
	}
	const sf::Shader& qsf::framework::get_shader(const std::string& name) const {
		return qsf::get_shader(name);
	}
	const qsf::text& qsf::framework::get_text(const std::string& name) const {
		return qsf::get_text(name);
	}

	void qsf::framework::create() {
		if (!this->is_created()) {

			sf::String s = this->title.c_str(); //??? SFML why is this needed

			this->event.m_screen_dimension = this->dimension;
			this->window.create(sf::VideoMode({ this->dimension.x, this->dimension.y }), s, this->style, this->context_settings);
			this->window.setFramerateLimit(this->framerate_limit);
			this->created = true;

#if !defined QPL_INTERN_STATIC_NO_GLEW
			if (this->use_gl) {
				this->create_gl();
			}
#endif

			if (this->states.size()) {
				this->init_back();
			}
		}
	}
	bool qsf::framework::is_open() const {
		return this->window.isOpen();
	}
	bool qsf::framework::is_created() const {
		return this->created;
	}
	void qsf::framework::set_info(const std::string& title, qpl::vector2u dimension, qpl::u32 style) {
		this->set_title(title);
		this->set_dimension(dimension);
		this->set_style(style);
	}
	void qsf::framework::set_title(const std::string& title) {
		this->title = title;
		if (this->created) {
			this->window.setTitle(title);
		}
	}
	void qsf::framework::set_dimension(qpl::vector2u dimension) {
		this->dimension = dimension;
		this->event.m_screen_dimension = dimension;

		for (auto& i : this->states) {
			i->last_dimension = this->dimension;
		}
	}
	void qsf::framework::set_style(qpl::u32 style) {
		this->style = style;
	}
	void qsf::framework::hide_cursor() {
		this->window.setMouseCursorVisible(false);
	}
	void qsf::framework::set_window_position(qpl::vector2u position) {
		this->window.setPosition(sf::Vector2i(position));
	}
	qpl::vector2u qsf::framework::get_window_position() const {
		return qpl::vector2u(this->window.getPosition());
	}
	void qsf::framework::show_cursor() {
		this->window.setMouseCursorVisible(true);
	}
	void qsf::framework::set_cursor_position(qpl::vector2i position) {
		sf::Mouse::setPosition(position, this->window);
		this->event.m_mouse_position_screen = position;
		this->event.m_delta_mouse_position = this->event.m_mouse_position_screen_before - this->event.m_mouse_position_screen;
		this->event.m_mouse_position_screen_before = this->event.m_mouse_position_screen;
	}

	void qsf::framework::set_speed_factor(qpl::f64 speed) {
		this->speed_factor = speed;
	}

	void qsf::framework::set_icon(std::string path, std::string name) {
		this->add_image(name, path);
		const auto& image = this->get_image(name);
		this->window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
	}

	void qsf::base_state::clear() {
#if !defined QPL_INTERN_STATIC_NO_GLEW
		if (this->use_gl) {
			auto c = qpl::frgba(this->clear_color);
			qgl::gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			qgl::gl::clear_color(c.r, c.g, c.b, c.a);
		}
		else {
			this->framework->window.clear(this->clear_color);
		}
#else
		this->framework->window.clear(this->clear_color);
#endif

	}
	void qsf::base_state::call_on_resize() {

	}
	void qsf::base_state::call_before_create() {

	}
	void qsf::base_state::call_on_close() {

	}
	void qsf::base_state::call_on_activate() {

	}
	void qsf::base_state::draw_call() {
		this->framework->draw_call();
	}
	void qsf::base_state::display() {
		this->framework->display();
	}
	bool qsf::base_state::game_loop_segment() {
		return this->framework->game_loop_segment();
	}
	void qsf::base_state::set_antialising_level(qpl::u32 antialising) {
		this->framework->context_settings.antialiasingLevel = antialising;
	}
	void qsf::base_state::set_sRGB(bool srgb) {
		this->framework->context_settings.sRgbCapable = srgb;
	}
	void qsf::base_state::set_depth_bits(qpl::u32 depth_bits) {
		this->framework->context_settings.depthBits = depth_bits;
	}
	void qsf::base_state::set_shader(const std::string& name) {
		this->render_states.shader = &qsf::get_shader(name);
	}
	void qsf::base_state::set_shader(sf::Shader& shader) {
		this->render_states.shader = &shader;
	}
	void qsf::base_state::unbind_shader() {
		this->render_states.shader = nullptr;
	}
	void qsf::base_state::set_active(bool active) {
		this->framework->set_active(active);
	}
	void qsf::base_state::push_gl_states() {
		this->states_pushed = true;
		this->framework->push_gl_states();
	}
	void qsf::base_state::pop_gl_states() {
		this->states_pushed = false;
		this->framework->pop_gl_states();
	}
#if !defined QPL_INTERN_STATIC_NO_GLEW
	void qsf::base_state::enable_gl() {
		this->use_gl = true;

		if (!this->framework->created_gl) {
			this->framework->create_gl();
		}
	}
	void qsf::base_state::disable_gl() {
		this->use_gl = false;
	}
#endif

	void qsf::base_state::reset_view() {
		this->render_states = sf::RenderStates::Default;
	}
	void qsf::base_state::create() {
		return this->framework->create();
	}
	bool qsf::base_state::is_open() const {
		return this->framework->is_open();
	}
	void qsf::base_state::update_close_window() {
		if (this->event().window_closed() && this->is_allow_exit) {
			this->framework->window.close();
			this->call_on_close();
		}
	}
	void qsf::base_state::hide_cursor(){
		this->framework->hide_cursor();
	}
	void qsf::base_state::show_cursor() {
		this->framework->show_cursor();
	}
	void qsf::base_state::set_cursor_position(qpl::vector2i position) {
		this->framework->set_cursor_position(position);
	}
	void qsf::base_state::set_window_position(qpl::vector2u position) {
		this->framework->set_window_position(position);
	}
	qpl::vector2u qsf::base_state::get_window_position() const {
		return this->framework->get_window_position();
	}
	void qsf::base_state::set_speed_factor(qpl::f64 speed) {
		this->speed_factor = speed;
	}
	qpl::vector2i qsf::base_state::dimension() const {
		return this->framework->dimension;
	}
	qpl::vector2f qsf::base_state::center() const {
		return qpl::vector2f(this->framework->dimension) / 2;
	}

	void qsf::base_state::play_sound(const std::string& name, qpl::f32 volume, qpl::f32 speed) {
		qsf::play_sound(name, volume, speed);
	}
	void qsf::base_state::add_font(const std::string& name, const std::string& path) {
		qsf::add_font(name, path);
	}
	void qsf::base_state::add_sound(const std::string& name, const std::string& path) {
		qsf::add_sound(name, path);
	}
	void qsf::base_state::add_texture(const std::string& name, const std::string& path) {
		qsf::add_texture(name, path);
	}
	void qsf::base_state::add_sprite(const std::string& name, const std::string& path) {
		qsf::add_sprite(name, path);
	}
	void qsf::base_state::add_sprite(const std::string & name, sf::Texture& texture) {
		qsf::add_sprite(name, texture);
	}
	void qsf::base_state::add_shader(const std::string& name, const std::string& path, sf::Shader::Type shader_type) {
		qsf::add_shader(name, path, shader_type);
	}
	void qsf::base_state::add_shader(const std::string& name, const std::string& path) {
		qsf::add_shader(name, path);
	}
	void qsf::base_state::add_text(const std::string& name) {
		qsf::add_text(name);
	}


	void qsf::base_state::add_font_from_memory(const std::string& name, const std::string& memory) {
		qsf::add_font_from_memory(name, memory);
	}
	void qsf::base_state::add_sound_from_memory(const std::string& name, const std::string& memory) {
		qsf::add_sound_from_memory(name, memory);
	}
	void qsf::base_state::add_texture_from_memory(const std::string& name, const std::string& memory) {
		qsf::add_texture_from_memory(name, memory);
	}
	void qsf::base_state::add_shader_from_memory(const std::string& name, const std::string& memory, sf::Shader::Type shader_type) {
		qsf::add_shader_from_memory(name, memory, shader_type);
	}


	sf::Font& qsf::base_state::get_font(const std::string& name) {
		return qsf::get_font(name);
	}
	sf::SoundBuffer& qsf::base_state::get_sound(const std::string& name) {
		return qsf::get_sound(name);
	}
	sf::Texture& qsf::base_state::get_texture(const std::string& name) {
		return qsf::get_texture(name);
	}
	sf::Sprite& qsf::base_state::get_sprite(const std::string& name) {
		return qsf::get_sprite(name);
	}
	sf::Shader& qsf::base_state::get_shader(const std::string& name) {
		return qsf::get_shader(name);
	}
	qsf::text& qsf::base_state::get_text(const std::string& name) {
		return qsf::get_text(name);
	}

	const sf::Font& qsf::base_state::get_font(const std::string& name) const {
		return qsf::get_font(name);
	}
	const sf::SoundBuffer& qsf::base_state::get_sound(const std::string& name) const {
		return qsf::get_sound(name);
	}
	const sf::Texture& qsf::base_state::get_texture(const std::string& name) const {
		return qsf::get_texture(name);
	}
	const sf::Sprite& qsf::base_state::get_sprite(const std::string& name) const {
		return qsf::get_sprite(name);
	}
	const sf::Shader& qsf::base_state::get_shader(const std::string& name) const {
		return qsf::get_shader(name);
	}
	const qsf::text& qsf::base_state::get_text(const std::string& name) const {
		return qsf::get_text(name);
	}

	void qsf::base_state::add_render(const std::string& name, bool smooth) {
		this->framework->add_render(name, smooth);
	}
	qsf::render_texture& qsf::base_state::get_render(const std::string& name) {
		return this->framework->get_render(name);
	}
	const qsf::render_texture& qsf::base_state::get_render(const std::string& name) const {
		return this->framework->get_render(name);
	}


	void qsf::base_state::pop_this_state() {
		this->is_pop_this_state = true;
	}
	void qsf::base_state::allow_exit() {
		this->is_allow_exit = true;
	}
	void qsf::base_state::disallow_exit() {
		this->is_allow_exit = false;
	}
	bool qsf::base_state::is_exit_allowed() const {
		return this->is_allow_exit;
	}
	void qsf::base_state::allow_clear() {
		this->is_allow_clear = true;
	}
	void qsf::base_state::disallow_clear() {
		this->is_allow_clear = false;
	}
	bool qsf::base_state::is_clear_allowed() const {
		return this->is_allow_clear;
	}
	void qsf::base_state::allow_display() {
		this->is_allow_display = true;
	}
	void qsf::base_state::disallow_display() {
		this->is_allow_display = false;
	}
	bool qsf::base_state::is_display_allowed() const {
		return this->is_allow_display;
	}
	bool qsf::base_state::has_focus() const {
		return this->framework->has_focus();
	}
	bool qsf::base_state::has_gained_focus() const {
		return this->framework->has_gained_focus();
	}
	bool qsf::base_state::has_lost_focus() const {
		return this->framework->has_lost_focus();
	}
	void qsf::base_state::close() {
		this->framework->close();
	}
	qpl::size qsf::base_state::frame_count() const {
		return this->frame_ctr;
	}
	qpl::time qsf::base_state::get_no_focus_time() const {
		return this->framework->get_no_focus_time();
	}
	qpl::time qsf::base_state::frame_time() const {
		return this->framework->frame_time();
	}
	qpl::f64 qsf::base_state::frame_time_f() const {
		return this->framework->frame_time().secs_f();
	}
	qpl::time qsf::base_state::run_time() const {
		return this->framework->run_time();
	}
	const qsf::event_info& qsf::base_state::event() const {
		return this->framework->event;
	}
	qsf::event_info& qsf::base_state::event() {
		return this->framework->event;
	}
}

#endif