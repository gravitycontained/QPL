#ifndef QPLSF_RESOURCES_HPP
#define QPLSF_RESOURCES_HPP
#pragma once

#ifndef QPL_NO_SFML

#include <qpl/qpldeclspec.hpp>
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace qsf {
	struct resources {
		QPLDLL void add_font(const std::string& name, const std::string& path);
		QPLDLL void add_texture(const std::string& name, const std::string& path);
		QPLDLL void add_sprite(const std::string& name, const std::string& path);
		QPLDLL void add_sprite(const std::string& name, sf::Texture& texture);

		QPLDLL bool find_font(const std::string& name) const;
		QPLDLL bool find_texture(const std::string& name) const;
		QPLDLL bool find_sprite(const std::string& name) const;

		QPLDLL sf::Font& get_font(const std::string& name);
		QPLDLL sf::Texture& get_texture(const std::string& name);
		QPLDLL sf::Sprite& get_sprite(const std::string& name);

		QPLDLL const sf::Font& get_font(const std::string& name) const;
		QPLDLL const sf::Texture& get_texture(const std::string& name) const;
		QPLDLL const sf::Sprite& get_sprite(const std::string& name) const;

		std::unordered_map<std::string, sf::Font> fonts;
		std::unordered_map<std::string, sf::Texture> textures;
		std::unordered_map<std::string, sf::Sprite> sprites;
	};



	namespace detail {
		QPLDLL extern qsf::resources resources;
	}

	QPLDLL void load_font(sf::Font& Font, const std::string& path);
	QPLDLL void load_texture(sf::Texture& texture, const std::string& path);
	QPLDLL void load_sound(sf::SoundBuffer& sound, const std::string& path);

	QPLDLL void add_font(const std::string& name, const std::string& path);
	QPLDLL void add_texture(const std::string& name, const std::string& path);
	QPLDLL void add_sprite(const std::string& name, const std::string& path);
	QPLDLL void add_sprite(const std::string& name, sf::Texture& texture);

	QPLDLL sf::Font& get_font(const std::string& name);
	QPLDLL sf::Texture& get_texture(const std::string& name);
	QPLDLL sf::Sprite& get_sprite(const std::string& name);

	QPLDLL bool find_font(const std::string& name);
	QPLDLL bool find_texture(const std::string& name);
	QPLDLL bool find_sprite(const std::string& name);
};

#endif
#endif