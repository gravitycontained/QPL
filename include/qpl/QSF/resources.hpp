#ifndef QSF_RESOURCES_HPP
#define QSF_RESOURCES_HPP
#pragma once

#include <qpl/defines.hpp>
#if defined QPL_INTERN_SFML_USE

#include <qpl/qpldeclspec.hpp>
#include <qpl/vardef.hpp>
#include <unordered_map>
#include <string>
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace qsf {

	QPLDLL void load_font(sf::Font& Font, const std::string& path);
	QPLDLL void load_texture(sf::Texture& texture, const std::string& path, bool set_smooth = false);
	QPLDLL void load_image(sf::Image& image, const std::string& path);
	QPLDLL void load_sound(sf::SoundBuffer& sound, const std::string& path);
	QPLDLL void load_shader(sf::Shader& shader, const std::string& path, sf::Shader::Type shader_type);
	QPLDLL void load_shader(sf::Shader& shader, const std::string& path);

	QPLDLL void load_font_from_memory(sf::Font& Font, const std::string& memory);
	QPLDLL void load_texture_from_memory(sf::Texture& texture, const std::string& memory, bool set_smooth = false);
	QPLDLL void load_sound_from_memory(sf::SoundBuffer& sound, const std::string& memory);
	QPLDLL void load_shader_from_memory(sf::Shader& shader, const std::string& memory, sf::Shader::Type shader_type);


	struct resources {
		QPLDLL void play_sound(const std::string& name, qpl::f32 volume = 100.0f, qpl::f32 speed = 1.0f);
		QPLDLL void update_sounds();

		QPLDLL void add_font(const std::string& name, const std::string& path);
		QPLDLL void add_sound(const std::string& name, const std::string& path);
		QPLDLL void add_texture(const std::string& name, const std::string& path);
		QPLDLL void add_image(const std::string& name, const std::string& path);
		QPLDLL void add_sprite(const std::string& name, const std::string& path);
		QPLDLL void add_sprite(const std::string& name, sf::Texture& texture);
		QPLDLL void add_shader(const std::string& name, const std::string& path, sf::Shader::Type shader_type);
		QPLDLL void add_shader(const std::string& name, const std::string& path);

		QPLDLL void add_font_from_memory(const std::string& name, const std::string& memory);
		QPLDLL void add_sound_from_memory(const std::string& name, const std::string& memory);
		QPLDLL void add_texture_from_memory(const std::string& name, const std::string& memory);
		QPLDLL void add_shader_from_memory(const std::string& name, const std::string& memory, sf::Shader::Type shader_type);

		QPLDLL bool find_font(const std::string& name) const;
		QPLDLL bool find_sound(const std::string& name) const;
		QPLDLL bool find_texture(const std::string& name) const;
		QPLDLL bool find_sprite(const std::string& name) const;
		QPLDLL bool find_shader(const std::string& name) const;
		QPLDLL bool find_image(const std::string& name) const;

		QPLDLL sf::Font& get_font(const std::string& name);
		QPLDLL sf::SoundBuffer& get_sound(const std::string& name);
		QPLDLL sf::Texture& get_texture(const std::string& name);
		QPLDLL sf::Sprite& get_sprite(const std::string& name);
		QPLDLL sf::Shader& get_shader(const std::string& name);
		QPLDLL sf::Image& get_image(const std::string& name);

		QPLDLL const sf::Font& get_font(const std::string& name) const;
		QPLDLL const sf::SoundBuffer& get_sound(const std::string& name) const;
		QPLDLL const sf::Texture& get_texture(const std::string& name) const;
		QPLDLL const sf::Sprite& get_sprite(const std::string& name) const;
		QPLDLL const sf::Shader& get_shader(const std::string& name) const;
		QPLDLL const sf::Image& get_image(const std::string& name) const;

		std::unordered_map<std::string, sf::Font> fonts;
		std::unordered_map<std::string, sf::SoundBuffer> sounds;
		std::unordered_map<std::string, sf::Texture> textures;
		std::unordered_map<std::string, sf::Sprite> sprites;
		std::unordered_map<std::string, sf::Shader> shaders;
		std::unordered_map<std::string, sf::Image> images;
		std::list<sf::Sound> active_sounds;
	};



	namespace detail {
		QPLDLL extern qsf::resources resources;
	}
	QPLDLL void play_sound(const std::string& name, qpl::f32 volume = 100.0f, qpl::f32 speed = 1.0f);
	QPLDLL void update_sounds();

	QPLDLL void add_font(const std::string& name, const std::string& path);
	QPLDLL void add_sound(const std::string& name, const std::string& path);
	QPLDLL void add_texture(const std::string& name, const std::string& path);
	QPLDLL void add_image(const std::string& name, const std::string& path);
	QPLDLL void add_sprite(const std::string& name, const std::string& path);
	QPLDLL void add_sprite(const std::string& name, sf::Texture& texture);
	QPLDLL void add_shader(const std::string& name, const std::string& path, sf::Shader::Type shader_type);
	QPLDLL void add_shader(const std::string& name, const std::string& path);

	QPLDLL void add_font_from_memory(const std::string& name, const std::string& memory);
	QPLDLL void add_sound_from_memory(const std::string& name, const std::string& memory);
	QPLDLL void add_texture_from_memory(const std::string& name, const std::string& memory);
	QPLDLL void add_shader_from_memory(const std::string& name, const std::string& memory, sf::Shader::Type shader_type);

	QPLDLL sf::Font& get_font(const std::string& name);
	QPLDLL sf::SoundBuffer& get_sound(const std::string& name);
	QPLDLL sf::Texture& get_texture(const std::string& name);
	QPLDLL sf::Image& get_image(const std::string& name);
	QPLDLL sf::Sprite& get_sprite(const std::string& name);
	QPLDLL sf::Shader& get_shader(const std::string& name);

	QPLDLL bool find_font(const std::string& name);
	QPLDLL bool find_sound(const std::string& name);
	QPLDLL bool find_texture(const std::string& name);
	QPLDLL bool find_image(const std::string& name);
	QPLDLL bool find_sprite(const std::string& name);
	QPLDLL bool find_shader(const std::string& name);
};

#endif
#endif