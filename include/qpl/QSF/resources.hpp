#ifndef QPLSF_RESOURCES_HPP
#define QPLSF_RESOURCES_HPP
#pragma once

#if !defined (QPL_NO_SFML) || defined(QPL_USE_ALL)

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
	QPLDLL void load_sound(sf::SoundBuffer& sound, const std::string& path);
	QPLDLL void load_shader(sf::Shader& shader, const std::string& path, sf::Shader::Type shader_type);
	QPLDLL void load_shader(sf::Shader& shader, const std::string& path);


	struct resources {
		QPLDLL void play_sound(const std::string& name, qpl::f32 volume = 100.0f, qpl::f32 speed = 1.0f);
		QPLDLL void update_sounds();

		QPLDLL void add_font(const std::string& name, const std::string& path);
		QPLDLL void add_sound(const std::string& name, const std::string& path);
		QPLDLL void add_texture(const std::string& name, const std::string& path);
		QPLDLL void add_sprite(const std::string& name, const std::string& path);
		QPLDLL void add_sprite(const std::string& name, sf::Texture& texture);
		QPLDLL void add_shader(const std::string& name, const std::string& path, sf::Shader::Type shader_type);
		QPLDLL void add_shader(const std::string& name, const std::string& path);

		QPLDLL bool find_font(const std::string& name) const;
		QPLDLL bool find_sound(const std::string& name) const;
		QPLDLL bool find_texture(const std::string& name) const;
		QPLDLL bool find_sprite(const std::string& name) const;
		QPLDLL bool find_shader(const std::string& name) const;

		QPLDLL sf::Font& get_font(const std::string& name);
		QPLDLL sf::SoundBuffer& get_sound(const std::string& name);
		QPLDLL sf::Texture& get_texture(const std::string& name);
		QPLDLL sf::Sprite& get_sprite(const std::string& name);
		QPLDLL sf::Shader& get_shader(const std::string& name);

		QPLDLL const sf::Font& get_font(const std::string& name) const;
		QPLDLL const sf::SoundBuffer& get_sound(const std::string& name) const;
		QPLDLL const sf::Texture& get_texture(const std::string& name) const;
		QPLDLL const sf::Sprite& get_sprite(const std::string& name) const;
		QPLDLL const sf::Shader& get_shader(const std::string& name) const;

		std::unordered_map<std::string, sf::Font> fonts;
		std::unordered_map<std::string, sf::SoundBuffer> sounds;
		std::unordered_map<std::string, sf::Texture> textures;
		std::unordered_map<std::string, sf::Sprite> sprites;
		std::unordered_map<std::string, sf::Shader> shaders;
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
	QPLDLL void add_sprite(const std::string& name, const std::string& path);
	QPLDLL void add_sprite(const std::string& name, sf::Texture& texture);
	QPLDLL void add_shader(const std::string& name, const std::string& path, sf::Shader::Type shader_type);
	QPLDLL void add_shader(const std::string& name, const std::string& path);

	QPLDLL sf::Font& get_font(const std::string& name);
	QPLDLL sf::SoundBuffer& get_sound(const std::string& name);
	QPLDLL sf::Texture& get_texture(const std::string& name);
	QPLDLL sf::Sprite& get_sprite(const std::string& name);
	QPLDLL sf::Shader& get_shader(const std::string& name);

	QPLDLL bool find_font(const std::string& name);
	QPLDLL bool find_sound(const std::string& name);
	QPLDLL bool find_texture(const std::string& name);
	QPLDLL bool find_sprite(const std::string& name);
	QPLDLL bool find_shader(const std::string& name);
};

#endif
#endif