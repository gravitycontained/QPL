#ifndef QSF_FRAMEWORK_HPP
#define QSF_FRAMEWORK_HPP
#pragma once

#if !defined (QPL_NO_SFML) || defined(QPL_USE_ALL)

#include <qpl/qpldeclspec.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>

#include <qpl/QSF/event_info.hpp>
#include <qpl/color.hpp>
#include <qpl/QSF/drawables.hpp>
#include <qpl/QSF/resources.hpp>
#include <qpl/vector.hpp>
#include <qpl/time.hpp>

namespace qsf {
	struct base_state;
	/*
	qsf::framework framework;
	framework.set_dimension({ 1800, 720 });
	framework.set_title("x");
	framework.set_style(sf::Style::Default);
	framework.add_state<state>();
	framework.game_loop();
	*/
	struct framework {
		framework() {
			this->set_title(" ");
			this->set_dimension({ 1280, 720 });
			this->set_style(sf::Style::Default);
			this->m_created = false;

			this->context_settings.antialiasingLevel = 12u;
		}
		

		template<typename C> requires (std::is_base_of_v<base_state, C>)
		void add_state() {
			this->states.push_back(std::make_unique<C>());
			this->states.back()->framework = this;
			this->states.back()->init();
		}
		

		template<typename C> requires (std::is_base_of_v<base_state, C>)
		void add_state(C& state) {
			this->states.push_back(std::make_unique<C>(state));
			this->states.back()->init();
		}

		QPLDLL void play_sound(const std::string& name, qpl::f32 volume = 100.0f, qpl::f32 speed = 1.0f);
		QPLDLL void add_font(const std::string& name, const std::string& path);
		QPLDLL void add_sound(const std::string& name, const std::string& path);
		QPLDLL void add_texture(const std::string& name, const std::string& path);
		QPLDLL void add_sprite(const std::string& name, const std::string& path);
		QPLDLL void add_sprite(const std::string& name, sf::Texture& texture);
		QPLDLL void add_shader(const std::string& name, const std::string& path, sf::Shader::Type shader_type);
		QPLDLL void add_shader(const std::string& name, const std::string& path);
		QPLDLL void add_text(const std::string& name);

		QPLDLL void add_font_from_memory(const std::string& name, const std::string& memory);
		QPLDLL void add_sound_from_memory(const std::string& name, const std::string& memory);
		QPLDLL void add_texture_from_memory(const std::string& name, const std::string& memory);
		QPLDLL void add_shader_from_memory(const std::string& name, const std::string& memory, sf::Shader::Type shader_type);

		QPLDLL sf::Font& get_font(const std::string& name);
		QPLDLL sf::SoundBuffer& get_sound(const std::string& name);
		QPLDLL sf::Texture& get_texture(const std::string& name);
		QPLDLL sf::Sprite& get_sprite(const std::string& name);
		QPLDLL sf::Shader& get_shader(const std::string& name);
		QPLDLL qsf::text& get_text(const std::string& name);

		QPLDLL const sf::Font& get_font(const std::string& name) const;
		QPLDLL const sf::SoundBuffer& get_sound(const std::string& name) const;
		QPLDLL const sf::Texture& get_texture(const std::string& name) const;
		QPLDLL const sf::Sprite& get_sprite(const std::string& name) const;
		QPLDLL const sf::Shader& get_shader(const std::string& name) const;
		QPLDLL const qsf::text& get_text(const std::string& name) const;

		QPLDLL void create();
		QPLDLL bool is_created() const;
		QPLDLL bool is_open() const;
		QPLDLL void set_info(const std::string& title, qpl::vector2u dimension, qpl::u32 style);
		QPLDLL void set_title(const std::string& title);
		QPLDLL void set_dimension(qpl::vector2u dimension);
		QPLDLL void set_style(qpl::u32 style);
		QPLDLL void hide_cursor();
		QPLDLL void set_window_position(qpl::vector2u position);
		QPLDLL qpl::vector2u get_window_position() const;
		QPLDLL void show_cursor();
		QPLDLL void set_cursor_position(qpl::vector2i position);
		QPLDLL void draw_call();
		QPLDLL void display();
		QPLDLL void internal_update();
		QPLDLL bool game_loop_segment();
		QPLDLL void game_loop();


		QPLDLL void set_framerate_limit(qpl::u32 value);
		QPLDLL qpl::u32 get_framerate_limit() const;
		QPLDLL void disable_framerate_limit();

		QPLDLL void enable_update_if_no_focus();
		QPLDLL void disable_update_if_no_focus();
		QPLDLL bool is_update_if_no_focus_enabled() const;

		QPLDLL bool has_focus() const;
		QPLDLL bool has_gained_focus() const;
		QPLDLL bool has_lost_focus() const;

		QPLDLL qpl::time no_focus_time() const;
		QPLDLL qpl::time run_time() const;
		QPLDLL qpl::time frame_time() const;

		QPLDLL void add_render(const std::string& name, bool smooth = true);
		QPLDLL qsf::render_texture& get_render(const std::string& name);
		QPLDLL const qsf::render_texture& get_render(const std::string& name) const;

		std::vector<std::unique_ptr<qsf::base_state>> states;
		qsf::event_info event;
		sf::RenderWindow window;
		std::unordered_map<std::string, qsf::render_texture> m_render_textures;
		sf::ContextSettings context_settings;
		std::string m_title;
		qpl::vector2u m_dimension;
		qpl::u32 m_style = sf::Style::Default;
		qpl::clock m_run_time_clock;
		qpl::clock m_frametime_clock;
		qpl::time m_frametime;
		qpl::u32 m_framerate_limit = 144u;
		bool m_created = false;
		bool m_update_if_no_focus = true;

		bool m_focus = true;
		bool m_lost_focus = false;
		bool m_gained_focus = false;
		qpl::small_clock m_no_focus_timer;
		qpl::time m_no_focus_time;
	};
	
	/* TO OVERLOAD:
		void init() override {
			
		}
		void updating() override {
			
		}
		void drawing() override {
			
		}
	*/
	struct base_state {

		virtual ~base_state() = default;

		virtual void init() = 0;
		virtual void updating() = 0;
		virtual void drawing() = 0;
		
		QPLDLL virtual void clear();
		QPLDLL virtual void call_on_resize();
		QPLDLL virtual void call_before_create();
		QPLDLL virtual void call_on_close();
		QPLDLL virtual void call_after_create();

		QPLDLL void draw_call();
		QPLDLL void display();
		QPLDLL bool game_loop_segment();

		QPLDLL void set_antialising_level(qpl::u32 antialising);
		QPLDLL void set_sRGB(bool srgb);
		QPLDLL void set_depth_bits(qpl::u32 depth_bits);
		QPLDLL void set_shader(const std::string& name);
		QPLDLL void set_shader(sf::Shader& shader);
		QPLDLL void unbind_shader();

		template<typename T>
		void set_view(const qsf::view_rectangle_t<T>& view) {
			this->render_states.transform = view.get_render_states().transform;
		}
		QPLDLL void reset_view();

		template<typename T> requires (qsf::has_any_draw<T>() || (qpl::is_container<T>() && qsf::has_any_draw<qpl::container_deepest_subtype<T>>()))
		void draw(const T& drawable, sf::RenderStates states) {
			if constexpr (qsf::has_any_draw<T>()) {
				if constexpr (qsf::is_render_texture<T>()) {
					this->framework->window.draw(drawable.get_sprite(), states);
				}
				else if constexpr (std::is_base_of<sf::Drawable, T>()) {
					this->framework->window.draw(drawable, states);
				}
				else if constexpr (qsf::has_draw_object<T>()) {
					draw_object draw(this->framework->window, states);
					drawable.draw(draw);
				}
				else if constexpr (qsf::has_draw_sf<T>()) {
					drawable.draw(this->framework->window, states);
				}
			}
			else {
				for (auto& i : drawable) {
					this->draw(i);
				}
			}
		}
		template<typename T> requires (qsf::has_any_draw<T>() || (qpl::is_container<T>() && qsf::has_any_draw<qpl::container_deepest_subtype<T>>()))
		void draw(const T& drawable) {
			if constexpr (qsf::has_any_draw<T>()) {
				if constexpr (qsf::is_render_texture<T>()) {
					this->framework->window.draw(drawable.get_sprite(), this->render_states);
				}
				else if constexpr (std::is_base_of<sf::Drawable, T>()) {
					this->framework->window.draw(drawable, this->render_states);
				}
				else if constexpr (qsf::has_draw_object<T>()) {
					draw_object draw(this->framework->window, this->render_states);
					drawable.draw(draw);
				}
				else if constexpr (qsf::has_draw_sf<T>()) {
					drawable.draw(this->framework->window, this->render_states);
				}
			}
			else {
				for (auto& i : drawable) {
					this->draw(i);
				}
			}
		}

		template<typename T, typename V> requires (qsf::has_any_draw<T>() || (qpl::is_container<T>() && qsf::has_any_draw<qpl::container_deepest_subtype<T>>()))
		void draw(const T& drawable, qsf::view_rectangle_t<V> view) {
			if (!view.enabled) {
				this->draw(drawable);
				return;
			}
			if constexpr (qsf::has_any_draw<T>()) {
				sf::RenderStates states = view.get_render_states();
				if constexpr (qsf::is_render_texture<T>()) {
					this->framework->window.draw(drawable.get_sprite(), states);
				}
				else if constexpr (std::is_base_of<sf::Drawable, T>()) {
					this->framework->window.draw(drawable, states);
				}
				else if constexpr (qsf::has_draw_object<T>()) {
					draw_object draw(this->framework->window, states);
					drawable.draw(draw);
				}
				else if constexpr (qsf::has_draw_sf<T>()) {
					drawable.draw(this->framework->window, states);
				}
			}
			else {
				for (auto& i : drawable) {
					this->draw(i, view);
				}
			}
		}

		template<typename T> requires (qsf::has_any_draw<T>() || (qpl::is_container<T>() && qsf::has_any_draw<qpl::container_deepest_subtype<T>>()))
		void draw_into(const std::string& name, const T& drawable) {
			if constexpr (qsf::has_any_draw<T>()) {
			this->get_render(name).draw(drawable, this->render_states);
			}
			else {
				for (auto& i : drawable) {
					this->draw(name, i);
				}
			}
		}
		template<typename T> requires (qsf::has_any_draw<T>() || (qpl::is_container<T>() && qsf::has_any_draw<qpl::container_deepest_subtype<T>>()))
		void draw_into(const std::string& name, const T& drawable, sf::RenderStates states) {
			if constexpr (qsf::has_any_draw<T>()) {
				this->get_render(name).draw(drawable, states);
			}
			else {
				for (auto& i : drawable) {
					this->draw_into(name, i, states);
				}
			}
		}
		template<typename T, typename V> requires (qsf::has_any_draw<T>() || (qpl::is_container<T>() && qsf::has_any_draw<qpl::container_deepest_subtype<T>>()))
		void draw_into(const std::string& name, const T& drawable, qsf::view_rectangle_t<V> view) {
			if (!view.enabled) {
				this->draw_into(name, drawable);
				return;
			}
			if constexpr (qsf::has_any_draw<T>()) {
				sf::RenderStates states = view.get_render_states();
				this->get_render(name).draw(drawable, states);
			}
			else {
				for (auto& i : drawable) {
					this->draw_into(name, i, view);
				}
			}
		}


		template<typename T> requires (qsf::has_any_draw<T>() || (qpl::is_container<T>() && qsf::has_any_draw<qpl::container_deepest_subtype<T>>()))
		void draw_with_shader(const T& drawable, const std::string& name) {
			if constexpr (qsf::has_any_draw<T>()) {
				sf::RenderStates states = this->render_states;
				states.shader = &qsf::get_shader(name);
				this->draw(drawable, states);
			}
			else {
				for (auto& i : drawable) {
					this->draw_with_shader(i, name);
				}
			}
		}
		template<typename T> requires (qsf::has_any_draw<T>() || (qpl::is_container<T>() && qsf::has_any_draw<qpl::container_deepest_subtype<T>>()))
		void draw_with_shader(const T& drawable, sf::Shader& shader) {
			if constexpr (qsf::has_any_draw<T>()) {
				sf::RenderStates states = this->render_states;
				states.shader = &shader;
				this->draw(drawable, states);
			}
			else {
				for (auto& i : drawable) {
					this->draw_with_shader(i, shader);
				}
			}
		}
		template<typename T> requires (qsf::has_any_draw<T>() || (qpl::is_container<T>() && qsf::has_any_draw<qpl::container_deepest_subtype<T>>()))
		void draw_with_shader_into(const std::string& render_name, const T& drawable, const std::string& shader_name) {
			if constexpr (qsf::has_any_draw<T>()) {
				sf::RenderStates states = this->render_states;
				states.shader = &qsf::get_shader(shader_name);
				this->get_render(render_name).draw(drawable, states);
			}
			else {
				for (auto& i : drawable) {
					this->draw_with_shader_into(drawable, i, shader_name);
				}
			}
		}
		template<typename T> requires (qsf::has_any_draw<T>() || (qpl::is_container<T>() && qsf::has_any_draw<qpl::container_deepest_subtype<T>>()))
		void draw_with_shader_into(const std::string& render_name, const T& drawable, sf::Shader& shader) {
			if constexpr (qsf::has_any_draw<T>()) {
				sf::RenderStates states = this->render_states;
				states.shader = &shader;
				this->get_render(render_name).draw(drawable, states);
			}
			else {
				for (auto& i : drawable) {
					this->draw_with_shader_into(drawable, i, shader);
				}
			}
		}

		template<typename T> requires (qsf::has_update<T>() || (qpl::is_container<T>() && qsf::has_update<qpl::container_deepest_subtype<T>>()))
		void update(T& updatable) {
			if constexpr (qsf::has_update<T>()) {
				updatable.update(this->event());
			}
			else {
				for (auto& i : updatable) {
					this->update(i);
				}
			}
		}

		template<typename T, typename V> requires (qsf::has_update<T>() || (qpl::is_container<T>() && qsf::has_update<qpl::container_deepest_subtype<T>>()))
		void update(T& updatable, const qsf::view_rectangle_t<V>& view) {
			if (!view.enabled) {
				this->update(updatable);
				return;
			}
			
			auto before = this->event().m_mouse_position;
		
			this->event().m_mouse_position = this->event().m_mouse_position * view.scale + view.position;
		
			if constexpr (qsf::has_update<T>()) {
				updatable.update(this->event());
			}
			else {
				for (auto& i : updatable) {
					this->update(i);
				}
			}
			this->event().m_mouse_position = before;
		}
		QPLDLL void create();
		QPLDLL bool is_open() const;
		QPLDLL void update_close_window();
		QPLDLL void hide_cursor();
		QPLDLL void show_cursor();
		QPLDLL void set_cursor_position(qpl::vector2i position);
		QPLDLL void set_window_position(qpl::vector2u position);
		QPLDLL qpl::vector2u get_window_position() const;

		QPLDLL qpl::vector2i dimension() const;
		QPLDLL qpl::vector2f center() const;

		QPLDLL void play_sound(const std::string& name, qpl::f32 volume = 100.0f, qpl::f32 speed = 1.0f);

		QPLDLL void add_font(const std::string& name, const std::string& path);
		QPLDLL void add_sound(const std::string& name, const std::string& path);
		QPLDLL void add_texture(const std::string& name, const std::string& path);
		QPLDLL void add_sprite(const std::string& name, const std::string& path);
		QPLDLL void add_sprite(const std::string& name, sf::Texture& texture);
		QPLDLL void add_shader(const std::string& name, const std::string& path, sf::Shader::Type shader_type);
		QPLDLL void add_shader(const std::string& name, const std::string& path);
		QPLDLL void add_text(const std::string& name);

		QPLDLL void add_font_from_memory(const std::string& name, const std::string& memory);
		QPLDLL void add_sound_from_memory(const std::string& name, const std::string& memory);
		QPLDLL void add_texture_from_memory(const std::string& name, const std::string& memory);
		QPLDLL void add_shader_from_memory(const std::string& name, const std::string& memory, sf::Shader::Type shader_type);

		QPLDLL sf::Font& get_font(const std::string& name);
		QPLDLL sf::SoundBuffer& get_sound(const std::string& name);
		QPLDLL sf::Texture& get_texture(const std::string& name);
		QPLDLL sf::Sprite& get_sprite(const std::string& name);
		QPLDLL sf::Shader& get_shader(const std::string& name);
		QPLDLL qsf::text& get_text(const std::string& name);

		QPLDLL const sf::Font& get_font(const std::string& name) const;
		QPLDLL const sf::SoundBuffer& get_sound(const std::string& name) const;
		QPLDLL const sf::Texture& get_texture(const std::string& name) const;
		QPLDLL const sf::Sprite& get_sprite(const std::string& name) const;
		QPLDLL const sf::Shader& get_shader(const std::string& name) const;
		QPLDLL const qsf::text& get_text(const std::string& name) const;

		QPLDLL void add_render(const std::string& name, bool smooth = true);
		QPLDLL qsf::render_texture& get_render(const std::string& name);
		QPLDLL const qsf::render_texture& get_render(const std::string& name) const;


		template<typename C> requires (std::is_base_of_v<qsf::base_state, C>)
		void add_state() {
			this->framework->add_state<C>();
		}
		QPLDLL void pop_this_state();
		QPLDLL void allow_exit();
		QPLDLL void disallow_exit();
		QPLDLL bool is_exit_allowed() const;
		QPLDLL void allow_clear();
		QPLDLL void disallow_clear();
		QPLDLL bool is_clear_allowed() const;
		QPLDLL void allow_display();
		QPLDLL void disallow_display();
		QPLDLL bool is_display_allowed() const;
		QPLDLL bool has_focus() const;
		QPLDLL bool has_gained_focus() const;
		QPLDLL bool has_lost_focus() const;

		QPLDLL qpl::size frame_ctr() const;
		QPLDLL qpl::time no_focus_time() const;
		QPLDLL qpl::time frame_time() const;
		QPLDLL qpl::f64 frame_time_f() const;
		QPLDLL qpl::time run_time() const;
		QPLDLL const qsf::event_info& event() const;
		QPLDLL qsf::event_info& event();

		qsf::framework* framework;

		sf::Color clear_color = sf::Color::Black;
		sf::RenderStates render_states = sf::RenderStates::Default;
		qpl::size m_frame_ctr = 0u;
		bool m_pop_this_state = false;
		bool m_allow_exit = true;
		bool m_allow_clear = true;
		bool m_allow_display = true;
	};
}

#endif
#endif