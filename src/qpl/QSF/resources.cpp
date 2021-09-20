#include <qpl/QSF/resources.hpp>

#if !defined (QPL_NO_SFML) || defined(QPL_USE_ALL)
#include <qpl/string.hpp>

namespace qsf {

	void qsf::load_font(sf::Font& font, const std::string& path) {
		if (!font.loadFromFile(path)) {
			throw std::exception(qpl::to_string("load font: couldn't find / load \"", path, "\"").c_str());
		}
	}
	void qsf::load_texture(sf::Texture& texture, const std::string& path, bool set_smooth) {
		if (!texture.loadFromFile(path)) {
			throw std::exception(qpl::to_string("load texture: couldn't find / load \"", path, "\"").c_str());
		}
		texture.setSmooth(set_smooth);
	}
	void qsf::load_sound(sf::SoundBuffer& sound, const std::string& path) {
		if (!sound.loadFromFile(path)) {
			throw std::exception(qpl::to_string("load sound: couldn't find / load \"", path, "\"").c_str());
		}
	}

	void qsf::resources::play_sound(const std::string& name, qpl::f32 volume, qpl::f32 speed) {

		if (!this->find_sound(name)) {
			throw std::exception(qpl::to_string("play_sound: couldn't find \"", name, "\"").c_str());
		}
		this->active_sounds.push_back(sf::Sound{});
		this->active_sounds.back().setBuffer(this->sounds[name]);
		this->active_sounds.back().setVolume(volume);
		this->active_sounds.back().setPitch(speed);
		this->active_sounds.back().play();
	}
	void qsf::resources::update_sounds() {
		std::vector<std::list<sf::Sound>::iterator> remove;

		for (auto it = this->active_sounds.begin(); it != this->active_sounds.end(); ++it) {
			if (it->getStatus() == sf::SoundSource::Status::Stopped) {
				remove.push_back(it);
			}
		}
		for (auto& i : remove) {
			this->active_sounds.erase(i);
		}
	}
	void qsf::resources::add_font(const std::string& name, const std::string& path) {
		qsf::load_font(this->fonts[name], path);
	}
	void qsf::resources::add_sound(const std::string& name, const std::string& path) {
		qsf::load_sound(this->sounds[name], path);
	}
	void qsf::resources::add_texture(const std::string& name, const std::string& path) {
		qsf::load_texture(this->textures[name], path);
	}
	void qsf::resources::add_sprite(const std::string& name, const std::string& path) {
		if (this->textures.find(name) == this->textures.cend()) {
			this->add_texture(name, path);
		}		
		qsf::load_texture(this->textures[name], path);
	}
	void qsf::resources::add_sprite(const std::string& name, sf::Texture& texture) {
		this->sprites[name].setTexture(texture);
	}
	bool qsf::resources::find_font(const std::string& name) const {
		return this->fonts.find(name) != this->fonts.cend();
	}
	bool qsf::resources::find_sound(const std::string& name) const {
		return this->sounds.find(name) != this->sounds.cend();
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
	sf::SoundBuffer& qsf::resources::get_sound(const std::string& name) {
		if (this->sounds.find(name) == this->sounds.cend()) {
			throw std::runtime_error(qpl::to_string("couldn't find sound with name \"", name, "\""));
		}
		return this->sounds[name];
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
	const sf::SoundBuffer& qsf::resources::get_sound(const std::string& name) const {
		if (this->sounds.find(name) == this->sounds.cend()) {
			throw std::runtime_error(qpl::to_string("couldn't find sound with name \"", name, "\""));
		}
		return this->sounds.at(name);
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


	void qsf::play_sound(const std::string& name, qpl::f32 volume, qpl::f32 speed) {
		qsf::detail::resources.play_sound(name, volume, speed);
	}	
	void qsf::update_sounds() {
		qsf::detail::resources.update_sounds();
	}
	void qsf::add_font(const std::string& name, const std::string& path) {
		qsf::detail::resources.add_font(name, path);
	}
	void qsf::add_sound(const std::string& name, const std::string& path) {
		qsf::detail::resources.add_sound(name, path);
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
	sf::SoundBuffer& qsf::get_sound(const std::string& name) {
		return qsf::detail::resources.get_sound(name);
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
	bool qsf::find_sound(const std::string& name) {
		return qsf::detail::resources.find_sound(name);
	}
	bool qsf::find_texture(const std::string& name) {
		return qsf::detail::resources.find_texture(name);
	}
	bool qsf::find_sprite(const std::string& name){
		return qsf::detail::resources.find_sprite(name);
	}
}
#endif