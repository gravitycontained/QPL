#include <qpl/QSF/framework.hpp>

#if !defined (QPL_NO_SFML) || defined(QPL_USE_ALL)

namespace qsf {

	void qsf::framework::draw_call() {
		if (this->states.back()->is_clear_allowed()) {
			this->states.back()->clear();
			for (auto& i : this->m_render_textures) {
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
	void qsf::framework::display() {
		this->window.display();
	}
	void qsf::framework::internal_update() {

		this->m_frametime = this->m_frametime_clock.elapsed_reset();

		this->event.reset(this->window);
		sf::Event event;
		while (this->window.pollEvent(event)) {
			this->event.update(event);
		}
		this->event.m_frame_time = this->m_frametime;

		if (this->event.resized()) {
			auto new_dimension = this->event.resized_size();
			sf::FloatRect view(0.0f, 0.0f, static_cast<float>(new_dimension.x), static_cast<float>(new_dimension.y));
			this->window.setView(sf::View(view));
			this->m_dimension = new_dimension;
			this->event.m_screen_dimension = this->m_dimension;

			for (auto& i : this->m_render_textures) {
				if (i.second.is_resize_with_window_enabled()) {
					i.second.resize(new_dimension, true);
				}
			}

			this->states.back()->call_on_resize();
		}
		qsf::update_sounds();
		qpl::update_tasks();
		auto focus_before = this->m_focus;
		auto focus = this->window.hasFocus();
		this->m_focus = focus;
		this->m_lost_focus = (focus_before && !focus);
		this->m_gained_focus = (!focus_before && focus);
		if (this->m_lost_focus) {
			this->m_no_focus_timer.reset();
		}
		if (this->m_gained_focus) {
			this->m_no_focus_time = this->m_no_focus_timer.elapsed();
		}
	}
	bool qsf::framework::game_loop_segment() {
		if (!this->is_created()) {
			this->create();
		}

		auto focus_before = this->m_focus;
		this->internal_update();

		if (this->m_update_if_no_focus || this->m_focus || (focus_before != this->m_focus)) {
			this->states.back()->updating();
			++this->states.back()->m_frame_ctr;
		}
		this->states.back()->update_close_window();

		if (this->states.back()->m_pop_this_state) {
			this->states.pop_back();
			if (this->states.empty()) {
				return false;
			}
		}
		this->draw_call();
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

	void qsf::framework::set_framerate_limit(qpl::u32 value) {
		this->m_framerate_limit = value;
	}
	qpl::u32 qsf::framework::get_framerate_limit() const {
		return this->m_framerate_limit;
	}
	void qsf::framework::disable_framerate_limit() {
		this->m_framerate_limit = 0u;
	}
	void qsf::framework::enable_update_if_no_focus() {
		this->m_update_if_no_focus = true;
	}
	void qsf::framework::disable_update_if_no_focus() {
		this->m_update_if_no_focus = false;
	}
	bool qsf::framework::is_update_if_no_focus_enabled() const {
		return this->m_update_if_no_focus;
	}
	bool qsf::framework::has_focus() const {
		return this->m_focus;
	}
	bool qsf::framework::has_gained_focus() const {
		return this->m_gained_focus;
	}
	bool qsf::framework::has_lost_focus() const {
		return this->m_lost_focus;
	}
	qpl::time qsf::framework::no_focus_time() const {
		if (this->m_focus || this->m_gained_focus) {
			return this->m_no_focus_time;
		}
		else {
			return qpl::time(0);
		}
	}
	qpl::time qsf::framework::run_time() const {
		return this->m_run_time_clock.elapsed();
	}
	qpl::time qsf::framework::frame_time() const {
		return this->m_frametime;
	}
	void qsf::framework::add_render(const std::string& name, bool smooth) {
		this->m_render_textures[name].set_antialiasing(this->context_settings.antialiasingLevel);
		this->m_render_textures[name].resize(this->m_dimension, true);
		this->m_render_textures[name].set_smooth(smooth);
	}
	qsf::render_texture& qsf::framework::get_render(const std::string& name) {
		if (this->m_render_textures.find(name) == this->m_render_textures.cend()) {
			throw std::runtime_error(qpl::to_string("couldn't find render with name \"", name, "\""));
		}
		return this->m_render_textures[name];
	}
	const qsf::render_texture& qsf::framework::get_render(const std::string& name) const {
		if (this->m_render_textures.find(name) == this->m_render_textures.cend()) {
			throw std::runtime_error(qpl::to_string("couldn't find render with name \"", name, "\""));
		}
		return this->m_render_textures.at(name);
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

			if (this->states.size()) {
				this->states.back()->call_before_create();
			}

			sf::String s = this->m_title.c_str(); //??? SFML why is this needed

			this->event.m_screen_dimension = this->m_dimension;
			this->window.create(sf::VideoMode({ this->m_dimension.x, this->m_dimension.y }), s, this->m_style, this->context_settings);
			this->window.setVerticalSyncEnabled(true);
			this->m_created = true;
			this->window.setFramerateLimit(this->m_framerate_limit);

			if (this->states.size()) {
				this->states.back()->call_after_create();
			}

		}
	}
	bool qsf::framework::is_open() const {
		return this->window.isOpen();
	}
	bool qsf::framework::is_created() const {
		return this->m_created;
	}
	void qsf::framework::set_info(const std::string& title, qpl::vector2u dimension, qpl::u32 style) {
		this->set_title(title);
		this->set_dimension(dimension);
		this->set_style(style);
	}
	void qsf::framework::set_title(const std::string& title) {
		this->m_title = title;
		if (this->m_created) {
			this->window.setTitle(title);
		}
	}
	void qsf::framework::set_dimension(qpl::vector2u dimension) {
		this->m_dimension = dimension;
		this->event.m_screen_dimension = dimension;
	}
	void qsf::framework::set_style(qpl::u32 style) {
		this->m_style = style;
	}
	void qsf::framework::hide_cursor() {
		this->window.setMouseCursorVisible(false);
		//todo
	}
	void qsf::framework::set_window_position(qpl::vector2u position) {
		this->window.setPosition(sf::Vector2i(position));
	}
	qpl::vector2u qsf::framework::get_window_position() const {
		return qpl::vector2u(this->window.getPosition());
	}
	void qsf::framework::show_cursor() {
		this->window.setMouseCursorVisible(true);
		//todo
	}
	void qsf::framework::set_cursor_position(qpl::vector2i position) {
		//todo
	}

	void qsf::base_state::clear() {
		this->framework->window.clear(this->clear_color);
	}
	void qsf::base_state::call_on_resize() {

	}
	void qsf::base_state::call_before_create() {

	}
	void qsf::base_state::call_on_close() {

	}
	void qsf::base_state::call_after_create() {

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
		if (this->event().window_closed() && this->m_allow_exit) {
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
	qpl::vector2i qsf::base_state::dimension() const {
		return this->framework->m_dimension;
	}
	qpl::vector2f qsf::base_state::center() const {
		return qpl::vector2f(this->framework->m_dimension) / 2;
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
		this->m_pop_this_state = true;
	}
	void qsf::base_state::allow_exit() {
		this->m_allow_exit = true;
	}
	void qsf::base_state::disallow_exit() {
		this->m_allow_exit = false;
	}
	bool qsf::base_state::is_exit_allowed() const {
		return this->m_allow_exit;
	}
	void qsf::base_state::allow_clear() {
		this->m_allow_clear = true;
	}
	void qsf::base_state::disallow_clear() {
		this->m_allow_clear = false;
	}
	bool qsf::base_state::is_clear_allowed() const {
		return this->m_allow_clear;
	}
	void qsf::base_state::allow_display() {
		this->m_allow_display = true;
	}
	void qsf::base_state::disallow_display() {
		this->m_allow_display = false;
	}
	bool qsf::base_state::is_display_allowed() const {
		return this->m_allow_display;
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
	qpl::size qsf::base_state::frame_ctr() const {
		return this->m_frame_ctr;
	}
	qpl::time qsf::base_state::no_focus_time() const {
		return this->framework->no_focus_time();
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