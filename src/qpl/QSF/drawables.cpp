#include <qpl/QSF/drawables.hpp>

#if defined QPL_INTERN_SFML_USE

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
		qsf::smooth_corner qsf::detail::smooth_corner;
		qsf::point qsf::detail::point;
		qsf::points qsf::detail::points;
		qsf::circle qsf::detail::circle;
		qsf::circles qsf::detail::circles;
		qsf::line qsf::detail::line;
		qsf::lines qsf::detail::lines;
		qsf::thick_line qsf::detail::thick_line;
		qsf::thick_lines qsf::detail::thick_lines;
		qsf::graph qsf::detail::graph;
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
	void qsf::vertex_array::set_color(qpl::rgba color) {
		for (auto& i : this->vertices) {
			i.color = color;
		}
	}
	void qsf::vertex_array::move(qpl::vector2f delta) {
		for (auto& i : this->vertices) {
			i.position += delta;
		}
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
	void qsf::vtext::set_color(qpl::rgba color) {
		this->color = color;
	}
	void qsf::vtext::set_outline_thickness(qpl::f32 outline_thickness) {
		this->outline_thickness = outline_thickness;
	}
	void qsf::vtext::set_outline_color(qpl::rgba color) {
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
			this->string == other.string;
	}
	void qsf::vtext::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::text = *this;
		qsf::detail::text.draw(window, states);
	}

	const sf::Font& qsf::text::get_sf_font() const {
		return *this->m_text.getFont();
	}
	const sf::Glyph& qsf::text::get_glyph(qpl::wchar_type c) const {
		if (this->m_font.empty()) {
			throw qpl::exception("qsf::text::get_glyph : font not set");
		}
		return this->get_sf_font().getGlyph(c, this->get_character_size(), this->is_bold(), this->get_outline_thickness());
	}
	qpl::hitbox qsf::text::get_glyph_hitbox(qpl::wchar_type c) const {
		auto rect = this->get_glyph(c).textureRect;
		return qpl::hitbox({ rect.left, rect.top }, { rect.width, rect.height });
	}

	qpl::f32 qsf::text::get_character_advance(qpl::wchar_type current) const {
		return this->get_glyph(current).advance + this->get_letter_spacing_pixels();
	}
	qpl::f32 qsf::text::get_next_character_advance() const {


		auto x = this->get_standard_hitbox().dimension.x;
		auto w = this->get_wstring();
		if (w.length()) {
			auto cw = w[w.length() - 1];
			auto special = (cw == L'\t' || cw == L'\n');
			if (!special) {

				auto width = this->get_letter_width(cw);
				auto advance = this->get_letter_advance(cw);
				auto spaced_advance = this->get_letter_advance_and_spacing(cw);

				qpl::f32 diff = 0;
				if (!std::iswspace(cw)) {
					diff = spaced_advance - width;
				}

				if (w.length() > 1) {
					auto before_left = this->get_glyph(w[w.length() - 2]).bounds.left;
					if (std::iswspace(cw)) {
						before_left -= this->get_glyph(cw).bounds.left;
					}
					diff += before_left;
				}

				x += diff;
			}
		}
		return x;
	}
	qpl::f32 qsf::text::get_line_spacing_pixels() const {
		return this->get_sf_font().getLineSpacing(this->get_character_size()) * this->m_text.getLineSpacing();
	}
	qpl::f32 qsf::text::get_line_spacing() const {
		return this->get_line_height();
		//return this->m_text.getLineSpacing();
	}
	qpl::f32 qsf::text::get_letter_spacing_pixels() const {
		float white_width = this->get_sf_font().getGlyph(L' ', this->get_character_size(), this->is_bold(), this->get_outline_thickness()).advance;
		float letter_spacing = (white_width / 3.f) * (this->get_letter_spacing() - 1.f);
		return letter_spacing;
	}
	qpl::f32 qsf::text::get_whitespace_width() const {
		float white_width = this->get_sf_font().getGlyph(L' ', this->get_character_size(), this->is_bold(), this->get_outline_thickness()).advance;
		float letter_spacing = (white_width / 3.f) * (this->get_letter_spacing() - 1.f);
		return white_width + letter_spacing;
	}
	qpl::f32 qsf::text::get_italic_shear() const {
		return (this->get_style() == sf::Text::Style::Italic) ? (12.0f * (qpl::pi_32 / 180.0f)) : 0.f;
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
	qpl::rgba qsf::text::get_color() const {
		return this->m_text.getFillColor();
	}
	qpl::f32 qsf::text::get_outline_thickness() const {
		return this->m_text.getOutlineThickness();
	}
	qpl::rgba qsf::text::get_outline_color() const {
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
		return this->m_text.getString().toAnsiString();
	}
	std::wstring qsf::text::get_wstring() const {
		return this->m_text.getString().toWideString();
	}
	sf::String qsf::text::get_sfstring() const {
		return this->m_text.getString();
	}
	void qsf::text::set_font(const sf::Font& font) {
		this->m_font = font.getInfo().family;
		this->m_text.setFont(font);
	}
	void qsf::text::set_font(const std::string& font_name) {
		this->m_font = font_name;
		if (this->m_font.empty()) return;
		if (qsf::find_font(font_name)) {
			this->m_text.setFont(qsf::get_font(font_name));
		}
		else {
			throw qpl::exception("couldn't find font \"", font_name, "\"");
		}
	}
	void qsf::text::set_style(qpl::u32 style) {
		this->m_text.setStyle(style);
	}
	void qsf::text::set_character_size(qpl::u32 character_size) {
		this->m_text.setCharacterSize(character_size);
	}
	void qsf::text::set_color(qpl::rgba color) {
		this->m_text.setFillColor(color);
	}
	void qsf::text::set_outline_thickness(qpl::f32 outline_thickness) {
		this->m_text.setOutlineThickness(outline_thickness);
	}
	void qsf::text::set_outline_color(qpl::rgba color) {
		this->m_text.setOutlineColor(color);
	}
	void qsf::text::set_rotation(qpl::f32 angle) {
		this->m_text.setRotation(angle);
	}
	void qsf::text::set_letter_spacing(qpl::f32 spacing) {
		this->m_text.setLetterSpacing(spacing);
	}
	void qsf::text::set_line_spacing(qpl::f32 spacing) {
		this->m_text.setLineSpacing(spacing);
	}
	void qsf::text::set_position(qpl::vector2f position) {
		this->m_text.setPosition(position);
	}
	void qsf::text::set_center(qpl::vector2f position) {
		this->centerize();
		this->set_position(position);
	}
	void qsf::text::set_string(const std::string& string) {
		this->m_text.setString(string);
	}
	void qsf::text::set_string(const std::wstring& string) {
		this->m_text.setString(string);
	}
	void qsf::text::set_string(const sf::String& string) {
		this->m_text.setString(string);
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
	qpl::vector2f qsf::text::find_character_position(qpl::size index) const {
		return this->m_text.findCharacterPos(index);
	}
	qpl::f32 qsf::text::get_underline_baseline() const {
		return this->m_text.getFont()->getUnderlinePosition(this->get_character_size());
	}
	qpl::f32 qsf::text::get_underline_thickness() const {
		return this->m_text.getFont()->getUnderlineThickness(this->get_character_size());
	}
	qpl::f32 qsf::text::get_character_size_plus_baseline() const {
		return this->get_character_size() + this->get_underline_baseline();
	}
	std::vector<std::pair<qpl::size, qpl::hitbox>> qsf::text::get_all_characters_hitbox() const {
		std::vector<std::pair<qpl::size, qpl::hitbox>> result;
		auto x = this->find_character_position(0).x;
		auto y = qpl::f32_cast(this->get_character_size());
		auto string = this->get_wstring();

		wchar_t prev_char = L'\0';
		for (qpl::size i = 0u; i < string.size(); ++i) {
			qpl::vector2f pos = this->find_character_position(i);
			sf::Glyph glyph;

			auto c = string[i];
			if (c == L'\t' || c == L'\n') {
				glyph = this->get_glyph(L' ');
			}
			else {
				glyph = this->get_glyph(string[i]);
			}

			x += this->get_sf_font().getKerning(prev_char, c, this->get_character_size());
			prev_char = c;

			qpl::f32 extra_width = 0.0f;
			qpl::f32 extra_height = 0.0f;
			qpl::vector2f shift = qpl::vec(0, 0);
			bool special = false;
			switch (c) {
			case L'\n':
				x = this->find_character_position(0).x;
				continue;
			case L'\t':
				extra_width = this->get_whitespace_width() * 4;
				x += extra_width;
				extra_height = this->get_character_size() / 2.0f;
				shift = -qpl::vec(extra_width, extra_height);
				special = true;
				continue;
			case L' ':
				extra_width = this->get_whitespace_width();
				x += extra_width;
				extra_height = this->get_character_size() / 2.0f;
				shift = -qpl::vec(extra_width, extra_height);
				special = true;
				continue;
			}

			auto left = glyph.bounds.left;
			auto top = glyph.bounds.top;
			auto right = glyph.bounds.left + glyph.bounds.width;
			auto bottom = glyph.bounds.top + glyph.bounds.height;

			left = x + left - this->get_italic_shear() * bottom;
			right = x + right - this->get_italic_shear() * top;
			top = this->get_position().y + y + top;
			bottom = this->get_position().y + y + bottom;


			qpl::hitbox hitbox;
			hitbox.position = qpl::vec(left, top + pos.y) + shift;
			hitbox.dimension = qpl::vec(right - left, bottom - top) + qpl::vec(extra_width, extra_height);

			result.push_back(std::make_pair(i, hitbox));

			auto base_line = this->get_underline_baseline();
			auto base_thickness = this->get_underline_thickness();

			pos.y += this->get_character_size() + base_line + base_thickness;
			pos.x = x;
			if (!special) {
				x += this->get_character_advance(c);
			}
		}
		return result;
	}

	std::vector<qpl::hitbox> qsf::text::get_all_characters_hitbox_whitespace_included() const {
		std::vector<qpl::hitbox> result;
		auto x = this->find_character_position(0).x;
		auto y = qpl::f32_cast(this->get_character_size());
		auto string = this->get_wstring();

		wchar_t prev_char = L'\0';
		for (qpl::size i = 0u; i < string.size(); ++i) {
			qpl::vector2f pos = this->find_character_position(i);
			sf::Glyph glyph;

			auto c = string[i];
			if (c == L'\t' || c == L'\n') {
				glyph = this->get_glyph(L' ');
			}
			else {
				glyph = this->get_glyph(string[i]);
			}

			x += this->get_sf_font().getKerning(prev_char, c, this->get_character_size());
			prev_char = c;

			qpl::f32 extra_width = 0.0f;
			qpl::f32 extra_height = 0.0f;
			qpl::vector2f shift = qpl::vec(0, 0);
			bool special = false;
			switch (c) {
			case L'\n':
				x = this->find_character_position(0).x;
				continue;
			case L'\t':
				extra_width = this->get_whitespace_width() * 4;
				x += extra_width;
				extra_height = this->get_character_size() / 2.0f;
				shift = -qpl::vec(extra_width, extra_height);
				special = true;
				break;
			case L' ':
				extra_width = this->get_whitespace_width();
				x += extra_width;
				extra_height = this->get_character_size() / 2.0f;
				shift = -qpl::vec(extra_width, extra_height);
				special = true;
				break;
			}

			auto left = glyph.bounds.left;
			auto top = glyph.bounds.top;
			auto right = glyph.bounds.left + glyph.bounds.width;
			auto bottom = glyph.bounds.top + glyph.bounds.height;

			left = x + left - this->get_italic_shear() * bottom;
			right = x + right - this->get_italic_shear() * top;
			top = this->get_position().y + y + top;
			bottom = this->get_position().y + y + bottom;


			qpl::hitbox hitbox;
			hitbox.position = qpl::vec(left, top + pos.y) + shift;
			hitbox.dimension = qpl::vec(right - left, bottom - top) + qpl::vec(extra_width, extra_height);

			result.push_back(hitbox);

			auto base_line = this->get_underline_baseline();
			auto base_thickness = this->get_underline_thickness();

			pos.y += this->get_character_size() + base_line + base_thickness;
			pos.x = x;
			if (!special) {
				x += this->get_character_advance(c);
			}
		}
		return result;
	}
	qpl::vector2f qsf::text::get_starting_line_position() const {

		auto glyph = this->get_glyph(L'A');

		auto x = this->find_character_position(0).x;
		auto y = this->get_character_size();

		auto left = glyph.bounds.left;
		auto top = glyph.bounds.top;
		auto right = glyph.bounds.left + glyph.bounds.width;
		auto bottom = glyph.bounds.top + glyph.bounds.height;

		left = x + left - this->get_italic_shear() * bottom;
		right = x + right - this->get_italic_shear() * top;
		top = this->get_position().y + y + top;
		return qpl::vec(left, top);
	}
	qpl::f32 qsf::text::get_line_height() const {

		auto glyph = this->get_glyph(L'A');

		auto y = this->get_character_size();
		auto top = glyph.bounds.top;
		auto a_top = this->get_position().y + y + top;

		glyph = this->get_glyph(L'g');

		y = this->get_character_size();
		auto bottom = glyph.bounds.top + glyph.bounds.height;
		auto g_bottom = this->get_position().y + y + bottom;

		return (g_bottom - a_top);
	}
	qpl::f32 qsf::text::get_delta_underline() const {
		auto glyph = this->get_glyph(L'x');

		auto y = this->get_character_size();
		auto bottom = glyph.bounds.top + glyph.bounds.height;
		auto x_bottom = this->get_position().y + y + bottom;

		glyph = this->get_glyph(L'g');

		y = this->get_character_size();
		bottom = glyph.bounds.top + glyph.bounds.height;
		auto g_bottom = this->get_position().y + y + bottom;

		return g_bottom - x_bottom;
	}
	qpl::vector2f qsf::text::get_character_dimension(wchar_t character) const {

		auto glyph = this->get_glyph(character);

		auto left = glyph.bounds.left;
		auto top = glyph.bounds.top;
		auto right = glyph.bounds.left + glyph.bounds.width;
		auto bottom = glyph.bounds.top + glyph.bounds.height;

		left = left - this->get_italic_shear() * bottom;
		right = right - this->get_italic_shear() * top;
		top = this->get_position().y + top;
		bottom = this->get_position().y + bottom;

		return qpl::vec(right - left, bottom - top);
	}
	qpl::size qsf::text::get_line_number(qpl::size index) const {
		qpl::size line = 0u;
		auto str = this->get_wstring();
		for (qpl::size i = 0u; i <= index && i < str.size(); ++i) {
			if (str[i] == L'\n') {
				++line;
			}
		}
		return line;
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

	qpl::size qsf::text::size() const {
		return this->m_text.getString().getSize();
	}
	std::string qsf::text::string() const {
		return this->m_text.getString().toAnsiString();
	}

	void qsf::text::clear() {
		sf::String s = L"";
		this->m_text.setString(s);
	}
	qsf::text& qsf::text::operator<<(const std::string& string) {
		sf::String s = this->get_sfstring();
		s += string;
		this->m_text.setString(s);
		return *this;
	}
	qsf::text& qsf::text::operator<<(const std::wstring& string) {
		sf::String s = this->get_sfstring();
		s += string;
		this->m_text.setString(s);
		return *this;
	}
	qsf::text& qsf::text::operator<<(const sf::String& string) {
		sf::String s = this->get_sfstring();
		s += string;
		this->m_text.setString(s);
		return *this;
	}
	qsf::text& qsf::text::operator=(const qsf::vtext& text) {
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
		return *this;
	}

	void qsf::text::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		window.draw(this->m_text, states);
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
	void qsf::vrectangle::set_color(qpl::rgba color) {
		this->color = color;
	}
	void qsf::vrectangle::set_outline_thickness(qpl::f32 outline_thickness) {
		this->outline_thickness = outline_thickness;
	}
	void qsf::vrectangle::set_outline_color(qpl::rgba outline_color) {
		this->outline_color = outline_color;
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
	qpl::rgba qsf::vrectangle::get_color() const {
		return this->color;
	}
	qpl::f32 qsf::vrectangle::get_outline_thickness() const {
		return this->outline_thickness;
	}
	qpl::rgba qsf::vrectangle::get_outline_color() const {
		return this->color;
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
			position.y >(this->position.y - hitbox_increase) && position.y < (this->position.y + this->dimension.y + hitbox_increase));
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
	void qsf::rectangle::set_color(qpl::rgba color) {
		this->m_rect.setFillColor(color);
	}
	void qsf::rectangle::set_outline_color(qpl::rgba color) {
		this->m_rect.setOutlineColor(color);
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
	void qsf::va_rectangle::set_color(qpl::rgba color) {
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

	qpl::rgba qsf::rectangle::get_color() const {
		return this->m_rect.getFillColor();
	}
	qpl::rgba qsf::rectangle::get_outline_color() const {
		return this->m_rect.getOutlineColor();
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
		this->set_color(rectangle.color);
		this->set_position(rectangle.position);
		this->set_dimension(rectangle.dimension);
		this->set_outline_color(rectangle.outline_color);
		this->set_outline_thickness(rectangle.outline_thickness);
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
	void qsf::vrectangles::add_rectangle(qpl::vector2f position, qpl::vector2f dimension, qpl::rgba color) {
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
	void qsf::polygon::set_outline_color(qpl::rgba color) {
		this->shape.setOutlineColor(color);
	}
	void qsf::polygon::set_color(qpl::rgba color) {
		this->shape.setFillColor(color);
	}
	void qsf::polygon::set_position(qpl::vector2f position) {
		this->shape.setPosition(position);
	}

	qpl::f32 qsf::polygon::get_outline_thickness() const {
		return this->shape.getOutlineThickness();
	}
	qpl::rgba qsf::polygon::get_outline_color() const {
		return this->shape.getOutlineColor();
	}
	qpl::rgba qsf::polygon::get_color() const {
		return this->shape.getFillColor();
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
	void qsf::vsmooth_rectangle::set_color(qpl::rgba color) {
		this->color = color;
	}
	void qsf::vsmooth_rectangle::set_outline_color(qpl::rgba color) {
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
	void qsf::vsmooth_rectangle::set_round_corners(qpl::bitset<4u> round_corners) {
		this->round_corners = round_corners;
	}
	qpl::vector2f qsf::vsmooth_rectangle::get_dimension() const {
		return this->dimension;
	}
	qpl::vector2f qsf::vsmooth_rectangle::get_position() const {
		return this->position;
	}
	qpl::hitbox qsf::vsmooth_rectangle::get_hitbox() const {
		return qpl::hitbox(this->get_position(), this->get_dimension());
	}
	qpl::vector2f qsf::vsmooth_rectangle::get_center() const {
		return this->position + this->dimension / 2;
	}
	qpl::f64 qsf::vsmooth_rectangle::get_slope() const {
		return this->slope;
	}
	qpl::rgba qsf::vsmooth_rectangle::get_color() const {
		return this->color;
	}
	qpl::rgba qsf::vsmooth_rectangle::get_outline_color() const {
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
	qpl::bitset<4u> qsf::vsmooth_rectangle::get_round_corners() const {
		return this->round_corners;
	}

	bool qsf::vsmooth_rectangle::contains(qpl::vector2f point) const {
		qsf::detail::smooth_rectangle = *this;
		return qsf::detail::smooth_rectangle.contains(point);
	}
	void qsf::vsmooth_rectangle::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::smooth_rectangle = *this;
		qsf::detail::smooth_rectangle.draw(window, states);
	}

	void qsf::smooth_rectangle::set_position(qpl::vector2f position) {
		this->position = position;
		this->internal_check = true;
	}
	void qsf::smooth_rectangle::set_dimension(qpl::vector2f dimension) {
		this->dimension = dimension;
		this->internal_check = true;
	}
	void qsf::smooth_rectangle::set_hitbox(qpl::hitbox hitbox) {
		this->position = hitbox.position;
		this->dimension = hitbox.dimension;
		this->internal_check = true;
	}
	void qsf::smooth_rectangle::set_center(qpl::vector2f position) {
		this->position = position - this->dimension / 2;
		this->internal_check = true;
	}
	void qsf::smooth_rectangle::set_slope(qpl::f64 slope) {
		this->slope = slope;
		this->internal_check = true;
	}
	void qsf::smooth_rectangle::set_color(qpl::rgba color) {
		this->polygon.set_color(color);
		this->internal_check = true;
	}
	void qsf::smooth_rectangle::set_outline_color(qpl::rgba color) {
		this->polygon.set_outline_color(color);
		this->internal_check = true;
	}
	void qsf::smooth_rectangle::set_outline_thickness(qpl::f32 thickness) {
		this->outline_thickness = thickness;
		this->internal_check = true;
	}
	void qsf::smooth_rectangle::set_slope_dimension(qpl::vector2f dimension) {
		this->slope_dim = dimension;
		this->internal_check = true;
	}
	void qsf::smooth_rectangle::set_slope_dimension(qpl::f32 dimension) {
		this->set_slope_dimension(qpl::vec(dimension, dimension));
	}
	void qsf::smooth_rectangle::set_slope_point_count(qpl::size point_count) {
		this->slope_point_count = point_count;
		this->internal_check = true;
	}	
	void qsf::smooth_rectangle::set_round_corners(qpl::bitset<4u> round_corners) {
		this->round_corners = round_corners;
	}
	qpl::vector2f qsf::smooth_rectangle::get_dimension() const {
		return this->dimension;
	}
	qpl::vector2f qsf::smooth_rectangle::get_position() const {
		return this->position;
	}
	qpl::hitbox qsf::smooth_rectangle::get_hitbox() const {
		return qpl::hitbox(this->position, this->dimension);
	}
	qpl::vector2f qsf::smooth_rectangle::get_center() const {
		return this->position + this->dimension / 2;
	}
	qpl::f64 qsf::smooth_rectangle::get_slope() const {
		return this->slope;
	}
	qpl::rgba qsf::smooth_rectangle::get_color() const {
		return this->polygon.get_color();
	}
	qpl::rgba qsf::smooth_rectangle::get_outline_color() const {
		return this->polygon.get_outline_color();
	}
	qpl::f32 qsf::smooth_rectangle::get_outline_thickness() const {
		return this->outline_thickness;
	}
	qpl::vector2f qsf::smooth_rectangle::get_slope_dimension() const {
		return this->slope_dim;
	}
	qpl::size qsf::smooth_rectangle::get_slope_point_count() const {
		return this->slope_point_count;
	}
	qpl::bitset<4u> qsf::smooth_rectangle::get_round_corners() const {
		return this->round_corners;
	}

	void qsf::smooth_rectangle::move(qpl::vector2f delta) {
		this->check_create();
		this->position.move(delta);
		this->internal_check = true;
	}
	bool qsf::smooth_rectangle::contains(qpl::vector2f point) const {
		this->check_create();
		return this->polygon.contains(point);
	}
	void qsf::smooth_rectangle::check_create() const {
		if (this->internal_check) {
			qsf::vsmooth_rectangle rect;
			rect.set_dimension(this->dimension);
			rect.set_position(this->position);
			rect.set_slope(this->slope);
			rect.set_slope_point_count(this->slope_point_count);
			rect.set_color(this->get_color());
			rect.set_outline_color(this->get_outline_color());
			rect.set_slope_dimension(this->slope_dim);
			rect.set_outline_thickness(this->outline_thickness);
			rect.set_round_corners(this->get_round_corners());

			*this = rect;
		}
	}
	qsf::smooth_rectangle& qsf::smooth_rectangle::operator=(const qsf::vsmooth_rectangle& smooth_rectangle) {
		this->position = smooth_rectangle.get_position();
		this->dimension = smooth_rectangle.get_dimension();

		this->polygon.set_color(smooth_rectangle.color);
		this->polygon.set_outline_color(smooth_rectangle.outline_color);
		this->polygon.set_outline_thickness(smooth_rectangle.outline_thickness);
		this->polygon.shape.setFillColor(smooth_rectangle.color);
		this->polygon.shape.setOutlineColor(smooth_rectangle.outline_color);

		auto active_corners = smooth_rectangle.round_corners.number_of_set_bits();
		auto size = smooth_rectangle.slope_point_count;
		this->polygon.resize(active_corners * size + (4 - active_corners));

		auto position = smooth_rectangle.position;
		auto dimension = smooth_rectangle.slope_dim;
		auto rect_dimension = smooth_rectangle.dimension;
		auto slope_exponent = std::pow(smooth_rectangle.slope, 1.0);

		qpl::u32 ctr = 0u;

		if (smooth_rectangle.round_corners[0u]) {
			for (qpl::u32 i = 0u; i < size; ++i) {
				auto progress = std::pow(qpl::f64_cast(i) / (size - 1), slope_exponent);
				auto value = qpl::smooth_curve(progress, smooth_rectangle.slope);
				auto curve_pos = qpl::vec(progress, -value) * dimension;
				auto pos = position + qpl::vec(0, dimension.y) + curve_pos;
				this->polygon.set_point(ctr, pos);
				++ctr;
			}
		}
		else {
			auto pos = position;
			this->polygon.set_point(ctr, pos);
			++ctr;
		}
		if (smooth_rectangle.round_corners[1u]) {
			for (qpl::u32 i = 0u; i < size; ++i) {
				auto progress = std::pow(1 - qpl::f64_cast(i) / (size - 1), slope_exponent);
				auto value = qpl::smooth_curve(progress, smooth_rectangle.slope);
				auto curve_pos = qpl::vec(progress, value) * dimension;
				auto pos = position + qpl::vec(rect_dimension.x, dimension.y) - curve_pos;
				this->polygon.set_point(ctr, pos);
				++ctr;
			}
		}
		else {
			auto pos = position + rect_dimension.just_x();
			this->polygon.set_point(ctr, pos);
			++ctr;
		}
		if (smooth_rectangle.round_corners[2u]) {
			for (qpl::u32 i = 0u; i < size; ++i) {
				auto progress = std::pow(qpl::f64_cast(i) / (size - 1), slope_exponent);
				auto value = qpl::smooth_curve(progress, smooth_rectangle.slope);
				auto curve_pos = qpl::vec(-progress, value) * dimension;
				auto pos = position + qpl::vec(rect_dimension.x, rect_dimension.y - dimension.y) + curve_pos;
				this->polygon.set_point(ctr, pos);
				++ctr;
			}
		}
		else {
			auto pos = position + rect_dimension;
			this->polygon.set_point(ctr, pos);
			++ctr;
		}
		if (smooth_rectangle.round_corners[3u]) {
			for (qpl::u32 i = 0u; i < size; ++i) {
				auto progress = std::pow(1 - qpl::f64_cast(i) / (size - 1), slope_exponent);
				auto value = qpl::smooth_curve(progress, smooth_rectangle.slope);
				auto curve_pos = qpl::vec(progress, value) * dimension;
				auto pos = position + qpl::vec(0, rect_dimension.y - dimension.y) + curve_pos;
				this->polygon.set_point(ctr, pos);
				++ctr;
			}
		}
		else {
			auto pos = position + rect_dimension.just_y();
			this->polygon.set_point(ctr, pos);
			++ctr;
		}
		this->internal_check = false;
		return *this;
	}
	const qsf::smooth_rectangle& qsf::smooth_rectangle::operator=(const qsf::vsmooth_rectangle& smooth_rectangle) const {
		this->position = smooth_rectangle.get_position();
		this->dimension = smooth_rectangle.get_dimension();
		
		this->polygon.set_color(smooth_rectangle.color);
		this->polygon.set_outline_color(smooth_rectangle.outline_color);
		this->polygon.set_outline_thickness(smooth_rectangle.outline_thickness);
		this->polygon.shape.setFillColor(smooth_rectangle.color);
		this->polygon.shape.setOutlineColor(smooth_rectangle.outline_color);

		auto active_corners = smooth_rectangle.round_corners.number_of_set_bits();
		auto size = smooth_rectangle.slope_point_count;
		this->polygon.resize(active_corners * size + (4 - active_corners));

		auto position = smooth_rectangle.position;
		auto dimension = smooth_rectangle.slope_dim;
		auto rect_dimension = smooth_rectangle.dimension;
		auto slope_exponent = std::pow(smooth_rectangle.slope, 1.0);

		qpl::u32 ctr = 0u;

		if (smooth_rectangle.round_corners[0u]) {
			for (qpl::u32 i = 0u; i < size; ++i) {
				auto progress = std::pow(qpl::f64_cast(i) / (size - 1), slope_exponent);
				auto value = qpl::smooth_curve(progress, smooth_rectangle.slope);
				auto curve_pos = qpl::vec(progress, -value) * dimension;
				auto pos = position + qpl::vec(0, dimension.y) + curve_pos;
				this->polygon.set_point(ctr, pos);
				++ctr;
			}
		}
		else {
			auto pos = position;
			this->polygon.set_point(ctr, pos);
			++ctr;
		}
		if (smooth_rectangle.round_corners[1u]) {
			for (qpl::u32 i = 0u; i < size; ++i) {
				auto progress = std::pow(1 - qpl::f64_cast(i) / (size - 1), slope_exponent);
				auto value = qpl::smooth_curve(progress, smooth_rectangle.slope);
				auto curve_pos = qpl::vec(progress, value) * dimension;
				auto pos = position + qpl::vec(rect_dimension.x, dimension.y) - curve_pos;
				this->polygon.set_point(ctr, pos);
				++ctr;
			}
		}
		else {
			auto pos = position + rect_dimension.just_x();
			this->polygon.set_point(ctr, pos);
			++ctr;
		}
		if (smooth_rectangle.round_corners[2u]) {
			for (qpl::u32 i = 0u; i < size; ++i) {
				auto progress = std::pow(qpl::f64_cast(i) / (size - 1), slope_exponent);
				auto value = qpl::smooth_curve(progress, smooth_rectangle.slope);
				auto curve_pos = qpl::vec(-progress, value) * dimension;
				auto pos = position + qpl::vec(rect_dimension.x, rect_dimension.y - dimension.y) + curve_pos;
				this->polygon.set_point(ctr, pos);
				++ctr;
			}
		}
		else {
			auto pos = position + rect_dimension;
			this->polygon.set_point(ctr, pos);
			++ctr;
		}
		if (smooth_rectangle.round_corners[3u]) {
			for (qpl::u32 i = 0u; i < size; ++i) {
				auto progress = std::pow(1 - qpl::f64_cast(i) / (size - 1), slope_exponent);
				auto value = qpl::smooth_curve(progress, smooth_rectangle.slope);
				auto curve_pos = qpl::vec(progress, value) * dimension;
				auto pos = position + qpl::vec(0, rect_dimension.y - dimension.y) + curve_pos;
				this->polygon.set_point(ctr, pos);
				++ctr;
			}
		}
		else {
			auto pos = position + rect_dimension.just_y();
			this->polygon.set_point(ctr, pos);
			++ctr;
		}
		this->internal_check = false;
		return *this;
	}
	void qsf::smooth_rectangle::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		this->check_create();
		this->polygon.draw(window, states);
	}


	void qsf::vsmooth_corner::invert() {
		this->slope = 1.0 / this->slope;
	}
	void qsf::vsmooth_corner::move(qpl::vector2f delta) {
		this->position.move(delta);
	}
	void qsf::vsmooth_corner::set_rotation(qpl::f64 rotation) {
		this->rotation = rotation;
	}
	void qsf::vsmooth_corner::set_dimension(qpl::vector2f dimension) {
		this->dimension = dimension;
	}
	void qsf::vsmooth_corner::set_position(qpl::vector2f position) {
		this->position = position;
	}
	void qsf::vsmooth_corner::set_hitbox(qpl::hitbox hitbox) {
		this->position = hitbox.position;
		this->dimension = hitbox.dimension;
	}
	void qsf::vsmooth_corner::set_center(qpl::vector2f position) {
		this->position = position - this->dimension / 2;
	}
	void qsf::vsmooth_corner::set_slope(qpl::f64 slope) {
		this->slope = slope;
	}
	void qsf::vsmooth_corner::set_color(qpl::rgba color) {
		this->color = color;
	}
	void qsf::vsmooth_corner::set_slope_point_count(qpl::size point_count) {
		this->slope_point_count = point_count;
	}

	qpl::f64 qsf::vsmooth_corner::get_rotation() const {
		return this->rotation;
	}
	qpl::vector2f qsf::vsmooth_corner::get_dimension() const {
		return this->dimension;
	}
	qpl::vector2f qsf::vsmooth_corner::get_position() const {
		return this->position;
	}
	qpl::hitbox qsf::vsmooth_corner::get_hitbox() const {
		return qpl::hitbox(this->get_position(), this->get_dimension());
	}
	qpl::vector2f qsf::vsmooth_corner::get_center() const {
		return this->position + this->dimension / 2;
	}
	qpl::f64 qsf::vsmooth_corner::get_slope() const {
		return this->slope;
	}
	qpl::rgba qsf::vsmooth_corner::get_color() const {
		return this->color;
	}
	qpl::size qsf::vsmooth_corner::get_slope_point_count() const {
		return this->slope_point_count;
	}
	void qsf::vsmooth_corner::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::smooth_corner = *this;
		qsf::detail::smooth_corner.draw(window, states);
	}


	void qsf::smooth_corner::invert() {
		this->slope = 1.0 / this->slope;
	}
	void qsf::smooth_corner::set_rotation(qpl::f64 rotation) {
		this->rotation = rotation;
		this->internal_check = true;
	}
	void qsf::smooth_corner::set_position(qpl::vector2f position) {
		this->position = position;
		this->internal_check = true;
	}
	void qsf::smooth_corner::set_dimension(qpl::vector2f dimension) {
		this->dimension = dimension;
		this->internal_check = true;
	}
	void qsf::smooth_corner::set_hitbox(qpl::hitbox hitbox) {
		this->position = hitbox.position;
		this->dimension = hitbox.dimension;
		this->internal_check = true;
	}
	void qsf::smooth_corner::set_center(qpl::vector2f position) {
		this->position = position - this->dimension / 2;
		this->internal_check = true;
	}
	void qsf::smooth_corner::set_slope(qpl::f64 slope) {
		this->slope = slope;
		this->internal_check = true;
	}
	void qsf::smooth_corner::set_color(qpl::rgba color) {
		this->color = color;
		this->internal_check = true;
	}
	void qsf::smooth_corner::set_slope_point_count(qpl::size point_count) {
		this->slope_point_count = point_count;
		this->internal_check = true;
	}
	qpl::f64 qsf::smooth_corner::get_rotation() const {
		return this->rotation;
	}
	qpl::vector2f qsf::smooth_corner::get_dimension() const {
		return this->dimension;
	}
	qpl::vector2f qsf::smooth_corner::get_position() const {
		return this->position;
	}
	qpl::hitbox qsf::smooth_corner::get_hitbox() const {
		return qpl::hitbox(this->position, this->dimension);
	}
	qpl::vector2f qsf::smooth_corner::get_center() const {
		return this->position + this->dimension / 2;
	}
	qpl::f64 qsf::smooth_corner::get_slope() const {
		return this->slope;
	}
	qpl::rgba qsf::smooth_corner::get_color() const {
		return this->color;
	}
	qpl::size qsf::smooth_corner::get_slope_point_count() const {
		return this->slope_point_count;
	}
	void qsf::smooth_corner::move(qpl::vector2f delta) {
		this->check_create();
		this->position.move(delta);
		this->internal_check = true;
	}
	void qsf::smooth_corner::check_create() const {
		if (this->internal_check) {
			qsf::vsmooth_corner corner;
			corner.set_dimension(this->dimension);
			corner.set_position(this->position);
			corner.set_slope(this->slope);
			corner.set_slope_point_count(this->slope_point_count);
			corner.set_color(this->get_color());
			corner.set_rotation(this->rotation);

			*this = corner;
		}
	}

	const qsf::smooth_corner& qsf::smooth_corner::operator=(const qsf::vsmooth_corner& smooth_corner) const {
		this->position = smooth_corner.get_position();
		this->dimension = smooth_corner.get_dimension();
		auto size = smooth_corner.slope_point_count;
		this->polygon.resize(size + 1);

		auto offset = smooth_corner.position;
		auto dimension = smooth_corner.dimension;

		auto center = smooth_corner.get_center();

		qpl::u32 ctr = 0u;
		this->polygon[ctr].position = smooth_corner.get_hitbox().get_bottom_right().rotated_around(center, smooth_corner.rotation);
		this->polygon[ctr].color = smooth_corner.get_color();
		++ctr;

		auto slope_exponent = std::pow(smooth_corner.slope, 1.0);

		for (qpl::u32 i = 0u; i < size; ++i) {
			auto progress = std::pow(qpl::f64_cast(i) / (size - 1), slope_exponent);
			auto value = qpl::smooth_curve(progress, smooth_corner.slope);
			auto curve_pos = qpl::vec(progress, 1.0 - value);

			auto pos = offset + curve_pos * dimension;
			this->polygon[ctr].position = pos.rotated_around(center, smooth_corner.rotation);
			this->polygon[ctr].color = smooth_corner.get_color();
			++ctr;
		}
		this->internal_check = false;
		return *this;
	}
	void qsf::smooth_corner::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		this->check_create();
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
	qsf::line& qsf::line::operator=(const qpl::straight_line& line) {
		this->vertices[0].position = line.a;
		this->vertices[1].position = line.b;
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
	void qsf::vpoints::add_point(qpl::vector2f position, qpl::rgba color) {
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



	bool qsf::vcircle::contains(qpl::vector2f position) const {
		return (this->get_center() - position).length() < this->radius;
	}
	void qsf::vcircle::set_position(qpl::vector2f position) {
		this->point.position = position;
	}
	void qsf::vcircle::set_radius(qpl::f32 radius) {
		this->radius = radius;
	}
	void qsf::vcircle::set_color(qpl::rgba color) {
		this->point.color = color;
	}
	void qsf::vcircle::set_center(qpl::vector2f center) {
		this->point.position = center - qpl::vector2f{ this->radius, this->radius };
	}
	void qsf::vcircle::set_outline_thickness(qpl::f32 outline_thickness) {
		this->outline_thickness = outline_thickness;
	}
	void qsf::vcircle::set_outline_color(qpl::rgba outline_color) {
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
	bool qsf::circle::contains(qpl::vector2f position) const {
		return (this->get_center() - position).length() < this->get_radius();
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
	void qsf::circle::set_color(qpl::rgba color) {
		this->circle_shape.setFillColor(color);
	}
	qpl::rgba qsf::circle::get_color() const {
		return this->circle_shape.getFillColor();
	}

	void qsf::circle::set_outline_thickness(qpl::f32 outline_thickness) {
		this->circle_shape.setOutlineThickness(outline_thickness);
	}
	qpl::f32 qsf::circle::get_outline_thickness() const {
		return this->circle_shape.getOutlineThickness();
	}
	void qsf::circle::set_outline_color(qpl::rgba outline_color) {
		this->circle_shape.setOutlineColor(outline_color);
	}
	qpl::rgba qsf::circle::get_outline_color() const {
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
	void qsf::vcircles::add_circle(qsf::vpoint point, qpl::f32 radius, qpl::rgba color) {
		this->circles.push_back(qsf::vcircle(point.position, radius, color));
	}
	void qsf::vcircles::add_circle(qpl::vector2f position, qpl::f32 radius, qpl::rgba color) {
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
	void qsf::circles::add_circle(const qsf::circle& circle) {
		this->circles_.push_back(circle);
	}
	void qsf::circles::add_circle(qsf::vpoint point, qpl::f32 radius, qpl::rgba color) {
		this->circles_.push_back(qsf::vcircle(point.position, radius, color));
	}
	void qsf::circles::add_circle(qpl::vector2f position, qpl::f32 radius, qpl::rgba color) {
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
	void qsf::vline::set_color(qpl::rgba color) {
		this->a.color = this->b.color = color;
	}
	void qsf::vline::set_a_color(qpl::rgba color) {
		this->a.color = color;
	}
	void qsf::vline::set_b_color(qpl::rgba color) {
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
	void qsf::line::set_color(qpl::rgba color) {
		this->vertices[0].color = this->vertices[1].color = color;
	}
	void qsf::line::set_a_color(qpl::rgba color) {
		this->vertices[0].color = color;
	}
	void qsf::line::set_b_color(qpl::rgba color) {
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
	void qsf::vlines::add_point(qpl::vector2f position, qpl::rgba color) {
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
	void qsf::lines::add_point(qpl::vector2f position, qpl::rgba color) {
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
	void qsf::vthick_line::set_color(qpl::rgba color) {
		this->a.color = this->b.color = color;
	}
	void qsf::vthick_line::set_a_color(qpl::rgba color) {
		this->a.color = color;
	}
	void qsf::vthick_line::set_b_color(qpl::rgba color) {
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
	void qsf::thick_line::extend_a(qpl::f32 delta) {

		auto direction = qpl::vector2f{ this->vertices[2].position.y - this->vertices[0].position.y, this->vertices[2].position.x - this->vertices[0].position.x } / this->length();
		this->vertices[0].position -= sf::Vector2f(direction * delta);
		this->vertices[1].position -= sf::Vector2f(direction * delta);
	}
	void qsf::thick_line::set_b(qsf::vpoint point) {
		this->vertices[2].position = this->vertices[3].position = point.position;
		this->vertices[2].color = this->vertices[3].color = point.color;
	}
	void qsf::thick_line::set_b(qpl::vector2f position) {
		this->vertices[2].position = this->vertices[3].position = position;
	}
	void qsf::thick_line::extend_b(qpl::f32 delta) {
		auto direction = qpl::vector2f{ this->vertices[2].position.y - this->vertices[0].position.y, this->vertices[2].position.x - this->vertices[0].position.x } / this->length();
		this->vertices[2].position += sf::Vector2f(direction * delta);
		this->vertices[3].position += sf::Vector2f(direction * delta);
	}
	void qsf::thick_line::set_color(qpl::rgba color) {
		this->vertices[0].color = this->vertices[1].color = this->vertices[2].color = this->vertices[3].color = color;
	}
	void qsf::thick_line::set_a_color(qpl::rgba color) {
		this->vertices[0].color = this->vertices[1].color = color;
	}
	void qsf::thick_line::set_b_color(qpl::rgba color) {
		this->vertices[2].color = this->vertices[3].color = color;
	}
	void qsf::thick_line::set_thickness(qpl::f32 thickness) {
		auto normal = this->normal();

		qpl::vector2f a_pos = this->vertices[0].position;
		qpl::vector2f b_pos = this->vertices[2].position;

		this->vertices[0].position = a_pos - normal * thickness;
		this->vertices[1].position = a_pos + normal * thickness;
		this->vertices[2].position = b_pos + normal * thickness;
		this->vertices[3].position = b_pos - normal * thickness;
	}
	void qsf::thick_line::move(qpl::vector2f delta) {
		this->vertices[0].position += sf::Vector2f(delta);
		this->vertices[1].position += sf::Vector2f(delta);
		this->vertices[2].position += sf::Vector2f(delta);
		this->vertices[3].position += sf::Vector2f(delta);
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
	void qsf::vthick_lines::add_thick_line(qpl::vector2f position, qpl::rgba color) {
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
	void qsf::thick_lines::add_thick_line(qpl::vector2f position, qpl::rgba color, qpl::f32 thickness) {
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
		qpl::rgba first_color;
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

		qpl::rgba mid_color;
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
	void qsf::sprite::set_color(qpl::rgba color) {
		this->m_sprite.setColor(color);
	}
	void qsf::sprite::set_position(qpl::vector2f position) {
		this->m_sprite.setPosition(position);
	}
	void qsf::sprite::set_position_x(qpl::f32 x) {
		auto y = this->get_position().y;
		this->m_sprite.setPosition(qpl::vec(x, y));
	}
	void qsf::sprite::set_position_y(qpl::f32 y) {
		auto x = this->get_position().x;
		this->m_sprite.setPosition(qpl::vec(x, y));
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
	qpl::rgba qsf::sprite::get_color() const {
		return this->m_sprite.getColor();
	}
	qpl::vector2f qsf::sprite::get_position() const {
		return this->m_sprite.getPosition();
	}
	qpl::vector2f qsf::sprite::get_dimension() const {
		return qsf::get_sprite_hitbox(this->m_sprite).dimension;
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
	void qsf::sprite::centerize() {
		this->set_position(this->get_position() - this->get_dimension() / 2);
	}
	void qsf::sprite::centerize_x() {
		auto y = this->get_position().y;
		auto x = this->get_position().x - this->get_dimension().x / 2;
		this->set_position(qpl::vec(x, y));
	}
	void qsf::sprite::centerize_y() {
		auto y = this->get_position().y - this->get_dimension().y / 2;
		auto x = this->get_position().x;
		this->set_position(qpl::vec(x, y));
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
		return *this;
	}

	qsf::transition_overlay::transition_overlay() {
		this->overlay.set_color(qpl::rgba::black());
		this->set_duration(0.3);
	}

	void qsf::transition_overlay::set_color(qpl::rgba color) {
		this->overlay.set_color(color);
	}
	void qsf::transition_overlay::set_slope(qpl::f64 slope) {
		this->slope = slope;
	}
	void qsf::transition_overlay::set_dimension(qpl::vector2f dimension) {
		this->overlay.set_dimension(dimension);
	}
	void qsf::transition_overlay::set_duration(qpl::f64 duration) {
		this->animation.set_duration(duration);
	}
	void qsf::transition_overlay::make_disappear() {
		this->animation.reset_and_start();
	}
	void qsf::transition_overlay::make_appear() {
		this->animation.reset_and_start_reverse();
	}
	void qsf::transition_overlay::update(const qsf::event_info& event) {
		this->animation.update(event.frame_time_f());
		if (this->animation.is_running()) {
			auto p = this->animation.get_curve_progress(this->slope);
			auto a = qpl::u8_cast(255 * p);
			this->overlay.set_multiplied_alpha(a);
		}
	}
	bool qsf::transition_overlay::just_finished_disappearing() const {
		return this->animation.just_finished_no_reverse();
	}
	bool qsf::transition_overlay::just_finished_appearing() const {
		return this->animation.just_finished_reverse();
	}
	void qsf::transition_overlay::draw(qsf::draw_object& draw) const {
		draw.draw(this->overlay);
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

	void qsf::render_texture::set_color(qpl::rgba color) {
		this->m_sprite.set_color(color);
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
	qpl::rgba qsf::render_texture::get_color() const {
		return this->m_sprite.get_color();
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
	void qsf::pixel_image::set(qpl::size x, qpl::size y, qpl::rgba color) {
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

	void qsf::tile_map::set_texture_ptr(const sf::Texture& texture, qpl::vector2u texture_tile_dimension) {
		this->texture_ptr = &texture;
		this->texture_tile_dimension = texture_tile_dimension;
		this->position_tile_dimension = this->texture_tile_dimension;
		this->texture_ptr_set = true;
	}
	void qsf::tile_map::set_texture_ptr(const sf::Texture& texture, qpl::u32 texture_tile_width) {
		this->set_texture_ptr(texture, qpl::vec(texture_tile_width, texture_tile_width));
	}
	void qsf::tile_map::set_position(qpl::vector2f position) {
		this->position = position;
	}

	void qsf::tile_map::create(const std::vector<std::pair<qpl::u32, qpl::u32>>& indices, qpl::size width, qpl::rgba color) {
		if (!this->texture_ptr_set) {
			throw qpl::exception("tile_map::create: texture_ptr not set");
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

		if (color == qpl::rgba::white()) {
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
		this->create(indices, width, qpl::rgba::white());
	}


	void qsf::tile_map::create(const std::vector<std::pair<qpl::u32, qpl::f32>>& indices, qpl::size width, qpl::rgba color) {
		if (!this->texture_ptr_set) {
			throw qpl::exception("tile_map::create: texture_ptr not set");
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

		if (color == qpl::rgba::white()) {
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
		this->create(indices, width, qpl::rgba::white());
	}
	void qsf::tile_map::create(const std::vector<qpl::u32>& indices, qpl::size width, qpl::rgba color) {
		if (!this->texture_ptr_set) {
			throw qpl::exception("tile_map::create: texture_ptr not set");
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

		if (color == qpl::rgba::white()) {
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
		this->create(indices, width, qpl::rgba::white());
	}


	void qsf::tile_map::create_skip_empty(const std::vector<std::pair<qpl::u32, qpl::u32>>& indices, qpl::size width, qpl::rgba color, qpl::u32 skip_index) {
		if (!this->texture_ptr_set) {
			throw qpl::exception("tile_map::create: texture_ptr not set");
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

		if (color == qpl::rgba::white()) {
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
		this->create_skip_empty(indices, width, qpl::rgba::white(), skip_index);
	}

	void qsf::tile_map::create_skip_empty(const std::vector<std::pair<qpl::u32, qpl::f32>>& indices, qpl::size width, qpl::rgba color, qpl::u32 skip_index) {
		if (!this->texture_ptr_set) {
			throw qpl::exception("tile_map::create: texture_ptr not set");
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

		if (color == qpl::rgba::white()) {
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
		this->create_skip_empty(indices, width, qpl::rgba::white(), skip_index);
	}
	void qsf::tile_map::create_skip_empty(const std::vector<qpl::u32>& indices, qpl::size width, qpl::rgba color, qpl::u32 skip_index) {
		if (!this->texture_ptr_set) {
			throw qpl::exception("tile_map::create: texture_ptr not set");
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
		if (color == qpl::rgba::white()) {
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
		this->create_skip_empty(indices, width, qpl::rgba::white(), skip_index);
	}

	void qsf::tile_map::set_color(qpl::rgba color) {
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
		if (this->vertices.size()) {
			states.transform.translate(this->position);
			states.texture = this->texture_ptr;
			this->vertices.draw(window, states);
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
	void qsf::vgraph::info_graph::set_text_color(qpl::rgba text_color) {
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
	void qsf::vgraph::info_graph::set_text_outline_color(qpl::rgba color) {
		this->text.outline_color = color;
	}

	qpl::f64 qsf::vgraph::info_graph::get_percentage_height_at(qpl::size index) const {
		auto [low, high] = this->get_low_high();

		return (this->data[index].data - low.data) / (high.data - low.data);
	}
	void qsf::vgraph::info_graph::add_data(qpl::f64 data) {
		this->add_data(data, this->color, this->thickness);
	}
	void qsf::vgraph::info_graph::add_data(qpl::f64 data, qpl::rgba color) {
		this->add_data(data, color, this->thickness);
	}
	void qsf::vgraph::info_graph::add_data(qpl::f64 data, qpl::rgba color, qpl::f64 thickness) {
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
	void qsf::vgraph::info_graph::add_data(qpl::f64 data, std::string string, qpl::rgba color) {
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
	void qsf::vgraph::standard_graph::add_data(qpl::f64 data, qpl::rgba color) {
		this->add_data(data, color, this->thickness);
	}
	void qsf::vgraph::standard_graph::add_data(qpl::f64 data, qpl::rgba color, qpl::f64 thickness) {
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
	void qsf::vgraph::set_visible_range_max() {
		this->index_start = 0u;
		this->index_end = qpl::size_max;
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
		this->add_y_axis_line(value, qpl::rgba::red(), this->axis_thickness / 2);
	}
	void qsf::vgraph::add_y_axis_line(qpl::f64 value, qpl::rgba color, qpl::f64 thickness, bool foreground) {
		this->y_axis_highlighted_lines[value].color = color;
		this->y_axis_highlighted_lines[value].thickness = thickness;
		this->y_axis_highlighted_lines[value].foreground = foreground;
	}
	qsf::vgraph::highlighted_line& qsf::vgraph::get_y_axis_line(qpl::f64 value) {
		if (this->y_axis_highlighted_lines.find(value) == this->y_axis_highlighted_lines.cend()) {
			this->y_axis_highlighted_lines[value].color = qpl::rgba::red();
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

				if (!use_interpolated_color && span[i].color != qpl::frgba::unset()) {
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
			qpl::frgba using_color = g.second.color;
			if (using_color == qpl::frgba::unset()) {
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

				if (!use_interpolated_color && span[i].color != qpl::frgba::unset()) {
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

			qpl::frgba using_color = g.second.color;
			if (using_color == qpl::frgba::unset()) {
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

			qpl::frgba using_color = g.second.color;
			if (using_color == qpl::frgba::unset()) {
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
			qpl::rgba use_color = graph.axis_color;


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
							this->y_lines.back().add_thick_line(a, y.second.color, qpl::f32_cast(y.second.thickness));
							this->y_lines.back().add_thick_line(b, y.second.color, qpl::f32_cast(y.second.thickness));
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
					this->y_lines[i].add_thick_line(a, graph.axis_line_color, qpl::f32_cast(graph.axis_thickness));
					this->y_lines[i].add_thick_line(b, graph.axis_line_color, qpl::f32_cast(graph.axis_thickness));
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

		this->cursor_graph_text.set_color(qpl::rgba::unset());
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
		this->background.draw(window, states);

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
	void qsf::border_graphic::set_color(qpl::rgba color) {
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
			throw qpl::exception("qsf::border_graphic : texture was not set.");
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
	void qsf::border_graphic::add_top(qpl::f32 correction_gap) {
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

			pos.x += this->texture_dimension.x * this->scale.y + correction_gap;
			++ctr;
		}
	}
	void qsf::border_graphic::add_bottom(qpl::f32 correction_gap) {
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

			pos.x += this->texture_dimension.x * this->scale.y + correction_gap;
			++ctr;
		}
	}
	void qsf::border_graphic::add_left(qpl::f32 correction_gap) {
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

			pos.y += this->texture_dimension.x * this->scale.x + correction_gap;
			++ctr;
		}
	}
	void qsf::border_graphic::add_right(qpl::f32 correction_gap) {
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

			pos.y += this->texture_dimension.x * this->scale.x + correction_gap;
			++ctr;
		}
	}
	void qsf::border_graphic::add_all_sides(qpl::f32 correction_gap) {
		this->add_top(correction_gap);
		this->add_left(correction_gap);
		this->add_right(correction_gap);
		this->add_bottom(correction_gap);
	}
	void qsf::border_graphic::draw(qsf::draw_object& object) const {
		object.draw(this->sprites);
	}

	namespace {
		void addLine(qsf::vertex_array& vertices,
			qpl::f32 line_length,
			qpl::f32 lineTop,
			const qpl::rgba& color,
			qpl::f32 offset,
			qpl::f32 thickness,
			qpl::f32 outline_thickness = 0) {
			qpl::f32 top = std::floor(lineTop + offset - (thickness / 2) + 0.5f);
			qpl::f32 bottom = top + std::floor(thickness + 0.5f);

			vertices.add(qsf::vertex(qpl::vec2f(-outline_thickness, top - outline_thickness), color, qpl::vec2f(1, 1)));
			vertices.add(qsf::vertex(qpl::vec2f(line_length + outline_thickness, top - outline_thickness), color, qpl::vec2f(1, 1)));
			vertices.add(qsf::vertex(qpl::vec2f(-outline_thickness, bottom + outline_thickness), color, qpl::vec2f(1, 1)));
			vertices.add(qsf::vertex(qpl::vec2f(-outline_thickness, bottom + outline_thickness), color, qpl::vec2f(1, 1)));
			vertices.add(qsf::vertex(qpl::vec2f(line_length + outline_thickness, top - outline_thickness), color, qpl::vec2f(1, 1)));
			vertices.add(qsf::vertex(qpl::vec2f(line_length + outline_thickness, bottom + outline_thickness), color, qpl::vec2f(1, 1)));
		}


		glyph_quad get_glyph_quad(const sf::Glyph& glyph, qpl::f32 italic_shear) {
			qpl::f32 padding = 1.0;

			qpl::f32 left = glyph.bounds.left - padding;
			qpl::f32 top = glyph.bounds.top - padding;
			qpl::f32 right = glyph.bounds.left + glyph.bounds.width + padding;
			qpl::f32 bottom = glyph.bounds.top + glyph.bounds.height + padding;

			qpl::f32 u1 = qpl::f32_cast(glyph.textureRect.left) - padding;
			qpl::f32 v1 = qpl::f32_cast(glyph.textureRect.top) - padding;
			qpl::f32 u2 = qpl::f32_cast(glyph.textureRect.left + glyph.textureRect.width) + padding;
			qpl::f32 v2 = qpl::f32_cast(glyph.textureRect.top + glyph.textureRect.height) + padding;

			qpl::size ctr = 0u;
			glyph_quad result;
			result[0u] = glyph_quad_vertex{ qpl::vec2f(left - italic_shear * top, top), qpl::vec2f(u1, v1) };
			result[1u] = glyph_quad_vertex{ qpl::vec2f(right - italic_shear * top, top), qpl::vec2f(u2, v1) };
			result[2u] = glyph_quad_vertex{ qpl::vec2f(left - italic_shear * bottom, bottom), qpl::vec2f(u1, v2) };
			result[3u] = glyph_quad_vertex{ qpl::vec2f(left - italic_shear * bottom, bottom), qpl::vec2f(u1, v2) };
			result[4u] = glyph_quad_vertex{ qpl::vec2f(right - italic_shear * top, top), qpl::vec2f(u2, v1) };
			result[5u] = glyph_quad_vertex{ qpl::vec2f(right - italic_shear * bottom, bottom), qpl::vec2f(u2, v2) };
			return result;
		}

		// Add a glyph quad to the vertex array
		void add_glyph_quad(qsf::vertex_array& vertices, qpl::vec2f position, const qpl::rgba& color, const sf::Glyph& glyph, qpl::f32 italic_shear) {
			auto quad = get_glyph_quad(glyph, italic_shear);

			for (auto& i : quad) {
				qsf::vertex vertex;
				vertex.position = i.position + position;
				vertex.tex_coords = i.tex_coord;
				vertex.color = color;
				vertices.add(vertex);
			}
		}
	}

	qsf::colored_text::colored_text() {
		this->clear();
	}
	qsf::colored_text::colored_text(const qsf::colored_text&) = default;
	qsf::colored_text& qsf::colored_text::operator=(const qsf::colored_text&) = default;
	qsf::colored_text::colored_text(qsf::colored_text&&) noexcept = default;
	qsf::colored_text& qsf::colored_text::operator=(qsf::colored_text&&) noexcept = default;
	void qsf::colored_text::set_font(const sf::Font& font) {
		this->font = &font;
	}
	void qsf::colored_text::set_unicode_font(const sf::Font& font) {
		this->unicode_font = &font;
	}
	void qsf::colored_text::set_character_size(qpl::u32 size) {
		if (this->character_size != size) {
			this->character_size = size;
		}
	}
	void qsf::colored_text::set_letter_spacing(qpl::f32 spacingFactor) {
		if (this->letter_spacing_factor != spacingFactor) {
			this->letter_spacing_factor = spacingFactor;
		}
	}
	void qsf::colored_text::set_line_spacing(qpl::f32 spacingFactor) {
		if (this->line_spacing_factor != spacingFactor) {
			this->line_spacing_factor = spacingFactor;
		}
	}
	void qsf::colored_text::set_style(qpl::u32 style) {
		if (this->style != style) {
			this->style = style;
		}
	}
	qpl::u32 qsf::colored_text::get_character_size() const {
		return this->character_size;
	}
	qpl::f32 qsf::colored_text::get_line_spacing() const {
		return this->line_spacing_factor;
	}
	qpl::f32 qsf::colored_text::get_line_spacing_pixels() const {
		return this->font->getLineSpacing(this->character_size) * this->line_spacing_factor;
	}
	qpl::f32 qsf::colored_text::get_outline_thickness() const {
		return this->outline_thickness;
	}
	qpl::f32 qsf::colored_text::get_white_space_width() const {
		auto whitespace_width = this->font->getGlyph(U' ', this->character_size, false).advance;
		auto letter_spacing = (whitespace_width / 3.f) * (this->letter_spacing_factor - 1.f);
		return whitespace_width + letter_spacing;
	}
	qpl::f32 qsf::colored_text::get_underline_position() const {
		return this->font->getUnderlinePosition(this->character_size);
	}
	qpl::f32 qsf::colored_text::get_chracter_top_offset() const {
		return this->character_size + this->font->getGlyph(U'A', this->character_size, false).bounds.top;
	}
	const sf::Glyph& qsf::colored_text::get_glyph(qpl::u32 character, qpl::u32 character_size, bool is_bold, qpl::f32 outline_thickness) {
		return this->font->getGlyph(character, character_size, is_bold, outline_thickness);
	}
	const sf::Glyph& qsf::colored_text::get_unicode_glyph(qpl::u32 character, qpl::u32 character_size, bool is_bold, qpl::f32 outline_thickness) {
		return this->unicode_font->getGlyph(character, character_size, is_bold, outline_thickness);
	}
	qpl::u32 qsf::colored_text::get_style() const {
		return this->style;
	}
	void qsf::colored_text::clear() {
		this->vertices.clear();
		this->outline_vertices.clear();
		this->unicode_vertices.clear();
		this->unicode_outline_vertices.clear();
		this->text_position.x = 0.f;
		this->text_position.y = 0.f;
		this->rows = 0u;
	}
	void qsf::colored_text::pop_last_character() {
		if (this->last_element.style & sf::Text::Style::Bold) {
			this->outline_vertices.resize(qpl::max(0ll, qpl::signed_cast(this->outline_vertices.size()) - 6));
		}
		this->vertices.resize(qpl::max(0ll, qpl::signed_cast(this->vertices.size()) - 6));
	}
	void qsf::colored_text::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		if (this->font) {

			sf::RenderStates new_states{ states };

			new_states.texture = &this->font->getTexture(this->character_size);

			if (this->outline_vertices.size()) {
				this->outline_vertices.draw(target, new_states);
			}

			this->vertices.draw(target, new_states);
		}
		if (this->unicode_font) {

			sf::RenderStates new_states{ states };

			new_states.texture = &this->unicode_font->getTexture(this->character_size);

			if (this->unicode_outline_vertices.size()) {
				this->unicode_outline_vertices.draw(target, new_states);
			}

			this->unicode_vertices.draw(target, new_states);
		}
	}
	void qsf::colored_text::create(const qpl::styled_string<qpl::u32_string>& string, qpl::f32 visible_y_min, qpl::f32 visible_y_max) {
		this->clear();
		this->add(string, visible_y_min, visible_y_max);
	}
	void qsf::colored_text::add(const qpl::styled_string<qpl::u32_string>& string, qpl::f32 visible_y_min, qpl::f32 visible_y_max) {
		if (!this->font) {
			return;
		}

		this->hitbox = qpl::hitbox{};

		if (string.empty()) {
			return;
		}
		auto underline_offset = this->font->getUnderlinePosition(this->character_size);
		auto underline_thickness = this->font->getUnderlineThickness(this->character_size);


		auto line_spacing = this->get_line_spacing_pixels();

		if (this->text_position.y == 0.f) {
			this->text_position.y = this->font->getLineSpacing(this->character_size);
		}

		auto minX = qpl::f32_cast(this->character_size);
		auto minY = 0.f;
		auto maxX = 0.f;
		auto maxY = 0.f;
		qpl::u32 previous = 0;

		this->last_element.copy_style(string.elements.back());
		for (auto& element : string) {
			bool is_bold = element.style & sf::Text::Style::Bold;
			bool is_underlined = element.style & sf::Text::Style::Underlined;
			bool is_strike_through = element.style & sf::Text::Style::StrikeThrough;
			auto italic_shear = (element.style & sf::Text::Style::Italic) ? 0.2094395102393f : 0.f;

			const auto& default_glyph = this->get_glyph(U'x', this->character_size, is_bold);
			sf::FloatRect default_bounds = default_glyph.bounds;

			sf::Glyph default_unicode_glyph;
			if (this->unicode_font) {
				default_unicode_glyph = this->get_unicode_glyph(U'x', this->character_size, is_bold);
			}

			auto unreadable_glyph_texture = this->get_glyph(63744u, this->character_size, is_bold).textureRect;

			auto strike_through_offset = default_bounds.top + default_bounds.height / 2.f;
			auto whitespace_width = this->font->getGlyph(U' ', this->character_size, is_bold).advance;
			auto letter_spacing = (whitespace_width / 3.f) * (this->letter_spacing_factor - 1.f);
			whitespace_width += letter_spacing;

			for (qpl::size i = 0u; i < element.text.length(); ++i) {
				qpl::u32 c = element.text[i];

				if (c == U'\r') {
					continue;
				}
				this->text_position.x += this->font->getKerning(previous, c, this->character_size);
				if (is_underlined && (c == U'\n' && previous != U'\n')) {
					addLine(this->vertices, this->text_position.x, this->text_position.y, element.color, underline_offset, underline_thickness);

					if (element.outline_thickness != 0) {
						addLine(this->outline_vertices, this->text_position.x, this->text_position.y, element.outline_color, underline_offset, underline_thickness, element.outline_thickness);
					}
				}
				if (is_strike_through && (c == U'\n' && previous != U'\n')) {
					addLine(this->vertices, this->text_position.x, this->text_position.y, element.color, strike_through_offset, underline_thickness);

					if (element.outline_thickness != 0) {
						addLine(this->outline_vertices, this->text_position.x, this->text_position.y, element.outline_color, strike_through_offset, underline_thickness, element.outline_thickness);
					}
				}

				previous = c;

				if ((c == U' ') || (c == U'\n') || (c == U'\t')) {
					minX = std::min(minX, this->text_position.x);
					minY = std::min(minY, this->text_position.y);

					switch (c) {
					case U' ':
						this->text_position.x += whitespace_width;
						break;
					case U'\t':
						this->text_position.x += whitespace_width * 4;
						break;
					case U'\n':
						this->text_position.y += line_spacing;
						this->text_position.x = 0;
						++this->rows;
						break;
					}

					maxX = std::max(maxX, this->text_position.x);
					maxY = std::max(maxY, this->text_position.y);
					continue;
				}

				bool in_range = (this->text_position.y - line_spacing) > visible_y_min && (text_position.y + line_spacing) < visible_y_max;

				if (in_range && element.outline_thickness != 0) {
					sf::Glyph glyph;
					if (this->unicode_font && qpl::unicode_character_length(c) == 2u) {
						glyph = this->get_unicode_glyph(c, this->character_size, is_bold, element.outline_thickness);
						if (in_range) {
							add_glyph_quad(this->unicode_outline_vertices, this->text_position, element.color, glyph, italic_shear);
						}
					}
					else {
						glyph = this->get_glyph(c, this->character_size, is_bold, element.outline_thickness);
						if (in_range) {
							add_glyph_quad(this->outline_vertices, this->text_position, element.color, glyph, italic_shear);
						}
					}
				}

				sf::Glyph glyph;
				glyph = this->get_glyph(c, this->character_size, is_bold);
				auto glyph_bounds = glyph.textureRect;
				if (this->unicode_font && glyph_bounds.left == unreadable_glyph_texture.left && glyph_bounds.top == unreadable_glyph_texture.top &&
					glyph_bounds.width == unreadable_glyph_texture.width && glyph_bounds.height == unreadable_glyph_texture.height) {

					glyph = this->get_unicode_glyph(c, this->character_size, is_bold);

					auto factor = glyph.advance / default_unicode_glyph.advance;
					if (factor >= 1.5) {
						glyph.advance = default_glyph.advance * 2u;
					}
					else {
						glyph.advance = default_glyph.advance;
					}
					if (in_range) {
						add_glyph_quad(this->unicode_vertices, this->text_position, element.color, glyph, italic_shear);
					}
				}
				else {
					glyph = this->get_glyph(c, this->character_size, is_bold);
					if (in_range) {
						add_glyph_quad(this->vertices, this->text_position, element.color, glyph, italic_shear);
					}
				}


				qpl::f32 left = glyph.bounds.left;
				qpl::f32 top = glyph.bounds.top;
				qpl::f32 right = glyph.bounds.left + glyph.bounds.width;
				qpl::f32 bottom = glyph.bounds.top + glyph.bounds.height;

				minX = std::min(minX, this->text_position.x + left - italic_shear * bottom);
				maxX = std::max(maxX, this->text_position.x + right - italic_shear * top);
				minY = std::min(minY, this->text_position.y + top);
				maxY = std::max(maxY, this->text_position.y + bottom);

				this->text_position.x += glyph.advance + letter_spacing;
			}
		}

		//if (this->outline_vertices.size()) {
		//    qpl::f32 outline = std::abs(std::ceil(element.outline_thickness));
		//    minX -= outline;
		//    maxX += outline;
		//    minY -= outline;
		//    maxY += outline;
		//}

		//if (is_underlined && (x > 0)) {
		//    addLine(this->vertices, x, y, element.color, underline_offset, underline_thickness);
		//
		//    if (element.outline_thickness != 0) {
		//        addLine(this->outline_vertices, x, y, element.outline_color, underline_offset, underline_thickness, element.outline_thickness);
		//    }
		//}
		//if (is_strike_through && (x > 0)) {
		//    addLine(this->vertices, x, y, element.color, strike_through_offset, underline_thickness);
		//
		//    if (element.outline_thickness != 0) {
		//        addLine(this->outline_vertices, x, y, element.outline_color, strike_through_offset, underline_thickness, element.outline_thickness);
		//    }
		//}

		this->hitbox.position.x = minX;
		this->hitbox.position.y = 0;
		this->hitbox.dimension.x = maxX - minX;
		this->hitbox.dimension.y = maxY - minY;
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