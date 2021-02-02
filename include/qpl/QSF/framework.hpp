#ifndef QPLSF_FRAMEWORK_HPP
#define QPLSF_FRAMEWORK_HPP
#pragma once
#if defined(QPL_USE_SFML) || defined(QPL_USE_ALL)

#include <qpl/qpldeclspec.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include <qpl/QSF/event_info.hpp>
#include <qpl/QSF/vector.hpp>
#include <qpl/QSF/color.hpp>
#include <qpl/QSF/drawables.hpp>
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
			this->set_title("");
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

		QPLDLL void add_font(const std::string& name, const std::string& path);
		QPLDLL void add_texture(const std::string& name, const std::string& path);
		QPLDLL void add_sprite(const std::string& name, const std::string& path);
		QPLDLL void add_text(const std::string& name);

		QPLDLL sf::Font& get_font(const std::string& name);
		QPLDLL sf::Texture& get_texture(const std::string& name);
		QPLDLL sf::Sprite& get_sprite(const std::string& name);
		QPLDLL qsf::text& get_text(const std::string& name);

		QPLDLL const sf::Font& get_font(const std::string& name) const;
		QPLDLL const sf::Texture& get_texture(const std::string& name) const;
		QPLDLL const sf::Sprite& get_sprite(const std::string& name) const;
		QPLDLL const qsf::text& get_text(const std::string& name) const;

		QPLDLL void create();
		QPLDLL bool is_created() const;
		QPLDLL bool is_open() const;
		QPLDLL void set_info(const std::string& title, qsf::vector2u dimension, qpl::u32 style);
		QPLDLL void set_title(const std::string& title);
		QPLDLL void set_dimension(qsf::vector2u dimension);
		QPLDLL void set_style(qpl::u32 style);
		QPLDLL void hide_cursor();
		QPLDLL void set_window_position(qsf::vector2u position);
		QPLDLL qsf::vector2u get_window_position() const;
		QPLDLL void show_cursor();
		QPLDLL void set_cursor_position(qsf::vector2i position);
		QPLDLL void draw_call();
		QPLDLL void display();
		QPLDLL bool game_loop_basic_segment();
		QPLDLL bool game_loop_segment();
		QPLDLL void game_loop();


		template<typename T>
		QPLDLL void draw_graph(const std::vector<T>& data, const std::string name = "") {
			qsf::drawing_graph[name].set_data(data);
			this->window.draw(qsf::drawing_graph);
		}
		template<typename T>
		QPLDLL void draw_graph(const std::vector<T>& data, qsf::rgb color, qpl::f64 thickness, const std::string& name = "") {
			qsf::drawing_graph[name].color = color;
			qsf::drawing_graph[name].thickness = thickness;
			this->draw_graph(data);
		}
		QPLDLL void set_graph_axis_font(const std::string& font_name);
		QPLDLL void set_graph_color(qsf::rgb color, const std::string& name);
		QPLDLL void set_graph_thickness(qpl::f64 thickness, const std::string& name);
		QPLDLL void set_graph_interpolation_steps(qpl::size interpolation_steps, const std::string& name);
		QPLDLL void set_graph_color(qsf::rgb color);
		QPLDLL void set_graph_thickness(qpl::f64 thickness);
		QPLDLL void set_graph_interpolation_steps(qpl::size interpolation_steps);
		QPLDLL void set_graph_dimension(qsf::vector2f dimension);
		QPLDLL void set_graph_position(qsf::vector2f position);
		QPLDLL qpl::time run_time() const;
		QPLDLL qpl::time frame_time() const;


		std::vector<std::unique_ptr<qsf::base_state>> states;
		sf::RenderWindow window;
		std::string m_title;
		qsf::vector2u m_dimension;
		qpl::u32 m_style;
		qpl::clock m_run_time_clock;
		qpl::clock m_frametime_clock;
		qpl::time m_frametime;
		bool m_created;
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
		QPLDLL bool game_loop_basic_segment();
		QPLDLL bool game_loop_segment();
		template<typename T>
		QPLDLL void draw(const T& drawable, sf::RenderStates states = sf::RenderStates::Default) {
			if constexpr (std::is_base_of<sf::Drawable, T>()) {
				this->framework->window.draw(drawable, states);
			}
			else {
				drawable.draw(this->framework->window, states);
			}
		}
		template<typename T>
		QPLDLL void update(T& updatable) {
			updatable.update(this->event);
		}
		QPLDLL void create();
		QPLDLL bool is_open() const;
		QPLDLL void event_update();
		QPLDLL void update_close_window();
		QPLDLL void hide_cursor();
		QPLDLL void show_cursor();
		QPLDLL void set_cursor_position(qsf::vector2i position);
		QPLDLL void set_window_position(qsf::vector2u position);
		QPLDLL qsf::vector2u get_window_position() const;

		QPLDLL qsf::vector2i dimension() const;

		QPLDLL void add_font(const std::string& name, const std::string& path);
		QPLDLL void add_texture(const std::string& name, const std::string& path);
		QPLDLL void add_sprite(const std::string& name, const std::string& path);
		QPLDLL void add_text(const std::string& name);

		QPLDLL sf::Font& get_font(const std::string& name);
		QPLDLL sf::Texture& get_texture(const std::string& name);
		QPLDLL sf::Sprite& get_sprite(const std::string& name);
		QPLDLL qsf::text& get_text(const std::string& name);

		QPLDLL const sf::Font& get_font(const std::string& name) const;
		QPLDLL const sf::Texture& get_texture(const std::string& name) const;
		QPLDLL const sf::Sprite& get_sprite(const std::string& name) const;
		QPLDLL const qsf::text& get_text(const std::string& name) const;


		template<typename C>
		void add_state() {
			this->framework->add_state<C>();
		}
		template<typename T>
		QPLDLL void draw_graph(const std::vector<T>& data, const std::string& name = "") {
			this->framework->draw_graph(data, name);
		}
		template<typename T>
		QPLDLL void draw_graph(const std::vector<T>& data, qsf::rgb color, qpl::f64 thickness, const std::string& name = "") {
			this->framework->draw_graph(data, color, thickness, name);
		}
		QPLDLL void set_graph_color(qsf::rgb color);
		QPLDLL void set_graph_thickness(qpl::f64 thickness);
		QPLDLL void set_graph_interpolation_steps(qpl::size interpolation_steps);
		QPLDLL void set_graph_color(qsf::rgb color, const std::string& name);
		QPLDLL void set_graph_thickness(qpl::f64 thickness, const std::string& name);
		QPLDLL void set_graph_interpolation_steps(qpl::size interpolation_steps, const std::string& name);
		QPLDLL void set_graph_dimension(qsf::vector2f dimension);
		QPLDLL void set_graph_position(qsf::vector2f position);
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
		QPLDLL qpl::time frame_time() const;
		QPLDLL qpl::time run_time() const;

		qsf::framework* framework;
		qsf::event_info event;

		sf::Color clear_color = sf::Color::Black;
		bool m_pop_this_state = false;
		bool m_allow_exit = true;
		bool m_allow_clear = true;
		bool m_allow_display = true;
	};

}

#endif
#endif