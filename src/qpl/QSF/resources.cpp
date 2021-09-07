#include <qpl/QSF/resources.hpp>

#ifndef QPL_NO_SFML
#include <qpl/string.hpp>

namespace qsf {
	void qsf::resources::add_font(const std::string& name, const std::string& path) {
		this->fonts[name].loadFromFile(path);
	}
	void qsf::resources::add_texture(const std::string& name, const std::string& path) {
		this->textures[name].loadFromFile(path);
	}
	void qsf::resources::add_sprite(const std::string& name, const std::string& path) {
		if (this->textures.find(name) == this->textures.cend()) {
			this->add_texture(name, path);
		}
		this->sprites[name].setTexture(this->textures[name]);
	}
	void qsf::resources::add_sprite(const std::string& name, sf::Texture& texture) {
		this->sprites[name].setTexture(texture);
	}
	bool qsf::resources::find_font(const std::string& name) const {
		return this->fonts.find(name) != this->fonts.cend();
	}
	bool qsf::resources::find_texture(const std::string& name) const {
		return this->textures.find(name) != this->textures.cend();
	}
	bool qsf::resources::find_sprite(const std::string& name) const {
		return this->sprites.find(name) != this->sprites.cend();
	}

	sf::Font& qsf::resources::get_font(const std::string& name) {
		if (this->fonts.find(name) == this->fonts.cend()) {
			throw std::runtime_error(qpl::to_string("couldn't find font with name \"", name, "\""));
		}
		return this->fonts[name];
	}
	sf::Texture& qsf::resources::get_texture(const std::string& name) {
		if (this->textures.find(name) == this->textures.cend()) {
			throw std::runtime_error(qpl::to_string("couldn't find texture with name \"", name, "\""));
		}
		return this->textures[name];
	}
	sf::Sprite& qsf::resources::get_sprite(const std::string& name) {
		if (this->sprites.find(name) == this->sprites.cend()) {
			throw std::runtime_error(qpl::to_string("couldn't find sprite with name \"", name, "\""));
		}
		return this->sprites[name];
	}

	const sf::Font& qsf::resources::get_font(const std::string& name) const {
		if (this->fonts.find(name) == this->fonts.cend()) {
			throw std::runtime_error(qpl::to_string("couldn't find font with name \"", name, "\""));
		}
		return this->fonts.at(name);
	}
	const sf::Texture& qsf::resources::get_texture(const std::string& name) const {
		if (this->textures.find(name) == this->textures.cend()) {
			throw std::runtime_error(qpl::to_string("couldn't find texture with name \"", name, "\""));
		}
		return this->textures.at(name);
	}
	const sf::Sprite& qsf::resources::get_sprite(const std::string& name) const {
		if (this->sprites.find(name) == this->sprites.cend()) {
			throw std::runtime_error(qpl::to_string("couldn't find sprite with name \"", name, "\""));
		}
		return this->sprites.at(name);
	}

	qsf::resources qsf::detail::resources;

	void qsf::load_font(sf::Font& font, const std::string& path) {
		if (!font.loadFromFile(path)) {
			throw std::exception(qpl::to_cstring("load font: couldn't find / load \"", path, "\""));
		}
	}
	void qsf::load_texture(sf::Texture& texture, const std::string& path) {
		if (!texture.loadFromFile(path)) {
			throw std::exception(qpl::to_cstring("load texture: couldn't find / load \"", path, "\""));
		}
	}
	void qsf::load_sound(sf::SoundBuffer& sound, const std::string& path) {
		if (!sound.loadFromFile(path)) {
			throw std::exception(qpl::to_cstring("load sound: couldn't find / load \"", path, "\""));
		}
	}

	void qsf::add_font(const std::string& name, const std::string& path) {
		qsf::detail::resources.add_font(name, path);
	}
	void qsf::add_texture(const std::string& name, const std::string& path) {
		qsf::detail::resources.add_texture(name, path);
	}
	void qsf::add_sprite(const std::string& name, const std::string& path) {
		qsf::detail::resources.add_sprite(name, path);
	}
	void qsf::add_sprite(const std::string& name, sf::Texture& texture) {
		qsf::detail::resources.add_sprite(name, texture);
	}

	sf::Font& qsf::get_font(const std::string& name) {
		return qsf::detail::resources.get_font(name);
	}
	sf::Texture& qsf::get_texture(const std::string& name) {
		return qsf::detail::resources.get_texture(name);
	}
	sf::Sprite& qsf::get_sprite(const std::string& name) {
		return qsf::detail::resources.get_sprite(name);
	}

	bool qsf::find_font(const std::string& name) {
		return qsf::detail::resources.find_font(name);
	}
	bool qsf::find_texture(const std::string& name) {
		return qsf::detail::resources.find_texture(name);
	}
	bool qsf::find_sprite(const std::string& name){
		return qsf::detail::resources.find_sprite(name);
	}
}
#endif