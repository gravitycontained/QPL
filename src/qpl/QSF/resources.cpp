#include <qpl/QSF/resources.hpp>

#if defined(QPL_USE_SFML) || defined(QPL_USE_ALL)
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

	void qsf::add_font(const std::string& name, const std::string& path) {
		qsf::detail::resources.add_font(name, path);
	}
	void qsf::add_texture(const std::string& name, const std::string& path) {
		qsf::detail::resources.add_texture(name, path);
	}
	void qsf::add_sprite(const std::string& name, const std::string& path) {
		qsf::detail::resources.add_sprite(name, path);
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
}
#endif