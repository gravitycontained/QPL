#include <qpl/QGL/vertex_array.hpp>

#ifdef QPL_INTERN_GLEW_USE
namespace qgl {
	void qgl::detail::va_tex::set_texture(const sf::Texture& texture) {
		this->texture = &texture;
	}
	void qgl::detail::va_tex::bind() const {
		if (this->texture == nullptr) {
			throw qpl::exception("qgl::vertex_array : texture not set");
		}
		sf::Texture::bind(this->texture);
	}
	void qgl::detail::va_tex::unbind() const {
		sf::Texture::bind(nullptr);
	}


	void qgl::detail::va_shader::set_vertex_shader(const std::string& vertex) {
		this->shader.set_vertex(vertex);
	}
	void qgl::detail::va_shader::set_fragment_shader(const std::string& fragment) {
		this->shader.set_fragment(fragment);
	}
	void qgl::detail::va_shader::set_geometry_shader(const std::string& geometry) {
		this->shader.set_fragment(geometry);
	}

	const std::string& qgl::detail::va_shader::get_vertex_shader() const {
		return this->shader.get_vertex_shader();
	}
	const std::string& qgl::detail::va_shader::get_fragment_shader() const {
		return this->shader.get_fragment_shader();
	}
	const std::string& qgl::detail::va_shader::get_geometry_shader() const {
		return this->shader.get_geometry_shader();
	}


	void qgl::detail::va_dynamic_primitive_type::set_primitive_type(qgl::primitive_type primitive_type) {
		this->dynamic_primitive_type = primitive_type;
	}
	qgl::primitive_type qgl::detail::va_dynamic_primitive_type::get_primitive_type() const {
		return this->dynamic_primitive_type;
	}
}
#endif