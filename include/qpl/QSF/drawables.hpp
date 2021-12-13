#ifndef QPLSF_DRAWABLES_HPP
#define QPLSF_DRAWABLES_HPP
#pragma once

#if !defined (QPL_NO_SFML) || defined(QPL_USE_ALL)
#include <qpl/qpldeclspec.hpp>
#include <qpl/QSF/color.hpp>
#include <qpl/QSF/event_info.hpp>
#include <qpl/string.hpp>
#include <qpl/vector.hpp>
#include <qpl/type_traits.hpp>

#include <functional>
#include <string>
#include <unordered_map>
#include <type_traits>
#include <SFML/Graphics.hpp>

namespace qsf {
	struct draw_object;
	struct render_texture;
	
	template<typename C>
	concept is_render_texture_c = qpl::is_same_decayed<C, qsf::render_texture>();
	
	template<typename C>
	constexpr bool is_render_texture() {
		return is_render_texture_c<C>;
	}
	template<typename C>
	concept has_draw_object_c = requires (const C x, draw_object & object) {
		x.draw(object);
	};
	template<typename C>
	constexpr bool has_draw_object() {
		return has_draw_object_c<C>;
	}
	template<typename C>
	concept has_draw_sf_c = requires (const C x, sf::RenderTarget & render, sf::RenderStates states) {
		x.draw(render, states);
	};
	template<typename C>
	constexpr bool has_draw_sf() {
		return has_draw_sf_c<C>;
	}

	template<typename C>
	concept has_any_draw_c = is_render_texture_c<C> || has_draw_sf_c<C> || has_draw_object_c<C> || std::is_base_of_v<sf::Drawable, C>;

	template<typename C>
	constexpr bool has_any_draw() {
		return has_any_draw_c<C>;
	}

	struct draw_object {
		draw_object(sf::RenderWindow& window, sf::RenderStates states = sf::RenderStates::Default) {
			this->window = &window;
			this->texture = nullptr;
			this->states = states;
		}
		draw_object(sf::RenderTexture& texture, sf::RenderStates states = sf::RenderStates::Default) {
			this->texture = &texture;
			this->window = nullptr;
			this->states = states;
		}

		template<typename T> requires (qsf::has_any_draw<T>() || qpl::is_container<T>())
		void draw(const T& object) {
			if constexpr (qsf::has_any_draw<T>()) {
				if constexpr (qsf::is_render_texture<T>()) {
					if (this->window) {
						this->window->draw(object.get_sprite(), this->states);
					}
					else if (this->texture) {
						this->texture->draw(object.get_sprite(), this->states);
					}
				}
				else if constexpr (std::is_base_of<sf::Drawable, T>()) {
					if (this->window) {
						this->window->draw(object, this->states);
					}
					else if (this->texture) {
						this->texture->draw(object, this->states);
					}
				}
				else if constexpr (qsf::has_draw_object<T>()) {
					object.draw(*this);
				}
				else if constexpr (qsf::has_draw_sf<T>()) {
					if (this->window) {
						object.draw(*this->window, this->states);
					}
					else if (this->texture) {
						object.draw(*this->texture, this->states);
					}
				}
			}
			else {
				for (auto& i : object) {
					this->draw(i);
				}
			}
		}
		template<typename T> requires (qsf::has_any_draw<T>() || qpl::is_container<T>())
		void draw(const T& object, sf::RenderStates states) {
			if constexpr (qsf::has_any_draw<T>()) {
				if constexpr (qsf::is_render_texture<T>()) {
					if (this->window) {
						this->window->draw(object.get_sprite(), states);
					}
					else if (this->texture) {
						this->texture->draw(object.get_sprite(), states);
					}
				}
				else if constexpr (std::is_base_of<sf::Drawable, T>()) {
					if (this->window) {
						this->window->draw(object, states);
					}
					else if (this->texture) {
						this->texture->draw(object, states);
					}
				}
				else if constexpr (qsf::has_draw_object<T>()) {
					auto copy = this->states;
					this->states = states;
					object.draw(*this);
					this->states = copy;
				}
				else if constexpr (qsf::has_draw_sf<T>()) {
					if (this->window) {
						object.draw(*this->window, states);
					}
					else if (this->texture) {
						object.draw(*this->texture, states);
					}
				}
			}
			else {
				for (auto& i : object) {
					this->draw(i, states);
				}
			}
		}
		sf::RenderWindow* window;
		sf::RenderTexture* texture;
		sf::RenderStates states;
	};

	

	struct vtext;
	struct text;
	struct vrectangle;
	struct rectangle;
	struct vrectangles;
	struct rectangles;
	struct vsmooth_rectangle;
	struct smooth_rectangle;
	struct vpoint;
	struct point;
	struct vpoints;
	struct points;
	struct vcircle;
	struct circle;
	struct vcircles;
	struct circles;
	struct vline;
	struct line;
	struct vlines;
	struct lines;
	struct vthick_line;
	struct thick_line;
	struct vthick_lines;
	struct thick_lines;
	struct vgraph;
	struct graph;
	struct vbutton;
	struct button;

	struct event_info;



	QPLDLL qpl::hitbox get_text_hitbox(const sf::Text& text, bool ignore_outline = true);
	QPLDLL qpl::hitbox get_text_hitbox(const qsf::text& text, bool ignore_outline = true);
	QPLDLL void centerize_text(sf::Text& text);
	QPLDLL void centerize_text(qsf::text& text);

	QPLDLL qpl::hitbox get_sprite_hitbox(const sf::Sprite& sprite);
	QPLDLL qpl::vector2f get_sprite_center(const sf::Sprite& sprite);
	QPLDLL void centerize_sprite_origin(sf::Sprite& sprite);
	QPLDLL void centerize_sprite(sf::Sprite& sprite);

	namespace detail {
		QPLDLL extern qsf::text text;
		QPLDLL extern qsf::rectangle rectangle;
		QPLDLL extern qsf::rectangles rectangles;
		QPLDLL extern qsf::smooth_rectangle smooth_rectangle;
		QPLDLL extern qsf::point point;
		QPLDLL extern qsf::points points;
		QPLDLL extern qsf::circle circle;
		QPLDLL extern qsf::circles circles;
		QPLDLL extern qsf::line line;
		QPLDLL extern qsf::lines lines;
		QPLDLL extern qsf::thick_line thick_line;
		QPLDLL extern qsf::thick_lines thick_lines;
		QPLDLL extern qsf::graph graph;
		QPLDLL extern qsf::button button;
	}
	QPLDLL extern qsf::vgraph drawing_graph;

	struct vertex {
		vertex() {

		}
		vertex(qpl::vector2f position, qsf::rgb color) {
			this->position = position;
			this->color = color;
		}

		qpl::vector2f position;
		qsf::rgb color;
		qpl::vector2f tex_coords;

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
	};

	enum class primitive_type {
		points,        
		lines,         
		line_strip,    
		triangles,     
		triangle_strip,
		triangle_fan,  
		quads,         
	};

	struct vertex_array {

		QPLDLL void set_primitive_type(qsf::primitive_type primitive_type);

		QPLDLL qpl::size size() const;
		QPLDLL bool empty() const;
		QPLDLL void resize(qpl::size new_size);
		QPLDLL void reserve(qpl::size new_size);

		QPLDLL void add(const qsf::vertex& vertex);

		QPLDLL void clear();

		QPLDLL qsf::vertex& operator[](qpl::size index);
		QPLDLL const qsf::vertex& operator[](qpl::size index) const;

		QPLDLL qsf::vertex& front();
		QPLDLL const qsf::vertex& front() const;

		QPLDLL qsf::vertex& back();
		QPLDLL const qsf::vertex& back() const;

		QPLDLL std::vector<qsf::vertex>::iterator begin();
		QPLDLL std::vector<qsf::vertex>::const_iterator begin() const;
		QPLDLL std::vector<qsf::vertex>::const_iterator cbegin() const;

		QPLDLL std::vector<qsf::vertex>::iterator end();
		QPLDLL std::vector<qsf::vertex>::const_iterator end() const;
		QPLDLL std::vector<qsf::vertex>::const_iterator cend() const;

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;

		std::vector<qsf::vertex> vertices;
		qsf::primitive_type primitive_type = qsf::primitive_type::points;
	};

	struct vtext {
		QPLDLL void set_font(const std::string& font_name);
		QPLDLL void set_style(qpl::u32 style);
		QPLDLL void set_character_size(qpl::u32 character_size);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_outline_thickness(qpl::f32 outline_thickness);
		QPLDLL void set_outline_color(qsf::rgb color);
		QPLDLL void set_letter_spacing(qpl::f32 spacing);
		QPLDLL void set_position(qpl::vector2f position);
		QPLDLL void set_string(const std::string& string);
		QPLDLL bool operator==(const vtext& other) const;

		template<typename T, typename ...Args> requires (qpl::is_printable<T, Args...>())
			void set_string(T first, Args... values) {
			this->set_string(qpl::to_string(first, values...));
		}

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;

		std::string font_name;
		qpl::u32 style = sf::Text::Style::Regular;
		qpl::u32 character_size = 15u;
		qpl::f32 outline_thickness = 0.0f;
		qsf::rgb color = qsf::rgb::black;
		qsf::rgb outline_color;
		qsf::rgb multiplied_color = qsf::rgb::white;
		qpl::f32 letter_spacing = 1.0f;
		qpl::vector2f position;
		std::string string;
	};

	struct text {
		QPLDLL std::string get_font() const;
		QPLDLL qpl::u32 get_style() const;
		QPLDLL qpl::u32 get_character_size() const;
		QPLDLL qsf::rgb get_color() const;
		QPLDLL qpl::f32 get_outline_thickness() const;
		QPLDLL qsf::rgb get_outline_color() const;
		QPLDLL qpl::f32 get_letter_spacing() const;
		QPLDLL qpl::vector2f get_position() const;
		QPLDLL qpl::vector2f get_center() const;
		QPLDLL std::string get_string() const;
		QPLDLL void set_font(const sf::Font& font);
		QPLDLL void set_font(const std::string& font_name);
		QPLDLL void set_style(qpl::u32 style);
		QPLDLL void set_character_size(qpl::u32 character_size);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_outline_thickness(qpl::f32 outline_thickness);
		QPLDLL void set_outline_color(qsf::rgb color);
		QPLDLL void set_rotation(qpl::f32 angle);
		QPLDLL void set_letter_spacing(qpl::f32 spacing);
		QPLDLL void set_position(qpl::vector2f position);
		QPLDLL void set_center(qpl::vector2f position);
		QPLDLL void set_string(const std::string& string);
		QPLDLL void set_multiplied_color(qsf::rgb color);
		QPLDLL qsf::rgb get_multiplied_color() const;

		template<typename T, typename ...Args> requires (qpl::is_printable<T, Args...>())
		void set_string(T first, Args... values) {
			this->set_string(qpl::to_string(first, values...));
		}

		QPLDLL void centerize();
		QPLDLL void centerize_x();
		QPLDLL void centerize_y();
		QPLDLL void move(qpl::vector2f delta);

		qsf::text() {

		}
		qsf::text(const qsf::vtext& other) {
			*this = other;
		}

		QPLDLL qpl::hitbox get_visible_hitbox(bool ignore_outline = true) const;
		QPLDLL qpl::hitbox get_standard_hitbox() const;
		QPLDLL qpl::vector2f offset() const;

		
		QPLDLL std::string string() const;
		QPLDLL void clear();
		QPLDLL qsf::text& operator<<(const std::string& string);

		QPLDLL qsf::text& operator=(const qsf::vtext& text);

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;

		QPLDLL void draw_if_visible(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;


		std::string m_font;
		sf::Text m_text;
		std::string m_string;
		qsf::rgb color = qsf::rgb::white;
		qsf::rgb outline_color = qsf::rgb::white;
		qsf::rgb multiplied_color = qsf::rgb::white;
	};

	struct endl_type {

	};

	QPLDLL extern endl_type endl;

	struct vrectangle {
		vrectangle() {

		}
		vrectangle(qpl::vector2f position, qpl::vector2f dimension) {
			this->position = position;
			this->dimension = dimension;
		}
		vrectangle(qpl::vector2f position, qpl::vector2f dimension, qsf::rgb color) {
			this->position = position;
			this->dimension = dimension;
			this->color = color;
		}
		vrectangle(qpl::hitbox hitbox) {
			this->set_position(hitbox.position);
			this->set_dimension(hitbox.dimension);
		}

		QPLDLL void set_dimension(qpl::vector2f dimension);
		QPLDLL void set_position(qpl::vector2f position);
		QPLDLL void set_center(qpl::vector2f position);
		QPLDLL void set_hitbox(const qpl::hitbox& hitbox);
		QPLDLL void set_outline_thickness(qpl::f32 outline_thickness);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_outline_color(qsf::rgb outline_color);
		QPLDLL void set_multiplied_color(qsf::rgb color);

		QPLDLL qpl::vector2f get_dimension() const;
		QPLDLL qpl::vector2f get_position() const;
		QPLDLL qpl::vector2f get_center() const;
		QPLDLL qpl::hitbox get_hitbox() const;
		QPLDLL qsf::rgb get_color() const;
		QPLDLL qpl::f32 get_outline_thickness() const;
		QPLDLL qsf::rgb get_outline_color() const;
		QPLDLL qsf::rgb get_multiplied_color() const;

		QPLDLL std::string string() const;

		QPLDLL void increase(qpl::f32 delta);
		QPLDLL qsf::vrectangle increased(qpl::f32 delta) const;

		QPLDLL bool contains(qpl::vector2f position) const;
		QPLDLL bool contains(qpl::vector2f position, qpl::f32 hitbox_increase) const;
		QPLDLL bool collides(qpl::straight_line line) const;
		QPLDLL bool collides(qpl::straight_line line, qpl::f32 hitbox_increase) const;


		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;

		qpl::vector2f dimension;
		qpl::vector2f position;
		qpl::f32 outline_thickness = 0.0f;
		qsf::rgb color;
		qsf::rgb outline_color;
		qsf::rgb multiplied_color = qsf::rgb::white;
	};

	struct rectangle {
		QPLDLL void set_dimension(qpl::vector2f dimension);
		QPLDLL void set_position(qpl::vector2f position);
		QPLDLL void set_center(qpl::vector2f position);
		QPLDLL void set_hitbox(const qpl::hitbox& hitbox);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_outline_thickness(qpl::f32 outline_thickness);
		QPLDLL void set_outline_color(qsf::rgb outline_color);
		QPLDLL void set_multiplied_color(qsf::rgb color);

		QPLDLL void increase(qpl::f64 delta);

		QPLDLL bool contains(qpl::vector2f position) const;
		QPLDLL bool contains(qpl::vector2f position, qpl::f32 hitbox_increase) const;
		QPLDLL bool contains(qpl::vector2f position, qpl::vector2f hitbox_increase) const;
		QPLDLL bool collides(qpl::straight_line line) const;
		QPLDLL bool collides(qpl::straight_line line, qpl::f32 hitbox_increase) const;

		QPLDLL qsf::rgb get_multiplied_color() const;
		QPLDLL qsf::rgb get_color() const;
		QPLDLL qsf::rgb get_outline_color() const;
		QPLDLL qpl::vector2f get_position() const;
		QPLDLL qpl::vector2f get_dimension() const;
		QPLDLL qpl::vector2f get_center() const;
		QPLDLL qpl::f32 get_outline_thickness() const;
		QPLDLL qpl::hitbox get_hitbox() const;

		QPLDLL qsf::vlines as_lines() const;
		QPLDLL qsf::vlines as_lines_completed() const;

		QPLDLL qpl::vector2f center() const;
		QPLDLL void move(qpl::vector2f delta);

		QPLDLL qsf::rectangle& operator=(const qsf::vrectangle& rectangle);

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;

		sf::RectangleShape m_rect;
		qsf::rgb color;
		qsf::rgb outline_color;
		qsf::rgb multiplied_color = qsf::rgb::white;
	};

	struct vrectangles {
		QPLDLL qpl::size size() const;
		QPLDLL void resize(qpl::size new_size);
		QPLDLL void reserve(qpl::size new_size);
		QPLDLL void clear();
		QPLDLL void add(const qsf::vrectangle& rect);

		QPLDLL qsf::vrectangle& operator[](qpl::size index);
		QPLDLL const qsf::vrectangle& operator[](qpl::size index) const;

		QPLDLL qsf::vrectangle& front();
		QPLDLL const qsf::vrectangle& front() const;

		QPLDLL qsf::vrectangle& back();
		QPLDLL const qsf::vrectangle& back() const;

		QPLDLL std::vector<qsf::vrectangle>::iterator begin();
		QPLDLL std::vector<qsf::vrectangle>::const_iterator begin() const;
		QPLDLL std::vector<qsf::vrectangle>::const_iterator cbegin() const;

		QPLDLL std::vector<qsf::vrectangle>::iterator end();
		QPLDLL std::vector<qsf::vrectangle>::const_iterator end() const;
		QPLDLL std::vector<qsf::vrectangle>::const_iterator cend() const;

		QPLDLL void add_rectangle(vrectangle rectangle);
		QPLDLL void add_rectangle(qpl::vector2f position, qpl::vector2f dimension);
		QPLDLL void add_rectangle(qpl::vector2f position, qpl::vector2f dimension, qsf::rgb color);

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
		QPLDLL qsf::vrectangles& operator=(const qsf::vrectangles& rectangles);

		std::vector<qsf::vrectangle> rectangles;
	};

	struct rectangles {

		QPLDLL qpl::size size() const;
		QPLDLL void resize(qpl::size new_size);
		QPLDLL void reserve(qpl::size new_size);
		QPLDLL void clear();
		QPLDLL void add(const qsf::rectangle& rect);

		QPLDLL qsf::rectangle& operator[](qpl::size index);
		QPLDLL const qsf::rectangle& operator[](qpl::size index) const;

		QPLDLL qsf::rectangle& front();
		QPLDLL const qsf::rectangle& front() const;

		QPLDLL qsf::rectangle& back();
		QPLDLL const qsf::rectangle& back() const;

		QPLDLL std::vector<qsf::rectangle>::iterator begin();
		QPLDLL std::vector<qsf::rectangle>::const_iterator begin() const;
		QPLDLL std::vector<qsf::rectangle>::const_iterator cbegin() const;

		QPLDLL std::vector<qsf::rectangle>::iterator end();
		QPLDLL std::vector<qsf::rectangle>::const_iterator end() const;
		QPLDLL std::vector<qsf::rectangle>::const_iterator cend() const;

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
		QPLDLL qsf::rectangles& operator=(const qsf::vrectangles& rectangles);

		std::vector<qsf::rectangle> rectangles_;
	};

	struct polygon {
		struct polygon_proxy {
			sf::ConvexShape* ptr;
			qpl::size index;

			QPLDLL sf::Vector2f operator=(qpl::vector2f position);
			QPLDLL operator qpl::vector2f() const;
		};
		struct const_polygon_proxy {
			const sf::ConvexShape* ptr;
			qpl::size index;

			QPLDLL operator qpl::vector2f() const;
		};

		sf::ConvexShape shape;



		QPLDLL polygon_proxy operator[](qpl::size index);
		QPLDLL const_polygon_proxy operator[](qpl::size index) const;
		QPLDLL polygon_proxy front();
		QPLDLL const_polygon_proxy front() const;
		QPLDLL polygon_proxy back();
		QPLDLL const_polygon_proxy back() const;
		QPLDLL qpl::size size() const;
		
		QPLDLL void set_point(qpl::size index, qpl::vector2f position);
		QPLDLL qpl::vector2f get_point(qpl::size index) const;
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void resize(qpl::size size);
		QPLDLL void add(qpl::vector2f point);

		QPLDLL bool contains(qpl::vector2f point, qpl::size increment = 1u) const;

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
	};

	struct vsmooth_rectangle {

		QPLDLL void move(qpl::vector2f delta);

		QPLDLL void set_dimension(qpl::vector2f dimension);
		QPLDLL void set_position(qpl::vector2f position);
		QPLDLL void set_center(qpl::vector2f position);
		QPLDLL void set_slope(qpl::f64 slope);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_slope_dimension(qpl::vector2f dimension);
		QPLDLL void set_slope_point_count(qpl::size point_count);

		QPLDLL qpl::vector2f get_dimension() const;
		QPLDLL qpl::vector2f get_position() const;
		QPLDLL qpl::vector2f get_center() const;
		QPLDLL qpl::f64 get_slope() const;
		QPLDLL qsf::rgb get_color() const;
		QPLDLL qpl::vector2f get_slope_dimension() const;
		QPLDLL qpl::size get_slope_point_count() const;
		QPLDLL bool contains(qpl::vector2f point) const;
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
		
		friend smooth_rectangle;
	private:
		qpl::vector2f dimension;
		qpl::vector2f position;

		qpl::f64 slope = 2.0;
		qpl::vector2f slope_dim = { 10, 10 };
		qsf::rgb color;
		qpl::size slope_point_count = 20u;
		mutable bool geometry_changed = true;
		mutable bool color_changed = false;
	};
	struct smooth_rectangle {
		qsf::polygon polygon;

		QPLDLL bool contains(qpl::vector2f point) const;
		QPLDLL qsf::smooth_rectangle& operator=(const qsf::vsmooth_rectangle& smooth_rectangle);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
	};

	struct vpoint {
		qpl::vector2f position;
		qsf::rgb color;

		vpoint() {

		}

		vpoint(qpl::vector2f position, qsf::rgb color) {
			this->position = position;
			this->color = color;
		}
		vpoint(qpl::vector2f position) {
			this->position = position;
		}

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
		QPLDLL vpoint& operator=(qpl::vector2f position);

		QPLDLL bool operator==(const vpoint& other) const;
		QPLDLL bool operator!=(const vpoint& other) const;
		//QPLDLL vpoint& with_color(qsf::rgb color);
	};
	struct point {
		qsf::vertex vertex;

		QPLDLL qsf::point& operator=(const qsf::vpoint& point);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
	};

	struct vpoints {
		vpoints(qpl::size size = qpl::size{}) {
			this->points.resize(size);
		}


		QPLDLL qpl::size size() const;
		QPLDLL void resize(qpl::size new_size);
		QPLDLL void reserve(qpl::size new_size);

		QPLDLL qsf::vpoint& operator[](qpl::size index);
		QPLDLL const qsf::vpoint& operator[](qpl::size index) const;

		QPLDLL qsf::vpoint& front();
		QPLDLL const qsf::vpoint& front() const;

		QPLDLL qsf::vpoint& back();
		QPLDLL const qsf::vpoint& back() const;


		QPLDLL std::vector<qsf::vpoint>::iterator begin();
		QPLDLL std::vector<qsf::vpoint>::const_iterator begin() const;
		QPLDLL std::vector<qsf::vpoint>::const_iterator cbegin() const;

		QPLDLL std::vector<qsf::vpoint>::iterator end();
		QPLDLL std::vector<qsf::vpoint>::const_iterator end() const;
		QPLDLL std::vector<qsf::vpoint>::const_iterator cend() const;


		QPLDLL void add_point(qsf::vpoint point);
		QPLDLL void add_point(qpl::vector2f position, qsf::rgb color);
		QPLDLL qsf::vlines as_lines() const;

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
		QPLDLL void clear();

		std::vector<qsf::vpoint> points;
	};

	struct points {
		points() {
			this->vertices.set_primitive_type(qsf::primitive_type::points);
		}
		points(qpl::size size) {
			this->vertices.resize(size);
		}


		QPLDLL void resize(qpl::size new_size);
		QPLDLL void reserve(qpl::size new_size);

		QPLDLL qsf::vertex& operator[](qpl::size index);
		QPLDLL const qsf::vertex& operator[](qpl::size index) const;

		QPLDLL qsf::vertex& front();
		QPLDLL const qsf::vertex& front() const;

		QPLDLL qsf::vertex& back();
		QPLDLL const qsf::vertex& back() const;


		QPLDLL qsf::vlines as_lines() const;
		QPLDLL qsf::points& operator=(const qsf::vpoints& points);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
		QPLDLL void clear();
		QPLDLL qpl::size size() const;
		qsf::vertex_array vertices;
	};



	struct vcircle {
		vcircle() {
			this->radius = 0.0f;
		}
		vcircle(qsf::vpoint point, qpl::f32 radius) {
			this->point = point;
			this->radius = radius;
			this->point.position.move(qpl::vector2f{ -this->radius, -this->radius });
		}
		vcircle(qpl::vector2f center, qpl::f32 radius, qsf::rgb color) {
			this->point.position = center - qpl::vector2f{ radius, radius };
			this->point.color = color;
			this->radius = radius;
		}

		QPLDLL void set_position(qpl::vector2f center);
		QPLDLL void set_radius(qpl::f32 radius);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_center(qpl::vector2f center);
		QPLDLL void set_outline_thickness(qpl::f32 outline_thickness);
		QPLDLL void set_outline_color(qsf::rgb outline_color);

		QPLDLL qpl::vector2f get_center() const;
		QPLDLL void centerize();
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;

		qsf::vpoint point;
		qpl::f32 radius;
		qpl::f32 outline_thickness = 0.0f;
		qsf::rgb outline_color = qsf::rgb::unset;
	};
	struct circle {
		circle() {

		}
		circle(const qsf::vcircle& circle) {
			*this = circle;
		}
		circle(const qsf::circle& circle) {
			*this = circle;
		}

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
		QPLDLL qsf::circle& operator=(const qsf::vcircle& circle);
		QPLDLL qsf::circle& operator=(const qsf::circle& circle);

		QPLDLL void set_radius(qpl::f32 radius);
		QPLDLL qpl::f32 get_radius() const;
		QPLDLL void set_position(qpl::vector2f position);
		QPLDLL void set_center(qpl::vector2f center);
		QPLDLL qpl::vector2f get_center() const;
		QPLDLL qpl::vector2f get_position() const;
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL qsf::rgb get_color() const;
		QPLDLL void set_outline_thickness(qpl::f32 outline_thickness);
		QPLDLL qpl::f32 get_outline_thickness() const;
		QPLDLL void set_outline_color(qsf::rgb outline_color);
		QPLDLL qsf::rgb get_outline_color() const;

		sf::CircleShape circle_shape;
	};

	struct vcircles {
		QPLDLL void resize(qpl::size new_size);
		QPLDLL void reserve(qpl::size new_size);

		QPLDLL qsf::vcircle& operator[](qpl::size index);
		QPLDLL const qsf::vcircle& operator[](qpl::size index) const;

		QPLDLL qsf::vcircle& front();
		QPLDLL const qsf::vcircle& front() const;

		QPLDLL qsf::vcircle& back();
		QPLDLL const qsf::vcircle& back() const;

		QPLDLL void add_circle(qsf::vcircle circle);
		QPLDLL void add_circle(qsf::vpoint point, qpl::f32 radius, qsf::rgb color);
		QPLDLL void add_circle(qpl::vector2f position, qpl::f32 radius, qsf::rgb color);

		QPLDLL qpl::size size() const;
		QPLDLL void clear();

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;

		std::vector<qsf::vcircle> circles;
	};

	struct circles {
		circles() {

		}
		circles(const qsf::vcircles& circles) {
			*this = circles;
		}
		circles(const qsf::circles& circles) {
			*this = circles;
		}

		QPLDLL void resize(qpl::size new_size);
		QPLDLL void reserve(qpl::size new_size);

		QPLDLL qsf::circle& operator[](qpl::size index);
		QPLDLL const qsf::circle& operator[](qpl::size index) const;

		QPLDLL qsf::circle& front();
		QPLDLL const qsf::circle& front() const;

		QPLDLL qsf::circle& back();
		QPLDLL const qsf::circle& back() const;

		QPLDLL void add_circle(const qsf::vcircle& circle);
		QPLDLL void add_circle(qsf::vpoint point, qpl::f32 radius, qsf::rgb color);
		QPLDLL void add_circle(qpl::vector2f position, qpl::f32 radius, qsf::rgb color);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
		QPLDLL qsf::circles& operator=(const qsf::vcircles& circles);
		QPLDLL qsf::circles& operator=(const qsf::circles& circles);

		QPLDLL qpl::size size() const;
		QPLDLL void clear();

		std::vector<qsf::circle> circles_;
	};

	struct vline {


		QPLDLL void set_a(qsf::vpoint point);
		QPLDLL void set_a(qpl::vector2f position);
		QPLDLL void set_b(qsf::vpoint point);
		QPLDLL void set_b(qpl::vector2f position);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_a_color(qsf::rgb color);
		QPLDLL void set_b_color(qsf::rgb color);

		QPLDLL qpl::vector2f normal() const;
		QPLDLL qpl::f32 length() const;
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;

		QPLDLL qsf::vline& rotate_around_a(qpl::f64 degree);
		QPLDLL qsf::vline& rotate_around_b(qpl::f64 degree);

		QPLDLL operator qpl::straight_line() const;

		vpoint a;
		vpoint b;
	};

	struct line {
		QPLDLL void set_a(qsf::vpoint point);
		QPLDLL void set_a(qpl::vector2f position);
		QPLDLL void set_b(qsf::vpoint point);
		QPLDLL void set_b(qpl::vector2f position);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_a_color(qsf::rgb color);
		QPLDLL void set_b_color(qsf::rgb color);
		QPLDLL vpoint get_a() const;
		QPLDLL vpoint get_b() const;

		QPLDLL qpl::vector2f normal() const;
		QPLDLL qpl::f32 length() const;

		QPLDLL operator qpl::straight_line() const;
		

		QPLDLL qsf::line& operator=(const qsf::vline& line);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
		std::array<sf::Vertex, 2> vertices;
	};

	struct vlines {

		QPLDLL void resize(qpl::size new_size);
		QPLDLL void reserve(qpl::size new_size);

		QPLDLL qsf::vpoint& operator[](qpl::size index);
		QPLDLL const qsf::vpoint& operator[](qpl::size index) const;

		QPLDLL qsf::vpoint& front();
		QPLDLL const qsf::vpoint& front() const;

		QPLDLL qsf::vpoint& back();
		QPLDLL const qsf::vpoint& back() const;


		QPLDLL void clear();
		QPLDLL qpl::size size() const;
		QPLDLL void add_point(qsf::vpoint point);
		QPLDLL void add_point(qpl::vector2f position, qsf::rgb color = qsf::rgb::white);
		QPLDLL void complete();

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;

		std::vector<qsf::vpoint> points;
	};

	struct lines {
		lines() {
			this->vertices.set_primitive_type(qsf::primitive_type::line_strip);
		}
		lines(const qsf::vlines& lines) {
			this->vertices.set_primitive_type(qsf::primitive_type::line_strip);
			*this = lines;
		}

		QPLDLL void resize(qpl::size new_size);
		QPLDLL void reserve(qpl::size new_size);

		QPLDLL qsf::vertex& operator[](qpl::size index);
		QPLDLL const qsf::vertex& operator[](qpl::size index) const;

		QPLDLL qsf::vertex& front();
		QPLDLL const qsf::vertex& front() const;

		QPLDLL qsf::vertex& back();
		QPLDLL const qsf::vertex& back() const;


		QPLDLL void complete();

		QPLDLL qsf::lines& operator=(const qsf::vlines& lines);
		QPLDLL void clear();
		QPLDLL qpl::size size() const;
		QPLDLL void add_point(qsf::vpoint point);
		QPLDLL void add_point(qpl::vector2f position, qsf::rgb color = qsf::rgb::white);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;

		qsf::vertex_array vertices;
	};

	struct vthick_line {

		QPLDLL void set_a(qsf::vpoint point);
		QPLDLL void set_a(qpl::vector2f position);
		QPLDLL void set_b(qsf::vpoint point);
		QPLDLL void set_b(qpl::vector2f position);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_a_color(qsf::rgb color);
		QPLDLL void set_b_color(qsf::rgb color);

		QPLDLL qsf::vpoints as_points() const;

		QPLDLL qpl::vector2f normal() const;
		QPLDLL qpl::f32 length() const;
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;

		qsf::vpoint a;
		qsf::vpoint b;
		qpl::f32 thickness = 1.0f;
	};

	struct thick_line {
		thick_line() {

		}
		thick_line(const qsf::vthick_line& line) {
			*this = line;
		}

		QPLDLL void set_a(qsf::vpoint point);
		QPLDLL void set_a(qpl::vector2f position);
		QPLDLL void set_b(qsf::vpoint point);
		QPLDLL void set_b(qpl::vector2f position);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_a_color(qsf::rgb color);
		QPLDLL void set_b_color(qsf::rgb color);
		QPLDLL void set_thickness(qpl::f32 thickness);

		QPLDLL qsf::vpoints as_points() const;
		QPLDLL qpl::vector2f normal() const;
		QPLDLL qpl::f32 length() const;
		QPLDLL qsf::thick_line& operator=(const qsf::vthick_line& line);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;

		std::array<sf::Vertex, 4> vertices;
	};
	struct vthick_lines {

		QPLDLL void resize(qpl::size new_size);
		QPLDLL void reserve(qpl::size new_size);

		QPLDLL qsf::vpoint& operator[](qpl::size index);
		QPLDLL const qsf::vpoint& operator[](qpl::size index) const;

		QPLDLL qsf::vpoint& front();
		QPLDLL const qsf::vpoint& front() const;

		QPLDLL qsf::vpoint& back();
		QPLDLL const qsf::vpoint& back() const;


		QPLDLL void add_thick_line(qsf::vpoint point);
		QPLDLL void add_thick_line(qpl::vector2f position, qsf::rgb color);
		QPLDLL void set_thickness(qpl::f32 thickness);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
		QPLDLL void clear();
		QPLDLL qpl::size size() const;

		qpl::vector2f position;
		qsf::vpoints points;
		qpl::f32 thickness = 1.0f;
	};
	struct thick_lines {
		thick_lines() {
			this->vertices.set_primitive_type(qsf::primitive_type::quads);
		}

		QPLDLL void resize(qpl::size new_size);
		QPLDLL void reserve(qpl::size new_size);

		QPLDLL qsf::vertex& operator[](qpl::size index);
		QPLDLL const qsf::vertex& operator[](qpl::size index) const;

		QPLDLL qsf::vertex& front();
		QPLDLL const qsf::vertex& front() const;

		QPLDLL qsf::vertex& back();
		QPLDLL const qsf::vertex& back() const;


		QPLDLL void add_thick_line(qsf::vpoint point, qpl::f32 thickness);
		QPLDLL void add_thick_line(qpl::vector2f position, qsf::rgb color, qpl::f32 thickness);

		QPLDLL void complete();

		QPLDLL qsf::thick_lines& operator=(const qsf::vthick_lines& lines);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
		QPLDLL void clear();
		QPLDLL qpl::size size() const;
		QPLDLL bool empty() const;

		qsf::vertex_array vertices;
	};

	struct sprite {
		sf::Sprite m_sprite;
		qsf::rgb color = qsf::rgb::white;
		qsf::rgb multiplied_color = qsf::rgb::white;

		sprite() {

		}
		sprite(const qsf::sprite& sprite) {
			*this = sprite;
		}
		sprite(const sf::Sprite& sprite) {
			*this = sprite;
		}

		QPLDLL void set_texture(const sf::Texture& texture);
		QPLDLL void set_texture_rect(const sf::IntRect& rect);
		QPLDLL void set_texture_rect(qpl::hitbox hitbox);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_multiplied_color(qsf::rgb color);
		QPLDLL void set_position(qpl::vector2f position);
		QPLDLL void set_center(qpl::vector2f position);
		QPLDLL void set_scale(qpl::vector2f scale);
		QPLDLL void set_scale(qpl::f32 scale);
		QPLDLL void set_origin(qpl::vector2f origin);
		QPLDLL void set_rotation(qpl::f32 rotation);

		QPLDLL qsf::rgb get_color() const;
		QPLDLL qsf::rgb get_multiplied_color() const;
		QPLDLL qpl::vector2f get_position() const;
		QPLDLL qpl::vector2f get_scale() const;
		QPLDLL qpl::vector2f get_origin() const;
		QPLDLL qpl::f32 get_rotation() const;

		QPLDLL void move(qpl::vector2f delta);
		QPLDLL void move_scaled(qpl::vector2f delta);

		QPLDLL operator sf::Sprite& ();
		QPLDLL operator const sf::Sprite& () const;
		QPLDLL qsf::sprite& operator=(const sf::Sprite& sprite);

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
	};


	struct render_texture {

		QPLDLL void set_antialiasing(qpl::u32 antialiasing);
		QPLDLL void resize(qpl::vector2i dimension, bool resize_with_window = false);
		QPLDLL void set_smooth(bool smooth);
		QPLDLL void enable_smooth();
		QPLDLL void disable_smooth();
		QPLDLL bool is_smooth() const;
		QPLDLL void enable_resize_with_window();
		QPLDLL void disable_resize_with_window();
		QPLDLL bool is_resize_with_window_enabled() const;

		QPLDLL void enable_clear_with_window();
		QPLDLL void disable_clear_with_window();
		QPLDLL bool is_clear_with_window_enabled() const;

		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_multiplied_color(qsf::rgb color);
		QPLDLL void set_position(qpl::vector2f position);
		QPLDLL void set_scale(qpl::vector2f scale);
		QPLDLL void set_scale(qpl::f32 scale);
		QPLDLL void set_origin(qpl::vector2f origin);
		QPLDLL void set_rotation(qpl::f32 rotation);

		QPLDLL qsf::rgb get_color() const;
		QPLDLL qsf::rgb get_multiplied_color() const;
		QPLDLL qpl::vector2f get_position() const;
		QPLDLL qpl::vector2f get_scale() const;
		QPLDLL qpl::vector2f get_origin() const;
		QPLDLL qpl::f32 get_rotation() const;
		QPLDLL qpl::vector2f get_dimension() const;
		QPLDLL qpl::vector2f get_center() const;

		QPLDLL void move(qpl::vector2f delta);
		QPLDLL void move_scaled(qpl::vector2f delta);

		QPLDLL const qsf::sprite& get_sprite() const;
		QPLDLL void clear();
		QPLDLL void display();

		QPLDLL const sf::RenderStates& get_render_states() const;
		QPLDLL const sf::Texture& get_texture() const;

		QPLDLL void set_shader(const std::string& name);
		QPLDLL void set_shader(sf::Shader& shader);
		QPLDLL void unbind_shader();

		template<typename T> requires (qsf::has_any_draw<T>())
		void draw(const T& object) {
			if constexpr (qsf::is_render_texture<T>()) {
				this->m_texture.draw(object.get_sprite(), this->m_states);
				this->m_changed = true;
			}
			else if constexpr (std::is_base_of<sf::Drawable, T>()) {
				this->m_texture.draw(object, this->m_states);
				this->m_changed = true;
			}
			else if constexpr (qsf::has_draw_object<T>()) {
				draw_object draw(this->m_texture, this->m_states);
				object.draw(draw);
				this->m_changed = true;
			}
			else if constexpr (qsf::has_draw_sf<T>()) {
				object.draw(this->m_texture, this->m_states);
				this->m_changed = true;
			}
		}

		template<typename T> requires (qsf::has_any_draw<T>())
		void draw(const T& object, sf::RenderStates states) {
			if constexpr (qsf::is_render_texture<T>()) {
				this->m_texture.draw(object.get_sprite(), states);
				this->m_changed = true;
			}
			else if constexpr (std::is_base_of<sf::Drawable, T>()) {
				this->m_texture.draw(object, states);
				this->m_changed = true;
			}
			else if constexpr (qsf::has_draw_object<T>()) {
				draw_object draw(this->m_texture, states);
				object.draw(draw);
				this->m_changed = true;
			}
			else if constexpr (qsf::has_draw_sf<T>()) {
				object.draw(this->m_texture, states);
				this->m_changed = true;
			}
		}



	private:
		QPLDLL void apply() const;

		mutable sf::RenderTexture m_texture;
		mutable qsf::sprite m_sprite;
		mutable bool m_changed = false;
		qpl::vector2i m_create_size;
		bool m_smooth = false;
		bool m_resize_with_window = true;
		bool m_clear_with_window = true;
		sf::RenderStates m_states;
		sf::ContextSettings m_settings;
	};

	struct pixel_image {
		pixel_image() {
			this->vertices.set_primitive_type(qsf::primitive_type::quads);
		}

		QPLDLL void set_array_dimension(qpl::vector2u dimension);
		QPLDLL void set_pixel_dimension(qpl::vector2f dimension);
		QPLDLL void set_position(qpl::vector2f position);
		QPLDLL void create_positions();

		QPLDLL void set(qpl::size x, qpl::size y, qsf::rgb color);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;


		qpl::vector2u array_dimension;
		qpl::vector2f pixel_dimension;
		qpl::vector2f position;
		qsf::vertex_array vertices;
		bool positions_set = false;
	};


	struct text_stream {
		text_stream() {
			this->states.push_back({});
		}

		enum class duration_type {
			next_entry, end_of_line, until_end
		};

		struct object_t {
			qsf::text text;
			qpl::hitbox hitbox;
			qpl::u32 sprite_index = qpl::u32_max;
			qpl::vector2f shift;
			qpl::vector2f shift_before;
			bool visible = true;
			bool absolute = false;

			bool is_text() const {
				return this->sprite_index == qpl::u32_max;
			}
		};

		constexpr static duration_type next_entry = duration_type::next_entry;
		constexpr static duration_type end_of_line = duration_type::end_of_line;
		constexpr static duration_type until_end = duration_type::until_end;

		struct font {
			font(const std::string& font, duration_type duration = next_entry) {
				this->value = font;
				this->duration = duration;
			}
			font until_next_entry() const {
				auto copy = *this; copy.duration = duration_type::next_entry; return copy;
			}
			font until_end_of_line() const {
				auto copy = *this; copy.duration = duration_type::end_of_line; return copy;
			}
			font until_end() const {
				auto copy = *this; copy.duration = duration_type::until_end; return copy;
			}
			std::string value;
			duration_type duration;
		};
		struct color {
			constexpr color(qsf::rgb color, duration_type duration = next_entry) : value(color), duration(duration) {

			}
			color until_next_entry() const {
				auto copy = *this; copy.duration = duration_type::next_entry; return copy;
			}
			color until_end_of_line() const {
				auto copy = *this; copy.duration = duration_type::end_of_line; return copy;
			}
			color until_end() const {
				auto copy = *this; copy.duration = duration_type::until_end; return copy;
			}
			qsf::rgb value;
			duration_type duration;
		};
		struct character_size {
			character_size(qpl::u32 character_size, duration_type duration = next_entry) {
				this->value = character_size;
				this->duration = duration;
			}
			character_size until_next_entry() const {
				auto copy = *this; copy.duration = duration_type::next_entry; return copy;
			}
			character_size until_end_of_line() const {
				auto copy = *this; copy.duration = duration_type::end_of_line; return copy;
			}
			character_size until_end() const {
				auto copy = *this; copy.duration = duration_type::until_end; return copy;
			}
			qpl::u32 value;
			duration_type duration;
		};
		struct style {
			style(qpl::u32 style, duration_type duration = next_entry) {
				this->duration = duration;
				this->value = style;
			}
			style until_next_entry() const {
				auto copy = *this; copy.duration = duration_type::next_entry; return copy;
			}
			style until_end_of_line() const {
				auto copy = *this; copy.duration = duration_type::end_of_line; return copy;
			}
			style until_end() const {
				auto copy = *this; copy.duration = duration_type::until_end; return copy;
			}
			qpl::u32 value;
			duration_type duration;
		};
		struct outline_thickness {
			outline_thickness(qpl::f32 outline_thickness, duration_type duration = next_entry) {
				this->value = outline_thickness;
				this->duration = duration;
			}

			outline_thickness until_next_entry() const {
				auto copy = *this; copy.duration = duration_type::next_entry; return copy;
			}
			outline_thickness until_end_of_line() const {
				auto copy = *this; copy.duration = duration_type::end_of_line; return copy;
			}
			outline_thickness until_end() const {
				auto copy = *this; copy.duration = duration_type::until_end; return copy;
			}
			qpl::f32 value;
			duration_type duration;
		};
		struct outline_color {
			outline_color(qsf::rgb outline_color, duration_type duration = next_entry) {
				this->value = outline_color;
				this->duration = duration;
			}

			outline_color until_next_entry() const {
				auto copy = *this; copy.duration = duration_type::next_entry; return copy;
			}
			outline_color until_end_of_line() const {
				auto copy = *this; copy.duration = duration_type::end_of_line; return copy;
			}
			outline_color until_end() const {
				auto copy = *this; copy.duration = duration_type::until_end; return copy;
			}
			qsf::rgb value;
			duration_type duration;
		};
		struct letter_spacing {
			letter_spacing(qpl::f32 spacing, duration_type duration = next_entry) {
				this->value = spacing;
				this->duration = duration;
			}

			letter_spacing until_next_entry() const {
				auto copy = *this; copy.duration = duration_type::next_entry; return copy;
			}
			letter_spacing until_end_of_line() const {
				auto copy = *this; copy.duration = duration_type::end_of_line; return copy;
			}
			letter_spacing until_end() const {
				auto copy = *this; copy.duration = duration_type::until_end; return copy;
			}
			qpl::f32 value;
			duration_type duration;
		};
		struct spacing {
			spacing(qpl::f32 space, duration_type duration = next_entry) {
				this->value = space;
				this->duration = duration;
			}

			spacing until_next_entry() const {
				auto copy = *this; copy.duration = duration_type::next_entry; return copy;
			}
			spacing until_end_of_line() const {
				auto copy = *this; copy.duration = duration_type::end_of_line; return copy;
			}
			spacing until_end() const {
				auto copy = *this; copy.duration = duration_type::until_end; return copy;
			}
			qpl::f32 value;
			duration_type duration;
		};
		struct absolute_spacing {
			absolute_spacing(qpl::f32 space, duration_type duration = next_entry) {
				this->value = space;
				this->duration = duration;
			}

			absolute_spacing until_next_entry() const {
				auto copy = *this; copy.duration = duration_type::next_entry; return copy;
			}
			absolute_spacing until_end_of_line() const {
				auto copy = *this; copy.duration = duration_type::end_of_line; return copy;
			}
			absolute_spacing until_end() const {
				auto copy = *this; copy.duration = duration_type::until_end; return copy;
			}
			qpl::f32 value;
			duration_type duration;
		};
		struct shift {
			shift(qpl::vector2f shift, duration_type duration = next_entry) {
				this->value = shift;
				this->duration = next_entry;
			}

			shift until_next_entry() const {
				auto copy = *this; copy.duration = duration_type::next_entry; return copy;
			}
			shift until_end_of_line() const {
				auto copy = *this; copy.duration = duration_type::end_of_line; return copy;
			}
			shift until_end() const {
				auto copy = *this; copy.duration = duration_type::until_end; return copy;
			}
			qpl::vector2f value;
			duration_type duration;
		};
		struct shift_y {
			shift_y(qpl::f32 shift, duration_type duration = next_entry) {
				this->value = shift;
				this->duration = next_entry;
			}

			shift_y until_next_entry() const {
				auto copy = *this; copy.duration = duration_type::next_entry; return copy;
			}
			shift_y until_end_of_line() const {
				auto copy = *this; copy.duration = duration_type::end_of_line; return copy;
			}
			shift_y until_end() const {
				auto copy = *this; copy.duration = duration_type::until_end; return copy;
			}
			qpl::f32 value;
			duration_type duration;
		};
		struct scaling {
			scaling(qpl::vector2f scaling, duration_type duration = next_entry) {
				this->value = scaling;
				this->duration = next_entry;
			}
			scaling(qpl::f32 scaling, duration_type duration = next_entry) {
				this->value = qpl::vector2f(scaling, scaling);
				this->duration = next_entry;
			}

			scaling until_next_entry() const {
				auto copy = *this; copy.duration = duration_type::next_entry; return copy;
			}
			scaling until_end_of_line() const {
				auto copy = *this; copy.duration = duration_type::end_of_line; return copy;
			}
			scaling until_end() const {
				auto copy = *this; copy.duration = duration_type::until_end; return copy;
			}
			qpl::vector2f value;
			duration_type duration;
		};



		template<typename T>
		text_stream& operator<<(const T& value) {
			return this->operator<<(qpl::to_auto_string(value));
		}
		template<>
		text_stream& operator<<(const text_stream::font& font) {
			if (this->states.back().duration == font.duration) {
				this->states.back().font = font.value;
			}
			else {
				this->states.push_back(this->states.back());
				this->states.back().font = font.value;
				this->states.back().duration = font.duration;
			}
			return *this;
		}
		template<>
		text_stream& operator<<(const text_stream::color& color) {
			if (this->states.back().duration == color.duration) {
				this->states.back().color = color.value;
			}
			else {
				this->states.push_back(this->states.back());
				this->states.back().color = color.value;
				this->states.back().duration = color.duration;
			}
			return *this;
		}
		template<>
		text_stream& operator<<(const text_stream::character_size& character_size) {
			if (this->states.back().duration == character_size.duration) {
				this->states.back().character_size = character_size.value;
			}
			else {
				this->states.push_back(this->states.back());
				this->states.back().character_size = character_size.value;
				this->states.back().duration = character_size.duration;
			}
			return *this;
		}
		template<>
		text_stream& operator<<(const text_stream::style& style) {
			if (this->states.back().duration == style.duration) {
				this->states.back().style = style.value;
			}
			else {
				this->states.push_back(this->states.back());
				this->states.back().style = style.value;
				this->states.back().duration = style.duration;
			}
			return *this;
		}
		template<>
		text_stream& operator<<(const text_stream::outline_thickness& outline_thickness) {
			if (this->states.back().duration == outline_thickness.duration) {
				this->states.back().outline_thickness = outline_thickness.value;
			}
			else {
				this->states.push_back(this->states.back());
				this->states.back().outline_thickness = outline_thickness.value;
				this->states.back().duration = outline_thickness.duration;
			}
			return *this;
		}
		template<>
		text_stream& operator<<(const text_stream::outline_color& outline_color) {
			if (this->states.back().duration == outline_color.duration) {
				this->states.back().outline_color = outline_color.value;
			}
			else {
				this->states.push_back(this->states.back());
				this->states.back().outline_color = outline_color.value;
				this->states.back().duration = outline_color.duration;
			}
			return *this;
		}
		template<>
		text_stream& operator<<(const text_stream::letter_spacing& letter_spacing) {
			if (this->states.back().duration == letter_spacing.duration) {
				this->states.back().letter_spacing = letter_spacing.value;
			}
			else {
				this->states.push_back(this->states.back());
				this->states.back().letter_spacing = letter_spacing.value;
				this->states.back().duration = letter_spacing.duration;
			}
			return *this;
		}
		template<>
		text_stream& operator<<(const text_stream::spacing& space) {
			if (this->states.back().duration == space.duration) {
				this->states.back().spacing = space.value;
			}
			else {
				this->states.push_back(this->states.back());
				this->states.back().spacing = space.value;
				this->states.back().duration = space.duration;
			}
			return *this;
		}
		template<>
		text_stream& operator<<(const text_stream::absolute_spacing& space) {
			if (this->states.back().duration == space.duration) {
				this->states.back().absolute_spacing = space.value;
			}
			else {
				this->states.push_back(this->states.back());
				this->states.back().absolute_spacing = space.value;
				this->states.back().duration = space.duration;
			}
			return *this;
		}
		template<>
		text_stream& operator<<(const text_stream::shift& shift) {
			if (this->states.back().duration == shift.duration) {
				this->states.back().shift = shift.value;
			}
			else {
				this->states.push_back(this->states.back());
				this->states.back().shift = shift.value;
				this->states.back().duration = shift.duration;
			}
			return *this;
		}
		template<>
		text_stream& operator<<(const text_stream::shift_y& shift) {
			if (this->states.back().duration == shift.duration) {
				this->states.back().shift.y = shift.value;
				this->states.back().shift.x = 0;
			}
			else {
				this->states.push_back(this->states.back());
				this->states.back().shift.y = shift.value;
				this->states.back().shift.x = 0;
				this->states.back().duration = shift.duration;
			}
			return *this;
		}
		template<>
		text_stream& operator<<(const text_stream::scaling& scaling) {
			if (this->states.back().duration == scaling.duration) {
				this->states.back().scaling = scaling.value;
			}
			else {
				this->states.push_back(this->states.back());
				this->states.back().scaling = scaling.value;
				this->states.back().duration = scaling.duration;
			}
			return *this;
		}
		template<>
		text_stream& operator<<(const qsf::endl_type& end) {
			this->new_line();
			return *this;
		}



		QPLDLL void apply_state();

		QPLDLL void new_line();
		QPLDLL void clear();
		QPLDLL text_stream& operator<<(const sf::Texture& texture);
		QPLDLL text_stream& operator<<(const qsf::sprite& sprite);
		QPLDLL text_stream& operator<<(const std::string& string);
		QPLDLL text_stream& operator<<(const std::wstring& string);
		QPLDLL text_stream& add_texture(const sf::Texture& texture);
		QPLDLL text_stream& add_sprite(const qsf::sprite& sprite);
		QPLDLL text_stream& add_string(const std::string& string, bool has_no_newline = false, bool pop_state = true);
		QPLDLL text_stream& add_string(const std::wstring& string, bool has_no_newline = false, bool pop_state = true);
		QPLDLL qpl::hitbox line_text_hitbox(qpl::size index) const;
		QPLDLL qpl::hitbox line_sprite_hitbox(qpl::size index) const;
		QPLDLL qpl::hitbox line_hitbox(qpl::size index) const;
		QPLDLL qpl::hitbox line_visible_hitbox(qpl::size index) const;

		//I don't have seen it get stuck yet, but because there is a possibility due to floating points being weird, I have a loop check here
		QPLDLL void centerize_line(qpl::size index, qpl::u32 loop = 0u) const;
		QPLDLL void centerize_lines() const;

		QPLDLL qpl::size size() const;
		QPLDLL qpl::size lines() const;
		QPLDLL object_t& operator[](qpl::size index);
		QPLDLL const object_t& operator[](qpl::size index) const;

		QPLDLL qpl::size text_size() const;
		QPLDLL qsf::text& get_text(qpl::size index);
		QPLDLL const qsf::text& get_text(qpl::size index) const;
		QPLDLL qsf::text& get_text(qpl::size line, qpl::size index);
		QPLDLL const qsf::text& get_text(qpl::size line, qpl::size index) const;

		QPLDLL qpl::size sprite_size() const;
		QPLDLL qsf::sprite& get_sprite(qpl::size index);
		QPLDLL const qsf::sprite& get_sprite(qpl::size index) const;
		QPLDLL qsf::sprite& get_sprite(qpl::size line, qpl::size index);
		QPLDLL const qsf::sprite& get_sprite(qpl::size line, qpl::size index) const;

		QPLDLL object_t& get_object(qpl::size line, qpl::size index);
		QPLDLL const object_t& get_object(qpl::size line, qpl::size index) const;

		QPLDLL std::vector<qsf::text_stream::object_t>& line(qpl::size index);
		QPLDLL const std::vector<qsf::text_stream::object_t>& line(qpl::size index) const;
		QPLDLL void finalize() const;
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
		QPLDLL void set_font(const std::string& font);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_alpha(qpl::u8 alpha);
		QPLDLL void set_multiplied_color(qsf::rgb color);
		QPLDLL void set_style(qpl::u32 style);
		QPLDLL void set_character_size(qpl::u32 character_size);
		QPLDLL void set_line_spacing(qpl::f32 spacing);
		QPLDLL void set_outline_thickness(qpl::f32 thickness);
		QPLDLL void set_outline_color(qsf::rgb color);
		QPLDLL void set_letter_spacing(qpl::f32 spacing);
		QPLDLL void set_position(qpl::vector2f position);
		QPLDLL void move(qpl::vector2f delta);
		QPLDLL qpl::hitbox get_visible_hitbox() const;

		QPLDLL void update_size_changes();

		struct state {
			std::string font;
			qsf::rgb color = qsf::rgb::white;
			qpl::u32 style = sf::Text::Style::Regular;
			qpl::u32 character_size = 20u;
			qpl::f32 outline_thickness = 0.0f;
			qsf::rgb outline_color;
			qpl::f32 letter_spacing = 1.0f;
			qpl::vector2f scaling = qpl::vector2f(qpl::f32_max, qpl::f32_max);
			qpl::vector2f shift = qpl::vector2f(0, 0);
			qpl::f32 spacing = 0;
			qpl::f32 absolute_spacing = qpl::f32_max;
			duration_type duration = duration_type::until_end;
		};


		std::vector<state> states;
		mutable std::vector<qsf::sprite> sprites;
		mutable std::vector<std::vector<object_t>> objects;
		mutable bool changed = false;
		qpl::f32 line_spacing = 5.0f;
		mutable qpl::hitbox visible_hitbox;

		qpl::vector2f position = { 20, 20 };
	private:
		QPLDLL void prepare_next_entry();
		QPLDLL void calculate_visible_hitbox() const;
	};


	struct tile_map {

		QPLDLL void set_texture_ptr(const sf::Texture& texture, qpl::vector2u texture_tile_dimension);
		QPLDLL void set_texture_ptr(const sf::Texture& texture, qpl::u32 texture_tile_width);
		QPLDLL void set_position(qpl::vector2f position);

		//pair.first = index - pair.second = rotation (0-7)
		QPLDLL void create(const std::vector<std::pair<qpl::u32, qpl::u32>>& indices_and_rotations, qpl::size index_width, qsf::rgb color);
		//pair.first = index - pair.second = rotation (0-7)
		QPLDLL void create(const std::vector<std::pair<qpl::u32, qpl::u32>>& indices_and_rotations, qpl::size index_width);


		//pair.first = index - pair.second = rotation (0-360)
		QPLDLL void create(const std::vector<std::pair<qpl::u32, qpl::f32>>& indices_and_rotations, qpl::size index_width, qsf::rgb color);
		//pair.first = index - pair.second = rotation (0-360)
		QPLDLL void create(const std::vector<std::pair<qpl::u32, qpl::f32>>& indices_and_rotations, qpl::size index_width);


		QPLDLL void create(const std::vector<qpl::u32>& indices, qpl::size index_width, qsf::rgb color);
		QPLDLL void create(const std::vector<qpl::u32>& indices, qpl::size index_width);


		//pair.first = index - pair.second = rotation (0-7)
		QPLDLL void create_skip_empty(const std::vector<std::pair<qpl::u32, qpl::u32>>& indices_and_rotations, qpl::size index_width, qsf::rgb color, qpl::u32 skip_index = 0u);
		//pair.first = index - pair.second = rotation (0-7)
		QPLDLL void create_skip_empty(const std::vector<std::pair<qpl::u32, qpl::u32>>& indices_and_rotations, qpl::size index_width, qpl::u32 skip_index = 0u);

		//pair.first = index - pair.second = rotation (0-360)
		QPLDLL void create_skip_empty(const std::vector<std::pair<qpl::u32, qpl::f32>>& indices_and_rotations, qpl::size index_width, qsf::rgb color, qpl::u32 skip_index = 0u);
		//pair.first = index - pair.second = rotation (0-360)
		QPLDLL void create_skip_empty(const std::vector<std::pair<qpl::u32, qpl::f32>>& indices_and_rotations, qpl::size index_width, qpl::u32 skip_index = 0u);

		QPLDLL void create_skip_empty(const std::vector<qpl::u32>& indices, qpl::size index_width, qsf::rgb color, qpl::u32 skip_index = 0u);
		QPLDLL void create_skip_empty(const std::vector<qpl::u32>& indices, qpl::size index_width, qpl::u32 skip_index = 0u);

		template<typename C> requires (qpl::is_container<C>() && qpl::has_size<C>())
		void create_skip_empty(const C& indices, qpl::size index_width, qpl::u32 skip_index = 0u, qsf::rgb color = qsf::rgb::white) {
			if (!this->texture_ptr_set) {
				qpl::println("tile_map::create: texture_ptr not set");
				return;
			}

			this->chunks.clear();
			this->color = color;

			auto chunk_width = (index_width - 1) / this->max_chunk_size.x + 1;
			auto chunk_height = (indices.size() / index_width - 1) / this->max_chunk_size.y + 1;
			auto chunk_dim = chunk_width * chunk_height;

			this->chunk_width_count = chunk_width;
			this->chunks.resize(chunk_dim);
			for (auto& chunk : this->chunks) {
				chunk.setPrimitiveType(sf::Quads);
			}

			auto texture_row_tile_count = texture_ptr->getSize().x / this->texture_tile_dimension.x;

			qpl::u32 ctr = 0;
			if (color == qsf::rgb::white) {
				for (qpl::u32 i = 0; i < indices.size(); ++i) {
					auto index = indices[i];
					if (index == skip_index) {
						continue;
					}
					auto chunk_x = (i % index_width) / this->max_chunk_size.x;
					auto chunk_y = (i / index_width) / this->max_chunk_size.y;
					auto chunk_index = chunk_x + chunk_y * chunk_width;

					auto& chunk = this->chunks[chunk_index];
					auto ctr = chunk.getVertexCount();
					chunk.resize(ctr + 4);

					auto [y, x] = qpl::div_mod(i, index_width);

					auto tile_x = (index % texture_row_tile_count);
					auto tile_y = (index / texture_row_tile_count);

					chunk[ctr + 0].position = this->position + qpl::vector2u(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
					chunk[ctr + 1].position = this->position + qpl::vector2u((x + 1) * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
					chunk[ctr + 2].position = this->position + qpl::vector2u((x + 1) * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);
					chunk[ctr + 3].position = this->position + qpl::vector2u(x * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);

					chunk[ctr + 0].texCoords = qpl::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
					chunk[ctr + 1].texCoords = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
					chunk[ctr + 2].texCoords = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
					chunk[ctr + 3].texCoords = qpl::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				}
			}
			else {
				for (qpl::u32 i = 0; i < indices.size(); ++i) {
					auto index = indices[i];
					if (index == skip_index) {
						continue;
					}

					auto chunk_x = (i % index_width) / this->max_chunk_size.x;
					auto chunk_y = (i / index_width) / this->max_chunk_size.y;
					auto chunk_index = chunk_x + chunk_y * chunk_width;

					auto& chunk = this->chunks[chunk_index];
					auto ctr = chunk.getVertexCount();
					chunk.resize(ctr + 4);

					auto [y, x] = qpl::div_mod(i, index_width);
					auto tile_x = (index % texture_row_tile_count);
					auto tile_y = (index / texture_row_tile_count);

					chunk[ctr + 0].position = this->position + qpl::vector2u(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
					chunk[ctr + 1].position = this->position + qpl::vector2u((x + 1) * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
					chunk[ctr + 2].position = this->position + qpl::vector2u((x + 1) * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);
					chunk[ctr + 3].position = this->position + qpl::vector2u(x * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);

					chunk[ctr + 0].color = color;
					chunk[ctr + 1].color = color;
					chunk[ctr + 2].color = color;
					chunk[ctr + 3].color = color;

					chunk[ctr + 0].texCoords = qpl::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
					chunk[ctr + 1].texCoords = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
					chunk[ctr + 2].texCoords = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
					chunk[ctr + 3].texCoords = qpl::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				}
			}

		}

		QPLDLL void set_color(qsf::rgb color);

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
		QPLDLL qpl::size size() const;
		QPLDLL qpl::size chunk_count() const;
		QPLDLL void set_chunk_dimension(qpl::u32 x, qpl::u32 y);
		QPLDLL void clear();

		qpl::vector2u max_chunk_size = { qpl::u32_max, qpl::u32_max };
		qpl::size chunk_width_count = 0u;
		std::vector<sf::VertexArray> chunks;
		const sf::Texture* texture_ptr;
		bool texture_ptr_set = false;
		qpl::vector2u texture_tile_dimension;
		qpl::vector2u position_tile_dimension;
		qpl::vector2f position;

		qsf::rgb color;
	};

	namespace detail {
		template<typename T>
		concept has_light_c = requires(const T x) {
			{x.light() } -> std::convertible_to<qpl::f64>;
		};

		template<typename T>
		constexpr bool has_light() {
			return has_light_c<T>;
		}
	}

	struct small_tile_map {


		sf::VertexArray vertices;
		qpl::vector2f position;
		const sf::Texture* texture_ptr;
		qpl::vector2f scale = { 1, 1 };
		qpl::vector2u texture_tile_dimension;

		QPLDLL void set_texture(const sf::Texture& texture, qpl::u32 texture_tile_width);
		QPLDLL void set_position(qpl::vector2f position);
		QPLDLL void set_scale(qpl::vector2f scale);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;

		template<typename C> requires (qpl::is_container<C>() && qpl::has_size<C>())
		void create(const C& indices, qpl::size index_width) {
			if (!this->texture_ptr) {
				qpl::println("small_tile_map::create: texture_ptr not set");
				return;
			}

			this->vertices.clear();
			this->vertices.setPrimitiveType(sf::Quads);
			this->vertices.resize(indices.size() * 4);

			auto texture_row_tile_count = this->texture_ptr->getSize().x * this->texture_tile_dimension.x;

			qpl::u32 ctr = 0;
			for (qpl::u32 i = 0; i < indices.size(); ++i) {
				auto index = indices[i];
				//if (index == skip_index) {
				//	continue;
				//}

				auto [y, x] = qpl::div_mod(i, index_width);

				auto tile_x = (index % texture_row_tile_count);
				auto tile_y = (index / texture_row_tile_count);

				this->vertices[ctr + 0].position = this->position + qpl::vector2u(x, y) * this->scale * this->texture_tile_dimension;
				this->vertices[ctr + 1].position = this->position + qpl::vector2u((x + 1), y) * this->scale * this->texture_tile_dimension;
				this->vertices[ctr + 2].position = this->position + qpl::vector2u((x + 1), (y + 1)) * this->scale * this->texture_tile_dimension;
				this->vertices[ctr + 3].position = this->position + qpl::vector2u(x, (y + 1)) * this->scale * this->texture_tile_dimension;

				this->vertices[ctr + 0].texCoords = qpl::vector2u(tile_x, tile_y) * this->texture_tile_dimension;
				this->vertices[ctr + 1].texCoords = qpl::vector2u((tile_x + 1), tile_y) * this->texture_tile_dimension;
				this->vertices[ctr + 2].texCoords = qpl::vector2u((tile_x + 1), (tile_y + 1)) * this->texture_tile_dimension;
				this->vertices[ctr + 3].texCoords = qpl::vector2u(tile_x, (tile_y + 1)) * this->texture_tile_dimension;
				ctr += 4;
			}
		}

		template<typename C> requires (qpl::is_container<C>() && qpl::has_size<C>() && qsf::detail::has_light<qpl::container_subtype<C>>())
		void create_light(const C& indices, qpl::size index_width, qsf::rgb no_light = qsf::rgb::black, qsf::rgb full_light = qsf::rgb::white) {
			if (!this->texture_ptr) {
				qpl::println("small_tile_map::create: texture_ptr not set");
				return;
			}

			this->vertices.clear();
			this->vertices.setPrimitiveType(sf::Quads);
			this->vertices.resize(indices.size() * 4);

			auto texture_row_tile_count = this->texture_ptr->getSize().x * this->texture_tile_dimension.x;

			qpl::u32 ctr = 0;
			for (qpl::u32 i = 0; i < indices.size(); ++i) {
				auto index = indices[i];
				//if (index == skip_index) {
				//	continue;
				//}

				auto [y, x] = qpl::div_mod(i, index_width);

				auto tile_x = (index % texture_row_tile_count);
				auto tile_y = (index / texture_row_tile_count);

				this->vertices[ctr + 0].position = (qpl::vector2u(x, y) * this->scale * this->texture_tile_dimension);
				this->vertices[ctr + 1].position = (qpl::vector2u((x + 1), y) * this->scale * this->texture_tile_dimension);
				this->vertices[ctr + 2].position = (qpl::vector2u((x + 1), (y + 1)) * this->scale * this->texture_tile_dimension);
				this->vertices[ctr + 3].position = (qpl::vector2u(x, (y + 1)) * this->scale * this->texture_tile_dimension);

				this->vertices[ctr + 0].texCoords = qpl::vector2u(tile_x, tile_y) * this->texture_tile_dimension;
				this->vertices[ctr + 1].texCoords = qpl::vector2u((tile_x + 1), tile_y) * this->texture_tile_dimension;
				this->vertices[ctr + 2].texCoords = qpl::vector2u((tile_x + 1), (tile_y + 1)) * this->texture_tile_dimension;
				this->vertices[ctr + 3].texCoords = qpl::vector2u(tile_x, (tile_y + 1)) * this->texture_tile_dimension;

				this->vertices[ctr + 0].color = no_light.interpolated(full_light, indices[i].light());
				this->vertices[ctr + 1].color = no_light.interpolated(full_light, indices[i].light());
				this->vertices[ctr + 2].color = no_light.interpolated(full_light, indices[i].light());
				this->vertices[ctr + 3].color = no_light.interpolated(full_light, indices[i].light());
				ctr += 4;
			}
		}

		template<typename C> requires (qpl::is_container<C>() && qpl::has_size<C>())
		void create_skip_empty(const C& indices, qpl::size index_width, qpl::u32 skip_index) {
			if (!this->texture_ptr) {
				qpl::println("small_tile_map::create: texture_ptr not set");
				return;
			}

			this->vertices.clear();
			this->vertices.setPrimitiveType(sf::Quads);
			this->vertices.resize(indices.size() * 4);

			auto texture_row_tile_count = this->texture_ptr->getSize().x * this->texture_tile_dimension.x;

			qpl::u32 ctr = 0;
			for (qpl::u32 i = 0; i < indices.size(); ++i) {
				auto index = indices[i];
				if (index == skip_index) {
					continue;
				}

				auto [y, x] = qpl::div_mod(i, index_width);

				auto tile_x = (index % texture_row_tile_count);
				auto tile_y = (index / texture_row_tile_count);

				this->vertices[ctr + 0].position = this->position + qpl::vector2u(x, y) * this->scale * this->texture_tile_dimension;
				this->vertices[ctr + 1].position = this->position + qpl::vector2u((x + 1), y) * this->scale * this->texture_tile_dimension;
				this->vertices[ctr + 2].position = this->position + qpl::vector2u((x + 1), (y + 1)) * this->scale * this->texture_tile_dimension;
				this->vertices[ctr + 3].position = this->position + qpl::vector2u(x, (y + 1)) * this->scale * this->texture_tile_dimension;

				this->vertices[ctr + 0].texCoords = qpl::vector2u(tile_x, tile_y) * this->texture_tile_dimension;
				this->vertices[ctr + 1].texCoords = qpl::vector2u((tile_x + 1), tile_y) * this->texture_tile_dimension;
				this->vertices[ctr + 2].texCoords = qpl::vector2u((tile_x + 1), (tile_y + 1)) * this->texture_tile_dimension;
				this->vertices[ctr + 3].texCoords = qpl::vector2u(tile_x, (tile_y + 1)) * this->texture_tile_dimension;
				ctr += 4;
			}
		}


	};

	struct vgraph {
		
		vgraph() {
			this->x_axis_string_function = [](qpl::size index) {
				return qpl::to_string(index);
			};
			this->closest_graph_at_cursor_text.set_character_size(30);
			this->closest_graph_at_cursor_text.set_style(sf::Text::Style::Bold);
			this->closest_graph_at_cursor_background.set_outline_thickness(2);
		}

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;


		struct highlighted_line {
			qsf::rgb color;
			qpl::f64 thickness = 0.0;
			bool foreground = true;
		};

		struct data_point_info {
			qpl::f64 data = 0.0;
			qpl::f64 thickness = qpl::f64_min;
			qsf::frgb color = qsf::rgb::unset;

			qsf::vcircle circle;
			qsf::vtext text;

			data_point_info() {
				this->thickness = qpl::f64_min;
				this->color = qsf::frgb::unset;
			}
			data_point_info(qpl::f64 value) {
				this->data = value;
			}
			data_point_info(const data_point_info& other) {
				this->data = other.data;
				this->color = other.color;
				this->thickness = other.thickness;
				this->circle = other.circle;
				this->text = other.text;
			}
			QPLDLL data_point_info& operator=(const data_point_info& other);

			QPLDLL data_point_info operator*(qpl::f64 n) const;
			QPLDLL data_point_info operator*(data_point_info other) const;
			QPLDLL data_point_info operator-(data_point_info other) const;
			QPLDLL data_point_info operator+(data_point_info other) const;
			QPLDLL bool operator<(data_point_info other) const;
			QPLDLL bool operator>(data_point_info other) const;
		};

		struct data_point {
			qpl::f64 data = 0.0;
			qpl::f64 thickness = qpl::f64_min;
			qsf::frgb color = qsf::rgb::unset;

			data_point() {

			}
			data_point(qpl::f64 data) : data(data) {

			}
			data_point(const data_point& other) {
				this->data = other.data;
				this->color = other.color;
				this->thickness = other.thickness;
			}
			QPLDLL data_point& operator=(const data_point& other);

			QPLDLL data_point operator*(qpl::f64 n) const;
			QPLDLL data_point operator*(data_point other) const;
			QPLDLL data_point operator-(data_point other) const;
			QPLDLL data_point operator+(data_point other) const;
			QPLDLL bool operator<(data_point other) const;
			QPLDLL bool operator>(data_point other) const;
		};

		struct data_point_simple {
			qpl::f64 data = 0.0;

			data_point_simple() {

			}
			data_point_simple(qpl::f64 data) : data(data) {

			}
			data_point_simple(const data_point_simple& other) {
				this->data = other.data;
			}
			QPLDLL data_point_simple& operator=(const data_point_simple& other);

			QPLDLL data_point_simple operator*(qpl::f64 n) const;
			QPLDLL data_point_simple operator*(data_point_simple other) const;
			QPLDLL data_point_simple operator-(data_point_simple other) const;
			QPLDLL data_point_simple operator+(data_point_simple other) const;
			QPLDLL bool operator<(data_point_simple other) const;
			QPLDLL bool operator>(data_point_simple other) const;
		};

		qpl::f64 min_min_value = qpl::f64_min;
		qpl::f64 min_max_value = qpl::f64_max;
		qpl::f64 max_min_value = qpl::f64_min;
		qpl::f64 max_max_value = qpl::f64_max;

		qpl::f64 height_delta = 0.1;

		qsf::rgb x_axis_color = qsf::rgb::unset;
		qsf::rgb y_axis_color = qsf::rgb::unset;
		qsf::rgb axis_color = qsf::rgb::white;
		qpl::f64 y_axis_text_space = 40.0;
		qpl::size desired_y_axis_lines = 10u;
		qsf::vtext y_axis_text;
		bool y_axis_text_left = true;
		bool y_axis_text_percent = false;
		bool y_axis_text_integer = false;
		qpl::size y_axis_text_precision = 3u;
		bool use_y_axis = true;
		bool y_axis_start_at_0 = false;
		qpl::f64 y_axis_padding = 0.1;

		qpl::f64 x_axis_text_space = 5.0;
		mutable qpl::size x_axis_line_frequency = 200u;
		qsf::vtext x_axis_text;
		bool x_axis_text_left = true;
		bool x_axis_text_percent = false;
		qpl::size x_axis_text_precision = 3u;


		qpl::size x_data_steps = 10u;
		qpl::u32 desired_x_axis_lines = 10u;
		qpl::u32 desired_visible_size = 200u;

		bool allow_dragging = true;
		bool enable_last_n_when_dragging_right_lock = false;
		qpl::size display_last_n_entries = qpl::size_max;
		qpl::size index_start = 0u;
		qpl::size index_end = qpl::size_max;
		mutable qpl::size index_skip_size = 1u;
		mutable std::pair<qpl::size, qpl::size> visible_index_range_before = { qpl::size_max, qpl::size_max };

		bool use_x_axis = true;
		std::function<std::string(qpl::size)> x_axis_string_function;
		std::function<std::string(qpl::f64)> y_axis_string_function;
		std::function<std::string(qpl::f64)> y_axis_cursor_string_function;

		qsf::rgb axis_line_color = qsf::rgb(30, 30, 30);
		qpl::f64 axis_thickness = 1.5f;



		std::string font;
		qpl::f64 zoom_factor = 1.3;
		qpl::vector2f position;
		qpl::vector2f dimension = { 500, 300 };

		mutable qpl::vector2f low_high_graph;
		std::string closest_graph_at_cursor = "";
		qpl::f64 closest_graph_at_cursor_value = qpl::f64_max;
		qpl::u32 closest_graph_at_cursor_index = qpl::u32_max;
		qsf::rgb closest_graph_at_cursor_color;
		qpl::f32 closest_graph_at_cursor_distance = 40.0f;
		qpl::f32 closest_graph_at_cursor_multiply_thickness = 2.0f;
		qpl::vector2f mouse_position;
		bool display_closest_graph_at_cursor = true;

		qsf::vtext closest_graph_at_cursor_text;
		qsf::vrectangle closest_graph_at_cursor_background;
		bool closest_graph_at_cursor_background_is_graph_background = true;
		std::function<std::string(std::string, qpl::f64, qpl::u32)> closest_graph_at_cursor_string_function;

		enum class interpolation_type : qpl::u32 {
			unset = 0,
			linear = 1,
			cubic = 2,
		};

		qsf::rgb color = qsf::rgb::white;
		qpl::f64 thickness = 1.0f;
		interpolation_type interpolation = interpolation_type::cubic;
		qpl::size interpolation_steps = 1u;


		struct info_graph {
			template<typename T>
			void set_data(const std::vector<T>& data) {
				this->data.resize(data.size());
				for (qpl::u32 i = 0u; i < data.size(); ++i) {
					this->data[i].data = static_cast<qpl::f64>(data[i]);
					this->data[i].color = this->color;
					this->data[i].thickness = this->thickness;
				}
			}
			template<typename T>
			void set_data(const std::vector<T>& data, qsf::rgb color, qpl::f64 thickness) {
				this->color = color;
				this->thickness = thickness;
				this->set_data(data);
			}
			QPLDLL void add_data(qpl::f64 data);
			QPLDLL void add_data(qpl::f64 data, qsf::rgb color);
			QPLDLL void add_data(qpl::f64 data, qsf::rgb color, qpl::f64 thickness);
			QPLDLL void add_data(qpl::f64 data, std::string string);
			QPLDLL void add_data(qpl::f64 data, std::string string, qsf::rgb color	);

			QPLDLL void set_font(const std::string& font);
			QPLDLL void set_text_color(qsf::rgb text_color);
			QPLDLL void set_text_character_size(qpl::u32 character_size);
			QPLDLL void set_text_style(qpl::u32 text_style);
			QPLDLL void set_text_outline_thickness(qpl::f32 thickness);
			QPLDLL void set_text_outline_color(qsf::rgb color);

			QPLDLL qpl::f64 get_percentage_height_at(qpl::size index) const;
			QPLDLL std::pair<data_point_info, data_point_info> get_low_high() const;
			QPLDLL qpl::size size() const;
			QPLDLL void clear();
			QPLDLL data_point_info& operator[](qpl::size index);
			QPLDLL const data_point_info& operator[](qpl::size index) const;

			QPLDLL data_point_info& back();
			QPLDLL const data_point_info& back() const;
			QPLDLL data_point_info& front();
			QPLDLL const data_point_info& front() const;

			QPLDLL std::vector<data_point_info>::iterator begin();
			QPLDLL std::vector<data_point_info>::const_iterator begin() const;
			QPLDLL std::vector<data_point_info>::const_iterator cbegin();

			QPLDLL std::vector<data_point_info>::iterator end();
			QPLDLL std::vector<data_point_info>::const_iterator end() const;
			QPLDLL std::vector<data_point_info>::const_iterator cend();

			interpolation_type interpolation = interpolation_type::cubic;
			qsf::rgb color = qsf::rgb::unset;
			qpl::f64 thickness = qpl::f64_min;
			qpl::size interpolation_steps = qpl::size_max;
			qsf::vtext text;
			bool only_draw_if_close = true;

			std::vector<data_point_info> data;
		};

		struct standard_graph {
			template<typename T>
			void set_data(const std::vector<T>& data) {
				this->data.resize(data.size());
				for (qpl::u32 i = 0u; i < data.size(); ++i) {
					this->data[i].data = static_cast<qpl::f64>(data[i]);
					this->data[i].color = this->color;
					this->data[i].thickness = this->thickness;
				}
			}
			template<typename T>
			void set_data(const std::vector<T>& data, qsf::rgb color, qpl::f64 thickness) {
				this->color = color;
				this->thickness = thickness;
				this->set_data(data);
			}
			QPLDLL void add_data(qpl::f64 data);
			QPLDLL void add_data(qpl::f64 data, qsf::rgb color);
			QPLDLL void add_data(qpl::f64 data, qsf::rgb color, qpl::f64 thickness);

			QPLDLL qpl::f64 get_percentage_height_at(qpl::size index) const;
			QPLDLL std::pair<data_point, data_point> get_low_high() const;
			QPLDLL qpl::size size() const;
			QPLDLL void clear();
			QPLDLL data_point& operator[](qpl::size index);
			QPLDLL const data_point& operator[](qpl::size index) const;

			QPLDLL data_point& back();
			QPLDLL const data_point& back() const;
			QPLDLL data_point& front();
			QPLDLL const data_point& front() const;

			QPLDLL std::vector<data_point>::iterator begin();
			QPLDLL std::vector<data_point>::const_iterator begin() const;
			QPLDLL std::vector<data_point>::const_iterator cbegin();

			QPLDLL std::vector<data_point>::iterator end();
			QPLDLL std::vector<data_point>::const_iterator end() const;
			QPLDLL std::vector<data_point>::const_iterator cend();

			interpolation_type interpolation = interpolation_type::cubic;
			qsf::rgb color = qsf::rgb::unset;
			qpl::f64 thickness = qpl::f64_min;
			qpl::size interpolation_steps = qpl::size_max;


			std::vector<data_point> data;
		};

		struct simple_graph {
			template<typename T>
			void set_data(const std::vector<T>& data) {
				this->data.resize(data.size());
				for (qpl::u32 i = 0u; i < data.size(); ++i) {
					this->data[i].data = static_cast<qpl::f64>(data[i]);
				}
			}
			template<typename T>
			void set_data(const std::vector<T>& data, qsf::rgb color, qpl::f64 thickness) {
				this->color = color;
				this->thickness = thickness;
				this->set_data(data);
			}
			QPLDLL void add_data(qpl::f64 data);

			QPLDLL qpl::f64 get_percentage_height_at(qpl::size index) const;
			QPLDLL std::pair<data_point_simple, data_point_simple> get_low_high() const;
			QPLDLL qpl::size size() const;
			QPLDLL void clear();
			QPLDLL data_point_simple& operator[](qpl::size index);
			QPLDLL const data_point_simple& operator[](qpl::size index) const;

			QPLDLL data_point_simple& back();
			QPLDLL const data_point_simple& back() const;
			QPLDLL data_point_simple& front();
			QPLDLL const data_point_simple& front() const;

			QPLDLL std::vector<data_point_simple>::iterator begin();
			QPLDLL std::vector<data_point_simple>::const_iterator begin() const;
			QPLDLL std::vector<data_point_simple>::const_iterator cbegin();

			QPLDLL std::vector<data_point_simple>::iterator end();
			QPLDLL std::vector<data_point_simple>::const_iterator end() const;
			QPLDLL std::vector<data_point_simple>::const_iterator cend();

			interpolation_type interpolation = interpolation_type::cubic;
			qsf::rgb color = qsf::rgb::unset;
			qpl::f64 thickness = qpl::f64_min;
			qpl::size interpolation_steps = qpl::size_max;


			std::vector<data_point_simple> data;
		};

		bool drag = false;
		qpl::vector2f click_position;

		QPLDLL void clear_data();
		QPLDLL bool empty() const;
		QPLDLL bool changed() const;
		QPLDLL void update_change() const;
		QPLDLL void check_x_axis() const;
		QPLDLL void enable_track_new_entries();
		QPLDLL void disable_track_new_entries();
		QPLDLL void enable_axis_info();
		QPLDLL void update(const event_info& event_info);
		QPLDLL void copy_visible_range(const vgraph& other);
		QPLDLL void set_visible_range(qpl::size begin, qpl::size end);

		QPLDLL void set_font(std::string name);
		QPLDLL std::string get_font() const;
		QPLDLL void set_dimension(qpl::vector2f dimension);
		QPLDLL void set_position(qpl::vector2f position);

		QPLDLL qpl::f64 true_graph_width() const;
		QPLDLL qpl::f64 true_graph_height() const;
		QPLDLL qpl::vector2f true_graph_dimension() const;

		QPLDLL std::pair<qpl::f64, qpl::f64> get_low_high() const;
		QPLDLL qpl::size total_graph_size() const;
		QPLDLL qpl::size graph_element_size() const;
		QPLDLL qpl::size visible_element_size() const;
		QPLDLL qpl::size visible_index_start() const;
		QPLDLL qpl::size visible_index_end() const;
		QPLDLL std::pair<qpl::size, qpl::size> visible_index_range() const;
		QPLDLL bool is_range_enabled() const;

		QPLDLL void add_info_graph(std::string name);
		QPLDLL info_graph& get_info_graph(std::string name);
		QPLDLL const info_graph& get_info_graph(std::string name) const;
		QPLDLL std::span<const data_point_info> get_info_graph_span(std::string name) const;

		QPLDLL void add_standard_graph(std::string name);
		QPLDLL standard_graph& get_standard_graph(std::string name);
		QPLDLL const standard_graph& get_standard_graph(std::string name) const;
		QPLDLL std::span<const data_point> get_standard_graph_span(std::string name) const;

		QPLDLL void add_simple_graph(std::string name);
		QPLDLL simple_graph& get_simple_graph(std::string name);
		QPLDLL const simple_graph& get_simple_graph(std::string name) const;
		QPLDLL std::span<const data_point_simple> get_simple_graph_span(std::string name) const;

		QPLDLL void add_y_axis_line(qpl::f64 value);
		QPLDLL void add_y_axis_line(qpl::f64 value, qsf::rgb color, qpl::f64 thickness = 2.0, bool foreground = true);
		QPLDLL highlighted_line& get_y_axis_line(qpl::f64 value);
		QPLDLL const highlighted_line& get_y_axis_line(qpl::f64 value) const;

		std::unordered_map<std::string, info_graph> info_graphs;
		std::unordered_map<std::string, standard_graph> standard_graphs;
		std::unordered_map<std::string, simple_graph> simple_graphs;


		std::unordered_map<qpl::f64, highlighted_line> y_axis_highlighted_lines;

		qsf::rgb background_color = qsf::rgb::black;
	};

	struct graph {

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
		QPLDLL qsf::graph& operator=(const qsf::vgraph& graph);


		std::vector<std::vector<qsf::text>> circle_texts;
		std::vector<qsf::circles> circles;
		std::vector<qsf::thick_lines> y_lines;
		std::vector<qsf::thick_lines> y_lines_foreground;
		std::vector<qsf::text> y_texts;
		std::vector<qsf::thick_lines> x_lines;
		std::vector<qsf::text> x_texts;

		std::vector<qsf::thick_lines> lines;

		qsf::text cursor_graph_text;
		qsf::rectangle cursor_graph_background;

		qsf::rectangle background;
	};

	template<typename T>
	struct view_rectangle;

	struct vbutton {

		vbutton() {
			this->background.set_color(qsf::rgb::black);
		}
		QPLDLL qpl::vector2f get_position() const;
		QPLDLL qsf::rgb get_final_text_color() const;
		QPLDLL qsf::rgb get_final_background_color() const;
		QPLDLL void set_dimension(qpl::vector2f dimension);
		QPLDLL void set_position(qpl::vector2f position);
		QPLDLL void set_center(qpl::vector2f center);
		QPLDLL void set_background_color(qsf::rgb color);
		QPLDLL void set_hover_background_color(qsf::rgb color);
		QPLDLL void set_text_color(qsf::rgb color);
		QPLDLL void set_hover_text_color(qsf::rgb color);
		QPLDLL void set_text_font(std::string font);
		QPLDLL void set_text_character_size(qpl::u32 character_size);
		QPLDLL void set_text_style(qpl::u32 character_style);
		QPLDLL void set_text(std::string text);
		QPLDLL void centerize_text();
		QPLDLL bool is_hovering() const;
		QPLDLL bool is_clicked() const;
		QPLDLL void update(const event_info& event_info);
		QPLDLL void set_text_alpha(qpl::u8 alpha);
		QPLDLL void set_background_alpha(qpl::u8 alpha);

		template<typename T>
		void update(const event_info& event_info, qsf::view_rectangle<T> view) {

			auto pos = view.mouse_position;

			auto new_hovering = this->background.contains(pos);
			if (new_hovering != this->hovering) {
				if (new_hovering) {
					if (this->hover_background_color.is_unset()) {
						this->background.set_color(this->background_color.inverted());
					}
					else {
						this->background.set_color(this->hover_background_color);
					}
					if (this->hover_text_color.is_unset()) {
						this->text.set_color(this->text_color.inverted());
					}
					else {
						this->text.set_color(this->hover_text_color);
					}
				}
				else {
					this->background.set_color(this->background_color);
					this->text.set_color(this->text_color);
				}
			}

			this->hovering = new_hovering;

			this->clicked = this->hovering && event_info.left_mouse_clicked();
		}

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
		QPLDLL qsf::vbutton& operator=(const qsf::vbutton& button);

		qsf::rgb text_color;
		qsf::rgb hover_text_color = qsf::rgb::unset;
		qsf::rgb hover_background_color = qsf::rgb::unset;
		qsf::rgb background_color;
		qsf::vtext text;
		qsf::vrectangle background;
		bool hovering = false;
		bool clicked = false;
		bool outline_on_hover = true;
		bool invert_on_hover = true;
		qpl::u8 text_alpha = 255;
		qpl::u8 background_alpha = 255;
	};

	struct button {

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const;
		QPLDLL qsf::button& operator=(const qsf::vbutton& button);


		button() {
			this->background.set_color(qsf::rgb::black);
		}

		QPLDLL void set_dimension(qpl::vector2f dimension);
		QPLDLL void set_position(qpl::vector2f position);
		QPLDLL void set_center(qpl::vector2f center);
		QPLDLL void set_background_color(qsf::rgb color);
		QPLDLL void set_hover_background_color(qsf::rgb color);
		QPLDLL void set_text_color(qsf::rgb color);
		QPLDLL void set_text_font(std::string font);
		QPLDLL void set_text_character_size(qpl::u32 character_size);
		QPLDLL void set_text_style(qpl::u32 character_style);
		QPLDLL void set_text(std::string text);
		QPLDLL void centerize_text();
		QPLDLL bool is_hovering() const;
		QPLDLL bool is_clicked() const;
		QPLDLL void update(const event_info& event_info);
		QPLDLL void update(const event_info& event_info, bool& hovering);

		template<typename T>
		void update(const event_info& event_info, qsf::view_rectangle<T> view) {

			auto pos = view.mouse_position;

			auto new_hovering = this->background.contains(pos);
			if (new_hovering != this->hovering) {
				if (this->invert_on_hover) {
					if (new_hovering) {
						this->background.set_color(this->background_color.inverted());
						this->text.set_color(this->text_color.inverted());
					}
					else {
						this->background.set_color(this->background_color);
						this->text.set_color(this->text_color);
					}
				}
				else {
					if (new_hovering) {
						this->background.set_color(this->hover_background_color);
					}
					else {
						this->background.set_color(this->background_color);
					}
				}
			}

			this->hovering = new_hovering;

			this->clicked = this->hovering && event_info.left_mouse_clicked();
		}

		bool outline_on_hover = true;
		bool invert_on_hover = true;
		qsf::rgb text_color = qsf::rgb::white;
		qsf::rgb hover_background_color = qsf::rgb::white;
		qsf::rgb background_color = qsf::rgb::black;
		qsf::text text;
		qsf::rectangle background;
		bool hovering = false;
		bool clicked = false;
	};


	template<typename T>
	class slider {
	public:
		slider() {
			this->ptr = &this->dummy;
			this->set_colors(qsf::rgb::grey_shade(50), qsf::rgb::white);
		}

		qpl::f64 get_progress() const {
			if (this->end == this->start) return 0.0;
			return qpl::clamp_0_1(qpl::f64_cast(*this->ptr - this->start) / qpl::f64_cast(this->end - this->start));
		}
		T get_value() const {
			return *this->ptr;
		}
		void set_value(T value) {
			*this->ptr = value;
		}
		void set_pointer(T* value) {
			this->ptr = value;
			if (this->range_set) {
				*this->ptr = qpl::clamp(this->start, *this->ptr, this->end);
			}
		}
		void set_knob_position() {
			auto progress = qpl::f32_cast(this->get_progress());
			auto pos = this->knob.get_position();
			pos.x = this->position.x + this->range_pixel_width * progress;
			this->knob.set_position(pos);
		}
		std::pair<T, T> get_range() const {
			return std::make_pair(this->start, this->end);
		}
		void set_range(T start, T end) {
			this->start = start;
			this->end = end;
			this->range_set = true;
		}
		void set_range(T start, T end, T value) {
			this->start = start;
			this->end = end;
			this->dummy = qpl::clamp(this->start, value, this->end);
			this->range_set = true;
		}
		template<typename P> requires (std::is_pointer_v<P>)
		void set_range(T start, T end, P ptr) {
			this->start = start;
			this->end = end;
			this->range_set = true;
			this->set_pointer(ptr);
		}
		qpl::vector2f get_position() const {
			return this->position;
		}
		void set_position(qpl::vector2f position) {
			this->position = position;
			this->background.set_position(position);

			auto y_extra = this->background.get_dimension().y - this->knob.get_dimension().y;
			this->knob.set_position(position + qpl::vector2f(0, y_extra / 2));
		}
		qpl::vector2f get_dimension() const {
			return this->background.get_dimension();
		}
		qpl::hitbox get_hitbox() const {
			return this->background.get_hitbox();
		}
		qpl::vector2f get_knob_dimension() const {
			return this->knob.get_dimension();
		}
		qpl::vector2f get_knob_position() const {
			return this->knob.get_position();
		}
		void set_knob_dimension(qpl::vector2f dimension) {
			this->knob.set_dimension(dimension);
			this->range_pixel_width = this->background.get_dimension().x - this->knob.get_dimension().x;
		}
		void set_knob_width(qpl::f32 width)  {
			auto dim = this->knob.get_dimension();
			this->knob.set_dimension(qpl::vector2f(width, dim.y));
			this->range_pixel_width = this->background.get_dimension().x - width;
		}
		qpl::f32 get_knob_width() const {
			return this->knob.get_dimension().x;
		}
		qpl::hitbox get_knob_hitbox() const {
			return this->knob.get_hitbox();
		}
		void set_dimensions(qpl::vector2f slider_dimension, qpl::vector2f knob_dimension) {
			this->background.set_dimension(slider_dimension);
			this->knob.set_dimension(knob_dimension);
			this->range_pixel_width = this->background.get_dimension().x - this->knob.get_dimension().x;
		}
		void set_dimension(qpl::vector2f dimension) {
			this->set_dimensions(dimension, sf::Vector2f(dimension.y, dimension.y));
		}
		void set_colors(qsf::rgb background_color, qsf::rgb knob_color) {
			this->background.set_color(background_color);
			this->background_color = background_color;
			this->background_hover_color = background_color;
			this->background_hover_outline_color = background_color;

			this->knob.set_color(knob_color);
			this->knob_hover_color = knob_color;
			this->knob_hover_outline_color = knob_color;
			this->knob_color = knob_color;
		}
		void set_alpha(qpl::u8 alpha) {
			this->background.set_color(this->background.get_color().with_alpha(alpha));
			this->background_color = this->background_color.with_alpha(alpha);
			this->background_hover_color = this->background_hover_color.with_alpha(alpha);
			this->background_hover_outline_color = this->background_hover_outline_color.with_alpha(alpha);

			this->knob.set_color(this->knob.get_color().with_alpha(alpha));
			this->knob_hover_color = this->knob_hover_color.with_alpha(alpha);
			this->knob_hover_outline_color = this->knob_hover_outline_color.with_alpha(alpha);
			this->knob_color = this->knob_color.with_alpha(alpha);
		}
		bool value_has_changed() const {
			return this->value_changed;
		}
		bool value_was_modified() const {
			return this->value_modified;
		}
		bool is_dragging() const {
			return this->dragging;
		}
		bool was_released() const {
			return this->released;
		}

		void set_knob_color(qsf::rgb color) {
			this->knob_color = color;
			this->knob_hover_color = color;
			this->knob_hover_outline_color = color;
			this->knob.set_color(color);
		}
		qsf::rgb get_knob_color() const {
			return this->knob_color;
		}
		void set_knob_outline_thickness(qpl::f32 thickness) {
			this->knob_outline_thickness = thickness;
			this->knob.set_outline_thickness(thickness);
		}
		qpl::f32 get_knob_outline_thickness() const {
			return this->knob_outline_thickness;
		}
		void set_knob_outline_color(qsf::rgb color) {
			this->knob_outline_color = color;
			this->knob.set_outline_color(color);
		}
		qsf::rgb get_knob_outline_color() const {
			return this->knob_outline_color;
		}

		void set_knob_hover_color(qsf::rgb color) {
			this->knob_hover_color = color;
		}
		qsf::rgb get_knob_hover_color() const {
			return this->knob_hover_color;
		}
		void set_knob_hover_outline_thickness(qpl::f32 thickness) {
			this->knob_hover_outline_thickness = thickness;
			if (this->is_hovering_over_knob()) {
				this->knob.set_outline_thickness(thickness);
			}
		}
		qpl::f32 get_knob_hover_outline_thickness() const {
			return this->knob_hover_outline_thickness;
		}
		void set_knob_hover_outline_color(qsf::rgb color) {
			this->knob_hover_outline_color = color;
			if (this->is_hovering_over_knob()) {
				this->knob.set_outline_color(color);
			}
		}
		qsf::rgb get_knob_hover_outline_color() const {
			return this->knob_hover_outline_color;
		}

		void set_background_color(qsf::rgb color) {
			this->background_color = color;
			this->background_hover_color = color;
			this->background_hover_outline_color = color;
			this->background.set_color(color);
		}
		qsf::rgb get_background_color() const {
			return this->background_color;
		}
		void set_background_outline_thickness(qpl::f32 thickness) {
			this->background_outline_thickness = thickness;
			this->background.set_outline_thickness(thickness);
		}
		qpl::f32 get_background_outline_thickness() const {
			return this->background_outline_thickness;
		}
		void set_background_outline_color(qsf::rgb color) {
			this->background_outline_color = color;
			this->background_outline_thickness.set_outline_color(color);
		}
		qsf::rgb get_background_outline_color() const {
			return this->background_outline_color;
		}

		void set_background_hover_color(qsf::rgb color) {
			this->background_hover_color = color;
			if (this->is_hovering_over_background()) {
				this->background.set_color(color);
			}
		}
		qsf::rgb get_background_hover_color() const {
			return this->background_hover_color;
		}
		void set_background_hover_outline_thickness(qpl::f32 thickness) {
			this->background_hover_outline_thickness = thickness;
			if (this->is_hovering_over_background()) {
				this->background.set_outline_thickness(thickness);
			}
		}
		qpl::f32 get_background_hover_outline_thickness() const {
			return this->background_hover_outline_thickness;
		}
		void set_background_hover_outline_color(qsf::rgb color) {
			this->background_hover_outline_color = color;
			if (this->is_hovering_over_background()) {
				this->background.set_outline_color(color);
			}
		}
		qsf::rgb get_background_hover_outline_color() const {
			return this->background_hover_outline_color;
		}

		
		void set_hitbox_increase(qpl::f32 increase) {
			this->hitbox_increase = increase;
		}
		qpl::f32 get_hitbox_increase() const {
			return this->hitbox_increase;
		}

		void enable_smooth_input() {
			this->input_smooth_values = true;
		}
		void enable_smooth_input(sf::Keyboard::Key key) {
			this->input_smooth_values = true;
			this->smooth_input_key = key;
		}
		void disable_smooth_input() {
			this->input_smooth_values = false;
		}
		bool is_smooth_input_enabled() const {
			return this->input_smooth_values;
		}
		void set_smooth_input_multiply(qpl::f64 factor) {
			this->smooth_input_multiply = factor;
		}
		qpl::f64 get_smooth_input_multiply() const {
			return this->smooth_input_multiply;
		}

		void allow_click_on_background_to_skip() {
			this->click_on_background_to_skip = true;
		}
		void disallow_click_on_background_to_skip() {
			this->click_on_background_to_skip = false;
		}
		void is_click_on_background_to_skip_allowed() const {
			return this->click_on_background_to_skip;
		}

		bool is_hovering_over_background() const {
			return this->hovering_over_background;
		}
		bool is_hovering_over_knob() const {
			return this->hovering_over_knob;
		}

		void disallow_dragging() {
			this->allow_drag = false;
			this->dragging = false;
		}
		void allow_dragging() {
			this->allow_drag = true;
		}
		bool dragging_allowed() const {
			return this->allow_drag;
		}



		void update(const qsf::event_info& event) {
			auto pos = event.mouse_position();
			this->hovering_over_background = false;
			auto y_is = qpl::max(this->knob.get_dimension().y, this->background.get_dimension().y);
			this->hovering_over_background = this->background.contains(pos, qpl::vector2f(this->hitbox_increase, this->hitbox_increase + (y_is - this->background.get_dimension().y) / 2));

			this->value_modified = false;
			this->hovering_over_knob = this->knob.contains(pos, { this->hitbox_increase });

			if (this->background_hover_effect && this->hovering_over_background && !this->dragging) {
				this->background.set_color(this->background_hover_color);
				this->background.set_outline_color(this->background_hover_outline_color);
				this->background.set_outline_thickness(this->background_hover_outline_thickness);
			}
			else {
				this->background.set_color(this->background_color);
				this->background.set_outline_color(this->background_outline_color);
				this->background.set_outline_thickness(this->background_outline_thickness);
			}

			if (this->hovering_over_knob) {
				if (this->knob_hover_effect) {
					this->knob.set_color(this->knob_hover_color);
					this->knob.set_outline_color(this->knob_hover_outline_color);
					this->knob.set_outline_thickness(this->knob_hover_outline_thickness);
				}
				if (!this->dragging && event.left_mouse_clicked() && this->allow_drag) {
					this->dragging = true;
					this->click_pos_x = qpl::clamp(this->position.x, qpl::f32_cast(pos.x), this->position.x + this->range_pixel_width);
					this->click_knob_pos_x = pos.x - this->knob.get_position().x;
				}
			}
			else {
				if (this->click_on_background_to_skip && event.left_mouse_clicked() && this->allow_drag && this->hovering_over_background && (this->end != this->start)) {

					this->click_pos_x = qpl::clamp(this->position.x, qpl::f32_cast(pos.x), this->position.x + this->range_pixel_width);
					this->click_knob_pos_x = this->knob.get_dimension().x / 2;
					auto diff = (pos.x - this->position.x - this->click_knob_pos_x);
					auto progress = qpl::clamp_0_1(diff / this->range_pixel_width);

					if constexpr (qpl::is_integer<T>()) {
						*this->ptr = qpl::clamp(this->start, this->start + static_cast<T>((this->end - this->start) * progress + 0.5), this->end);
					}
					else {
						*this->ptr = qpl::clamp(this->start, this->start + static_cast<T>((this->end - this->start) * progress), this->end);
					}
					if (*this->ptr != this->value_before) {
						this->value_modified = true;
					}
					this->dragging = true;
				}

				if (this->knob_hover_effect && !this->dragging) {
					this->knob.set_color(this->knob_color);
					this->knob.set_outline_color(this->knob_outline_color);
					this->knob.set_outline_thickness(this->knob_outline_thickness);
				}
			}

			if (this->input_smooth_values && event.key_single_pressed(this->smooth_input_key)) {
				this->click_pos_x = qpl::clamp(this->position.x, qpl::f32_cast(pos.x), this->position.x + this->range_pixel_width);
			}

			if (!this->value_modified && this->dragging && this->allow_change && (this->end != this->start)) {
				qpl::f64 diff;
				if (this->input_smooth_values && event.key_holding(this->smooth_input_key)) {
					diff = ((pos.x - this->click_pos_x) * this->smooth_input_multiply) + (this->click_pos_x - this->position.x - this->click_knob_pos_x);
				}
				else {
					diff = (pos.x - this->position.x - this->click_knob_pos_x);
				}

				auto progress = qpl::clamp_0_1(diff / this->range_pixel_width);

				if constexpr (qpl::is_integer<T>()) {
					*this->ptr = qpl::clamp(this->start, this->start + static_cast<T>((this->end - this->start) * progress + 0.5), this->end);
				}
				else {
					*this->ptr = qpl::clamp(this->start, this->start + static_cast<T>((this->end - this->start) * progress), this->end);
				}
				if (*this->ptr != this->value_before) {
					this->value_modified = true;
				}
			}


			this->value_changed = false;
			if (this->value_before != *this->ptr) {
				this->set_knob_position();
				this->value_changed = true;
			}
			this->value_before = *this->ptr;

			this->released = false;
			if (event.left_mouse_released() && this->dragging) {
				this->released = true;
				this->dragging = false;
			}
		}

		void draw(qsf::draw_object& object) const {
			if (this->background_visible) object.draw(this->background);
			if (this->knob_visible) object.draw(this->knob);
		}

		qsf::rectangle background;
		qsf::rectangle knob;

		qpl::vector2f position;
		bool knob_visible = true;
		bool background_visible = true;
		bool knob_hover_effect = true;
		bool background_hover_effect = true;

		qsf::rgb knob_hover_color = qsf::rgb::unset;
		qpl::f32 knob_hover_outline_thickness = 4.0f;
		qsf::rgb knob_hover_outline_color = qsf::rgb::unset;

		qpl::f32 hitbox_increase = 5.0;

		qsf::rgb background_hover_color = qsf::rgb::unset;
		qpl::f32 background_hover_outline_thickness = 4.0f;
		qsf::rgb background_hover_outline_color = qsf::rgb::unset;

		T start = T{ 0 };
		T end = T{ 1 };
		T dummy = T{ 1 };
		T value_before = qpl::type_max<T>();

		T* ptr = nullptr;

		qsf::rgb background_color = qsf::rgb::unset;
		qpl::f32 background_outline_thickness = 0.0f;
		qsf::rgb background_outline_color = qsf::rgb::unset;

		qsf::rgb knob_color = qsf::rgb::unset;
		qpl::f32 knob_outline_thickness = 0.0f;
		qsf::rgb knob_outline_color = qsf::rgb::unset;

		sf::Keyboard::Key smooth_input_key = sf::Keyboard::LShift;
		qpl::f64 smooth_input_multiply = 0.1;

		qpl::f32 click_knob_pos_x = 0.0f;
		qpl::f32 click_pos_x = 0.0f;
		qpl::f32 range_pixel_width = 0.0f;


		bool hovering_over_background = false;
		bool click_on_background_to_skip = true;
		bool input_smooth_values = true;
		bool value_changed = false;
		bool value_modified = false;
		bool range_set = false;
		bool allow_change = true;
		bool allow_drag = true;
		bool dragging = false;
		bool released = false;
		bool hovering_over_knob = false;
	};

	struct border_graphic {
		const sf::Texture* texture;
		qpl::vector2f texture_dimension;
		qpl::vector2f dimension;
		qpl::vector2f position;
		qpl::vector2f scale = { 1, 1 };
		qsf::rgb color;
		std::vector<qsf::sprite> sprites;

		QPLDLL void set_dimension(qpl::vector2f dimension);
		QPLDLL void set_position(qpl::vector2f dimension);
		QPLDLL void increase(qpl::vector2f increase);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_scale(qpl::vector2f scale);
		QPLDLL void set_texture(const sf::Texture& texture);
		QPLDLL void check_texture();
		QPLDLL void update_dimensions(qpl::vector2f position, qpl::vector2f dimension);
		QPLDLL void move(qpl::vector2f delta);
		QPLDLL void add_top();
		QPLDLL void add_bottom();
		QPLDLL void add_left();
		QPLDLL void add_right();
		QPLDLL void make_all_sides();
		QPLDLL void draw(qsf::draw_object& object) const;
	};

	namespace detail {
		QPLDLL extern std::unordered_map<std::string, qsf::text> texts;
	}

	QPLDLL qsf::text& get_text(const std::string& name = "QSF_DEFAULT");
	QPLDLL void add_text(const std::string& name = "QSF_DEFAULT");

}

#endif
#endif