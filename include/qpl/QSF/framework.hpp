#ifndef QPLSF_FRAMEWORK_HPP
#define QPLSF_FRAMEWORK_HPP
#pragma once

#if !defined (QPL_NO_SFML) || defined(QPL_USE_ALL)

#include <qpl/qpldeclspec.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>

#include <qpl/QSF/event_info.hpp>
#include <qpl/QSF/color.hpp>
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
	framework.gameloop();
	*/
	struct framework {
		framework() {
			this->set_title(" ");
			this->set_dimension({ 1280, 720 });
			this->set_style(sf::Style::Default);
			this->m_created = false;
		}
		

		template<typename C>
		void add_state() {
			this->states.push_back(std::make_unique<C>());
			this->states.back()->framework = this;
			this->states.back()->init();
		}
		template<typename C>
		void add_state(C& state) {
			this->states.push_back(std::make_unique<C>(state));
			this->states.back()->framework = this;
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
		QPLDLL void set_antialising(qpl::u32 antialising);
		QPLDLL qpl::u32 get_antialising() const;
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
		QPLDLL bool game_loop_event_update_draw();
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
		sf::RenderWindow window;
		std::unordered_map<std::string, qsf::render_texture> m_render_textures;
		sf::ContextSettings m_settings;
		std::string m_title;
		qpl::vector2u m_dimension;
		qpl::u32 m_style = sf::Style::Default;
		qpl::clock m_run_time_clock;
		qpl::clock m_frametime_clock;
		qpl::time m_frametime;
		qpl::u32 m_antialising = 12u;
		qpl::u32 m_framerate_limit = 144u;
		bool m_created = false;
		bool m_update_if_no_focus = false;

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
		virtual void init() = 0;
		virtual void updating() = 0;
		virtual void drawing() = 0;
		
		QPLDLL virtual void clear();
		QPLDLL virtual void call_on_resize();
		QPLDLL virtual void call_on_close();
		QPLDLL virtual void call_after_window_create();

		QPLDLL void draw_call();
		QPLDLL void display();
		QPLDLL bool game_loop_segment();
		QPLDLL bool game_loop_event_update_draw();

		QPLDLL void set_shader(const std::string& name);
		QPLDLL void set_shader(sf::Shader& shader);
		QPLDLL void unbind_shader();

		template<typename T> requires qsf::has_any_draw_c<T>
		void draw(const T& drawable, sf::RenderStates states) {
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
		template<typename T> requires qsf::has_any_draw_c<T>
		void draw(const T& drawable) {
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

		template<typename T, typename V> requires qsf::has_any_draw_c<T>
		void draw(const T& drawable, qsf::view_rectangle<V> view) {
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

		template<typename T> requires qsf::has_any_draw_c<T>
		void draw_into(const std::string& name, const T& drawable) {
			this->get_render(name).draw(drawable, this->render_states);
		}
		template<typename T> requires qsf::has_any_draw_c<T>
		void draw_into(const std::string& name, const T& drawable, sf::RenderStates states) {
			this->get_render(name).draw(drawable, states);
		}
		template<typename T, typename V> requires qsf::has_any_draw_c<T>
		void draw_into(const std::string& name, const T& drawable, qsf::view_rectangle<V> view) {
			sf::RenderStates states = view.get_render_states();
			this->get_render(name).draw(drawable, states);
		}


		template<typename T> requires qsf::has_any_draw_c<T>
		void draw_with_shader(const T& drawable, const std::string& name) {
			sf::RenderStates states = this->render_states;
			states.shader = &qsf::get_shader(name);
			this->draw(drawable, states);
		}
		template<typename T> requires qsf::has_any_draw_c<T>
		void draw_with_shader(const T& drawable, sf::Shader& shader) {
			sf::RenderStates states = this->render_states;
			states.shader = &shader;
			this->draw(drawable, states);
		}
		template<typename T> requires qsf::has_any_draw_c<T>
		void draw_with_shader_into(const std::string& render_name, const T& drawable, const std::string& shader_name) {
			sf::RenderStates states = this->render_states;
			states.shader = &qsf::get_shader(shader_name);
			this->get_render(render_name).draw(drawable, states);
		}
		template<typename T> requires qsf::has_any_draw_c<T>
		void draw_with_shader_into(const std::string& render_name, const T& drawable, sf::Shader& shader) {
			sf::RenderStates states = this->render_states;
			states.shader = &shader;
			this->get_render(render_name).draw(drawable, states);
		}

		template<typename T> requires qsf::has_update_c<T>
		void update(T& updatable) {
			updatable.update(this->event);
		}
		QPLDLL void create();
		QPLDLL bool is_open() const;
		QPLDLL void event_update();
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


		template<typename C>
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

		QPLDLL qpl::time no_focus_time() const;
		QPLDLL qpl::time frame_time() const;
		QPLDLL qpl::time run_time() const;

		qsf::framework* framework;
		qsf::event_info event;

		sf::Color clear_color = sf::Color::Black;
		sf::RenderStates render_states;
		bool m_pop_this_state = false;
		bool m_allow_exit = true;
		bool m_allow_clear = true;
		bool m_allow_display = true;
	};
}

#endif
#endif