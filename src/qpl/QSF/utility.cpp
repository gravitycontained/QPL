#include <qpl/QSF/utility.hpp>

#if defined QPL_INTERN_SFML_USE

#include <qpl/QSF/framework.hpp>

namespace qsf {
	namespace detail {
		sf::Clipboard clipboard;
	}

	void qsf::run_exception_window(const std::string& exception, std::string font_path, qpl::vector2i dimension) {
		struct exception_state : qsf::base_state {
			void init() override {
				this->text.set_font(*this->font);
				this->text.set_character_size(50);
				this->text.set_string(this->info);

				this->button.set_text_font(*this->font);
				this->button.set_text_string("OK");
				this->button.set_background_color(qpl::rgb(40, 40, 60));
				this->button.set_text_character_size(100);
				this->button.set_dimension({ 150, 150 });

				this->call_on_resize();
			}
			void call_on_resize() override {
				this->text.set_center(this->center() - qpl::vec(0, 200));
				this->button.set_center(this->text.get_visible_hitbox().middle_bottom().moved(0, 200));
				this->button.centerize_text();
			}
			void updating() override {
				this->update(this->button);
				if (this->button.is_clicked()) {
					this->close();
				}
			}
			void drawing() override {
				this->draw(this->text);
				this->draw(this->button);
			}
			std::string info;
			qsf::text text;
			qsf::smooth_button button;
			sf::Font* font = nullptr;
		};

		sf::Font font;
		if (!font.loadFromFile(font_path)) {
			qpl::println("Exception caught: ", exception);
			qpl::println("( tried to run exception window, couldn't load font \"", font_path, "\"");
			qpl::system_pause();
			return;
		}

		qsf::framework framework;
		framework.set_dimension(dimension);

		exception_state state;
		state.info = qpl::to_string("Exception caught:\n", exception);
		state.font = &font;
		framework.set_title(state.info);
		framework.add_state(state);

		framework.game_loop();
	}
}

#endif