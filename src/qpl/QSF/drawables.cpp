#include <qpl/QSF/drawables.hpp>
#if defined(QPL_USE_SFML) || defined(QPL_USE_ALL)
#include <qpl/QSF/resources.hpp>
#include <qpl/QSF/event_info.hpp>
#include <qpl/QSF/utility.hpp>
#include <qpl/algorithm.hpp>

#include <qpl/time.hpp>

#include <qpl/system.hpp>
#include <qpl/memory.hpp>
#include <qpl/span.hpp>

namespace qsf {

	qsf::vrectangle qsf::text_hitbox(const sf::Text& text) {
		qsf::vrectangle rectangle;
		auto local_bounds = text.getLocalBounds();
		auto global_bounds = text.getGlobalBounds();

		rectangle.set_position({ global_bounds.left, global_bounds.top });
		rectangle.set_dimension({ local_bounds.width, local_bounds.height });

		return rectangle;
	}
	qsf::vrectangle qsf::text_hitbox(const qsf::text& text) {
		return text.visible_hitbox();
	}
	void qsf::centerize_text(sf::Text& text) {
		auto rect = qsf::text_hitbox(text).dimension;
		text.move(-rect / 2);
	}
	void qsf::centerize_text(qsf::text& text) {
		text.centerize();
	}


	namespace detail {
		qsf::text qsf::detail::text;
		qsf::rectangle qsf::detail::rectangle;
		qsf::rectangles qsf::detail::rectangles;
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
	qsf::vgraph qsf::drawing_graph;


	void qsf::vertex::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		window.draw(reinterpret_cast<const sf::Vertex*>(this), 1, sf::PrimitiveType::Points, states);

		//sf::Vertex v;
		//v.position = this->position;
		//v.color = this->color;
		//v.texCoords = this->texCoords;
		//
		//
		//
		//auto mem1 = qpl::memory_to_u64_array(v);
		//auto mem2 = qpl::memory_to_u64_array(*this);
		//
		//
		//qpl::println_container_hex(mem1);
		//qpl::println_container_hex(mem2);
		//qpl::println();
		//
		//qpl::system_pause();
		//
		//window.draw(&v, 1, sf::PrimitiveType::Points, states);
	}


	void qsf::vertex_array::set_primitive_type(qsf::primitive_type primitive_type) {
		this->primitive_type = primitive_type;
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
	void qsf::vtext::set_color(qsf::rgb color) {
		this->color = color;
	}
	void qsf::vtext::set_outline_thickness(qpl::f32 outline_thickness) {
		this->outline_thickness = outline_thickness;
	}
	void qsf::vtext::set_outline_color(qsf::rgb color) {
		this->color = color;
	}
	void qsf::vtext::set_letter_spacing(qpl::f32 spacing) {
		this->letter_spacing = spacing;
	}
	void qsf::vtext::set_position(qsf::vector2f position) {
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

	std::string qsf::text::get_font() const {
		return this->m_font;
	}
	qpl::u32 qsf::text::get_style() const {
		return this->m_text.getStyle();
	}
	qpl::u32 qsf::text::get_character_size() const {
		return this->m_text.getCharacterSize();
	}
	qsf::rgb qsf::text::get_color() const {
		return this->m_text.getFillColor();
	}
	qpl::f32 qsf::text::get_outline_thickness() const {
		return this->m_text.getOutlineThickness();
	}
	qsf::rgb qsf::text::get_outline_color() const {
		return this->m_text.getOutlineColor();
	}
	qpl::f32 qsf::text::get_letter_spacing() const {
		return this->m_text.getLetterSpacing();
	}
	qsf::vector2f qsf::text::get_position() const {
		return this->m_text.getPosition();
	}
	qsf::vector2f qsf::text::get_center() const {
		return this->standard_hitbox().get_center();
	}
	std::string qsf::text::get_string() const {
		return this->m_string;
	}
	void qsf::text::set_font(const std::string& font_name) {
		this->m_font = font_name;
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
	void qsf::text::set_color(qsf::rgb color) {
		this->m_text.setFillColor(color);
	}
	void qsf::text::set_outline_thickness(qpl::f32 outline_thickness) {
		this->m_text.setOutlineThickness(outline_thickness);
	}
	void qsf::text::set_outline_color(qsf::rgb color) {
		this->m_text.setOutlineColor(color);
	}
	void qsf::text::set_letter_spacing(qpl::f32 spacing) {
		this->m_text.setLetterSpacing(spacing);
	}
	void qsf::text::set_position(qsf::vector2f position) {
		this->m_text.setPosition(position);
	}
	void qsf::text::set_center(qsf::vector2f position) {
		this->set_position(position);
		this->centerize();
	}
	void qsf::text::set_string(const std::string& string) {
		this->m_string = string;
		this->m_text.setString(this->m_string);
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
	void qsf::text::move(qsf::vector2f delta) {
		this->m_text.move(delta);
	}
	qsf::vrectangle qsf::text::visible_hitbox() const {
		qsf::vrectangle rectangle;
		auto local_bounds = this->m_text.getLocalBounds();
		auto global_bounds = this->m_text.getGlobalBounds();

		rectangle.set_position({ global_bounds.left, global_bounds.top });
		rectangle.set_dimension({ local_bounds.width, local_bounds.height });

		return rectangle;
	}
	qsf::vrectangle qsf::text::standard_hitbox() const {
		qsf::vrectangle rectangle;
		auto local_bounds = this->m_text.getLocalBounds();
		//auto global_bounds = this->m_text.getGlobalBounds();

		rectangle.set_position(this->m_text.getPosition());
		rectangle.set_dimension({ local_bounds.width, local_bounds.height });

		

		return rectangle;
	}
	qsf::vector2f qsf::text::offset() const {
		auto visible = this->visible_hitbox();
		return visible.position - this->get_position();
	}

	std::string qsf::text::string() const {
		return this->m_string;
	}

	void qsf::text::clear() {
		this->m_string.clear();
		this->m_text.setString(this->m_string);
	}
	qsf::text& qsf::text::operator<<(const std::string& string) {
		this->m_string.append(string);
		this->m_text.setString(this->m_string);
		return *this;
	}
	qsf::text& qsf::text::operator=(const qsf::vtext& text) {
		this->set_character_size(text.character_size);
		this->set_color(text.color);
		this->m_font = text.font_name;
		this->set_letter_spacing(text.letter_spacing);
		this->set_outline_color(text.outline_color);
		this->set_outline_thickness(text.outline_thickness);
		this->set_position(text.position);
		this->set_style(text.style);
		this->set_string(text.string);
		return *this;
	}

	void qsf::text::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		window.draw(this->m_text, states);
	}

	void qsf::text::draw_if_visible(sf::RenderTarget& window, sf::RenderStates states) const {

		auto hb = this->visible_hitbox();

		auto rect = states.transform.transformRect(sf::FloatRect(hb.position.x, hb.position.y, hb.dimension.x, hb.dimension.y));
		auto fov = sf::FloatRect(0, 0, window.getSize().x, window.getSize().y);


		if (rect.intersects(fov)) {
			window.draw(this->m_text, states);
		}
	}


	qsf::endl_type endl;

	void qsf::text_stream::apply_state() {
		this->texts.back().back().set_character_size(this->states.back().character_size);
		this->texts.back().back().set_font(this->states.back().font);
		this->texts.back().back().set_style(this->states.back().style);
		this->texts.back().back().set_color(this->states.back().color);
		this->texts.back().back().set_letter_spacing(this->states.back().letter_spacing);
		this->texts.back().back().set_outline_thickness(this->states.back().outline_thickness);
		this->texts.back().back().set_outline_color(this->states.back().outline_color);
	}
	void qsf::text_stream::new_line() {
		auto hitbox = this->line_hitbox(this->lines() - 1);
		this->texts.push_back({});
		this->texts.back().push_back({});
		this->texts.back().back().set_position(qsf::vector2f(hitbox.position + qsf::vector2f(0, hitbox.dimension.y + this->line_spacing)));
		this->apply_state();


		if (this->states.size() > 1) {
			if (this->states.back().duration == qsf::text_stream::duration_type::next_entry || this->states.back().duration == qsf::text_stream::duration_type::end_of_line) {
				this->states.pop_back();
			}
		}
	}
	void qsf::text_stream::clear() {
		this->texts.clear();
		this->states.resize(1);
	}
	text_stream& qsf::text_stream::operator<<(const std::string& string) {
		return this->add_string(string);
	}
	text_stream& qsf::text_stream::operator<<(const std::wstring& string) {
		return this->add_string(string);
	}

	text_stream& qsf::text_stream::add_string(const std::string& string, bool has_no_newline) {
		if (!has_no_newline) {
			qpl::u32 last = 0u;
			for (qpl::u32 i = 0u; !has_no_newline && i < string.size(); ++i) {
				if (string[i] == '\n') {
					qpl::i32 size = qpl::i32_cast(i) - 1 - qpl::i32_cast(last);
					if (size > 0) {
						std::string sub = std::string(string.begin() + last, string.begin() + i);
						this->add_string(sub, true);
					}
					while (i < string.length() && string[i] == '\n') {
						this->new_line();
						++i;
					}
					last = i;
				}
			}
			std::string sub = std::string(string.begin() + last, string.end());
			return this->add_string(sub, true);
		}
		if (this->texts.empty()) {
			this->texts.push_back({});
			this->texts.back().push_back({});
			this->texts.back().back().set_position(this->position);
		}
		else {
			this->texts.back().push_back(this->texts.back().back());
		}
		auto hitbox = this->line_hitbox(this->lines() - 1);
		this->apply_state();
		this->texts.back().back().set_string(string);
		this->texts.back().back().set_position(hitbox.position + hitbox.dimension.just_x() + qsf::vector2f(this->states.back().letter_spacing, -this->texts.back().back().offset().y));

		this->centerize_line(this->lines() - 1);


		if (this->states.size() > 1) {
			if (this->states.back().duration == qsf::text_stream::duration_type::next_entry) {
				this->states.pop_back();
			}
		}
		return *this;
	}

	text_stream& qsf::text_stream::add_string(const std::wstring& string, bool has_no_newline) {
		if (!has_no_newline) {
			qpl::u32 last = 0u;
			for (qpl::u32 i = 0u; !has_no_newline && i < string.size(); ++i) {
				if (string[i] == L'\n') {
					qpl::i32 size = qpl::i32_cast(i) - 1 - qpl::i32_cast(last);
					if (size > 0) {
						std::wstring sub = std::wstring(string.begin() + last, string.begin() + i);
						this->add_string(sub, true);
					}
					while (i < string.length() && string[i] == L'\n') {
						this->new_line();
						++i;
					}
					last = i;
				}
			}
			std::wstring sub = std::wstring(string.begin() + last, string.end());
			return this->add_string(sub, true);
		}
		if (this->texts.empty()) {
			this->texts.push_back({});
			this->texts.back().push_back({});
			this->texts.back().back().set_position(this->position);
		}
		else {
			this->texts.back().push_back(this->texts.back().back());
		}
		auto hitbox = this->line_hitbox(this->lines() - 1);
		this->apply_state();
		this->texts.back().back().m_string = qpl::wstring_to_string(string);
		this->texts.back().back().m_text.setString(string);
		this->texts.back().back().set_position(hitbox.position + hitbox.dimension.just_x() + qsf::vector2f(this->states.back().letter_spacing, -this->texts.back().back().offset().y));

		this->centerize_line(this->lines() - 1);

		if (this->states.size() > 1) {
			if (this->states.back().duration == qsf::text_stream::duration_type::next_entry) {
				this->states.pop_back();
			}
		}

		return *this;
	}
	qsf::vrectangle qsf::text_stream::line_hitbox(qpl::size index) const {
		if (index >= this->texts.size()) {
			return {};
		}
		auto front_hitbox = this->texts[index].front().visible_hitbox();
		qpl::f32 min_y = front_hitbox.position.y;
		qpl::f32 max_y = front_hitbox.position.y + front_hitbox.dimension.y;

		for (qpl::u32 i = 1u; i < this->texts[index].size(); ++i) {
			auto hitbox = this->texts[index][i].visible_hitbox();
			if (min_y > hitbox.position.y) {
				min_y = hitbox.position.y;
			}
			if (max_y < hitbox.position.y + hitbox.dimension.y) {
				max_y = hitbox.position.y + hitbox.dimension.y;
			}
		}

		qsf::vrectangle hitbox;
		hitbox.position.x = front_hitbox.position.x;
		hitbox.position.y = min_y;
		hitbox.dimension.y = max_y - min_y;
		auto back_hitbox = this->texts[index].back().visible_hitbox();
		hitbox.dimension.x = (back_hitbox.position.x + back_hitbox.dimension.x) - hitbox.position.x;

		if (this->texts[index].size() == 1 && this->texts[index].front().get_string().empty()) {
			hitbox.dimension.y = this->texts[index].back().get_character_size();
		}
		return hitbox;

	}
	void qsf::text_stream::centerize_line(qpl::size index) {
		auto hitbox = this->line_hitbox(index);
		
		for (auto& line : this->texts[index]) {
			auto diff = hitbox.dimension.y - line.visible_hitbox().dimension.y;
			line.set_position(qsf::vector2f(line.get_position().x, hitbox.position.y - line.offset().y) + qsf::vector2f(0, diff / 2));
		}
	}
	void qsf::text_stream::centerize_lines() {
		for (qpl::u32 i = 0; i < this->lines(); ++i) {
			this->centerize_line(i);
		}
	}
	qpl::size qsf::text_stream::size() const {
		qpl::size result = 0u;
		for (auto& i : this->texts) {
			result += i.size();
		}
		return result;
	}
	qpl::size qsf::text_stream::lines() const {
		return this->texts.size();
	}
	qsf::text& qsf::text_stream::operator[](qpl::size index) {
		qpl::size result = 0u;
		for (auto& i : this->texts) {
			if (index >= result && index < result + i.size()) {
				return i[index - result];
			}
			result += i.size();
		}
		return this->texts.back().back();
	}
	const qsf::text& qsf::text_stream::operator[](qpl::size index) const {
		qpl::size result = 0u;
		for (auto& i : this->texts) {
			if (index >= result && index < result + i.size()) {
				return i[index - result];
			}
			result += i.size();
		}
		return this->texts.back().back();
	}
	std::vector<qsf::text>& qsf::text_stream::line(qpl::size index) {
		return this->texts[index];
	}
	const std::vector<qsf::text>& qsf::text_stream::line(qpl::size index) const {
		return this->texts[index];
	}
	void qsf::text_stream::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		for (auto& i : this->texts) {
			for (auto& j : i) {
				j.draw(window, states);
			}
		}
	}
	void qsf::text_stream::set_font(const std::string& font) {
		for (auto& i : this->states) {
			i.font = font;
		}
	}
	void qsf::text_stream::set_color(qsf::rgb color) {
		for (auto& i : this->states) {
			i.color = color;
		}
	}
	void qsf::text_stream::set_style(qpl::u32 style) {
		for (auto& i : this->states) {
			i.style = style;
		}
	}
	void qsf::text_stream::set_character_size(qpl::u32 character_size) {
		for (auto& i : this->states) {
			i.character_size = character_size;
		}
	}
	void qsf::text_stream::set_outline_thickness(qpl::f32 thickness) {
		for (auto& i : this->states) {
			i.outline_thickness = thickness;
		}
	}
	void qsf::text_stream::set_outline_color(qsf::rgb color) {
		for (auto& i : this->states) {
			i.outline_color = color;
		}
	}
	void qsf::text_stream::set_letter_spacing(qpl::f32 spacing) {
		for (auto& i : this->states) {
			i.letter_spacing = spacing;
		}
	}
	void qsf::text_stream::set_position(qsf::vector2f position) {
		this->position = position;
	}
	qsf::vrectangle qsf::text_stream::get_visible_hitbox() const {
		if (!this->lines()) {
			return {};
		}
		qpl::size index = this->lines() - 1;
		while (index && (this->texts[index].size() == 1u && this->texts[index][0].get_string().empty())) {
			--index;
		}
		auto back_box = this->line_hitbox(index);
		
		qsf::vrectangle front_box;
		qpl::f32 max_x = 0;
		for (qpl::u32 i = 0u; i <= index; ++i) {
			auto hitbox = this->line_hitbox(i);
			if (!i) {
				front_box = this->line_hitbox(0u);
			}
			max_x = qpl::max(max_x, hitbox.dimension.x);
		}

		qsf::vrectangle result;
		result.position = front_box.position;
		result.dimension.y= (back_box.position.y + back_box.dimension.y) - result.position.y;
		result.dimension.x = max_x;
		return result;
	}


	void qsf::vrectangle::set_dimension(qsf::vector2f dimension) {
		this->dimension = dimension;
	}
	void qsf::vrectangle::set_position(qsf::vector2f position) {
		this->position = position;
	}
	void qsf::vrectangle::set_center(qsf::vector2f position) {
		this->position = position - this->dimension / 2;
	}
	void qsf::vrectangle::set_color(qsf::rgb color) {
		this->color = color;
	}
	void qsf::vrectangle::set_outline_thickness(qpl::f32 outline_thickness) {
		this->outline_thickness = outline_thickness;
	}
	void qsf::vrectangle::set_outline_color(qsf::rgb outline_color) {
		this->outline_color = outline_color;
	}
	qsf::vector2f qsf::vrectangle::get_dimension() const {
		return this->dimension;
	}
	qsf::vector2f qsf::vrectangle::get_position() const {
		return this->position;
	}
	qsf::vector2f qsf::vrectangle::get_center() const {
		return this->position + (this->dimension / 2);
	}
	qsf::rgb qsf::vrectangle::get_color() const {
		return this->color;
	}
	qpl::f32 qsf::vrectangle::get_outline_thickness() const {
		return this->outline_thickness;
	}
	qsf::rgb qsf::vrectangle::get_outline_color() const {
		return this->outline_thickness;
	}
	void qsf::vrectangle::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::rectangle = *this;
		qsf::detail::rectangle.draw(window, states);
	}

	std::string qsf::vrectangle::string() const {
		return qpl::to_string('[', this->position.string(), ", ", this->dimension.string(), ']');
	}

	void qsf::vrectangle::increase(qpl::f32 delta) {
		this->position -= qsf::vector2f(delta, delta) / 2;
		this->dimension += qsf::vector2f(delta, delta);
	}
	qsf::vrectangle qsf::vrectangle::increased(qpl::f32 delta) const {
		auto copy = *this;
		copy.increase(delta);
		return copy;
	}
	bool qsf::vrectangle::contains(qsf::vector2f position) const {
		return (position.x > this->position.x && position.x < (this->position.x + this->dimension.x) &&
			position.y > this->position.y && position.y < (this->position.y + this->dimension.y));
	}


	void qsf::rectangle::set_dimension(qsf::vector2f dimension) {
		this->m_rect.setSize(dimension);
	}
	void qsf::rectangle::set_position(qsf::vector2f position) {
		this->m_rect.setPosition(position);
	}
	void qsf::rectangle::set_center(qsf::vector2f position) {
		this->m_rect.setPosition(position - this->get_dimension() / 2);
	}
	void qsf::rectangle::set_color(qsf::rgb color) {
		this->m_rect.setFillColor(color);
	}
	void qsf::rectangle::set_outline_thickness(qpl::f32 outline_thickness) {
		this->m_rect.setOutlineThickness(outline_thickness);
	}
	void qsf::rectangle::set_outline_color(qsf::rgb color) {
		this->m_rect.setOutlineColor(color);
	}
	bool qsf::rectangle::contains(qsf::vector2f position) const {
		auto pos = this->get_position();
		auto dim = this->get_dimension();
		return (position.x > pos.x && position.x < (pos.x + dim.x) &&
			position.y > pos.y && position.y < (pos.y + dim.y));
	}
	void qsf::rectangle::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		window.draw(this->m_rect, states);
	}

	qsf::vector2f qsf::rectangle::get_position() const {
		return this->m_rect.getPosition();
	}
	qsf::vector2f qsf::rectangle::get_dimension() const {
		return this->m_rect.getSize();
	}
	qsf::rgb qsf::rectangle::get_color() const {
		return this->m_rect.getFillColor();
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
	qsf::vector2f qsf::rectangle::center() const {
		return this->get_position() + (this->get_dimension() / 2);
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
	void qsf::vrectangles::add_rectangle(qsf::vector2f position, qsf::vector2f dimension) {
		this->rectangles.push_back(qsf::vrectangle(position, dimension));
	}
	void qsf::vrectangles::add_rectangle(qsf::vector2f position, qsf::vector2f dimension, qsf::rgb color) {
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



	vpoint& qsf::vpoint::operator=(qsf::vector2f position) {
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
	void qsf::vpoints::add_point(qsf::vector2f position, qsf::rgb color) {
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



	void qsf::vcircle::set_position(qsf::vector2f position) {
		this->point.position = position;
	}
	void qsf::vcircle::set_radius(qpl::f32 radius) {
		this->radius = radius;
	}
	void qsf::vcircle::set_color(qsf::rgb color) {
		this->point.color = color;
	}
	void qsf::vcircle::set_center(qsf::vector2f center) {
		this->point.position = center - qsf::vector2f{ this->radius, this->radius };
	}
	qsf::vector2f qsf::vcircle::center() const {
		return this->point.position + qsf::vector2f{ this->radius, this->radius };
	}
	void qsf::vcircle::centerize() {
		this->point.position -= qsf::vector2f{ this->radius, this->radius };
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
		return *this;
	}
	qsf::circle& qsf::circle::operator=(const qsf::circle& circle) {
		this->circle_shape.setPosition(circle.circle_shape.getPosition());
		this->circle_shape.setRadius(circle.circle_shape.getRadius());
		this->circle_shape.setFillColor(circle.circle_shape.getFillColor());
		return *this;
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
	void qsf::vcircles::add_circle(qsf::vpoint point, qpl::f32 radius, qsf::rgb color) {
		this->circles.push_back(qsf::vcircle(point.position, radius, color));
	}
	void qsf::vcircles::add_circle(qsf::vector2f position, qpl::f32 radius, qsf::rgb color) {
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
	void qsf::circles::add_circle(qsf::vpoint point, qpl::f32 radius, qsf::rgb color) {
		this->circles_.push_back(qsf::vcircle(point.position, radius, color));
	}
	void qsf::circles::add_circle(qsf::vector2f position, qpl::f32 radius, qsf::rgb color) {
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
	void qsf::vline::set_a(qsf::vector2f position) {
		this->a = position;
	}
	void qsf::vline::set_b(qsf::vpoint point) {
		this->b = point.position;
	}
	void qsf::vline::set_b(qsf::vector2f position) {
		this->b = position;
	}
	void qsf::vline::set_color(qsf::rgb color) {
		this->a.color = this->b.color = color;
	}
	void qsf::vline::set_a_color(qsf::rgb color) {
		this->a.color = color;
	}
	void qsf::vline::set_b_color(qsf::rgb color) {
		this->b.color = color;
	}
	qpl::f32 qsf::vline::length() const {
		auto diff = this->b.position - this->a.position;
		return std::sqrt(diff.x * diff.x + diff.y * diff.y);
	}
	qsf::vector2f qsf::vline::normal() const {
		return qsf::vector2f{ this->a.position.y - this->b.position.y, this->b.position.x - this->a.position.x } / this->length();
	}
	qpl::f32 qsf::vline::rotation() const {
		return std::asin((this->b.position.y - this->a.position.y) / this->length());
	}
	qsf::vline& qsf::vline::rotate_around_a(qpl::f64 degree) {


		return *this;
	}
	qsf::vline& qsf::vline::rotate_around_b(qpl::f64 degree) {
		//a / sin(a) = b / sin(b) = c / sin(c)

		//(x, y) + 
		return *this;
	}
	void qsf::vline::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::line = *this;
		qsf::detail::line.draw(window, states);
	}

	void qsf::line::set_a(qsf::vpoint point) {
		this->vertices[0].position = point.position;
	}
	void qsf::line::set_a(qsf::vector2f position) {
		this->vertices[0].position = position;
	}
	void qsf::line::set_b(qsf::vpoint point) {
		this->vertices[1].position = point.position;
	}
	void qsf::line::set_b(qsf::vector2f position) {
		this->vertices[1].position = position;
	}
	void qsf::line::set_color(qsf::rgb color) {
		this->vertices[0].color = this->vertices[1].color = color;
	}
	void qsf::line::set_a_color(qsf::rgb color) {
		this->vertices[0].color = color;
	}
	void qsf::line::set_b_color(qsf::rgb color) {
		this->vertices[1].color = color;
	}
	qsf::vector2f qsf::line::normal() const {
		return qsf::vector2f{ this->vertices[0].position.y - this->vertices[1].position.y, this->vertices[1].position.x - this->vertices[0].position.x } / this->length();
	}
	qpl::f32 qsf::line::length() const {
		qsf::vector2f diff = this->vertices[0].position - this->vertices[1].position;
		return diff.normal();
	}	
	qpl::f32 qsf::line::angle_from_a() const {
		auto angle = std::atan2(this->vertices[1].position.y - this->vertices[0].position.y, this->vertices[1].position.x - this->vertices[0].position.x);
		if (angle < 0) {
			angle += qpl::pi * 2;
		}
		return angle;
	}
	qpl::f32 qsf::line::angle_from_b() const {
		auto angle = std::atan2(this->vertices[0].position.y - this->vertices[1].position.y, this->vertices[0].position.x - this->vertices[1].position.x);
		if (angle < 0) {
			angle += qpl::pi * 2;
		}
		return angle;
	}
	//qsf::vline& qsf::line::rotate_a(qpl::f64 degree) {
	//	return *this;
	//}
	//qsf::vline& qsf::line::rotate_b(qpl::f64 degree) {
		//a / sin(a) = b / sin(b) = c / sin(c)

		//(x, y) + 
	//}

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
	void qsf::vlines::add_point(qsf::vector2f position, qsf::rgb color) {
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
	void qsf::lines::add_point(qsf::vector2f position, qsf::rgb color) {
		this->vertices.add(qsf::vertex(position, color));
	}
	void qsf::lines::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		this->vertices.draw(window, states);
	}

	void qsf::vthick_line::set_a(qsf::vpoint point) {
		this->a = point.position;
	}
	void qsf::vthick_line::set_a(qsf::vector2f position) {
		this->a = position;
	}
	void qsf::vthick_line::set_b(qsf::vpoint point) {
		this->b = point.position;
	}
	void qsf::vthick_line::set_b(qsf::vector2f position) {
		this->b = position;
	}
	void qsf::vthick_line::set_color(qsf::rgb color) {
		this->a.color = this->b.color = color;
	}
	void qsf::vthick_line::set_a_color(qsf::rgb color) {
		this->a.color = color;
	}
	void qsf::vthick_line::set_b_color(qsf::rgb color) {
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
	qsf::vector2f qsf::vthick_line::normal() const {
		return qsf::vector2f{ this->a.position.y - this->b.position.y, this->b.position.x - this->a.position.x } / this->length();
	}
	void qsf::vthick_line::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		qsf::detail::thick_line = *this;
		qsf::detail::thick_line.draw(window, states);
	}

	void qsf::thick_line::set_a(qsf::vpoint point) {
		this->vertices[0].position = this->vertices[1].position = point.position;
		this->vertices[0].color = this->vertices[1].color = point.color;
	}
	void qsf::thick_line::set_a(qsf::vector2f position) {
		this->vertices[0].position = this->vertices[1].position = position;
	}
	void qsf::thick_line::set_b(qsf::vpoint point) {
		this->vertices[2].position = this->vertices[3].position = point.position;
		this->vertices[2].color = this->vertices[3].color = point.color;
	}
	void qsf::thick_line::set_b(qsf::vector2f position) {
		this->vertices[2].position = this->vertices[3].position = position;
	}
	void qsf::thick_line::set_color(qsf::rgb color) {
		this->vertices[0].color = this->vertices[1].color = this->vertices[2].color = this->vertices[3].color = color;
	}
	void qsf::thick_line::set_a_color(qsf::rgb color) {
		this->vertices[0].color = this->vertices[1].color = color;
	}
	void qsf::thick_line::set_b_color(qsf::rgb color) {
		this->vertices[2].color = this->vertices[3].color = color;
	}
	void qsf::thick_line::set_thickness(qpl::f32 thickness) {
		auto normal = this->normal();

		this->vertices[0].position = qsf::vector2f(this->vertices[0].position) - normal * thickness;
		this->vertices[1].position = qsf::vector2f(this->vertices[0].position) + normal * thickness;
		this->vertices[2].position = qsf::vector2f(this->vertices[2].position) + normal * thickness;
		this->vertices[3].position = qsf::vector2f(this->vertices[2].position) - normal * thickness;
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
	qsf::vector2f qsf::thick_line::normal() const {
		return qsf::vector2f{ this->vertices[0].position.y - this->vertices[2].position.y, this->vertices[2].position.x - this->vertices[0].position.x } / this->length();
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
	void qsf::vthick_lines::add_thick_line(qsf::vector2f position, qsf::rgb color) {
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
	void qsf::thick_lines::add_thick_line(qsf::vector2f position, qsf::rgb color, qpl::f32 thickness) {
		auto last_point = position;
		auto last_color = color;
		auto last_thickness = thickness;
		if (this->vertices.size()) {
			last_point = (this->vertices[this->vertices.size() - 1].position + this->vertices[this->vertices.size() - 2].position) / 2.0f;

			last_color.c.r = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.c.r) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.c.r)) / 2);
			last_color.c.g = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.c.g) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.c.g)) / 2);
			last_color.c.b = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.c.b) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.c.b)) / 2);
			last_color.c.a = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.c.a) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.c.a)) / 2);

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
		qsf::vector2f first_point;
		qsf::rgb first_color;
		qpl::f32 first_thickness;

		if (this->vertices.size()) {
			first_point = (this->vertices[0].position + this->vertices[1].position) / 2.0f;

			first_color.c.r = qpl::u8_cast((qpl::u32_cast(this->vertices[0].color.c.r) + qpl::u32_cast(this->vertices[1].color.c.r)) / 2);
			first_color.c.g = qpl::u8_cast((qpl::u32_cast(this->vertices[0].color.c.g) + qpl::u32_cast(this->vertices[1].color.c.g)) / 2);
			first_color.c.b = qpl::u8_cast((qpl::u32_cast(this->vertices[0].color.c.b) + qpl::u32_cast(this->vertices[1].color.c.b)) / 2);
			first_color.c.a = qpl::u8_cast((qpl::u32_cast(this->vertices[0].color.c.a) + qpl::u32_cast(this->vertices[1].color.c.a)) / 2);

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

			last_color.c.r = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.c.r) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.c.r)) / 2);
			last_color.c.g = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.c.g) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.c.g)) / 2);
			last_color.c.b = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.c.b) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.c.b)) / 2);
			last_color.c.a = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.c.a) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.c.a)) / 2);

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

		qsf::rgb mid_color;
		mid_color.c.r = qpl::u8_cast((qpl::u32_cast(this->vertices[8].color.c.r) + qpl::u32_cast(this->vertices[9].color.c.r)) / 2);
		mid_color.c.g = qpl::u8_cast((qpl::u32_cast(this->vertices[8].color.c.g) + qpl::u32_cast(this->vertices[9].color.c.g)) / 2);
		mid_color.c.b = qpl::u8_cast((qpl::u32_cast(this->vertices[8].color.c.b) + qpl::u32_cast(this->vertices[9].color.c.b)) / 2);
		mid_color.c.a = qpl::u8_cast((qpl::u32_cast(this->vertices[8].color.c.a) + qpl::u32_cast(this->vertices[9].color.c.a)) / 2);

		line.set_a(this->vertices[8].position);
		line.set_b(this->vertices[9].position);
		auto mid_thickness = line.length() / 2;

		last_point = (this->vertices[this->vertices.size() - 1].position + this->vertices[this->vertices.size() - 2].position) / 2.0f;

		last_color.c.r = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.c.r) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.c.r)) / 2);
		last_color.c.g = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.c.g) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.c.g)) / 2);
		last_color.c.b = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.c.b) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.c.b)) / 2);
		last_color.c.a = qpl::u8_cast((qpl::u32_cast(this->vertices[this->vertices.size() - 1].color.c.a) + qpl::u32_cast(this->vertices[this->vertices.size() - 2].color.c.a)) / 2);

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

	void qsf::pixel_image::set_array_dimension(qsf::vector2u dimension) {
		this->positions_set = this->array_dimension == dimension;
		this->array_dimension = dimension;
		this->vertices.resize((this->array_dimension.x - 1) * (this->array_dimension.y - 1) * 4);

		this->create_positions();
	}
	void qsf::pixel_image::set_pixel_dimension(qsf::vector2f dimension) {
		this->positions_set = this->pixel_dimension == dimension;
		this->pixel_dimension = dimension;
		this->create_positions();
	}
	void qsf::pixel_image::set_position(qsf::vector2f position) {
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

						this->vertices[index * 4 + 0].position = (this->position + this->pixel_dimension * qsf::vector2f(x, y));
						this->vertices[index * 4 + 1].position = (this->position + this->pixel_dimension * qsf::vector2f(x, y + 1.0f));
						this->vertices[index * 4 + 2].position = (this->position + this->pixel_dimension * qsf::vector2f(x + 1.0f, y + 1.0f));
						this->vertices[index * 4 + 3].position = (this->position + this->pixel_dimension * qsf::vector2f(x + 1.0f, y));
					}
				}
			}

		}
	}
	void qsf::pixel_image::set(qpl::size x, qpl::size y, qsf::rgb color) {
		auto index = (y * this->array_dimension.y + x);

		auto above_index = ((qpl::signed_cast(y) - 1) * (qpl::signed_cast(this->array_dimension.x) - 1) + qpl::signed_cast(x)) * 4;
		if (above_index - 2 >= 0 && above_index - 2 < this->vertices.size()) {
			this->vertices[above_index - 2].color = color;
		}
		if (above_index + 1 >= 0 && above_index + 1 < this->vertices.size()) {
			this->vertices[above_index + 1].color = color;
		}
		auto below_index = ((qpl::signed_cast(y)) * (qpl::signed_cast(this->array_dimension.x) - 1) + qpl::signed_cast(x)) * 4;
		if (below_index >= 0 && below_index < this->vertices.size()) {
			this->vertices[below_index].color = color;
		}
		if (below_index - 1 >= 0 && below_index - 1 < this->vertices.size()) {
			this->vertices[below_index - 1].color = color;
		}
	}
	void qsf::pixel_image::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		this->vertices.draw(window, states);
	}


	void qsf::tile_map::set_texture_ptr(const sf::Texture& texture, qsf::vector2u texture_tile_dimension) {
		this->texture_ptr = &texture;
		this->texture_tile_dimension = texture_tile_dimension;
		this->position_tile_dimension = this->texture_tile_dimension;
		this->texture_ptr_set = true;
	}
	void qsf::tile_map::set_texture_ptr(const sf::Texture& texture, qpl::u32 texture_tile_width) {
		this->set_texture_ptr(texture, qsf::vector2u(texture_tile_width, texture_tile_width));
	}

	void qsf::tile_map::create(const std::vector<std::pair<qpl::u32, qpl::u32>>& indices, qpl::size width, qsf::rgb color) {
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

		if (color == qsf::rgb::white) {
			for (qpl::u32 i = 0; i < indices.size(); ++i) {

				auto [y, x] = qpl::div_mod(i, width);
				auto tile = indices[i];
				auto tile_x = (tile.first % texture_row_tile_count);
				auto tile_y = (tile.first / texture_row_tile_count);

				auto chunk_x = (i % width) / this->max_chunk_size.x;
				auto chunk_y = (i / width) / this->max_chunk_size.y;

				auto& chunk = this->chunks[chunk_x + chunk_y * chunk_width];

				auto chunk_index = ((i % width) % this->max_chunk_size.x) + ((i / width) % this->max_chunk_size.y) * this->max_chunk_size.x;

				chunk[chunk_index * 4 + 0].position = qsf::vector2u(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 1].position = qsf::vector2u((x + 1) * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 2].position = qsf::vector2u((x + 1) * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 3].position = qsf::vector2u(x * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);

				auto tex_0 = qsf::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_1 = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_2 = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				auto tex_3 = qsf::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);

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

				chunk[chunk_index * 4 + 0].position = qsf::vector2u(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 1].position = qsf::vector2u((x + 1) * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 2].position = qsf::vector2u((x + 1) * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 3].position = qsf::vector2u(x * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);

				chunk[chunk_index * 4 + 0].color = color;
				chunk[chunk_index * 4 + 1].color = color;
				chunk[chunk_index * 4 + 2].color = color;
				chunk[chunk_index * 4 + 3].color = color;

				auto tex_0 = qsf::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_1 = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_2 = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				auto tex_3 = qsf::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);

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
		this->create(indices, width, qsf::rgb::white);
	}


	void qsf::tile_map::create(const std::vector<std::pair<qpl::u32, qpl::f32>>& indices, qpl::size width, qsf::rgb color) {
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

		if (color == qsf::rgb::white) {
			for (qpl::u32 i = 0; i < indices.size(); ++i) {

				auto [y, x] = qpl::div_mod(i, width);
				auto tile = indices[i];
				auto tile_x = (tile.first % texture_row_tile_count);
				auto tile_y = (tile.first / texture_row_tile_count);

				auto chunk_x = (i % width) / this->max_chunk_size.x;
				auto chunk_y = (i / width) / this->max_chunk_size.y;

				auto& chunk = this->chunks[chunk_x + chunk_y * chunk_width];

				auto chunk_index = ((i % width) % this->max_chunk_size.x) + ((i / width) % this->max_chunk_size.y) * this->max_chunk_size.x;

				auto middle = qsf::vector2f(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y) + this->position_tile_dimension / 2;

				auto diagonal = std::sqrt(std::pow(this->position_tile_dimension.x, 2) + std::pow(this->position_tile_dimension.y, 2));
				auto angle = tile.second + (qpl::pi * 1.25);
				auto pos_1 = middle + qsf::vector2f(std::cos(angle + (qpl::pi * 0.0)), std::sin(angle + (qpl::pi * 0.0))) * diagonal / 2;
				auto pos_2 = middle + qsf::vector2f(std::cos(angle + (qpl::pi * 0.5)), std::sin(angle + (qpl::pi * 0.5))) * diagonal / 2;
				auto pos_3 = middle + qsf::vector2f(std::cos(angle + (qpl::pi * 1.0)), std::sin(angle + (qpl::pi * 1.0))) * diagonal / 2;
				auto pos_4 = middle + qsf::vector2f(std::cos(angle + (qpl::pi * 1.5)), std::sin(angle + (qpl::pi * 1.5))) * diagonal / 2;

				chunk[chunk_index * 4 + 0].position = pos_1;
				chunk[chunk_index * 4 + 1].position = pos_2;
				chunk[chunk_index * 4 + 2].position = pos_3;
				chunk[chunk_index * 4 + 3].position = pos_4;

				auto tex_0 = qsf::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_1 = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_2 = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				auto tex_3 = qsf::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);

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

				auto middle = qsf::vector2f(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y) + this->position_tile_dimension / 2;

				auto diagonal = std::sqrt(std::pow(this->position_tile_dimension.x, 2) + std::pow(this->position_tile_dimension.y, 2));
				auto angle = tile.second + (qpl::pi * 1.25);
				auto pos_1 = middle + qsf::vector2f(std::cos(angle + (qpl::pi * 0.0)), std::sin(angle + (qpl::pi * 0.0))) * diagonal / 2;
				auto pos_2 = middle + qsf::vector2f(std::cos(angle + (qpl::pi * 0.5)), std::sin(angle + (qpl::pi * 0.5))) * diagonal / 2;
				auto pos_3 = middle + qsf::vector2f(std::cos(angle + (qpl::pi * 1.0)), std::sin(angle + (qpl::pi * 1.0))) * diagonal / 2;
				auto pos_4 = middle + qsf::vector2f(std::cos(angle + (qpl::pi * 1.5)), std::sin(angle + (qpl::pi * 1.5))) * diagonal / 2;

				chunk[chunk_index * 4 + 0].position = pos_1;
				chunk[chunk_index * 4 + 1].position = pos_2;
				chunk[chunk_index * 4 + 2].position = pos_3;
				chunk[chunk_index * 4 + 3].position = pos_4;

				chunk[chunk_index * 4 + 0].color = color;
				chunk[chunk_index * 4 + 1].color = color;
				chunk[chunk_index * 4 + 2].color = color;
				chunk[chunk_index * 4 + 3].color = color;

				auto tex_0 = qsf::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_1 = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_2 = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				auto tex_3 = qsf::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);

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
		this->create(indices, width, qsf::rgb::white);
	}
	void qsf::tile_map::create(const std::vector<qpl::u32>& indices, qpl::size width, qsf::rgb color) {
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

		if (color == qsf::rgb::white) {
			for (qpl::u32 i = 0; i < indices.size(); ++i) {
				auto [y, x] = qpl::div_mod(i, width);
				auto index = indices[i];
				auto tile_x = (index % texture_row_tile_count);
				auto tile_y = (index / texture_row_tile_count);


				auto chunk_x = (i % width) / this->max_chunk_size.x;
				auto chunk_y = (i / width) / this->max_chunk_size.y;

				auto& chunk = this->chunks[chunk_x + chunk_y * chunk_width];

				auto chunk_index = ((i % width) % this->max_chunk_size.x) + ((i / width) % this->max_chunk_size.y) * this->max_chunk_size.x;

				chunk[chunk_index * 4 + 0].position = qsf::vector2u(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 1].position = qsf::vector2u((x + 1) * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 2].position = qsf::vector2u((x + 1) * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 3].position = qsf::vector2u(x * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);

				chunk[chunk_index * 4 + 0].texCoords = qsf::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				chunk[chunk_index * 4 + 1].texCoords = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				chunk[chunk_index * 4 + 2].texCoords = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				chunk[chunk_index * 4 + 3].texCoords = qsf::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
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

				chunk[chunk_index * 4 + 0].position = qsf::vector2u(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 1].position = qsf::vector2u((x + 1) * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 2].position = qsf::vector2u((x + 1) * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);
				chunk[chunk_index * 4 + 3].position = qsf::vector2u(x * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);

				chunk[chunk_index * 4 + 0].color = color;
				chunk[chunk_index * 4 + 1].color = color;
				chunk[chunk_index * 4 + 2].color = color;
				chunk[chunk_index * 4 + 3].color = color;

				chunk[chunk_index * 4 + 0].texCoords = qsf::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				chunk[chunk_index * 4 + 1].texCoords = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				chunk[chunk_index * 4 + 2].texCoords = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				chunk[chunk_index * 4 + 3].texCoords = qsf::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
			}
		}
		
	}
	void qsf::tile_map::create(const std::vector<qpl::u32>& indices, qpl::size width) {
		this->create(indices, width, qsf::rgb::white);
	}


	void qsf::tile_map::create_skip_empty(const std::vector<std::pair<qpl::u32, qpl::u32>>& indices, qpl::size width, qsf::rgb color, qpl::u32 skip_index) {
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

		if (color == qsf::rgb::white) {
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


				auto pos_0 = qsf::vector2u(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				auto pos_1 = qsf::vector2u((x + 1) * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				auto pos_2 = qsf::vector2u((x + 1) * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);
				auto pos_3 = qsf::vector2u(x * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);

				chunk[ctr + 0].position = pos_0;
				chunk[ctr + 1].position = pos_1;
				chunk[ctr + 2].position = pos_2;
				chunk[ctr + 3].position = pos_3;

				auto tex_0 = qsf::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_1 = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_2 = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				auto tex_3 = qsf::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);

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

				auto pos_0 = qsf::vector2u(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				auto pos_1 = qsf::vector2u((x + 1) * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				auto pos_2 = qsf::vector2u((x + 1) * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);
				auto pos_3 = qsf::vector2u(x * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);

				chunk[ctr + 0].position = pos_0;
				chunk[ctr + 1].position = pos_1;
				chunk[ctr + 2].position = pos_2;
				chunk[ctr + 3].position = pos_3;
						  
				chunk[ctr + 0].color = color;
				chunk[ctr + 1].color = color;
				chunk[ctr + 2].color = color;
				chunk[ctr + 3].color = color;

				auto tex_0 = qsf::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_1 = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_2 = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				auto tex_3 = qsf::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);

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
		this->create_skip_empty(indices, width, qsf::rgb::white, skip_index);
	}

	void qsf::tile_map::create_skip_empty(const std::vector<std::pair<qpl::u32, qpl::f32>>& indices, qpl::size width, qsf::rgb color, qpl::u32 skip_index) {
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

		if (color == qsf::rgb::white) {
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

				auto middle = qsf::vector2f(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y) + this->position_tile_dimension / 2;

				auto angle = ((tile.second / 180) * qpl::pi) + (qpl::pi * 1.25);
				auto pos_1 = middle + qsf::vector2f(std::cos(angle + (qpl::pi * 0.0)), std::sin(angle + (qpl::pi * 0.0))) * diagonal / 2;
				auto pos_2 = middle + qsf::vector2f(std::cos(angle + (qpl::pi * 0.5)), std::sin(angle + (qpl::pi * 0.5))) * diagonal / 2;
				auto pos_3 = middle + qsf::vector2f(std::cos(angle + (qpl::pi * 1.0)), std::sin(angle + (qpl::pi * 1.0))) * diagonal / 2;
				auto pos_4 = middle + qsf::vector2f(std::cos(angle + (qpl::pi * 1.5)), std::sin(angle + (qpl::pi * 1.5))) * diagonal / 2;

				chunk[ctr + 0].position = pos_1;
				chunk[ctr + 1].position = pos_2;
				chunk[ctr + 2].position = pos_3;
				chunk[ctr + 3].position = pos_4;

				auto tex_0 = qsf::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_1 = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_2 = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				auto tex_3 = qsf::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);

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

				auto middle = qsf::vector2f(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y) + this->position_tile_dimension / 2;

				auto angle = tile.second + (qpl::pi * 1.25);
				auto pos_1 = middle + qsf::vector2f(std::cos(angle + (qpl::pi * 0.0)), std::sin(angle + (qpl::pi * 0.0))) * diagonal / 2;
				auto pos_2 = middle + qsf::vector2f(std::cos(angle + (qpl::pi * 0.5)), std::sin(angle + (qpl::pi * 0.5))) * diagonal / 2;
				auto pos_3 = middle + qsf::vector2f(std::cos(angle + (qpl::pi * 1.0)), std::sin(angle + (qpl::pi * 1.0))) * diagonal / 2;
				auto pos_4 = middle + qsf::vector2f(std::cos(angle + (qpl::pi * 1.5)), std::sin(angle + (qpl::pi * 1.5))) * diagonal / 2;

				chunk[ctr + 0].position = pos_1;
				chunk[ctr + 1].position = pos_2;
				chunk[ctr + 2].position = pos_3;
				chunk[ctr + 3].position = pos_4;

				chunk[ctr + 0].color = color;
				chunk[ctr + 1].color = color;
				chunk[ctr + 2].color = color;
				chunk[ctr + 3].color = color;

				auto tex_0 = qsf::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_1 = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				auto tex_2 = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				auto tex_3 = qsf::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);

				chunk[ctr + 0].texCoords = tex_0;
				chunk[ctr + 1].texCoords = tex_1;
				chunk[ctr + 2].texCoords = tex_2;
				chunk[ctr + 3].texCoords = tex_3;
				
			}
		}

	}
	void qsf::tile_map::create_skip_empty(const std::vector<std::pair<qpl::u32, qpl::f32>>& indices, qpl::size width, qpl::u32 skip_index) {
		this->create_skip_empty(indices, width, qsf::rgb::white, skip_index);
	}
	void qsf::tile_map::create_skip_empty(const std::vector<qpl::u32>& indices, qpl::size width, qsf::rgb color, qpl::u32 skip_index) {
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
		if (color == qsf::rgb::white) {
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

				chunk[ctr + 0].position = qsf::vector2u(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[ctr + 1].position = qsf::vector2u((x + 1) * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[ctr + 2].position = qsf::vector2u((x + 1) * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);
				chunk[ctr + 3].position = qsf::vector2u(x * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);

				chunk[ctr + 0].texCoords = qsf::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				chunk[ctr + 1].texCoords = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				chunk[ctr + 2].texCoords = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				chunk[ctr + 3].texCoords = qsf::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
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

				chunk[ctr + 0].position = qsf::vector2u(x * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[ctr + 1].position = qsf::vector2u((x + 1) * this->position_tile_dimension.x, y * this->position_tile_dimension.y);
				chunk[ctr + 2].position = qsf::vector2u((x + 1) * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);
				chunk[ctr + 3].position = qsf::vector2u(x * this->position_tile_dimension.x, (y + 1) * this->position_tile_dimension.y);

				chunk[ctr + 0].color = color;
				chunk[ctr + 1].color = color;
				chunk[ctr + 2].color = color;
				chunk[ctr + 3].color = color;

				chunk[ctr + 0].texCoords = qsf::vector2u(tile_x * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				chunk[ctr + 1].texCoords = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, tile_y * this->texture_tile_dimension.y);
				chunk[ctr + 2].texCoords = qsf::vector2u((tile_x + 1) * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
				chunk[ctr + 3].texCoords = qsf::vector2u(tile_x * this->texture_tile_dimension.x, (tile_y + 1) * this->texture_tile_dimension.y);
			}
		}

	}
	void qsf::tile_map::create_skip_empty(const std::vector<qpl::u32>& indices, qpl::size width, qpl::u32 skip_index) {
		this->create_skip_empty(indices, width, qsf::rgb::white, skip_index);
	}

	void qsf::tile_map::set_color(qsf::rgb color) {
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

			auto rx = x * this->max_chunk_size.x * this->texture_tile_dimension.x;
			auto ry = y * this->max_chunk_size.y * this->texture_tile_dimension.y;
			auto rw = this->max_chunk_size.x * this->texture_tile_dimension.x;
			auto rh = this->max_chunk_size.y * this->texture_tile_dimension.y;

			auto rect = states.transform.transformRect(sf::FloatRect(rx, ry, rw, rh));
			auto fov = sf::FloatRect(0, 0, window.getSize().x, window.getSize().y);


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
		this->max_chunk_size = qsf::vector2u(x, y);
	}
	void qsf::tile_map::clear() {
		this->chunks.clear();
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
	void qsf::vgraph::info_graph::set_text_color(qsf::rgb text_color) {
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
	void qsf::vgraph::info_graph::set_text_outline_color(qsf::rgb color) {
		this->text.outline_color = color;
	}

	qpl::f64 qsf::vgraph::info_graph::get_percentage_height_at(qpl::size index) const {
		auto [low, high] = this->get_low_high();

		return (this->data[index].data - low.data) / (high.data - low.data);
	}
	void qsf::vgraph::info_graph::add_data(qpl::f64 data) {
		this->add_data(data, this->color, this->thickness);
	}
	void qsf::vgraph::info_graph::add_data(qpl::f64 data, qsf::rgb color) {
		this->add_data(data, color, this->thickness);
	}
	void qsf::vgraph::info_graph::add_data(qpl::f64 data, qsf::rgb color, qpl::f64 thickness) {
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
	void qsf::vgraph::info_graph::add_data(qpl::f64 data, std::string string, qsf::rgb color) {
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
	void qsf::vgraph::standard_graph::add_data(qpl::f64 data, qsf::rgb color) {
		this->add_data(data, color, this->thickness);
	}
	void qsf::vgraph::standard_graph::add_data(qpl::f64 data, qsf::rgb color, qpl::f64 thickness) {
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
			rect.position.x += this->y_axis_text_space;

			if (rect.contains(event_info.mouse_position())) {
				this->drag = true;
				this->click_position = event_info.mouse_position();
			}
		}

		if (event_info.mouse_moved()) {
			this->mouse_position = event_info.mouse_position();

			if (this->drag) {
				auto diff = this->click_position - event_info.mouse_position();

				auto size = this->visible_element_size();
				auto index_f = (diff.x / this->true_graph_width()) * size;
				auto index_delta = qpl::i32_cast(index_f);

				if (index_delta) {
					this->click_position = event_info.mouse_position();
					this->click_position.x += ((index_f - index_delta) / size) * this->true_graph_width();

					this->index_start = qpl::max(qpl::i32_cast(0), qpl::i32_cast(this->visible_index_start()) + index_delta);
					this->index_end = this->index_start + size;
					auto over = qpl::i32_cast(this->index_end) - qpl::i32_cast(this->graph_element_size());
					if (this->index_end >= this->graph_element_size()) {
						this->index_end = this->graph_element_size();
						this->index_start -= over;
					}
				}
			}
			if (this->display_closest_graph_at_cursor) {

				this->update_change();

				qsf::vgraph::interpolation_type interpolation = qsf::vgraph::interpolation_type::unset;

				auto visible_size = this->visible_element_size() - 1;
				auto offset = this->position.x + this->y_axis_text_space;
				auto progress = (event_info.mouse_position().x - offset) / this->true_graph_width();
				auto index = visible_size * progress;
				auto int_index = qpl::int_cast(index);
				auto left_over = index - int_index;
				auto [low, high] = this->low_high_graph;

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

						auto y_progress = (value - low) / (high - low);
						y_progress = qpl::clamp_0_1((1.0 - y_progress) * (1.0 - this->height_delta) + (this->height_delta) / 2);
						auto pos_y = this->position.y + this->dimension.y * y_progress;

						auto distance = qpl::abs(pos_y - event_info.mouse_position().y);

						if (min_distance > distance && distance < this->closest_graph_at_cursor_distance) {
							min_distance = distance;
							this->closest_graph_at_cursor = g.first;
							this->closest_graph_at_cursor_value = value;
							this->closest_graph_at_cursor_color = g.second.color;
							this->closest_graph_at_cursor_index = qpl::int_cast(this->visible_index_start() + index + 0.5);
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

						auto y_progress = (value - low) / (high - low);
						y_progress = qpl::clamp_0_1((1.0 - y_progress) * (1.0 - this->height_delta) + (this->height_delta) / 2);
						auto pos_y = this->position.y + this->dimension.y * y_progress;

						auto distance = qpl::abs(pos_y - event_info.mouse_position().y);

						if (min_distance > distance && distance < this->closest_graph_at_cursor_distance) {
							min_distance = distance;
							this->closest_graph_at_cursor = g.first;
							this->closest_graph_at_cursor_value = value;
							this->closest_graph_at_cursor_color = g.second.color;
							this->closest_graph_at_cursor_index = qpl::int_cast(this->visible_index_start() + index + 0.5);
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

						auto y_progress = (value - low) / (high - low);
						y_progress = qpl::clamp_0_1((1.0 - y_progress) * (1.0 - this->height_delta) + (this->height_delta) / 2);
						auto pos_y = this->position.y + this->dimension.y * y_progress;

						auto distance = qpl::abs(pos_y - event_info.mouse_position().y);

						if (min_distance > distance && distance < this->closest_graph_at_cursor_distance) {
							min_distance = distance;
							this->closest_graph_at_cursor = g.first;
							this->closest_graph_at_cursor_value = value;
							this->closest_graph_at_cursor_color = g.second.color;
							this->closest_graph_at_cursor_index = qpl::int_cast(this->visible_index_start() + index + 0.5);
							if (this->closest_graph_at_cursor_color.is_unset()) {
								this->closest_graph_at_cursor_color = this->color;
							}
						}
					}
				}
				
			}
		}
		if (event_info.left_mouse_released()) {
			this->drag = false;
		}
		if (event_info.scrolled_up()) {
			qsf::vrectangle rect(this->position, this->true_graph_dimension());
			rect.position.x += this->y_axis_text_space;

			if (rect.contains(event_info.mouse_position())) {
				auto progress = (event_info.mouse_position().x - rect.position.x) / rect.dimension.x;
				auto change = this->visible_element_size();
				auto start = qpl::i32_cast(this->visible_index_start());
				auto end = qpl::i32_cast(this->visible_index_end());

				change = qpl::size_cast(qpl::f64_cast(change) * (this->zoom_factor - 1));

				if (change < 5) {
					change = 0;
				}

				start += qpl::i32_cast(change * progress);
				end -= qpl::i32_cast(change * (1 - progress));

				if (start >= this->graph_element_size()) {
					start = this->graph_element_size() - 1;
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
			rect.position.x += this->y_axis_text_space;

			if (rect.contains(event_info.mouse_position())) {
				auto progress = (event_info.mouse_position().x - rect.position.x) / rect.dimension.x;
				auto change = this->visible_element_size();
				auto start = qpl::i32_cast(this->visible_index_start());
				auto end = qpl::i32_cast(this->visible_index_end());

				change = qpl::size_cast(qpl::f64_cast(change) * (this->zoom_factor - 1));
				if (change == 0) {
					change = 1;
				}

				start -= qpl::i32_cast(change * progress);
				end += qpl::i32_cast(change * (1 - progress));

				if (end >= this->graph_element_size()) {
					end = this->graph_element_size() - 1;
				}
				if (start < 0) {
					start = 0;
				}
				this->index_start = qpl::u32_cast(start);
				this->index_end = qpl::u32_cast(end);

				this->check_x_axis();
			}
		}
	}

	void qsf::vgraph::set_font(std::string name) {
		this->font = name;
		this->enable_axis_info();
	}
	std::string qsf::vgraph::get_font() const {
		return this->font;
	}

	qpl::f64 qsf::vgraph::true_graph_width() const {
		return this->dimension.x - this->y_axis_text_space;
	}
	qpl::f64 qsf::vgraph::true_graph_height() const {
		return this->dimension.y - this->x_axis_text_space;
	}
	qsf::vector2f qsf::vgraph::true_graph_dimension() const {
		return qsf::vector2f(this->true_graph_width(), this->true_graph_height());
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
		if (low < this->min_value) {
			low = this->min_value;
		}
		if (high > this->max_value) {
			high = this->max_value;
		}
		if (this->y_axis_start_at_0) {
			if (low < 0 && high < 0) {
				high = 0;
			}
			if (low > 0 && high > 0) {
				low = 0;
			}
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
					throw std::exception(qpl::to_cstring("qsf::vgraph: graphs have different element sizes (", i.first, ")"));
				}
			}
		}
		for (auto& i : this->standard_graphs) {
			if (result == qpl::size_max) {
				result = i.second.size();
			}
			else {
				if (result != i.second.size()) {
					throw std::exception(qpl::to_cstring("qsf::vgraph: graphs have different element sizes (", i.first, ")"));
				}
			}
		}
		for (auto& i : this->simple_graphs) {
			if (result == qpl::size_max) {
				result = i.second.size();
			}
			else {
				if (result != i.second.size()) {
					throw std::exception(qpl::to_cstring("qsf::vgraph: graphs have different element sizes (", i.first, ")"));
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
		else{
			return this->graph_element_size();
		}
	}
	qpl::u32 qsf::vgraph::visible_index_start() const {
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
	qpl::u32 qsf::vgraph::visible_index_end() const {
		if (this->is_range_enabled()) {
			return this->index_end;
		}
		else if (this->display_last_n_entries) {
			auto size = this->visible_element_size();
			return this->visible_index_start() + size;
		}
		return this->visible_element_size();
	}
	std::pair<qpl::u32, qpl::u32> qsf::vgraph::visible_index_range() const {
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
	qpl::span<const qsf::vgraph::data_point_info> qsf::vgraph::get_info_graph_span(std::string name) const {
		auto start = this->visible_index_start();
		auto end = this->visible_index_end();

		auto begin = this->get_info_graph(name).data.begin();
		return qpl::make_span(begin + start, begin + end);
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
	qpl::span<const qsf::vgraph::data_point> qsf::vgraph::get_standard_graph_span(std::string name) const {
		auto start = this->visible_index_start();
		auto end = this->visible_index_end();

		auto begin = this->get_standard_graph(name).data.begin();
		return qpl::make_span(begin + start, begin + end);
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
	qpl::span<const qsf::vgraph::data_point_simple> qsf::vgraph::get_simple_graph_span(std::string name) const {
		auto start = this->visible_index_start();
		auto end = this->visible_index_end();

		auto begin = this->get_simple_graph(name).data.begin();
		return qpl::make_span(begin + start, begin + end);
	}

	qsf::graph& qsf::graph::operator=(const qsf::vgraph& graph) {
		if (graph.empty()) {
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

				if (!use_interpolated_color && span[i].color != qsf::frgb::unset) {
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
			qsf::frgb using_color = g.second.color;
			if (using_color == qsf::frgb::unset) {
				using_color = graph.color;
			}

			this->lines[u].clear();
			for (qpl::u32 i = 0u; i < interpolated_data.size(); ++i) {

				qsf::vector2f position;
				position.x = graph.position.x + (graph.dimension.x - graph.y_axis_text_space) * (i / static_cast<double>(interpolated_data.size() - 1)) + graph.y_axis_text_space;


				auto y_progress = (interpolated_data[i].data - low) / (high - low);
				y_progress = qpl::clamp_0_1((1.0 - y_progress) * (1.0 - graph.height_delta) + (graph.height_delta) / 2);
				position.y = graph.position.y + graph.dimension.y * y_progress;

				auto color = use_interpolated_color ? interpolated_data[i].color : using_color;
				auto thickness = use_interpolated_thickness ? interpolated_data[i].thickness : using_thickness;
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

				if (!use_interpolated_color && span[i].color != qsf::frgb::unset) {
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

			qsf::frgb using_color = g.second.color;
			if (using_color == qsf::frgb::unset) {
				using_color = graph.color;
			}


			this->lines[u].clear();
			for (qpl::u32 i = 0u; i < interpolated_data.size(); ++i) {

				qsf::vector2f position;
				position.x = graph.position.x + (graph.dimension.x - graph.y_axis_text_space) * (i / static_cast<double>(interpolated_data.size() - 1)) + graph.y_axis_text_space;


				auto y_progress = (interpolated_data[i].data - low) / (high - low);
				y_progress = qpl::clamp_0_1((1.0 - y_progress) * (1.0 - graph.height_delta) + (graph.height_delta) / 2);
				position.y = graph.position.y + graph.dimension.y * y_progress;

				auto color = use_interpolated_color ? interpolated_data[i].color : using_color;
				auto thickness = use_interpolated_thickness ? interpolated_data[i].thickness : using_thickness;
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

			qsf::frgb using_color = g.second.color;
			if (using_color == qsf::frgb::unset) {
				using_color = graph.color;
			}


			this->lines[u].clear();

			for (qpl::u32 i = 0u; i < interpolated_data.size(); ++i) {

				qsf::vector2f position;
				position.x = graph.position.x + (graph.dimension.x - graph.y_axis_text_space) * (i / static_cast<double>(interpolated_data.size() - 1)) + graph.y_axis_text_space;


				auto y_progress = (interpolated_data[i].data - low) / (high - low);
				y_progress = qpl::clamp_0_1((1.0 - y_progress) * (1.0 - graph.height_delta) + (graph.height_delta) / 2);
				position.y = graph.position.y + graph.dimension.y * y_progress;

				auto color = using_color;
				auto thickness = using_thickness;
				this->lines[u].add_thick_line(position, color, thickness);
			}


			++u;
		}


		if (u) {
			qsf::rgb use_color = graph.axis_color;


			if (graph.use_y_axis) {
				auto y_range = (high - low);
				if (high == qpl::f64_min && low == qpl::f64_max) {
					y_range = 0.0;
				}
				auto low_padded = low - y_range * graph.y_axis_padding;
				auto high_padded = high + y_range * graph.y_axis_padding;
				auto exponent = static_cast<qpl::i64>(std::log10(y_range)) - 1;
				auto multiply = std::pow(10, -exponent);
				y_range *= multiply;

				qpl::f64 y_delta = 0.0;
				qpl::f64 y_start = 0.0;
				qpl::f64 y_end = 0.0;
				qpl::u32 y_steps = 0;

				if (!used_font.empty() && y_range != 0) {
					y_delta = qpl::i64_cast(y_range) / (multiply * graph.y_axis_line_count);

					y_start = qpl::i64_cast(low_padded / y_delta + 1) * y_delta;
					y_end = qpl::i64_cast(high_padded / y_delta) * y_delta;
					y_steps = qpl::i64_cast((y_end - y_start) / y_delta) + 1;
				}

				this->y_lines.resize(y_steps);
				this->y_texts.resize(y_steps);

				for (qpl::u32 i = 0u; i < y_steps; ++i) {
					auto y_position = y_start + y_delta * i;
					auto y_progress = ((y_position)-low_padded) / (high_padded - low_padded);
					y_progress = (1.0 - y_progress) * (1.0 - graph.height_delta) + (graph.height_delta) / 2;
					qsf::vector2f position;
					position.x = graph.position.x;
					position.y = graph.position.y + graph.true_graph_height() * y_progress + graph.x_axis_text_space;



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
					this->y_lines[i].add_thick_line(a, graph.axis_line_color, graph.axis_thickness);
					this->y_lines[i].add_thick_line(b, graph.axis_line_color, graph.axis_thickness);
				}
			}
			else {
				this->y_lines.clear();
				this->y_texts.clear();
			}

			if (graph.use_x_axis) {
				auto index_mod = graph.x_axis_line_frequency - (graph.visible_index_start() % graph.x_axis_line_frequency) - 1;
				auto x_size = graph.visible_element_size() / graph.x_axis_line_frequency;
				this->x_lines.resize(x_size);
				this->x_texts.resize(x_size);

				for (qpl::u32 i = 0u; i < x_size; ++i) {
					auto progress = (((i * graph.x_axis_line_frequency) + index_mod)) / static_cast<double>(graph.visible_element_size() - 1);
					auto multiple = qpl::approximate_multiple_up(graph.visible_index_start(), qpl::u32_cast(graph.x_axis_line_frequency));
					if (graph.visible_index_start() == 0u) {
						multiple = qpl::u32_cast(graph.x_axis_line_frequency);
					}
					qpl::size index = multiple + qpl::size_cast(i * graph.x_axis_line_frequency);

					

					qsf::vector2f position;
					position.x = graph.position.x + (graph.true_graph_width()) * progress + graph.y_axis_text_space;
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
					a.y -= graph.x_axis_text_space;
					auto b = a;
					b.y = graph.position.y;

					this->x_lines[i].clear();
					this->x_lines[i].add_thick_line(a, graph.axis_line_color, graph.axis_thickness);
					this->x_lines[i].add_thick_line(b, graph.axis_line_color, graph.axis_thickness);
				}
			}
			else {
				this->x_lines.clear();
				this->x_texts.clear();
			}
		}

		this->cursor_graph_text.set_color(qsf::rgb::unset);
		if (graph.display_closest_graph_at_cursor && !graph.closest_graph_at_cursor.empty()) {
			this->cursor_graph_text = graph.closest_graph_at_cursor_text;
			if (this->cursor_graph_text.get_font().empty()) {
				this->cursor_graph_text.set_font(used_font);
			}
			if (graph.closest_graph_at_cursor_string_function) {
				this->cursor_graph_text.set_string(graph.closest_graph_at_cursor_string_function(graph.closest_graph_at_cursor, graph.closest_graph_at_cursor_value, graph.closest_graph_at_cursor_index));
			}
			else {
				this->cursor_graph_text.set_string(qpl::to_string(graph.closest_graph_at_cursor, " : ", graph.closest_graph_at_cursor_value));
			}
			this->cursor_graph_text.set_color(graph.closest_graph_at_cursor_color.with_alpha(255));
			this->cursor_graph_text.set_position(graph.mouse_position);

			auto hitbox = this->cursor_graph_text.visible_hitbox();
			this->cursor_graph_text.move(qsf::vector2f{ 15, -hitbox.dimension.y - 15 });
			hitbox = this->cursor_graph_text.visible_hitbox();
			auto diff = (hitbox.position.x + hitbox.dimension.x) - (graph.position.x + graph.dimension.x) + 15;
			if (diff > 0) {
				this->cursor_graph_text.move(qsf::vector2f{ -diff, 0 });
				hitbox = this->cursor_graph_text.visible_hitbox();
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
		if (!this->cursor_graph_text.get_color().is_unset()) {
			this->cursor_graph_background.draw(window, states);
			this->cursor_graph_text.draw(window, states);
		}
	}

	void qsf::vbutton::set_dimension(qsf::vector2f dimension) {
		this->background.dimension = dimension;
	}
	void qsf::vbutton::set_position(qsf::vector2f position) {
		this->background.position = position;
	}
	void qsf::vbutton::set_center(qsf::vector2f center) {
		this->background.set_center(center);
	}
	void qsf::vbutton::set_background_color(qsf::rgb color) {
		this->background.set_color(color);
		this->background_color = color;
	}
	void qsf::vbutton::set_hover_background_color(qsf::rgb color) {
		this->hover_background_color = color;
	}
	void qsf::vbutton::set_text_color(qsf::rgb color) {
		this->text.set_color(color);
		this->text_color = color;
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
	void qsf::vbutton::update(const event_info& event_info, qsf::view_rectangle rectangle) {
		auto pos = rectangle.mouse_position;

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

		return *this;
	}

	void qsf::button::draw(sf::RenderTarget& window, sf::RenderStates states) const {
		this->background.draw(window, states);
		this->text.draw(window, states);
	}
	qsf::button& qsf::button::operator=(const qsf::vbutton& button) {
		this->background = button.background;
		this->text = button.text;
		this->text.set_center(this->background.center());
		return *this;
	}

	void qsf::button::set_dimension(qsf::vector2f dimension) {
		this->background.set_dimension(dimension);
	}
	void qsf::button::set_position(qsf::vector2f position) {
		this->background.set_position(position);
	}
	void qsf::button::set_center(qsf::vector2f center) {
		this->background.set_center(center);
	}
	void qsf::button::set_background_color(qsf::rgb color) {
		this->background.set_color(color);
		this->background_color = color;
	}
	void qsf::button::set_hover_background_color(qsf::rgb color) {
		this->hover_background_color = color;
	}
	void qsf::button::set_text_color(qsf::rgb color) {
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
	void qsf::button::set_text(std::string text) {
		this->text.set_string(text);
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
	void qsf::button::update(const event_info& event_info, qsf::view_rectangle view) {
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