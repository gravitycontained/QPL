#pragma once
#ifndef QSF_VIEW_HPP
#define QSF_VIEW_HPP

#include <qpl/defines.hpp>
#if defined QPL_INTERN_SFML_USE

#include <qpl/qpldeclspec.hpp>
#include <qpl/vector.hpp>
#include <qpl/type_traits.hpp>
#include <SFML/Graphics.hpp>

namespace qsf {

	template<bool opengl>
	struct draw_object_t;
	using draw_object = draw_object_t<false>;
	using draw_object_gl = draw_object_t<true>;

	template<typename T>
	struct view_t {
		qpl::vector2<T> position = { T{0}, T{0} };
		qpl::vector2<T> scale = { T{1}, T{1} };
		T rotation = T{ 0 };

		sf::Transform get_transform() const {
			sf::Transform transform;
			transform.rotate(qpl::f32_cast(this->rotation)).scale(1 / qpl::vector2f(this->scale)).translate(-qpl::vector2f(this->position));
			//transform.rotate(qpl::f32_cast(this->rotation)).scale(qpl::vector2f(this->scale)).translate(qpl::vector2f(this->position));
			return transform;
		}
		sf::Transform get_transform_no_offset() const {
			sf::Transform transform;
			transform.rotate(qpl::f32_cast(this->rotation)).scale(1 / qpl::vector2f(this->scale));
			//transform.rotate(qpl::f32_cast(this->rotation)).scale(qpl::vector2f(this->scale));
			return transform;
		}
		sf::RenderStates get_render_states() const {
			sf::RenderStates states;
			states.transform = this->get_transform();
			return states;
		}
		qpl::vec2f transform_point(qpl::vec2f position) const {
			auto transform = this->get_transform().getInverse();
			//auto transform = this->get_transform();
			return transform.transformPoint(position);
		}
		qpl::vec2f transform_point_no_offset(qpl::vec2f position) const {
			auto transform = this->get_transform_no_offset().getInverse();
			//auto transform = this->get_transform_no_offset();
			return transform.transformPoint(position);
		}
		qpl::hitbox transform_hitbox(qpl::hitbox hitbox) const {
			hitbox.position = this->transform_point(hitbox.position);
			hitbox.dimension = this->transform_point_no_offset(hitbox.dimension);
			return hitbox;
		}

		constexpr void reset() {
			this->position = { T{0}, T{0} };
			this->scale = { T{1}, T{1} };
			this->rotation = T{ 0 };
		}
		constexpr void set_rotation(T rotation) {
			this->rotation = rotation;
		}
		constexpr void move(qpl::vector2<T> delta) {
			this->position.move(delta);
		}
		constexpr void set_position(qpl::vector2<T> position) {
			this->position = position;
		}
		constexpr void set_scale(qpl::vector2<T> scale) {
			this->scale = scale;
		}
		constexpr bool is_default_view() const {
			return 
				this->position == qpl::vector2<T>{ T{0}, T{0} } &&
				this->scale == qpl::vector2<T>{ T{1}, T{1} } &&
				this->rotation == T{ 0 };
		}
		constexpr T get_rotation() const {
			return this->rotation;
		}
		constexpr qpl::vector2<T> get_position() const {
			return this->position;
		}
		constexpr qpl::vector2<T> get_scale() const {
			return this->scale;
		}
		//constexpr void apply_to(sf::RenderStates& states) const {
		//	this->apply_to(states.transform);
		//}
		//constexpr void apply_to(sf::Transform& transform) const {
		//	transform.combine(this->get_transform().getInverse());
		//	//transform.combine(this->get_transform());
		//}
		constexpr void apply_to(sf::RenderStates& states) const {
			this->apply_to(states.transform);
		}
		constexpr void apply_to(sf::Transform& transform) const {
			transform.combine(this->get_transform().getInverse());
		}
	};

	using view = qsf::view_t<qpl::f32>;

	template<typename T>
	struct view_extension : T {
		qsf::view view;
	};

	namespace detail {
		template<typename T>
		constexpr auto view_signature(qsf::view_t<T>) {
			return std::true_type{};
		}
		template<typename T>
		constexpr auto view_signature(T) {
			return std::false_type{};
		}
	}
	template<typename T>
	constexpr bool is_view() {
		return decltype(qsf::detail::view_signature(qpl::declval<T>())){};
	}

	template<typename T>
	concept has_view_c = requires (T x) {
		{ x.view };
		requires qsf::is_view<decltype(T::view)>();
	};

	template<typename T>
	constexpr bool has_view() {
		return has_view_c<T>;
	}
}

#endif
#endif