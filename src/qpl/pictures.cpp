#include <qpl/pictures.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/memory.hpp>
#include <qpl/string.hpp>
#include <sstream>

namespace qpl {
	std::string qpl::pixel_rgb::string() const {
		return qpl::to_string("(", qpl::i32_cast(this->r), ", ", qpl::i32_cast(this->g), ", ", qpl::i32_cast(this->b), ")");
	}

	void qpl::pixels::load(std::string_view sv) {
		qpl::string_to_vector_memory(sv, this->data);
	}
	void qpl::pixels::load_bmp(std::string_view sv) {
		qpl::string_to_vector_memory(sv.substr(64), this->data);
	}
	void qpl::pixels::set_dimension(qpl::u32 width, qpl::u32 height) { 
		this->width = width;
		this->height = height;
		this->data.resize(this->width * this->height);
	}
	qpl::pixels qpl::pixels::rectangle(qpl::u32 x, qpl::u32 y, qpl::u32 width, qpl::u32 height) const {
		qpl::pixels result;
		result.set_dimension(width, height);
		
		for (qpl::u32 i = 0u; i < height; ++i) {
			auto index = ((this->height - y - height) + i) * this->width + x;
			memcpy(result.data.data() + (i * width), this->data.data() + index, width * qpl::pixel_rgb::pixel_bytes());
		}

		return result;
	}
	std::string qpl::pixels::generate_bmp_string() const {
		char padding[3] = { 0, 0, 0 };
		int padding_size = (4 - (this->width * qpl::pixel_rgb::pixel_bytes()) % 4) % 4;

		auto file_header = qpl::detail::create_bitmap_file_header(this->width,this->height, padding_size);
		auto info_header = qpl::detail::create_bitmap_info_header(this->width,this->height);

		std::ostringstream stream;
		stream.write(file_header.data(), file_header.size());
		stream.write(info_header.data(), info_header.size());

		for (qpl::u32 i = 0u; i < this->height; ++i) {
			stream.write(reinterpret_cast<const char*>(this->data.data()) + (i * qpl::pixel_rgb::pixel_bytes() * this->width), qpl::pixel_rgb::pixel_bytes() * this->width);
			stream.write(padding, padding_size);
		}
		return stream.str();
	}
	void qpl::pixels::generate_bmp(std::string filename) const {
		auto memory = this->generate_bmp_string();

		FILE* file;
		fopen_s(&file, filename.c_str(), "wb");

		fwrite(memory.c_str(), 1, memory.size(), file);

		fclose(file);
	}
	qpl::size qpl::pixels::size() const {
		return this->data.size();
	}

	std::array<char, qpl::detail::info_header_size> qpl::detail::create_bitmap_info_header(qpl::size width, qpl::size height) {
		static std::array<char, qpl::detail::info_header_size> info_header = {
			0,0,0,0, /// header size
			0,0,0,0, /// image width
			0,0,0,0, /// image height
			0,0, /// number of color planes
			0,0, /// bits per pixel
			0,0,0,0, /// compression
			0,0,0,0, /// image size
			0,0,0,0, /// horizontal resolution
			0,0,0,0, /// vertical resolution
			0,0,0,0, /// colors in color table
			0,0,0,0, /// important color count
		};

		info_header[0] = (char)(info_header_size);
		info_header[4] = (char)(width);
		info_header[5] = (char)(width >> 8);
		info_header[6] = (char)(width >> 16);
		info_header[7] = (char)(width >> 24);
		info_header[8] = (char)(height);
		info_header[9] = (char)(height >> 8);
		info_header[10] = (char)(height >> 16);
		info_header[11] = (char)(height >> 24);
		info_header[12] = (char)(1);
		info_header[14] = (char)(qpl::pixel_rgb::pixel_bytes() * 8);

		return info_header;
	}

	std::array<char, qpl::detail::file_header_size> qpl::detail::create_bitmap_file_header(qpl::size width, qpl::size height, qpl::size padding_size) {
		auto file_size = static_cast<int>(qpl::detail::file_header_size + qpl::detail::info_header_size + (qpl::pixel_rgb::pixel_bytes() * width + padding_size) * height);

		static std::array<char, 14> file_header = {
			0,0, /// signature
			0,0,0,0, /// image file size in bytes
			0,0,0,0, /// reserved
			0,0,0,0, /// start of pixel array
		};

		file_header[0] = (char)('B');
		file_header[1] = (char)('M');
		file_header[2] = (char)(file_size);
		file_header[3] = (char)(file_size >> 8);
		file_header[4] = (char)(file_size >> 16);
		file_header[5] = (char)(file_size >> 24);
		file_header[10] = (char)(file_header_size + info_header_size);

		return file_header;
	}
}