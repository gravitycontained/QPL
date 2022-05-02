#include <qpl/QSF/drawables.hpp>

#if !defined (QPL_NO_SFML) || defined(QPL_USE_ALL)

#include <qpl/QSF/resources.hpp>
#include <qpl/QSF/event_info.hpp>
#include <qpl/QSF/utility.hpp>
#include <qpl/algorithm.hpp>

#include <qpl/time.hpp>

#include <qpl/system.hpp>
#include <qpl/memory.hpp>

namespace qsf {

	qpl::hitbox qsf::get_text_hitbox(const sf::Text& text, bool ignore_outline) {
		qpl::hitbox rectangle;
		auto local_bounds = text.getLocalBounds();
		auto global_bounds = text.getGlobalBounds();

		rectangle.set_position({ global_bounds.left, global_bounds.top });
		rectangle.set_dimension({ local_bounds.width, local_bounds.height });

		if (ignore_outline) {
			rectangle.position -= text.getOutlineThickness();
			rectangle.dimension -= text.getOutlineThickness() * 2;
		}
		return rectangle;
	}
	qpl::hitbox qsf::get_text_hitbox(const qsf::text& text, bool ignore_outline) {
		return text.get_visible_hitbox();
	}
	void qsf::centerize_text(sf::Text& text) {
		auto rect = qsf::get_text_hitbox(text).dimension;
		text.move(-rect / 2);
	}
	void qsf::centerize_text(qsf::text& text) {
		text.centerize();
	}

	qpl::hitbox qsf::get_sprite_hitbox(const sf::Sprite& sprite) {

		qpl::hitbox rectangle;
		auto local_bounds = sprite.getLocalBounds();
		auto global_bounds = sprite.getGlobalBounds();

		rectangle.set_position({ global_bounds.left, global_bounds.top });
		rectangle.set_dimension(qpl::vec(local_bounds.width, local_bounds.height) * qpl::vector2f(sprite.getScale()));

		return rectangle;
	}
	qpl::vector2f qsf::get_sprite_center(const sf::Sprite& sprite) {
		auto hitbox = qsf::get_sprite_hitbox(sprite);
		return hitbox.position + hitbox.dimension / 2;
	}
	void qsf::centerize_sprite_origin(sf::Sprite& sprite) {
		auto local_bounds = sprite.getLocalBounds();
		auto dim = qpl::vec(local_bounds.width, local_bounds.height);
		sprite.setOrigin(dim / 2);
	}
	void qsf::centerize_sprite(sf::Sprite& sprite) {
		auto hitbox = qsf::get_sprite_hitbox(sprite);
		sprite.setPosition(hitbox.position - hitbox.dimension / 2);
	}


	namespace detail {
		qsf::text qsf::detail::text;
		qsf::rectangle qsf::detail::rectangle;
		qsf::rectangles qsf::detail::rectangles;
		qsf::smooth_rectangle qsf::detail::smooth_rectangle;
		qsf::point qsf::detail::point;
		qsf::points qsf::detail::points;
		qsf::circle qsf::detail::circle;
		qsf::circles qsf::detail::circles;
		qsf::line qsf::detail::line;
		qsf::lines qsf::detail::lines;
		qsf::thick_line qsf::detail::thick_line;
		qsf::thick_lines qsf::detail::thick_lines;
		qsf::graph qsf::detail::graph;
		qsf::button qsf::detail::button;
	}


	void qsf::vertex::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		window.draw(reinterpret_cast<const sf::Vertex*>(this), 1, sf::PrimitiveType::Points, states);
	}


	void qsf::vertex_array::set_primitive_type(qsf::primitive_type primitive_type) {
		this->primitive_type = primitive_type;
	}
	void qsf::vertex_array::set_primitive_type(sf::PrimitiveType primitive_type) {
		this->primitive_type = static_cast<qsf::primitive_type>(primitive_type);
	}

	qpl::size qsf::vertex_array::size() const {
		return this->vertices.size();
	}
	bool qsf::vertex_array::empty() const {
		return this->vertices.empty();
	}
	void qsf::vertex_array::resize(qpl::size new_size) {
		this->vertices.resize(new_size);
	}
	void qsf::vertex_array::reserve(qpl::size new_size) {
		this->vertices.reserve(new_size);
	}
	void qsf::vertex_array::add(const qsf::vertex& vertex) {
		this->vertices.push_back(vertex);
	}
	void qsf::vertex_array::clear() {
		this->vertices.clear();
	}
	qsf::vertex& qsf::vertex_array::operator[](qpl::size index) {
		return this->vertices[index];
	}
	const qsf::vertex& qsf::vertex_array::operator[](qpl::size index) const {
		return this->vertices[index];
	}

	qsf::vertex& qsf::vertex_array::front() {
		return this->vertices.front();
	}
	const qsf::vertex& qsf::vertex_array::front() const {
		return this->vertices.front();
	}

	qsf::vertex& qsf::vertex_array::back() {
		return this->vertices.back();
	}
	const qsf::vertex& qsf::vertex_array::back() const {
		return this->vertices.back();
	}

	std::vector<qsf::vertex>::iterator qsf::vertex_array::begin() {
		return this->vertices.begin();
	}
	std::vector<qsf::vertex>::const_iterator qsf::vertex_array::begin() const {
		return this->vertices.begin();
	}
	std::vector<qsf::vertex>::const_iterator qsf::vertex_array::cbegin() const {
		return this->vertices.cbegin();
	}

	std::vector<qsf::vertex>::iterator qsf::vertex_array::end() {
		return this->vertices.end();
	}
	std::vector<qsf::vertex>::const_iterator qsf::vertex_array::end() const {
		return this->vertices.end();
	}
	std::vector<qsf::vertex>::const_iterator qsf::vertex_array::cend() const {
		return this->vertices.cend();
	}

	void qsf::vertex_array::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		window.draw(reinterpret_cast<const sf::Vertex*>(this->vertices.data()), this->size(), static_cast<sf::PrimitiveType>(this->primitive_type), states);
	}


	void qsf::vtext::set_font(const std::string& font_name) {
		this->font_name = font_name;
	}
	void qsf::vtext::set_style(qpl::u32 style) {
		this->style = style;
	}
	void qsf::vtext::set_character_size(qpl::u32 character_size) {
		this->character_size = character_size;
	}
	void qsf::vtext::set_color(qpl::rgb color) {
		this->color = color;
	}
	void qsf::vtext::set_outline_thickness(qpl::f32 outline_thickness) {
		this->outline_thickness = outline_thickness;
	}
	void qsf::vtext::set_outline_color(qpl::rgb color) {
		this->color = color;
	}
	void qsf::vtext::set_letter_spacing(qpl::f32 spacing) {
		this->letter_spacing = spacing;
	}
	void qsf::vtext::set_position(qpl::vector2f position) {
		this->position = position;
	}
	void qsf::vtext::set_string(const std::string& string) {
		this->string = string;
	}
	bool qsf::vtext::operator==(const vtext& other) const {
		return
			this->character_size == other.character_size &&
			this->font_name == other.font_name &&
			this->style == other.style &&
			this->character_size == other.character_size &&
			this->color == other.color &&
			this->outline_thickness == other.outline_thickness &&
			this->outline_color == other.outline_color &&
			this->letter_spacing == other.letter_spacing &&
			this->position == other.position &&
			this->string == other.string &&
			this->multiplied_color == other.multiplied_color;
	}
	void qsf::vtext::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::text = *this;
		qsf::detail::text.draw(window, states);
	}

	const sf::Font& qsf::text::get_sf_font() const {
		return qsf::get_font(this->get_font());
	}
	const sf::Glyph& qsf::text::get_glyph(qpl::wchar_type c) const {
		return this->get_sf_font().getGlyph(c, this->get_character_size(), this->is_bold(), this->get_outline_thickness());
	}

	qpl::f32 qsf::text::get_next_advance(qpl::wchar_type c) const {

		auto x = this->get_standard_hitbox().dimension.x;
		auto w = this->get_wstring();
		if (w.length()) {
			auto cw = w[w.length() - 1];
			auto special = (cw == L'\t' || cw == L'\n');
			if (!special) {

				auto width = this->get_letter_width(cw);
				auto advance = this->get_letter_advance(cw);
				auto spaced_advance = this->get_letter_advance_and_spacing(cw);
				auto kerning = this->get_letter_kerning(c);

				auto diff = spaced_advance - width;

				if (w.length() > 1) {
					auto before_left = this->get_glyph(w[w.length() - 2]).bounds.left;
					auto left_diff = before_left - this->get_glyph(cw).bounds.left;
					diff += left_diff;
				}

				x += diff;
			}
		}
		return x;
	}
	qpl::f32 qsf::text::get_line_spacing() const {
		return this->get_sf_font().getLineSpacing(this->get_character_size()) * this->m_text.getLineSpacing();
	}
	bool qsf::text::is_bold() const {
		return this->get_style() == sf::Text::Style::Bold;
	}
	std::string qsf::text::get_font() const {
		return this->m_font;
	}
	qpl::u32 qsf::text::get_style() const {
		return this->m_text.getStyle();
	}
	qpl::u32 qsf::text::get_character_size() const {
		return this->m_text.getCharacterSize();
	}
	qpl::rgb qsf::text::get_color() const {
		return this->m_text.getFillColor();
	}
	qpl::f32 qsf::text::get_outline_thickness() const {
		return this->m_text.getOutlineThickness();
	}
	qpl::rgb qsf::text::get_outline_color() const {
		return this->m_text.getOutlineColor();
	}
	qpl::f32 qsf::text::get_letter_spacing() const {
		return this->m_text.getLetterSpacing();
	}
	qpl::vector2f qsf::text::get_position() const {
		return this->m_text.getPosition();
	}
	qpl::vector2f qsf::text::get_center() const {
		return this->get_standard_hitbox().get_center();
	}
	std::string qsf::text::get_string() const {
		return this->m_string;
	}
	std::wstring qsf::text::get_wstring() const {
		auto s = this->m_text.getString();
		return s.toWideString();
	}
	void qsf::text::set_font(const sf::Font& font) {
		this->m_text.setFont(font);
	}
	void qsf::text::set_font(const std::string& font_name) {
		this->m_font = font_name;
		if (this->m_font.empty()) return;
		if (qsf::find_font(font_name)) {
			this->m_text.setFont(qsf::get_font(font_name));
		}
		else {
			qpl::println("couldn't find font \"", font_name, "\"");
		}
	}
	void qsf::text::set_style(qpl::u32 style) {
		this->m_text.setStyle(style);
	}
	void qsf::text::set_character_size(qpl::u32 character_size) {
		this->m_text.setCharacterSize(character_size);
	}
	void qsf::text::set_color(qpl::rgb color) {
		this->color = color;
		this->m_text.setFillColor(this->color.multiplied_color(this->multiplied_color));
	}
	void qsf::text::set_outline_thickness(qpl::f32 outline_thickness) {
		this->m_text.setOutlineThickness(outline_thickness);
	}
	void qsf::text::set_outline_color(qpl::rgb color) {
		this->outline_color = color;
		this->m_text.setOutlineColor(this->outline_color.multiplied_color(this->multiplied_color));
	}
	void qsf::text::set_rotation(qpl::f32 angle) {
		this->m_text.setRotation(angle);
	}
	void qsf::text::set_letter_spacing(qpl::f32 spacing) {
		this->m_text.setLetterSpacing(spacing);
	}
	void qsf::text::set_position(qpl::vector2f position) {
		this->m_text.setPosition(position);
	}
	void qsf::text::set_center(qpl::vector2f position) {
		this->set_position(position);
		this->centerize();
	}
	void qsf::text::set_string(const std::string& string) {
		this->m_string = string;
		sf::String s = this->m_string.c_str();
		this->m_text.setString(s);
	}
	void qsf::text::set_string(const std::wstring& string) {
		this->m_string = qpl::wstring_to_string(string);
		sf::String s = string.c_str();
		this->m_text.setString(s);
	}
	void qsf::text::set_multiplied_color(qpl::rgb color) {
		this->multiplied_color = color;
		this->m_text.setFillColor(this->color.multiplied_color(color));
		this->m_text.setOutlineColor(this->outline_color.multiplied_color(color));
	}
	void qsf::text::set_multiplied_alpha(qpl::u8 alpha) {
		this->set_multiplied_color(qpl::rgb::white.with_alpha(alpha));
	}
	qpl::rgb qsf::text::get_multiplied_color() const {
		return this->multiplied_color;
	}
	void qsf::text::centerize() {
		auto pos = this->m_text.getPosition();
		sf::FloatRect textRect = this->m_text.getLocalBounds();
		this->m_text.setOrigin(textRect.left + textRect.width / 2.0f,
			textRect.top + textRect.height / 2.0f);
		this->m_text.setPosition(pos);
	}
	void qsf::text::centerize_x() {
		auto pos = this->m_text.getPosition();
		sf::FloatRect textRect = this->m_text.getLocalBounds();
		this->m_text.setOrigin(textRect.left + textRect.width / 2.0f, 0.0f);
		this->m_text.setPosition(pos);
	}
	void qsf::text::centerize_y() {
		auto pos = this->m_text.getPosition();
		sf::FloatRect textRect = this->m_text.getLocalBounds();
		this->m_text.setOrigin(0.0f, textRect.top + textRect.height / 2.0f);
		this->m_text.setPosition(pos);
	}
	void qsf::text::move(qpl::vector2f delta) {
		this->m_text.move(delta);
	}
	qpl::hitbox qsf::text::get_visible_hitbox(bool ignore_outline) const {

		qpl::hitbox rectangle;
		auto local_bounds = this->m_text.getLocalBounds();
		auto global_bounds = this->m_text.getGlobalBounds();

		rectangle.set_position({ global_bounds.left, global_bounds.top });
		rectangle.set_dimension({ local_bounds.width, local_bounds.height });

		if (ignore_outline) {
			rectangle.position -= this->m_text.getOutlineThickness();
			rectangle.dimension -= this->m_text.getOutlineThickness() * 2;
		}
		return rectangle;
	}
	qpl::hitbox qsf::text::get_standard_hitbox() const {
		qpl::hitbox rectangle;
		auto local_bounds = this->m_text.getLocalBounds();

		rectangle.set_position(this->m_text.getPosition());
		rectangle.set_dimension({ local_bounds.width, local_bounds.height });

		return rectangle;
	}
	qpl::vector2f qsf::text::get_offset() const {
		auto global_bounds = this->m_text.getGlobalBounds();
		return qpl::vec(global_bounds.left, global_bounds.top) - this->get_position();
	}

	std::string qsf::text::string() const {
		return this->m_string;
	}

	void qsf::text::clear() {
		this->m_string.clear();
		sf::String s = this->m_string.c_str();
		this->m_text.setString(s);
	}
	qsf::text& qsf::text::operator<<(const std::string& string) {
		this->m_string.append(string);
		sf::String s = this->m_string.c_str();
		this->m_text.setString(s);
		return *this;
	}
	qsf::text& qsf::text::operator<<(const sf::String& string) {
		this->m_string.append(string.toAnsiString());
		this->m_text.setString(string);
		return *this;
	}
	qsf::text& qsf::text::operator=(const qsf::vtext& text) {
		this->multiplied_color = qpl::rgb::white;
		this->set_character_size(text.character_size);
		this->set_color(text.color);
		this->set_letter_spacing(text.letter_spacing);
		this->set_outline_color(text.outline_color);
		this->set_outline_thickness(text.outline_thickness);
		this->set_position(text.position);
		this->set_style(text.style);
		this->set_string(text.string);
		this->m_font = text.font_name;
		this->set_font(text.font_name);
		this->set_multiplied_color(text.multiplied_color);
		return *this;
	}

	void qsf::text::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		window.draw(this->m_text, states);
	}

	qpl::f32 qsf::text::get_letter_spacing_pixels() const {
		float white_width = this->get_sf_font().getGlyph(L' ', this->get_character_size(), this->is_bold(), this->get_outline_thickness()).advance;
		float letter_spacing = (white_width / 3.f) * (this->get_letter_spacing() - 1.f);
		return letter_spacing;
	}
	qpl::f32 qsf::text::get_letter_kerning(qpl::wchar_type c) const {
		auto w = this->get_wstring();
		if (!w.length()) {
			return 0.0f;
		}
		else {
			auto w = this->get_wstring();
			auto wc = w[w.length() - 1];
			return this->get_sf_font().getKerning(c, wc, this->get_character_size());
		}
	}
	qpl::f32 qsf::text::get_letter_advance(qpl::wchar_type c) const {
		return this->get_sf_font().getGlyph(c, this->get_character_size(), this->is_bold(), this->get_outline_thickness()).advance;
	}
	qpl::f32 qsf::text::get_letter_advance_and_spacing(qpl::wchar_type c) const {
		return this->get_letter_advance(c) + this->get_letter_spacing_pixels();
	}
	qpl::f32 qsf::text::get_letter_width(qpl::wchar_type c) const {
		const auto& glyph = this->get_sf_font().getGlyph(c, this->get_character_size(), this->is_bold(), this->get_outline_thickness());
		return glyph.bounds.width;
	}
	qpl::f32 qsf::text::get_whitespace_width() const {
		float white_width = this->get_sf_font().getGlyph(L' ', this->get_character_size(), this->is_bold(), this->get_outline_thickness()).advance;
		float letter_spacing = (white_width / 3.f) * (this->get_letter_spacing() - 1.f);
		return white_width + letter_spacing;
	}


	void qsf::vrectangle::set_dimension(qpl::vector2f dimension) {
		this->dimension = dimension;
	}
	void qsf::vrectangle::set_position(qpl::vector2f position) {
		this->position = position;
	}
	void qsf::vrectangle::set_center(qpl::vector2f position) {
		this->position = position - this->dimension / 2;
	}
	void qsf::vrectangle::set_hitbox(const qpl::hitbox& hitbox) {
		this->set_position(hitbox.position);
		this->set_dimension(hitbox.dimension);
	}
	void qsf::vrectangle::set_color(qpl::rgb color) {
		this->color = color;
	}
	void qsf::vrectangle::set_outline_thickness(qpl::f32 outline_thickness) {
		this->outline_thickness = outline_thickness;
	}
	void qsf::vrectangle::set_outline_color(qpl::rgb outline_color) {
		this->outline_color = outline_color;
	}
	void qsf::vrectangle::set_multiplied_color(qpl::rgb color) {
		this->multiplied_color = color;
	}
	qpl::vector2f qsf::vrectangle::get_dimension() const {
		return this->dimension;
	}
	qpl::vector2f qsf::vrectangle::get_position() const {
		return this->position;
	}
	qpl::vector2f qsf::vrectangle::get_center() const {
		return this->position + (this->dimension / 2);
	}
	qpl::hitbox qsf::vrectangle::get_hitbox() const {
		return qpl::hitbox(this->position, this->dimension);
	}
	qpl::rgb qsf::vrectangle::get_color() const {
		return this->color;
	}
	qpl::f32 qsf::vrectangle::get_outline_thickness() const {
		return this->outline_thickness;
	}
	qpl::rgb qsf::vrectangle::get_outline_color() const {
		return this->color;
	}
	qpl::rgb qsf::vrectangle::get_multiplied_color() const {
		return this->multiplied_color;
	}
	void qsf::vrectangle::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::rectangle = *this;
		qsf::detail::rectangle.draw(window, states);
	}

	std::string qsf::vrectangle::string() const {
		return qpl::to_string('[', this->position.string(), ", ", this->dimension.string(), ']');
	}

	void qsf::vrectangle::increase(qpl::f32 delta) {
		this->position -= qpl::vector2f(delta, delta);
		this->dimension += qpl::vector2f(delta, delta) * 2;
	}
	qsf::vrectangle qsf::vrectangle::increased(qpl::f32 delta) const {
		auto copy = *this;
		copy.increase(delta);
		return copy;
	}
	bool qsf::vrectangle::contains(qpl::vector2f position) const {
		return (position.x > this->position.x && position.x < (this->position.x + this->dimension.x) &&
			position.y > this->position.y && position.y < (this->position.y + this->dimension.y));
	}
	bool qsf::vrectangle::contains(qpl::vector2f position, qpl::f32 hitbox_increase) const {
		return (position.x > (this->position.x - hitbox_increase) && position.x < (this->position.x + this->dimension.x + hitbox_increase) &&
			position.y > (this->position.y - hitbox_increase) && position.y < (this->position.y + this->dimension.y + hitbox_increase));
	}
	bool qsf::vrectangle::collides(qpl::straight_line line) const {
		return this->get_hitbox().collides(line);
	}
	bool qsf::vrectangle::collides(qpl::straight_line line, qpl::f32 hitbox_increase) const {
		return this->get_hitbox().collides(line, hitbox_increase);
	}


	void qsf::rectangle::set_dimension(qpl::vector2f dimension) {
		this->m_rect.setSize(dimension);
	}
	void qsf::rectangle::set_position(qpl::vector2f position) {
		this->m_rect.setPosition(position);
	}
	void qsf::rectangle::set_center(qpl::vector2f position) {
		this->m_rect.setPosition(position - this->get_dimension() / 2);
	}
	void qsf::rectangle::set_hitbox(const qpl::hitbox& hitbox) {
		this->set_position(hitbox.position);
		this->set_dimension(hitbox.dimension);
	}
	void qsf::rectangle::set_outline_thickness(qpl::f32 outline_thickness) {
		this->m_rect.setOutlineThickness(outline_thickness);
	}
	void qsf::rectangle::set_color(qpl::rgb color) {
		this->color = color;
		this->m_rect.setFillColor(this->color.multiplied_color(this->multiplied_color));
	}
	void qsf::rectangle::set_outline_color(qpl::rgb color) {
		this->outline_color = color;
		this->m_rect.setOutlineColor(this->outline_color.multiplied_color(this->multiplied_color));
	}
	void qsf::rectangle::set_multiplied_color(qpl::rgb color) {
		this->multiplied_color = color;
		this->m_rect.setFillColor(this->color.multiplied_color(this->multiplied_color));
		this->m_rect.setOutlineColor(this->outline_color.multiplied_color(this->multiplied_color));
	}
	void qsf::rectangle::increase(qpl::f64 delta) {

		this->set_position(this->get_position() - qpl::vector2f(delta, delta));
		this->set_dimension(this->get_dimension() + qpl::vector2f(delta, delta) * 2);
	}
	bool qsf::rectangle::contains(qpl::vector2f position) const {
		auto pos = this->get_position();
		auto dim = this->get_dimension();
		return (position.x > pos.x && position.x < (pos.x + dim.x) &&
			position.y > pos.y && position.y < (pos.y + dim.y));
	}
	bool qsf::rectangle::contains(qpl::vector2f position, qpl::f32 hitbox_increase) const {
		auto pos = this->get_position() - hitbox_increase;
		auto dim = this->get_dimension() + hitbox_increase * 2;
		return (position.x > pos.x && position.x < (pos.x + dim.x) &&
			position.y > pos.y && position.y < (pos.y + dim.y));
	}
	bool qsf::rectangle::contains(qpl::vector2f position, qpl::vector2f hitbox_increase) const {
		auto pos = this->get_position() - hitbox_increase;
		auto dim = this->get_dimension() + hitbox_increase * 2;
		return (position.x > pos.x && position.x < (pos.x + dim.x) &&
			position.y > pos.y && position.y < (pos.y + dim.y));
	}
	bool qsf::rectangle::collides(qpl::straight_line line) const {
		return this->get_hitbox().collides(line);
	}
	bool qsf::rectangle::collides(qpl::straight_line line, qpl::f32 hitbox_increase) const {
		return this->get_hitbox().collides(line, hitbox_increase);
	}
	void qsf::rectangle::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		window.draw(this->m_rect, states);
	}

	void qsf::va_rectangle::move(qpl::vector2f delta) {
		this->hitbox.move(delta);
		this->apply_hitbox();
	}
	void qsf::va_rectangle::set_color(qpl::rgb color) {
		for (auto& i : this->va) {
			i.color = color;
		}
	}
	void qsf::va_rectangle::set_position(qpl::vector2f position) {
		this->hitbox.position = position;
		this->apply_hitbox();
	}
	void qsf::va_rectangle::set_dimension(qpl::vector2f dimension) {
		this->hitbox.dimension = dimension;
		this->apply_hitbox();
	}
	void qsf::va_rectangle::set_hitbox(const qpl::hitbox& hitbox) {
		this->hitbox = hitbox;
		this->apply_hitbox();
	}
	qsf::vertex& qsf::va_rectangle::top_left() {
		return this->va[0];
	}
	qsf::vertex& qsf::va_rectangle::top_right() {
		return this->va[1];
	}
	qsf::vertex& qsf::va_rectangle::bottom_left() {
		return this->va[3];
	}
	qsf::vertex& qsf::va_rectangle::bottom_right() {
		return this->va[2];
	}

	const qsf::vertex& qsf::va_rectangle::top_left() const {
		return this->va[0];
	}
	const qsf::vertex& qsf::va_rectangle::top_right() const {
		return this->va[1];
	}
	const qsf::vertex& qsf::va_rectangle::bottom_left() const {
		return this->va[3];
	}
	const qsf::vertex& qsf::va_rectangle::bottom_right() const {
		return this->va[2];
	}

	qpl::vector2f qsf::va_rectangle::get_position() const {
		return this->hitbox.position;
	}
	qpl::vector2f qsf::va_rectangle::get_dimension() const {
		return this->hitbox.dimension;
	}
	qpl::hitbox qsf::va_rectangle::get_hitbox() const {
		return this->hitbox;
	}

	void qsf::va_rectangle::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		this->va.draw(window, states);
	}

	void qsf::va_rectangle::apply_hitbox() {
		this->va[0].position = this->hitbox.position;
		this->va[1].position = this->hitbox.position + qpl::vec(this->hitbox.dimension.x, 0);
		this->va[2].position = this->hitbox.position + this->hitbox.dimension;
		this->va[3].position = this->hitbox.position + qpl::vec(0, this->hitbox.dimension.y);
	}

	qpl::rgb qsf::rectangle::get_color() const {
		return this->color;
	}
	qpl::rgb qsf::rectangle::get_outline_color() const {
		return this->outline_color;
	}
	qpl::rgb qsf::rectangle::get_multiplied_color() const {
		return this->multiplied_color;
	}
	qpl::vector2f qsf::rectangle::get_position() const {
		return this->m_rect.getPosition();
	}
	qpl::vector2f qsf::rectangle::get_dimension() const {
		return this->m_rect.getSize();
	}
	qpl::vector2f qsf::rectangle::get_center() const {
		return this->get_position() + this->get_dimension() / 2;
	}
	qpl::f32 qsf::rectangle::get_outline_thickness() const {
		return this->m_rect.getOutlineThickness();
	}
	qpl::hitbox qsf::rectangle::get_hitbox() const {
		qpl::hitbox result;
		result.position = this->get_position();
		result.dimension = this->get_dimension();
		return result;
	}
	qsf::vlines qsf::rectangle::as_lines() const {
		qsf::vlines lines;

		lines.add_point(this->get_position());
		lines.add_point(this->get_position() + this->get_dimension().just_x());
		lines.add_point(this->get_position() + this->get_dimension());
		lines.add_point(this->get_position() + this->get_dimension().just_y());
		return lines;
	}
	qsf::vlines qsf::rectangle::as_lines_completed() const {
		qsf::vlines lines;

		lines.add_point(this->get_position());
		lines.add_point(this->get_position() + this->get_dimension().just_x());
		lines.add_point(this->get_position() + this->get_dimension());
		lines.add_point(this->get_position() + this->get_dimension().just_y());
		lines.complete();
		return lines;
	}
	qpl::vector2f qsf::rectangle::center() const {
		return this->get_position() + (this->get_dimension() / 2);
	}

	void qsf::rectangle::move(qpl::vector2f delta) {
		this->m_rect.move(delta);
	}
	qsf::rectangle& qsf::rectangle::operator=(const qsf::vrectangle& rectangle) {
		this->multiplied_color = qpl::rgb::white;
		this->set_color(rectangle.color);
		this->set_position(rectangle.position);
		this->set_dimension(rectangle.dimension);
		this->set_outline_color(rectangle.outline_color);
		this->set_outline_thickness(rectangle.outline_thickness);
		this->set_multiplied_color(rectangle.multiplied_color);
		return *this;
	}


	qpl::size qsf::vrectangles::size() const {
		return this->rectangles.size();
	}
	void qsf::vrectangles::resize(qpl::size new_size) {
		this->rectangles.resize(new_size);
	}
	void qsf::vrectangles::reserve(qpl::size new_size) {
		this->rectangles.reserve(new_size);
	}
	void qsf::vrectangles::clear() {
		this->rectangles.clear();
	}
	void qsf::vrectangles::add(const qsf::vrectangle& rect) {
		this->rectangles.push_back(rect);
	}
	qsf::vrectangle& qsf::vrectangles::operator[](qpl::size index) {
		return this->rectangles[index];
	}
	const qsf::vrectangle& qsf::vrectangles::operator[](qpl::size index) const {
		return this->rectangles[index];
	}

	qsf::vrectangle& qsf::vrectangles::front() {
		return this->rectangles.front();
	}
	const qsf::vrectangle& qsf::vrectangles::front() const {
		return this->rectangles.front();
	}

	qsf::vrectangle& qsf::vrectangles::back() {
		return this->rectangles.back();
	}
	const qsf::vrectangle& qsf::vrectangles::back() const {
		return this->rectangles.back();
	}

	std::vector<qsf::vrectangle>::iterator qsf::vrectangles::begin() {
		return this->rectangles.begin();
	}
	std::vector<qsf::vrectangle>::const_iterator qsf::vrectangles::begin() const {
		return this->rectangles.begin();
	}
	std::vector<qsf::vrectangle>::const_iterator qsf::vrectangles::cbegin() const {
		return this->rectangles.cbegin();
	}

	std::vector<qsf::vrectangle>::iterator qsf::vrectangles::end() {
		return this->rectangles.end();
	}
	std::vector<qsf::vrectangle>::const_iterator qsf::vrectangles::end() const {
		return this->rectangles.end();
	}
	std::vector<qsf::vrectangle>::const_iterator qsf::vrectangles::cend() const {
		return this->rectangles.cend();
	}




	void qsf::vrectangles::add_rectangle(vrectangle rectangle) {
		this->rectangles.push_back(rectangle);
	}
	void qsf::vrectangles::add_rectangle(qpl::vector2f position, qpl::vector2f dimension) {
		this->rectangles.push_back(qsf::vrectangle(position, dimension));
	}
	void qsf::vrectangles::add_rectangle(qpl::vector2f position, qpl::vector2f dimension, qpl::rgb color) {
		this->rectangles.push_back(qsf::vrectangle(position, dimension, color));
	}

	qsf::vrectangles& qsf::vrectangles::operator=(const qsf::vrectangles& rectangles) {
		this->rectangles = rectangles.rectangles;
		return *this;
	}
	void qsf::vrectangles::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::rectangles = *this;
		qsf::detail::rectangles.draw(window, states);
	}


	qpl::size qsf::rectangles::size() const {
		return this->rectangles_.size();
	}
	void qsf::rectangles::resize(qpl::size new_size) {
		this->rectangles_.resize(new_size);
	}
	void qsf::rectangles::reserve(qpl::size new_size) {
		this->rectangles_.reserve(new_size);
	}
	void qsf::rectangles::clear() {
		this->rectangles_.clear();
	}
	void qsf::rectangles::add(const qsf::rectangle& rect) {
		this->rectangles_.push_back(rect);
	}
	qsf::rectangle& qsf::rectangles::operator[](qpl::size index) {
		return this->rectangles_[index];
	}
	const qsf::rectangle& qsf::rectangles::operator[](qpl::size index) const {
		return this->rectangles_[index];
	}

	qsf::rectangle& qsf::rectangles::front() {
		return this->rectangles_.front();
	}
	const qsf::rectangle& qsf::rectangles::front() const {
		return this->rectangles_.front();
	}

	qsf::rectangle& qsf::rectangles::back() {
		return this->rectangles_.back();
	}
	const qsf::rectangle& qsf::rectangles::back() const {
		return this->rectangles_.back();
	}

	std::vector<qsf::rectangle>::iterator qsf::rectangles::begin() {
		return this->rectangles_.begin();
	}
	std::vector<qsf::rectangle>::const_iterator qsf::rectangles::begin() const {
		return this->rectangles_.begin();
	}
	std::vector<qsf::rectangle>::const_iterator qsf::rectangles::cbegin() const {
		return this->rectangles_.cbegin();
	}

	std::vector<qsf::rectangle>::iterator qsf::rectangles::end() {
		return this->rectangles_.end();
	}
	std::vector<qsf::rectangle>::const_iterator qsf::rectangles::end() const {
		return this->rectangles_.end();
	}
	std::vector<qsf::rectangle>::const_iterator qsf::rectangles::cend() const {
		return this->rectangles_.cend();
	}


	qsf::rectangles& qsf::rectangles::operator=(const qsf::vrectangles& rectangles) {
		this->rectangles_.resize(rectangles.size());
		for (qpl::u32 i = 0u; i < this->rectangles_.size(); ++i) {
			this->rectangles_[i] = rectangles[i];
		}
		return *this;
	}
	void qsf::rectangles::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		for (auto& i : this->rectangles_) {
			i.draw(window, states);
		}
	}

	sf::Vector2f qsf::polygon::polygon_proxy::operator=(qpl::vector2f position) {
		this->ptr->setPoint(this->index, position);
		return this->ptr->getPoint(this->index);
	}
	qsf::polygon::polygon_proxy::operator qpl::vector2f() const {
		return this->ptr->getPoint(this->index);
	}
	qsf::polygon::const_polygon_proxy::operator qpl::vector2f() const {
		return this->ptr->getPoint(this->index);
	}

	qsf::polygon::polygon_proxy qsf::polygon::operator[](qpl::size index) {
		qsf::polygon::polygon_proxy result;
		result.ptr = &this->shape;
		result.index = index;
		return result;
	}
	qsf::polygon::const_polygon_proxy qsf::polygon::operator[](qpl::size index) const {
		qsf::polygon::const_polygon_proxy result;
		result.ptr = &this->shape;
		result.index = index;
		return result;
	}
	qsf::polygon::polygon_proxy qsf::polygon::front() {
		qsf::polygon::polygon_proxy result;
		result.ptr = &this->shape;
		result.index = 0u;
		return result;
	}
	qsf::polygon::const_polygon_proxy qsf::polygon::front() const {
		qsf::polygon::const_polygon_proxy result;
		result.ptr = &this->shape;
		result.index = 0u;
		return result;
	}
	qsf::polygon::polygon_proxy qsf::polygon::back() {
		qsf::polygon::polygon_proxy result;
		result.ptr = &this->shape;
		result.index = this->size() - 1;
		return result;
	}
	qsf::polygon::const_polygon_proxy qsf::polygon::back() const {
		qsf::polygon::const_polygon_proxy result;
		result.ptr = &this->shape;
		result.index = 0u;
		return result;
	}

	void qsf::polygon::set_outline_thickness(qpl::f32 thickness) {
		this->shape.setOutlineThickness(thickness);
	}
	void qsf::polygon::set_outline_color(qpl::rgb color) {
		this->outline_color = color;
		this->shape.setOutlineColor(color.multiplied_color(this->multiplied_color));
	}
	void qsf::polygon::set_color(qpl::rgb color) {
		this->color = color;
		this->shape.setFillColor(color.multiplied_color(this->multiplied_color));
	}
	void qsf::polygon::set_multiplied_color(qpl::rgb color) {
		this->multiplied_color = color;
		this->shape.setFillColor(this->color.multiplied_color(this->multiplied_color));
		this->shape.setOutlineColor(this->outline_color.multiplied_color(this->multiplied_color));
	}
	void qsf::polygon::set_multiplied_alpha(qpl::u8 alpha) {
		this->set_multiplied_color(qpl::rgb::white.with_alpha(alpha));
	}
	void qsf::polygon::set_position(qpl::vector2f position) {
		this->shape.setPosition(position);
	}

	qpl::f32 qsf::polygon::get_outline_thickness() const {
		return this->shape.getOutlineThickness();
	}
	qpl::rgb qsf::polygon::get_outline_color() const {
		return this->outline_color;
	}
	qpl::rgb qsf::polygon::get_color() const {
		return this->color;
	}
	qpl::rgb qsf::polygon::get_multiplied_color() const {
		return this->multiplied_color;
	}

	void qsf::polygon::move(qpl::vector2f delta) {
		this->shape.move(delta);
	}
	void qsf::polygon::set_point(qpl::size index, qpl::vector2f position) {
		this->shape.setPoint(index, position);
	}
	qpl::vector2f qsf::polygon::get_point(qpl::size index) const {
		return this->shape.getPoint(index);
	}
	qpl::size qsf::polygon::size() const {
		return this->shape.getPointCount();
	}
	void qsf::polygon::resize(qpl::size size) {
		this->shape.setPointCount(size);
	}
	void qsf::polygon::add(qpl::vector2f point) {
		this->shape.setPointCount(this->size() + 1);
	}
	bool qsf::polygon::contains(qpl::vector2f point, qpl::size increment) const {

		bool flag = false;

		qpl::straight_line compare;
		compare.a = { 0, 0 };
		compare.b = point;

		auto j = this->size() - 1;
		for (qpl::size i = 0u; i < this->size(); i += increment) {
			qpl::straight_line edge_line;
			edge_line.a = this->get_point(i);
			edge_line.b = this->get_point(j);
			if (compare.collides(edge_line)) {
				flag = !flag;
			}
			j = i;
		}
		return flag;
	}
	void qsf::polygon::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		window.draw(this->shape, states);
	}


	void qsf::vsmooth_rectangle::move(qpl::vector2f delta) {
		this->position.move(delta);
	}
	void qsf::vsmooth_rectangle::set_dimension(qpl::vector2f dimension) {
		this->dimension = dimension;
	}
	void qsf::vsmooth_rectangle::increase(qpl::vector2f delta) {
		this->dimension += delta * 2;
		this->position -= delta;
	}
	void qsf::vsmooth_rectangle::increase(qpl::f32 delta) {
		this->increase(qpl::vec(delta, delta));
	}
	vsmooth_rectangle qsf::vsmooth_rectangle::increased(qpl::vector2f delta) const {
		auto copy = *this;
		copy.increase(delta);
		return copy;
	}
	vsmooth_rectangle qsf::vsmooth_rectangle::increased(qpl::f32 delta) const {
		auto copy = *this;
		copy.increase(delta);
		return copy;
	}

	void qsf::vsmooth_rectangle::set_position(qpl::vector2f position) {
		this->position = position;
	}
	void qsf::vsmooth_rectangle::set_hitbox(qpl::hitbox hitbox) {
		this->position = hitbox.position;
		this->dimension = hitbox.dimension;
	}
	void qsf::vsmooth_rectangle::set_center(qpl::vector2f position) {
		this->position = position - this->dimension / 2;
	}
	void qsf::vsmooth_rectangle::set_slope(qpl::f64 slope) {
		this->slope = slope;
	}
	void qsf::vsmooth_rectangle::set_color(qpl::rgb color) {
		this->color = color;
	}
	void qsf::vsmooth_rectangle::set_multiplied_color(qpl::rgb color) {
		this->multiplied_color = color;
	}
	void qsf::vsmooth_rectangle::set_outline_color(qpl::rgb color) {
		this->outline_color = color;
	}
	void qsf::vsmooth_rectangle::set_outline_thickness(qpl::f32 thickness) {
		this->outline_thickness = thickness;
	}
	void qsf::vsmooth_rectangle::set_slope_dimension(qpl::vector2f dimension) {
		this->slope_dim = dimension;
	}
	void qsf::vsmooth_rectangle::set_slope_point_count(qpl::size point_count) {
		this->slope_point_count = point_count;
	}
	qpl::vector2f qsf::vsmooth_rectangle::get_dimension() const {
		return this->dimension;
	}
	qpl::vector2f qsf::vsmooth_rectangle::get_position() const {
		return this->position;
	}
	qpl::vector2f qsf::vsmooth_rectangle::get_center() const {
		return this->position + this->dimension / 2;
	}
	qpl::f64 qsf::vsmooth_rectangle::get_slope() const {
		return this->slope;
	}
	qpl::rgb qsf::vsmooth_rectangle::get_color() const {
		return this->color;
	}
	qpl::rgb qsf::vsmooth_rectangle::get_multiplied_color() const {
		return this->multiplied_color;
	}
	qpl::rgb qsf::vsmooth_rectangle::get_outline_color() const {
		return this->outline_color;
	}
	qpl::f32 qsf::vsmooth_rectangle::get_outline_thickness() const {
		return this->outline_thickness;
	}
	qpl::vector2f qsf::vsmooth_rectangle::get_slope_dimension() const {
		return this->slope_dim;
	}
	qpl::size qsf::vsmooth_rectangle::get_slope_point_count() const {
		return this->slope_point_count;
	}
	bool qsf::vsmooth_rectangle::contains(qpl::vector2f point) const {
		qsf::detail::smooth_rectangle = *this;
		return qsf::detail::smooth_rectangle.contains(point);
	}
	void qsf::vsmooth_rectangle::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::smooth_rectangle = *this;
		qsf::detail::smooth_rectangle.draw(window, states);
	}


	void qsf::smooth_rectangle::set_color(qpl::rgb color) {
		this->polygon.set_color(color);
	}
	void qsf::smooth_rectangle::set_outline_color(qpl::rgb color) {
		this->polygon.set_outline_color(color);
	}
	void qsf::smooth_rectangle::set_outline_thickness(qpl::f32 thickness) {
		this->polygon.set_outline_thickness(thickness);
	}
	void qsf::smooth_rectangle::set_multiplied_color(qpl::rgb color) {
		this->polygon.set_multiplied_color(color);
	}
	void qsf::smooth_rectangle::set_multiplied_alpha(qpl::u8 alpha) {
		this->set_multiplied_color(qpl::rgb::white.with_alpha(alpha));
	}
	void qsf::smooth_rectangle::move(qpl::vector2f delta) {
		this->polygon.move(delta);
	}
	void qsf::smooth_rectangle::set_position(qpl::vector2f delta) {
		this->polygon.set_position(delta);
	}
	bool qsf::smooth_rectangle::contains(qpl::vector2f point) const {
		return this->polygon.contains(point);
	}
	qsf::smooth_rectangle& qsf::smooth_rectangle::operator=(const qsf::vsmooth_rectangle& smooth_rectangle) {
		this->polygon.set_color(smooth_rectangle.color.multiplied_color(smooth_rectangle.multiplied_color));
		this->polygon.set_outline_thickness(smooth_rectangle.outline_thickness);
		this->polygon.set_outline_color(smooth_rectangle.outline_color.multiplied_color(smooth_rectangle.multiplied_color));

		auto size = smooth_rectangle.slope_point_count;
		this->polygon.resize(size * 4);

		auto offset = smooth_rectangle.position;
		auto dimension = smooth_rectangle.slope_dim;
		auto rect_dimension = smooth_rectangle.dimension;
		qpl::u32 ctr = 0u;
		for (qpl::u32 i = 0u; i < size; ++i) {
			auto progress = std::pow(qpl::f64_cast(i) / (size - 1), 2);
			auto value = qpl::smooth_curve(progress, smooth_rectangle.slope);
			auto curve_pos = qpl::vec(progress, -value) * dimension;
			auto pos = offset + qpl::vec(0, dimension.y) + curve_pos;
			this->polygon.set_point(ctr, pos);
			++ctr;
		}
		for (qpl::u32 i = 0u; i < size; ++i) {
			auto progress = std::pow(1 - qpl::f64_cast(i) / (size - 1), 2);
			auto value = qpl::smooth_curve(progress, smooth_rectangle.slope);
			auto curve_pos = qpl::vec(progress, value) * dimension;
			auto pos = offset + qpl::vec(rect_dimension.x, dimension.y) - curve_pos;
			this->polygon.set_point(ctr, pos);
			++ctr;
		}
		for (qpl::u32 i = 0u; i < size; ++i) {
			auto progress = std::pow(qpl::f64_cast(i) / (size - 1), 2);
			auto value = qpl::smooth_curve(progress, smooth_rectangle.slope);
			auto curve_pos = qpl::vec(-progress, value) * dimension;
			auto pos = offset + qpl::vec(rect_dimension.x, rect_dimension.y - dimension.y) + curve_pos;
			this->polygon.set_point(ctr, pos);
			++ctr;
		}
		for (qpl::u32 i = 0u; i < size; ++i) {
			auto progress = std::pow(1 - qpl::f64_cast(i) / (size - 1), 2);
			auto value = qpl::smooth_curve(progress, smooth_rectangle.slope);
			auto curve_pos = qpl::vec(progress, value) * dimension;
			auto pos = offset + qpl::vec(0, rect_dimension.y - dimension.y) + curve_pos;
			this->polygon.set_point(ctr, pos);
			++ctr;
		}
		return *this;
	}
	void qsf::smooth_rectangle::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		this->polygon.draw(window, states);
	}

	vpoint& qsf::vpoint::operator=(qpl::vector2f position) {
		this->position = position;
		return *this;
	}
	bool qsf::vpoint::operator==(const vpoint& other) const {
		return this->position == other.position && this->color == other.color;
	}
	bool qsf::vpoint::operator!=(const vpoint& other) const {
		return !(*this == other);
	}
	void qsf::vpoint::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::point = *this;
		qsf::detail::point.draw(window, states);
	}

	qsf::line& qsf::line::operator=(const qsf::vline& line) {
		this->vertices[0].position = line.a.position;
		this->vertices[0].color = line.a.color;
		this->vertices[1].position = line.b.position;
		this->vertices[1].color = line.b.color;
		return *this;
	}
	void qsf::line::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		window.draw(this->vertices.data(), this->vertices.size(), sf::PrimitiveType::Lines, states);
	}


	qsf::point& qsf::point::operator=(const qsf::vpoint& point) {
		this->vertex.position = point.position;
		this->vertex.color = point.color;
		return *this;
	}
	void qsf::point::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		this->vertex.draw(window, states);
	}


	qpl::size qsf::vpoints::size() const {
		return this->points.size();
	}
	void qsf::vpoints::resize(qpl::size new_size) {
		this->points.resize(new_size);
	}
	void qsf::vpoints::reserve(qpl::size new_size) {
		this->points.reserve(new_size);
	}
	qsf::vpoint& qsf::vpoints::operator[](qpl::size index) {
		return this->points[index];
	}
	const qsf::vpoint& qsf::vpoints::operator[](qpl::size index) const {
		return this->points[index];
	}

	qsf::vpoint& qsf::vpoints::front() {
		return this->points.front();
	}
	const qsf::vpoint& qsf::vpoints::front() const {
		return this->points.front();
	}

	qsf::vpoint& qsf::vpoints::back() {
		return this->points.back();
	}
	const qsf::vpoint& qsf::vpoints::back() const {
		return this->points.back();
	}


	std::vector<qsf::vpoint>::iterator qsf::vpoints::begin() {
		return this->points.begin();
	}
	std::vector<qsf::vpoint>::const_iterator qsf::vpoints::begin() const {
		return this->points.begin();
	}
	std::vector<qsf::vpoint>::const_iterator qsf::vpoints::cbegin() const {
		return this->points.cbegin();
	}

	std::vector<qsf::vpoint>::iterator qsf::vpoints::end() {
		return this->points.end();
	}
	std::vector<qsf::vpoint>::const_iterator qsf::vpoints::end() const {
		return this->points.end();
	}
	std::vector<qsf::vpoint>::const_iterator qsf::vpoints::cend() const {
		return this->points.cend();
	}


	void qsf::vpoints::add_point(qsf::vpoint point) {
		this->points.push_back(point);
	}
	void qsf::vpoints::add_point(qpl::vector2f position, qpl::rgb color) {
		this->points.push_back(qsf::vpoint(position, color));
	}
	qsf::vlines qsf::vpoints::as_lines() const {
		qsf::vlines result;
		result.points = this->points;
		result.complete();
		return result;
	}
	void qsf::vpoints::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::points = *this;
		qsf::detail::points.draw(window, states);
	}
	void qsf::vpoints::clear() {
		this->points.clear();
	}


	void qsf::points::resize(qpl::size new_size) {
		this->vertices.resize(new_size);
	}
	void qsf::points::reserve(qpl::size new_size) {
		this->vertices.reserve(new_size);
	}
	qsf::vertex& qsf::points::operator[](qpl::size index) {
		return this->vertices[index];
	}
	const qsf::vertex& qsf::points::operator[](qpl::size index) const {
		return this->vertices[index];
	}

	qsf::vertex& qsf::points::front() {
		return this->vertices[0u];
	}
	const qsf::vertex& qsf::points::front() const {
		return this->vertices[0u];
	}

	qsf::vertex& qsf::points::back() {
		return this->vertices[this->size() - 1];
	}
	const qsf::vertex& qsf::points::back() const {
		return this->vertices[this->size() - 1];
	}

	qsf::vlines qsf::points::as_lines() const {
		qsf::vlines result;

		result.points.resize(this->vertices.size());
		for (qpl::u32 i = 0u; i < result.points.size(); ++i) {
			result.points[i].position = this->vertices[i].position;
			result.points[i].color = this->vertices[i].color;
		}

		result.complete();
		return result;
	}
	qsf::points& qsf::points::operator=(const qsf::vpoints& points) {
		this->vertices.resize(points.points.size());
		for (qpl::u32 i = 0u; i < points.points.size(); ++i) {
			this->vertices[i].color = points.points[i].color;
			this->vertices[i].position = points.points[i].position;
		}
		return *this;
	}
	void qsf::points::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		this->vertices.draw(window, states);
	}
	void qsf::points::clear() {
		this->vertices.clear();
	}
	qpl::size qsf::points::size() const {
		return this->vertices.size();
	}



	void qsf::vcircle::set_position(qpl::vector2f position) {
		this->point.position = position;
	}
	void qsf::vcircle::set_radius(qpl::f32 radius) {
		this->radius = radius;
	}
	void qsf::vcircle::set_color(qpl::rgb color) {
		this->point.color = color;
	}
	void qsf::vcircle::set_center(qpl::vector2f center) {
		this->point.position = center - qpl::vector2f{ this->radius, this->radius };
	}
	void qsf::vcircle::set_outline_thickness(qpl::f32 outline_thickness) {
		this->outline_thickness = outline_thickness;
	}
	void qsf::vcircle::set_outline_color(qpl::rgb outline_color) {
		this->outline_color = outline_color;
	}
	qpl::vector2f qsf::vcircle::get_center() const {
		return this->point.position + qpl::vector2f{ this->radius, this->radius };
	}
	void qsf::vcircle::centerize() {
		this->point.position -= qpl::vector2f{ this->radius, this->radius };
	}
	void qsf::vcircle::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::circle = *this;
		qsf::detail::circle.draw(window, states);
	}
	void qsf::circle::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		window.draw(this->circle_shape, states);
	}
	qsf::circle& qsf::circle::operator=(const qsf::vcircle& circle) {
		this->circle_shape.setPosition(circle.point.position);
		this->circle_shape.setRadius(circle.radius);
		this->circle_shape.setFillColor(circle.point.color);
		this->circle_shape.setOutlineThickness(circle.outline_thickness);
		this->circle_shape.setOutlineColor(circle.outline_color);
		return *this;
	}
	qsf::circle& qsf::circle::operator=(const qsf::circle& circle) {
		this->circle_shape.setPosition(circle.circle_shape.getPosition());
		this->circle_shape.setRadius(circle.circle_shape.getRadius());
		this->circle_shape.setFillColor(circle.circle_shape.getFillColor());
		this->circle_shape.setOrigin(circle.circle_shape.getOrigin());
		return *this;
	}
	void qsf::circle::centerize_origin() {
		this->circle_shape.setOrigin(qpl::vec(this->get_radius(), this->get_radius()));
	}
	void qsf::circle::set_radius(qpl::f32 radius) {
		this->circle_shape.setRadius(radius);
	}
	qpl::f32 qsf::circle::get_radius() const {
		return this->circle_shape.getRadius();
	}
	void qsf::circle::set_position(qpl::vector2f position) {
		this->circle_shape.setPosition(position);
	}
	void qsf::circle::set_center(qpl::vector2f center) {
		this->set_position(center - qpl::vector2f{ this->get_radius(), this->get_radius() });
	}
	qpl::vector2f qsf::circle::get_center() const {

		return this->get_position() + qpl::vector2f{ this->get_radius(), this->get_radius() };
	}
	qpl::vector2f qsf::circle::get_position() const {
		return this->circle_shape.getPosition();
	}
	void qsf::circle::set_color(qpl::rgb color) {
		this->circle_shape.setFillColor(color);
	}
	qpl::rgb qsf::circle::get_color() const {
		return this->circle_shape.getFillColor();
	}

	void qsf::circle::set_outline_thickness(qpl::f32 outline_thickness) {
		this->circle_shape.setOutlineThickness(outline_thickness);
	}
	qpl::f32 qsf::circle::get_outline_thickness() const {
		return this->circle_shape.getOutlineThickness();
	}
	void qsf::circle::set_outline_color(qpl::rgb outline_color) {
		this->circle_shape.setOutlineColor(outline_color);
	}
	qpl::rgb qsf::circle::get_outline_color() const {
		return this->circle_shape.getOutlineColor();
	}
	void qsf::circle::set_point_count(qpl::size count) {
		this->circle_shape.setPointCount(count);
	}
	qpl::size qsf::circle::get_point_count() const {
		return this->circle_shape.getPointCount();
	}

	void qsf::vcircles::resize(qpl::size new_size) {
		this->circles.resize(new_size);
	}
	void qsf::vcircles::reserve(qpl::size new_size) {
		this->circles.reserve(new_size);
	}


	qsf::vcircle& qsf::vcircles::operator[](qpl::size index) {
		return this->circles[index];
	}
	const qsf::vcircle& qsf::vcircles::operator[](qpl::size index) const {
		return this->circles[index];
	}


	qsf::vcircle& qsf::vcircles::front() {
		return this->circles.front();
	}
	const qsf::vcircle& qsf::vcircles::front() const {
		return this->circles.front();
	}

	qsf::vcircle& qsf::vcircles::back() {
		return this->circles.back();
	}
	const qsf::vcircle& qsf::vcircles::back() const {
		return this->circles.back();
	}

	void qsf::vcircles::add_circle(qsf::vcircle circle) {
		this->circles.push_back(circle);
	}
	void qsf::vcircles::add_circle(qsf::vpoint point, qpl::f32 radius, qpl::rgb color) {
		this->circles.push_back(qsf::vcircle(point.position, radius, color));
	}
	void qsf::vcircles::add_circle(qpl::vector2f position, qpl::f32 radius, qpl::rgb color) {
		this->circles.push_back(qsf::vcircle(position, radius, color));
	}
	qpl::size qsf::vcircles::size() const {
		return this->circles.size();
	}
	void qsf::vcircles::clear() {
		this->circles.clear();
	}

	void qsf::vcircles::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::circles = *this;
		qsf::detail::circles.draw(window, states);
	}


	void qsf::circles::resize(qpl::size new_size) {
		this->circles_.resize(new_size);
	}
	void qsf::circles::reserve(qpl::size new_size) {
		this->circles_.reserve(new_size);
	}
	qsf::circle& qsf::circles::operator[](qpl::size index) {
		return this->circles_[index];
	}
	const qsf::circle& qsf::circles::operator[](qpl::size index) const {
		return this->circles_[index];
	}

	qsf::circle& qsf::circles::front() {
		return this->circles_.front();
	}
	const qsf::circle& qsf::circles::front() const {
		return this->circles_.front();
	}

	qsf::circle& qsf::circles::back() {
		return this->circles_.back();
	}
	const qsf::circle& qsf::circles::back() const {
		return this->circles_.back();
	}

	void qsf::circles::add_circle(const qsf::vcircle& circle) {
		this->circles_.push_back(circle);
	}
	void qsf::circles::add_circle(qsf::vpoint point, qpl::f32 radius, qpl::rgb color) {
		this->circles_.push_back(qsf::vcircle(point.position, radius, color));
	}
	void qsf::circles::add_circle(qpl::vector2f position, qpl::f32 radius, qpl::rgb color) {
		this->circles_.push_back(qsf::vcircle(position, radius, color));
	}
	void qsf::circles::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		for (auto& i : this->circles_) {
			i.draw(window, states);
		}
	}
	qsf::circles& qsf::circles::operator=(const qsf::vcircles& circles) {
		this->circles_.resize(circles.size());
		for (qpl::u32 i = 0u; i < circles.size(); ++i) {
			this->circles_[i] = circles.circles[i];
		}
		return *this;
	}
	qsf::circles& qsf::circles::operator=(const qsf::circles& circles) {
		this->circles_ = circles.circles_;
		return *this;
	}
	void qsf::circles::clear() {
		this->circles_.clear();
	}
	qpl::size qsf::circles::size() const {
		return this->circles_.size();
	}


	void qsf::vline::set_a(qsf::vpoint point) {
		this->a = point.position;
	}
	void qsf::vline::set_a(qpl::vector2f position) {
		this->a = position;
	}
	void qsf::vline::set_b(qsf::vpoint point) {
		this->b = point.position;
	}
	void qsf::vline::set_b(qpl::vector2f position) {
		this->b = position;
	}
	void qsf::vline::set_color(qpl::rgb color) {
		this->a.color = this->b.color = color;
	}
	void qsf::vline::set_a_color(qpl::rgb color) {
		this->a.color = color;
	}
	void qsf::vline::set_b_color(qpl::rgb color) {
		this->b.color = color;
	}
	qpl::f32 qsf::vline::length() const {
		auto diff = this->b.position - this->a.position;
		return std::sqrt(diff.x * diff.x + diff.y * diff.y);
	}
	qpl::vector2f qsf::vline::normal() const {
		return qpl::vector2f{ this->a.position.y - this->b.position.y, this->b.position.x - this->a.position.x } / this->length();
	}
	qsf::vline& qsf::vline::rotate_around_a(qpl::f64 degree) {


		return *this;
	}
	qsf::vline& qsf::vline::rotate_around_b(qpl::f64 degree) {
		//a / sin(a) = b / sin(b) = c / sin(c)

		//(x, y) + 
		return *this;
	}
	qsf::vline::operator qpl::straight_line() const {
		return qpl::straight_line(this->a.position, this->b.position);
	}
	void qsf::vline::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::line = *this;
		qsf::detail::line.draw(window, states);
	}

	void qsf::line::set_a(qsf::vpoint point) {
		this->vertices[0].position = point.position;
	}
	void qsf::line::set_a(qpl::vector2f position) {
		this->vertices[0].position = position;
	}
	void qsf::line::set_b(qsf::vpoint point) {
		this->vertices[1].position = point.position;
	}
	void qsf::line::set_b(qpl::vector2f position) {
		this->vertices[1].position = position;
	}
	void qsf::line::set_color(qpl::rgb color) {
		this->vertices[0].color = this->vertices[1].color = color;
	}
	void qsf::line::set_a_color(qpl::rgb color) {
		this->vertices[0].color = color;
	}
	void qsf::line::set_b_color(qpl::rgb color) {
		this->vertices[1].color = color;
	}
	qsf::vpoint qsf::line::get_a() const {
		return qsf::vpoint(this->vertices[0].position, this->vertices[0].color);
	}
	qsf::vpoint qsf::line::get_b() const {
		return qsf::vpoint(this->vertices[1].position, this->vertices[1].color);
	}
	qpl::vector2f qsf::line::normal() const {
		return qpl::vector2f{ this->vertices[0].position.y - this->vertices[1].position.y, this->vertices[1].position.x - this->vertices[0].position.x } / this->length();
	}
	qpl::f32 qsf::line::length() const {
		qpl::vector2f diff = this->vertices[0].position - this->vertices[1].position;
		return std::sqrt(diff.x * diff.x + diff.y * diff.y);
	}
	qsf::line::operator qpl::straight_line() const {
		return qpl::straight_line(this->vertices[0].position, this->vertices[1].position);
	}

	void qsf::vlines::resize(qpl::size new_size) {
		this->points.resize(new_size);
	}
	void qsf::vlines::reserve(qpl::size new_size) {
		this->points.reserve(new_size);
	}

	qsf::vpoint& qsf::vlines::operator[](qpl::size index) {
		return this->points[index];
	}
	const qsf::vpoint& qsf::vlines::operator[](qpl::size index) const {
		return this->points[index];
	}

	qsf::vpoint& qsf::vlines::front() {
		return this->points.front();
	}
	const qsf::vpoint& qsf::vlines::front() const {
		return this->points.front();
	}

	qsf::vpoint& qsf::vlines::back() {
		return this->points.back();
	}
	const qsf::vpoint& qsf::vlines::back() const {
		return this->points.back();
	}


	void qsf::vlines::clear() {
		this->points.clear();
	}
	qpl::size qsf::vlines::size() const {
		return this->points.size();
	}
	void qsf::vlines::add_point(qsf::vpoint point) {
		this->points.push_back(point);
	}
	void qsf::vlines::add_point(qpl::vector2f position, qpl::rgb color) {
		this->points.push_back(qsf::vpoint{ position, color });
	}
	void qsf::vlines::complete() {
		this->add_point(this->points[0]);
	}

	void qsf::vlines::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::lines = *this;
		qsf::detail::lines.draw(window, states);
	}



	void qsf::lines::resize(qpl::size new_size) {
		this->vertices.resize(new_size);
	}
	void qsf::lines::reserve(qpl::size new_size) {
		this->vertices.reserve(new_size);
	}
	qsf::vertex& qsf::lines::operator[](qpl::size index) {
		return this->vertices[index];
	}
	const qsf::vertex& qsf::lines::operator[](qpl::size index) const {
		return this->vertices[index];
	}

	qsf::vertex& qsf::lines::front() {
		return this->vertices[0u];
	}
	const qsf::vertex& qsf::lines::front() const {
		return this->vertices[0u];
	}

	qsf::vertex& qsf::lines::back() {
		return this->vertices[this->size() - 1u];
	}
	const qsf::vertex& qsf::lines::back() const {
		return this->vertices[this->size() - 1u];
	}


	void qsf::lines::complete() {
		this->add_point(this->vertices[0].position, this->vertices[0].color);
	}

	qsf::lines& qsf::lines::operator=(const qsf::vlines& lines) {
		this->vertices.resize(lines.size());
		for (qpl::u32 i = 0u; i < lines.points.size(); ++i) {
			this->vertices[i] = qsf::vertex(lines.points[i].position, lines.points[i].color);
		}
		return *this;
	}
	void qsf::lines::clear() {
		this->vertices.clear();
	}
	qpl::size qsf::lines::size() const {
		return this->vertices.size();
	}
	void qsf::lines::add_point(qsf::vpoint point) {
		this->vertices.add(qsf::vertex(point.position, point.color));
	}
	void qsf::lines::add_point(qpl::vector2f position, qpl::rgb color) {
		this->vertices.add(qsf::vertex(position, color));
	}
	void qsf::lines::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		this->vertices.draw(window, states);
	}

	void qsf::vthick_line::set_a(qsf::vpoint point) {
		this->a = point.position;
	}
	void qsf::vthick_line::set_a(qpl::vector2f position) {
		this->a = position;
	}
	void qsf::vthick_line::set_b(qsf::vpoint point) {
		this->b = point.position;
	}
	void qsf::vthick_line::set_b(qpl::vector2f position) {
		this->b = position;
	}
	void qsf::vthick_line::set_color(qpl::rgb color) {
		this->a.color = this->b.color = color;
	}
	void qsf::vthick_line::set_a_color(qpl::rgb color) {
		this->a.color = color;
	}
	void qsf::vthick_line::set_b_color(qpl::rgb color) {
		this->b.color = color;
	}
	qpl::f32 qsf::vthick_line::length() const {
		auto diff = this->b.position - this->a.position;
		return std::sqrt(diff.x * diff.x + diff.y * diff.y);
	}
	qsf::vpoints qsf::vthick_line::as_points() const {
		qsf::vpoints result(4);
		auto normal = this->normal();

		result.points[0].position = this->a.position - normal * this->thickness;
		result.points[1].position = this->a.position + normal * this->thickness;
		result.points[2].position = this->b.position + normal * this->thickness;
		result.points[3].position = this->b.position - normal * this->thickness;

		return result;
	}
	qpl::vector2f qsf::vthick_line::normal() const {
		return qpl::vector2f{ this->a.position.y - this->b.position.y, this->b.position.x - this->a.position.x } / this->length();
	}
	void qsf::vthick_line::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::thick_line = *this;
		qsf::detail::thick_line.draw(window, states);
	}

	void qsf::thick_line::set_a(qsf::vpoint point) {
		this->vertices[0].position = this->vertices[1].position = point.position;
		this->vertices[0].color = this->vertices[1].color = point.color;
	}
	void qsf::thick_line::set_a(qpl::vector2f position) {
		this->vertices[0].position = this->vertices[1].position = position;
	}
	void qsf::thick_line::set_b(qsf::vpoint point) {
		this->vertices[2].position = this->vertices[3].position = point.position;
		this->vertices[2].color = this->vertices[3].color = point.color;
	}
	void qsf::thick_line::set_b(qpl::vector2f position) {
		this->vertices[2].position = this->vertices[3].position = position;
	}
	void qsf::thick_line::set_color(qpl::rgb color) {
		this->vertices[0].color = this->vertices[1].color = this->vertices[2].color = this->vertices[3].color = color;
	}
	void qsf::thick_line::set_a_color(qpl::rgb color) {
		this->vertices[0].color = this->vertices[1].color = color;
	}
	void qsf::thick_line::set_b_color(qpl::rgb color) {
		this->vertices[2].color = this->vertices[3].color = color;
	}
	void qsf::thick_line::set_thickness(qpl::f32 thickness) {
		auto normal = this->normal();

		this->vertices[0].position = qpl::vector2f(this->vertices[0].position) - normal * thickness;
		this->vertices[1].position = qpl::vector2f(this->vertices[0].position) + normal * thickness;
		this->vertices[2].position = qpl::vector2f(this->vertices[2].position) + normal * thickness;
		this->vertices[3].position = qpl::vector2f(this->vertices[2].position) - normal * thickness;
	}
	qpl::f32 qsf::thick_line::length() const {
		auto diff = this->vertices[0].position - this->vertices[2].position;
		return std::sqrt(diff.x * diff.x + diff.y * diff.y);
	}
	qsf::vpoints qsf::thick_line::as_points() const {
		qsf::vpoints result(4);
		auto normal = this->normal();

		result.points[0].position = this->vertices[0].position;
		result.points[1].position = this->vertices[1].position;
		result.points[2].position = this->vertices[2].position;
		result.points[3].position = this->vertices[3].position;

		return result;
	}
	qpl::vector2f qsf::thick_line::normal() const {
		return qpl::vector2f{ this->vertices[0].position.y - this->vertices[2].position.y, this->vertices[2].position.x - this->vertices[0].position.x } / this->length();
	}
	qsf::thick_line& qsf::thick_line::operator=(const qsf::vthick_line& line) {
		auto normal = line.normal();

		this->vertices[0].position = line.a.position - normal * line.thickness;
		this->vertices[0].color = line.a.color;
		this->vertices[1].position = line.a.position + normal * line.thickness;
		this->vertices[1].color = line.a.color;
		this->vertices[2].position = line.b.position + normal * line.thickness;
		this->vertices[2].color = line.b.color;
		this->vertices[3].position = line.b.position - normal * line.thickness;
		this->vertices[3].color = line.b.color;
		return *this;
	}
	void qsf::thick_line::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		window.draw(this->vertices.data(), this->vertices.size(), sf::PrimitiveType::Quads, states);
	}

	void qsf::vthick_lines::resize(qpl::size new_size) {
		this->points.resize(new_size);
	}
	void qsf::vthick_lines::reserve(qpl::size new_size) {
		this->points.reserve(new_size);
	}

	qsf::vpoint& qsf::vthick_lines::operator[](qpl::size index) {
		return this->points[index];
	}
	const qsf::vpoint& qsf::vthick_lines::operator[](qpl::size index) const {
		return this->points[index];
	}

	qsf::vpoint& qsf::vthick_lines::front() {
		return this->points.front();
	}
	const qsf::vpoint& qsf::vthick_lines::front() const {
		return this->points.front();
	}

	qsf::vpoint& qsf::vthick_lines::back() {
		return this->points.back();
	}
	const qsf::vpoint& qsf::vthick_lines::back() const {
		return this->points.back();
	}


	void qsf::vthick_lines::add_thick_line(qsf::vpoint point) {
		this->points.add_point(point);
	}
	void qsf::vthick_lines::add_thick_line(qpl::vector2f position, qpl::rgb color) {
		this->points.add_point(position, color);
	}
	void qsf::vthick_lines::set_thickness(qpl::f32 thickness) {
		this->thickness = thickness;
	}
	void qsf::vthick_lines::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::thick_lines = *this;
		qsf::detail::thick_lines.draw(window, states);
	}
	void qsf::vthick_lines::clear() {
		this->points.clear();
	}
	qpl::size qsf::vthick_lines::size() const {
		return this->points.size();
	}


	void qsf::thick_lines::resize(qpl::size new_size) {
		this->vertices.resize(new_size);
	}
	void qsf::thick_lines::reserve(qpl::size new_size) {
		this->vertices.reserve(new_size);
	}

	qsf::vertex& qsf::thick_lines::operator[](qpl::size index) {
		return this->vertices[index];
	}
	const qsf::vertex& qsf::thick_lines::operator[](qpl::size index) const {
		return this->vertices[index];
	}

	qsf::vertex& qsf::thick_lines::front() {
		return this->vertices[0u];
	}
	const qsf::vertex& qsf::thick_lines::front() const {
		return this->vertices[0u];
	}

	qsf::vertex& qsf::thick_lines::back() {
		return this->vertices[this->size() - 1u];
	}
	const qsf::vertex& qsf::thick_lines::back() const {
		return this->vertices[this->size() - 1u];
	}


	void qsf::thick_lines::add_thick_line(qsf::vpoint point, qpl::f32 thickness) {
		this->add_thick_line(point.position, point.color, thickness);
	}
	void qsf::thick_lines::add_thick_line(qpl::vector2f position, qpl::rgb color, qpl::f32 thickness) {
		auto last_point = position;
		auto last_color = color;
		auto last_thickness = thickness;
		if (this->vertices.size()) {
			last_point = (this->vertices[this->vertices.size() - 1].position + this->vertices[this->vertices.size() - 2].position) / 2.0f;

			last_color.r = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.r) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.r)) / 2);
			last_color.g = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.g) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.g)) / 2);
			last_color.b = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.b) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.b)) / 2);
			last_color.a = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.a) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.a)) / 2);

			qsf::vline line;
			line.set_a(this->vertices[this->vertices.size() - 1].position);
			line.set_b(this->vertices[this->vertices.size() - 2].position);
			last_thickness = line.length() / 2;
		}


		qsf::vline line;
		line.set_b(last_point);
		line.set_a(position);

		auto normal = line.normal();
		if (position == last_point) {
			normal.x = 1.0;
			normal.y = 0.0;
		}

		if (this->vertices.size() > 4u) {
			this->vertices.resize(this->vertices.size() + 8);
			this->vertices[this->vertices.size() - 8] = this->vertices[this->vertices.size() - 10];
			this->vertices[this->vertices.size() - 7] = this->vertices[this->vertices.size() - 9];
			this->vertices[this->vertices.size() - 6].position = last_point + normal * last_thickness;
			this->vertices[this->vertices.size() - 6].color = last_color;
			this->vertices[this->vertices.size() - 5].position = last_point - normal * last_thickness;
			this->vertices[this->vertices.size() - 5].color = last_color;

			this->vertices[this->vertices.size() - 4] = this->vertices[this->vertices.size() - 6];
			this->vertices[this->vertices.size() - 3] = this->vertices[this->vertices.size() - 5];
			this->vertices[this->vertices.size() - 2].position = position - normal * thickness;
			this->vertices[this->vertices.size() - 2].color = color;
			this->vertices[this->vertices.size() - 1].position = position + normal * thickness;
			this->vertices[this->vertices.size() - 1].color = color;
		}
		else {
			this->vertices.resize(this->vertices.size() + 4);
			this->vertices[this->vertices.size() - 4].position = last_point + normal * last_thickness;
			this->vertices[this->vertices.size() - 4].color = last_color;
			this->vertices[this->vertices.size() - 3].position = last_point - normal * last_thickness;
			this->vertices[this->vertices.size() - 3].color = last_color;
			this->vertices[this->vertices.size() - 2].position = position - normal * thickness;
			this->vertices[this->vertices.size() - 2].color = color;
			this->vertices[this->vertices.size() - 1].position = position + normal * thickness;
			this->vertices[this->vertices.size() - 1].color = color;
		}

	}
	void qsf::thick_lines::complete() {
		if (this->empty()) {
			return;
		}
		qpl::vector2f first_point;
		qpl::rgb first_color;
		qpl::f32 first_thickness = 0.0f;

		if (this->vertices.size()) {
			first_point = (this->vertices[0].position + this->vertices[1].position) / 2.0f;

			first_color.r = qpl::u8_cast((qpl::u32_cast(this->vertices[0].color.r) + qpl::u32_cast(this->vertices[1].color.r)) / 2);
			first_color.g = qpl::u8_cast((qpl::u32_cast(this->vertices[0].color.g) + qpl::u32_cast(this->vertices[1].color.g)) / 2);
			first_color.b = qpl::u8_cast((qpl::u32_cast(this->vertices[0].color.b) + qpl::u32_cast(this->vertices[1].color.b)) / 2);
			first_color.a = qpl::u8_cast((qpl::u32_cast(this->vertices[0].color.a) + qpl::u32_cast(this->vertices[1].color.a)) / 2);

			qsf::vline line;
			line.set_a(this->vertices[0].position);
			line.set_b(this->vertices[1].position);
			first_thickness = line.length() / 2;
		}

		auto last_point = first_point;
		auto last_color = first_color;
		auto last_thickness = first_thickness;
		if (this->vertices.size() > 4) {
			last_point = (this->vertices[this->vertices.size() - 1].position + this->vertices[this->vertices.size() - 2].position) / 2.0f;

			last_color.r = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.r) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.r)) / 2);
			last_color.g = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.g) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.g)) / 2);
			last_color.b = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.b) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.b)) / 2);
			last_color.a = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.a) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.a)) / 2);

			qsf::vline line;
			line.set_a(this->vertices[this->vertices.size() - 1].position);
			line.set_b(this->vertices[this->vertices.size() - 2].position);
			last_thickness = line.length() / 2;
		}

		qsf::vline line;
		line.set_b(last_point);
		line.set_a(first_point);

		auto normal = line.normal();
		if (first_point == last_point) {
			normal.x = 1.0;
			normal.y = 0.0;
		}

		this->vertices.resize(this->vertices.size() + 8);

		this->vertices[this->vertices.size() - 8] = this->vertices[this->vertices.size() - 10];
		this->vertices[this->vertices.size() - 7] = this->vertices[this->vertices.size() - 9];
		this->vertices[this->vertices.size() - 6].position = last_point + normal * last_thickness;
		this->vertices[this->vertices.size() - 6].color = last_color;
		this->vertices[this->vertices.size() - 5].position = last_point - normal * last_thickness;
		this->vertices[this->vertices.size() - 5].color = last_color;

		this->vertices[this->vertices.size() - 4].position = last_point + normal * last_thickness;
		this->vertices[this->vertices.size() - 4].color = last_color;
		this->vertices[this->vertices.size() - 3].position = last_point - normal * last_thickness;
		this->vertices[this->vertices.size() - 3].color = last_color;
		this->vertices[this->vertices.size() - 2].position = first_point - normal * first_thickness;
		this->vertices[this->vertices.size() - 2].color = first_color;
		this->vertices[this->vertices.size() - 1].position = first_point + normal * first_thickness;
		this->vertices[this->vertices.size() - 1].color = first_color;

		auto mid_point = (this->vertices[8].position + this->vertices[9].position) / 2.0f;

		qpl::rgb mid_color;
		mid_color.r = qpl::u8_cast((qpl::u32_cast(this->vertices[8].color.r) + qpl::u32_cast(this->vertices[9].color.r)) / 2);
		mid_color.g = qpl::u8_cast((qpl::u32_cast(this->vertices[8].color.g) + qpl::u32_cast(this->vertices[9].color.g)) / 2);
		mid_color.b = qpl::u8_cast((qpl::u32_cast(this->vertices[8].color.b) + qpl::u32_cast(this->vertices[9].color.b)) / 2);
		mid_color.a = qpl::u8_cast((qpl::u32_cast(this->vertices[8].color.a) + qpl::u32_cast(this->vertices[9].color.a)) / 2);

		line.set_a(this->vertices[8].position);
		line.set_b(this->vertices[9].position);
		auto mid_thickness = line.length() / 2;

		last_point = (this->vertices[this->vertices.size() - 1].position + this->vertices[this->vertices.size() - 2].position) / 2.0f;

		last_color.r = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.r) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.r)) / 2);
		last_color.g = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.g) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.g)) / 2);
		last_color.b = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.b) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.b)) / 2);
		last_color.a = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.a) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.a)) / 2);

		line.set_a(this->vertices[this->vertices.size() - 1].position);
		line.set_b(this->vertices[this->vertices.size() - 2].position);
		last_thickness = line.length() / 2;

		line.set_b(last_point);
		line.set_a(mid_point);

		normal = line.normal();
		if (mid_point == last_point) {
			normal.x = 1.0;
			normal.y = 0.0;
		}

		this->vertices.resize(this->vertices.size() + 4);
		this->vertices[this->vertices.size() - 4] = this->vertices[this->vertices.size() - 6];
		this->vertices[this->vertices.size() - 3] = this->vertices[this->vertices.size() - 5];
		this->vertices[this->vertices.size() - 2].position = last_point + normal * last_thickness;
		this->vertices[this->vertices.size() - 2].color = last_color;
		this->vertices[this->vertices.size() - 1].position = last_point - normal * last_thickness;
		this->vertices[this->vertices.size() - 1].color = last_color;

	}
	qsf::thick_lines& qsf::thick_lines::operator=(const qsf::vthick_lines& lines) {
		this->vertices.clear();
		for (auto& p : lines.points.points) {
			this->add_thick_line(p.position + lines.position, p.color, lines.thickness);
		}
		return *this;
	}
	void qsf::thick_lines::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		this->vertices.draw(window, states);
	}
	void qsf::thick_lines::clear() {
		this->vertices.clear();
	}
	qpl::size qsf::thick_lines::size() const {
		return this->vertices.size();
	}
	bool qsf::thick_lines::empty() const {
		return this->vertices.empty();
	}


	void qsf::sprite::set_texture(const sf::Texture& texture) {
		this->m_sprite.setTexture(texture);
	}
	void qsf::sprite::set_texture_rect(const sf::IntRect& rect) {
		this->m_sprite.setTextureRect(rect);
	}
	void qsf::sprite::set_texture_rect(qpl::hitbox hitbox) {
		sf::IntRect rect;
		rect.left = qpl::i32_cast(hitbox.position.x);
		rect.top = qpl::i32_cast(hitbox.position.y);
		rect.width = qpl::i32_cast(hitbox.dimension.x);
		rect.height = qpl::i32_cast(hitbox.dimension.y);
		this->m_sprite.setTextureRect(rect);
	}
	void qsf::sprite::set_color(qpl::rgb color) {
		this->color = color;
		this->m_sprite.setColor(this->color.multiplied_color(this->multiplied_color));
	}
	void qsf::sprite::set_multiplied_color(qpl::rgb color) {
		this->multiplied_color = color;
		this->m_sprite.setColor(this->color.multiplied_color(this->multiplied_color));
	}
	void qsf::sprite::set_multiplied_alpha(qpl::u8 alpha) {
		this->multiplied_color.a = alpha;
		this->m_sprite.setColor(this->color.multiplied_color(this->multiplied_color));
	}
	void qsf::sprite::set_position(qpl::vector2f position) {
		this->m_sprite.setPosition(position);
	}
	void qsf::sprite::set_center(qpl::vector2f position) {
		this->m_sprite.setPosition(position);
		qsf::centerize_sprite(this->m_sprite);
	}
	void qsf::sprite::set_scale(qpl::vector2f scale) {
		this->m_sprite.setScale(scale);
	}
	void qsf::sprite::set_scale(qpl::f32 scale) {
		this->m_sprite.setScale(scale, scale);
	}
	void qsf::sprite::set_origin(qpl::vector2f origin) {
		this->m_sprite.setOrigin(origin);
	}
	void qsf::sprite::set_rotation(qpl::f32 rotation) {
		this->m_sprite.setRotation(rotation);
	}
	qpl::rgb qsf::sprite::get_color() const {
		return this->color;
	}
	qpl::rgb qsf::sprite::get_multiplied_color() const {
		return this->multiplied_color;
	}
	qpl::vector2f qsf::sprite::get_position() const {
		return this->m_sprite.getPosition();
	}
	qpl::vector2f qsf::sprite::get_scale() const {
		return this->m_sprite.getScale();
	}
	qpl::vector2f qsf::sprite::get_origin() const {
		return this->m_sprite.getOrigin();
	}
	qpl::f32 qsf::sprite::get_rotation() const {
		return this->m_sprite.getRotation();
	}
	qpl::hitbox qsf::sprite::get_hitbox() const {
		return qsf::get_sprite_hitbox(this->m_sprite);
	}
	qpl::vector2f qsf::sprite::get_center() const {
		return qsf::get_sprite_center(this->m_sprite);
	}

	void qsf::sprite::centerize_origin() {
		qsf::centerize_sprite_origin(this->m_sprite);
	}
	void qsf::sprite::move(qpl::vector2f delta) {
		this->m_sprite.move(delta);
	}
	void qsf::sprite::move_scaled(qpl::vector2f delta) {
		this->m_sprite.move(delta / this->get_scale());
	}
	void qsf::sprite::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		window.draw(this->m_sprite, states);
	}

	qsf::sprite::operator sf::Sprite& () {
		return this->m_sprite;
	}
	qsf::sprite::operator const sf::Sprite& () const {
		return this->m_sprite;
	}
	qsf::sprite& qsf::sprite::operator=(const sf::Sprite& sprite) {
		this->m_sprite = sprite;
		this->color = this->multiplied_color = qpl::rgb::white;
		return *this;
	}


	void qsf::render_texture::set_antialiasing(qpl::u32 antialiasing) {
		this->m_settings.antialiasingLevel = antialiasing;
	}
	void qsf::render_texture::resize(qpl::vector2i dimension, bool resize_with_window) {
		//if (this->m_texture) {
		//	this->m_texture.get_deleter();
		//}
		//this->m_texture = std::make_unique<sf::RenderTexture>();
		//if (this->m_texture) {
		//	this->m_texture->~RenderTexture();
		//	delete this->m_texture;
		//}
		//this->m_texture = new sf::RenderTexture;
		if (this->m_create_size.x < dimension.x || this->m_create_size.y < dimension.y) {
			this->m_create_size = dimension;
			this->m_texture.create(dimension.x, dimension.y, this->m_settings);
			this->m_sprite.set_texture(this->m_texture.getTexture());
		}

		sf::IntRect rect{ 0, 0, dimension.x, dimension.y };
		this->m_sprite.m_sprite.setTextureRect(rect);
		this->m_changed = true;
		this->m_resize_with_window = resize_with_window;
	}
	void qsf::render_texture::set_smooth(bool b) {
		this->m_smooth = b;
		this->m_texture.setSmooth(b);
	}
	void qsf::render_texture::enable_smooth() {
		this->set_smooth(true);
	}
	void qsf::render_texture::disable_smooth() {
		this->set_smooth(false);
	}
	bool qsf::render_texture::is_smooth() const {
		return this->m_smooth;
	}
	void qsf::render_texture::enable_resize_with_window() {
		this->m_resize_with_window = true;
	}
	void qsf::render_texture::disable_resize_with_window() {
		this->m_resize_with_window = false;
	}
	bool qsf::render_texture::is_resize_with_window_enabled() const {
		return this->m_resize_with_window;
	}
	void qsf::render_texture::enable_clear_with_window() {
		this->m_clear_with_window = true;
	}
	void qsf::render_texture::disable_clear_with_window() {
		this->m_clear_with_window = false;
	}
	bool qsf::render_texture::is_clear_with_window_enabled() const {
		return this->m_clear_with_window;
	}

	void qsf::render_texture::set_color(qpl::rgb color) {
		this->m_sprite.set_color(color);
	}
	void qsf::render_texture::set_multiplied_color(qpl::rgb color) {
		this->m_sprite.set_multiplied_color(color);
	}
	void qsf::render_texture::set_position(qpl::vector2f position) {
		this->m_sprite.set_position(position);
	}
	void qsf::render_texture::set_scale(qpl::vector2f scale) {
		this->m_sprite.set_scale(scale);
	}
	void qsf::render_texture::set_scale(qpl::f32 scale) {
		this->m_sprite.set_scale(scale);
	}
	void qsf::render_texture::set_origin(qpl::vector2f origin) {
		this->m_sprite.set_origin(origin);
	}
	void qsf::render_texture::set_rotation(qpl::f32 rotation) {
		this->m_sprite.set_rotation(rotation);
	}
	qpl::rgb qsf::render_texture::get_color() const {
		return this->m_sprite.get_color();
	}
	qpl::rgb qsf::render_texture::get_multiplied_color() const {
		return this->m_sprite.get_multiplied_color();
	}
	qpl::vector2f qsf::render_texture::get_position() const {
		return this->m_sprite.get_position();
	}
	qpl::vector2f qsf::render_texture::get_scale() const {
		return this->m_sprite.get_scale();
	}
	qpl::vector2f qsf::render_texture::get_origin() const {
		return this->m_sprite.get_origin();
	}
	qpl::f32 qsf::render_texture::get_rotation() const {
		return this->m_sprite.get_rotation();
	}
	qpl::vector2f qsf::render_texture::get_dimension() const {
		return this->m_texture.getSize();
	}
	qpl::vector2f qsf::render_texture::get_center() const {
		return this->m_sprite.get_position() + this->get_dimension() / 2;
	}

	void qsf::render_texture::move(qpl::vector2f delta) {
		this->m_sprite.move(delta);
	}
	void qsf::render_texture::move_scaled(qpl::vector2f delta) {
		this->m_sprite.move(delta / this->get_scale());
	}
	const qsf::sprite& qsf::render_texture::get_sprite() const {
		if (this->m_changed) {
			this->apply();
		}
		return this->m_sprite;
	}
	void qsf::render_texture::clear() {
		this->m_texture.clear();
	}
	void qsf::render_texture::display() {
		this->m_texture.display();
	}

	const sf::RenderStates& qsf::render_texture::get_render_states() const {
		return this->m_states;
	}
	const sf::Texture& qsf::render_texture::get_texture() const {
		return this->m_texture.getTexture();
	}

	void qsf::render_texture::set_shader(const std::string& name) {
		this->m_states.shader = &qsf::get_shader(name);
	}
	void qsf::render_texture::set_shader(sf::Shader& shader) {
		this->m_states.shader = &shader;
	}
	void qsf::render_texture::unbind_shader() {
		this->m_states.shader = nullptr;
	}
	void qsf::render_texture::apply() const {
		this->m_texture.display();
		this->m_changed = false;
	}

	void qsf::pixel_image::set_array_dimension(qpl::vector2u dimension) {
		this->positions_set = this->array_dimension == dimension;
		this->array_dimension = dimension;
		this->vertices.resize((this->array_dimension.x - 1) * (this->array_dimension.y - 1) * 4);

		this->create_positions();
	}
	void qsf::pixel_image::set_pixel_dimension(qpl::vector2f dimension) {
		this->positions_set = this->pixel_dimension == dimension;
		this->pixel_dimension = dimension;
		this->create_positions();
	}
	void qsf::pixel_image::set_position(qpl::vector2f position) {
		this->positions_set = this->position == position;
		this->position = position;
		this->create_positions();
	}
	void qsf::pixel_image::create_positions() {
		if (!this->positions_set) {
			this->positions_set = true;

			if (this->vertices.size()) {
				for (qpl::u32 y = 0u; y < this->array_dimension.y - 1; ++y) {
					for (qpl::u32 x = 0u; x < this->array_dimension.x - 1; ++x) {
						auto index = (y * (this->array_dimension.x - 1) + x);

						this->vertices[index * 4 + 0].position = (this->position + this->pixel_dimension * qpl::vector2f(x, y));
						this->vertices[index * 4 + 1].position = (this->position + this->pixel_dimension * qpl::vector2f(x, y + 1.0f));
						this->vertices[index * 4 + 2].position = (this->position + this->pixel_dimension * qpl::vector2f(x + 1.0f, y + 1.0f));
						this->vertices[index * 4 + 3].position = (this->position + this->pixel_dimension * qpl::vector2f(x + 1.0f, y));
					}
				}
			}

		}
	}
	void qsf::pixel_image::set(qpl::size x, qpl::size y, qpl::rgb color) {
		auto index = (y * this->array_dimension.y + x);

		auto above_index = ((qpl::signed_cast(y) - 1) * (qpl::signed_cast(this->array_dimension.x) - 1) + qpl::signed_cast(x)) * 4;
		if (above_index - 2 >= 0 && above_index - 2 < qpl::signed_cast(this->vertices.size())) {
			this->vertices[above_index - 2].color = color;
		}
		if (above_index + 1 >= 0 && above_index + 1 < qpl::signed_cast(this->vertices.size())) {
			this->vertices[above_index + 1].color = color;
		}
		auto below_index = ((qpl::signed_cast(y)) * (qpl::signed_cast(this->array_dimension.x) - 1) + qpl::signed_cast(x)) * 4;
		if (below_index >= 0 && below_index < qpl::signed_cast(this->vertices.size())) {
			this->vertices[below_index].color = color;
		}
		if (below_index - 1 >= 0 && below_index - 1 < qpl::signed_cast(this->vertices.size())) {
			this->vertices[below_index - 1].color = color;
		}
	}
	void qsf::pixel_image::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		this->vertices.draw(window, states);
	}


	qsf::endl_type endl;

	void qsf::text_stream_old::apply_state() {
		this->objects.back().back().text.set_character_size(this->states.back().character_size);
		this->objects.back().back().text.set_font(this->states.back().font);
		this->objects.back().back().text.set_style(this->states.back().style);
		this->objects.back().back().text.set_color(this->states.back().color);
		this->objects.back().back().text.set_letter_spacing(this->states.back().letter_spacing);
		this->objects.back().back().text.set_outline_thickness(this->states.back().outline_thickness);
		this->objects.back().back().text.set_outline_color(this->states.back().outline_color);
	}
	void qsf::text_stream_old::new_line() {
		this->objects.push_back({});

		while (this->states.size() > 1 && this->states.back().duration != qsf::text_stream_old::duration_type::end_of_line) {
			this->states.pop_back();
		}
	}
	void qsf::text_stream_old::clear() {
		this->sprites.clear();
		this->objects.clear();
		this->states.resize(1);
		this->changed = true;
	}
	text_stream_old& qsf::text_stream_old::operator<<(const sf::Texture& texture) {
		return this->add_texture(texture);
	}
	text_stream_old& qsf::text_stream_old::operator<<(const qsf::sprite& sprite) {
		return this->add_sprite(sprite);
	}
	text_stream_old& qsf::text_stream_old::operator<<(const std::string& string) {
		return this->add_string(string);
	}
	text_stream_old& qsf::text_stream_old::operator<<(const std::wstring& string) {
		return this->add_string(string);
	}
	text_stream_old& qsf::text_stream_old::add_texture(const sf::Texture& texture) {

		this->prepare_next_entry();
		auto hitbox = this->objects.back().back().hitbox;

		auto pos = hitbox.position + qpl::vector2f(hitbox.dimension.x + this->states.back().spacing, 0);
		if (this->states.back().absolute_spacing != qpl::f32_max) {
			auto diff = this->states.back().absolute_spacing - pos.x;
			pos += diff;
		}

		this->objects.back().back().shift = this->states.back().shift;
		this->objects.back().back().shift_before = this->states.back().shift;
		this->objects.back().back().sprite_index = qpl::u32_cast(this->sprites.size());

		this->sprites.push_back(qsf::sprite{});
		this->sprites.back().set_texture(texture);
		this->sprites.back().set_position(pos);

		auto s = qpl::vector2f(1.0, 1.0);
		if (this->states.back().scaling != qpl::vector2f(qpl::f32_max, qpl::f32_max)) {
			s = this->states.back().scaling;
		}
		this->sprites.back().set_scale(s);

		auto sprite_hitbox = qsf::get_sprite_hitbox(this->sprites.back());

		this->objects.back().back().hitbox = sprite_hitbox;

		this->sprites.back().set_color(this->states.back().color);

		this->changed = true;


		if (this->states.size() > 1) {
			if (this->states.back().duration == qsf::text_stream_old::duration_type::next_entry) {
				this->states.pop_back();
			}
		}

		return *this;
	}

	text_stream_old& qsf::text_stream_old::add_sprite(const qsf::sprite& sprite) {

		this->prepare_next_entry();

		auto line_hitbox = this->line_hitbox(this->lines() - 1);
		auto pos = line_hitbox.position + qpl::vector2f(line_hitbox.dimension.x + this->states.back().spacing, 0);
		this->objects.back().back().absolute = (this->states.back().absolute_spacing != qpl::f32_max);
		if (this->objects.back().back().absolute) {
			auto diff = this->states.back().absolute_spacing - pos.x;
			pos += diff;

		}


		this->objects.back().back().shift = this->states.back().shift;
		this->objects.back().back().shift_before = this->states.back().shift;
		this->objects.back().back().sprite_index = qpl::u32_cast(this->sprites.size());

		this->sprites.push_back(sprite);
		this->sprites.back().set_position(pos);

		auto s = sprite.get_scale();
		if (this->states.back().scaling != qpl::vector2f(qpl::f32_max, qpl::f32_max)) {
			s = this->states.back().scaling;
		}
		this->sprites.back().set_scale(s);

		auto sprite_hitbox = qsf::get_sprite_hitbox(this->sprites.back());

		this->objects.back().back().hitbox = sprite_hitbox;
		this->sprites.back().set_color(this->states.back().color);

		this->changed = true;


		if (this->states.size() > 1) {
			if (this->states.back().duration == qsf::text_stream_old::duration_type::next_entry) {
				this->states.pop_back();
			}
		}

		return *this;
	}
	text_stream_old& qsf::text_stream_old::add_string(const std::string& string, bool has_no_newline, bool pop_state) {
		if (string.empty()) return *this;
		if (!has_no_newline) {
			qpl::u32 last = 0u;
			for (qpl::u32 i = 0u; !has_no_newline && i < string.size(); ++i) {
				if (string[i] == '\n') {
					qpl::i32 size = qpl::i32_cast(i) - 1 - qpl::i32_cast(last);
					if (size > 0) {
						std::string sub = std::string(string.begin() + last, string.begin() + i);
						this->add_string(sub, true, false);
					}
					while (i < string.length() && string[i] == '\n') {
						this->new_line();
						++i;
					}
					last = i;
				}
			}

			if (string.back() == '\n') {
				if (this->states.size() > 1) {
					if (this->states.back().duration == qsf::text_stream_old::duration_type::next_entry) {
						this->states.pop_back();
					}
				}
			}
			std::string sub = std::string(string.begin() + last, string.end());
			return this->add_string(sub, true, true);
		}
		if (!string.empty()) {
			this->prepare_next_entry();


			auto hitbox = this->objects.back().back().hitbox;

			this->apply_state();
			this->objects.back().back().text.set_string(string);
			this->objects.back().back().shift = this->states.back().shift;
			this->objects.back().back().shift_before = this->states.back().shift;


			this->objects.back().back().text.set_position(hitbox.position + hitbox.dimension.just_x() + qpl::vector2f(this->states.back().letter_spacing + this->states.back().spacing, 0));
			this->objects.back().back().hitbox = this->objects.back().back().text.get_visible_hitbox();
			this->objects.back().back().absolute = (this->states.back().absolute_spacing != qpl::f32_max);
			if (this->objects.back().back().absolute) {
				auto diff = this->states.back().absolute_spacing - this->objects.back().back().hitbox.position.x;
				this->objects.back().back().text.move(qpl::vec(diff, 0));
				this->objects.back().back().hitbox.move(qpl::vec(diff, 0));
			}


			this->changed = true;
		}

		if (pop_state && this->states.size() > 1) {
			if (this->states.back().duration == qsf::text_stream_old::duration_type::next_entry) {
				this->states.pop_back();
			}
		}
		return *this;
	}

	text_stream_old& qsf::text_stream_old::add_string(const std::wstring& string, bool has_no_newline, bool pop_state) {
		if (!has_no_newline) {
			qpl::u32 last = 0u;
			for (qpl::u32 i = 0u; !has_no_newline && i < string.size(); ++i) {
				if (string[i] == L'\n') {
					qpl::i32 size = qpl::i32_cast(i) - 1 - qpl::i32_cast(last);
					if (size > 0) {
						std::wstring sub = std::wstring(string.begin() + last, string.begin() + i);
						this->add_string(sub, true, false);
					}
					while (i < string.length() && string[i] == L'\n') {
						this->new_line();
						++i;
					}
					last = i;
				}
			}
			if (string.back() == L'\n') {
				if (this->states.size() > 1) {
					if (this->states.back().duration == qsf::text_stream_old::duration_type::next_entry) {
						this->states.pop_back();
					}
				}
			}
			std::wstring sub = std::wstring(string.begin() + last, string.end());
			return this->add_string(sub, true, false);
		}
		if (!string.empty()) {
			this->prepare_next_entry();

			auto hitbox = this->objects.back().back().hitbox;
			this->apply_state();
			this->objects.back().back().text.m_string = qpl::wstring_to_string(string);
			sf::String s = string.c_str();
			this->objects.back().back().text.m_text.setString(s);
			this->objects.back().back().shift = this->states.back().shift;
			this->objects.back().back().shift_before = this->states.back().shift;
			this->objects.back().back().text.set_position(hitbox.position + hitbox.dimension.just_x() + qpl::vector2f(this->states.back().letter_spacing + this->states.back().spacing, 0));
			this->objects.back().back().hitbox = this->objects.back().back().text.get_visible_hitbox();
			this->objects.back().back().absolute = (this->states.back().absolute_spacing != qpl::f32_max);
			if (this->objects.back().back().absolute) {
				auto diff = this->states.back().absolute_spacing - this->objects.back().back().hitbox.position.x;
				this->objects.back().back().text.move(qpl::vec(diff, 0));
				this->objects.back().back().hitbox.move(qpl::vec(diff, 0));
			}

			//this->objects.back().back().set_position(hitbox.position + hitbox.dimension.just_x() + qpl::vector2f(this->states.back().letter_spacing, -this->objects.back().back().offset().y));
			this->changed = true;
		}
		if (pop_state && this->states.size() > 1) {
			if (this->states.back().duration == qsf::text_stream_old::duration_type::next_entry) {
				this->states.pop_back();
			}
		}

		return *this;
	}
	qpl::hitbox qsf::text_stream_old::line_text_hitbox(qpl::size index) const {
		if (index >= this->objects.size()) {
			return {};
		}

		bool found_first = false;
		qpl::hitbox first_hitbox;
		qpl::hitbox last_hitbox;
		qpl::f32 min_y = 0;
		qpl::f32 max_y = 0;

		for (qpl::u32 i = 0u; i < this->objects[index].size(); ++i) {
			if (this->objects[index][i].is_text()) {
				auto hitbox = this->objects[index][i].hitbox;

				if (!found_first) {
					found_first = true;
					first_hitbox = hitbox;
					min_y = first_hitbox.position.y;
					max_y = first_hitbox.position.y + first_hitbox.dimension.y;
				}

				if (min_y > hitbox.position.y) {
					min_y = hitbox.position.y;
				}
				if (max_y < hitbox.position.y + hitbox.dimension.y) {
					max_y = hitbox.position.y + hitbox.dimension.y;
				}
				last_hitbox = hitbox;
			}
		}

		if (!found_first) {
			qpl::hitbox result;
			if (index < this->objects.size()) {
				if (this->objects[index].size()) {
					result.position = this->objects[index].front().hitbox.position;
				}
			}
			result.dimension = { 0,0 };
			return result;
		}

		qpl::hitbox hitbox;
		hitbox.position.x = first_hitbox.position.x;
		hitbox.position.y = min_y;
		hitbox.dimension.y = max_y - min_y;
		hitbox.dimension.x = (last_hitbox.position.x + last_hitbox.dimension.x) - hitbox.position.x;

		//if (this->objects[index].size() == 1 && this->objects[index].front().text.get_string().empty()) {
		//	hitbox.dimension.y = qpl::f32_cast(this->objects[index].back().text.get_character_size());
		//}
		return hitbox;
	}

	qpl::hitbox qsf::text_stream_old::line_sprite_hitbox(qpl::size index) const {
		if (index >= this->objects.size()) {
			return {};
		}
		if (this->objects[index].empty()) {
			return {};
		}

		bool found_first = false;
		qpl::hitbox first_hitbox;
		qpl::hitbox last_hitbox;
		qpl::f32 min_y = 0;
		qpl::f32 max_y = 0;

		for (qpl::u32 i = 0u; i < this->objects[index].size(); ++i) {
			if (!this->objects[index][i].is_text()) {
				auto hitbox = this->objects[index][i].hitbox;

				if (!found_first) {
					found_first = true;
					first_hitbox = hitbox;
					min_y = first_hitbox.position.y;
					max_y = first_hitbox.position.y + first_hitbox.dimension.y;
				}

				if (min_y > hitbox.position.y) {
					min_y = hitbox.position.y;
				}
				if (max_y < hitbox.position.y + hitbox.dimension.y) {
					max_y = hitbox.position.y + hitbox.dimension.y;
				}
				last_hitbox = hitbox;
			}
		}

		if (!found_first) {
			qpl::hitbox result;
			result.position = this->objects[index].front().hitbox.position;
			result.dimension = { 0,0 };
			return result;
		}

		qpl::hitbox hitbox;
		hitbox.position.x = first_hitbox.position.x;
		hitbox.position.y = min_y;
		hitbox.dimension.y = max_y - min_y;
		hitbox.dimension.x = (last_hitbox.position.x + last_hitbox.dimension.x) - hitbox.position.x;

		//if (this->objects[index].size() == 1 && this->objects[index].front().text.get_string().empty()) {
		//	hitbox.dimension.y = qpl::f32_cast(this->objects[index].back().text.get_character_size());
		//}
		return hitbox;

	}
	qpl::hitbox qsf::text_stream_old::line_hitbox(qpl::size index) const {
		if (index >= this->objects.size()) {
			return {};
		}
		if (this->objects[index].empty()) {
			return {};
		}
		auto front_hitbox = this->objects[index].front().hitbox;
		qpl::f32 min_y = front_hitbox.position.y;
		qpl::f32 max_y = front_hitbox.position.y + front_hitbox.dimension.y;

		for (qpl::u32 i = 1u; i < this->objects[index].size(); ++i) {
			auto hitbox = this->objects[index][i].hitbox;
			if (min_y > hitbox.position.y) {
				min_y = hitbox.position.y;
			}
			if (max_y < hitbox.position.y + hitbox.dimension.y) {
				max_y = hitbox.position.y + hitbox.dimension.y;
			}
		}

		qpl::hitbox hitbox;
		hitbox.position.x = front_hitbox.position.x;
		hitbox.position.y = min_y;
		hitbox.dimension.y = max_y - min_y;
		auto back_hitbox = this->objects[index].back().hitbox;
		hitbox.dimension.x = (back_hitbox.position.x + back_hitbox.dimension.x) - hitbox.position.x;

		if (this->objects[index].size() == 1 && this->objects[index].front().text.get_string().empty()) {
			hitbox.dimension.y = qpl::f32_cast(this->objects[index].back().text.get_character_size());
		}
		return hitbox;

	}

	qpl::hitbox qsf::text_stream_old::line_visible_hitbox(qpl::size index) const {
		if (index >= this->objects.size()) {
			return {};
		}
		qpl::hitbox front_hitbox;
		if (this->objects[index].front().is_text()) {
			front_hitbox = this->objects[index].front().text.get_visible_hitbox();
		}
		else {
			front_hitbox = this->objects[index].front().hitbox;
		}
		qpl::f32 min_y = front_hitbox.position.y;
		qpl::f32 max_y = front_hitbox.position.y + front_hitbox.dimension.y;

		for (qpl::u32 i = 1u; i < this->objects[index].size(); ++i) {
			qpl::hitbox hitbox;
			if (this->objects[index][i].is_text()) {
				hitbox = this->objects[index][i].text.get_visible_hitbox();
			}
			else {
				hitbox = this->objects[index][i].hitbox;
			}

			if (min_y > hitbox.position.y) {
				min_y = hitbox.position.y;
			}
			if (max_y < hitbox.position.y + hitbox.dimension.y) {
				max_y = hitbox.position.y + hitbox.dimension.y;
			}
		}

		qpl::hitbox hitbox;
		hitbox.position.x = front_hitbox.position.x;
		hitbox.position.y = min_y;
		hitbox.dimension.y = max_y - min_y;
		qpl::hitbox back_hitbox;
		if (this->objects[index].back().is_text()) {
			back_hitbox = this->objects[index].back().text.get_visible_hitbox();
		}
		else {
			back_hitbox = this->objects[index].back().hitbox;
		}
		hitbox.dimension.x = (back_hitbox.position.x + back_hitbox.dimension.x) - hitbox.position.x;

		if (this->objects[index].size() == 1 && this->objects[index].front().text.get_string().empty()) {
			hitbox.dimension.y = qpl::f32_cast(this->objects[index].back().text.get_character_size());
		}
		return hitbox;

	}

	void qsf::text_stream_old::centerize_line(qpl::size index, qpl::u32 loop) const {

		auto sprite_hitbox = this->line_sprite_hitbox(index);
		auto text_hitbox = this->line_text_hitbox(index);

		qpl::hitbox hitbox;
		hitbox.position.x = qpl::min(sprite_hitbox.position.x, text_hitbox.position.x);
		hitbox.position.y = qpl::min(sprite_hitbox.position.y, text_hitbox.position.y);
		hitbox.dimension.x = qpl::max(sprite_hitbox.dimension.x, text_hitbox.dimension.x);
		hitbox.dimension.y = qpl::max(sprite_hitbox.dimension.y, text_hitbox.dimension.y);

		bool not_fully_alligned_yet = true;

		for (auto& line : this->objects[index]) {

			if (line.is_text()) {
				auto diff = hitbox.dimension.y - text_hitbox.dimension.y;

				auto real_diff = (line.hitbox.position.y - (hitbox.position.y + diff / 2));
				if (real_diff) {
					not_fully_alligned_yet = false;
				}

				auto pos = qpl::vector2f(line.hitbox.position.x, hitbox.position.y) + qpl::vector2f(0, diff / 2);
				line.hitbox.position = pos;
				line.text.set_position(pos + line.shift);

			}
			else {
				auto sprite_hitbox = line.hitbox;
				auto diff = hitbox.dimension.y - sprite_hitbox.dimension.y;


				auto real_diff = (line.hitbox.position.y - (hitbox.position.y + diff / 2));
				if (real_diff) {
					not_fully_alligned_yet = false;
				}

				line.hitbox.position = qpl::vector2f(sprite_hitbox.position.x, hitbox.position.y) + qpl::vector2f(0, diff / 2);
				this->sprites[line.sprite_index].set_position(line.hitbox.position + line.shift);
			}
		}

		if (!not_fully_alligned_yet && loop < 2) {
			this->centerize_line(index, loop + 1);
		}
	}
	void qsf::text_stream_old::centerize_lines() const {
		for (qpl::u32 i = 0; i < this->lines(); ++i) {
			this->centerize_line(i);
		}
	}
	qpl::size qsf::text_stream_old::size() const {
		qpl::size result = 0u;
		for (auto& i : this->objects) {
			result += i.size();
		}
		return result;
	}
	qpl::size qsf::text_stream_old::lines() const {
		return this->objects.size();
	}
	qsf::text_stream_old::object_t& qsf::text_stream_old::operator[](qpl::size index) {
		qpl::size result = 0u;
		for (auto& i : this->objects) {
			if (index >= result && index < result + i.size()) {
				return i[index - result];
			}
			result += i.size();
		}
		return this->objects.back().back();
	}
	const qsf::text_stream_old::object_t& qsf::text_stream_old::operator[](qpl::size index) const {
		qpl::size result = 0u;
		for (auto& i : this->objects) {
			if (index >= result && index < result + i.size()) {
				return i[index - result];
			}
			result += i.size();
		}
		return this->objects.back().back();
	}

	qpl::size qsf::text_stream_old::text_size() const {
		return this->size() - this->sprites.size();
	}
	qsf::text& qsf::text_stream_old::get_text(qpl::size index) {
		if (index >= this->text_size()) {
			throw std::exception(qpl::to_string("qsf::text_stream_old::get_text(", index, "): size of text is only ", this->text_size()).c_str());
		}

		qpl::size ctr = 0u;
		for (auto& j : this->objects) {
			for (auto& i : j) {
				if (i.is_text()) {
					if (ctr == index) {
						return i.text;
					}

					++ctr;
				}
			}
		}
		return this->objects.back().back().text;
	}
	const qsf::text& qsf::text_stream_old::get_text(qpl::size index) const {
		if (index >= this->text_size()) {
			throw std::exception(qpl::to_string("qsf::text_stream_old::get_text(", index, "): size of text is only ", this->text_size()).c_str());
		}

		qpl::size ctr = 0u;
		for (auto& j : this->objects) {
			for (auto& i : j) {
				if (i.is_text()) {
					if (ctr == index) {
						return i.text;
					}
					++ctr;
				}
			}
		}
		return this->objects.back().back().text;
	}

	qsf::text& qsf::text_stream_old::get_text(qpl::size line, qpl::size index) {
		if (line >= this->lines()) {
			throw std::exception(qpl::to_string("qsf::text_stream_old::get_text(", line, ", ", index, "): total line size is only ", this->lines()).c_str());
		}
		qpl::size ctr = 0u;
		for (auto& i : this->objects[line]) {
			if (i.is_text()) {
				if (ctr == index) {
					return i.text;
				}
				++ctr;
			}
		}
		throw std::exception(qpl::to_string("qsf::text_stream_old::get_text(", line, ", ", index, "): text size of line #", line, " is only ", ctr).c_str());
		return this->objects[line].back().text;
	}
	const qsf::text& qsf::text_stream_old::get_text(qpl::size line, qpl::size index) const {
		if (line >= this->lines()) {
			throw std::exception(qpl::to_string("qsf::text_stream_old::get_text(", line, ", ", index, "): total line size is only ", this->lines()).c_str());
		}
		qpl::size ctr = 0u;
		for (auto& i : this->objects[line]) {
			if (i.is_text()) {
				if (ctr == index) {
					return i.text;
				}
				++ctr;
			}
		}
		throw std::exception(qpl::to_string("qsf::text_stream_old::get_text(", line, ", ", index, "): text size of line #", line, " is only ", ctr).c_str());
		return this->objects[line].back().text;
	}

	qpl::size qsf::text_stream_old::sprite_size() const {
		return this->sprites.size();
	}
	qsf::sprite& qsf::text_stream_old::get_sprite(qpl::size index) {
		if (index >= this->sprite_size()) {
			throw std::exception(qpl::to_string("qsf::text_stream_old::get_sprite(", index, "): size of sprites is only ", this->sprite_size()).c_str());
		}

		return this->sprites[index];
	}
	const qsf::sprite& qsf::text_stream_old::get_sprite(qpl::size index) const {
		if (index >= this->sprite_size()) {
			throw std::exception(qpl::to_string("qsf::text_stream_old::get_sprite(", index, "): size of sprites is only ", this->sprite_size()).c_str());
		}

		return this->sprites[index];
	}
	qsf::sprite& qsf::text_stream_old::get_sprite(qpl::size line, qpl::size index) {
		if (line >= this->lines()) {
			throw std::exception(qpl::to_string("qsf::text_stream_old::get_sprite(", line, ", ", index, "): total line size is only ", this->lines()).c_str());
		}

		qpl::size ctr = 0u;
		for (auto& i : this->objects[line]) {
			if (!i.is_text()) {
				if (ctr == index) {
					return this->sprites[i.sprite_index];
				}
				++ctr;
			}
		}
		throw std::exception(qpl::to_string("qsf::text_stream_old::get_sprite(", line, ", ", index, "): sprite size of line #", line, " is only ", ctr).c_str());
		return this->sprites[this->objects[line].back().sprite_index];
	}
	const qsf::sprite& qsf::text_stream_old::get_sprite(qpl::size line, qpl::size index) const {
		if (line >= this->lines()) {
			throw std::exception(qpl::to_string("qsf::text_stream_old::get_sprite(", line, ", ", index, "): total line size is only ", this->lines()).c_str());
		}

		qpl::size ctr = 0u;
		for (auto& i : this->objects[line]) {
			if (!i.is_text()) {
				if (ctr == index) {
					return this->sprites[i.sprite_index];
				}
				++ctr;
			}
		}
		throw std::exception(qpl::to_string("qsf::text_stream_old::get_sprite(", line, ", ", index, "): sprite size of line #", line, " is only ", ctr).c_str());
		return this->sprites[this->objects[line].back().sprite_index];
	}


	qsf::text_stream_old::object_t& qsf::text_stream_old::get_object(qpl::size line, qpl::size index) {
		if (line >= this->lines()) {
			throw std::exception(qpl::to_string("qsf::text_stream_old::get_object(", line, ", ", index, "): total line size is only ", this->lines()).c_str());
		}
		if (line >= this->objects[line].size()) {
			throw std::exception(qpl::to_string("qsf::text_stream_old::get_object(", line, ", ", index, "): size of line #", line, " is only ", this->objects[line].size()).c_str());
		}
		return this->objects[line][index];
	}

	const qsf::text_stream_old::object_t& qsf::text_stream_old::get_object(qpl::size line, qpl::size index)const {
		if (line >= this->lines()) {
			throw std::exception(qpl::to_string("qsf::text_stream_old::get_object(", line, ", ", index, "): total line size is only ", this->lines()).c_str());
		}
		if (line >= this->objects[line].size()) {
			throw std::exception(qpl::to_string("qsf::text_stream_old::get_object(", line, ", ", index, "): size of line #", line, " is only ", this->objects[line].size()).c_str());
		}
		return this->objects[line][index];
	}

	std::vector<qsf::text_stream_old::object_t>& qsf::text_stream_old::line(qpl::size index) {
		return this->objects[index];
	}
	const std::vector<qsf::text_stream_old::object_t>& qsf::text_stream_old::line(qpl::size index) const {
		return this->objects[index];
	}

	void qsf::text_stream_old::finalize() const {
		if (this->changed) {
			this->centerize_lines();
			this->changed = false;
			this->calculate_visible_hitbox();
		}
	}
	void qsf::text_stream_old::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		this->finalize();
		for (auto& i : this->objects) {
			for (auto& j : i) {
				if (j.visible) {
					if (j.is_text()) {
						j.text.draw(window, states);
					}
					else {
						window.draw(this->sprites[j.sprite_index], states);
					}
				}
			}
		}
	}
	void qsf::text_stream_old::set_font(const std::string& font) {
		for (auto& i : this->states) {
			i.font = font;
		}
	}
	void qsf::text_stream_old::set_color(qpl::rgb color) {
		for (auto& i : this->states) {
			i.color = color;
		}
	}
	void qsf::text_stream_old::set_alpha(qpl::u8 alpha) {
		for (auto& j : this->objects) {
			for (auto& i : j) {
				if (i.is_text()) {
					i.text.set_multiplied_color(qpl::rgb::white.with_alpha(alpha));
				}
			}
		}
		for (auto& i : this->sprites) {
			i.set_multiplied_color(qpl::rgb::white.with_alpha(alpha));
		}
	}
	void qsf::text_stream_old::set_multiplied_color(qpl::rgb color) {
		for (auto& j : this->objects) {
			for (auto& i : j) {
				if (i.is_text()) {
					i.text.set_multiplied_color(color);
				}
			}
		}
		for (auto& i : this->sprites) {
			i.set_multiplied_color(color);
		}
	}
	void qsf::text_stream_old::set_style(qpl::u32 style) {
		for (auto& i : this->states) {
			i.style = style;
		}
	}
	void qsf::text_stream_old::set_character_size(qpl::u32 character_size) {
		for (auto& i : this->states) {
			i.character_size = character_size;
		}
	}

	void qsf::text_stream_old::set_line_spacing(qpl::f32 spacing) {
		this->line_spacing = spacing;
	}
	void qsf::text_stream_old::set_outline_thickness(qpl::f32 thickness) {
		for (auto& i : this->states) {
			i.outline_thickness = thickness;
		}
	}
	void qsf::text_stream_old::set_outline_color(qpl::rgb color) {
		for (auto& i : this->states) {
			i.outline_color = color;
		}
	}
	void qsf::text_stream_old::set_letter_spacing(qpl::f32 spacing) {
		for (auto& i : this->states) {
			i.letter_spacing = spacing;
		}
	}
	void qsf::text_stream_old::set_position(qpl::vector2f position) {
		auto diff = position - this->position;
		this->position = position;
		this->move(diff);
	}
	void qsf::text_stream_old::move(qpl::vector2f delta) {

		if (delta != qpl::vector2f::zero()) {

			for (auto& i : this->objects) {
				for (auto& j : i) {
					j.hitbox.move(delta);
					if (j.is_text()) {
						j.text.move(delta);
					}
					else {
						this->sprites[j.sprite_index].move(delta);
					}
				}
			}
		}
	}
	void qsf::text_stream_old::calculate_visible_hitbox() const {
		if (!this->lines()) {
			this->visible_hitbox = {};
		}

		qpl::f32 max_x = qpl::f32_min;
		qpl::vector2f first_pos;
		qpl::f32 last_y = 0;

		bool first = true;
		for (qpl::u32 i = 0u; i < this->lines(); ++i) {
			if (this->objects[i].size()) {
				auto hitbox = this->line_hitbox(i);
				if (first) {
					first_pos = hitbox.position;
					first = false;
				}
				max_x = qpl::max(max_x, hitbox.dimension.x);
				last_y = hitbox.position.y + hitbox.dimension.y;
			}
		}
		this->visible_hitbox.position = first_pos;
		this->visible_hitbox.dimension = qpl::vector2f(max_x, last_y - first_pos.y);
	}
	qpl::hitbox qsf::text_stream_old::get_visible_hitbox() const {
		if (this->changed) {
			this->calculate_visible_hitbox();
		}
		return this->visible_hitbox;
	}


	void qsf::text_stream_old::update_size_changes() {

		qpl::vector2f delta = qpl::vector2f(0, 0);

		for (qpl::u32 line = 0u; line < this->objects.size(); ++line) {
			delta.x = 0.0f;

			auto line_before = this->line_hitbox(line);

			for (auto& i : this->objects[line]) {

				if (i.absolute) {
					delta.x = 0;
				}

				qpl::hitbox hitbox;
				if (i.is_text()) {
					if (i.text.string().empty()) {
						hitbox = i.hitbox;
					}
					else {
						hitbox = i.text.get_visible_hitbox();
						hitbox.position -= i.text.get_offset();
					}
				}
				else {
					hitbox = qsf::get_sprite_hitbox(this->sprites[i.sprite_index]);
				}
				hitbox.position -= i.shift_before;

				auto x_before = i.hitbox.dimension.x + i.hitbox.position.x;
				auto x_now = hitbox.dimension.x + hitbox.position.x;


				auto diff = x_now - x_before;

				i.hitbox = hitbox;

				if (delta.x) {
					if (i.is_text()) {
						i.text.move(delta);
						i.hitbox.move(delta);
					}
					else {
						this->sprites[i.sprite_index].move(delta);
						i.hitbox.move(delta);
					}
				}

				delta.x += diff;

			}

			auto line_now = this->line_hitbox(line);


			auto x_before = line_before.dimension.y + line_before.position.y;
			auto x_now = line_now.dimension.y + line_now.position.y;

			auto diff = x_now - x_before;


			delta.y += diff;
		}
		this->changed = true;
	}
	void qsf::text_stream_old::prepare_next_entry() {
		if (this->objects.empty()) {
			this->objects.push_back({});
			this->objects.back().push_back({});
			this->objects.back().back().hitbox.set_position(this->position);

		}
		else if (this->objects.back().empty()) {
			auto hitbox = this->line_hitbox(this->lines() - 2);

			this->objects.back().push_back({});
			this->objects.back().back().hitbox.position = qpl::vector2f(hitbox.position + qpl::vector2f(0, hitbox.dimension.y + this->line_spacing));
			//this->apply_state();
			if (this->lines() > 1 && this->objects[this->lines() - 2].front().is_text()) {
				this->objects.back().back().hitbox.position.x -= (this->objects[this->lines() - 2].front().text.get_offset().x + 1);
			}
		}
		else {
			auto hitbox = this->objects.back().back().hitbox;
			this->objects.back().push_back({});
			this->objects.back().back().hitbox = hitbox;
		}
	}


	void qsf::tile_map::set_texture_ptr(const sf::Texture& texture, qpl::vector2u texture_tile_dimension) {
		this->texture_ptr = &texture;
		this->texture_tile_dimension = texture_tile_dimension;
		this->position_tile_dimension = this->texture_tile_dimension;
		this->texture_ptr_set = true;
	}
	void qsf::tile_map::set_texture_ptr(const sf::Texture& texture, qpl::u32 texture_tile_width) {
		this->set_texture_ptr(texture, qpl::vector2u(texture_tile_width, texture_tile_width));
	}
	void qsf::tile_map::set_position(qpl::vector2f position) {
		this->position = position;
	}

	void qsf::tile_map::create(const std::vector<std::pair<qpl::u32, qpl::u32>>& indices, qpl::size width, qpl::rgb color) {
		if (!this->texture_ptr_set) {
			qpl::println("tile_map::create: texture_ptr not set");
			return;
		}

		this->chunks.clear();
		this->color = color;
		auto chunk_width = (width - 1) / this->max_chunk_size.x + 1;
		auto chunk_height = (indices.size() / width - 1) / this->max_chunk_size.y + 1;
		auto chunk_dim = chunk_width * chunk_height;

		this->chunk_width_count = chunk_width;
		this->chunks.resize(chunk_dim);
		for (auto& chunk : this->chunks) {
			chunk.setPrimitiveType(sf::Quads);
			chunk.resize(this->max_chunk_size.y * this->max_chunk_size.x * 4);
		}

		auto texture_row_tile_count = texture_ptr->getSize().x / this->texture_tile_dimension.x;

		if (color == qpl::rgb::white) {
			for (qpl::u32 i = 0; i < indices.size(); ++i) {

				auto [y, x] = qpl::div_mod(i, width);
				auto tile = indices[i];
				auto tile_x = (tile.first % texture_row_tile_count);
				auto tile_y = (tile.first / texture_row_tile_count);

				auto chunk_x = (i % width) / this->max_chunk_size.x;
				auto chunk_y = (i / width) / this->max_chunk_size.y;

				auto& chunk = this->chunks[chunk_x + chunk_y * chunk_width];

				auto chunk_index = ((i % width) % this->max_chunk_size.x) + ((i / width) % this->max_chunk_size.y) * this->max_chunk_size.x;

				chunk[chunk_index * 4 + 0].position = this->position + qpl::vector2u(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 1].position = this->position + qpl::vector2u((x + 1) * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 2].position = this->position + qpl::vector2u((x + 1) * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 3].position = this->position + qpl::vector2u(x * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);

				auto tex_0 = qpl::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_1 = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_2 = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				auto tex_3 = qpl::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);

				if (tile.second == 0b000) {
					chunk[chunk_index * 4 + 0].texCoords = tex_0;
					chunk[chunk_index * 4 + 1].texCoords = tex_1;
					chunk[chunk_index * 4 + 2].texCoords = tex_2;
					chunk[chunk_index * 4 + 3].texCoords = tex_3;
				}
				else if (tile.second == 0b001) {
					chunk[chunk_index * 4 + 0].texCoords = tex_1;
					chunk[chunk_index * 4 + 1].texCoords = tex_0;
					chunk[chunk_index * 4 + 2].texCoords = tex_3;
					chunk[chunk_index * 4 + 3].texCoords = tex_2;
				}
				else if (tile.second == 0b010) {
					chunk[chunk_index * 4 + 0].texCoords = tex_3;
					chunk[chunk_index * 4 + 1].texCoords = tex_2;
					chunk[chunk_index * 4 + 2].texCoords = tex_1;
					chunk[chunk_index * 4 + 3].texCoords = tex_0;
				}
				else if (tile.second == 0b011) {
					chunk[chunk_index * 4 + 0].texCoords = tex_2;
					chunk[chunk_index * 4 + 1].texCoords = tex_3;
					chunk[chunk_index * 4 + 2].texCoords = tex_0;
					chunk[chunk_index * 4 + 3].texCoords = tex_1;
				}
				else if (tile.second == 0b100) {
					chunk[chunk_index * 4 + 0].texCoords = tex_3;
					chunk[chunk_index * 4 + 1].texCoords = tex_0;
					chunk[chunk_index * 4 + 2].texCoords = tex_1;
					chunk[chunk_index * 4 + 3].texCoords = tex_2;
				}
				else if (tile.second == 0b101) {
					chunk[chunk_index * 4 + 0].texCoords = tex_2;
					chunk[chunk_index * 4 + 1].texCoords = tex_1;
					chunk[chunk_index * 4 + 2].texCoords = tex_0;
					chunk[chunk_index * 4 + 3].texCoords = tex_3;
				}
				else if (tile.second == 0b110) {
					chunk[chunk_index * 4 + 0].texCoords = tex_0;
					chunk[chunk_index * 4 + 1].texCoords = tex_3;
					chunk[chunk_index * 4 + 2].texCoords = tex_2;
					chunk[chunk_index * 4 + 3].texCoords = tex_1;
				}
				else if (tile.second == 0b111) {
					chunk[chunk_index * 4 + 0].texCoords = tex_1;
					chunk[chunk_index * 4 + 1].texCoords = tex_2;
					chunk[chunk_index * 4 + 2].texCoords = tex_3;
					chunk[chunk_index * 4 + 3].texCoords = tex_0;
				}
			}
		}
		else {
			for (qpl::u32 i = 0; i < indices.size(); ++i) {
				auto [y, x] = qpl::div_mod(i, width);
				auto tile = indices[i];
				auto tile_x = (tile.first % texture_row_tile_count);
				auto tile_y = (tile.first / texture_row_tile_count);

				auto chunk_x = (i % width) / this->max_chunk_size.x;
				auto chunk_y = (i / width) / this->max_chunk_size.y;

				auto& chunk = this->chunks[chunk_x + chunk_y * chunk_width];

				auto chunk_index = ((i % width) % this->max_chunk_size.x) + ((i / width) % this->max_chunk_size.y) * this->max_chunk_size.x;

				chunk[chunk_index * 4 + 0].position = this->position + qpl::vector2u(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 1].position = this->position + qpl::vector2u((x + 1) * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 2].position = this->position + qpl::vector2u((x + 1) * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 3].position = this->position + qpl::vector2u(x * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);

				chunk[chunk_index * 4 + 0].color = color;
				chunk[chunk_index * 4 + 1].color = color;
				chunk[chunk_index * 4 + 2].color = color;
				chunk[chunk_index * 4 + 3].color = color;

				auto tex_0 = qpl::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_1 = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_2 = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				auto tex_3 = qpl::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);

				if (tile.second == 0b000) {
					chunk[chunk_index * 4 + 0].texCoords = tex_0;
					chunk[chunk_index * 4 + 1].texCoords = tex_1;
					chunk[chunk_index * 4 + 2].texCoords = tex_2;
					chunk[chunk_index * 4 + 3].texCoords = tex_3;
				}
				else if (tile.second == 0b001) {
					chunk[chunk_index * 4 + 0].texCoords = tex_1;
					chunk[chunk_index * 4 + 1].texCoords = tex_0;
					chunk[chunk_index * 4 + 2].texCoords = tex_3;
					chunk[chunk_index * 4 + 3].texCoords = tex_2;
				}
				else if (tile.second == 0b010) {
					chunk[chunk_index * 4 + 0].texCoords = tex_3;
					chunk[chunk_index * 4 + 1].texCoords = tex_2;
					chunk[chunk_index * 4 + 2].texCoords = tex_1;
					chunk[chunk_index * 4 + 3].texCoords = tex_0;
				}
				else if (tile.second == 0b011) {
					chunk[chunk_index * 4 + 0].texCoords = tex_2;
					chunk[chunk_index * 4 + 1].texCoords = tex_3;
					chunk[chunk_index * 4 + 2].texCoords = tex_0;
					chunk[chunk_index * 4 + 3].texCoords = tex_1;
				}
				else if (tile.second == 0b100) {
					chunk[chunk_index * 4 + 0].texCoords = tex_3;
					chunk[chunk_index * 4 + 1].texCoords = tex_0;
					chunk[chunk_index * 4 + 2].texCoords = tex_1;
					chunk[chunk_index * 4 + 3].texCoords = tex_2;
				}
				else if (tile.second == 0b101) {
					chunk[chunk_index * 4 + 0].texCoords = tex_2;
					chunk[chunk_index * 4 + 1].texCoords = tex_1;
					chunk[chunk_index * 4 + 2].texCoords = tex_0;
					chunk[chunk_index * 4 + 3].texCoords = tex_3;
				}
				else if (tile.second == 0b110) {
					chunk[chunk_index * 4 + 0].texCoords = tex_0;
					chunk[chunk_index * 4 + 1].texCoords = tex_3;
					chunk[chunk_index * 4 + 2].texCoords = tex_2;
					chunk[chunk_index * 4 + 3].texCoords = tex_1;
				}
				else if (tile.second == 0b111) {
					chunk[chunk_index * 4 + 0].texCoords = tex_1;
					chunk[chunk_index * 4 + 1].texCoords = tex_2;
					chunk[chunk_index * 4 + 2].texCoords = tex_3;
					chunk[chunk_index * 4 + 3].texCoords = tex_0;
				}
			}
		}

	}
	void qsf::tile_map::create(const std::vector<std::pair<qpl::u32, qpl::u32>>& indices, qpl::size width) {
		this->create(indices, width, qpl::rgb::white);
	}


	void qsf::tile_map::create(const std::vector<std::pair<qpl::u32, qpl::f32>>& indices, qpl::size width, qpl::rgb color) {
		if (!this->texture_ptr_set) {
			qpl::println("tile_map::create: texture_ptr not set");
			return;
		}

		this->chunks.clear();
		this->color = color;
		auto chunk_width = (width - 1) / this->max_chunk_size.x + 1;
		auto chunk_height = (indices.size() / width - 1) / this->max_chunk_size.y + 1;
		auto chunk_dim = chunk_width * chunk_height;

		this->chunk_width_count = chunk_width;
		this->chunks.resize(chunk_dim);
		for (auto& chunk : this->chunks) {
			chunk.setPrimitiveType(sf::Quads);
			chunk.resize(this->max_chunk_size.y * this->max_chunk_size.x * 4);
		}

		auto texture_row_tile_count = texture_ptr->getSize().x / this->texture_tile_dimension.x;

		if (color == qpl::rgb::white) {
			for (qpl::u32 i = 0; i < indices.size(); ++i) {

				auto [y, x] = qpl::div_mod(i, width);
				auto tile = indices[i];
				auto tile_x = (tile.first % texture_row_tile_count);
				auto tile_y = (tile.first / texture_row_tile_count);

				auto chunk_x = (i % width) / this->max_chunk_size.x;
				auto chunk_y = (i / width) / this->max_chunk_size.y;

				auto& chunk = this->chunks[chunk_x + chunk_y * chunk_width];

				auto chunk_index = ((i % width) % this->max_chunk_size.x) + ((i / width) % this->max_chunk_size.y) * this->max_chunk_size.x;

				auto middle = qpl::vector2f(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y) + this->position_tile_dimension / 2;

				auto diagonal = std::sqrt(std::pow(this->position_tile_dimension.x, 2) + std::pow(this->position_tile_dimension.y, 2));
				auto angle = tile.second + (qpl::pi * 1.25);
				auto pos_1 = this->position + middle + qpl::vector2f(std::cos(angle + (qpl::pi * 0.0)), std::sin(angle + (qpl::pi * 0.0))) * diagonal / 2;
				auto pos_2 = this->position + middle + qpl::vector2f(std::cos(angle + (qpl::pi * 0.5)), std::sin(angle + (qpl::pi * 0.5))) * diagonal / 2;
				auto pos_3 = this->position + middle + qpl::vector2f(std::cos(angle + (qpl::pi * 1.0)), std::sin(angle + (qpl::pi * 1.0))) * diagonal / 2;
				auto pos_4 = this->position + middle + qpl::vector2f(std::cos(angle + (qpl::pi * 1.5)), std::sin(angle + (qpl::pi * 1.5))) * diagonal / 2;

				chunk[chunk_index * 4 + 0].position = pos_1;
				chunk[chunk_index * 4 + 1].position = pos_2;
				chunk[chunk_index * 4 + 2].position = pos_3;
				chunk[chunk_index * 4 + 3].position = pos_4;

				auto tex_0 = qpl::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_1 = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_2 = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				auto tex_3 = qpl::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);

				chunk[chunk_index * 4 + 0].texCoords = tex_0;
				chunk[chunk_index * 4 + 1].texCoords = tex_1;
				chunk[chunk_index * 4 + 2].texCoords = tex_2;
				chunk[chunk_index * 4 + 3].texCoords = tex_3;
			}
		}
		else {
			for (qpl::u32 i = 0; i < indices.size(); ++i) {
				auto [y, x] = qpl::div_mod(i, width);
				auto tile = indices[i];
				auto tile_x = (tile.first % texture_row_tile_count);
				auto tile_y = (tile.first / texture_row_tile_count);

				auto chunk_x = (i % width) / this->max_chunk_size.x;
				auto chunk_y = (i / width) / this->max_chunk_size.y;

				auto& chunk = this->chunks[chunk_x + chunk_y * chunk_width];

				auto chunk_index = ((i % width) % this->max_chunk_size.x) + ((i / width) % this->max_chunk_size.y) * this->max_chunk_size.x;

				auto middle = qpl::vector2f(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y) + this->position_tile_dimension / 2;

				auto diagonal = std::sqrt(std::pow(this->position_tile_dimension.x, 2) + std::pow(this->position_tile_dimension.y, 2));
				auto angle = tile.second + (qpl::pi * 1.25);
				auto pos_1 = this->position + middle + qpl::vector2f(std::cos(angle + (qpl::pi * 0.0)), std::sin(angle + (qpl::pi * 0.0))) * diagonal / 2;
				auto pos_2 = this->position + middle + qpl::vector2f(std::cos(angle + (qpl::pi * 0.5)), std::sin(angle + (qpl::pi * 0.5))) * diagonal / 2;
				auto pos_3 = this->position + middle + qpl::vector2f(std::cos(angle + (qpl::pi * 1.0)), std::sin(angle + (qpl::pi * 1.0))) * diagonal / 2;
				auto pos_4 = this->position + middle + qpl::vector2f(std::cos(angle + (qpl::pi * 1.5)), std::sin(angle + (qpl::pi * 1.5))) * diagonal / 2;

				chunk[chunk_index * 4 + 0].position = pos_1;
				chunk[chunk_index * 4 + 1].position = pos_2;
				chunk[chunk_index * 4 + 2].position = pos_3;
				chunk[chunk_index * 4 + 3].position = pos_4;

				chunk[chunk_index * 4 + 0].color = color;
				chunk[chunk_index * 4 + 1].color = color;
				chunk[chunk_index * 4 + 2].color = color;
				chunk[chunk_index * 4 + 3].color = color;

				auto tex_0 = qpl::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_1 = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_2 = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				auto tex_3 = qpl::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);

				if (tile.second == 0b000) {
					chunk[chunk_index * 4 + 0].texCoords = tex_0;
					chunk[chunk_index * 4 + 1].texCoords = tex_1;
					chunk[chunk_index * 4 + 2].texCoords = tex_2;
					chunk[chunk_index * 4 + 3].texCoords = tex_3;
				}
				else if (tile.second == 0b001) {
					chunk[chunk_index * 4 + 0].texCoords = tex_1;
					chunk[chunk_index * 4 + 1].texCoords = tex_0;
					chunk[chunk_index * 4 + 2].texCoords = tex_3;
					chunk[chunk_index * 4 + 3].texCoords = tex_2;
				}
				else if (tile.second == 0b010) {
					chunk[chunk_index * 4 + 0].texCoords = tex_3;
					chunk[chunk_index * 4 + 1].texCoords = tex_2;
					chunk[chunk_index * 4 + 2].texCoords = tex_1;
					chunk[chunk_index * 4 + 3].texCoords = tex_0;
				}
				else if (tile.second == 0b011) {
					chunk[chunk_index * 4 + 0].texCoords = tex_2;
					chunk[chunk_index * 4 + 1].texCoords = tex_3;
					chunk[chunk_index * 4 + 2].texCoords = tex_0;
					chunk[chunk_index * 4 + 3].texCoords = tex_1;
				}
				else if (tile.second == 0b100) {
					chunk[chunk_index * 4 + 0].texCoords = tex_3;
					chunk[chunk_index * 4 + 1].texCoords = tex_0;
					chunk[chunk_index * 4 + 2].texCoords = tex_1;
					chunk[chunk_index * 4 + 3].texCoords = tex_2;
				}
				else if (tile.second == 0b101) {
					chunk[chunk_index * 4 + 0].texCoords = tex_2;
					chunk[chunk_index * 4 + 1].texCoords = tex_1;
					chunk[chunk_index * 4 + 2].texCoords = tex_0;
					chunk[chunk_index * 4 + 3].texCoords = tex_3;
				}
				else if (tile.second == 0b110) {
					chunk[chunk_index * 4 + 0].texCoords = tex_0;
					chunk[chunk_index * 4 + 1].texCoords = tex_3;
					chunk[chunk_index * 4 + 2].texCoords = tex_2;
					chunk[chunk_index * 4 + 3].texCoords = tex_1;
				}
				else if (tile.second == 0b111) {
					chunk[chunk_index * 4 + 0].texCoords = tex_1;
					chunk[chunk_index * 4 + 1].texCoords = tex_2;
					chunk[chunk_index * 4 + 2].texCoords = tex_3;
					chunk[chunk_index * 4 + 3].texCoords = tex_0;
				}
			}
		}

	}
	void qsf::tile_map::create(const std::vector<std::pair<qpl::u32, qpl::f32>>& indices, qpl::size width) {
		this->create(indices, width, qpl::rgb::white);
	}
	void qsf::tile_map::create(const std::vector<qpl::u32>& indices, qpl::size width, qpl::rgb color) {
		if (!this->texture_ptr_set) {
			qpl::println("tile_map::create: texture_ptr not set");
			return;
		}

		this->chunks.clear();
		this->color = color;

		auto chunk_width = (width - 1) / this->max_chunk_size.x + 1;
		auto chunk_height = (indices.size() / width - 1) / this->max_chunk_size.y + 1;
		auto chunk_dim = chunk_width * chunk_height;
		this->chunk_width_count = chunk_width;
		this->chunks.resize(chunk_dim);
		for (auto& chunk : this->chunks) {
			chunk.setPrimitiveType(sf::Quads);
			chunk.resize(this->max_chunk_size.y * this->max_chunk_size.x * 4);
		}

		auto texture_row_tile_count = texture_ptr->getSize().x / this->texture_tile_dimension.x;

		if (color == qpl::rgb::white) {
			for (qpl::u32 i = 0; i < indices.size(); ++i) {
				auto [y, x] = qpl::div_mod(i, width);
				auto index = indices[i];
				auto tile_x = (index % texture_row_tile_count);
				auto tile_y = (index / texture_row_tile_count);


				auto chunk_x = (i % width) / this->max_chunk_size.x;
				auto chunk_y = (i / width) / this->max_chunk_size.y;

				auto& chunk = this->chunks[chunk_x + chunk_y * chunk_width];

				auto chunk_index = ((i % width) % this->max_chunk_size.x) + ((i / width) % this->max_chunk_size.y) * this->max_chunk_size.x;

				chunk[chunk_index * 4 + 0].position = this->position + qpl::vector2u(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 1].position = this->position + qpl::vector2u((x + 1) * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 2].position = this->position + qpl::vector2u((x + 1) * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 3].position = this->position + qpl::vector2u(x * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);

				chunk[chunk_index * 4 + 0].texCoords = qpl::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				chunk[chunk_index * 4 + 1].texCoords = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				chunk[chunk_index * 4 + 2].texCoords = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				chunk[chunk_index * 4 + 3].texCoords = qpl::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
			}
		}
		else {
			for (qpl::u32 i = 0; i < indices.size(); ++i) {
				auto [y, x] = qpl::div_mod(i, width);
				auto index = indices[i];
				auto tile_x = (index % texture_row_tile_count);
				auto tile_y = (index / texture_row_tile_count);

				auto chunk_x = (i % width) / this->max_chunk_size.x;
				auto chunk_y = (i / width) / this->max_chunk_size.y;

				auto& chunk = this->chunks[chunk_x + chunk_y * chunk_width];

				auto chunk_index = ((i % width) % this->max_chunk_size.x) + ((i / width) % this->max_chunk_size.y) * this->max_chunk_size.x;

				chunk[chunk_index * 4 + 0].position = this->position + qpl::vector2u(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 1].position = this->position + qpl::vector2u((x + 1) * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 2].position = this->position + qpl::vector2u((x + 1) * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 3].position = this->position + qpl::vector2u(x * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);

				chunk[chunk_index * 4 + 0].color = color;
				chunk[chunk_index * 4 + 1].color = color;
				chunk[chunk_index * 4 + 2].color = color;
				chunk[chunk_index * 4 + 3].color = color;

				chunk[chunk_index * 4 + 0].texCoords = qpl::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				chunk[chunk_index * 4 + 1].texCoords = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				chunk[chunk_index * 4 + 2].texCoords = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				chunk[chunk_index * 4 + 3].texCoords = qpl::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
			}
		}

	}
	void qsf::tile_map::create(const std::vector<qpl::u32>& indices, qpl::size width) {
		this->create(indices, width, qpl::rgb::white);
	}


	void qsf::tile_map::create_skip_empty(const std::vector<std::pair<qpl::u32, qpl::u32>>& indices, qpl::size width, qpl::rgb color, qpl::u32 skip_index) {
		if (!this->texture_ptr_set) {
			qpl::println("tile_map::create: texture_ptr not set");
			return;
		}

		this->chunks.clear();
		this->color = color;
		auto texture_row_tile_count = texture_ptr->getSize().x / this->texture_tile_dimension.x;

		auto chunk_width = (width - 1) / this->max_chunk_size.x + 1;
		auto chunk_height = (indices.size() / width - 1) / this->max_chunk_size.y + 1;
		auto chunk_dim = chunk_width * chunk_height;

		this->chunk_width_count = chunk_width;
		this->chunks.resize(chunk_dim);
		for (auto& chunk : this->chunks) {
			chunk.setPrimitiveType(sf::Quads);
		}

		if (color == qpl::rgb::white) {
			for (qpl::u32 i = 0; i < indices.size(); ++i) {
				auto tile = indices[i];
				if (tile.first == skip_index) {
					continue;
				}

				auto chunk_x = (i % width) / this->max_chunk_size.x;
				auto chunk_y = (i / width) / this->max_chunk_size.y;
				auto chunk_index = chunk_x + chunk_y * chunk_width;

				auto& chunk = this->chunks[chunk_index];

				auto ctr = chunk.getVertexCount();
				chunk.resize(ctr + 4);

				auto [y, x] = qpl::div_mod(i, width);
				auto tile_x = (tile.first % texture_row_tile_count);
				auto tile_y = (tile.first / texture_row_tile_count);


				auto pos_0 = this->position + qpl::vector2u(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				auto pos_1 = this->position + qpl::vector2u((x + 1) * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				auto pos_2 = this->position + qpl::vector2u((x + 1) * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);
				auto pos_3 = this->position + qpl::vector2u(x * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);

				chunk[ctr + 0].position = pos_0;
				chunk[ctr + 1].position = pos_1;
				chunk[ctr + 2].position = pos_2;
				chunk[ctr + 3].position = pos_3;

				auto tex_0 = qpl::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_1 = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_2 = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				auto tex_3 = qpl::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);

				if (tile.second == 0b000) {
					chunk[ctr + 0].texCoords = tex_0;
					chunk[ctr + 1].texCoords = tex_1;
					chunk[ctr + 2].texCoords = tex_2;
					chunk[ctr + 3].texCoords = tex_3;
				}
				else if (tile.second == 0b001) {
					chunk[ctr + 0].texCoords = tex_1;
					chunk[ctr + 1].texCoords = tex_0;
					chunk[ctr + 2].texCoords = tex_3;
					chunk[ctr + 3].texCoords = tex_2;
				}
				else if (tile.second == 0b010) {
					chunk[ctr + 0].texCoords = tex_3;
					chunk[ctr + 1].texCoords = tex_2;
					chunk[ctr + 2].texCoords = tex_1;
					chunk[ctr + 3].texCoords = tex_0;
				}
				else if (tile.second == 0b011) {
					chunk[ctr + 0].texCoords = tex_2;
					chunk[ctr + 1].texCoords = tex_3;
					chunk[ctr + 2].texCoords = tex_0;
					chunk[ctr + 3].texCoords = tex_1;
				}
				else if (tile.second == 0b100) {
					chunk[ctr + 0].texCoords = tex_3;
					chunk[ctr + 1].texCoords = tex_0;
					chunk[ctr + 2].texCoords = tex_1;
					chunk[ctr + 3].texCoords = tex_2;
				}
				else if (tile.second == 0b101) {
					chunk[ctr + 0].texCoords = tex_2;
					chunk[ctr + 1].texCoords = tex_1;
					chunk[ctr + 2].texCoords = tex_0;
					chunk[ctr + 3].texCoords = tex_3;
				}
				else if (tile.second == 0b110) {
					chunk[ctr + 0].texCoords = tex_0;
					chunk[ctr + 1].texCoords = tex_3;
					chunk[ctr + 2].texCoords = tex_2;
					chunk[ctr + 3].texCoords = tex_1;
				}
				else if (tile.second == 0b111) {
					chunk[ctr + 0].texCoords = tex_1;
					chunk[ctr + 1].texCoords = tex_2;
					chunk[ctr + 2].texCoords = tex_3;
					chunk[ctr + 3].texCoords = tex_0;
				}

			}
		}
		else {
			for (qpl::u32 i = 0; i < indices.size(); ++i) {
				auto tile = indices[i];
				if (tile.first == skip_index) {
					continue;
				}
				auto chunk_x = (i % width) / this->max_chunk_size.x;
				auto chunk_y = (i / width) / this->max_chunk_size.y;
				auto chunk_index = chunk_x + chunk_y * chunk_width;

				auto& chunk = this->chunks[chunk_index];
				auto ctr = chunk.getVertexCount();
				chunk.resize(ctr + 4);

				auto [y, x] = qpl::div_mod(i, width);
				auto tile_x = (tile.first % texture_row_tile_count);
				auto tile_y = (tile.first / texture_row_tile_count);

				auto pos_0 = this->position + qpl::vector2u(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				auto pos_1 = this->position + qpl::vector2u((x + 1) * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				auto pos_2 = this->position + qpl::vector2u((x + 1) * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);
				auto pos_3 = this->position + qpl::vector2u(x * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);

				chunk[ctr + 0].position = pos_0;
				chunk[ctr + 1].position = pos_1;
				chunk[ctr + 2].position = pos_2;
				chunk[ctr + 3].position = pos_3;

				chunk[ctr + 0].color = color;
				chunk[ctr + 1].color = color;
				chunk[ctr + 2].color = color;
				chunk[ctr + 3].color = color;

				auto tex_0 = qpl::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_1 = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_2 = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				auto tex_3 = qpl::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);

				if (tile.second == 0b000) {
					chunk[ctr + 0].texCoords = tex_0;
					chunk[ctr + 1].texCoords = tex_1;
					chunk[ctr + 2].texCoords = tex_2;
					chunk[ctr + 3].texCoords = tex_3;
				}
				else if (tile.second == 0b001) {
					chunk[ctr + 0].texCoords = tex_1;
					chunk[ctr + 1].texCoords = tex_0;
					chunk[ctr + 2].texCoords = tex_3;
					chunk[ctr + 3].texCoords = tex_2;
				}
				else if (tile.second == 0b010) {
					chunk[ctr + 0].texCoords = tex_3;
					chunk[ctr + 1].texCoords = tex_2;
					chunk[ctr + 2].texCoords = tex_1;
					chunk[ctr + 3].texCoords = tex_0;
				}
				else if (tile.second == 0b011) {
					chunk[ctr + 0].texCoords = tex_2;
					chunk[ctr + 1].texCoords = tex_3;
					chunk[ctr + 2].texCoords = tex_0;
					chunk[ctr + 3].texCoords = tex_1;
				}
				else if (tile.second == 0b100) {
					chunk[ctr + 0].texCoords = tex_3;
					chunk[ctr + 1].texCoords = tex_0;
					chunk[ctr + 2].texCoords = tex_1;
					chunk[ctr + 3].texCoords = tex_2;
				}
				else if (tile.second == 0b101) {
					chunk[ctr + 0].texCoords = tex_2;
					chunk[ctr + 1].texCoords = tex_1;
					chunk[ctr + 2].texCoords = tex_0;
					chunk[ctr + 3].texCoords = tex_3;
				}
				else if (tile.second == 0b110) {
					chunk[ctr + 0].texCoords = tex_0;
					chunk[ctr + 1].texCoords = tex_3;
					chunk[ctr + 2].texCoords = tex_2;
					chunk[ctr + 3].texCoords = tex_1;
				}
				else if (tile.second == 0b111) {
					chunk[ctr + 0].texCoords = tex_1;
					chunk[ctr + 1].texCoords = tex_2;
					chunk[ctr + 2].texCoords = tex_3;
					chunk[ctr + 3].texCoords = tex_0;
				}
			}
		}

	}
	void qsf::tile_map::create_skip_empty(const std::vector<std::pair<qpl::u32, qpl::u32>>& indices, qpl::size width, qpl::u32 skip_index) {
		this->create_skip_empty(indices, width, qpl::rgb::white, skip_index);
	}

	void qsf::tile_map::create_skip_empty(const std::vector<std::pair<qpl::u32, qpl::f32>>& indices, qpl::size width, qpl::rgb color, qpl::u32 skip_index) {
		if (!this->texture_ptr_set) {
			qpl::println("tile_map::create: texture_ptr not set");
			return;
		}

		this->chunks.clear();
		this->color = color;
		auto texture_row_tile_count = texture_ptr->getSize().x / this->texture_tile_dimension.x;

		auto chunk_width = (width - 1) / this->max_chunk_size.x + 1;
		auto chunk_height = (indices.size() / width - 1) / this->max_chunk_size.y + 1;
		auto chunk_dim = chunk_width * chunk_height;

		this->chunk_width_count = chunk_width;
		this->chunks.resize(chunk_dim);
		for (auto& chunk : this->chunks) {
			chunk.setPrimitiveType(sf::Quads);
		}

		auto diagonal = std::sqrt(std::pow(this->texture_tile_dimension.x, 2) + std::pow(this->texture_tile_dimension.y, 2));

		if (color == qpl::rgb::white) {
			for (qpl::u32 i = 0; i < indices.size(); ++i) {
				auto tile = indices[i];
				if (tile.first == skip_index) {
					continue;
				}

				auto chunk_x = (i % width) / this->max_chunk_size.x;
				auto chunk_y = (i / width) / this->max_chunk_size.y;
				auto chunk_index = chunk_x + chunk_y * chunk_width;

				auto& chunk = this->chunks[chunk_index];

				auto ctr = chunk.getVertexCount();
				chunk.resize(ctr + 4);

				auto [y, x] = qpl::div_mod(i, width);
				auto tile_x = (tile.first % texture_row_tile_count);
				auto tile_y = (tile.first / texture_row_tile_count);

				auto middle = qpl::vector2f(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y) + this->position_tile_dimension / 2;

				auto angle = ((tile.second / 180) * qpl::pi) + (qpl::pi * 1.25);
				auto pos_1 = this->position + middle + qpl::vector2f(std::cos(angle + (qpl::pi * 0.0)), std::sin(angle + (qpl::pi * 0.0))) * diagonal / 2;
				auto pos_2 = this->position + middle + qpl::vector2f(std::cos(angle + (qpl::pi * 0.5)), std::sin(angle + (qpl::pi * 0.5))) * diagonal / 2;
				auto pos_3 = this->position + middle + qpl::vector2f(std::cos(angle + (qpl::pi * 1.0)), std::sin(angle + (qpl::pi * 1.0))) * diagonal / 2;
				auto pos_4 = this->position + middle + qpl::vector2f(std::cos(angle + (qpl::pi * 1.5)), std::sin(angle + (qpl::pi * 1.5))) * diagonal / 2;

				chunk[ctr + 0].position = pos_1;
				chunk[ctr + 1].position = pos_2;
				chunk[ctr + 2].position = pos_3;
				chunk[ctr + 3].position = pos_4;

				auto tex_0 = qpl::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_1 = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_2 = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				auto tex_3 = qpl::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);

				chunk[ctr + 0].texCoords = tex_0;
				chunk[ctr + 1].texCoords = tex_1;
				chunk[ctr + 2].texCoords = tex_2;
				chunk[ctr + 3].texCoords = tex_3;

			}
		}
		else {
			for (qpl::u32 i = 0; i < indices.size(); ++i) {
				auto tile = indices[i];
				if (tile.first == skip_index) {
					continue;
				}
				auto chunk_x = (i % width) / this->max_chunk_size.x;
				auto chunk_y = (i / width) / this->max_chunk_size.y;
				auto chunk_index = chunk_x + chunk_y * chunk_width;

				auto& chunk = this->chunks[chunk_index];
				auto ctr = chunk.getVertexCount();
				chunk.resize(ctr + 4);

				auto [y, x] = qpl::div_mod(i, width);
				auto tile_x = (tile.first % texture_row_tile_count);
				auto tile_y = (tile.first / texture_row_tile_count);

				auto middle = qpl::vector2f(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y) + this->position_tile_dimension / 2;

				auto angle = tile.second + (qpl::pi * 1.25);
				auto pos_1 = this->position + middle + qpl::vector2f(std::cos(angle + (qpl::pi * 0.0)), std::sin(angle + (qpl::pi * 0.0))) * diagonal / 2;
				auto pos_2 = this->position + middle + qpl::vector2f(std::cos(angle + (qpl::pi * 0.5)), std::sin(angle + (qpl::pi * 0.5))) * diagonal / 2;
				auto pos_3 = this->position + middle + qpl::vector2f(std::cos(angle + (qpl::pi * 1.0)), std::sin(angle + (qpl::pi * 1.0))) * diagonal / 2;
				auto pos_4 = this->position + middle + qpl::vector2f(std::cos(angle + (qpl::pi * 1.5)), std::sin(angle + (qpl::pi * 1.5))) * diagonal / 2;

				chunk[ctr + 0].position = pos_1;
				chunk[ctr + 1].position = pos_2;
				chunk[ctr + 2].position = pos_3;
				chunk[ctr + 3].position = pos_4;

				chunk[ctr + 0].color = color;
				chunk[ctr + 1].color = color;
				chunk[ctr + 2].color = color;
				chunk[ctr + 3].color = color;

				auto tex_0 = qpl::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_1 = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_2 = qpl::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				auto tex_3 = qpl::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);

				chunk[ctr + 0].texCoords = tex_0;
				chunk[ctr + 1].texCoords = tex_1;
				chunk[ctr + 2].texCoords = tex_2;
				chunk[ctr + 3].texCoords = tex_3;

			}
		}

	}
	void qsf::tile_map::create_skip_empty(const std::vector<std::pair<qpl::u32, qpl::f32>>& indices, qpl::size width, qpl::u32 skip_index) {
		this->create_skip_empty(indices, width, qpl::rgb::white, skip_index);
	}
	void qsf::tile_map::create_skip_empty(const std::vector<qpl::u32>& indices, qpl::size width, qpl::rgb color, qpl::u32 skip_index) {
		if (!this->texture_ptr_set) {
			qpl::println("tile_map::create: texture_ptr not set");
			return;
		}

		this->chunks.clear();
		this->color = color;

		auto chunk_width = (width - 1) / this->max_chunk_size.x + 1;
		auto chunk_height = (indices.size() / width - 1) / this->max_chunk_size.y + 1;
		auto chunk_dim = chunk_width * chunk_height;

		this->chunk_width_count = chunk_width;
		this->chunks.resize(chunk_dim);
		for (auto& chunk : this->chunks) {
			chunk.setPrimitiveType(sf::Quads);
		}

		auto texture_row_tile_count = texture_ptr->getSize().x / this->texture_tile_dimension.x;

		qpl::u32 ctr = 0;
		if (color == qpl::rgb::white) {
			for (qpl::u32 i = 0; i < indices.size(); ++i) {
				auto index = indices[i];
				if (index == skip_index) {
					continue;
				}
				auto chunk_x = (i % width) / this->max_chunk_size.x;
				auto chunk_y = (i / width) / this->max_chunk_size.y;
				auto chunk_index = chunk_x + chunk_y * chunk_width;

				auto& chunk = this->chunks[chunk_index];
				auto ctr = chunk.getVertexCount();
				chunk.resize(ctr + 4);

				auto [y, x] = qpl::div_mod(i, width);

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

				auto chunk_x = (i % width) / this->max_chunk_size.x;
				auto chunk_y = (i / width) / this->max_chunk_size.y;
				auto chunk_index = chunk_x + chunk_y * chunk_width;

				auto& chunk = this->chunks[chunk_index];
				auto ctr = chunk.getVertexCount();
				chunk.resize(ctr + 4);

				auto [y, x] = qpl::div_mod(i, width);
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
	void qsf::tile_map::create_skip_empty(const std::vector<qpl::u32>& indices, qpl::size width, qpl::u32 skip_index) {
		this->create_skip_empty(indices, width, qpl::rgb::white, skip_index);
	}

	void qsf::tile_map::set_color(qpl::rgb color) {
		this->color = color;
		for (auto& v : this->chunks) {
			for (qpl::u32 i = 0u; i < v.getVertexCount(); ++i) {
				v[i].color = color;
			}
		}
	}

	void qsf::tile_map::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		states.texture = this->texture_ptr;

		for (qpl::u32 i = 0u; i < this->chunks.size(); ++i) {
			auto x = i % this->chunk_width_count;
			auto y = i / this->chunk_width_count;

			auto rx = qpl::f32_cast(x * this->max_chunk_size.x * this->texture_tile_dimension.x);
			auto ry = qpl::f32_cast(y * this->max_chunk_size.y * this->texture_tile_dimension.y);
			auto rw = qpl::f32_cast(this->max_chunk_size.x * this->texture_tile_dimension.x);
			auto rh = qpl::f32_cast(this->max_chunk_size.y * this->texture_tile_dimension.y);

			auto rect = states.transform.transformRect(sf::FloatRect(rx, ry, rw, rh));
			auto fov = sf::FloatRect(0.f, 0.f, qpl::f32_cast(window.getSize().x), qpl::f32_cast(window.getSize().y));

			if (rect.intersects(fov)) {
				window.draw(this->chunks[i], states);
			}
		}
	}

	qpl::size qsf::tile_map::size() const {
		qpl::size count = 0u;
		for (auto& i : this->chunks) {
			count += i.getVertexCount() / 4;
		}
		return count;
	}
	qpl::size qsf::tile_map::chunk_count() const {
		return this->chunks.size();
	}
	void qsf::tile_map::set_chunk_dimension(qpl::u32 x, qpl::u32 y) {
		this->max_chunk_size = qpl::vector2u(x, y);
	}
	void qsf::tile_map::clear() {
		this->chunks.clear();
	}

	void qsf::small_tile_map::set_texture(const sf::Texture& texture, qpl::u32 texture_tile_width) {
		this->texture_ptr = &texture;
		this->texture_tile_dimension = qpl::vec(texture_tile_width, texture_tile_width);
	}
	void qsf::small_tile_map::set_position(qpl::vector2f position) {
		this->position = position;
	}
	void qsf::small_tile_map::set_scale(qpl::vector2f scale) {
		this->scale = scale;
	}
	void qsf::small_tile_map::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		if (this->vertices.getVertexCount()) {
			states.transform.translate(this->position);
			states.texture = this->texture_ptr;
			window.draw(this->vertices, states);
		}
	}

	void qsf::vgraph::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::graph = *this;
		qsf::detail::graph.draw(window, states);
	}


	qsf::vgraph::data_point_info& qsf::vgraph::data_point_info::operator=(const qsf::vgraph::data_point_info& other) {
		this->data = other.data;
		this->color = other.color;
		this->thickness = other.thickness;
		this->circle = other.circle;
		this->text = other.text;
		return *this;
	}
	qsf::vgraph::data_point_info qsf::vgraph::data_point_info::operator*(qpl::f64 n) const {
		auto copy = *this;
		copy.color *= n;
		copy.thickness *= n;
		copy.data *= n;
		return copy;
	}
	qsf::vgraph::data_point_info qsf::vgraph::data_point_info::operator*(qsf::vgraph::data_point_info other) const {
		auto copy = *this;
		copy.color *= other.color;
		copy.thickness *= other.thickness;
		copy.data *= other.data;

		return copy;
	}
	qsf::vgraph::data_point_info qsf::vgraph::data_point_info::operator-(qsf::vgraph::data_point_info other) const {
		auto copy = *this;
		copy.color -= other.color;
		copy.thickness -= other.thickness;
		copy.data -= other.data;
		return copy;
	}
	qsf::vgraph::data_point_info qsf::vgraph::data_point_info::operator+(qsf::vgraph::data_point_info other) const {
		auto copy = *this;
		copy.color += other.color;
		copy.thickness += other.thickness;
		copy.data += other.data;
		return copy;
	}
	bool qsf::vgraph::data_point_info::operator<(qsf::vgraph::data_point_info other) const {
		return this->data < other.data;
	}
	bool qsf::vgraph::data_point_info::operator>(qsf::vgraph::data_point_info other) const {
		return this->data > other.data;
	}



	qsf::vgraph::data_point& qsf::vgraph::data_point::operator=(const qsf::vgraph::data_point& other) {
		this->data = other.data;
		this->color = other.color;
		this->thickness = other.thickness;
		return *this;
	}
	qsf::vgraph::data_point qsf::vgraph::data_point::operator*(qpl::f64 n) const {
		auto copy = *this;
		copy.color *= n;
		copy.thickness *= n;
		copy.data *= n;
		return copy;
	}
	qsf::vgraph::data_point qsf::vgraph::data_point::operator*(qsf::vgraph::data_point other) const {
		auto copy = *this;
		copy.color *= other.color;
		copy.thickness *= other.thickness;
		copy.data *= other.data;
		return copy;
	}
	qsf::vgraph::data_point qsf::vgraph::data_point::operator-(qsf::vgraph::data_point other) const {
		auto copy = *this;
		copy.color -= other.color;
		copy.thickness -= other.thickness;
		copy.data -= other.data;
		return copy;
	}
	qsf::vgraph::data_point qsf::vgraph::data_point::operator+(qsf::vgraph::data_point other) const {
		auto copy = *this;
		copy.color += other.color;
		copy.thickness += other.thickness;
		copy.data += other.data;
		return copy;
	}
	bool qsf::vgraph::data_point::operator<(qsf::vgraph::data_point other) const {
		return this->data < other.data;
	}
	bool qsf::vgraph::data_point::operator>(qsf::vgraph::data_point other) const {
		return this->data > other.data;
	}


	qsf::vgraph::data_point_simple& qsf::vgraph::data_point_simple::operator=(const qsf::vgraph::data_point_simple& other) {
		this->data = other.data;
		return *this;
	}
	qsf::vgraph::data_point_simple qsf::vgraph::data_point_simple::operator*(qpl::f64 n) const {
		auto copy = *this;
		copy.data *= n;
		return copy;
	}
	qsf::vgraph::data_point_simple qsf::vgraph::data_point_simple::operator*(qsf::vgraph::data_point_simple other) const {
		auto copy = *this;
		copy.data *= other.data;
		return copy;
	}
	qsf::vgraph::data_point_simple qsf::vgraph::data_point_simple::operator-(qsf::vgraph::data_point_simple other) const {
		auto copy = *this;
		copy.data -= other.data;
		return copy;
	}
	qsf::vgraph::data_point_simple qsf::vgraph::data_point_simple::operator+(qsf::vgraph::data_point_simple other) const {
		auto copy = *this;
		copy.data += other.data;
		return copy;
	}
	bool qsf::vgraph::data_point_simple::operator<(qsf::vgraph::data_point_simple other) const {
		return this->data < other.data;
	}
	bool qsf::vgraph::data_point_simple::operator>(qsf::vgraph::data_point_simple other) const {
		return this->data > other.data;
	}

	void qsf::vgraph::info_graph::set_font(const std::string& font) {
		this->text.font_name = font;
	}
	void qsf::vgraph::info_graph::set_text_color(qpl::rgb text_color) {
		this->text.set_color(text_color);
	}
	void qsf::vgraph::info_graph::set_text_character_size(qpl::u32 character_size) {
		this->text.character_size = character_size;
	}
	void qsf::vgraph::info_graph::set_text_style(qpl::u32 text_style) {
		this->text.style = text_style;
	}
	void qsf::vgraph::info_graph::set_text_outline_thickness(qpl::f32 thickness) {
		this->text.outline_thickness = thickness;
	}
	void qsf::vgraph::info_graph::set_text_outline_color(qpl::rgb color) {
		this->text.outline_color = color;
	}

	qpl::f64 qsf::vgraph::info_graph::get_percentage_height_at(qpl::size index) const {
		auto [low, high] = this->get_low_high();

		return (this->data[index].data - low.data) / (high.data - low.data);
	}
	void qsf::vgraph::info_graph::add_data(qpl::f64 data) {
		this->add_data(data, this->color, this->thickness);
	}
	void qsf::vgraph::info_graph::add_data(qpl::f64 data, qpl::rgb color) {
		this->add_data(data, color, this->thickness);
	}
	void qsf::vgraph::info_graph::add_data(qpl::f64 data, qpl::rgb color, qpl::f64 thickness) {
		this->data.push_back({});
		this->data.back().data = data;
		this->data.back().color = color;
		this->data.back().thickness = thickness;
	}
	void qsf::vgraph::info_graph::add_data(qpl::f64 data, std::string string) {
		this->add_data(data, this->color, this->thickness);
		this->data.back().text = this->text;
		this->data.back().text.set_string(string);
	}
	void qsf::vgraph::info_graph::add_data(qpl::f64 data, std::string string, qpl::rgb color) {
		this->add_data(data, this->color, this->thickness);
		this->data.back().text = this->text;
		this->data.back().text.set_string(string);
		this->data.back().text.set_color(color);
	}
	std::pair<qsf::vgraph::data_point_info, qsf::vgraph::data_point_info> qsf::vgraph::info_graph::get_low_high() const {
		return qpl::min_max_vector(this->data);
	}
	qpl::size qsf::vgraph::info_graph::size() const {
		return this->data.size();
	}
	void qsf::vgraph::info_graph::clear() {
		return this->data.clear();
	}
	qsf::vgraph::data_point_info& qsf::vgraph::info_graph::operator[](qpl::size index) {
		return this->data[index];
	}
	const qsf::vgraph::data_point_info& qsf::vgraph::info_graph::operator[](qpl::size index) const {
		return this->data[index];
	}
	qsf::vgraph::data_point_info& qsf::vgraph::info_graph::back() {
		return this->data.back();
	}
	const qsf::vgraph::data_point_info& qsf::vgraph::info_graph::back() const {
		return this->data.back();
	}
	qsf::vgraph::data_point_info& qsf::vgraph::info_graph::front() {
		return this->data.front();
	}
	const qsf::vgraph::data_point_info& qsf::vgraph::info_graph::front() const {
		return this->data.front();
	}
	std::vector<qsf::vgraph::data_point_info>::iterator qsf::vgraph::info_graph::begin() {
		return this->data.begin();
	}
	std::vector<qsf::vgraph::data_point_info>::const_iterator qsf::vgraph::info_graph::begin() const {
		return this->data.begin();
	}
	std::vector<qsf::vgraph::data_point_info>::const_iterator qsf::vgraph::info_graph::cbegin() {
		return this->data.cbegin();
	}
	std::vector<qsf::vgraph::data_point_info>::iterator qsf::vgraph::info_graph::end() {
		return this->data.end();
	}
	std::vector<qsf::vgraph::data_point_info>::const_iterator qsf::vgraph::info_graph::end() const {
		return this->data.end();
	}
	std::vector<qsf::vgraph::data_point_info>::const_iterator qsf::vgraph::info_graph::cend() {
		return this->data.cend();
	}


	qpl::f64 qsf::vgraph::standard_graph::get_percentage_height_at(qpl::size index) const {
		auto [low, high] = this->get_low_high();

		return (this->data[index].data - low.data) / (high.data - low.data);
	}
	void qsf::vgraph::standard_graph::add_data(qpl::f64 data) {
		this->add_data(data, this->color, this->thickness);
	}
	void qsf::vgraph::standard_graph::add_data(qpl::f64 data, qpl::rgb color) {
		this->add_data(data, color, this->thickness);
	}
	void qsf::vgraph::standard_graph::add_data(qpl::f64 data, qpl::rgb color, qpl::f64 thickness) {
		this->data.push_back({});
		this->data.back().data = data;
		this->data.back().color = color;
		this->data.back().thickness = thickness;
	}
	std::pair<qsf::vgraph::data_point, qsf::vgraph::data_point> qsf::vgraph::standard_graph::get_low_high() const {
		return qpl::min_max_vector(this->data);
	}
	qpl::size qsf::vgraph::standard_graph::size() const {
		return this->data.size();
	}
	void qsf::vgraph::standard_graph::clear() {
		return this->data.clear();
	}
	qsf::vgraph::data_point& qsf::vgraph::standard_graph::operator[](qpl::size index) {
		return this->data[index];
	}
	const qsf::vgraph::data_point& qsf::vgraph::standard_graph::operator[](qpl::size index) const {
		return this->data[index];
	}
	qsf::vgraph::data_point& qsf::vgraph::standard_graph::back() {
		return this->data.back();
	}
	const qsf::vgraph::data_point& qsf::vgraph::standard_graph::back() const {
		return this->data.back();
	}
	qsf::vgraph::data_point& qsf::vgraph::standard_graph::front() {
		return this->data.front();
	}
	const qsf::vgraph::data_point& qsf::vgraph::standard_graph::front() const {
		return this->data.front();
	}
	std::vector<qsf::vgraph::data_point>::iterator qsf::vgraph::standard_graph::begin() {
		return this->data.begin();
	}
	std::vector<qsf::vgraph::data_point>::const_iterator qsf::vgraph::standard_graph::begin() const {
		return this->data.begin();
	}
	std::vector<qsf::vgraph::data_point>::const_iterator qsf::vgraph::standard_graph::cbegin() {
		return this->data.cbegin();
	}
	std::vector<qsf::vgraph::data_point>::iterator qsf::vgraph::standard_graph::end() {
		return this->data.end();
	}
	std::vector<qsf::vgraph::data_point>::const_iterator qsf::vgraph::standard_graph::end() const {
		return this->data.end();
	}
	std::vector<qsf::vgraph::data_point>::const_iterator qsf::vgraph::standard_graph::cend() {
		return this->data.cend();
	}


	qpl::f64 qsf::vgraph::simple_graph::get_percentage_height_at(qpl::size index) const {
		auto [low, high] = this->get_low_high();

		return (this->data[index].data - low.data) / (high.data - low.data);
	}
	void qsf::vgraph::simple_graph::add_data(qpl::f64 data) {
		this->data.push_back(data);
	}
	std::pair<qsf::vgraph::data_point_simple, qsf::vgraph::data_point_simple> qsf::vgraph::simple_graph::get_low_high() const {
		return qpl::min_max_vector(this->data);
	}
	qpl::size qsf::vgraph::simple_graph::size() const {
		return this->data.size();
	}
	void qsf::vgraph::simple_graph::clear() {
		return this->data.clear();
	}
	qsf::vgraph::data_point_simple& qsf::vgraph::simple_graph::operator[](qpl::size index) {
		return this->data[index];
	}
	const qsf::vgraph::data_point_simple& qsf::vgraph::simple_graph::operator[](qpl::size index) const {
		return this->data[index];
	}
	qsf::vgraph::data_point_simple& qsf::vgraph::simple_graph::back() {
		return this->data.back();
	}
	const qsf::vgraph::data_point_simple& qsf::vgraph::simple_graph::back() const {
		return this->data.back();
	}
	qsf::vgraph::data_point_simple& qsf::vgraph::simple_graph::front() {
		return this->data.front();
	}
	const qsf::vgraph::data_point_simple& qsf::vgraph::simple_graph::front() const {
		return this->data.front();
	}
	std::vector<qsf::vgraph::data_point_simple>::iterator qsf::vgraph::simple_graph::begin() {
		return this->data.begin();
	}
	std::vector<qsf::vgraph::data_point_simple>::const_iterator qsf::vgraph::simple_graph::begin() const {
		return this->data.begin();
	}
	std::vector<qsf::vgraph::data_point_simple>::const_iterator qsf::vgraph::simple_graph::cbegin() {
		return this->data.cbegin();
	}
	std::vector<qsf::vgraph::data_point_simple>::iterator qsf::vgraph::simple_graph::end() {
		return this->data.end();
	}
	std::vector<qsf::vgraph::data_point_simple>::const_iterator qsf::vgraph::simple_graph::end() const {
		return this->data.end();
	}
	std::vector<qsf::vgraph::data_point_simple>::const_iterator qsf::vgraph::simple_graph::cend() {
		return this->data.cend();
	}


	void qsf::vgraph::clear_data() {
		this->simple_graphs.clear();
		this->standard_graphs.clear();
		this->info_graphs.clear();
		this->index_start = 0u;
		this->index_end = qpl::size_max;
	}
	bool qsf::vgraph::empty() const {
		return this->simple_graphs.empty() && this->standard_graphs.empty() && this->info_graphs.empty();
	}
	bool qsf::vgraph::changed() const {
		return (this->visible_index_range_before != this->visible_index_range());
	}
	void qsf::vgraph::update_change() const {
		if (!this->changed()) {
			return;
		}
		this->check_x_axis();
		auto x = this->get_low_high();
		this->low_high_graph = { x.first, x.second };

		this->visible_index_range_before = this->visible_index_range();
	}
	void qsf::vgraph::check_x_axis() const {
		while (this->visible_element_size() / this->x_axis_line_frequency < this->desired_x_axis_lines / 2) {
			this->x_axis_line_frequency /= 2;
			if (!this->x_axis_line_frequency) {
				this->x_axis_line_frequency = 1u;
				break;
			}
		}
		while (this->visible_element_size() / this->x_axis_line_frequency > this->desired_x_axis_lines * 2) {
			this->x_axis_line_frequency *= 2;
		}

		this->index_skip_size = this->x_axis_line_frequency / this->desired_visible_size;
		if (!this->index_skip_size) {
			this->index_skip_size = 1u;
		}
	}
	void qsf::vgraph::enable_track_new_entries() {
		this->display_last_n_entries = this->visible_element_size();
		this->index_start = 0u;
		this->index_end = qpl::size_max;
		this->enable_last_n_when_dragging_right_lock = false;
	}
	void qsf::vgraph::disable_track_new_entries() {
		auto size = this->visible_element_size();
		this->index_end = this->graph_element_size();
		this->index_start = qpl::u32_cast(qpl::max(qpl::i32_cast(0), qpl::i32_cast(this->index_end) - qpl::i32_cast(size)));
	}
	void qsf::vgraph::enable_axis_info() {
		this->use_x_axis = true;
		this->use_y_axis = true;
	}

	void qsf::vgraph::update(const event_info& event_info) {
		if (this->empty()) {
			return;
		}
		if (event_info.left_mouse_clicked()) {
			qsf::vrectangle rect(this->position, this->true_graph_dimension());
			rect.position.x += qpl::f32_cast(this->y_axis_text_space);

			if (rect.contains(event_info.mouse_position())) {
				this->drag = true;
				this->click_position = event_info.mouse_position();
			}
		}

		this->mouse_position = event_info.mouse_position();
		if (event_info.mouse_moved()) {

			if (this->drag && this->allow_dragging) {
				auto diff = this->click_position - event_info.mouse_position();

				auto size = this->visible_element_size();
				auto index_f = (diff.x / this->true_graph_width()) * size;
				auto index_delta = qpl::i64_cast(index_f);


				if (index_delta) {
					this->click_position = event_info.mouse_position();
					this->click_position.x += qpl::f32_cast(((index_f - index_delta) / size) * this->true_graph_width());

					this->index_start = qpl::size_cast(qpl::max(qpl::i64_cast(0), qpl::i64_cast(this->visible_index_start()) + index_delta));
					this->index_end = this->index_start + size;
					auto over = qpl::i64_cast(this->index_end) - qpl::i64_cast(this->graph_element_size());

					if (this->index_end >= this->graph_element_size()) {
						this->index_end = this->graph_element_size();
						this->index_start = qpl::size_cast(this->index_start - over);

						this->enable_last_n_when_dragging_right_lock = true;
					}
				}
				if (this->enable_last_n_when_dragging_right_lock) {
					this->enable_track_new_entries();
				}
			}
		}

		if (this->display_closest_graph_at_cursor && this->visible_element_size()) {

			qsf::vgraph::interpolation_type interpolation = qsf::vgraph::interpolation_type::unset;

			auto visible_size = this->visible_element_size() - 1;
			auto offset = this->position.x + this->y_axis_text_space;
			auto progress = (event_info.mouse_position().x - offset) / this->true_graph_width();
			auto index = visible_size * progress;
			auto int_index = qpl::size_cast(index);
			auto left_over = index - int_index;
			auto [low, high] = this->get_low_high();

			auto y_range = (high - low);
			if (high == qpl::f64_min && low == qpl::f64_max) {
				y_range = 0.0;
			}
			auto low_padded = low - y_range * this->y_axis_padding;
			auto high_padded = high + y_range * this->y_axis_padding;

			this->closest_graph_at_cursor = "";
			if (index >= 0 && index <= visible_size) {
				qpl::f64 min_distance = qpl::f64_max;

				for (auto& g : this->info_graphs) {
					interpolation = interpolation;
					if (interpolation == qsf::vgraph::interpolation_type::unset) {
						interpolation = this->interpolation;
					}

					qpl::f64 value = 0.0;
					if (left_over == 0.0) {
						value = g.second.data[this->visible_index_start() + int_index].data;
					}
					else if (interpolation == qsf::vgraph::interpolation_type::cubic) {
						value = qpl::cubic_interpolation(g.second.data[this->visible_index_start() + int_index].data, g.second.data[this->visible_index_start() + int_index + this->index_skip_size].data, left_over);
					}
					else if (interpolation == qsf::vgraph::interpolation_type::linear) {
						value = qpl::linear_interpolation(g.second.data[this->visible_index_start() + int_index].data, g.second.data[this->visible_index_start() + int_index + this->index_skip_size].data, left_over);
					}

					auto y_progress = (value - low_padded) / (high_padded - low_padded);
					y_progress = qpl::clamp_0_1((1.0 - y_progress) * (1.0 - this->height_delta) + (this->height_delta) / 2);
					auto pos_y = this->position.y + this->dimension.y * y_progress;

					auto distance = qpl::abs(pos_y - event_info.mouse_position().y);

					if (min_distance > distance && distance < this->closest_graph_at_cursor_distance) {
						min_distance = distance;
						this->closest_graph_at_cursor = g.first;
						this->closest_graph_at_cursor_value = value;
						this->closest_graph_at_cursor_color = g.second.color;
						this->closest_graph_at_cursor_index = qpl::u32_cast(this->visible_index_start() + index + 0.5);
						if (this->closest_graph_at_cursor_color.is_unset()) {
							this->closest_graph_at_cursor_color = this->color;
						}
					}
				}
				for (auto& g : this->standard_graphs) {
					interpolation = interpolation;
					if (interpolation == qsf::vgraph::interpolation_type::unset) {
						interpolation = this->interpolation;
					}

					qpl::f64 value = 0.0;
					if (left_over == 0.0) {
						value = g.second.data[this->visible_index_start() + int_index].data;
					}
					else if (interpolation == qsf::vgraph::interpolation_type::cubic) {
						value = qpl::cubic_interpolation(g.second.data[this->visible_index_start() + int_index].data, g.second.data[this->visible_index_start() + int_index + this->index_skip_size].data, left_over);
					}
					else if (interpolation == qsf::vgraph::interpolation_type::linear) {
						value = qpl::linear_interpolation(g.second.data[this->visible_index_start() + int_index].data, g.second.data[this->visible_index_start() + int_index + this->index_skip_size].data, left_over);
					}

					auto y_progress = (value - low_padded) / (high_padded - low_padded);
					y_progress = qpl::clamp_0_1((1.0 - y_progress) * (1.0 - this->height_delta) + (this->height_delta) / 2);
					auto pos_y = this->position.y + this->dimension.y * y_progress;

					auto distance = qpl::abs(pos_y - event_info.mouse_position().y);

					if (min_distance > distance && distance < this->closest_graph_at_cursor_distance) {
						min_distance = distance;
						this->closest_graph_at_cursor = g.first;
						this->closest_graph_at_cursor_value = value;
						this->closest_graph_at_cursor_color = g.second.color;
						this->closest_graph_at_cursor_index = qpl::u32_cast(this->visible_index_start() + index + 0.5);
						if (this->closest_graph_at_cursor_color.is_unset()) {
							this->closest_graph_at_cursor_color = this->color;
						}
					}
				}
				for (auto& g : this->simple_graphs) {
					interpolation = interpolation;
					if (interpolation == qsf::vgraph::interpolation_type::unset) {
						interpolation = this->interpolation;
					}

					qpl::f64 value = 0.0;
					if (left_over == 0.0) {
						value = g.second.data[this->visible_index_start() + int_index].data;
					}
					else if (interpolation == qsf::vgraph::interpolation_type::cubic) {
						value = qpl::cubic_interpolation(g.second.data[this->visible_index_start() + int_index].data, g.second.data[this->visible_index_start() + int_index + this->index_skip_size].data, left_over);
					}
					else if (interpolation == qsf::vgraph::interpolation_type::linear) {
						value = qpl::linear_interpolation(g.second.data[this->visible_index_start() + int_index].data, g.second.data[this->visible_index_start() + int_index + this->index_skip_size].data, left_over);
					}

					auto y_progress = (value - low_padded) / (high_padded - low_padded);
					y_progress = qpl::clamp_0_1((1.0 - y_progress) * (1.0 - this->height_delta) + (this->height_delta) / 2);
					auto pos_y = this->position.y + this->dimension.y * y_progress;

					auto distance = qpl::abs(pos_y - event_info.mouse_position().y);

					if (min_distance > distance && distance < this->closest_graph_at_cursor_distance) {
						min_distance = distance;
						this->closest_graph_at_cursor = g.first;
						this->closest_graph_at_cursor_value = value;
						this->closest_graph_at_cursor_color = g.second.color;
						this->closest_graph_at_cursor_index = qpl::u32_cast(this->visible_index_start() + index + 0.5);
						if (this->closest_graph_at_cursor_color.is_unset()) {
							this->closest_graph_at_cursor_color = this->color;
						}
					}
				}
			}

		}
		if (event_info.left_mouse_released()) {
			this->drag = false;
			this->enable_last_n_when_dragging_right_lock = false;
		}
		if (event_info.scrolled_up()) {
			qsf::vrectangle rect(this->position, this->true_graph_dimension());
			rect.position.x += qpl::f32_cast(this->y_axis_text_space);

			if (rect.contains(event_info.mouse_position())) {
				auto progress = (event_info.mouse_position().x - rect.position.x) / rect.dimension.x;
				auto change = this->visible_element_size();
				auto start = qpl::i64_cast(this->visible_index_start());
				auto end = qpl::i64_cast(this->visible_index_end());

				change = qpl::size_cast(qpl::f64_cast(change) * (this->zoom_factor - 1));

				if (change < 5) {
					change = 0;
				}

				start += qpl::i64_cast(change * progress);
				end -= qpl::i64_cast(change * (1 - progress));

				if (start >= qpl::signed_cast(this->graph_element_size())) {
					start = qpl::i64_cast(this->graph_element_size()) - 1;
				}
				if (end < 0) {
					end = 0;
				}

				this->index_start = qpl::u32_cast(start);
				this->index_end = qpl::u32_cast(end);
				this->check_x_axis();
			}
		}
		if (event_info.scrolled_down()) {
			qsf::vrectangle rect(this->position, this->true_graph_dimension());
			rect.position.x += qpl::f32_cast(this->y_axis_text_space);

			if (rect.contains(event_info.mouse_position())) {
				auto progress = (event_info.mouse_position().x - rect.position.x) / rect.dimension.x;
				auto change = this->visible_element_size();
				auto start = qpl::i64_cast(this->visible_index_start());
				auto end = qpl::i64_cast(this->visible_index_end());

				change = qpl::size_cast(qpl::f64_cast(change) * (this->zoom_factor - 1));
				if (change == 0) {
					change = 1;
				}

				start -= qpl::i64_cast(change * progress);
				end += qpl::i64_cast(change * (1 - progress));

				if (end >= qpl::signed_cast(this->graph_element_size())) {
					end = qpl::i64_cast(this->graph_element_size());
				}
				if (start < 0) {
					start = 0;
				}
				this->index_start = qpl::size_cast(start);
				this->index_end = qpl::size_cast(end);

				this->check_x_axis();
			}
		}
	}

	void qsf::vgraph::copy_visible_range(const vgraph& other) {
		this->visible_index_range_before = this->visible_index_range();
		this->index_start = other.index_start;
		this->index_end = other.index_end;
		this->index_skip_size = other.index_skip_size;
		this->display_last_n_entries = other.display_last_n_entries;
	}
	void qsf::vgraph::set_visible_range(qpl::size begin, qpl::size end) {
		this->visible_index_range_before = this->visible_index_range();
		this->index_start = begin;
		this->index_end = end;
		this->check_x_axis();
		this->display_last_n_entries = 0u;
	}
	void qsf::vgraph::set_font(std::string name) {
		this->font = name;
		this->enable_axis_info();
	}
	std::string qsf::vgraph::get_font() const {
		return this->font;
	}
	void qsf::vgraph::set_dimension(qpl::vector2f dimension) {
		this->dimension = dimension;
	}
	void qsf::vgraph::set_position(qpl::vector2f position) {
		this->position = position;
	}

	qpl::f64 qsf::vgraph::true_graph_width() const {
		return this->dimension.x - this->y_axis_text_space;
	}
	qpl::f64 qsf::vgraph::true_graph_height() const {
		return this->dimension.y - this->x_axis_text_space;
	}
	qpl::vector2f qsf::vgraph::true_graph_dimension() const {
		return qpl::vector2f(this->true_graph_width(), this->true_graph_height());
	}
	std::pair<qpl::f64, qpl::f64> qsf::vgraph::get_low_high() const {
		qpl::f64 low, high;
		low = qpl::f64_max;
		high = qpl::f64_min;

		qpl::size interpolation_steps;
		qsf::vgraph::interpolation_type interpolation = qsf::vgraph::interpolation_type::unset;

		for (auto& g : this->info_graphs) {
			qpl::f64 min, max;

			interpolation_steps = g.second.interpolation_steps;
			if (interpolation_steps == qpl::size_max) {
				interpolation_steps = this->interpolation_steps;
			}


			interpolation = g.second.interpolation;
			if (interpolation == qsf::vgraph::interpolation_type::unset) {
				interpolation = this->interpolation;
			}

			if (interpolation == qsf::vgraph::interpolation_type::cubic) {
				auto span = this->get_info_graph_span(g.first);

				auto result = qpl::cubic_vector_interpolation_min_max(span, interpolation_steps, 0, this->index_skip_size, qsf::vgraph::data_point_info{ qpl::f64_max }, qsf::vgraph::data_point_info{ qpl::f64_min });

				std::tie(min, max) = std::make_pair(result.first.data, result.second.data);
			}
			else {
				auto span = this->get_info_graph_span(g.first);

				auto result = qpl::min_max_vector(span, this->index_skip_size);

				std::tie(min, max) = std::make_pair(result.first.data, result.second.data);
			}


			low = qpl::min(min, low);
			high = qpl::max(max, high);
		}
		for (auto& g : this->standard_graphs) {
			qpl::f64 min, max;

			interpolation_steps = g.second.interpolation_steps;
			if (interpolation_steps == qpl::size_max) {
				interpolation_steps = this->interpolation_steps;
			}


			interpolation = g.second.interpolation;
			if (interpolation == qsf::vgraph::interpolation_type::unset) {
				interpolation = this->interpolation;
			}

			if (interpolation == qsf::vgraph::interpolation_type::cubic) {
				auto span = this->get_standard_graph_span(g.first);

				auto result = qpl::cubic_vector_interpolation_min_max(span, interpolation_steps, 0, this->index_skip_size, qsf::vgraph::data_point{ qpl::f64_max }, qsf::vgraph::data_point{ qpl::f64_min });

				std::tie(min, max) = std::make_pair(result.first.data, result.second.data);
			}
			else {
				auto span = this->get_standard_graph_span(g.first);

				auto result = qpl::min_max_vector(span, this->index_skip_size);

				std::tie(min, max) = std::make_pair(result.first.data, result.second.data);
			}


			low = qpl::min(min, low);
			high = qpl::max(max, high);
		}
		for (auto& g : this->simple_graphs) {
			qpl::f64 min, max;

			interpolation_steps = g.second.interpolation_steps;
			if (interpolation_steps == qpl::size_max) {
				interpolation_steps = this->interpolation_steps;
			}


			interpolation = g.second.interpolation;
			if (interpolation == qsf::vgraph::interpolation_type::unset) {
				interpolation = this->interpolation;
			}

			if (interpolation == qsf::vgraph::interpolation_type::cubic) {
				auto span = this->get_simple_graph_span(g.first);


				auto result = qpl::cubic_vector_interpolation_min_max(span, interpolation_steps, 0, this->index_skip_size, qsf::vgraph::data_point_simple{ qpl::f64_max }, qsf::vgraph::data_point_simple{ qpl::f64_min });

				std::tie(min, max) = std::make_pair(result.first.data, result.second.data);
			}
			else {
				auto span = this->get_simple_graph_span(g.first);

				auto result = qpl::min_max_vector(span, this->index_skip_size);

				std::tie(min, max) = std::make_pair(result.first.data, result.second.data);

			}


			low = qpl::min(min, low);
			high = qpl::max(max, high);
		}

		low = qpl::clamp(this->min_min_value, low, this->min_max_value);
		high = qpl::clamp(this->max_min_value, high, this->max_max_value);

		if (this->y_axis_start_at_0) {
			if (low < 0 && high < 0) {
				high = 0;
			}
			if (low > 0 && high > 0) {
				low = 0;
			}
		}
		if (low == high) {
			low -= 1;
			high += 1;
		}

		return std::make_pair(low, high);
	}
	qpl::size qsf::vgraph::total_graph_size() const {
		return this->standard_graphs.size() + this->info_graphs.size() + this->simple_graphs.size();
	}
	qpl::size qsf::vgraph::graph_element_size() const {
		qpl::size result = qpl::size_max;
		for (auto& i : this->info_graphs) {
			if (result == qpl::size_max) {
				result = i.second.size();
			}
			else {
				if (result != i.second.size()) {
					throw std::exception(qpl::to_string("qsf::vgraph: graphs have different element sizes (", i.first, ")").c_str());
				}
			}
		}
		for (auto& i : this->standard_graphs) {
			if (result == qpl::size_max) {
				result = i.second.size();
			}
			else {
				if (result != i.second.size()) {
					throw std::exception(qpl::to_string("qsf::vgraph: graphs have different element sizes (", i.first, ")").c_str());
				}
			}
		}
		for (auto& i : this->simple_graphs) {
			if (result == qpl::size_max) {
				result = i.second.size();
			}
			else {
				if (result != i.second.size()) {
					throw std::exception(qpl::to_string("qsf::vgraph: graphs have different element sizes (", i.first, ")").c_str());
				}
			}
		}
		if (result == qpl::size_max) {
			return qpl::size{ 0u };
		}
		return result;
	}
	qpl::size qsf::vgraph::visible_element_size() const {
		if (this->is_range_enabled()) {
			return (this->index_end - this->index_start);
		}
		else if (this->display_last_n_entries) {
			auto size = this->graph_element_size();
			return qpl::min(size, this->display_last_n_entries);
		}
		else {
			return this->graph_element_size();
		}
	}
	qpl::size qsf::vgraph::visible_index_start() const {
		if (this->is_range_enabled()) {
			return this->index_start;
		}
		else if (this->display_last_n_entries) {
			auto size = this->graph_element_size();
			if (size >= this->display_last_n_entries) {
				return size - this->display_last_n_entries;
			}
		}
		return 0u;
	}
	qpl::size qsf::vgraph::visible_index_end() const {
		if (this->is_range_enabled()) {
			return this->index_end;
		}
		else if (this->display_last_n_entries) {
			auto size = this->visible_element_size();
			return this->visible_index_start() + size;
		}
		return this->visible_element_size();
	}
	std::pair<qpl::size, qpl::size> qsf::vgraph::visible_index_range() const {
		return std::make_pair(this->visible_index_start(), this->visible_index_end());
	}
	bool qsf::vgraph::is_range_enabled() const {
		return !(this->index_start == 0u && this->index_end == qpl::size_max);
	}

	void qsf::vgraph::add_info_graph(std::string name) {
		if (this->info_graphs.find(name) == this->info_graphs.cend()) {
			this->info_graphs[name];
		}
	}
	qsf::vgraph::info_graph& qsf::vgraph::get_info_graph(std::string name) {
		return this->info_graphs[name];
	}
	const qsf::vgraph::info_graph& qsf::vgraph::get_info_graph(std::string name) const {
		return this->info_graphs.at(name);
	}
	std::span<const qsf::vgraph::data_point_info> qsf::vgraph::get_info_graph_span(std::string name) const {
		auto start = this->visible_index_start();
		auto end = this->visible_index_end();

		auto begin = this->get_info_graph(name).data.begin();
		return std::span(begin + start, begin + end);
	}

	void qsf::vgraph::add_standard_graph(std::string name) {
		if (this->standard_graphs.find(name) == this->standard_graphs.cend()) {
			this->standard_graphs[name];
		}
	}
	qsf::vgraph::standard_graph& qsf::vgraph::get_standard_graph(std::string name) {
		return this->standard_graphs[name];
	}
	const qsf::vgraph::standard_graph& qsf::vgraph::get_standard_graph(std::string name) const {
		return this->standard_graphs.at(name);
	}
	std::span<const qsf::vgraph::data_point> qsf::vgraph::get_standard_graph_span(std::string name) const {
		auto start = this->visible_index_start();
		auto end = this->visible_index_end();

		auto begin = this->get_standard_graph(name).data.begin();
		return std::span(begin + start, begin + end);
	}

	void qsf::vgraph::add_simple_graph(std::string name) {
		if (this->simple_graphs.find(name) == this->simple_graphs.cend()) {
			this->simple_graphs[name];
		}
	}
	qsf::vgraph::simple_graph& qsf::vgraph::get_simple_graph(std::string name) {
		return this->simple_graphs[name];
	}
	const qsf::vgraph::simple_graph& qsf::vgraph::get_simple_graph(std::string name) const {
		return this->simple_graphs.at(name);
	}
	std::span<const qsf::vgraph::data_point_simple> qsf::vgraph::get_simple_graph_span(std::string name) const {
		auto start = this->visible_index_start();
		auto end = this->visible_index_end();

		auto begin = this->get_simple_graph(name).data.begin();
		return std::span(begin + start, begin + end);
	}

	void qsf::vgraph::add_y_axis_line(qpl::f64 value) {
		this->add_y_axis_line(value, qpl::rgb::red, this->axis_thickness / 2);
	}
	void qsf::vgraph::add_y_axis_line(qpl::f64 value, qpl::rgb color, qpl::f64 thickness, bool foreground) {
		this->y_axis_highlighted_lines[value].color = color;
		this->y_axis_highlighted_lines[value].thickness = thickness;
		this->y_axis_highlighted_lines[value].foreground = foreground;
	}
	qsf::vgraph::highlighted_line& qsf::vgraph::get_y_axis_line(qpl::f64 value) {
		if (this->y_axis_highlighted_lines.find(value) == this->y_axis_highlighted_lines.cend()) {
			this->y_axis_highlighted_lines[value].color = qpl::rgb::red;
			this->y_axis_highlighted_lines[value].thickness = this->axis_thickness;
		}
		return this->y_axis_highlighted_lines[value];
	}
	const qsf::vgraph::highlighted_line& qsf::vgraph::get_y_axis_line(qpl::f64 value) const {
		return this->y_axis_highlighted_lines.at(value);
	}

	qsf::graph& qsf::graph::operator=(const qsf::vgraph& graph) {
		if (graph.empty() || graph.graph_element_size() == 0u) {
			return *this;
		}
		graph.update_change();

		std::string used_font = graph.font;

		if (used_font.empty()) {
			used_font = graph.y_axis_text.font_name;
		}
		if (used_font.empty()) {
			used_font = graph.x_axis_text.font_name;
		}

		this->lines.resize(graph.total_graph_size());
		this->circle_texts.resize(graph.info_graphs.size());
		this->circles.resize(graph.info_graphs.size());


		qpl::size interpolation_steps;
		qsf::vgraph::interpolation_type interpolation = qsf::vgraph::interpolation_type::unset;

		for (auto& g : graph.info_graphs) {
			interpolation_steps = g.second.interpolation_steps;
			if (interpolation_steps == qpl::size_max) {
				interpolation_steps = graph.interpolation_steps;
			}
		}
		for (auto& g : graph.standard_graphs) {
			interpolation_steps = g.second.interpolation_steps;
			if (interpolation_steps == qpl::size_max) {
				interpolation_steps = graph.interpolation_steps;
			}

			interpolation = g.second.interpolation;
			if (interpolation == qsf::vgraph::interpolation_type::unset) {
				interpolation = graph.interpolation;
			}
		}
		for (auto& g : graph.simple_graphs) {
			interpolation_steps = g.second.interpolation_steps;
			if (interpolation_steps == qpl::size_max) {
				interpolation_steps = graph.interpolation_steps;
			}

			interpolation = g.second.interpolation;
			if (interpolation == qsf::vgraph::interpolation_type::unset) {
				interpolation = graph.interpolation;
			}
		}

		qpl::f64 low, high;
		std::tie(low, high) = graph.get_low_high();

		auto y_range = (high - low);
		if (high == qpl::f64_min && low == qpl::f64_max) {
			y_range = 0.0;
		}
		auto low_padded = low - y_range * graph.y_axis_padding;
		auto high_padded = high + y_range * graph.y_axis_padding;

		qpl::u32 u = 0u;
		for (auto& g : graph.info_graphs) {
			std::vector<qsf::vgraph::data_point_info> interpolated_data;

			auto span = graph.get_info_graph_span(g.first);

			if (interpolation == qsf::vgraph::interpolation_type::cubic) {
				interpolated_data = qpl::cubic_vector_interpolation(span, interpolation_steps, graph.index_skip_size);
			}
			else if (interpolation == qsf::vgraph::interpolation_type::linear) {
				interpolated_data = qpl::linear_vector_interpolation(span, interpolation_steps, graph.index_skip_size);
			}

			bool use_interpolated_thickness = false;
			bool use_interpolated_color = false;

			this->circles[u].clear();
			this->circle_texts[u].clear();

			for (qpl::u32 i = 0u; i < span.size(); ++i) {
				if (!use_interpolated_thickness && span[i].thickness != qpl::f64_min) {
					use_interpolated_thickness = true;
				}

				if (!use_interpolated_color && span[i].color != qpl::frgb::unset) {
					use_interpolated_color = true;
				}

				if (!use_interpolated_thickness && !use_interpolated_color) {
					break;
				}
			}

			qpl::f64 using_thickness = g.second.thickness;
			if (using_thickness == qpl::f64_min) {
				using_thickness = graph.thickness;
			}

			if (graph.display_closest_graph_at_cursor && !graph.closest_graph_at_cursor.empty() && graph.closest_graph_at_cursor == g.first && graph.closest_graph_at_cursor_color == g.second.color) {
				using_thickness *= graph.closest_graph_at_cursor_multiply_thickness;
			}
			qpl::frgb using_color = g.second.color;
			if (using_color == qpl::frgb::unset) {
				using_color = graph.color;
			}

			this->lines[u].clear();
			for (qpl::u32 i = 0u; i < interpolated_data.size(); ++i) {

				qpl::vector2f position;
				position.x = qpl::f32_cast(graph.position.x + (graph.dimension.x - graph.y_axis_text_space) * (i / static_cast<double>(interpolated_data.size() - 1)) + graph.y_axis_text_space);


				auto y_progress = (interpolated_data[i].data - low_padded) / (high_padded - low_padded);
				y_progress = qpl::clamp_0_1((1.0 - y_progress) * (1.0 - graph.height_delta) + (graph.height_delta) / 2);
				position.y = qpl::f32_cast(graph.position.y + graph.dimension.y * y_progress);

				auto color = use_interpolated_color ? interpolated_data[i].color : using_color;
				auto thickness = qpl::f32_cast(use_interpolated_thickness ? interpolated_data[i].thickness : using_thickness);
				this->lines[u].add_thick_line(position, color, thickness);


				if (g.second.only_draw_if_close && graph.index_skip_size == 1) {
					if (interpolated_data[i].circle.radius > 0.0) {

						//graph.data[i].circle.point.position = position;

						this->circles[u].add_circle(position, interpolated_data[i].circle.radius, interpolated_data[i].circle.point.color);
					}

					if (!interpolated_data[i].text.string.empty()) {
						this->circle_texts[u].push_back(interpolated_data[i].text);
						if (interpolated_data[i].text.font_name.empty()) {
							this->circle_texts[u].back().set_font(used_font);
						}
						else {
							if (used_font.empty()) {
								used_font = interpolated_data[i].text.font_name;
							}
							this->circle_texts[u].back().set_font(interpolated_data[i].text.font_name);
						}
						this->circle_texts[u].back().set_position(position);
						this->circle_texts[u].back().centerize();
					}
				}

			}

			++u;
		}
		for (auto& g : graph.standard_graphs) {
			std::vector<qsf::vgraph::data_point> interpolated_data;

			auto span = graph.get_standard_graph_span(g.first);

			if (interpolation == qsf::vgraph::interpolation_type::cubic) {
				interpolated_data = qpl::cubic_vector_interpolation(span, interpolation_steps, graph.index_skip_size);
			}
			else if (interpolation == qsf::vgraph::interpolation_type::linear) {
				interpolated_data = qpl::linear_vector_interpolation(span, interpolation_steps, graph.index_skip_size);
			}

			bool use_interpolated_thickness = false;
			bool use_interpolated_color = false;

			for (qpl::u32 i = 0u; i < span.size(); ++i) {
				if (!use_interpolated_thickness && span[i].thickness != qpl::f64_min) {
					use_interpolated_thickness = true;
				}

				if (!use_interpolated_color && span[i].color != qpl::frgb::unset) {
					use_interpolated_color = true;
				}

				if (!use_interpolated_thickness && !use_interpolated_color) {
					break;
				}
			}

			qpl::f64 using_thickness = g.second.thickness;
			if (using_thickness == qpl::f64_min) {
				using_thickness = graph.thickness;
			}

			if (graph.display_closest_graph_at_cursor && !graph.closest_graph_at_cursor.empty() && graph.closest_graph_at_cursor == g.first && graph.closest_graph_at_cursor_color == g.second.color) {
				using_thickness *= graph.closest_graph_at_cursor_multiply_thickness;
			}

			qpl::frgb using_color = g.second.color;
			if (using_color == qpl::frgb::unset) {
				using_color = graph.color;
			}


			this->lines[u].clear();
			for (qpl::u32 i = 0u; i < interpolated_data.size(); ++i) {

				qpl::vector2f position;
				position.x = qpl::f32_cast(graph.position.x + (graph.dimension.x - graph.y_axis_text_space) * (i / static_cast<double>(interpolated_data.size() - 1)) + graph.y_axis_text_space);


				auto y_progress = (interpolated_data[i].data - low_padded) / (high_padded - low_padded);
				y_progress = qpl::clamp_0_1((1.0 - y_progress) * (1.0 - graph.height_delta) + (graph.height_delta) / 2);
				position.y = qpl::f32_cast(graph.position.y + graph.dimension.y * y_progress);

				auto color = use_interpolated_color ? interpolated_data[i].color : using_color;
				auto thickness = qpl::f32_cast(use_interpolated_thickness ? interpolated_data[i].thickness : using_thickness);
				this->lines[u].add_thick_line(position, color, thickness);
			}


			++u;
		}
		for (auto& g : graph.simple_graphs) {
			std::vector<qsf::vgraph::data_point_simple> interpolated_data;

			auto span = graph.get_simple_graph_span(g.first);

			if (interpolation == qsf::vgraph::interpolation_type::cubic) {
				interpolated_data = qpl::cubic_vector_interpolation(span, interpolation_steps, graph.index_skip_size);
			}
			else if (interpolation == qsf::vgraph::interpolation_type::linear) {
				interpolated_data = qpl::linear_vector_interpolation(span, interpolation_steps, graph.index_skip_size);
			}

			qpl::f64 using_thickness = g.second.thickness;
			if (using_thickness == qpl::f64_min) {
				using_thickness = graph.thickness;
			}


			if (graph.display_closest_graph_at_cursor && !graph.closest_graph_at_cursor.empty() && graph.closest_graph_at_cursor == g.first && graph.closest_graph_at_cursor_color == g.second.color) {
				using_thickness *= graph.closest_graph_at_cursor_multiply_thickness;
			}

			qpl::frgb using_color = g.second.color;
			if (using_color == qpl::frgb::unset) {
				using_color = graph.color;
			}


			this->lines[u].clear();

			for (qpl::u32 i = 0u; i < interpolated_data.size(); ++i) {

				qpl::vector2f position;
				position.x = qpl::f32_cast(graph.position.x + (graph.dimension.x - graph.y_axis_text_space) * (i / static_cast<double>(interpolated_data.size() - 1)) + graph.y_axis_text_space);


				auto y_progress = (interpolated_data[i].data - low_padded) / (high_padded - low_padded);
				y_progress = qpl::clamp_0_1((1.0 - y_progress) * (1.0 - graph.height_delta) + (graph.height_delta) / 2);
				position.y = qpl::f32_cast(graph.position.y + graph.dimension.y * y_progress);

				auto color = using_color;
				auto thickness = qpl::f32_cast(using_thickness);
				this->lines[u].add_thick_line(position, color, thickness);
			}


			++u;
		}

		if (u) {
			qpl::rgb use_color = graph.axis_color;


			if (graph.use_y_axis) {
				auto exponent = static_cast<qpl::i64>(std::log10(y_range)) - 1;
				auto multiply = std::pow(10, -exponent);
				y_range *= multiply;

				qpl::f64 y_delta = 0.0;
				qpl::f64 y_start = 0.0;
				qpl::f64 y_end = 0.0;
				qpl::u32 y_steps = 0;

				if (!used_font.empty() && y_range != 0) {
					y_delta = qpl::i64_cast(y_range) / (multiply * graph.desired_y_axis_lines);

					y_start = qpl::i64_cast(low_padded / y_delta + 1) * y_delta;
					y_end = qpl::i64_cast(high_padded / y_delta) * y_delta;
					y_steps = qpl::u32_cast(qpl::i64_cast((y_end - y_start) / y_delta) + 2);
				}

				this->y_lines.resize(y_steps);
				this->y_lines_foreground.clear();
				this->y_texts.resize(y_steps);

				for (auto& y : graph.y_axis_highlighted_lines) {
					if (y.first >= low_padded && y.first <= high_padded) {

						auto y_position = y.first;
						auto y_progress = ((y_position)-low_padded) / (high_padded - low_padded);
						y_progress = (1.0 - y_progress) * (1.0 - graph.height_delta) + (graph.height_delta) / 2;

						qpl::vector2f position;
						position.x = graph.position.x;
						position.y = qpl::f32_cast(graph.position.y + graph.true_graph_height() * y_progress + graph.x_axis_text_space);

						auto a = position;
						auto b = a;
						b.x += graph.dimension.x;

						if (y.second.foreground) {
							this->y_lines_foreground.resize(this->y_lines_foreground.size() + 1);
							this->y_lines_foreground.back().clear();
							this->y_lines_foreground.back().add_thick_line(a, y.second.color, qpl::f32_cast(y.second.thickness));
							this->y_lines_foreground.back().add_thick_line(b, y.second.color, qpl::f32_cast(y.second.thickness));
						}
						else {
							this->y_lines.resize(this->y_lines.size() + 1);
							this->y_lines.back().clear();
							this->y_lines.back().add_thick_line(a, y.second.color,  qpl::f32_cast(y.second.thickness));
							this->y_lines.back().add_thick_line(b, y.second.color,  qpl::f32_cast(y.second.thickness));
						}
					}
				}
				for (qpl::u32 i = 0u; i < y_steps; ++i) {
					auto y_position = y_start + y_delta * i;
					auto y_progress = ((y_position)-low_padded) / (high_padded - low_padded);
					y_progress = (1.0 - y_progress) * (1.0 - graph.height_delta) + (graph.height_delta) / 2;
					qpl::vector2f position;
					position.x = graph.position.x;
					position.y = qpl::f32_cast(graph.position.y + graph.true_graph_height() * y_progress + graph.x_axis_text_space);



					this->y_texts[i] = graph.y_axis_text;
					if (this->y_texts[i].get_font().empty()) {
						this->y_texts[i].set_font(used_font);
					}
					if (use_color.is_unset()) {
						if (!graph.y_axis_color.is_unset()) {
							use_color = graph.y_axis_color;
							this->y_texts[i].set_color(use_color);
						}
					}
					else {
						this->y_texts[i].set_color(use_color);
					}
					if (graph.y_axis_text_left) {
						if (graph.y_axis_string_function) {
							this->y_texts[i].set_string(graph.y_axis_string_function(y_position));
						}
						else if (graph.y_axis_text_percent) {
							this->y_texts[i].set_string(graph.y_axis_text.string + qpl::to_string_precision(graph.y_axis_text_precision, y_position * 100) + "%");
						}
						else if (graph.y_axis_text_integer) {
							this->y_texts[i].set_string(graph.y_axis_text.string + qpl::to_string(qpl::i64_cast(y_position)));
						}
						else {
							this->y_texts[i].set_string(graph.y_axis_text.string + qpl::to_string_precision(graph.y_axis_text_precision, y_position));
						}
					}
					else {
						if (graph.y_axis_string_function) {
							this->y_texts[i].set_string(graph.y_axis_string_function(y_position));
						}
						else if (graph.y_axis_text_percent) {
							this->y_texts[i].set_string(qpl::to_string_precision(graph.y_axis_text_precision, y_position * 100) + "%" + graph.y_axis_text.string);
						}
						else if (graph.y_axis_text_integer) {
							this->y_texts[i].set_string(qpl::to_string(qpl::i64_cast(y_position)));
						}
						else {
							this->y_texts[i].set_string(qpl::to_string_precision(graph.y_axis_text_precision, y_position) + graph.y_axis_text.string);
						}
					}
					this->y_texts[i].set_position(position);
					this->y_texts[i].centerize_y();

					auto a = position;
					auto b = a;
					b.x += graph.dimension.x;

					this->y_lines[i].clear();
					this->y_lines[i].add_thick_line(a, graph.axis_line_color,  qpl::f32_cast(graph.axis_thickness));
					this->y_lines[i].add_thick_line(b, graph.axis_line_color,  qpl::f32_cast(graph.axis_thickness));
				}

			}
			else {
				this->y_lines.clear();
				this->y_texts.clear();
			}

			if (graph.use_x_axis) {
				auto index_mod = graph.x_axis_line_frequency - (graph.visible_index_start() % graph.x_axis_line_frequency) - 1;
				auto x_size = graph.visible_element_size() / graph.x_axis_line_frequency + 1;
				this->x_lines.resize(x_size);
				this->x_texts.resize(x_size);

				for (qpl::u32 i = 0u; i < x_size; ++i) {
					auto progress = (((i * graph.x_axis_line_frequency) + index_mod)) / static_cast<double>(graph.visible_element_size() - 1);
					auto multiple = qpl::approximate_multiple_up(graph.visible_index_start(), graph.x_axis_line_frequency);
					if (graph.visible_index_start() == 0u) {
						multiple = qpl::u32_cast(graph.x_axis_line_frequency);
					}
					qpl::size index = multiple + qpl::size_cast(i * graph.x_axis_line_frequency);



					qpl::vector2f position;
					position.x = qpl::f32_cast(graph.position.x + (graph.true_graph_width()) * progress + graph.y_axis_text_space);
					position.y = graph.position.y + graph.dimension.y;

					this->x_texts[i] = graph.x_axis_text;
					if (this->x_texts[i].get_font().empty()) {
						this->x_texts[i].set_font(used_font);
					}

					if (use_color.is_unset()) {
						if (!graph.x_axis_color.is_unset()) {
							use_color = graph.x_axis_color;
							this->x_texts[i].set_color(use_color);
						}
					}
					else {
						this->x_texts[i].set_color(use_color);
					}
					this->x_texts[i].set_string(graph.x_axis_text.string + graph.x_axis_string_function(index));
					this->x_texts[i].set_position(position.moved(0, -qpl::signed_cast(graph.x_axis_text.character_size)));
					this->x_texts[i].centerize_x();

					auto a = position;
					a.y -= qpl::f32_cast(graph.x_axis_text_space);
					auto b = a;
					b.y = graph.position.y;

					this->x_lines[i].clear();
					this->x_lines[i].add_thick_line(a, graph.axis_line_color, qpl::f32_cast(graph.axis_thickness));
					this->x_lines[i].add_thick_line(b, graph.axis_line_color, qpl::f32_cast(graph.axis_thickness));
				}
			}
			else {
				this->x_lines.clear();
				this->x_texts.clear();
			}
		}

		this->cursor_graph_text.set_color(qpl::rgb::unset);
		if (graph.display_closest_graph_at_cursor && !graph.closest_graph_at_cursor.empty()) {
			this->cursor_graph_text = graph.closest_graph_at_cursor_text;
			if (this->cursor_graph_text.get_font().empty()) {
				this->cursor_graph_text.set_font(used_font);
			}
			if (graph.closest_graph_at_cursor_string_function) {
				this->cursor_graph_text.set_string(graph.closest_graph_at_cursor_string_function(graph.closest_graph_at_cursor, graph.closest_graph_at_cursor_value, graph.closest_graph_at_cursor_index));
			}
			else {
				if (graph.y_axis_cursor_string_function) {
					this->cursor_graph_text.set_string(qpl::to_string(graph.closest_graph_at_cursor, " : ", graph.y_axis_cursor_string_function(graph.closest_graph_at_cursor_value)));
				}
				else {
					if (graph.y_axis_string_function) {
						this->cursor_graph_text.set_string(qpl::to_string(graph.closest_graph_at_cursor, " : ", graph.y_axis_string_function(graph.closest_graph_at_cursor_value)));
					}
					else {
						this->cursor_graph_text.set_string(qpl::to_string(graph.closest_graph_at_cursor, " = ", graph.closest_graph_at_cursor_value));
					}
				}
			}
			this->cursor_graph_text.set_color(graph.closest_graph_at_cursor_color.with_alpha(255));
			this->cursor_graph_text.set_position(graph.mouse_position);

			auto hitbox = this->cursor_graph_text.get_visible_hitbox();
			this->cursor_graph_text.move(qpl::vector2f{ 15, -hitbox.dimension.y - 15 });
			hitbox = this->cursor_graph_text.get_visible_hitbox();
			auto diff = (hitbox.position.x + hitbox.dimension.x) - (graph.position.x + graph.dimension.x) + 15;
			if (diff > 0) {
				this->cursor_graph_text.move(qpl::vector2f{ -diff, 0 });
				hitbox = this->cursor_graph_text.get_visible_hitbox();
			}

			this->cursor_graph_background = graph.closest_graph_at_cursor_background;
			if (graph.closest_graph_at_cursor_background_is_graph_background) {
				this->cursor_graph_background.set_color(graph.background_color.with_alpha(255));
			}
			this->cursor_graph_background.set_dimension(hitbox.dimension + 5);
			this->cursor_graph_background.set_center(hitbox.get_center());
			this->cursor_graph_background.set_outline_color(graph.closest_graph_at_cursor_color.with_alpha(255));
		}


		this->background.set_position(graph.position);
		this->background.set_dimension(graph.dimension);
		this->background.set_color(graph.background_color);
		return *this;
	}
	void qsf::graph::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		if (this->background.m_rect.getFillColor().a) {
			this->background.draw(window, states);
		}

		for (qpl::u32 i = 0u; i < this->y_lines.size(); ++i) {
			this->y_lines[i].draw(window, states);
		}
		for (qpl::u32 i = 0u; i < this->x_lines.size(); ++i) {
			this->x_lines[i].draw(window, states);
		}
		for (qpl::u32 i = 0u; i < this->y_texts.size(); ++i) {
			this->y_texts[i].draw(window, states);
		}
		for (qpl::u32 i = 0u; i < this->x_texts.size(); ++i) {
			this->x_texts[i].draw(window, states);
		}
		for (qpl::u32 i = 0u; i < this->lines.size(); ++i) {
			this->lines[i].draw(window, states);
		}
		for (qpl::u32 i = 0u; i < this->circles.size(); ++i) {
			this->circles[i].draw(window, states);
			for (auto& i : this->circle_texts[i]) {
				i.draw(window, states);
			}
		}
		for (qpl::u32 i = 0u; i < this->y_lines_foreground.size(); ++i) {
			this->y_lines_foreground[i].draw(window, states);
		}
		if (!this->cursor_graph_text.get_color().is_unset()) {
			this->cursor_graph_background.draw(window, states);
			this->cursor_graph_text.draw(window, states);
		}
	}

	qpl::vector2f qsf::vbutton::get_position() const {
		return this->background.position;
	}
	qpl::rgb qsf::vbutton::get_final_text_color() const {
		return this->text.color.multiplied_alpha(this->text_alpha);
	}
	qpl::rgb qsf::vbutton::get_final_background_color() const {
		return this->background.color.multiplied_alpha(this->background_alpha);
	}
	void qsf::vbutton::set_dimension(qpl::vector2f dimension) {
		this->background.dimension = dimension;
	}
	void qsf::vbutton::set_position(qpl::vector2f position) {
		this->background.position = position;
	}
	void qsf::vbutton::set_center(qpl::vector2f center) {
		this->background.set_center(center);
	}
	void qsf::vbutton::set_background_color(qpl::rgb color) {
		this->background.set_color(color);
		this->background_color = color;
	}
	void qsf::vbutton::set_hover_background_color(qpl::rgb color) {
		this->hover_background_color = color;
	}
	void qsf::vbutton::set_text_color(qpl::rgb color) {
		this->text.set_color(color);
		this->text_color = color;
	}
	void qsf::vbutton::set_hover_text_color(qpl::rgb color) {
		this->hover_text_color = color;
	}
	void qsf::vbutton::set_text_font(std::string font) {
		this->text.set_font(font);
	}
	void qsf::vbutton::set_text_character_size(qpl::u32 character_size) {
		this->text.set_character_size(character_size);
	}
	void qsf::vbutton::set_text_style(qpl::u32 character_style) {
		this->text.set_style(character_style);
	}
	void qsf::vbutton::set_text(std::string text) {
		this->text.set_string(text);
	}
	void qsf::vbutton::centerize_text() {
		this->text.set_position(this->background.get_center());
	}
	bool qsf::vbutton::is_hovering() const {
		return this->hovering;
	}
	bool qsf::vbutton::is_clicked() const {
		return this->clicked;
	}
	void qsf::vbutton::update(const event_info& event_info) {
		auto pos = event_info.mouse_position();

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

	void qsf::vbutton::set_text_alpha(qpl::u8 alpha) {
		this->text_alpha = alpha;
	}
	void qsf::vbutton::set_background_alpha(qpl::u8 alpha) {
		this->background_alpha = alpha;
	}

	void qsf::vbutton::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::button = *this;
		qsf::detail::button.draw(window, states);
	}
	qsf::vbutton& qsf::vbutton::operator=(const qsf::vbutton& button) {
		this->outline_on_hover = button.outline_on_hover;
		this->invert_on_hover = button.invert_on_hover;
		this->text_color = button.text_color;
		this->hover_background_color = button.hover_background_color;
		this->background_color = button.background_color;
		this->text = button.text;
		this->background = button.background;
		this->hovering = button.hovering;
		this->clicked = button.clicked;
		this->background_alpha = button.background_alpha;
		this->text_alpha = button.text_alpha;
		return *this;
	}

	void qsf::button::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		this->background.draw(window, states);
		this->text.draw(window, states);
	}
	qsf::button& qsf::button::operator=(const qsf::vbutton& button) {
		this->background = button.background;
		this->background.set_color(button.get_final_background_color());

		this->text = button.text;
		this->text.set_color(button.get_final_text_color());

		this->text.set_center(this->background.center());
		return *this;
	}

	void qsf::button::set_dimension(qpl::vector2f dimension) {
		this->background.set_dimension(dimension);
	}
	void qsf::button::set_position(qpl::vector2f position) {
		this->background.set_position(position);
	}
	void qsf::button::set_center(qpl::vector2f center) {
		this->background.set_center(center);
	}
	void qsf::button::set_background_color(qpl::rgb color) {
		this->background.set_color(color);
		this->background_color = color;
	}
	void qsf::button::set_hover_background_color(qpl::rgb color) {
		this->hover_background_color = color;
	}
	void qsf::button::set_text_color(qpl::rgb color) {
		this->text.set_color(color);
		this->text_color = color;
	}
	void qsf::button::set_text_font(std::string font) {
		this->text.set_font(font);
	}
	void qsf::button::set_text_character_size(qpl::u32 character_size) {
		this->text.set_character_size(character_size);
	}
	void qsf::button::set_text_style(qpl::u32 character_style) {
		this->text.set_style(character_style);
	}
	void qsf::button::set_text_string(std::string text) {
		this->text.set_string(text);
	}
	void qsf::button::set_text_string(std::wstring text) {
		this->text.set_string(text);
	}

	qpl::vector2f qsf::button::get_dimension() const {
		return this->background.get_dimension();
	}
	qpl::vector2f qsf::button::get_position() const {
		return this->background.get_position();
	}
	qpl::vector2f qsf::button::get_center() const {
		return this->background.get_center();
	}
	qpl::rgb qsf::button::get_background_color() const {
		return this->background_color;
	}
	qpl::rgb qsf::button::get_hover_background_color() const {
		return this->hover_background_color;
	}
	qpl::rgb qsf::button::get_text_color() const {
		return this->text_color;
	}
	std::string qsf::button::get_text_font() const {
		return this->text.get_font();
	}
	qpl::u32 qsf::button::get_text_character_size() const {
		return this->text.get_character_size();
	}
	qpl::u32 qsf::button::get_text_style() const {
		return this->text.get_style();
	}
	std::string qsf::button::get_text_string() const {
		return this->text.get_string();
	}
	std::wstring qsf::button::get_text_wstring() const {
		return this->text.get_wstring();
	}
	void qsf::button::centerize_text() {
		this->text.set_center(this->background.center());
	}
	bool qsf::button::is_hovering() const {
		return this->hovering;
	}
	bool qsf::button::is_clicked() const {
		return this->clicked;
	}
	void qsf::button::update(const event_info& event_info) {
		auto pos = event_info.mouse_position();

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
	void qsf::button::update(const event_info& event_info, bool& hovering) {
		this->update(event_info);
		hovering = hovering || this->hovering;
	}

	void qsf::smooth_button::enable_simple_hitbox() {
		this->simple_hitbox = true;
	}
	void qsf::smooth_button::disable_simple_hitbox() {
		this->simple_hitbox = false;
	}
	void qsf::smooth_button::set_hitbox_increase(qpl::vector2f delta) {
		this->hitbox_increase = delta;
	}
	void qsf::smooth_button::set_hitbox_increase(qpl::f32 delta) {
		this->hitbox_increase = qpl::vec(delta, delta);
	}
	void qsf::smooth_button::set_multiplied_color(qpl::rgb color) {
		this->multiplied_color = color;
		this->text.set_multiplied_color(color);
		this->smooth_layout.set_multiplied_color(color);
		this->rectangle.polygon.set_multiplied_color(color);
	}
	void qsf::smooth_button::set_dimension(qpl::vector2f dimension) {
		this->smooth_layout.set_dimension(dimension);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_position(qpl::vector2f position) {
		this->smooth_layout.set_position(position);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_hitbox(qpl::hitbox hitbox) {
		this->smooth_layout.set_hitbox(hitbox);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_center(qpl::vector2f center) {
		this->smooth_layout.set_center(center);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_slope(qpl::f64 slope) {
		this->smooth_layout.set_slope(slope);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_background_color(qpl::rgb color) {
		this->smooth_layout.set_color(color);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_background_outline_thickness(qpl::f32 thickness) {
		this->smooth_layout.set_outline_thickness(thickness);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_background_outline_color(qpl::rgb color) {
		this->smooth_layout.set_outline_color(color);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_background_slope_dimension(qpl::vector2f dimension) {
		this->smooth_layout.set_slope_dimension(dimension);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_background_slope_point_count(qpl::size point_count) {
		this->smooth_layout.set_slope_point_count(point_count);
		this->layout_changed = true;
	}
	void qsf::smooth_button::set_text_font(const sf::Font& font) {
		this->text.set_font(font);
	}
	void qsf::smooth_button::set_text_font(const std::string& font_name) {
		this->text.set_font(font_name);
	}
	void qsf::smooth_button::set_text_style(qpl::u32 style) {
		this->text.set_style(style);
	}
	void qsf::smooth_button::set_text_character_size(qpl::u32 character_size) {
		this->text.set_character_size(character_size);
	}
	void qsf::smooth_button::set_text_color(qpl::rgb color) {
		this->text.set_color(color);
	}
	void qsf::smooth_button::set_text_outline_thickness(qpl::f32 outline_thickness) {
		this->text.set_outline_thickness(outline_thickness);
	}
	void qsf::smooth_button::set_text_outline_color(qpl::rgb color) {
		this->text.set_outline_color(color);
	}
	void qsf::smooth_button::set_text_rotation(qpl::f32 angle) {
		this->text.set_rotation(angle);
	}
	void qsf::smooth_button::set_text_letter_spacing(qpl::f32 spacing) {
		this->text.set_letter_spacing(spacing);
	}
	void qsf::smooth_button::set_text_position(qpl::vector2f position) {
		this->text.set_position(position);
	}
	void qsf::smooth_button::set_text_center(qpl::vector2f position) {
		this->text.set_center(position);
	}
	void qsf::smooth_button::set_text_string(const std::string& string) {
		this->text.set_string(string);
	}
	void qsf::smooth_button::set_text_string(const std::wstring& string) {
		this->text.set_string(string);
	}
	void qsf::smooth_button::set_text_multiplied_color(qpl::rgb color) {
		this->text.set_multiplied_color(color);
	}
	void qsf::smooth_button::centerize_text() {
		this->text.set_center(this->smooth_layout.get_center());
	}

	qpl::vector2f qsf::smooth_button::get_hitbox_increase() const {
		return this->hitbox_increase;
	}
	qpl::rgb qsf::smooth_button::get_multiplied_color() const {
		return this->multiplied_color;
	}
	qpl::vector2f qsf::smooth_button::get_dimension() const {
		return this->smooth_layout.get_dimension();
	}
	qpl::vector2f qsf::smooth_button::get_position() const {
		return this->smooth_layout.get_position();
	}
	qpl::vector2f qsf::smooth_button::get_center() const {
		return this->smooth_layout.get_center();
	}
	qpl::f64 qsf::smooth_button::get_slope() const {
		return this->smooth_layout.get_slope();
	}
	qpl::rgb qsf::smooth_button::get_background_color() const {
		return this->smooth_layout.get_color();
	}
	qpl::f32 qsf::smooth_button::get_background_outline_thickness() const {
		return this->smooth_layout.get_outline_thickness();
	}
	qpl::rgb qsf::smooth_button::get_background_outline_color() const {
		return this->smooth_layout.get_outline_color();
	}
	qpl::vector2f qsf::smooth_button::get_slope_dimension() const {
		return this->smooth_layout.get_slope_dimension();
	}
	qpl::size qsf::smooth_button::get_slope_point_count() const {
		return this->smooth_layout.get_slope_point_count();
	}
	std::string qsf::smooth_button::get_text_font() const {
		return this->text.get_font();
	}
	qpl::u32 qsf::smooth_button::get_text_style() const {
		return this->text.get_style();
	}
	qpl::u32 qsf::smooth_button::get_text_character_size() const {
		return this->text.get_character_size();
	}
	qpl::rgb qsf::smooth_button::get_text_color() const {
		return this->text.get_color();
	}
	qpl::f32 qsf::smooth_button::get_text_outline_thickness() const {
		return this->text.get_outline_thickness();
	}
	qpl::rgb qsf::smooth_button::get_text_outline_color() const {
		return this->text.get_outline_color();
	}
	qpl::f32 qsf::smooth_button::get_text_letter_spacing() const {
		return this->text.get_letter_spacing();
	}
	qpl::vector2f qsf::smooth_button::get_text_position() const {
		return this->text.get_visible_hitbox().position;
	}
	qpl::vector2f qsf::smooth_button::get_text_center() const {
		return this->text.get_visible_hitbox().get_center();
	}
	std::string qsf::smooth_button::get_text_string() const {
		return this->text.get_string();
	}
	std::wstring qsf::smooth_button::get_text_wstring() const {
		return this->text.get_wstring();
	}
	bool qsf::smooth_button::is_hovering() const {
		return this->hovering;
	}
	bool qsf::smooth_button::is_clicked() const {
		return this->clicked;
	}
	void qsf::smooth_button::update(const qsf::event_info& event) {
		this->create_check();
		this->hovering = this->rectangle.contains(event.mouse_position());
		this->clicked = this->hovering && event.left_mouse_clicked();
	}
	void qsf::smooth_button::create_check() const {
		if (this->layout_changed) {
			this->rectangle = this->smooth_layout;
			this->layout_changed = false;
		}
	}
	void qsf::smooth_button::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		this->create_check();
		this->rectangle.draw(window, states);
		this->text.draw(window, states);
	}

	void qsf::border_graphic::set_dimension(qpl::vector2f dimension) {
		this->dimension = dimension;
	}
	void qsf::border_graphic::set_position(qpl::vector2f position) {
		this->position = position;
	}
	void qsf::border_graphic::increase(qpl::vector2f delta) {
		this->position -= delta;
		this->dimension += delta * 2;
	}
	void qsf::border_graphic::set_color(qpl::rgb color) {
		this->color = color;
	}
	void qsf::border_graphic::set_scale(qpl::vector2f scale) {
		this->scale = scale;
	}
	void qsf::border_graphic::set_scale(qpl::f32 scale) {
		this->scale = qpl::vec(scale, scale);
	}
	void qsf::border_graphic::set_texture(const sf::Texture& texture) {
		this->texture = &texture;
		this->texture_dimension = texture.getSize();
	}
	void qsf::border_graphic::check_texture() {
		if (!this->texture) {
			qpl::println("qsf::border_graphic : texture was not set.");
			qpl::system_pause();
		}
	}
	void qsf::border_graphic::update_dimensions(qpl::vector2f position, qpl::vector2f dimension) {
		this->dimension = dimension;
		this->position = position;
		this->sprites.clear();
	}
	void qsf::border_graphic::move(qpl::vector2f delta) {
		this->position += delta;
		for (auto& i : this->sprites) {
			i.move(delta);
		}
	}
	void qsf::border_graphic::clear() {
		this->sprites.clear();
		this->position = qpl::vec(0, 0);
	}
	void qsf::border_graphic::add_top() {
		this->check_texture();

		auto x_size = qpl::u32_cast(this->dimension.x / ((this->texture_dimension.x - 1) * this->scale.x) + 1);

		auto ctr = this->sprites.size();
		this->sprites.resize(ctr + x_size);



		auto pos = this->position;
		for (qpl::u32 i = 0u; i < x_size; ++i) {
			this->sprites[ctr].set_texture(*this->texture);
			this->sprites[ctr].set_rotation(270.f);
			this->sprites[ctr].set_position(pos + qpl::vector2f(0, this->texture_dimension.x * (this->scale.x)));
			this->sprites[ctr].set_scale(this->scale);
			this->sprites[ctr].set_color(this->color);

			if (i == x_size - 1) {
				auto leftover = std::fmod(this->dimension.x, this->texture_dimension.x * this->scale.y) / this->scale.y;
				qpl::hitbox hitbox;
				hitbox.dimension = { this->texture_dimension.x, leftover };
				this->sprites[ctr].set_texture_rect(hitbox);
			}

			pos.x += this->texture_dimension.x * this->scale.y;
			++ctr;
		}
	}
	void qsf::border_graphic::add_bottom() {
		this->check_texture();

		auto x_size = qpl::u32_cast(this->dimension.x / ((this->texture_dimension.x - 1) * this->scale.x) + 1);

		auto ctr = this->sprites.size();
		this->sprites.resize(ctr + x_size);

		auto pos = this->position + qpl::vector2f(0, this->dimension.y - this->texture_dimension.x);
		for (qpl::u32 i = 0u; i < x_size; ++i) {
			this->sprites[ctr].set_texture(*this->texture);
			this->sprites[ctr].set_rotation(90.f);
			this->sprites[ctr].set_position(pos + qpl::vector2f(this->texture_dimension.x * this->scale.y, this->texture_dimension.x * (1 - this->scale.x)));
			this->sprites[ctr].set_scale(this->scale);
			this->sprites[ctr].set_color(this->color);

			if (i == x_size - 1) {
				auto leftover = std::fmod(this->dimension.x, this->texture_dimension.x * this->scale.y);
				qpl::hitbox hitbox;
				hitbox.dimension = { this->texture_dimension.x, leftover / this->scale.y };
			
				this->sprites[ctr].move(qpl::vec(-(this->texture_dimension.x * this->scale.y - leftover), 0));
				this->sprites[ctr].set_texture_rect(hitbox);
			}

			pos.x += this->texture_dimension.x * this->scale.y;
			++ctr;
		}
	}
	void qsf::border_graphic::add_left() {
		this->check_texture();

		auto y_size = qpl::u32_cast(this->dimension.y / ((this->texture_dimension.x - 1) * this->scale.y) + 1);

		auto ctr = this->sprites.size();
		this->sprites.resize(ctr + y_size);

		auto pos = this->position;
		for (qpl::u32 i = 0u; i < y_size; ++i) {
			this->sprites[ctr].set_texture(*this->texture);
			this->sprites[ctr].set_rotation(180.f);
			this->sprites[ctr].set_position(pos + qpl::vector2f(this->texture_dimension.x * this->scale.x, this->texture_dimension.x * this->scale.y));
			this->sprites[ctr].set_scale(this->scale);
			this->sprites[ctr].set_color(this->color);

			if (i == y_size - 1) {
				auto leftover = std::fmod(this->dimension.y, this->texture_dimension.y * this->scale.x);
				qpl::hitbox hitbox;
				hitbox.dimension = { this->texture_dimension.x, leftover / this->scale.x };
				this->sprites[ctr].move(qpl::vec(0, -(this->texture_dimension.y * this->scale.x - leftover)));
				this->sprites[ctr].set_texture_rect(hitbox);
			}

			pos.y += this->texture_dimension.x * this->scale.x;
			++ctr;
		}
	}
	void qsf::border_graphic::add_right() {
		this->check_texture();

		auto y_size = qpl::u32_cast(this->dimension.y / ((this->texture_dimension.x - 1) * this->scale.y) + 1);

		auto ctr = this->sprites.size();
		this->sprites.resize(ctr + y_size);

		auto pos = this->position + qpl::vector2f(this->dimension.x - this->texture_dimension.x, 0);
		for (qpl::u32 i = 0u; i < y_size; ++i) {
			this->sprites[ctr].set_texture(*this->texture);
			this->sprites[ctr].set_position(pos + qpl::vector2f(this->texture_dimension.x * (1 - this->scale.y), 0));
			this->sprites[ctr].set_rotation(0.f);
			this->sprites[ctr].set_scale(this->scale);
			this->sprites[ctr].set_color(this->color);


			if (i == y_size - 1) {
				auto leftover = std::fmod(this->dimension.y, this->texture_dimension.y * this->scale.x);
				qpl::hitbox hitbox;
				hitbox.dimension = { this->texture_dimension.x, leftover / this->scale.x };
				this->sprites[ctr].set_texture_rect(hitbox);
			}

			pos.y += this->texture_dimension.x * this->scale.x;
			++ctr;
		}
	}
	void qsf::border_graphic::add_all_sides() {
		this->add_top();
		this->add_left();
		this->add_right();
		this->add_bottom();
	}
	void qsf::border_graphic::draw(qsf::draw_object& object) const {
		object.draw(this->sprites);
	}

	std::unordered_map<std::string, qsf::text> qsf::detail::texts;

	qsf::text& qsf::get_text(const std::string& name) {
		if (qsf::detail::texts.find(name) == qsf::detail::texts.cend()) {
			qsf::add_text();
		}
		return qsf::detail::texts[name];
	}
	void qsf::add_text(const std::string& name) {
		if (qsf::detail::texts.find(name) == qsf::detail::texts.cend()) {
			if (qsf::detail::texts.size()) {
				qsf::detail::texts[name] = qsf::detail::texts.begin()->second;
			}
			else if (qsf::detail::resources.fonts.size()) {
				qsf::detail::texts[name].set_font(qsf::detail::resources.fonts.begin()->first);
			}
		}
	}
}
#endif