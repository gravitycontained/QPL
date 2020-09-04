#ifndef QPLSF_DRAWABLES_HPP
#define QPLSF_DRAWABLES_HPP
#pragma once

#if defined(QPL_USE_SFML) || defined(QPL_USE_ALL)
#include <qpl/qpldeclspec.hpp>
#include <qpl/QSF/vector.hpp>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <qpl/QSF/color.hpp>
#include <qpl/string.hpp>
#include <functional>
#include <string>

/*
void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;
*/

namespace qsf {
	struct vtext;
	struct text;
	struct vrectangle;
	struct rectangle;
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

	namespace detail {
		QPLDLL extern qsf::text text;
		QPLDLL extern qsf::rectangle rectangle;
		QPLDLL extern qsf::point point;
		QPLDLL extern qsf::points points;
		QPLDLL extern qsf::circle circle;
		QPLDLL extern qsf::circles circles;
		QPLDLL extern qsf::line line;
		QPLDLL extern qsf::lines lines;
		QPLDLL extern qsf::thick_line thick_line;
		QPLDLL extern qsf::thick_lines thick_lines;
		QPLDLL extern qsf::graph graph;
	}
	QPLDLL extern qsf::vgraph drawing_graph;

	struct vtext : public sf::Drawable {
		QPLDLL void set_font(const std::string& font_name);
		QPLDLL void set_style(qpl::u32 style);
		QPLDLL void set_character_size(qpl::u32 character_size);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_outline_thickness(qpl::f32 outline_thickness);
		QPLDLL void set_outline_color(qsf::rgb color);
		QPLDLL void set_letter_spacing(qpl::f32 spacing);
		QPLDLL void set_position(qsf::vector2f position);
		QPLDLL void set_string(const std::string& string);
		QPLDLL bool operator==(const vtext& other) const;


		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;

		std::string font_name;
		qpl::u32 style = sf::Text::Style::Regular;
		qpl::u32 character_size = 15u;
		qsf::rgb color = qsf::rgb::black;
		qpl::f32 outline_thickness;
		qsf::rgb outline_color;
		qpl::f32 letter_spacing;
		qsf::vector2f position;
		std::string string;
	};

	struct text : public sf::Drawable {
		QPLDLL void set_font(const std::string& font_name);
		QPLDLL void set_style(qpl::u32 style);
		QPLDLL void set_character_size(qpl::u32 character_size);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_outline_thickness(qpl::f32 outline_thickness);
		QPLDLL void set_outline_color(qsf::rgb color);
		QPLDLL void set_letter_spacing(qpl::f32 spacing);
		QPLDLL void set_position(qsf::vector2f position);
		QPLDLL void set_string(const std::string& string);
		QPLDLL void centerize();
		QPLDLL void centerize_x();
		QPLDLL void centerize_y();

		qsf::text() {

		}
		qsf::text(const qsf::vtext& other) {
			*this = other;
		}

		QPLDLL qsf::vrectangle hitbox() const;

		QPLDLL std::string string() const;
		QPLDLL void clear();
		QPLDLL qsf::text& operator<<(const std::string& string);

		QPLDLL qsf::text& operator=(const qsf::vtext& text);

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;



		std::string m_string;
		sf::Text m_text;
	};


	struct vrectangle : public sf::Drawable {
		QPLDLL void set_dimension(qsf::vector2f dimension);
		QPLDLL void set_position(qsf::vector2f position);
		QPLDLL void set_center(qsf::vector2f position);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_outline_thickness(qpl::f32 outline_thickness);
		QPLDLL void set_outline_color(qsf::rgb outline_color);

		QPLDLL qsf::vector2f center() const;

		QPLDLL std::string string() const;

		QPLDLL void increase(qpl::f32 delta);
		QPLDLL qsf::vrectangle increased(qpl::f32 delta) const;

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;

		qsf::vector2f dimension;
		qsf::vector2f position;
		qsf::rgb color;
		qpl::f32 outline_thickness;
		qsf::rgb outline_color;
	};

	struct rectangle : public sf::Drawable {
		QPLDLL void set_dimension(qsf::vector2f dimension);
		QPLDLL void set_position(qsf::vector2f position);
		QPLDLL void set_center(qsf::vector2f position);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_outline_thickness(qpl::f32 outline_thickness);
		QPLDLL void set_outline_color(qsf::rgb outline_color);

		QPLDLL qsf::vector2f position() const;
		QPLDLL qsf::vector2f dimension() const;

		QPLDLL qsf::vlines as_lines() const;
		QPLDLL qsf::vlines as_lines_completed() const;

		QPLDLL qsf::vector2f center() const;

		QPLDLL qsf::rectangle& operator=(const qsf::vrectangle& rectangle);

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;

		sf::RectangleShape m_rect;
	};

	struct vpoint : public sf::Drawable {
		qsf::vector2f position;
		qsf::rgb color;

		vpoint() {

		}

		vpoint(qsf::vector2f position, qsf::rgb color) {
			this->position = position;
			this->color = color;
		}

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;
		QPLDLL vpoint& operator=(qsf::vector2f position);

		QPLDLL bool operator==(const vpoint& other) const;
		QPLDLL bool operator!=(const vpoint& other) const;
		//QPLDLL vpoint& with_color(qsf::rgb color);
	};
	struct point : public sf::Drawable {
		sf::Vertex vertex;

		QPLDLL qsf::point& operator=(const qsf::vpoint& point);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;
	};

	struct vpoints : public sf::Drawable {
		vpoints(qpl::size size = qpl::size{}) {
			this->points.resize(size);
		}

		QPLDLL void add_point(qsf::vpoint point);
		QPLDLL void add_point(qsf::vector2f position, qsf::rgb color);
		QPLDLL qsf::vlines as_lines() const;

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;
		QPLDLL void clear();
		QPLDLL qpl::size size() const;

		std::vector<qsf::vpoint> points;
	};

	struct points : public sf::Drawable {
		points() {
			this->vertices.setPrimitiveType(sf::PrimitiveType::Points);
		}
		points(qpl::size size) {
			this->vertices.resize(size);
		}

		QPLDLL qsf::vlines as_lines() const;
		QPLDLL qsf::points& operator=(const qsf::vpoints& points);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;
		QPLDLL void clear();
		QPLDLL qpl::size size() const;
		sf::VertexArray vertices;
	};



	struct vcircle : public sf::Drawable {
		vcircle() {
			this->radius = 0.0f;
		}
		vcircle(qsf::vpoint point, qpl::f32 radius) {
			this->point = point;
			this->radius = radius;
			this->point.position.move(qsf::vector2f{ -this->radius, -this->radius });
		}
		vcircle(qsf::vector2f center, qpl::f32 radius, qsf::rgb color) {
			this->point.position = center - qsf::vector2f{ radius, radius };
			this->point.color = color;
			this->radius = radius;
		}

		QPLDLL void set_position(qsf::vector2f center);
		QPLDLL void set_radius(qpl::f32 radius);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_center(qsf::vector2f center);
		QPLDLL void centerize();
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;

		qsf::vpoint point;
		qpl::f32 radius;
	};
	struct circle : public sf::Drawable {
		circle() {

		}
		circle(const qsf::vcircle& circle) {
			*this = circle;
		}
		circle(const qsf::circle& circle) {
			*this = circle;
		}

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;
		QPLDLL qsf::circle& operator=(const qsf::vcircle& circle);
		QPLDLL qsf::circle& operator=(const qsf::circle& circle);

		sf::CircleShape circle_shape;
	};

	struct vcircles : public sf::Drawable {
		QPLDLL void add_circle(qsf::vpoint point, qpl::f32 radius, qsf::rgb color);
		QPLDLL void add_circle(qsf::vector2f position, qpl::f32 radius, qsf::rgb color);

		QPLDLL qpl::size size() const;
		QPLDLL void clear();

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;

		std::vector<qsf::vcircle> circles;
	};

	struct circles : public sf::Drawable {
		circles() {

		}
		circles(const qsf::vcircles& circles) {
			*this = circles;
		}
		circles(const qsf::circles& circles) {
			*this = circles;
		}

		QPLDLL void add_circle(const qsf::vcircle& circle);
		QPLDLL void add_circle(qsf::vpoint point, qpl::f32 radius, qsf::rgb color);
		QPLDLL void add_circle(qsf::vector2f position, qpl::f32 radius, qsf::rgb color);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;
		QPLDLL qsf::circles& operator=(const qsf::vcircles& circles);
		QPLDLL qsf::circles& operator=(const qsf::circles& circles);

		QPLDLL qpl::size size() const;
		QPLDLL void clear();

		std::vector<qsf::circle> circles_;
	};

	struct vline : public sf::Drawable {


		QPLDLL void set_a(qsf::vpoint point);
		QPLDLL void set_a(qsf::vector2f position);
		QPLDLL void set_b(qsf::vpoint point);
		QPLDLL void set_b(qsf::vector2f position);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_a_color(qsf::rgb color);
		QPLDLL void set_b_color(qsf::rgb color);

		QPLDLL qsf::vector2f normal() const;
		QPLDLL qpl::f32 length() const;
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;

		//pi radians (0 - 2 pi)
		qpl::f32 rotation() const;
		qsf::vline& rotate_around_a(qpl::f64 degree);
		qsf::vline& rotate_around_b(qpl::f64 degree);

		vpoint a;
		vpoint b;
	};

	struct line : public sf::Drawable {
		QPLDLL void set_a(qsf::vpoint point);
		QPLDLL void set_a(qsf::vector2f position);
		QPLDLL void set_b(qsf::vpoint point);
		QPLDLL void set_b(qsf::vector2f position);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_a_color(qsf::rgb color);
		QPLDLL void set_b_color(qsf::rgb color);

		QPLDLL qsf::vector2f normal() const;
		QPLDLL qpl::f32 length() const;
		qpl::f32 rotation() const;
		qsf::vline& rotate_a(qpl::f64 degree);
		qsf::vline& rotate_b(qpl::f64 degree);

		QPLDLL qsf::line& operator=(const qsf::vline& line);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;
		std::array<sf::Vertex, 2> vertices;
	};

	struct vlines : public sf::Drawable {
		QPLDLL void clear();
		QPLDLL qpl::size size() const;
		QPLDLL void add_point(qsf::vpoint point);
		QPLDLL void add_point(qsf::vector2f position, qsf::rgb color = qsf::rgb::white);
		QPLDLL void complete();

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;

		std::vector<qsf::vpoint> points;
	};

	struct lines : public sf::Drawable {
		lines() {
			this->vertices.setPrimitiveType(sf::LinesStrip);
		}
		lines(const qsf::vlines& lines) {
			this->vertices.setPrimitiveType(sf::LinesStrip);
			*this = lines;
		}

		QPLDLL void complete();

		QPLDLL qsf::lines& operator=(const qsf::vlines& lines);
		QPLDLL void clear();
		QPLDLL qpl::size size() const;
		QPLDLL void add_point(qsf::vpoint point);
		QPLDLL void add_point(qsf::vector2f position, qsf::rgb color = qsf::rgb::white);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;

		sf::VertexArray vertices;
	};

	struct vthick_line : sf::Drawable {

		QPLDLL void set_a(qsf::vpoint point);
		QPLDLL void set_a(qsf::vector2f position);
		QPLDLL void set_b(qsf::vpoint point);
		QPLDLL void set_b(qsf::vector2f position);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_a_color(qsf::rgb color);
		QPLDLL void set_b_color(qsf::rgb color);

		QPLDLL qsf::vpoints as_points() const;

		QPLDLL qsf::vector2f normal() const;
		QPLDLL qpl::f32 length() const;
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;

		qsf::vpoint a;
		qsf::vpoint b;
		qpl::f32 thickness = 1.0f;
	};

	struct thick_line : sf::Drawable {

		QPLDLL void set_a(qsf::vpoint point, qpl::f32 thickness = 1.0f);
		QPLDLL void set_a(qsf::vector2f position, qpl::f32 thickness = 1.0f);
		QPLDLL void set_b(qsf::vpoint point, qpl::f32 thickness = 1.0f);
		QPLDLL void set_b(qsf::vector2f position, qpl::f32 thickness = 1.0f);
		QPLDLL void set_color(qsf::rgb color);
		QPLDLL void set_a_color(qsf::rgb color);
		QPLDLL void set_b_color(qsf::rgb color);

		QPLDLL qsf::vpoints as_points() const;
		QPLDLL qsf::vector2f normal() const;
		QPLDLL qpl::f32 length() const;
		QPLDLL qsf::thick_line& operator=(const qsf::vthick_line& line);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;

		std::array<sf::Vertex, 4> vertices;
	};
	struct vthick_lines : sf::Drawable {
		QPLDLL void add_thick_line(qsf::vpoint point);
		QPLDLL void add_thick_line(qsf::vector2f position, qsf::rgb color);
		QPLDLL void set_thickness(qpl::f32 thickness);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;
		QPLDLL void clear();
		QPLDLL qpl::size size() const;

		qsf::vpoints points;
		qpl::f32 thickness = 1.0f;
	};
	struct thick_lines : sf::Drawable {
		thick_lines() {
			this->vertices.setPrimitiveType(sf::PrimitiveType::Quads);
		}
		QPLDLL void add_thick_line(qsf::vpoint point, qpl::f32 thickness);
		QPLDLL void add_thick_line(qsf::vector2f position, qsf::rgb color, qpl::f32 thickness);
		QPLDLL qsf::thick_lines& operator=(const qsf::vthick_lines& lines);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;
		QPLDLL void clear();
		QPLDLL qpl::size size() const;

		sf::VertexArray vertices;
	};

	struct pixel_image : sf::Drawable {
		pixel_image() {
			this->vertices.setPrimitiveType(sf::PrimitiveType::Quads);
		}

		QPLDLL void set_array_dimension(qsf::vector2u dimension);
		QPLDLL void set_pixel_dimension(qsf::vector2f dimension);
		QPLDLL void set_position(qsf::vector2f position);
		QPLDLL void create_positions();

		QPLDLL void set(qpl::size x, qpl::size y, qsf::rgb color);
		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;


		qsf::vector2u array_dimension;
		qsf::vector2f pixel_dimension;
		qsf::vector2f position;
		sf::VertexArray vertices;
		bool positions_set = false;
	};

	struct vgraph : sf::Drawable {
		
		vgraph() {
			this->hitbox.set_dimension({ 300, 200 });
			this->hitbox.set_position({ 0,0 });
			this->x_axis_string_function = [](qpl::size index) {
				return qpl::to_string(index);
			};
		}

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;


		struct data_point {


			qpl::f64 data = 0.0;
			qpl::f64 thickness = qpl::f64_min;
			qsf::frgb color = qsf::rgb::unset;

			qsf::vcircle circle;
			qsf::vtext text;

			data_point() {
				this->thickness = qpl::f64_min;
				this->color = qsf::frgb::unset;
			}
			data_point(const data_point& other) {
				*this = other;
			}
			QPLDLL data_point& operator=(const data_point& other);

			QPLDLL data_point operator*(qpl::f64 n) const;
			QPLDLL data_point operator*(data_point other) const;
			QPLDLL data_point operator-(data_point other) const;
			QPLDLL data_point operator+(data_point other) const;
			QPLDLL bool operator<(data_point other) const;
			QPLDLL bool operator>(data_point other) const;
		};

		qpl::f64 min_value = qpl::f64_min;
		qpl::f64 max_value = qpl::f64_max;

		qpl::f64 height_delta = 0.1;
		qpl::f64 x_interpolation_offset = 0.0;

		qpl::f64 y_axis_text_space = 5.0;
		qpl::size y_axis_line_count = 10u;
		qsf::vtext y_axis_text;
		bool y_axis_text_left = true;
		bool y_axis_text_percent = false;
		qpl::size y_axis_text_precision = 5u;
		bool use_y_axis = false;

		qpl::f64 x_axis_text_space = 5.0;
		qpl::size x_axis_line_frequency = 1u;
		qsf::vtext x_axis_text;
		bool x_axis_text_left = true;
		bool x_axis_text_percent = false;
		qpl::size x_axis_text_precision = 5u;

		qpl::size x_data_steps = 1u;
		qpl::size x_data_max_on_axis = 300u;

		bool use_x_axis = false;
		std::function<std::string(qpl::size)> x_axis_string_function;

		qsf::rgb axis_line_color = qsf::rgb(30, 30, 30);
		qpl::f64 axis_thickness = 1.5f;

		enum class interpolation_type : qpl::u32 {
			unset = 0, 
			linear = 1,
			cubic = 2,
		};

		qsf::rgb color = qsf::rgb::white;
		qpl::f64 thickness = 1.0f;
		interpolation_type interpolation = interpolation_type::linear;
		qpl::size interpolation_steps = 1u;


		struct graph_line {
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
			QPLDLL data_point& operator[](qpl::size index);
			QPLDLL const data_point& operator[](qpl::size index) const;

			QPLDLL std::vector<data_point>::iterator begin();
			QPLDLL std::vector<data_point>::const_iterator begin() const;
			QPLDLL std::vector<data_point>::const_iterator cbegin();

			QPLDLL std::vector<data_point>::iterator end();
			QPLDLL std::vector<data_point>::const_iterator end() const;
			QPLDLL std::vector<data_point>::const_iterator cend();

			interpolation_type interpolation = interpolation_type::unset;
			qsf::rgb color = qsf::rgb::unset;
			qpl::f64 thickness = qpl::f64_min;
			qpl::size interpolation_steps = qpl::size_max;
			std::vector<data_point> data;
		};
;
		QPLDLL std::pair<data_point, data_point> get_low_high() const;
		QPLDLL qpl::size size() const;
		QPLDLL graph_line& operator[](const std::string& name);

		QPLDLL std::unordered_map<std::string, graph_line>::iterator begin();
		QPLDLL std::unordered_map<std::string, graph_line>::const_iterator begin() const;
		QPLDLL std::unordered_map<std::string, graph_line>::const_iterator cbegin();

		QPLDLL std::unordered_map<std::string, graph_line>::iterator end();
		QPLDLL std::unordered_map<std::string, graph_line>::const_iterator end() const;
		QPLDLL std::unordered_map<std::string, graph_line>::const_iterator cend();


		qsf::vrectangle hitbox;
		std::unordered_map<std::string, graph_line> graphs;

		qsf::rgb background_color = qsf::rgb::transparent;
	};

	struct graph : sf::Drawable {

		QPLDLL void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;
		QPLDLL qsf::graph& operator=(const qsf::vgraph& graph);


		std::vector<std::vector<qsf::text>> circle_texts;
		std::vector<qsf::circles> circles;
		std::vector<qsf::thick_lines> y_lines;
		std::vector<qsf::text> y_texts;
		std::vector<qsf::thick_lines> x_lines;
		std::vector<qsf::text> x_texts;

		std::vector<qsf::thick_lines> lines;

		qsf::rectangle background;
	};

	struct vbutton : sf::Drawable {

		QPLDLL void set_dimension(qsf::vector2f dimension);
		QPLDLL void set_position(qsf::vector2f position);
		QPLDLL void set_center(qsf::vector2f center);
		QPLDLL void set_background_color(qsf::rgb color);
		QPLDLL void set_text_color(qsf::rgb color);
		QPLDLL void set_text_font(std::string font);
		QPLDLL void set_text_character_size(qpl::u32 character_size);
		QPLDLL void set_text(std::string text);
		QPLDLL void centerize_text();

		qsf::vtext text;
		qsf::vrectangle background;
	};

	namespace detail {
		QPLDLL extern std::unordered_map<std::string, qsf::text> texts;
		QPLDLL extern std::unordered_map<std::string, qsf::rectangle> rectangles;
	}

	QPLDLL qsf::text& get_text(const std::string& name = "QSF_DEFAULT");
	QPLDLL qsf::rectangle& get_rectangle(const std::string& name = "QSF_DEFAULT");


}

#endif
#endif