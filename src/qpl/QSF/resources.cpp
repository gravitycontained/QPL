#include <qpl/QSF/resources.hpp>
#include <qpl/exception.hpp>

#if defined QPL_INTERN_SFML_USE
#include <qpl/string.hpp>

namespace qsf {

	void qsf::load_font(sf::Font& font, const std::string& path) {
		if (!font.loadFromFile(path)) {
			throw qpl::exception("load font: couldn't find / load \"", path, "\"");
		}
	}
	void qsf::load_texture(sf::Texture& texture, const std::string& path, bool set_smooth) {
		if (!texture.loadFromFile(path)) {
			throw qpl::exception("load texture: couldn't find / load \"", path, "\"");
		}
		texture.setSmooth(set_smooth);
	}
	void qsf::load_image(sf::Image& image, const std::string& path) {
		if (!image.loadFromFile(path)) {
			throw qpl::exception("load image: couldn't find / load \"", path, "\"");
		}
	}
	void qsf::load_sound(sf::SoundBuffer& sound, const std::string& path) {
		if (!sound.loadFromFile(path)) {
			throw qpl::exception("load sound: couldn't find / load \"", path, "\"");
		}
	}
	void qsf::load_shader(sf::Shader& shader, const std::string& path, sf::Shader::Type shader_type) {
		if (!shader.loadFromFile(path, shader_type)) {
			throw qpl::exception("load shader: couldn't find / load \"", path, "\"");
		}
	}
	void qsf::load_shader(sf::Shader& shader, const std::string& path) {
		auto split = qpl::string_split(path, '.');
		if (split.size()) {
			auto back = split.back();
			if (qpl::string_equals_ignore_case(back, "frag")) {
				qsf::load_shader(shader, path, sf::Shader::Type::Fragment);
			}
			else if (qpl::string_equals_ignore_case(back, "fragment")) {
				qsf::load_shader(shader, path, sf::Shader::Type::Fragment);
			}
			else if (qpl::string_equals_ignore_case(back, "vert")) {
				qsf::load_shader(shader, path, sf::Shader::Type::Vertex);
			}
			else if (qpl::string_equals_ignore_case(back, "vertex")) {
				qsf::load_shader(shader, path, sf::Shader::Type::Vertex);
			}
			else if (qpl::string_equals_ignore_case(back, "geom")) {
				qsf::load_shader(shader, path, sf::Shader::Type::Geometry);
			}
			else if (qpl::string_equals_ignore_case(back, "geometry")) {
				qsf::load_shader(shader, path, sf::Shader::Type::Geometry);
			}
			else {
				throw qpl::exception("load shader: can't identify what the shader type for \".", back, "\" is");
			}
		}
	}


	void qsf::load_font_from_memory(sf::Font& font, const std::string& memory) {
		if (!font.loadFromMemory(memory.data(), memory.size())) {
			throw qpl::exception("load font: couldn't load from memory");
		}
	}
	void qsf::load_texture_from_memory(sf::Texture& texture, const std::string& memory, bool set_smooth) {
		if (!texture.loadFromMemory(memory.data(), memory.size())) {
			throw qpl::exception("load texture: couldn't load from memory");
		}
		texture.setSmooth(set_smooth);
	}
	void qsf::load_sound_from_memory(sf::SoundBuffer& sound, const std::string& memory) {
		if (!sound.loadFromMemory(memory.data(), memory.size())) {
			throw qpl::exception("load sound: couldn't load from memory");
		}
	}
	void qsf::load_shader_from_memory(sf::Shader& shader, const std::string& memory, sf::Shader::Type shader_type) {
		if (!shader.loadFromMemory(memory, shader_type)) {
			throw qpl::exception("load shader: couldn't load from memory");
		}
	}

	void qsf::resources::play_sound(const std::string& name, qpl::f32 volume, qpl::f32 speed) {

		if (!this->find_sound(name)) {
			throw qpl::exception(qpl::to_string("play_sound: couldn't find \"", name, "\"").c_str());
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
	void qsf::resources::add_image(const std::string& name, const std::string& path) {
		qsf::load_image(this->images[name], path);
	}
	void qsf::resources::add_shader(const std::string& name, const std::string& path, sf::Shader::Type shader_type) {
		qsf::load_shader(this->shaders[name], path, shader_type);
	}
	void qsf::resources::add_shader(const std::string& name, const std::string& path) {
		qsf::load_shader(this->shaders[name], path);
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


	void qsf::resources::add_font_from_memory(const std::string& name, const std::string& memory) {
		qsf::load_font_from_memory(this->fonts[name], memory);
	}
	void qsf::resources::add_sound_from_memory(const std::string& name, const std::string& memory) {
		qsf::load_sound_from_memory(this->sounds[name], memory);
	}
	void qsf::resources::add_texture_from_memory(const std::string& name, const std::string& memory) {
		qsf::load_texture_from_memory(this->textures[name], memory);
	}
	void qsf::resources::add_shader_from_memory(const std::string& name, const std::string& memory, sf::Shader::Type shader_type) {
		qsf::load_shader_from_memory(this->shaders[name], memory, shader_type);
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
	bool qsf::resources::find_image(const std::string& name) const {
		return this->images.find(name) != this->images.cend();
	}
	bool qsf::resources::find_sprite(const std::string& name) const {
		return this->sprites.find(name) != this->sprites.cend();
	}
	bool qsf::resources::find_shader(const std::string& name) const {
		return this->shaders.find(name) != this->shaders.cend();
	}

	sf::Font& qsf::resources::get_font(const std::string& name) {
		if (this->fonts.find(name) == this->fonts.cend()) {
			throw qpl::exception("couldn't find font with name \"", name, "\"");
		}
		return this->fonts[name];
	}
	sf::SoundBuffer& qsf::resources::get_sound(const std::string& name) {
		if (this->sounds.find(name) == this->sounds.cend()) {
			throw qpl::exception("couldn't find sound with name \"", name, "\"");
		}
		return this->sounds[name];
	}
	sf::Texture& qsf::resources::get_texture(const std::string& name) {
		if (this->textures.find(name) == this->textures.cend()) {
			throw qpl::exception("couldn't find texture with name \"", name, "\"");
		}
		return this->textures[name];
	}
	sf::Image& qsf::resources::get_image(const std::string& name) {
		if (this->images.find(name) == this->images.cend()) {
			throw qpl::exception("couldn't find image with name \"", name, "\"");
		}
		return this->images[name];
	}
	sf::Sprite& qsf::resources::get_sprite(const std::string& name) {
		if (this->sprites.find(name) == this->sprites.cend()) {
			throw qpl::exception("couldn't find sprite with name \"", name, "\"");
		}
		return this->sprites[name];
	}
	sf::Shader& qsf::resources::get_shader(const std::string& name) {
		if (this->shaders.find(name) == this->shaders.cend()) {
			throw qpl::exception("couldn't find shader with name \"", name, "\"");
		}
		return this->shaders[name];
	}

	const sf::Font& qsf::resources::get_font(const std::string& name) const {
		if (this->fonts.find(name) == this->fonts.cend()) {
			throw qpl::exception("couldn't find font with name \"", name, "\"");
		}
		return this->fonts.at(name);
	}
	const sf::SoundBuffer& qsf::resources::get_sound(const std::string& name) const {
		if (this->sounds.find(name) == this->sounds.cend()) {
			throw qpl::exception("couldn't find sound with name \"", name, "\"");
		}
		return this->sounds.at(name);
	}
	const sf::Texture& qsf::resources::get_texture(const std::string& name) const {
		if (this->textures.find(name) == this->textures.cend()) {
			throw qpl::exception("couldn't find texture with name \"", name, "\"");
		}
		return this->textures.at(name);
	}
	const sf::Image& qsf::resources::get_image(const std::string& name) const {
		if (this->images.find(name) == this->images.cend()) {
			throw qpl::exception("couldn't find image with name \"", name, "\"");
		}
		return this->images.at(name);
	}
	const sf::Sprite& qsf::resources::get_sprite(const std::string& name) const {
		if (this->sprites.find(name) == this->sprites.cend()) {
			throw qpl::exception("couldn't find sprite with name \"", name, "\"");
		}
		return this->sprites.at(name);
	}
	const sf::Shader& qsf::resources::get_shader(const std::string& name) const {
		if (this->shaders.find(name) == this->shaders.cend()) {
			throw qpl::exception("couldn't find shader with name \"", name, "\"");
		}
		return this->shaders.at(name);
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
	void qsf::add_image(const std::string& name, const std::string& path) {
		qsf::detail::resources.add_image(name, path);
	}
	void qsf::add_shader(const std::string& name, const std::string& path, sf::Shader::Type shader_type) {
		qsf::detail::resources.add_shader(name, path, shader_type);
	}
	void qsf::add_shader(const std::string& name, const std::string& path) {
		qsf::detail::resources.add_shader(name, path);
	}


	void qsf::add_font_from_memory(const std::string& name, const std::string& memory) {
		qsf::detail::resources.add_font_from_memory(name, memory);
	}
	void qsf::add_sound_from_memory(const std::string& name, const std::string& memory) {
		qsf::detail::resources.add_sound_from_memory(name, memory);
	}
	void qsf::add_texture_from_memory(const std::string& name, const std::string& memory) {
		qsf::detail::resources.add_texture_from_memory(name, memory);
	}
	void qsf::add_shader_from_memory(const std::string& name, const std::string& memory, sf::Shader::Type shader_type) {
		qsf::detail::resources.add_shader_from_memory(name, memory, shader_type);
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
	sf::Image& qsf::get_image(const std::string& name) {
		return qsf::detail::resources.get_image(name);
	}
	sf::Sprite& qsf::get_sprite(const std::string& name) {
		return qsf::detail::resources.get_sprite(name);
	}
	sf::Shader& qsf::get_shader(const std::string& name) {
		return qsf::detail::resources.get_shader(name);
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
	bool qsf::find_image(const std::string& name) {
		return qsf::detail::resources.find_texture(name);
	}
	bool qsf::find_sprite(const std::string& name){
		return qsf::detail::resources.find_sprite(name);
	}
	bool qsf::find_shader(const std::string& name) {
		return qsf::detail::resources.find_shader(name);
	}
}
#endif